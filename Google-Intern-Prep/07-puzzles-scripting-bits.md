# 07 — Puzzles, Probability, Scripting & Bit Manipulation

The test often has an aptitude section; interviews may open with a puzzle to watch you reason aloud (Google's "GCA" axis). Scripting (Python/TCL/Perl) is a JD preferred qualification — one small live task or a "how would you script this?" question is common.

---

## A. Classic puzzles (know the *method*, not just the answer)

1. **Two ropes, each burns in 60 min non-uniformly; measure 45 min.** Light rope A both ends + rope B one end. A finishes at 30; light B's other end → B's remaining 30 min burns in 15. Total 45.
2. **3 switches, 1 bulb upstairs, one visit.** Turn S1 on 5 min, off; S2 on; go up. On=S2, warm=S1, cold+off=S3. (Physical side channels — say the principle.)
3. **8 balls, one heavier, balance scale, min weighings?** 2. Weigh 3v3: balanced → weigh the remaining 2; unbalanced → weigh 1v1 from heavy trio. General principle: each weighing has 3 outcomes → ⌈log₃(cases)⌉. *This ternary-information argument is exactly what a hardware interviewer wants to hear.*
4. **25 horses, 5 lanes, top 3, min races?** 7. Five heats, then winners' race, then a 3rd race of the 5 candidates for places 2-3 (2nd/3rd of winner's heat, 2nd of runner-up's heat, 1st+... standard construction: heat winners race → eliminate; final candidate set {A2,A3,B1... } → the known answer: race #7 among A2,A3,B2,B1? Learn the construction: after winners' race with order A>B>C>D>E, candidates for 2nd/3rd = A2, A3, B1, B2, C1 → race them.)
5. **Gold bar, 7 days, 2 cuts.** Cut into 1,2,4 (binary weights) — pay any 1-7 with exchanges. Binary representation!
6. **100 doors / toggling**: door n toggled once per divisor → open iff perfect square (odd # divisors).
7. **2 eggs, 100 floors**: minimize worst-case drops → 14 (decreasing intervals 14,13,12…; n(n+1)/2 ≥ 100).
8. **Poison wine, 1000 bottles, 10 prisoners**: binary-encode bottle numbers; each prisoner drinks bottles with their bit set → dead pattern = bottle index. (Pure binary encoding — hardware-flavored favorite.)

**Meta-tip**: narrate constraints → information theory bound → construction. Even a wrong final answer with clean reasoning scores well on GCA.

## B. Probability quickies

- Expected coin tosses to get first head: 2 (geometric, 1/p). To get HH: 6; HT: 4 (worth pre-deriving once).
- Two dice sum 7: 6/36 = 1/6 (most likely sum).
- At least one head in n tosses: 1 − 2⁻ⁿ.
- 3 points on a circle in one semicircle: 3/4.
- Bayes sanity: rare-disease test question — always compute with a 10,000-person table.
- Birthday: ~23 people for 50%.
- **Hardware tie-in**: MTBF/failure probability questions are just expectation problems — connect if asked.

## C. Bit manipulation (C/Verilog-adjacent; near-certain in test or interview)

| Task | Trick |
|---|---|
| Check power of 2 | `x && !(x & (x-1))` |
| Count set bits | `while(x){x &= x-1; c++;}` (Kernighan — loops once per set bit) |
| Isolate lowest set bit | `x & -x` |
| Clear lowest set bit | `x & (x-1)` |
| Swap without temp | `a^=b; b^=a; a^=b;` (mention: fails if same address; XOR trick) |
| Set/clear/toggle/test bit k | `x|=(1<<k)`, `x&=~(1<<k)`, `x^=(1<<k)`, `(x>>k)&1` |
| Sign of int / abs without branch | arithmetic shift tricks — recognize, don't memorize |
| Reverse bits of a byte | swap nibbles, pairs, singles (mask-and-shift ladder) |
| Detect endianness | `int x=1; *(char*)&x == 1` → little-endian |
| Average of two ints w/o overflow | `(a&b) + ((a^b)>>1)` |
| Parity of a word | XOR-fold: `x^=x>>16; x^=x>>8; ...` |
| Multiply/divide by 2ⁿ | shifts; know arithmetic vs logical shift for signed |

C-pointer sanity (occasionally probed): `const int *p` vs `int *const p`; array decay; what `volatile` means (**hardware answer: compiler must not cache/reorder accesses — memory-mapped registers!**); struct padding/alignment.

## D. Python scripting (JD: Perl/TCL/Python)

Be able to live-write, in <10 minutes, things like:

**1. Parse a log, count error types:**
```python
import re
from collections import Counter
counts = Counter()
with open("sim.log") as f:
    for line in f:
        m = re.search(r"ERROR\s*:\s*(\w+)", line)
        if m:
            counts[m.group(1)] += 1
for err, n in counts.most_common():
    print(f"{err}: {n}")
```

**2. Extract failing test names + max value per test from a CSV** — `dict` accumulation, `max()`, string `split(',')`.

**3. Rename/scan files**: `os.walk`, `glob`, f-strings.

**4. Explain how you'd use it in a flow**: "parse regression logs to auto-triage failures", "generate testcases / register RTL from a spec spreadsheet", "wrap tool runs and diff reports" — the JD's "automating design flows" bullet, verbatim.

**TCL one-liner of knowledge**: TCL is the console language of EDA tools (Design Compiler, PrimeTime, Vivado): `set_false_path`, `report_timing`, `foreach_in_collection` — say you've seen/used constraint scripts (SDC *is* TCL). Perl: legacy flow glue, regex-heavy; Python has largely replaced it — knowing that sentence is enough.

## E. If they ask a coding question (possible in Google interviews even for HW)

Prepare basic DSA at easy-LeetCode level: arrays/strings (two-pointer, hashmap), reverse a linked list, binary search, simple recursion, and the bit problems above. HW-intern coding rarely exceeds this, but Google interviewers default to code when in doubt — 2-3 practice problems a day for a week is enough insurance.
