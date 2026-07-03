// Student 5 — Top-Level Sequential Multiplier (seq_multiplier)
// Wire all 5 modules: mult_regs, bit_counter, add_decision, controller, testbench.

module seq_multiplier (
	input clk, rst, start,
	input [7:0] a, b,
	output [15:0] product,
	output done
);

	// TODO: Instantiate all 5 modules and wire them together
	//
	wire [8:0] A, sum;
      	wire [7:0] M, Q;                                                                                   
      	wire [3:0] count;                                                                                  
      	wire last, do_add;
      	wire load, add_en, shift, count_en, clear;
	
// Modules to instantiate:

// U1: mult_regs (datapath registers)
	 mult_regs U1 (
          .clk(clk), .rst(rst),                                                                          
          .load(load), .add_en(add_en), .shift(shift),            
          .a_in(a), .b_in(b),
          .sum(sum),                                                                                     
          .A(A), .M(M), .Q(Q)
     	 );
    	
// U2: bit_counter (counter)
	bit_counter U2 (
	.clk(clk), .rst(rst), .clear(clear), .count_en(count_en),
	.count(count), .last(last)
	);

// U3: add_decision (combinational adder - NEW!)
	add_decision U3 (.A(A), .M(M), .Q_lsb(Q[0]),
        .sum(sum), .do_add(do_add)
	);
// U4: controller (FSM)
	controller U4 (.clk(clk), .rst(rst),
        .start(start), .last(last), .do_add(do_add),
        .load(load), .add_en(add_en), .shift(shift),
        .count_en(count_en), .clear(clear), .done(done)
	);

	// Key signals to connect:
	// - load, add_en, shift, count_en, clear (from controller to mult_regs)
	// - count and last (from bit_counter to controller)
	// - Q[0] from mult_regs to add_decision (as Q_lsb)
	// - sum from add_decision to mult_regs
	// - do_add from add_decision to controller
	//

	// product = {A[7:0], Q} after multiplication
	assign product = {A[7:0], Q};
	// Reference the instructor1 solution to see the complete wiring!

endmodule
