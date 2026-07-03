// Student 4 — Control FSM (controller)
module controller (
	input clk, rst,
	input start, last, do_add,
	output reg load, add_en, shift, count_en, clear, done
);

	localparam IDLE   = 3'd0,
	           LOAD   = 3'd1,
	           CHECK  = 3'd2,
	           ADD    = 3'd3,
	           SHIFT  = 3'd4,
	           FINISH = 3'd6,
	           DONE   = 3'd5;

	reg [2:0] state, nstate;

	always @(posedge clk or posedge rst) begin
		if (rst) state <= IDLE;
		else     state <= nstate;
	end

	always @(*) begin
		case (state)
			IDLE:  nstate = start ? LOAD : IDLE;
			LOAD:  nstate = CHECK;
			CHECK: nstate = do_add ? ADD : SHIFT;
			ADD:   nstate = SHIFT;
			SHIFT: nstate = last ? FINISH : CHECK;
			FINISH: nstate = DONE;
			DONE:  nstate = IDLE;
			default: nstate = IDLE;
		endcase
	end

	always @(*) begin
		load     = 1'b0;
		add_en   = 1'b0;
		shift    = 1'b0;
		count_en = 1'b0;
		clear    = 1'b0;
		done     = 1'b0;
		case (state)
			LOAD:  begin load = 1'b1; clear = 1'b1; end
			ADD:   begin add_en = 1'b1; end
			SHIFT: begin shift = 1'b1; count_en = 1'b1; end
			DONE:  begin done = 1'b1; end
			default: ;
		endcase
	end

endmodule
