.entry GUY
.extern W
ZIV: mov r3, GUY
GUY: jmp L1(#16,ZIV)
mcr m1
jmp ZIV(r3,GUY)
inc W
endmcr
prn #18
bne W(r7,r5)
sub r3, r2
bne ZIV
L1: inc K
m1
.entry LOOP
bne LOOP(K,W)
STR: .string "xyz"
LENGTH: .data 16,-21,152
K: .data 22
.extern L3

