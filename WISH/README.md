# WISH — TI WiSH Hardware Summer School (2026)

Coursework, labs, and the group project from the **WiSH (Winter/Summer school on Hardware)** program at IIT Bombay — organised as an **academic-help resource** for anyone taking the program or learning the same topics.

Everything here is meant to be *read to learn from*: Verilog labs come with testbenches and test plans, the analog and digital weeks keep the prework and daily quizzes, and the group project ships its full server-side design writeups (not just slides).

> Course-provided material that is large or purely reference (30 MB master PDF, full slide decks, heavy presentation exports) is intentionally left out to keep the repo lean. What remains is the hands-on work and the compact reference PDFs.

---

## Layout

| Folder | What's inside |
|---|---|
| [`Week1-Digital-Design/`](Week1-Digital-Design/) | Digital week — RTL/DV/DFT. Two Verilog labs (shift-add & DFT-instrumented sequential multipliers) with testbenches, test plans, and waveforms; prework guides and daily surveys. |
| [`Week2-Analog/`](Week2-Analog/) | Analog week — prework, the ASLKv2010 starter-kit reference, and topic quizzes (Fourier/Laplace, inductors, RC circuits). |
| [`Project-Smart-Bus-Tracking/`](Project-Smart-Bus-Tracking/) | Group software project — a rural smart-bus tracking + ETA system, with the full server-side math, prediction engine prototype, and design docs. |

---

## Week 1 — Digital Design

Hands-on RTL for a **sequential shift-add multiplier**, then a **DFT (Design-for-Test) instrumented** version.

### `Day2-ShiftAdd-Multiplier/`
- Verilog datapath + control: `seq_multiplier.v`, `controller.v`, `bit_counter.v`, `mult_regs.v`, `add_decision.v`
- Testbenches (`tb_seq_multiplier.v`, `tb_add_decision.v`) plus an SV verification plan under `test_bench/`
- `test_plan_add_decision.md` — written test plan for the add-decision block
- `gtkwave.png`, `terminal.png` — simulation waveform + run output (proof of working sim)

### `Day4-DFT-Multiplier/`
- `Final/` — the multiplier re-worked with DFT hooks: integrated clock gating (`ICG.v`) and a DFT testbench (`tb_seq_multiplier_dft.v`)
- `Lab Question.docx` / `Lab Solution.docx` — the lab prompt and worked solution

### `prework/` and `quizzes/`
- Prework: WiSH Hardware Digital prework + a System Architecture guide
- Quizzes: Digital Day 2–5 surveys

> **Reproduce the sim** (Icarus Verilog):
> ```sh
> iverilog -o sim tb_seq_multiplier.v seq_multiplier.v controller.v bit_counter.v mult_regs.v add_decision.v
> vvp sim && gtkwave *.vcd
> ```

---

## Week 2 — Analog

Prework and reference for the analog week, built around the TI **ASLKv2010** Analog System Lab Kit.

- `WiSH-Analog-2026-PreWork.pdf` — analog prework
- `aslkv2010-starter-kit.pdf` — starter-kit reference
- `quizzes/` — Fourier & Laplace transforms, inductors, voltage sources & RC circuits

---

## Project — Smart Bus Tracking

A low-connectivity **bus tracking + ETA prediction** system for rural routes. The bus sends GPS pings; a server figures out where the bus really is, when it reaches each stop, and whether it has gone off-route — then falls back to SMS/IVR when data is unavailable.

- **Design docs** — `Problem_Statement.pdf`, `DesignThinking.pdf`
- **`server/basic/`** — the maths, explained end-to-end:
  - `Server_Side_Math_Explained.md` — ETA, dead-reckoning and geofencing worked on one running example (Route 5)
  - `Person2_ETA_Prediction_Research.md`, `Projection_Method_Writeup.md` — the prediction/projection method
  - `diagrams/` — ETA workflow, prediction workflow, geofence equations, divert-alert pipeline (`.mmd` / `.drawio` sources + `.png`/`.pdf` renders)
- **`server/prototype/`** — runnable Python engine:
  - `eta_engine.py` — EWMA segment learning, snap-to-route, dead reckoning, geofence tracker
  - `demo.py` — three scenarios: normal trip, network blackout, off-route divert
  - `*.html` — story / hardware / simulator / end-to-end walkthrough pages
- `IJIRT162121_PAPER.pdf` — reference paper

> **Run the ETA demo:**
> ```sh
> cd server/prototype && python3 demo.py
> ```

---

*Part of [EE_Wisdom](../README.md) — academic projects and study material by Vedika Doke.*
