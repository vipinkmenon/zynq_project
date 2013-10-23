module mulitiplier(
input             i_clk,
input             i_rst_n,
input [31:0]      i_data,
input             i_data_valid,
output            o_data_ready,
output reg[31:0]  o_data,
output reg        o_data_valid,
input             i_data_ready
);

assign o_data_ready   =   i_data_ready;


always @(posedge i_clk)
begin
    if(!i_rst_n)
        o_data_valid   <=  1'b0;
    else
    begin
        o_data        <=    i_data[15:0]*i_data[31:0];
        o_data_valid  <=    i_data_valid;
	end  
end


endmodule
