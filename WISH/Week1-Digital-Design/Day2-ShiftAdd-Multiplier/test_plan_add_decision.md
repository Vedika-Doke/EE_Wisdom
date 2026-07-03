# Test Plan for Conditional Add Logic (add_decision)

DUT is purely combinational.
Inputs: A[8:0], M[7:0], Q_lsb
Outputs: sum = A + {1'b0, M}, do_add = Q_lsb

| DUT functionality to be tested | Type of stimulus | Expected behav for DUT |
|---|---|---|
| Sum computation | A=0,M=0 ; A=0,M=nonzero ; A=nonzero,M=0 ; A=nonzero,M=nonzero | sum = A + M |
| do_add follows Q_lsb | Q_lsb=0 then Q_lsb=1 with same A,M | do_add = Q_lsb, sum unchanged |
| Carry / 9-bit width | A=0FF M=FF ; A=1FF M=01 | full 9-bit sum, wraps if >9 bits |
| Independence of sum from Q_lsb | hold A,M, toggle Q_lsb | sum constant, only do_add changes |
| Corner cases | A,M = {min,max} combinations | sum = A+M, wrap if >9 bits |

Note: max sum = 511+255 = 766, exceeds 9 bits and wraps to 0xFE.
