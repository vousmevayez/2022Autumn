module test;

reg[7:0] in;
reg clk,reset;
wire[7:0] out;
integer i;

initial begin
	clk=0;
	forever
    #5 clk = !clk;
end

initial begin
    $dumpfile("cpu.vcd");
    $dumpvars;	
end

cpu cc(.in(in), .clk(clk), .reset(reset), .out(out));

initial begin
	in    =8'b00000000;
	reset =1'b1;

#12 in    =8'b01101111;
	reset =1'b0;
     
//write your test pattern----------------
#10 in = $random();

for (i = 0; i < 19; i=i+1) begin
	#10 in = $random()%256;
end 

//----------------------------------------
#10  $finish;

end
endmodule