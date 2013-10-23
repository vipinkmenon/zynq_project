TABLE OF CONTENTS
  1) Peripheral Summary
  2) Description of Generated Files
  3) Location to documentation of dependent libraries
================================================================================
*                             1) Peripheral Summary                            *
================================================================================
Peripheral Summary:

  XPS project / EDK repository               : D:\ug_labs\zynq_custom_ip\edk
  logical library name                       : fft_peripheral_v1_00_a
  top name                                   : fft_peripheral
  version                                    : 1.00.a
  type                                       : AXI4 slave
  features                                   : slave attachment
                                               user address ranges

Address Block for User Logic and IPIF Predefined Services

  User logic memory space 0                  : C_S_AXI_MEM0_BASEADDR
                                             : C_S_AXI_MEM0_HIGHADDR


================================================================================
*                          2) Description of Generated Files                   *
================================================================================
- HDL source file(s)

  hdl/vhdl/fft_peripheral.vhd

    This is the template file for your peripheral's top design entity. It
    configures and instantiates the corresponding design units in the way you
    indicated in the wizard GUI and hooks it up to the stub user logic where
    the actual functionalites should get implemented. You are not expected to
    modify this template file except certain marked places for adding user
    specific generics and ports.

  verilog/user_logic.v

    This is the template file for the stub user logic design entity, either in
    VHDL or Verilog, where the actual functionalities should get implemented.
    Some sample code snippet may be provided for demonstration purpose.

- XPS interface file(s)

  data/fft_peripheral_v2_1_0.mpd

    This Microprocessor Peripheral Description file contains information of the
    interface of your peripheral, so that other EDK tools can recognize your
    peripheral.

  data/fft_peripheral_v2_1_0.pao

    This Peripheral Analysis Order file defines the analysis order of all the HDL
    source files that are used to compile your peripheral.

- Other misc file(s)

  devl/ipwiz.opt

    This is the option setting file for the wizard batch mode, which should
    generate the same result as the wizard GUI mode.

  devl/README.txt

    This README file for your peripheral.

  devl/ipwiz.log

    This is the log file by operating on this wizard.


================================================================================
*          3) Location to documentation of dependent libraries                 *
*                                                                              *
*   In general, the documentation is located under:                            *
*   $XILINX_EDK/hw/XilinxProcessorIPLib/pcores/$libName/doc                    *
*                                                                              *
================================================================================
proc_common_v3_00_a
	No documentation for this library

axi_slave_burst_v1_00_a
	D:\ug_labs\zynq_custom_ip\edk\D:\Xilinx\14.4\ISE_DS\EDK\hw\XilinxProcessorIPLib\pcores\axi_slave_burst_v1_00_a\doc\ds769_axi_slave_burst.pdf

