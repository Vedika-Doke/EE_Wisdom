// Student 3 — Conditional Add Logic (add_decision)
// Combinational. Inputs: Q_lsb, A, M.
// Outputs: sum = A+{1'b0,M} (always), do_add = Q_lsb.

module add_decision (
	input [8:0] A,
	input [7:0] M,
	input Q_lsb,
	output [8:0] sum,
	output do_add
);
	assign sum = A + {1'b0, M};
	assign do_add = Q_lsb;

	// TODO: Implement the conditional add logic (combinational)
	// - sum should always compute A + {1'b0, M}
	// - do_add should be Q_lsb
	//
	// This is purely combinational logic. No registers, no reset.
	// The adder runs every instant and the sum is always available.
	// The controller (FSM) decides whether to use this sum or skip it.

endmodule
