# 03 — Static Timing Analysis & Clock Domain Crossing (JD: CDC, synthesis, STA)

These produce the most *numerical* interview questions. Master the two equations and the FIFO-depth calculation and you'll handle 90% of what's asked.

---

## A. The two timing equations

For a path FF1 → combinational logic → FF2, clock period T, skew s = (clock arrival at FF2) − (arrival at FF1):

- **Setup (max-delay) check:** `t_cq + t_comb(max) + t_setup ≤ T + s`
  → `f_max = 1 / (t_cq + t_comb + t_setup − s)`
- **Hold (min-delay) check:** `t_cq + t_comb(min) ≥ t_hold + s`

Things interviewers fish for:
- **Positive skew helps setup, hurts hold. Negative skew does the opposite.**
- **Hold violations are frequency-independent** — slowing the clock never fixes hold. Fix = add delay (buffers) in the data path. Setup fix = reduce logic depth / pipeline / slow the clock / upsize cells.
- **Jitter/uncertainty** effectively shrinks T for setup.
- A hold violation = silicon is dead at any frequency → why hold is checked so paranoidly at signoff.
- Setup checked at *next* edge, hold at the *same* edge.

### Worked numericals
1. t_cq = 0.5 ns, t_comb = 3.2 ns, t_setup = 0.3 ns, skew 0. **f_max?** T ≥ 4.0 ns → **250 MHz**.
2. Same path, skew at capture FF = +0.4 ns. **f_max?** T ≥ 4.0 − 0.4 = 3.6 ns → **277 MHz**. Hold check: t_cq + t_comb,min ≥ t_hold + 0.4 — skew ate into hold margin.
3. t_cq = 1 ns, t_hold = 1.5 ns, t_comb,min = 0.2 ns. Hold met? 1 + 0.2 = 1.2 < 1.5 → **violated**; insert ≥ 0.3 ns of buffer delay.
4. Pipeline a 12 ns combinational block into 3 balanced stages (t_cq 0.5, t_setup 0.5). f before: 1/13 ≈ 77 MHz. After: stage = 4+1 = 5 ns → **200 MHz**, latency 3 cycles. This is the "why pipeline?" question with numbers.
5. Time borrowing: latches (level-sensitive) let a long path borrow into the transparent phase — mention-level only.

### Vocabulary to recognize (synthesis/STA flow — JD keywords)
- **False path**: physically present, never functionally exercised (e.g., between async domains, or config-mutex paths) — `set_false_path`.
- **Multicycle path**: data allowed N cycles — `set_multicycle_path N` (and remember hold moves too).
- **Recovery/removal**: setup/hold analog for async reset deassertion vs clock.
- **WNS/TNS**: worst/total negative slack. Slack = required − arrival.
- **OCV/derating, corners (SS/TT/FF, PVT)**: timing signed off across process/voltage/temperature corners.
- Flow order: RTL → lint → synthesis (netlist) → **formal equivalence** (LEC: RTL vs netlist) → place & route → STA signoff. Being able to recite this flow answers "what happens after RTL?"

---

## B. Metastability & CDC

**Metastability**: capture a signal violating setup/hold → FF output floats at an intermediate level for an unbounded (probabilistically decaying) time. Cannot be prevented for async inputs — only *contained*.

**MTBF ≈ e^(t_r/τ) / (T_w · f_clk · f_data)** — resolution time t_r in the exponent is why adding a second FF (a full extra cycle to resolve) raises MTBF from hours to centuries. You don't need the formula's constants, just the shape: exponential in settling time, linear in both frequencies.

### The toolbox (know when each applies)
| Crossing | Solution |
|---|---|
| Single bit, level | **2-FF synchronizer** (3-FF at very high clock ratios) |
| Single-cycle pulse, fast→slow | Pulse would be missed → **toggle synchronizer**: toggle a level on each pulse, synchronize, edge-detect in destination |
| Multi-bit counter/pointer | **Gray code** the counter, then 2-FF sync — only 1 bit changes per step so sampled value is always either old or new, never garbage |
| Multi-bit data bus | Never sync bits independently (skew ⇒ torn word). Use **handshake** (req/ack, data held stable) or **async FIFO**; qualifier + mux-recirculation (sync the 1-bit valid, use it to load the stable bus) |
| Reset into a domain | **Reset synchronizer** — assert async, deassert sync |

**Why you can't just 2-FF a bus** — say it precisely: each bit's synchronizer resolves independently; with skew/metastability different bits take old vs new values → a transient bogus word. Gray coding makes "old or new" the only possibilities *for unit-increment values*.

**Async FIFO in 60 seconds** (say this fluently): dual-port RAM; write pointer in write domain, read pointer in read domain, both **gray-coded**; each pointer 2-FF-synchronized into the other domain; `empty` computed in read domain (sync'd wptr == rptr), `full` in write domain (sync'd rptr vs wptr with MSB trick). Synchronization delay makes flags **pessimistic (conservative), never wrong** — FIFO may look full/empty slightly late, which is safe.

**CDC verification** (JD keyword): structural CDC lint tools (Spyglass CDC / Questa CDC) find unsynchronized crossings, convergence of synchronized signals, glitchable crossing logic (a combinational glitch into a synchronizer can be captured — crossings must come straight from a FF).

---

## C. FIFO depth calculation (very frequently asked — practice until 5-minute-solvable)

Template: `depth ≥ max backlog = (items written − items read) over the worst-case burst window`.

**Worked example 1 (classic):** Write: burst of B = 120 words at f_w = 100 MHz (one word/cycle). Read: f_r = 80 MHz, one word/cycle continuous.
Burst duration = 120 × 10 ns = 1200 ns. Words read in that window = 1200 ns × 80 MHz = 96. **Depth ≥ 120 − 96 = 24.**

**Worked example 2 (duty-cycled read):** Writes 80 words per 100-cycle frame @200 MHz; reader drains 1 word per 2 cycles @200 MHz continuously (50 words per frame window is wrong — compute over the burst): burst = 80 cycles = 400 ns; read during burst = 80/2 = 40. Backlog = 40. Check steady-state: per frame, writes 80, reads 50 → **unsustainable, FIFO overflows for any finite depth** — the right answer is "no depth suffices; flow control needed." (Interviewers love planting an infeasible-rate case. Always sanity-check average rates first: avg write ≤ avg read must hold.)

**Worked example 3 (jittered/back-to-back bursts):** worst case is burst at end of window + burst at start of next → effective burst 2B with one idle gap's worth of reads. If asked, construct worst case explicitly rather than plugging a formula — that's what they're testing.

Rules of thumb to state: round up to power of 2; add margin for synchronizer latency in async FIFOs (a few entries per direction).

---

## D. Rapid-fire Q&A

1. Can a hold violation be fixed by lowering frequency? — *No; add data-path delay.*
2. Why 2 FFs and not 1 in a synchronizer? — *First may go metastable; second samples after ~1 period of resolution → exponentially better MTBF.*
3. Does a synchronizer remove metastability? — *No; it gives it time to resolve and isolates it. Probability, not certainty.*
4. What is slack? — *Required time − arrival time; negative = violation.*
5. Max frequency of a chip is set by? — *The critical (worst setup-slack) register-to-register path.*
6. Clock skew intentionally added to fix setup? — *Useful skew.*
7. What's a lockup latch? — *Latch inserted between scan chains in different clock domains to make hold timing safe during shift (DFT tie-in, file 05).*
8. Why is the async-FIFO full flag "conservative"? — *It uses a delayed (synchronized) copy of the other pointer, so it errs toward asserting; never lies dangerously.*
9. What breaks if a gray-coded pointer increments by 2 in one clock? — *Two bits change → torn sample possible; gray safety assumes unit increments (e.g., careful with "write 2 per cycle" FIFOs).*
10. Setup slack +0.1 ns at SS corner, hold slack −0.05 ns at FF corner — ship it? — *No: hold fails at fast corner; fix holds across all corners.*
