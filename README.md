# EE_Wisdom

**Engineering Projects & Research Repository**

Academic projects, hands-on engineering work, and interactive study tools spanning electrical engineering, embedded systems, digital design, and hardware development — built over multiple roles and coursework at IIT Bombay.

Each project lives in its own folder on `main`. Interactive study guides are served via GitHub Pages from the [`EE_LearningAids`](https://github.com/Vedika-Doke/EE_Wisdom/tree/EE_LearningAids) branch.

---

## Repository Structure

| Folder / Branch | Contents |
|---|---|
| [`EE309-Microprocessors-Project/`](EE309-Microprocessors-Project/) | AT89C5131A + MCP4921 DAC + MCP3008 ADC — sine wave generation, NCO, Goertzel tone detection |
| [`Electrical-trainee-racing-team/`](Electrical-trainee-racing-team/) | ECU firmware, AMS/BMS research, HV-LV isolation, and comms protocol work for a student electric racing team |
| [`JDE-Tenure/`](JDE-Tenure/) | Two complete KiCad PCB designs — STM32 dashboard board and a sensor testing board |
| [`ToyCPU-1281/`](ToyCPU-1281/) | VHDL-based 8-bit toy CPU (i281) with custom ISA and Bubble Sort implementation |
| [`EE_LearningAids`](https://github.com/Vedika-Doke/EE_Wisdom/tree/EE_LearningAids) (branch) | Interactive browser-based study guides — [live site](https://vedika-doke.github.io/EE_Wisdom/) |

---

## EE_LearningAids

Interactive, single-file HTML study tools — open directly in any browser, no setup or internet needed.

### IIT Bombay Course Guides

#### EE238 — Power Engineering II
- 10 topic modules: transmission lines, 3-phase systems, per-unit analysis, ABCD parameters, P & Q flow, Maxwell's equations
- 18+ MCQs with click-to-reveal solutions
- 6 long-form worked problems (Review Q 5A style) with step-by-step strategies
- Textbook examples from Weedy Ch 2–3, master formula sheet
- Dark theme, mobile-friendly, works offline

#### EE309 — Microprocessors & Computer Architecture
- Full 8051 coverage: Von Neumann vs Harvard, memory map (ROM/RAM/SFR), internal RAM breakdown
- Registers, PSW flags, 5 addressing modes, instruction set (arithmetic, logical, jump, bit-level)
- Timers, interrupts (IE/IP registers, interrupt priority), serial communication (SCON, SBUF, baud rate)
- Machine cycles, timing diagrams, and common exam traps

### Aptitude & Test Prep (TI WiSH 2025)

#### `wish_prep.html` — TI WiSH Interactive Quiz
- Sections: Past Year Questions (WiSH 2024), Mathematics, Basic Electronics, Programming (tricky C)
- Full Mixed Mock Test mode (timed, randomized)
- Click-to-reveal explanations with worked solutions
- Covers: LTI systems, ROC, op-amps (CMRR, slew rate, virtual ground), MOSFET regions, CMOS, Boolean algebra, pointer arithmetic, short overflow, recursion output

#### `math_aptitude_mcq.html` — Advanced Math Bank
- 20 questions: Bayes' Theorem, Derangements, Pigeonhole Principle, Inclusion-Exclusion, Modular Arithmetic, Eigenvalues, Geometric Probability

#### `theory_revision.html` — Theory Revision Sheet
- **Probability:** Addition/Multiplication/Bayes rules, nPr/nCr, circular permutations, Stars & Bars, distributions (Binomial, Geometric, Poisson, Uniform), Expected Value, Variance, Derangements
- **Profit & Loss:** CP/SP/Profit%/Loss%, Mark-up + Discount formula, successive discounts, dishonest dealer, "sell x for cost of y" — with fraction↔percentage cheatsheet
- **C++ FAQs:** Data types & overflow, pointer arithmetic traps, operator precedence, bitwise tricks, storage classes, struct vs union vs class, virtual functions, time complexity table
- 45+ new interactive MCQs across all three topics with explanations

---

## Electrical-trainee-racing-team

Work done as an Electrical Trainee for a student FSAE electric racing team.

### `Comms/` — Controls, Communication & Data Acquisition

**ECU Firmware** (`Comms/Src/ECU/`) — C++ / PlatformIO
- FSAE rulebook-compliant ECU (Rules T11.8.x)
- CAN bus communication at 500 kbps; DBC file included for signal decoding
- APPS (Accelerator Pedal Position Sensor) plausibility check — 10% agreement threshold (T11.8.4)
- BPS (Brake Pressure Sensor) monitoring with hard-braking detection (T11.8.6)
- Torque command pipeline: APPS → scaled torque request → CAN frame to motor controller
- RFE acknowledgement sequencing, motor off-sequence with timed delays
- I2C/SPI/UART protocol implementations (PlatformIO project under `Comms/PlatformIO/`)

**Theory Notes** (`Comms/Theory/`)
- Serial vs parallel communication, simplex/half-duplex/full-duplex
- Deep dive into CAN, I2C, SPI, UART — framing, arbitration, addressing, error handling

### `AMS/` — Accumulator Management System

- BMS fundamentals: SOC (State of Charge), SOH (State of Health), SOE (State of Envelope)
- 96s5p Li-ion pack configuration (480 cells, max 403.2 V), stack architecture
- BMS protection: overvoltage, overcurrent, thermal cutoff
- `DataAnalysis.ipynb` — Python notebook for crude data analysis of accumulator telemetry
- PDF module notes: `AMS module '25.pdf`

### `HV_LV/` — High Voltage / Low Voltage Systems

- Galvanic isolation — theory and FSAE rulebook requirements (EV 1.2.1)
- Optocoupler interfacing (datasheet included)
- 555-timer theory notes
- ESI (Electronic System Integration) module notes — noise rejection, fault containment, ground loops

### `TheoryBits/` — Reference PDFs

Quick-reference theory sheets produced during the tenure:
- MOSFETs, Op-Amps & Phasors, Op-Amp 2.0, Buck Converter
- Communication Protocols, Digital Logic
- Team session notes (Racing Team April 4)

---

## JDE-Tenure

Two complete PCB designs, both fully designed in KiCad (schematic + layout + BOM + 3D model).

### `DASH_PCB_STM32/` — Dashboard PCB (STM32-based)

- STM32 microcontroller at the core
- Full KiCad project: schematic (`.kicad_sch`), PCB layout (`.kicad_pcb`), project file, net files, BOM (`.csv`), 3D model (`.step`)
- Multiple netlist iterations (`net.net` → `netfinal.net`) reflecting design revisions
- Design backups preserved (Nov 2025)

### `Sensor_testing_PCB/` — Sensor Testing Board

- Purpose-built board for validating sensor interfacing before integration
- Full KiCad project: schematic, PCB layout, BOM, net file
- Backup snapshots included (Sep 2025)

---

## ToyCPU-1281

Course project: design and implementation of an 8-bit toy CPU (i281).

- VHDL-based datapath and control unit
- Custom instruction set architecture (ISA)
- Bubble Sort implemented directly on memory data
- Functional simulation and verification

---

## Tools & Technologies

| Category | Details |
|---|---|
| **Languages** | C++, Python, VHDL, HTML/JS |
| **Embedded** | PlatformIO, 8051, STM32, CAN bus |
| **PCB Design** | KiCad (schematic, layout, BOM, 3D) |
| **EDA / Simulation** | LTspice, Quartus, MATLAB/Simulink |
| **Protocols** | CAN (500 kbps), I2C, SPI, UART |
| **Domains** | Embedded systems, automotive electronics, digital design, power engineering, test prep tooling |

---

## EE309-Microprocessors-Project

Hardware project for EE309 (Microprocessors) — AT89C5131A microcontroller driving external DAC/ADC over SPI.

| Phase | Description |
|---|---|
| `phase1/` | Basic MCP4921 DAC sine wave output via lookup table |
| `phase2/` | NCO (Numerically Controlled Oscillator) — frequency-agile sine generation |
| `phase3/` | MCP3008 ADC integration — loopback DAC→ADC measurement |
| `phase3_goertzel/` | Goertzel algorithm for single-bin tone detection on ADC samples |
| `src/` | Latest working source files |

---

## Notes

- All HTML study tools are offline-capable — open in a browser or visit the [live site](https://vedika-doke.github.io/EE_Wisdom/).
- Study guides are served from the `EE_LearningAids` branch via GitHub Pages.
