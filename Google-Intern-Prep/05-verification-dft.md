# 05 — Design Verification & DFT (JD preferred: DV, DFT, Lint, formal equivalence)

DV is where Google (and every silicon company) puts most intern headcount. You don't need UVM mastery — you need to show you *think like a verifier*: "how would I prove this design is correct?"

---

## A. The verification mindset (use this framing in interviews)

Given any design you just described (FIFO, arbiter, FSM), expect: **"How would you verify it?"** Answer in this order:
1. **Plan**: list features → testpoints (FIFO: full/empty behavior, simultaneous rd+wr, wrap-around, reset mid-operation, overflow/underflow attempts, data integrity/ordering).
2. **Stimulus**: directed tests for corners + **constrained-random** for the space you didn't think of.
3. **Checking**: self-checking testbench — reference model/scoreboard (FIFO ⇒ a software queue), plus **assertions** for properties (never write when full).
4. **Coverage**: code coverage (line/toggle/branch/FSM) + **functional coverage** (did I *hit* full? simultaneous rd/wr at full?) — "done" = coverage closure, not "tests pass."
5. Mention **regression**: run the suite on every RTL change, triage failures.

That five-step answer, delivered unprompted, is an interview-winning move for a DV-flavored round.

## B. Concepts you should be able to define in one breath

- **Directed vs constrained-random**: hand-written corner cases vs randomized stimulus under constraints; random finds bugs you didn't imagine, needs coverage to know when to stop.
- **Scoreboard**: golden/reference model compared against DUT outputs, usually transaction-by-transaction.
- **Assertion (SVA)**: a property checked continuously, e.g.
  `assert property (@(posedge clk) disable iff (!rst_n) !(wr_en && full));`
  `assert property (@(posedge clk) req |-> ##[1:5] ack);`
  Know `|->` vs `|=>`, `##N` delay, and that assertions also serve **formal** tools. **Practice writing an assertion from a timing diagram — this exact task has been reported in Google hardware intern interviews.**
- **Functional vs code coverage**: code = did the *implementation* execute; functional = did the *specified behavior* happen. 100% code coverage ≠ verified (missing-feature bugs show up in neither).
- **UVM (awareness)**: standard SV class library — testbench = agents (driver + monitor + sequencer), scoreboard, env, test; sequences generate transactions; factory/config for reuse. As an intern say: "I know the architecture and have written class-based TBs; I haven't built a full UVM env solo" — honest and fine.
- **X-propagation, reset verification**: X in sim can hide or invent bugs; two-state vs four-state sim.
- **Emulation/FPGA prototyping**: orders faster than simulation, for SW bring-up and long scenarios.

## C. Formal & static (JD keywords — cheap points if you know the words)

- **Lint**: static RTL checks — width mismatch, latch inference, incomplete sensitivity, unreachable states, multiple drivers. Runs in seconds, first quality gate.
- **CDC checking**: structural analysis for unsynchronized crossings (see file 03).
- **Formal property verification**: exhaustively proves assertions via model checking — no stimulus at all; great for arbiters, FIFOs, security logic; state-space limits mean block-level use.
- **Formal equivalence (LEC)**: proves synthesized/edited netlist ≡ RTL — how you trust synthesis and manual ECOs without re-simulating.
- **Low-power verification**: UPF describes power domains; checks isolation cells, retention, level shifters (ties to file 06).

## D. DFT — Design for Testability (manufacturing test, not verification — know the difference!)

**Verification asks "did we design it right?" DFT/test asks "was each manufactured die built right?"** Interviewers love candidates who articulate this distinction.

- **Fault models**: stuck-at-0/1 (classic), transition/at-speed faults, bridging.
- **Controllability/observability**: internal nodes are hard to set/see from pins → DFT adds access.
- **Scan**: replace FFs with scan FFs (mux: functional D vs scan-in) → chain all FFs into shift registers → shift in a test pattern, one functional capture pulse, shift out and compare. Turns sequential test into (almost) combinational test.
- **ATPG**: tool generates minimal pattern set for target fault coverage (98%+ typical).
- **BIST**: on-chip test — **LFSR** generates pseudo-random patterns, **MISR** compacts responses into a signature; memory BIST (march algorithms) for SRAMs.
- **JTAG / boundary scan (IEEE 1149.1)**: serial TAP port, boundary cells around the I/O ring — board-level interconnect test + debug access.
- Cost angle: tester time is money; scan compression (EDT) reduces pattern volume ~100×.

## E. Practice questions

1. Verify a 2:1 arbiter — what properties? — *Never two grants at once (mutex), grant only when requested, no starvation (eventual grant), grant deasserts when request drops. First two are formal-friendly safety properties; starvation is a liveness property.*
2. Your FIFO test passes 10,000 random operations. Are you done? — *No — check functional coverage: hit full, empty, wr@full, rd@empty, wrap-around, reset mid-traffic; add assertions; consider formal for pointer invariants.*
3. Write an assertion: "gnt must come 1 cycle after req and last exactly 1 cycle." — `assert property (@(posedge clk) $rose(req) |=> gnt ##1 !gnt);`
4. A bug escapes to silicon despite 100% code coverage. How? — *Spec feature never implemented (no code to cover), coverage of wrong config, X-optimism, or a timing/CDC issue invisible to RTL sim.*
5. Why can't we test a chip by just running functional patterns on the tester? — *Sequential depth: setting an internal state through functional inputs takes unbounded cycles; coverage would be terrible. Scan gives direct load/observe of every FF.*
6. What's the overhead of scan? — *~2-5% area (mux per FF), slight timing impact, extra pins/compression logic. Universally worth it.*
7. Difference between simulation and formal? — *Sim: specific input sequences, samples the space. Formal: proves a property over ALL inputs/states, exhaustive but capacity-limited.*
8. What is metastability's impact on scan? — *Cross-domain paths handled with lockup latches; scan chains ordered per domain.*
