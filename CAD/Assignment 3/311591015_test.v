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
	// reset =1'b1;

// #12 in    =8'b01101111;
	reset =1'b0;
     
//write your test pattern----------------
	repeat(2) @(negedge clk);
	reset = 1'b1;
	repeat(2) @(negedge clk);
	reset = 1'b0;
	repeat(2) @(negedge clk);
	
	// input A = 8'b1111_1111
	in    =8'b0110_0000;	
	@(negedge clk);
	in    =8'b1111_1111;
	@(negedge clk);
	
	// input A = 'd0
	in    =8'b0110_0000;	
	@(negedge clk);
	in    =8'b0000_0000;
	@(negedge clk);

	// input B = 'd0
	in    =8'b0111_0000;	
	@(negedge clk);
	in    =8'b0000_0000;
	@(negedge clk);
	
	// input B = 8'b1111_1111
	in    =8'b0111_0000;	
	@(negedge clk);
	in    =8'b1111_1111;
	@(negedge clk);
	
	// input B = 'd0
	in    =8'b0111_0000;	
	@(negedge clk);
	in    =8'b0000_0000;
	@(negedge clk);
	
	// C = A + B
	in    =8'b0000_0000;	
	@(negedge clk);
	
	// storage C at address i
	for(i = 0; i < 16; i = i + 1) begin
		in    ={4'b1001, i[3:0]};	
		@(negedge clk);
	end
	
	// C = A - B
	in    =8'b0001_0000;	
	@(negedge clk);
	
	// C = A - B
	in    =8'b0001_0000;	
	@(negedge clk);
	
	// C = A + 1
	for(i = 0; i < 1024; i = i + 1) begin
		in    =8'b0010_0000;	
		@(negedge clk);
	end
	
	// C = A - 1
	in    =8'b0011_0000;	
	@(negedge clk);
	
	// C = A + B + 1
	in    =8'b0100_0000;	
	@(negedge clk);
	
	// C = -A
	in    =8'b0101_0000;	
	@(negedge clk);
	
	// output C
	in    =8'b1011_0000;	
	@(negedge clk);
	
	// A = C
	in    =8'b1101_0000;	
	@(negedge clk);
	
	// B = C
	in    =8'b1110_0000;	
	@(negedge clk);
	
	// input 8'b1111_1111 into memory at address i
	for(i = 0; i < 16; i = i + 1) begin
		in    ={4'b1000, i[3:0]};
		@(negedge clk);
		in    =8'b1111_1111;	
		@(negedge clk);
	end
	
	// load memory from address i
	for(i = 0; i < 16; i = i + 1) begin
		in    ={4'b1010, i[3:0]};
		repeat(2) @(negedge clk);
	end
	
	// output memory from address i
	for(i = 0; i < 16; i = i + 1) begin
		in    ={4'b1100, i[3:0]};
		repeat(2) @(negedge clk);
	end
	
	// input 8'b0000_0000 into memory at address i
	for(i = 0; i < 16; i = i + 1) begin
		in    ={4'b1000, i[3:0]};
		@(negedge clk);
		in    =8'b0000_0000;	
		@(negedge clk);
	end
	
	// load memory from address 'b0001
	for(i = 0; i < 16; i = i + 1) begin
		in    ={4'b1010, i[3:0]};
		repeat(2) @(negedge clk);
	end
	
	// output memory from address i
	for(i = 0; i < 16; i = i + 1) begin
		in    ={4'b1100, i[3:0]};
		repeat(2) @(negedge clk);
	end
	
	// input 8'b1111_1111 into memory at address i
	for(i = 0; i < 16; i = i + 1) begin
		in    ={4'b1000, i[3:0]};
		@(negedge clk);
		in    =8'b1111_1111;	
		@(negedge clk);
	end
	
	// load memory from address i
	for(i = 0; i < 16; i = i + 1) begin
		in    ={4'b1010, i[3:0]};
		repeat(2) @(negedge clk);
	end
	
	// output memory from address i
	for(i = 0; i < 16; i = i + 1) begin
		in    ={4'b1100, i[3:0]};
		repeat(2) @(negedge clk);
	end
	
	// load C then reset
	in    ={4'b1010, 4'b1010};
	repeat(2) @(negedge clk);
	reset = 1'b1;
	in    =8'b1111_0001;
	@(negedge clk);
	reset = 1'b0;
	
	// output memory then reset
	in    ={4'b1100, 4'b1010};
	repeat(2) @(negedge clk);
	reset = 1'b1;
	in    =8'b1111_0001;
	@(negedge clk);
	reset = 1'b0;
	
	// reserved
	in    =8'b1111_0001;
	repeat(2) @(negedge clk);

	repeat(2) @(negedge clk);
//----------------------------------------
#10  $finish;

end
endmodule