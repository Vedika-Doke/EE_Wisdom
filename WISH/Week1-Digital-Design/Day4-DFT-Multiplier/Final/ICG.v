// Integrated Clock Gate (latch-based, glitch-free)
// gated_clk = clk & en_latch, where en_latch is transparent while clk=0
module ICG (
    input  clk,
    input  enable,       // Functional enable (e.g., count_en)
    input  test_enable,  // DFT bypass: forces clock through during test
    output gated_clk
);
    reg en_latch;
    always @(clk or enable or test_enable) begin
        if (!clk)
            en_latch = enable | test_enable;
    end
    assign gated_clk = clk & en_latch;
endmodule
