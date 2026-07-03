// Top-level with full scan chain
// Chain order: scan_in -> state[2:0] -> A[8:0] -> M[7:0] -> Q[7:0] -> count[3:0] -> scan_out
// Total length: 3 + 9 + 8 + 8 + 4 = 32 bits
module seq_multiplier (
    input            clk,
    input            rst,
    input            start,
    input      [7:0] a,
    input      [7:0] b,
    // DFT
    input            test_mode,
    input            scan_enable,
    input            scan_in,
    output           scan_out,
    output    [15:0] product,
    output           done
);
    wire load, add_en, shift, count_en, clear;
    wire [8:0] A;
    wire [7:0] M, Q;
    wire [2:0] state;
    wire [3:0] cnt;
    wire       last;

    // Scan-chain stitching (LSB of each segment receives upstream bit)
    wire [2:0] scan_state_in;
    assign scan_state_in[0] = scan_in;
    assign scan_state_in[1] = state[0];
    assign scan_state_in[2] = state[1];

    wire [8:0] scan_A_in;
    assign scan_A_in[0] = state[2];
    assign scan_A_in[1] = A[0];
    assign scan_A_in[2] = A[1];
    assign scan_A_in[3] = A[2];
    assign scan_A_in[4] = A[3];
    assign scan_A_in[5] = A[4];
    assign scan_A_in[6] = A[5];
    assign scan_A_in[7] = A[6];
    assign scan_A_in[8] = A[7];

    wire [7:0] scan_M_in;
    assign scan_M_in[0] = A[8];
    assign scan_M_in[1] = M[0];
    assign scan_M_in[2] = M[1];
    assign scan_M_in[3] = M[2];
    assign scan_M_in[4] = M[3];
    assign scan_M_in[5] = M[4];
    assign scan_M_in[6] = M[5];
    assign scan_M_in[7] = M[6];

    wire [7:0] scan_Q_in;
    assign scan_Q_in[0] = M[7];
    assign scan_Q_in[1] = Q[0];
    assign scan_Q_in[2] = Q[1];
    assign scan_Q_in[3] = Q[2];
    assign scan_Q_in[4] = Q[3];
    assign scan_Q_in[5] = Q[4];
    assign scan_Q_in[6] = Q[5];
    assign scan_Q_in[7] = Q[6];

    wire [3:0] scan_cnt_in;
    assign scan_cnt_in[0] = Q[7];
    assign scan_cnt_in[1] = cnt[0];
    assign scan_cnt_in[2] = cnt[1];
    assign scan_cnt_in[3] = cnt[2];

    assign scan_out = cnt[3];

    controller U_FSM (
        .clk(clk), .rst(rst), .start(start),
        .last(last), .q_lsb(Q[0]),
        .scan_enable(scan_enable),
        .scan_in(scan_state_in),
        .state(state),
        .load(load), .add_en(add_en), .shift(shift),
        .count_en(count_en), .clear(clear), .done(done)
    );

    mult_regs U_REGS (
        .clk(clk), .rst(rst),
        .load(load), .add_en(add_en), .shift(shift),
        .a_in(a), .b_in(b),
        .scan_enable(scan_enable),
        .scan_in_A(scan_A_in),
        .scan_in_M(scan_M_in),
        .scan_in_Q(scan_Q_in),
        .A(A), .M(M), .Q(Q)
    );

    bit_counter U_CNT (
        .clk(clk), .rst(rst),
        .clear(clear), .count_en(count_en),
        .test_mode(test_mode),
        .scan_enable(scan_enable),
        .scan_in(scan_cnt_in),
        .count(cnt), .last(last)
    );

    assign product = {A[7:0], Q};
endmodule
