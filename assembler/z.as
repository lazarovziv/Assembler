.entry LABEL1
    .extern LABEL2
    .entry LABEL3
LABEL3: add r0, LABEL1
mov r1, LABEL1
prn LABEL2
jmp LABEL3(#-8,LABEL1)
inc K
    .extern K
LABEL1: .data 16,18,22,-1
LABEL2: .string "wassupppp"
mcr m1
add LABEL1, r3
not LABEL3
cmp #-1, LABEL2
cmp #2, LABEL1

endmcr
rts
X: .data 15, 13
Y: inc Z
Z: mov #-1, r2
jsr X(Y,Z)
    .entry LABEL4
    .extern LABEL5
    .entry LABEL6
mov r1, LABEL4
prn LABEL5
jmp LABEL8(#-8,LABEL4)
inc K1
    .extern K1
LABEL8: .string "wassupppp"
LABEL4: .data 16,18,22,-1, 16, 18
add LABEL4, r3
not LABEL5
cmp #-1, LABEL5
cmp #2, LABEL4
rts
X1: .data 15, 13
Y1: inc Z1
m1
Z1: mov #-1, r2
jsr X1(Y1,Z1)
m1
