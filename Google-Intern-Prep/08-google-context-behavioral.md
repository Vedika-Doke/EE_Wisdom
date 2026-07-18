# 08 — Google Silicon Context + Behavioral Prep

Knowing what Google's silicon teams actually build turns "why Google hardware?" from a generic answer into a specific one — and gives you vocabulary interviewers recognize. (Facts current as of July 2026; items marked *reported* are press leaks, not official.)

---

## A. Google's chip portfolio — the 2-minute map

**Pixel / consumer side:**
- **Tensor G5** — current Pixel 10 SoC. Landmark chip: first Tensor on **TSMC 3nm** (previous gens were Samsung-fabbed), first fully in-house design: 1× Cortex-X4 + 5× A725 + 2× A520, Imagination GPU, **4th-gen on-device TPU** (runs Gemini Nano locally), first fully Google-designed ISP.
- **Tensor G6** — expected with Pixel 11 (~Aug 2026, *reported*): first phone chip on **TSMC 2nm**, Titan M3 security chip, MediaTek modem; focus on efficiency + on-device AI.
- **Titan M2** — Pixel's security chip: a Google-designed **RISC-V** hardware root of trust isolating keys/biometrics/boot.

**Cloud / datacenter side (what the intern JD centers on — "TPUs, Arm-based servers, and network products"):**
- **Ironwood (TPU v7)** — current Cloud TPU, GA; pitched as "the first TPU for the age of inference." ~4.6 PFLOPS FP8, 192 GB HBM3e, 7.37 TB/s BW per chip; scales to **9,216-chip superpods (~42.5 exaFLOPS)** via ICI + optical circuit switching. Predecessor: **Trillium (v6e)**. 8th gen *reportedly* previewed as a training/inference split ("Sunfish"/"Zebrafish", ~2nm, late 2027).
- **Axion** — Google's Arm server CPU (**Neoverse V2**): claimed up to 50% better perf / 60% better efficiency vs comparable x86; the host-CPU half of the AI-infra story, supported by **Titanium** offload silicon (networking/storage/security off the main CPU).
- **Titan / OpenTitan** — datacenter root-of-trust chip on every motherboard; OpenTitan = first commercial open-source security chip.
- **Argos VCU** — YouTube's video-transcode ASIC, 20-33× more efficient than CPUs. **Best example to cite of Google's philosophy: find a warehouse-scale workload, co-design silicon for it.**

**India angle (worth mentioning — you'd be joining these teams):**
- Bengaluru "gChips" site built from 2021 with senior hires from Intel/Qualcomm/Broadcom/Nvidia; today it owns real blocks: **Cloud TPU RTL, DV and physical design + Consumer (Tensor) IP** — imaging, ML, video, security.
- Hyderabad: Google's **largest campus outside the US** (Gachibowli, ~3M sq ft) coming online 2026; silicon roles listed there.
- Pixel phones are now manufactured in India (Dixon/Foxconn) — India touches Google hardware from RTL to retail.

## B. How Google evaluates you — the four axes

Every interviewer scores the same four attributes; feedback goes to a hiring committee.
1. **GCA (General Cognitive Ability)** — structured reasoning on ambiguous problems. Behavior: clarify → decompose → state assumptions → explore trade-offs → sanity-check. Think out loud, always.
2. **RRK (Role-Related Knowledge)** — everything in files 01-07.
3. **Leadership** — "emergent leadership": stepping up without a title. Have one story where you took ownership in a project/club/course.
4. **Googleyness** — comfort with ambiguity, bias to action, intellectual humility, taking feedback well, collaboration.

Intern format is typically two ~45-60 min technical interviews with some behavioral mixed in — matching this posting's stated process exactly.

## C. Behavioral prep — do this once, reuse everywhere

Write out **four STAR stories** (Situation-Task-Action-Result, ~90 seconds each) from your projects/PORs/coursework:
1. An **ambiguous problem** you structured and solved (GCA story).
2. A **team conflict or setback** and how you handled it.
3. A time you **led/owned something end-to-end** without being asked (emergent leadership).
4. A time you **got hard feedback or were wrong**, and what you changed.

Also prepare, in 60-90 seconds each:
- **"Walk me through your resume / favorite project"** — for each project on your resume, be ready for 10 minutes of *deep* technical follow-ups; this is often the biggest chunk of interview 1. Rehearse: what was hard, what you'd do differently, what broke, how you verified it, what you measured.
- **"Why hardware?" / "Why Google?"** — connect your interests to specifics from section A ("the Argos/TPU workload-first philosophy", "Tensor going fully in-house", "Bengaluru owning TPU DV blocks"). Specific beats enthusiastic.

## D. Questions to ask your interviewers (pick 2-3)

1. "Does this team sit on Cloud silicon (TPU/Axion) or Consumer Hardware Silicon (Tensor IP), and which blocks does the Bengaluru/Hyderabad site own end-to-end?"
2. "Ironwood was framed as the TPU for the age of inference — how does inference-first change microarchitecture and verification priorities vs training chips?"
3. "How do intern projects get scoped — would I own a deliverable that feeds a real tape-out in 12 weeks?"
4. "How closely do RTL teams co-design with the compiler/software side (XLA, camera/Android) — do designers see workload profiles directly?"
5. "How is Google using ML inside the silicon flow itself — placement (AlphaChip), coverage closure, debug triage?"
6. "What does the DV stack look like here — UVM, formal, emulation — and where do interns usually plug in?"

## E. Ten talking points to have loaded

1. Tensor G5 = first fully in-house Google SoC, TSMC 3nm, in-house ISP, 4th-gen edge TPU.
2. G6 *reportedly* first phone SoC on 2nm — Google is pushing leading nodes now.
3. Ironwood = current Cloud TPU, inference-first, 9k-chip pods at exaFLOP scale.
4. Axion (Neoverse V2) + Titanium = the Arm server story the intern JD names.
5. Titan/Titan M2/OpenTitan = security silicon stack, RISC-V based.
6. Argos VCU = workload-first ASIC philosophy — my go-to "why custom silicon" example.
7. TPU = systolic array: data reuse across a MAC grid amortizes memory access — why it wins on TOPS/W (ties to file 04's accelerator section).
8. India site = first-class: TPU RTL/DV/PD + Tensor IP owned from Bengaluru/Hyderabad.
9. The four evaluation axes and STAR stories mapped to them.
10. PPA language: frame every design answer as a performance/power/area/schedule trade-off — the JD literally says interns "balance performance, power, features, schedule, and cost."
