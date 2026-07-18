# 04 — Computer Architecture (JD minimum qualification: "computer hardware architecture")

Test questions here are GATE-style numericals (pipeline speedup, cache AMAT); interview questions are conceptual ("what happens on a branch?", "walk me through a load instruction").

---

## A. Pipelining

- Classic 5-stage RISC: **IF → ID → EX → MEM → WB**.
- Ideal speedup = number of stages; real speedup eaten by hazards + register overhead.
- **Hazards**:
  - *Structural*: two instructions need the same resource (e.g., single memory port) → stall or duplicate resource.
  - *Data*: RAW (true), WAR/WAW (name hazards — only matter with out-of-order/register renaming). Fixes: **forwarding/bypassing** (EX→EX, MEM→EX), stall for load-use (1 bubble even with forwarding — classic question: *why can't forwarding fix load-use?* Because the data isn't available until end of MEM).
  - *Control*: branches. Fixes: predict-not-taken, delay slots (historical), **branch prediction** (1-bit, 2-bit saturating counter — know the 2-bit state machine, it's also an FSM question!), BTB.
- **Numericals**:
  1. 5-stage, 20% loads, half followed by dependent instr (1 stall), 15% branches with 2-cycle penalty, 40% taken... CPI = 1 + stalls per instruction. E.g. CPI = 1 + 0.20·0.5·1 + 0.15·0.4·2 = 1 + 0.1 + 0.12 = **1.22**.
  2. Unpipelined 8 ns datapath vs 5 stages of 2 ns (incl. register overhead): speedup at steady state = 8/2 = **4×**, not 5 (overhead).
  3. **Amdahl**: speedup = 1 / ((1−f) + f/s). 40% of runtime sped up 2× → 1/(0.6+0.2) = **1.25×**. Know the limit: 1/(1−f).

---

## B. Memory hierarchy (the richest question source)

- **Why caches work**: temporal + spatial locality.
- **Mapping**: direct, k-way set-associative, fully associative. Address split = tag | index | offset.
  - Worked: 32 KB, 4-way, 64 B lines, 32-bit address → lines = 32K/64 = 512 → sets = 512/4 = 128 → offset 6 b, index 7 b, **tag 19 b**. Practice 3 variants of this until 2-minute-solvable.
- **Misses — the 3 Cs**: compulsory (first touch), capacity, conflict (fixed by associativity).
- **AMAT = hit time + miss rate × miss penalty.** Two-level: AMAT = t_L1 + m_L1 × (t_L2 + m_L2 × t_mem). Worked: 1 + 0.05×(10 + 0.2×100) = 1 + 0.05×30 = **2.5 cycles**.
- **Write policies**: write-through (+write buffer) vs write-back (dirty bit); write-allocate vs no-allocate (usual pairs: WB+allocate, WT+no-allocate).
- **Replacement**: LRU, pseudo-LRU, random. LRU hardware cost for high associativity.
- **Virtual memory**: page tables, TLB (a cache for translations), page fault ≫ cache miss cost, multi-level tables save space. VIPT L1 caches (index with page-offset bits) let TLB and tag lookup overlap — advanced, mention only if pushed.
- **Coherence (awareness)**: MESI states, snooping vs directory; false sharing. One-liner each is enough at intern level.
- **SRAM vs DRAM**: 6T cell vs 1T1C; DRAM needs refresh, destructive read; SRAM = caches, DRAM = main memory. DDR "double data rate" = both clock edges.

---

## C. Beyond the basics (awareness-level, 1-2 sentences each)

- **Superscalar**: multiple issue per cycle. **Out-of-order**: rename (fixes WAR/WAW), issue when operands ready, retire in order (ROB) for precise exceptions.
- **Speculation**: predict + squash on mispredict.
- **Multithreading**: SMT shares core resources across threads to hide latency.
- **ISA vs microarchitecture**: contract vs implementation; RISC vs CISC; ARM vs x86; RISC-V = open ISA (nice to mention for silicon-team relevance).
- **Accelerators & domain-specific architectures**: why general-purpose scaling slowed (end of Dennard scaling) → TPUs/NPUs: systolic arrays doing dense matrix multiply with massively parallel MACs, trading generality for TOPS/W. **This is Google's home turf — being able to say "a TPU is a systolic-array matrix engine; the win is data reuse: weights/activations flow through a MAC grid so you amortize memory access over many ops" is a strong moment.**
- **Memory bandwidth vs compute (roofline)**: ML workloads are often memory-bound; HBM, quantization (INT8) reduce pressure. One sentence each.

---

## D. Interview walk-throughs to rehearse aloud

1. **"What happens when a load executes?"** IF/ID → address calc in EX → TLB lookup (hit/miss) → L1 lookup (hit/miss → L2 → memory) → writeback; mention alignment and load-use stall.
2. **"What happens on a taken branch?"** Fetch redirect, flush wrong-path bubbles = misprediction penalty; predictor + BTB reduce it.
3. **"Why does adding pipeline stages stop helping?"** Register overhead per stage, more hazard penalty (deeper flush), diminishing t_comb per stage.
4. **"How would you speed up a program that misses cache constantly?"** Blocking/tiling, prefetching (HW/SW), better layout (AoS→SoA), bigger/more associative cache — show you connect HW and SW.

---

## E. Rapid-fire
1. Load-use hazard needs how many stalls with full forwarding? — *1.*
2. 2-bit predictor beats 1-bit because…? — *Tolerates one anomaly (e.g., loop exit) without flipping prediction.*
3. Bigger cache line: pro/con? — *Exploits spatial locality / raises miss penalty & possible pollution; conflict for fixed size.*
4. TLB miss = page fault? — *No — TLB miss is a translation-cache miss (walk the page table, maybe microseconds→ns); page fault means page not in DRAM (OS + disk, ms).*
5. Precise exception meaning? — *All older instructions committed, no younger ones — enabled by in-order retire/ROB.*
6. Why is DRAM row buffer hit faster? — *Row already sensed/open; only column access needed.*
7. Harvard vs von Neumann? — *Separate I/D paths vs unified; modern L1s are Harvard-style, unified below.*
8. What limits single-thread performance today? — *Power density (Dennard scaling end), memory latency, ILP limits → multicore + accelerators.*

Numericals source: GATE CS/EC previous-year "CO & Architecture" sets — same difficulty band as the test.
