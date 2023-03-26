LABLE1:  mov r1, unknown
jmp unknown(r1,r2)
bne LABLE1(unknown,r1)
jsr LABLE1(r1,unknown)
.extern ThisLableWasNotUsed
.entry ThisLableWasNotDeclared