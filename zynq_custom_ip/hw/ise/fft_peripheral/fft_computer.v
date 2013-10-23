module fft_computer(
input        i_clk,
input        i_rst_n,
input        i_data_valid,
input [31:0] i_data,
output       o_data_ready,
output       o_data_valid,
output [31:0]o_data,
input        i_data_ready
); 

wire [31:0] fft_data;
wire        fft_data_valid;
wire        fft_data_ready;
wire [31:0] mult_data;
wire        mult_data_valid;
wire        mult_data_ready;

fft_core fft (
  .aclk(i_clk), // input aclk
  .aresetn(i_rst_n), // input aclken
  .s_axis_config_tdata(0), 
  .s_axis_config_tvalid(1'b0), 
  .s_axis_config_tready(), 
  .s_axis_data_tdata(i_data), 
  .s_axis_data_tvalid(i_data_valid), 
  .s_axis_data_tready(o_data_ready), 
  .s_axis_data_tlast(1'b0),
  .m_axis_data_tdata(fft_data), 
  .m_axis_data_tvalid(fft_data_valid),
  .m_axis_data_tready(fft_data_ready),
  .m_axis_data_tlast(),
  .event_frame_started(), 
  .event_tlast_unexpected(), 
  .event_tlast_missing(),
  .event_status_channel_halt(), 
  .event_data_in_channel_halt(),
  .event_data_out_channel_halt()
);


mulitiplier mult (
    .i_clk(i_clk), 
    .i_rst_n(i_rst_n), 
    .i_data(fft_data), 
    .i_data_valid(fft_data_valid), 
    .o_data_ready(fft_data_ready), 
    .o_data(mult_data), 
    .o_data_valid(mult_data_valid), 
    .i_data_ready(mult_data_ready)
);

output_ctrl ctrl (
    .i_clk(i_clk), 
    .i_rst_n(i_rst_n), 
    .i_data(mult_data), 
    .i_data_valid(mult_data_valid), 
    .o_data_ready(mult_data_ready), 
    .o_data(o_data), 
    .o_data_valid(o_data_valid), 
    .i_data_ready(i_data_ready)
);


endmodule

