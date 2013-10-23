/*
 * fft_test.c
 *
 *  Created on: Oct 17, 2013
 *      Author: vipin2
 */
#include "xparameters.h"
#include <stdio.h>
#include "xil_io.h"
#include "xdmaps.h"
#include "xtime_l.h"
#include "xil_cache.h"


XDmaPs DmaInstance;

#define DMA_DEVICE_ID 			XPAR_XDMAPS_1_DEVICE_ID
#define TIMEOUT_LIMIT 	0x2000	/* Loop count for timeout */
#define timer_base 0xF8F00000

int init_DMA (u16 DeviceId, XDmaPs *DmaInst);
int load_Data_dma(u32 src_addr, u32 dst_addr, u32 len, XDmaPs *DmaInst);

/***********************************************************
Timer Registers
************************************************************/
static volatile int *timer_counter_l=(volatile int *)(timer_base+0x200);
static volatile int *timer_counter_h=(volatile int *)(timer_base+0x204);
static volatile int *timer_ctrl=(volatile int *)(timer_base+0x208);
/***********************************************************
/***********************************************************
Function definitions
************************************************************/
void init_timer(volatile int *timer_ctrl, volatile int *timer_counter_l, volatile int *timer_counter_h){
        *timer_ctrl=0x0;
        *timer_counter_l=0x1;
        *timer_counter_h=0x0;
        DATA_SYNC;
}

void start_timer(volatile int *timer_ctrl){
        *timer_ctrl=*timer_ctrl | 0x00000001;
        DATA_SYNC;
}

void stop_timer(volatile int *timer_ctrl){
        *timer_ctrl=*timer_ctrl & 0xFFFFFFFE;
        DATA_SYNC;
}


int main(){
	u32 i;
	u32 ret[1025];
	u32 rat[1025];
	int Status;
	int *mem = XPAR_PS7_DDR_0_S_AXI_BASEADDR + 0x20000;
	XDmaPs *DmaInst = &DmaInstance;
	//Store some incremental pattern in the DRAM memory
	for(i=0;i<512;i++)
	{
		*mem = i;
		 mem++;
	}
	//Flush the cache to make sure that data has reached DRAM
	Xil_DCacheFlush();

	print("Writing data to the core \n\r");

	//Reinitialise the memory address to where data has been stored
	mem = XPAR_PS7_DDR_0_S_AXI_BASEADDR + 0x20000;
	//Initialise the timer for performance monitoring
	init_timer(timer_ctrl, timer_counter_l, timer_counter_h);
	start_timer(timer_ctrl);
	//Write 2 frames (256*2) into the fft core from the memory in PIO manner
	for(i=0;i<512;i++)
	{
		Xil_Out32(XPAR_FFT_PERIPHERAL_0_S_AXI_MEM0_BASEADDR,*mem);
		mem++;
	}
	stop_timer(timer_ctrl);
	//Calculate the time for the operation
	xil_printf("Communication time %d us\n\r", (*timer_counter_l)/333);
	print("Reading data from the core \n\r");
	//Read the output from the core one by one and store in an array and later print it
	for(i=0;i<256;i++)
		ret[i] = Xil_In32(XPAR_FFT_PERIPHERAL_0_S_AXI_MEM0_BASEADDR);
	for(i=0;i<256;i++)
		xil_printf("Data %d is %0x\n\r",i,ret[i]);

	print("Starting using DMA\n\r");
	//Initialise the PS DMA controller
	Status = init_DMA (DMA_DEVICE_ID,DmaInst);
    if(Status != XST_SUCCESS)
    	xil_printf("DMA initialisation failed\n\r",Status);
    else
    	print("DMA controller initialised\n\r");
	//Initialise the timer for profiling
	init_timer(timer_ctrl, timer_counter_l, timer_counter_h);
	start_timer(timer_ctrl);
	//Call the function to send data from DRAM to the fft core using the DMA controller with the source, destination, transfersize and DMA controller instance pointer
	Status = load_Data_dma(XPAR_PS7_DDR_0_S_AXI_BASEADDR + 0x20000, XPAR_FFT_PERIPHERAL_0_S_AXI_MEM0_BASEADDR, 512*4, DmaInst);
	stop_timer(timer_ctrl);
	xil_printf("Communication time using DMA is %d us--\n\r", (*timer_counter_l)/333);
    if(Status != XST_SUCCESS)
    	xil_printf("DMA failed with data %0x\n\r",Status);
    else
    	print("DMA passed\n\r");
    //Read the data from the core one by one and print it
	print("Reading data from the core \n\r");
    for(i=0;i<256;i++){
    		rat[i] = Xil_In32(XPAR_FFT_PERIPHERAL_0_S_AXI_MEM0_BASEADDR);
    }
    for(i=0;i<256;i++)
    		xil_printf("Data %d is %0x\n\r",i,rat[i]);
	return 0;
}



/*****************************************************************************/
/*
 * DMA controller initialisation.
 * This is the standard method for initialising the PS DMA controller
 ****************************************************************************/
int init_DMA (u16 DeviceId, XDmaPs *DmaInst)
{
	int Status;
	XDmaPs_Config *DmaCfg;
	/*
	 * Initialize the DMA Driver
	 */
	DmaCfg = XDmaPs_LookupConfig(DeviceId);
	if (DmaCfg == NULL) {
		return XST_FAILURE;
	}

	Status = XDmaPs_CfgInitialize(DmaInst,
				   DmaCfg,
				   DmaCfg->BaseAddress);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
	return XST_SUCCESS;
}


/**********************************************************************
 * Function to send data using the DMA controller
 * inputs  : source address (src_addr), destination address (dst_addr),
 * transfer size in bytes (len), and the DMA controller instance pointer
 * (DmsInst)
 * output  : Transfer status
 *********************************************************************/
int load_Data_dma(u32 src_addr, u32 dst_addr, u32 len, XDmaPs *DmaInst)
{
	unsigned int Channel = 0;
	int Status;

	XDmaPs_Cmd DmaCmd;
	memset(&DmaCmd, 0, sizeof(XDmaPs_Cmd));

	//The DMA command structure
	DmaCmd.ChanCtrl.SrcBurstSize = 4;   //Source burst size. 32-bits = 4 bytes
	DmaCmd.ChanCtrl.SrcBurstLen = 1;    //Source burst length. set to 1
	DmaCmd.ChanCtrl.SrcInc = 1;         //Whether source address should be incremented for each data. Since reading from memory, set to 1
	DmaCmd.ChanCtrl.DstBurstSize = 4;   //Destination burst size
	DmaCmd.ChanCtrl.DstBurstLen = 1;    //Destination burst length
	DmaCmd.ChanCtrl.DstInc = 0;         //Since data is finally going to a stream interface, it doesn't matter whether address increments or not
	DmaCmd.BD.SrcAddr = src_addr;       //Source start address for DMA
	DmaCmd.BD.DstAddr = dst_addr;       //Destination start address for DMA
	DmaCmd.BD.Length = len;             //DMA length in bytes

	//Clear any pending interrupt in the DMA controller
	Xil_Out32(0xF800302C,0xff);
    //Start the DMA controller
	Status = XDmaPs_Start(DmaInst, Channel, &DmaCmd, 0);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
    //Poll the interrupt status register to check whether DMA is over
	while(Xil_In32(0xF8003028) == 0x0)
	{

	}

	return XST_SUCCESS;
}
