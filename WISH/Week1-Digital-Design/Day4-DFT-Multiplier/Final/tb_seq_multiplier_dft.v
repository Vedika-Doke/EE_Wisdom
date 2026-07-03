`timescale 1ns/1ps
module tb_seq_multiplier_dft;
    reg clk = 0;
    reg rst = 1;
    reg start = 0;
    reg [7:0] a, b;
    reg test_mode = 0;
    reg scan_enable = 0;
    reg scan_in = 0;
    wire [15:0] product;
    wire done;
    wire scan_out;

    integer sys_clk_toggles = 0;
    integer gated_clk_toggles = 0;
    reg prev_gated_clk = 0;

    seq_multiplier dut (
        .clk(clk), .rst(rst), .start(start),
        .a(a), .b(b),
        .test_mode(test_mode),
        .scan_enable(scan_enable),
        .scan_in(scan_in),
        .product(product), .done(done),
        .scan_out(scan_out)
    );

    always #5 clk = ~clk;  // 100 MHz

    always @(posedge clk or negedge clk)
        sys_clk_toggles = sys_clk_toggles + 1;

    wire gated_clk_monitor = dut.U_CNT.gated_clk;
    always @(gated_clk_monitor) begin
        if (gated_clk_monitor !== prev_gated_clk) begin
            gated_clk_toggles = gated_clk_toggles + 1;
            prev_gated_clk = gated_clk_monitor;
        end
    end

    // ===== TEST C1: Functional + Power =====
    task test_c1_functional;
        begin
            $display("\n========================================");
            $display("TEST C1: FUNCTIONAL TEST");
            $display("========================================");
            sys_clk_toggles = 0;
            gated_clk_toggles = 0;
            test_mode = 0;
            scan_enable = 0;
            @(negedge clk); rst = 0;
            repeat (5) @(negedge clk);
            $display("Clock toggles during IDLE:");
            $display("  System clock: %0d", sys_clk_toggles);
            $display("  Gated clock : %0d", gated_clk_toggles);
            if (gated_clk_toggles < sys_clk_toggles / 2)
                $display("  PASS: Clock gating active in IDLE");
            else
                $display("  FAIL: Clock gating not working");

            $display("\nRunning multiplication: 13 x 11");
            @(negedge clk); a = 8'd13; b = 8'd11; start = 1;
            @(negedge clk); start = 0;
            wait (done);
            @(negedge clk);
            if (product == 16'd143)
                $display("  PASS: 13 x 11 = %0d", product);
            else
                $display("  FAIL: Expected 143, got %0d", product);

            $display("\nFinal toggle count:");
            $display("  System: %0d   Gated: %0d",
                     sys_clk_toggles, gated_clk_toggles);
            if (sys_clk_toggles > 0)
                $display("  Power savings ~= %0d%%",
                    (100 * (sys_clk_toggles - gated_clk_toggles)) / sys_clk_toggles);
        end
    endtask

    // ===== TEST C2: Scan Shift =====
    task test_c2_scan_shift;
        reg [31:0] scan_pattern;
        integer i;
        begin
            $display("\n========================================");
            $display("TEST C2: SCAN SHIFT TEST");
            $display("========================================");
            @(negedge clk); rst = 1;
            @(negedge clk); rst = 0;
            test_mode = 1;
            scan_enable = 1;

            // Chain head = state[0], tail = cnt[3]. First bit shifted (MSB
            // of `scan_pattern`) walks 32 cycles -> lands in cnt[3].
            // Layout the pattern so each segment lands in its register:
            // pattern = {cnt[3:0], Q[7:0], M[7:0], A[8:0], state[2:0]}
            scan_pattern = {4'h5, 8'h33, 8'hAA, 9'h055, 3'b100};
            $display("Shifting in pattern: %h", scan_pattern);

            for (i = 31; i >= 0; i = i - 1) begin
                @(negedge clk);
                scan_in = scan_pattern[i];
            end
            @(negedge clk); scan_enable = 0;
            #1;
            $display("  state=%b  A=%h  M=%h  Q=%h  count=%h",
                     dut.state, dut.A, dut.M, dut.Q, dut.cnt);
            if (dut.state === 3'b100 && dut.A === 9'h055 &&
                dut.M === 8'hAA && dut.Q === 8'h33 && dut.cnt === 4'h5)
                $display("  PASS: All values shifted correctly");
            else
                $display("  FAIL: Scan shift mismatch");
        end
    endtask

    // ===== TEST C3: Shift-Capture-Shift =====
    task test_c3_shift_capture_shift;
        reg [31:0] scan_in_pattern;
        reg [31:0] scan_out_pattern;
        reg [31:0] expected_pattern;
        integer i;
        begin
            $display("\n========================================");
            $display("TEST C3: SHIFT-CAPTURE-SHIFT TEST");
            $display("========================================");
            @(negedge clk); rst = 1;
            @(negedge clk); rst = 0;
            test_mode = 1;
            a = 8'd13;  // a_in -> M during LOAD
            b = 8'd11;  // b_in -> Q during LOAD

            scan_enable = 1;
            // Layout: {cnt, Q, M, A, state} = {0, 0, 0, 0, LOAD=001}
            // M/Q get loaded from a,b during the capture cycle anyway
            scan_in_pattern = {4'h0, 8'h00, 8'h00, 9'h000, 3'b001};
            for (i = 31; i >= 0; i = i - 1) begin
                @(negedge clk);
                scan_in = scan_in_pattern[i];
            end

            @(negedge clk); scan_enable = 0; scan_in = 0;
            @(negedge clk);
            $display("  After capture: state=%b (expected CHECK=010)", dut.state);

            scan_enable = 1;
            scan_in = 0;
            scan_out_pattern = 0;
            // Sample current cnt[3] *before* each shift so bit i maps to
            // pre-shift chain position i (state[2:0] lands in bits [2:0]).
            for (i = 31; i >= 0; i = i - 1) begin
                #1; scan_out_pattern[i] = scan_out;
                @(negedge clk);
            end
            scan_enable = 0;

            // Same layout: {cnt, Q, M, A, state}; state lands in [2:0]
            expected_pattern = {4'h0, 8'h0B, 8'h0D, 9'h000, 3'b010};
            $display("  Captured: %h", scan_out_pattern);
            $display("  Expected: %h", expected_pattern);
            if (scan_out_pattern[2:0] === 3'b010)
                $display("  PASS: FSM transitioned LOAD -> CHECK");
            else
                $display("  FAIL: FSM state mismatch");
        end
    endtask

    // ===== TEST C4: At-speed path =====
    task test_c4_at_speed;
        reg [31:0] scan_pattern;
        integer i;
        begin
            $display("\n========================================");
            $display("TEST C4: AT-SPEED PATH TEST");
            $display("========================================");
            @(negedge clk); rst = 1;
            @(negedge clk); rst = 0;
            test_mode = 1; scan_enable = 1;

            scan_pattern = {4'h6, 8'h00, 8'h00, 9'h000, 3'b100};
            for (i = 31; i >= 0; i = i - 1) begin
                @(negedge clk);
                scan_in = scan_pattern[i];
            end
            @(negedge clk); scan_enable = 0;
            #1;
            $display("  Initial: count=%0d last=%b", dut.cnt, dut.last);

            force dut.U_FSM.state = 3'd4;  // SHIFT state -> count_en=1
            @(posedge clk); #1;
            release dut.U_FSM.state;
            $display("  After  : count=%0d last=%b", dut.cnt, dut.last);
            if (dut.cnt === 4'd7 && dut.last === 1'b1)
                $display("  PASS: at-speed path verified");
            else
                $display("  FAIL: at-speed increment failed");
        end
    endtask

    initial begin
        $dumpfile("seq_mult_dft.vcd");
        $dumpvars(0, tb_seq_multiplier_dft);

        #20 rst = 0;
        #10 rst = 1;
        #20;

        test_c1_functional();
        test_c2_scan_shift();
        test_c3_shift_capture_shift();
        test_c4_at_speed();

        $display("\nALL TESTS COMPLETE");
        #100 $finish;
    end

    initial begin
        #200000;
        $display("TIMEOUT");
        $finish;
    end
endmodule
