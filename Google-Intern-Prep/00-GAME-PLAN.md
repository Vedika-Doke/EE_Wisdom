# 00 — Master Game Plan: Google Silicon Engineering Intern (IITB campus)

**The target:** Silicon Engineering Intern, Google Hardware Silicon (Cloud Silicon: TPU/Axion + Devices: Tensor). ₹1,35,000/month, Bengaluru/Hyderabad, mid-May–mid-July 2027 internship. You must be in your penultimate year (grad 2028 BTech / 2029 Dual).

**The gauntlet:** ① 1-hour technical test → ② resume filtering → ③ Technical interview 1 (1 hr) → ④ Technical interview 2 (1 hr). Each stage is eliminative.

---

## What each stage really is (from first-person IIT KGP/IITB/IITR + India accounts, 2022-2025 — details in file 10)

| Stage | Reality (well-supported across accounts) | Your counter |
|---|---|---|
| **Technical test** | **Objective MCQ, not a coding contest**: digital electronics, CMOS/VLSI, computer architecture, SoC/ASIC, microcontrollers, Verilog/HDL — coding appears as **"predict the output," you don't write code**. Questions can be revisited; no negative marking reported. GATE difficulty, basic→advanced. Speed is the filter. | Files 01, 03, 04, 06 + mock test (09). Drill GATE previous-year digital + CO MCQs for volume; practice predict-the-output Verilog/C snippets. |
| **Resume + CGPA filter** | Shortlist = **test + CV + CGPA together**. IIT KGP selectee: "8.5+ CGPA opens all the doors." Core projects (FPGA/UART/RTL, PCB, research) matter most; PoRs reportedly irrelevant; prior internship not mandatory — self-projects count. Google's own posting says to list HDL/coding proficiency explicitly. | Resume shows: Verilog/FPGA project(s), core courses (EE224 Digital Systems, EE309 Microprocessors, comp-arch, VLSI), Python. Quantify everything. 2-3 core projects you can defend to 3-question depth. |
| **Interview 1** (~1 hr, eliminative) | **Digital + Verilog heavy**: latches vs FFs (sync/async), FSM design with explicit state diagrams, K-maps, "build X using only Y" (full adder from 2:1 muxes, mux from NANDs, logic from pass transistors/transmission gates), Verilog probes (swap two variables with and without a temp = blocking/non-blocking). Coding in a shared Google Doc, circuits drawn on paper shown to camera. | Files 01, 02, 10. "Verilog is a must to beat the competition" — an actual selectee's words. |
| **Interview 2** (~1 hr, same day, ~30 min later) | **STA + CMOS + architecture + CV deep-dive**: setup/hold with detailed reasoning on multi-FF paths (**STA is the single most-flagged topic across every year's account**), NMOS vs PMOS sizing/speed/power, memory taxonomy (SRAM/DRAM/ROM/PROM/EPROM/EEPROM, cache, ISA), deep grilling on one CV project (specs, clocking, implementation), and **exactly one easy coding question** (palindrome, basic recursion/DP — LeetCode-easy, any language). | Files 03, 04, 06, 08 + your project stories. Re-derive the timing equations until automatic. |

**No separate HR round on campus** — but "why hardware over software?" gets asked *inside* technical rounds; have a crisp answer referencing Tensor/gChips (file 08). Google scores every interview on 4 axes — GCA (structured reasoning aloud), Role knowledge, Emergent leadership, Googleyness (file 08B). Selected candidates uniformly credit **thinking aloud**: interviewers are friendly, help when stuck, and grade approach over answer. A correct answer reached silently scores worse than a near-correct one reasoned transparently.

**Funnel reality check:** the one drive with numbers reported ~10 interview shortlists → 1 hire (non-IIT drive; IIT cohorts appear to be small single digits per campus). The test+CGPA gate is where most people fall — don't under-prepare the "easy" MCQ stage.

---

## The file map (your syllabus)

- **01 Digital design** — combinational, sequential, FSMs, classic circuits *(biggest test + interview weight)*
- **02 Verilog/SystemVerilog** — the three classic probes, templates to memorize, FIFO code, SVA
- **03 STA & CDC** — the two timing equations, synchronizers, FIFO depth calc
- **04 Computer architecture** — pipeline, caches, numericals, TPU/accelerator talking points
- **05 Verification & DFT** — the 5-step "how would you verify X" answer, assertions, scan/ATPG/BIST
- **06 CMOS & low power** — inverter/VTC, power equation, technique menu, SRAM/DRAM
- **07 Puzzles, scripting, bits** — GCA puzzles, Python log-parsing, C bit tricks
- **08 Google context & behavioral** — chip portfolio, four axes, STAR stories, questions to ask
- **09 Mock test** — 1-hour timed simulation with key
- **10 Reported questions bank** — what was actually asked at Google + peers; gap-fillers (BJT, 8085 debug, protocols, transistor-drawing drill)

## 4-week schedule (compress to the same order if you have less time)

**Week 1 — Core digital + Verilog (the make-or-break week)**
- Days 1-3: File 01 end-to-end; redo every FSM on paper. Start HDLBits (Circuits section).
- Days 4-6: File 02; write every template from memory; hand-simulate. HDLBits daily.
- Day 7: File 03 sections A-B (timing equations + synchronizers). 20 GATE digital MCQs timed.

**Week 2 — Timing, architecture, CMOS**
- Days 8-9: File 03 complete; do every FIFO-depth example + invent 2 variants.
- Days 10-12: File 04; cache/pipeline numericals until 2-min-solvable.
- Days 13-14: File 06 + file 10F transistor-drawing drill (draw all 8, twice).

**Week 3 — Verification, breadth, reported questions**
- Days 15-16: File 05; rehearse the 5-step verify answer on FIFO/arbiter/counter aloud.
- Days 17-18: File 10 end-to-end — every tagged question; fill gaps (BJT, protocols, 8085 debug ladder).
- Days 19-20: File 07: puzzles + write the Python log-parser from scratch; 10 easy bit/array problems.
- Day 21: **Mock test (09) under real conditions.** Score, triage, revisit weak file.

**Week 4 — Sharpening + interview simulation**
- Day 22-23: Redo weak areas from mock; second pass of file 10 section C (the universal list — be *perfect* on all 17).
- Day 24: File 08: write your four STAR stories; rehearse resume walkthrough aloud; prep "why Google hardware" with 2 specifics.
- Day 25: Mock interview 1 with a friend/senior: FSM + Verilog + resume grilling, in a shared Google Doc.
- Day 26: Mock interview 2: timing numericals + FIFO depth + CMOS drawing + one puzzle. 
- Day 27: Light review of files 03C, 10C, 08E. 
- Day 28: Rest. Sleep. Seriously.

**If you have only 1 week:** Days 1-2 file 01 + 02 · Day 3 file 03 · Day 4 files 04+06 · Day 5 file 10 (all) · Day 6 mock test + patch holes · Day 7 file 08 + STAR stories + rest.

## Day-before checklist
- [ ] Re-derive: setup and hold equations; f_max; FIFO depth method
- [ ] Re-draw: CMOS inverter VTC, NAND2, AOI22, glitch-free clock mux, 2-FF sync
- [ ] Re-code from memory: async-reset FF, 1011 FSM, sync FIFO, edge detector
- [ ] Reread file 10 sections A-C and file 08 talking points
- [ ] STAR stories: say each aloud once
- [ ] Logistics: quiet room, stable net, water, rough paper + pen for the test

## Resources selectees actually named (use these, not random YouTube)

- **NESO Academy** — digital electronics videos (cited by a 2022 selectee).
- **Morris Mano / Anand Kumar** — digital design texts; **GATE previous-year papers** (EC/EE digital + CO sections) for test-volume practice.
- **NPTEL: Prof. Indranil Sengupta's Verilog lectures** — the standard Verilog recommendation among selectees; NPTEL computer architecture as backup.
- **HDLBits** — free interactive Verilog problems with instant feedback (Circuits section ≈ the interview pool).
- **IITB courses that ARE the syllabus** (per an IITB selectee): EE224 (Digital Systems), EE214/204 (Analog/Circuits), EE309 (Microprocessors) — your own course notes are primary material.
- **GeeksforGeeks / InterviewBit** — computer-org basics and the one easy coding question + puzzles.
- File 10 section H: the Intel 101-question list and the autonomousvision 25-problem RTL practice list.

## Standing rules
1. **Paper first, simulator second.** The interview medium is a doc/whiteboard.
2. **Never answer silently.** Narrate assumptions and checks — it's scored.
3. **Sanity-check every numerical** (units, limiting cases) before stating it.
4. If stuck in an interview: restate the problem, do the trivial case, generalize. Asking a clarifying question is a plus, not a weakness.
5. It's fine to say "I haven't used UVM in anger, but I know the architecture" — honesty + adjacent knowledge beats bluffing every time at Google.
