// Student 1 — Datapath Registers (mult_regs)
// Holds M[7:0], Q[7:0], A[8:0]. Control: load, add_en, shift.
// On add_en: A ← sum (from add_decision). On shift: {A,Q} ← {A,Q} >> 1.

module mult_regs (
	input clk, rst,
	input load, add_en, shift,
	input [7:0] a_in, b_in,
	input [8:0] sum,           // From add_decision module
	output reg [8:0] A,
	output reg [7:0] M, Q
);

	always @(posedge clk) begin
		if(rst)
			begin
				A<=9'b0;
				M<=8'b0;
				Q<=8'b0;
			end
		else if(load)
			begin
				M<=a_in;
				Q<=b_in;
				A<=9'b0;
			end
		else if(add_en)
			begin
				A<=sum;
			end
		else if(shift)
			begin
				{A,Q}<={A,Q}>>1;
			end
	end
	// TODO: Implement the datapath register logic
	// - On reset: A, M, Q should be cleared
	// - On load: M ← a_in, Q ← b_in, A ← 0
	// - On add_en: A ← sum  (use the pre-computed sum from add_decision)
	// - On shift: {A, Q} ← {A, Q} >> 1
	//
	// Note: This time, you receive 'sum' as an input from add_decision module
	// You don't compute A+M here; you just latch the pre-computed sum!

endmodule
