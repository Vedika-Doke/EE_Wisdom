module bit_counter (
    input            clk,
    input            rst,
    input            clear,
    input            count_en,
    // DFT signals
    input            test_mode,    // Bypass clock gating
    input            scan_enable,  // 1: shift mode, 0: capture/functional
    input      [3:0] scan_in,      // Scan chain input
    output reg [3:0] count,
    output           last
);
    wire gated_clk;
    ICG u_icg (
        .clk        (clk),
        .enable     (count_en),
        .test_enable(test_mode),
        .gated_clk  (gated_clk)
    );

    wire [3:0] count_next_func = clear ? 4'b0 : count + 4'b1;

    always @(posedge gated_clk) begin
        if (rst)
            count <= 4'b0;
        else
            count <= scan_enable ? scan_in : count_next_func;
    end

    assign last = (count == 4'd7);
endmodule
