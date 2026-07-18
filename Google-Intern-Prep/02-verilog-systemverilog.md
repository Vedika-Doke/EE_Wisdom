# 02 — Verilog / SystemVerilog (JD explicitly lists this)

You will almost certainly be asked to *write* RTL live (doc or whiteboard). Interviewers care that your code is synthesizable, reset-correct, and that you can explain every line.

---

## A. The three questions everyone gets

### 1. Blocking (`=`) vs non-blocking (`<=`)
- `=` executes immediately in statement order; `<=` samples RHS now, updates at end of timestep.
- **Rule you state in the interview**: sequential logic (`always @(posedge clk)`) → non-blocking; combinational (`always @(*)`) → blocking. Never mix in one block.
- Classic probe: swap.
  ```verilog
  always @(posedge clk) begin a <= b; b <= a; end   // swaps — both RHS sampled before update
  always @(posedge clk) begin a = b;  b = a;  end   // both end up = b
  ```
- Shift register probe:
  ```verilog
  always @(posedge clk) begin q1 <= d; q2 <= q1; end  // 2-stage shift
  always @(posedge clk) begin q1 = d;  q2 = q1;  end  // one FF: q2 = d after synthesis? No — q2 gets NEW q1 → single stage of delay for both (wire-through), tools infer 1 FF for q2=d path.
  ```
  Expect the follow-up "what hardware does each synthesize to?" — blocking version collapses the pipeline.

### 2. Accidental latch inference
Combinational `always` where some path doesn't assign the output → tool must hold the old value → **latch**.
```verilog
always @(*) if (sel) y = a;          // latch! no else
always @(*) case (s) 2'b00: y = a; 2'b01: y = b; endcase  // latch! no default
```
Fixes: `else`, `default:`, or assign a default value at the top of the block (`y = '0;` first line — the cleanest habit, mention it).

### 3. Sensitivity list bugs
`always @(a)` when block also reads `b` → simulation/synthesis mismatch (synthesis ignores the list, sim doesn't). Always use `@(*)` — or in SV, `always_comb` (which also errors on latches) and `always_ff` (which enforces FF semantics). Using `always_comb`/`always_ff` unprompted signals good style.

---

## B. Templates to have memorized

**D-FF, async active-low reset:**
```verilog
always @(posedge clk or negedge rst_n)
  if (!rst_n) q <= 1'b0;
  else        q <= d;
```
Sync reset: drop `or negedge rst_n`, keep the if. Know the tradeoff: async reset acts without a clock (good for power-on), but its **deassertion** must be synchronized (reset synchronizer: two FFs, async assert, sync de-assert) or you risk recovery/removal violations.

**Parameterized counter:**
```verilog
module counter #(parameter W = 8) (
  input clk, rst_n, en,
  output reg [W-1:0] q
);
  always @(posedge clk or negedge rst_n)
    if (!rst_n)   q <= '0;
    else if (en)  q <= q + 1'b1;
endmodule
```

**FSM, two-process style (1011 detector, Moore, overlapping):**
```verilog
localparam S0=0, S1=1, S2=2, S3=3, S4=4;
reg [2:0] st, nst;
always @(posedge clk or negedge rst_n)
  if (!rst_n) st <= S0; else st <= nst;
always @(*) begin
  nst = st;
  case (st)
    S0: nst = in ? S1 : S0;
    S1: nst = in ? S1 : S2;
    S2: nst = in ? S3 : S0;
    S3: nst = in ? S4 : S2;
    S4: nst = in ? S1 : S2;
    default: nst = S0;
  endcase
end
assign detected = (st == S4);
```

**2-FF synchronizer:**
```verilog
reg m, s;
always @(posedge clk_dst) begin m <= async_in; s <= m; end
```
Say: single-bit signals only; for buses use gray coding, handshake, or an async FIFO (details in file 03).

**Synchronous FIFO (the big one — practice writing this in <15 min):**
```verilog
module sync_fifo #(parameter W=8, D=16, A=$clog2(D)) (
  input clk, rst_n, wr_en, rd_en,
  input  [W-1:0] din,
  output [W-1:0] dout,
  output full, empty
);
  reg [W-1:0] mem [0:D-1];
  reg [A:0] wptr, rptr;               // one extra bit disambiguates full vs empty
  wire [A-1:0] waddr = wptr[A-1:0], raddr = rptr[A-1:0];
  assign empty = (wptr == rptr);
  assign full  = (waddr == raddr) && (wptr[A] != rptr[A]);
  always @(posedge clk or negedge rst_n)
    if (!rst_n) wptr <= '0;
    else if (wr_en && !full) begin mem[waddr] <= din; wptr <= wptr + 1'b1; end
  always @(posedge clk or negedge rst_n)
    if (!rst_n) rptr <= '0;
    else if (rd_en && !empty) rptr <= rptr + 1'b1;
  assign dout = mem[raddr];
endmodule
```
Be ready for: why the extra pointer bit (else full==empty is ambiguous), what changes for an **async** FIFO (gray-coded pointers + 2-FF sync each way, full/empty computed conservatively in each domain).

**Edge-detector pulse:** `assign rise = sig & ~sig_q;` with `sig_q <= sig`.

**Clock divider by 2^n:** counter, tap bit n−1. Odd/50%: see file 01.

---

## C. Language traps they probe

| Probe | Answer |
|---|---|
| `==` vs `===` | `==` returns X if any operand bit is X/Z; `===` compares X/Z literally. `===` is non-synthesizable (testbench only). |
| `reg` vs `wire` | `reg` = assigned in procedural block (not necessarily a register!); `wire` = driven by assign/port. SV `logic` unifies both (single-driver). |
| What does `#5` mean in synthesis? | Nothing — delays are ignored by synthesis; testbench/modeling only. |
| Signed pitfall | Mixed signed/unsigned expression → whole expression unsigned. `$signed()` casts. |
| `casex` vs `casez` | `casez` treats `?`/Z as don't-care; `casex` also treats X — dangerous (X in sim matches anything). Prefer `casez` or SV `case…inside`, or plain case + default. |
| Width mismatch | RHS zero/sign-extends or truncates silently — lint flags it; state that lint (JD keyword!) catches width, latch, sensitivity issues. |
| `generate` | Compile-time structural replication: `for (genvar i…) generate` for arrays of instances/adder stages. |
| `function` vs `task` | Function: no time controls, returns value, combinational use. Task: can consume time, no return (outputs via args). |
| Initial block in synthesis | Ignored by ASIC synthesis (FPGA tools honor as init value) — reset logic must be explicit for ASIC. |
| Race between two always blocks writing same reg | Non-deterministic; multiple drivers on a reg is a bug; SV `always_ff` and single-driver `logic` help. |

---

## D. SystemVerilog you should *recognize* (intern-level depth)

- Types: `logic`, packed/unpacked arrays, `typedef enum` for FSM states (readable waveforms — say this).
- `always_comb / always_ff / always_latch` — intent-checked processes.
- Interfaces & modports: bundle related signals (awareness level).
- **Assertions (SVA)** — JD-relevant. Immediate: `assert (cond) else $error(...);` Concurrent:
  ```systemverilog
  assert property (@(posedge clk) disable iff (!rst_n) req |-> ##[1:3] gnt);
  ```
  Read as: "whenever req, gnt within 1-3 cycles." Know `|->` (same-cycle implication) vs `|=>` (next-cycle).
- Constrained-random & coverage: `class`, `rand`, `constraint`, `covergroup` — one-line awareness each (details in file 05).

---

## E. Write-and-verify drills (do all, hand-simulate)

1. Parameterized shift register with load.
2. Debouncer: counter that requires input stable for N cycles before propagating.
3. Serial-in parity tracker (1 FF + XOR).
4. Round-robin arbiter, 4 requesters (hint: rotate priority based on last grant; a clean trick is `grant = req & ~(req - base) ... ` — but a simple masked-priority two-pass scheme is fine to present).
5. Traffic light FSM with `typedef enum` states.
6. Testbench for your FIFO: clock gen (`always #5 clk = ~clk;`), reset task, write burst then read, `$display` on mismatch. Being able to sketch a self-checking TB distinguishes you.

Free practice with instant feedback: **HDLBits** (hdlbits.01xz.net) — do the "Circuits" section end-to-end; it maps 1:1 to these interview questions. Simulate on EDA Playground (free, iverilog).
