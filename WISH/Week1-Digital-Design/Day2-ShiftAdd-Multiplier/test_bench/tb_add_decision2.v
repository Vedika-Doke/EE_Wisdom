`timescale 1ns/1ps

module tb_add_decision;
    reg  [8:0] A;
    reg  [7:0] M;
    reg  Q_lsb;
    wire [8:0] sum;
    wire do_add;

    integer pass_count = 0;
    integer fail_count = 0;

    add_decision dut (
        .A(A), 
        .M(M), 
        .Q_lsb(Q_lsb),
        .sum(sum), 
        .do_add(do_add)
    );

    // Checker_Scoreboard
    task checker_scoreboard;
        input [8:0] expected_sum;
        input expected_do_add;
        input [127:0] test_name;
    begin
        if (sum == expected_sum && do_add == expected_do_add) begin
            $display("[PASS] %0s | A=%h M=%h Q_lsb=%b => sum=%h do_add=%b",
                      test_name, A, M, Q_lsb, sum, do_add);
        end else begin
            $display("[FAIL] %0s | A=%h M=%h Q_lsb=%b => sum=%h (exp %h) do_add=%b (exp %b)",
                      test_name, A, M, Q_lsb, sum, expected_sum, do_add, expected_do_add);
        end
    end
    endtask

    // Apply inputs
    task apply_inputs;
        input [8:0] A_;
        input [7:0] M_;
        input Q_lsb_;
    begin
        A     = A_;
        M     = M_;
        Q_lsb = Q_lsb_;
        #1;
    end
    endtask

    initial begin
        $dumpfile("tb_add_decision.vcd");
        $dumpvars(0, tb_add_decision);

        // Basic sum computation
        apply_inputs(9'h000, 8'h00, 1'b0);
        checker_scoreboard(9'h000, 1'b0, "Basic_A0_M0");

        apply_inputs(9'h000, 8'h0F, 1'b0);
        checker_scoreboard(9'h00F, 1'b0, "Basic_A0_Mnz");

        apply_inputs(9'h055, 8'h00, 1'b1);
        checker_scoreboard(9'h055, 1'b1, "Basic_Anz_M0");

        apply_inputs(9'h010, 8'h20, 1'b1);
        checker_scoreboard(9'h030, 1'b1, "Basic_Anz_Mnz");

        // do_add follows Q_lsb
        apply_inputs(9'h0AB, 8'hCD, 1'b0);
        checker_scoreboard(9'h178, 1'b0, "Qlsb_0");
        apply_inputs(9'h0AB, 8'hCD, 1'b1);
        checker_scoreboard(9'h178, 1'b1, "Qlsb_1");
        apply_inputs(9'h0AB, 8'hCD, 1'b0);
        checker_scoreboard(9'h178, 1'b0, "Qlsb_toggle_back");

        // Carry / 9-bit width
        apply_inputs(9'h0FF, 8'hFF, 1'b1);
        checker_scoreboard(9'h1FE, 1'b1, "Carry_FF_FF");

        apply_inputs(9'h1FF, 8'h01, 1'b0);
        checker_scoreboard(9'h000, 1'b0, "Carry_1FF_01_wrap");

        apply_inputs(9'h100, 8'h00, 1'b1);
        checker_scoreboard(9'h100, 1'b1, "MSB_100_00");

        // Independence of sum from Q_lsb
        apply_inputs(9'h077, 8'h33, 1'b0);
        checker_scoreboard(9'h0AA, 1'b0, "Indep_Q0");
        apply_inputs(9'h077, 8'h33, 1'b1);
        checker_scoreboard(9'h0AA, 1'b1, "Indep_Q1");

        // Corner cases: A,M = {min,max}
        apply_inputs(9'h000, 8'h00, 1'b0);
        checker_scoreboard(9'h000, 1'b0, "Corner_minmin");

        apply_inputs(9'h000, 8'hFF, 1'b1);
        checker_scoreboard(9'h0FF, 1'b1, "Corner_minMax");

        apply_inputs(9'h1FF, 8'h00, 1'b0);
        checker_scoreboard(9'h1FF, 1'b0, "Corner_Maxmin");

        apply_inputs(9'h1FF, 8'hFF, 1'b1);
        checker_scoreboard(9'h0FE, 1'b1, "Corner_MaxMax_wrap");

        $finish;
    end
endmodule