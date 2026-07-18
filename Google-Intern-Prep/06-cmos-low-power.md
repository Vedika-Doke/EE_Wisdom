# 06 — CMOS Basics & Low-Power Design (JD: "low power reduction techniques")

Test-level EE fundamentals plus the low-power vocabulary the JD explicitly names. A mobile-SoC company (Tensor) *will* touch power in interviews.

---

## A. MOSFET & inverter essentials

- **Operating regions** (NMOS): cutoff (V_GS < V_T), triode/linear (V_DS < V_GS − V_T), saturation (V_DS ≥ V_GS − V_T), with I_D,sat = ½ μCox (W/L)(V_GS − V_T)² (long-channel).
- **CMOS inverter VTC**: know the shape, switching threshold V_M (where both saturated), and that gain is highest in the transition region. Noise margins: NM_H = V_OH − V_IH, NM_L = V_IL − V_OL.
- **Sizing**: PMOS 2-3× wider than NMOS to balance rise/fall (mobility ratio). For NAND: series NMOS upsized; NOR's series PMOS is why **NAND > NOR** in CMOS.
- **Pass transistor**: NMOS passes strong 0, weak 1 (drops V_T); PMOS the reverse → **transmission gate** (both) passes full rail. TG mux = compact 2:1 mux.
- **Delay intuition**: t ∝ C_load·V/I_drive; bigger W → more drive but more input C — hence **logical effort / buffer sizing**: driving a big load uses a chain of exponentially larger inverters (optimal fanout ≈ 4). "How do you drive a 1 pF clock net?" → buffer tree, fanout ~4 per stage.
- **Latch-up**: parasitic PNPN (SCR) between wells fires and shorts rails → guard rings, well taps. One-liner suffices.
- **Body effect**: source-body reverse bias raises V_T. Used deliberately in back-biasing.

## B. Power: the three components (memorize the equation)

**P_total = α·C·V²·f  (dynamic switching) + P_short-circuit + V·I_leak (static)**

- **Dynamic**: activity factor α, switched capacitance C, supply V (quadratic!), frequency f.
- **Short-circuit**: both transistors momentarily on during slow input transitions — kept small with sharp edges.
- **Static/leakage**: subthreshold (exponential in −V_T — why low-V_T cells leak), gate leakage. Dominates in idle modes; grows worse with scaling.

Interview favorite: **"Why does lowering V_DD help so much?"** — quadratic dynamic savings, plus lower leakage; cost = slower transistors (delay rises as V_DD → V_T) → the V/f trade-off behind DVFS.

## C. The low-power technique menu (JD keyword — know all of these by name + one sentence)

| Technique | What | Attacks | Cost |
|---|---|---|---|
| **Clock gating** | ICG cells stop the clock to idle registers | Dynamic (clock tree is ~30-40% of dynamic power) | Gating logic, verification of enables |
| **Power gating** | Header/footer switches cut V_DD to idle blocks | Leakage | State loss → retention FFs; wake-up latency; rush current |
| **Multi-Vt** | High-Vt cells off critical path, low-Vt on it | Leakage | Extra masks; timing closure complexity |
| **Multi-voltage domains** | Different blocks at different V_DD | Dynamic | **Level shifters** at crossings |
| **DVFS** | Scale V and f to workload at runtime | Dynamic (V² · f ≈ cubic) | PMIC, characterized operating points, control loop |
| **Operand isolation** | Gate inputs of idle datapath (stop useless toggling) | Dynamic | Small logic |
| **Memory techniques** | Bank gating, retention voltage, cache way shutdown | Both | Complexity |
| **Architecture level** | Accelerators (do it in fewer ops), quantization, near-memory compute | Everything | Design effort — *this is literally why TPUs exist; connect it!* |

**UPF/power-intent awareness**: power domains described in UPF; **isolation cells** clamp outputs of off domains (else floating X's), **retention registers** keep state, **level shifters** cross voltage domains, **always-on** logic for wake-up. Verification must simulate power states (x-corruption on power-down).

## D. SRAM/DRAM cell basics (asked as EE fundamentals)

- **6T SRAM**: cross-coupled inverters + 2 access NMOS; read stability vs write margin tension; differential bitlines + sense amp.
- **DRAM 1T1C**: charge on cap, destructive read, refresh (~64 ms); density champion.
- Why SRAM for caches (speed, logic-compatible) and DRAM for main memory (density, cost).

## E. Rapid-fire

1. Power dissipated when a CMOS gate is *not* switching? — *Leakage only (ideally zero dynamic) — CMOS's founding advantage.*
2. Energy per switching event? — *½CV² per transition (CV² per full charge-discharge cycle from the supply's perspective… safest: E = αCV² per cycle framing as in P = αCV²f).*
3. Double the frequency — what happens to power and energy per task? — *Dynamic power doubles; energy/task ~unchanged (same switched charge)… unless V had to rise to reach f — then both worsen. Shows you separate power from energy.*
4. Why does clock gating verification matter? — *A wrong enable = functional bug (missed capture); gating conditions need assertions/coverage.*
5. Chip is over its leakage budget in standby — first three levers? — *Power-gate idle domains, raise V_T mix on non-critical paths, lower/collapse standby voltage (retention).*
6. Why can't we keep lowering V_DD? — *Delay explodes near V_T, variability and noise margins collapse; SRAM V_min usually limits first.*
7. What is IR drop and why do you care? — *Resistive supply-grid droop under current spikes → effectively lower V at cells → timing failures; needs grid analysis, decap.*
8. Inverter with input at V_DD/2 — what's happening? — *Both networks partially on → maximum short-circuit current, high gain region; also why floating CMOS inputs are bad.*
