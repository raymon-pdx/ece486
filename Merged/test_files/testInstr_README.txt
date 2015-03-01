Expected output for testInstr.pal

cla cll    1: cla = 0, lk = 0;
tad A      2: ac = A = 2
tad B      3: ac = A + B = 2 + 3
dca C      4: C(C) = 5
cla cll    5: ac = 0, lk = 0
tad D      6: ac = 16
and B      7: ac = 3
dca A      8: C(A) = 3, ac = 0;
jms JUMP   9: C(PC) = Eaddr(JUMP)
cma       10: ac = 0xFFF
iac       11: ac = 0, link = 1
sma       12: link = 1 so skip next instruction
tad B     13: ac = 3, this should not happen
tad C     14: ac = 5, this should happen
rar       15: rotate right ac = 0x802, link = 0
ral       16: rotate left  ac = 0x004, link = 1
cia       17: ac = 0xFFC
jmp i JUMP  18: return from subroutine
tad C     19: ac = 5
hlt       20: end program