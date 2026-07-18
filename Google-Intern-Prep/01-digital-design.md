# 01 — Digital Design (highest-weight topic)

Digital logic is the core of both the 1-hour test and the interviews. You should be able to do everything here on paper, fast, without a simulator.

---

## A. Combinational logic — what you must know cold

- **Boolean algebra**: De Morgan's, absorption (`A + AB = A`), consensus (`AB + A'C + BC = AB + A'C`).
- **K-maps** up to 4 variables, including don't-cares. Know what a *prime implicant* and *essential prime implicant* are.
- **Universal gates**: build NOT/AND/OR/XOR from only NAND (and only NOR). XOR from NAND = 4 gates.
- **Mux as universal logic**: any n-input function with a 2^(n-1):1 mux + inverter. Classic: **XOR from a 2:1 mux** — select = A, in0 = B, in1 = B'.
- **Adders**: ripple-carry delay O(n); carry-lookahead (`G = AB`, `P = A⊕B`, `Cout = G + P·Cin`); full adder from two half adders + OR.
- **Hazards**: static-1 hazard in SOP (e.g. `AB + A'C` when B=C=1 and A switches) — fix with consensus term BC. Glitches matter when the output drives a clock or async reset.

### Warm-up questions (answers at each bullet)
1. Implement a full adder using two 4:1 muxes. — *Sum and Cout are each functions of (A,B,Cin); use A,B as selects, Cin-terms as data inputs: Sum data = (Cin, Cin', Cin', Cin), Cout data = (0, Cin, Cin, 1).*
2. F = ΣΜ(1,3,5,7) of (A,B,C). Minimize. — *F = C (all minterms have C=1).*
3. How many 2:1 muxes to build a 16:1 mux? — *15 (tree: 8+4+2+1).*
4. Detect if a 4-bit number is divisible by 3 (combinational). — *Truth-table / K-map it, or note div-by-3 ⇔ (sum of even-position bits − sum of odd-position bits) mod 3 = 0. For 4 bits just enumerate {0,3,6,9,12,15}.*
5. Why are NAND-based designs preferred over NOR in CMOS? — *Series NMOS (NAND) beats series PMOS (NOR) because electron mobility ≈ 2-3× hole mobility → NAND is faster/smaller for same drive.*

---

## B. Sequential logic

- **Latch vs flip-flop**: latch = level-sensitive (transparent), FF = edge-triggered. A D-FF = two latches back-to-back (master-slave), opposite phases.
- **Where do latches come from accidentally?** Incomplete `if`/`case` in combinational always blocks (see Verilog file).
- **Flip-flop conversions**: JK↔D↔T. `D = JQ' + K'Q`; `T = 1` makes a toggle FF (divide-by-2).
- **Counters**: ripple (async) vs synchronous — ripple accumulates clk-to-Q delays (f_max ∝ 1/(n·t_cq)), sync all FFs clocked together. Mod-N counter needs ⌈log2 N⌉ FFs.
- **Ring counter**: N states with N FFs (one-hot). **Johnson (twisted-ring)**: 2N states with N FFs, glitch-free decode with 2-input gates.
- **Gray code**: successive values differ in 1 bit. Binary→Gray: `G = B ^ (B >> 1)`. Gray→Binary: prefix-XOR. **Why it matters: passing a multi-bit counter across clock domains (async FIFO pointers) — only one bit changes per increment, so a synchronizer can never capture an inconsistent multi-bit value.**

---

## C. FSM design — the single most-asked interview topic

Method (say it out loud in the interview):
1. Choose Mealy vs Moore. **Moore**: output = f(state) — one more state typically, output glitch-free and registered. **Mealy**: output = f(state, input) — fewer states, output can change mid-cycle and can glitch.
2. Draw state diagram → state table → assign encodings → next-state logic.
3. Always state your **reset state** and whether detection is **overlapping**.

### Worked example: detect `1011` (overlapping, Moore)
States: S0 (idle), S1 (`1`), S2 (`10`), S3 (`101`), S4 (`1011`, output=1).
Transitions: S0: 1→S1, 0→S0 · S1: 0→S2, 1→S1 · S2: 1→S3, 0→S0 · S3: 1→S4, 0→S2 · S4 (out=1): 1→S1, 0→S2 *(overlap: `1011` ends in `11`? No — it ends `…11`; after S4, seen suffix is `11`… careful: after detecting 1011, last two bits `11` → next 0 gives suffix `110`→ relevant suffix `10` → S2; next 1 gives suffix `111` → suffix `1` → S1).*
Interview tip: derive re-entry states by asking "what is the longest suffix of what I've seen that is also a prefix of the pattern" (KMP intuition). Saying this phrase impresses.

### Worked example: serial divisibility-by-3 (MSB first)
State = running remainder r ∈ {0,1,2}. New bit b: r_next = (2r + b) mod 3.
- r0: 0→r0, 1→r1
- r1: 0→r2, 1→r0
- r2: 0→r1, 1→r2
Output = 1 in r0. **LSB-first variant**: r_next = (r + b·2^k) mod 3; since 2^k mod 3 alternates 1,2,1,2… you need state (remainder, parity of position) → 6 states, or note the alternating weight trick. If asked, do MSB-first unless told otherwise, and mention the LSB-first subtlety.

### Other FSMs to practice on paper (10 min each)
- Sequence detector `0110` non-overlapping, Mealy.
- Serial two's-complementer (copy bits until after first `1`, then invert — LSB first).
- Vending machine: 5/10 coins, dispense at 15, with change.
- Round-robin arbiter for 2 requesters (grant alternates on contention) — describe states.
- Traffic-light controller with a timer input.

---

## D. Classic circuit-design questions (with solutions)

**1. Divide clock by 2.** T-FF: `always @(posedge clk) q <= ~q;` Output is 50% duty automatically.

**2. Divide by 3 with 50% duty cycle.** Div-by-3 counter (states 0,1,2) on posedge gives 33% duty. Trick: generate one signal on posedge (high for state 0,1) and another on **negedge** (delayed copy), OR/AND them to get 1.5-cycle high. Standard answer: posedge counter q_p high in states {0,1}; negedge-registered version q_n of the same; `out = q_p & q_n` → high 1.5 cycles, low 1.5 cycles. Be ready to draw the waveform. (General rule: odd division with 50% duty needs both clock edges.)

**3. Rising-edge detector (synchronous).** Register the signal; `pulse = sig & ~sig_d`. One-cycle pulse. Falling: `~sig & sig_d`. Any edge: XOR.

**4. Frequency doubler.** `out = sig ^ delay(sig)` — XOR of signal with a delayed version pulses on every edge. Caveats (say them): pulse width set by an uncontrolled delay chain, not synthesis-friendly, duty cycle terrible; real designs use a PLL/DLL.

**5. Glitch-free clock mux.** Naive mux glitches when select changes mid-cycle. Standard circuit: for each clock, an enable FF clocked on that clock's **negedge**, cross-coupled so each enable can assert only after the other has deasserted (each enable's input includes the other's synchronized complement). Gate each clock with its enable, OR the results. Know the *why*: never let a gated clock be chopped mid-pulse.

**6. Clock gating done right.** `en` ANDed with clk directly can glitch. Use an **ICG cell**: active-low latch on `en` (transparent when clk low) feeding the AND — enable can only change while clock is low.

**7. Count ones in a 7-bit vector.** Tree of full adders (a full adder is a 3:2 compressor: 3 bits → 2-bit count). 7 bits → population count with 4 FAs. Or in RTL, `$countones` / a for-loop — say both.

**8. Detect a 4-bit gray counter error** / why one-hot vs binary vs gray encoding: one-hot = fast decode, more FFs, good for small-state FSMs in FPGAs; binary = fewest FFs; gray = CDC-safe counters.

**9. Swap two registers without a temp** (hardware): in RTL it's trivial — `a <= b; b <= a;` with nonblocking assignments both read old values. This doubles as a Verilog-semantics question.

**10. Design a circuit to check if a bit-serial stream has more 1s than 0s so far.** Up/down saturating counter; output = sign bit. Discuss width/overflow.

---

## E. Rapid-fire MCQ practice (test-style; answers below)

1. Minimum NAND gates for XOR? 
2. A 4-bit ripple counter uses FFs with t_cq = 2 ns. Max clock frequency? (Need settled output before next edge; decode settling ignored)
3. Setup time is violated. Does reducing clock frequency help? Hold time violated — does it help?
4. How many FFs for a mod-12 counter? Unused states?
5. In a Johnson counter with 4 FFs, how many states? 
6. Which is glitch-free on decode: ring, Johnson, binary counter?
7. `A ⊕ A' = ?`, `A ⊕ 1 = ?`, `A ⊕ A = ?`
8. A Moore FSM detecting an n-bit sequence needs at least how many states?
9. What does metastability mean and when does it occur?
10. Two FFs, Q1→ combinational delay 3 ns →D2, t_cq=1 ns, t_setup=1 ns, t_hold=0.5 ns, skew=0. Max frequency?

**Answers:** 1) 4. 2) Worst path 4·2=8 ns → 125 MHz. 3) Setup: yes (more time per cycle). Hold: **no** — hold is checked at the same edge, independent of period; fix with delay buffers. 4) 4 FFs, 16−12 = 4 unused. 5) 2N = 8. 6) Ring & Johnson (one bit changes / adjacent decode); binary glitches on multi-bit transitions. 7) 1, A', 0. 8) n+1. 9) FF output hangs between 0/1 for unbounded time when data changes inside the setup/hold window — happens on async inputs / CDC. 10) T ≥ t_cq + t_comb + t_setup = 1+3+1 = 5 ns → 200 MHz.

---

## F. How to practice
- Redo every FSM in section C on blank paper, then code each in Verilog (file 02) and check by hand-simulating 10 cycles.
- Source for volume: any GATE digital-logic question set (previous-year GATE EC/EE digital questions are almost exactly test-level).
