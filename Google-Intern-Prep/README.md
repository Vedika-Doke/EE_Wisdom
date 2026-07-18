# Google Silicon Engineering Intern — Prep Package

Prep for the Google Hardware/Silicon Engineering Intern campus selection (IIT Bombay):
**1-hr technical test → resume + CGPA filter → two 1-hr eliminative technical interviews.**

Built July 2026 from first-person IIT KGP/IITB/IITR selectee accounts, reported question banks
(Google + Qualcomm/Nvidia/TI/AMD/Intel), and Google silicon-org research.

## Reading order

| File | What it covers |
|---|---|
| [00 — Game Plan](00-GAME-PLAN.md) | **Start here.** What each stage really is, 4-week & 1-week schedules, resources, day-before checklist |
| [01 — Digital Design](01-digital-design.md) | Combinational, sequential, FSMs, classic circuit questions — biggest test+interview weight |
| [02 — Verilog / SystemVerilog](02-verilog-systemverilog.md) | Blocking/non-blocking, latch inference, memorize-these templates, FIFO code, SVA |
| [03 — STA & CDC](03-sta-cdc.md) | Setup/hold equations, synchronizers, async FIFO, FIFO-depth calculation — the #1 flagged interview-2 topic |
| [04 — Computer Architecture](04-computer-architecture.md) | Pipeline hazards, caches/AMAT, virtual memory, TPU/accelerator talking points |
| [05 — Verification & DFT](05-verification-dft.md) | The 5-step "how would you verify X" answer, assertions, scan/ATPG/BIST |
| [06 — CMOS & Low Power](06-cmos-low-power.md) | Inverter/VTC, power equation, low-power technique menu, SRAM/DRAM |
| [07 — Puzzles, Scripting, Bits](07-puzzles-scripting-bits.md) | GCA puzzles, probability, Python log-parsing, C bit tricks |
| [08 — Google Context & Behavioral](08-google-context-behavioral.md) | Tensor/TPU/Axion portfolio, Google's 4 scoring axes, STAR stories, questions to ask |
| [09 — Mock Test](09-mock-test.md) | Timed 1-hour simulation with answer key and scoring guide |
| [10 — Reported Questions Bank](10-reported-questions-bank.md) | Every actually-reported question (tagged by company), IIT-campus specifics, gap-fillers |

## The 30-second version

- The test is **MCQ + predict-the-output code**, GATE difficulty, speed-limited.
- Interview 1 = **digital + Verilog** (FSMs, latch vs FF, build-X-from-Y, blocking/non-blocking).
- Interview 2 = **STA + CMOS + architecture + your CV project + one easy coding question**.
- Code goes in a shared Google Doc; circuits drawn on paper to camera. **Think aloud — approach is graded over answers.**
