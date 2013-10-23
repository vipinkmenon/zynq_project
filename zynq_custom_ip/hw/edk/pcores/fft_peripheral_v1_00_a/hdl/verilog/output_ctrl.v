module output_ctrl(
input        i_clk,
input        i_rst_n,
input [31:0] i_data,
input        i_data_valid,
output       o_data_ready,
output [31:0]o_data,
output  reg  o_data_valid,
input        i_data_ready
);



reg [7:0]   sample_cnt;
reg [7:0]   read_cnt;
wire        valid_input_data;
wire        valid_output_data;
wire        fifo_rd_en;
reg  [31:0] fifo_wr_data;
wire [31:0] fifo_rd_data;
reg         window_cnt;
reg         fifo_wr_en;


assign valid_input_data  = i_data_valid & o_data_ready;
assign valid_output_data = o_data_valid & i_data_ready;
assign fifo_rd_en        = (window_cnt == 0) ? valid_output_data : valid_input_data;
assign o_data            = fifo_rd_data;

always @(posedge i_clk)
begin
    if(!i_rst_n)
	 begin
	     sample_cnt  <=   0;
	 end
	 else
	 begin
       if(valid_input_data)
	         sample_cnt   <=   sample_cnt + 1'b1;
	 end			
end

always @(posedge i_clk)
begin
    if(!i_rst_n)
	 begin
	     read_cnt  <=   0;
	 end
	 else
	 begin
       if(valid_output_data)
	         read_cnt   <=   read_cnt + 1'b1;
	 end			
end


always @(posedge i_clk)
begin
    if(!i_rst_n)
	     window_cnt   <=   0;
	 else
	 begin
        if((sample_cnt == 255) & valid_input_data)
	         window_cnt   <=   window_cnt + 1;
	 end			
end

always @(posedge i_clk)
begin
    if(!i_rst_n)
	     o_data_valid   <=   0;
	 else
	 begin
        if((read_cnt == 255) & valid_output_data)
	         o_data_valid    <=    1'b0;
        else if((window_cnt==1) & (sample_cnt == 255) & valid_input_data)
	         o_data_valid    <=    1'b1;
	 end			
end

always @(posedge i_clk)
begin
    if(valid_input_data)
	 begin
	     fifo_wr_en    <=    1'b1;
	     if(window_cnt == 0)
		      fifo_wr_data <= i_data;
		  else
            fifo_wr_data <= i_data + fifo_rd_data;		  
	 end
	 else
	     fifo_wr_en    <=    1'b0;
end


output_fifo fifo (
  .s_aclk(i_clk), // input s_aclk
  .s_aresetn(i_rst_n), // input s_aresetn
  .s_axis_tvalid(fifo_wr_en), // input s_axis_tvalid
  .s_axis_tready(o_data_ready), // output s_axis_tready
  .s_axis_tdata(fifo_wr_data), // input [31 : 0] s_axis_tdata
  .m_axis_tvalid(),           // output m_axis_tvalid
  .m_axis_tready(fifo_rd_en), // input m_axis_tready
  .m_axis_tdata(fifo_rd_data) // output [31 : 0] m_axis_tdata
);


endmodule
