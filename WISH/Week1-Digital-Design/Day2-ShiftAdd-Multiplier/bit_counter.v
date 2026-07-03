module bit_counter (
    input clk, rst,
    input clear, count_en,
    output reg [3:0] count,
    output last
);
    // Comb: assert last when count == 7
    assign last = (count == 4'd7);

    // Seq: reset, clear, or increment
    always @(posedge clk or posedge rst) begin
        if (rst)
            count <= 4'd0;
        else if (clear)
            count <= 4'd0;
        else if (count_en)
            count <= count + 4'd1;
    end

endmodule	
			
		
			
			
