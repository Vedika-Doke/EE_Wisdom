# 09 — One-Hour Mock Technical Test

Simulate real conditions: 60 minutes, no notes, no calculator beyond rough work. 25 MCQs (~90 sec each) + 2 subjective (~10 min total). Answer key at the bottom — don't peek. Target: ≥ 19/25 MCQs.

---

## Section 1 — MCQs

**Digital logic**
1. The minimum number of 2-input NAND gates to implement a 2-input XOR is: (a) 3 (b) 4 (c) 5 (d) 6
2. An SOP expression `AB + A'C` can glitch. The added term to remove the static-1 hazard is: (a) AC (b) BC (c) AB' (d) B'C
3. A mod-10 synchronous counter requires how many flip-flops? (a) 3 (b) 4 (c) 5 (d) 10
4. A Johnson counter built with 5 FFs cycles through how many states? (a) 5 (b) 10 (c) 25 (d) 32
5. For a Moore FSM detecting the sequence 110 (overlapping), the minimum number of states is: (a) 3 (b) 4 (c) 5 (d) 6
6. Gray code for binary `1010` is: (a) 1111 (b) 1101 (c) 1011 (d) 0101
7. The 2's complement of 8-bit `1010 0110` represents (as signed decimal): (a) −90 (b) −86 (c) −102 (d) 166
8. A 4:1 mux has select lines S1=A, S0=B and data inputs I0..I3 = (0, 1, 1, 0). Output = ? (a) A⊕B (b) A'+B (c) (A⊕B)' (d) AB — *(work the truth table)*

**Timing**
9. FF parameters: t_cq = 1 ns, t_setup = 0.5 ns, t_hold = 0.6 ns; combinational path 2.5 ns. Max clock frequency: (a) 200 MHz (b) 250 MHz (c) 400 MHz (d) 500 MHz
10. In Q9, the minimum combinational delay to avoid a hold violation (zero skew): (a) 0 ns — always safe since t_cq > t_hold (b) 0.6 ns (c) 0.1 ns (d) 1.1 ns
11. Increasing clock period fixes: (a) both setup and hold violations (b) setup only (c) hold only (d) neither
12. Positive skew (capture clock late) makes: (a) setup easier, hold harder (b) setup harder, hold easier (c) both easier (d) both harder

**Verilog**
13. `always @(posedge clk) begin b = a; c = b; end` synthesizes to: (a) 2 FFs in series (b) both FFs loading `a` (c) 1 FF, c only (d) a latch
14. `always @(*) case(sel) 2'b00: y=a; 2'b01: y=b; 2'b10: y=c; endcase` infers: (a) 4:1 mux (b) 3:1 mux + latch (c) priority encoder (d) nothing, syntax error
15. In Verilog, `4'b10x1 == 4'b10x1` evaluates to: (a) 1 (b) 0 (c) x (d) z
16. Which is true about `<=` in a clocked always block? (a) RHS sampled at end of timestep (b) executes in statement order like C (c) all RHS sampled before any LHS updates (d) it is not synthesizable

**Architecture**
17. A 64 KB direct-mapped cache, 32-byte lines, 32-bit addresses. Tag width: (a) 15 (b) 16 (c) 17 (d) 21
18. With full forwarding in a 5-stage pipeline, a load followed immediately by a dependent ALU op stalls: (a) 0 cycles (b) 1 cycle (c) 2 cycles (d) 3 cycles
19. AMAT with 1-cycle hit, 4% miss rate, 50-cycle penalty: (a) 2 cycles (b) 3 cycles (c) 4 cycles (d) 5 cycles
20. Amdahl: 80% of a task is parallelizable with ∞ processors. Max speedup: (a) 4 (b) 5 (c) 8 (d) ∞

**CMOS / misc**
21. CMOS dynamic power scales as: (a) V (b) V² (c) V³ (d) independent of V
22. NAND is preferred over NOR in CMOS because: (a) fewer transistors (b) series NMOS beats series PMOS (c) NOR can't be universal (d) layout symmetry
23. An NMOS pass transistor passes: (a) strong 0, weak 1 (b) strong 1, weak 0 (c) both strong (d) both weak
24. Clock gating primarily reduces: (a) leakage (b) dynamic power (c) short-circuit power (d) IR drop
25. A 2-FF synchronizer: (a) removes metastability (b) reduces the probability of propagating a metastable value (c) synchronizes multi-bit buses safely (d) adds no latency

## Section 2 — Subjective (write actual Verilog / show actual work)

**S1 (10 min).** Design an FSM that outputs 1 whenever the last three serial input bits contain an odd number of 1s. Draw the state diagram, then write synthesizable Verilog (module, ports, reset included).

**S2 (8 min).** A producer writes bursts of 60 words at 150 MHz (1 word/cycle); the consumer reads continuously at 100 MHz (1 word/cycle). (i) Minimum FIFO depth? (ii) The consumer now reads only 1 word every 2 cycles — what depth is needed? Justify.

---

## Answer key

**MCQs:** 1-b · 2-b · 3-b · 4-b (2N=10) · 5-b (S0, S1="1", S2="11", S3="110"/out=1; from S3: input 1→S1, input 0→S0) · 6-a (1010→1111) · 7-a (invert+1 → 0101 1010 = 90 → −90) · 8-a (enumerate: sel AB = 00→0, 01→1, 10→1, 11→0 = A⊕B) · 9-b (T = 1+2.5+0.5 = 4 ns) · 10-a (t_cq 1 ns > t_hold 0.6 ns → margin even at 0 comb delay) · 11-b · 12-a · 13-b (blocking: b=a, then c=new b → both FFs load `a`; the point is it's NOT a 2-stage shift register) · 14-b · 15-c (`==` with x → x; `===` would give 1) · 16-c · 17-b (64KB/32B = 2048 lines → index 11, offset 5 → tag = 32−11−5 = 16) · 18-b · 19-b (1 + 0.04×50 = 3) · 20-b (1/0.2) · 21-b · 22-b · 23-a · 24-b · 25-b

**S1 sketch:** Track last 3 bits = 3-bit shift register (8 states); output = XOR of the 3 bits (odd parity). Cleanest: shift register + `assign out = ^bits;` — recognizing you don't need a hand-drawn 8-state FSM *is* the answer. If they insist on FSM form: states = last-2-bits + parity handling; but shift-register+XOR shows design maturity.

**S2:** (i) Burst = 60 × (1/150M) = 400 ns. Reads in window = 400 ns × 100 MHz = 40. Depth ≥ 60 − 40 = **20**. (ii) Reads at 50 M words/s → 400 ns window drains 20 → naive depth 40; **but check sustained rate**: to not overflow long-term, average write rate must ≤ 50 Mwords/s — depends on burst spacing. If bursts repeat faster than every 60/50M = 1.2 μs, no finite depth works. State the condition; that's the full answer.

---

## Scoring guide
- 22+ : test-ready. 
- 17-21: redo weak-area file, retake variants.
- <17: two more days on files 01/03/04 before retaking.
