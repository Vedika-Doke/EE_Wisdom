# 10 — Actually-Reported Questions (Google + peer companies)

Compiled from interview experiences (GeeksforGeeks, Glassdoor, personal blogs, prep guides). Tags: [GOOGLE] reported at Google; [QCOM]/[NVDA]/[TI]/[AMD]/[INTC]/[AAPL] at that company; [POOL] from standard VLSI-interview compilations these companies draw on. Files 01-09 contain the how-to-answer material; this file tells you what actually gets asked.

---

## A. Process intelligence — how Google's loop has actually run (India hardware)

From a first-person Google India hardware campus account (priyanshusinha.in) + prep guides:
- **Online test: ~25 objective questions in 30 minutes** — digital logic, CMOS design, SoC/ASIC, microprocessors, computer architecture, RTL coding, HDL/Python. Your posting says 1 hour — expect the same pool, more questions. **Speed matters: ~70-90 sec/question.**
- **Interview rounds are each eliminative, 40-60 min.** In that account:
  - Round 1 leaned **device/analog**: identify a BJT circuit and explain it; MOSFET leakage mechanisms, subthreshold operation, parasitic capacitances; how to reduce leakage power.
  - Round 2 leaned **digital systems**: 8085/microprocessor troubleshooting scenario, memory + DMA, FSM design, CMOS logic.
  - Round 3: almost entirely **"draw the transistor-level CMOS implementation of this logic block."**
- Google asks your **preferred HDL** beforehand; you code in a **shared Google Doc** (no syntax highlighting — practice typing clean Verilog in a plain doc!). A **Python coding** component is commonly reported.
- Behavioral content: past experiences, why hardware, why Google (see file 08).

## B. Questions reported at Google specifically

1. [GOOGLE] Identify/explain a BJT-based circuit; BJT device-physics follow-ups.
2. [GOOGLE] MOSFET leakage currents, subthreshold region, parasitic caps; **techniques to reduce leakage** (multi-Vt, power gating, transistor stacking, back-bias, lower VDD — file 06).
3. [GOOGLE] A microprocessor lab setup doesn't work — walk through debugging it (framework in section E below).
4. [GOOGLE] FSM design on the spot (file 01C); CMOS logic questions; transistor-level drawings of logic blocks (file 06A + practice below).
5. [GOOGLE-prep] Latch vs flip-flop; leakage power definition and significance; CDC challenges and solutions; "how would you use scripting to parse/analyze a config file?" (file 07D — this maps 1:1 to the JD's scripting line).
6. [GOOGLE-prep] I2C basics; 3-wire vs 4-wire SPI; DDR4 vs previous generations (section F below).
7. [GOOGLE-prep, DV-flavored] UVM testbench components for a FIFO DUT; verification plan from a spec; constrained-random vs directed; SVA; coverage closure (file 05).
8. [GOOGLE] Writing **SystemVerilog assertions from a timing diagram** (also in file 05B — practice this specifically).

## B2. Reported at Google IIT-campus drives specifically (KGP / IITB / IITR first-person accounts, 2022-2025)

**Test stage:** MCQ on digital electronics, microcontrollers, CMOS circuits, comp-arch, SoC/VLSI; short coding questions are **predict-the-output only**. Questions revisitable; no negative marking reported.

**Interview round 1 (digital + Verilog):**
1. Sync vs async flip-flops; latch vs FF; master-slave FF peculiarities [IITR].
2. FSM design **with explicit state diagram drawing** — every year's account.
3. K-map minimization on the spot.
4. **Swap two variables in Verilog with and without a temp variable** [KGP 2023] — this is the blocking/non-blocking probe in disguise (file 02A).
5. Build a **full adder using only 2:1 muxes** [IITR]; implement a **mux using only NAND gates** [IITR].
6. Circuit implementation using **pass transistors / transmission gates / CMOS** [KGP 2022].
7. "Implement binary search with digital circuits"-style logical design [KGP 2022] — priority-encoder / comparator-tree thinking.
8. One op-amp/analog question showed up in some years [KGP 2022] — basic op-amp configs (inverting/non-inverting gain, virtual ground) is enough.
9. Draw the output waveform of a given FF network; write the next-state equation of a given FSM; review/debug given Verilog [Glassdoor India].

**Interview round 2 (STA + CMOS + arch + CV):**
10. **Setup/hold analysis with detailed reasoning on multi-FF paths — the most-flagged topic in every single year's account.** (File 03A until it's automatic.)
11. PMOS vs NMOS: sizing difference, which is faster, power analysis [KGP 2023].
12. Memory taxonomy: RAM vs ROM vs PROM vs EPROM vs EEPROM, cache basics, ISA [KGP 2023/2022] — know one line each (EPROM = UV-erasable, EEPROM = electrically erasable/byte-wise, flash = block-wise EEPROM descendant).
13. Gate-level design + power and area analysis [KGP 2022].
14. **CV project deep-dive**: IC specs, memory capacity, clock speed, implementation choices of YOUR project [KGP 2023].
15. **Exactly one easy coding question**: palindrome check [KGP 2023], basic recursion/DP [KGP 2022] — any language, LeetCode-easy.
16. D-FF Verilog with and without async reset; sync vs async reset trade-offs; SVA from a timing diagram [Glassdoor India].
17. Motivation inside the technical round: "why hardware over software?" [KGP 2022].

**Mechanics:** interviews on Google Meet, same day, ~30 min apart, both eliminative, purely technical, no HR round; you state a preferred HDL beforehand; code typed into a shared Google Doc; circuits drawn on paper and held up to the camera — **have blank paper + dark pen ready**.

## C. The universal list — asked at nearly *every* company in this pool

If time is short, be perfect on exactly these:
1. Setup/hold + max-frequency arithmetic ([TI] verbatim: t_cq=3, t_su=2, comb=5 → T=10 ns). File 03A.
2. "Hold violation — does slowing the clock help?" (No.) File 03.
3. CMOS inverter: draw, 4 terminals, VTC, noise margins, no-VDD case, body-not-tied-to-source case [QCOM verbatim]. File 06A.
4. CMOS NAND transistor-level; NAND vs NOR preference. Files 01A/06A.
5. Gates from muxes: AND (sel=A: 0/B), OR (sel=A: B/1), NOT (sel=A: 1/0), XOR (sel=A: B/B'); [TI] 3-input NAND from 2:1 muxes; [AMD] NAND from mux. 
6. Sequence-detector FSMs: 1011, 1010, 0110, three consecutive 1s — overlapping vs not [AMD/POOL]. File 01C.
7. Divisibility-by-3 serial FSM [POOL classic]. File 01C.
8. Blocking vs non-blocking + swap/shift-register probes [everyone]. File 02A.
9. 2-FF synchronizer: why 2 flops, why not for buses, MTBF shape [QCOM/POOL]. File 03B.
10. Async FIFO + gray pointers; **FIFO depth calc** ([NVDA] verbatim: "write 80 per 100 clocks in bursts of 20, read 8 per 10 clocks, burst 160 — depth?"). File 03C — method: max writes − min reads over worst-case window, check back-to-back bursts.
11. Divide-by-3 clock with 50% duty [NVDA verbatim: "bonus for 50% duty"]. File 01D.
12. Glitch-free clock mux [POOL whiteboard]. File 01D.
13. Latch inference from incomplete if/case — "what does this code synthesize to?" [INTC]. File 02A.
14. 5-stage pipeline: stages, latency vs throughput [INTC verbatim]; hazards + forwarding. File 04A.
15. Cache: write-through vs write-back, dirty bit, MESI awareness, tag/index/offset arithmetic [INTC/QCOM]. File 04B.
16. Stuck-at fault test vector: f = AB+CD, B stuck-at-1 → detect with A=1, B=0, CD such that output flips (CD≠11) [INTC]. File 05D.
17. Burning-ropes 45-min puzzle [QCOM verbatim on Glassdoor]. File 07A.

## D. Notable company-tagged extras (curated)

- [QCOM] FSM for pattern with wildcards (e.g. `01*0010*1`) — same suffix-prefix method, just messier. Practice one.
- [QCOM] "Write Verilog: clock that is off until 100 ns, then runs" → `initial begin clk=0; #100; forever #5 clk=~clk; end`.
- [QCOM] Function vs task; can a function call a task? (No.)
- [QCOM] Ring oscillator: odd inverter chain, f = 1/(2N·t_d).
- [TI] fork...join timing question — know that fork branches run concurrently; join waits for all.
- [TI] NAND with output fed back to one input, other input A → oscillates when A=1 (ring of 1 inverting stage), holds 1 when A=0.
- [TI] Johnson vs ring vs binary counter ranking by FF count and decode logic (file 01B).
- [AMD] "Given a chip doing 8-bit add/sub/mul/div with a stack: what edge cases would you test?" → overflow/underflow, divide-by-zero, stack over/underflow, sign cases — verification-plan thinking (file 05A).
- [AMD] "How would you verify an IP?" — use the 5-step answer in file 05A.
- [AAPL] Low-power implementation sweep: UPF, level shifters, isolation, retention, clock gating, power-on-reset sequence (file 06C).
- [INTC] "Explain the ASIC design flow" / "steps of synthesis" — flow recitation in file 03A vocabulary section.
- [NVDA] "Self-rate your Verilog," which flow stages you know — have honest, concrete self-ratings ready.

## E. Gap-fillers — topics the reports expose that files 01-07 don't cover

**BJT one-page refresh (Google round 1 went here!):** NPN/PNP structure; active/saturation/cutoff regions; I_C = β·I_B; common-emitter amplifier recognition (input at base, output at collector, gain = −g_m·R_C); current mirror recognition (two matched transistors, shared V_BE); differential pair recognition (tail current source + two symmetric branches). Be able to *identify* these three circuits from a schematic and state their function — that's the reported question. Also: why BJT for analog (transconductance) vs MOS for digital (density, zero gate current).

**Microprocessor/8085 debug scenario ("the lab setup doesn't work"):** answer as a systematic ladder — power/ground → clock present (scope it) → reset released → check ALE/control signals toggling → verify address/data bus activity → ROM chip-select and read timing → single-step/free-run mode → swap-known-good. The interviewer wants a *binary-search debug method*, not 8085 trivia. Same ladder works for "your FPGA board doesn't respond."

**DMA:** peripheral transfers data to/from memory without CPU per-word involvement; CPU sets up descriptor (src, dst, length), DMA controller masters the bus, interrupts on completion; why: CPU cycles + cache pollution. Bus arbitration awareness.

**Interrupts vs polling:** latency/efficiency trade-off; vectored interrupts; ISR basics.

**Serial protocols (one paragraph each — asked as warm-ups):**
- **UART**: async, start/stop bits, agreed baud; no clock wire.
- **SPI**: master-slave, 4 wires (SCLK/MOSI/MISO/CS), full-duplex, fast, no addressing (CS per slave); 3-wire variant shares data line.
- **I2C**: 2 wires (SDA/SCL), open-drain + pullups, addressed multi-drop, ACK per byte, ~100k-3.4 MHz; slower but pin-cheap.
- **AMBA awareness**: APB (simple, low-speed peripherals), AHB (single-channel pipelined), **AXI** (5 independent channels: AR, R, AW, W, B; ready/valid handshake on each; supports outstanding/out-of-order transactions). One-liner ready/valid rule: transfer happens on the cycle both are high.
- **DDR4 vs older DDR** (asked in Google-prep guides): higher density banks + bank groups, lower voltage (1.2 V), higher prefetch/speed bins; know DRAM basics from file 06D and that's enough.

**Number-systems speed drills (test staple):** 2's complement conversions, ranges (n-bit: −2^(n−1)…2^(n−1)−1), overflow detection rule (carry-in ≠ carry-out of MSB), hex↔binary↔decimal fast, BCD, sign extension.

## F. Transistor-level drawing drill (Google round 3 was entirely this!)

Practice drawing CMOS (pull-up PMOS network = complement/dual of pull-down NMOS network) for:
1. NAND2, NOR2, NOT (warm-up)
2. F = (AB + C)′  → NMOS: (A series B) parallel C to GND; PMOS dual: (A parallel B) series C to VDD.
3. F = (A(B + C))′ 
4. F = (AB + CD)′ — AOI22 [INTC verbatim]
5. XOR2 at transistor level (with and without transmission gates; know the TG version: 2 TGs + 2 inverters ≈ 6-8T)
6. 2:1 mux from transmission gates
7. D-latch from TGs + inverters; then master-slave D-FF
8. Tri-state inverter
Count transistors as a check: (AB+CD)′ = 8T. Rule to state: series in NMOS ↔ parallel in PMOS. If handed a non-inverting function, cascade an inverter (CMOS gates are naturally inverting).

## G. Peer-company test formats (calibration for the 1-hour test)

- Google (reported): ~25 MCQ / 30 min, mixed digital/CMOS/arch/RTL/Python.
- Qualcomm: 3 × 20 Q × 30 min (aptitude / C / digital), **negative marking** +1/−0.25.
- TI: aptitude 30 min + role test 45 min (latches, FFs, 8085, slacks, counters, mux FSMs, Verilog fork-join).
- AMD: logic design, C, aptitude, micro, Verilog sections.
Assume Google's 1-hour test = more of the reported pool; if there's negative marking it'll be stated — read the rules slide carefully.

## H. Two drill sets worth doing end-to-end

- The classic **Intel 101-question list** (asic-soc.blogspot.com 2007 post "Intel interview questions") — covers ~80% of the pool above.
- The **25-problem RTL practice list** at autonomousvision.io/rtl-practice — the modern screen-test set (FIFO, arbiters, gray code, UART, LRU, popcount, handshake downsizer).
