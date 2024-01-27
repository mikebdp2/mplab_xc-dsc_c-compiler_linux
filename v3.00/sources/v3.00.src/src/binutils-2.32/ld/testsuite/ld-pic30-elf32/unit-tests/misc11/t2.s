        .global Func1
Func1:  clr w1

        .weak Func2
Func2:  nop

        .weak Func4
Func4:  nop

        .global Func5
Func5:  clr w5

        .global S1
        .equ S1,0x111111

        .weak S2
        .equ S2,0x999999
