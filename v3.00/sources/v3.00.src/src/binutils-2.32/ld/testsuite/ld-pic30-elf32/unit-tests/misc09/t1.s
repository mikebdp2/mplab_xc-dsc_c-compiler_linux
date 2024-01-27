        ;;
        ;; misc09
        ;;
        ;; - requires C startup library
        .text
        .global _main
_main:
   
T1:     
        ;; local function pointers in data space
        mov     #table1,w1
        mov     #8,w2
T11:    mov     [w1++],w0
        call    w0
        call    print_w0
        mov     #' ',w0
        call    print_char

        dec     w2,w2
        bra     gt,T11
        call    print_CR

T2:     
        ;; duplicate table
        mov     #table2,w1
        mov     #8,w2
T21:    mov     [w1++],w0
        call    w0
        call    print_w0
        mov     #' ',w0
        call    print_char

        dec     w2,w2
        bra     gt,T21
        call    print_CR

T3:     
        ;; global function pointers in data space
        mov     #table3,w1
        mov     #8,w2
T31:    mov     [w1++],w0
        call    w0
        call    print_w0
        mov     #' ',w0
        call    print_char

        dec     w2,w2
        bra     gt,T31
        call    print_CR

T4:     
        ;; duplicate table
        mov     #table4,w1
        mov     #8,w2
T41:    mov     [w1++],w0
        call    w0
        call    print_w0
        mov     #' ',w0
        call    print_char

        dec     w2,w2
        bra     gt,T41
        call    print_CR

T5:     
        ;; local function pointers in program space
        mov     #tblpage(table5),w1
        mov     w1,TBLPAG
        mov     #tbloffset(table5),w1
        mov     #8,w2
T51:    tblrdl  [w1++],w0
        call    w0
        call    print_w0
        mov     #' ',w0
        call    print_char

        dec     w2,w2
        bra     gt,T51
        call    print_CR

T6:     
        ;; duplicate table
        mov     #tblpage(table6),w1
        mov     w1,TBLPAG
        mov     #tbloffset(table6),w1
        mov     #8,w2
T61:    tblrdl  [w1++],w0
        call    w0
        call    print_w0
        mov     #' ',w0
        call    print_char

        dec     w2,w2
        bra     gt,T61
        call    print_CR

T7:     
        ;; global function pointers in program space
        mov     #tblpage(table7),w1
        mov     w1,TBLPAG
        mov     #tbloffset(table7),w1
        mov     #8,w2
T71:    tblrdl  [w1++],w0
        call    w0
        call    print_w0
        mov     #' ',w0
        call    print_char

        dec     w2,w2
        bra     gt,T71
        call    print_CR

T8:     
        ;; duplicate table
        mov     #tblpage(table8),w1
        mov     w1,TBLPAG
        mov     #tbloffset(table8),w1
        mov     #8,w2
T81:    tblrdl  [w1++],w0
        call    w0
        call    print_w0
        mov     #' ',w0
        call    print_char

        dec     w2,w2
        bra     gt,T81
        call    print_CR

T9:     
        ;; local function pointers in PSV window
        mov     #table9,w1
        mov     #8,w2
T91:    mov     [w1++],w0
        call    w0
        call    print_w0
        mov     #' ',w0
        call    print_char

        dec     w2,w2
        bra     gt,T91
        call    print_CR

T100:     
        ;; duplicate table
        mov     #table10,w1
        mov     #8,w2
T101:   mov     [w1++],w0
        call    w0
        call    print_w0
        mov     #' ',w0
        call    print_char

        dec     w2,w2
        bra     gt,T101
        call    print_CR

T110:     
        ;; global function pointers in data space
        mov     #table11,w1
        mov     #8,w2
T111:   mov     [w1++],w0
        call    w0
        call    print_w0
        mov     #' ',w0
        call    print_char

        dec     w2,w2
        bra     gt,T111
        call    print_CR

T120:     
        ;; duplicate table
        mov     #table12,w1
        mov     #8,w2
T121:   mov     [w1++],w0
        call    w0
        call    print_w0
        mov     #' ',w0
        call    print_char

        dec     w2,w2
        bra     gt,T121
        call    print_CR

        call    print_done
        .pword  0xda6000        ; break

        ;; local functions
func1:
        mov     #0x1,w0
        return
func2:
        mov     #0x2,w0
        return
func3:
        mov     #0x3,w0
        return
func4:
        mov     #0x4,w0
        return
func5:
        mov     #0x5,w0
        return
func6:
        mov     #0x6,w0
        return
func7:
        mov     #0x7,w0
        return
func8:
        mov     #0x8,w0
        return

        ;; global functions
        .global Func1,Func2,Func3,Func4,Func5,Func6,Func7,Func8
Func1:
        mov     #0x1,w0
        return
Func2:
        mov     #0x2,w0
        return
Func3:
        mov     #0x3,w0
        return
Func4:
        mov     #0x4,w0
        return
Func5:
        mov     #0x5,w0
        return
Func6:
        mov     #0x6,w0
        return
Func7:
        mov     #0x7,w0
        return
Func8:
        mov     #0x8,w0
        return

        ;; tables in data space
        .data
table1:
        .word   handle(func1)
        .word   handle(func2)
        .word   handle(func3)
        .word   handle(func4)
        .word   handle(func5)
        .word   handle(func6)
        .word   handle(func7)
        .word   handle(func8)

table2:
        .word   handle(func1)
        .word   handle(func2)
        .word   handle(func3)
        .word   handle(func4)
        .word   handle(func5)
        .word   handle(func6)
        .word   handle(func7)
        .word   handle(func8)

table3:
        .word   handle(Func1)
        .word   handle(Func2)
        .word   handle(Func3)
        .word   handle(Func4)
        .word   handle(Func5)
        .word   handle(Func6)
        .word   handle(Func7)
        .word   handle(Func8)

table4:
        .word   handle(Func1)
        .word   handle(Func2)
        .word   handle(Func3)
        .word   handle(Func4)
        .word   handle(Func5)
        .word   handle(Func6)
        .word   handle(Func7)
        .word   handle(Func8)

        ;; tables in program space
        
        .text
table5:
        .word   handle(func1)
        .word   handle(func2)
        .word   handle(func3)
        .word   handle(func4)
        .word   handle(func5)
        .word   handle(func6)
        .word   handle(func7)
        .word   handle(func8)

table6:
        .word   handle(func1)
        .word   handle(func2)
        .word   handle(func3)
        .word   handle(func4)
        .word   handle(func5)
        .word   handle(func6)
        .word   handle(func7)
        .word   handle(func8)

table7:
        .word   handle(Func1)
        .word   handle(Func2)
        .word   handle(Func3)
        .word   handle(Func4)
        .word   handle(Func5)
        .word   handle(Func6)
        .word   handle(Func7)
        .word   handle(Func8)

table8:
        .word   handle(Func1)
        .word   handle(Func2)
        .word   handle(Func3)
        .word   handle(Func4)
        .word   handle(Func5)
        .word   handle(Func6)
        .word   handle(Func7)
        .word   handle(Func8)

        ;; tables in PSV window
        ;; (any of these should work in v1.30+)
        ;.section .const,"awx"
        ;.section .const,"r"
        .section .const,psv
table9:
        .word   handle(func1)
        .word   handle(func2)
        .word   handle(func3)
        .word   handle(func4)
        .word   handle(func5)
        .word   handle(func6)
        .word   handle(func7)
        .word   handle(func8)

table10:
        .word   handle(func1)
        .word   handle(func2)
        .word   handle(func3)
        .word   handle(func4)
        .word   handle(func5)
        .word   handle(func6)
        .word   handle(func7)
        .word   handle(func8)

table11:
        .word   handle(Func1)
        .word   handle(Func2)
        .word   handle(Func3)
        .word   handle(Func4)
        .word   handle(Func5)
        .word   handle(Func6)
        .word   handle(Func7)
        .word   handle(Func8)

table12:
        .word   handle(Func1)
        .word   handle(Func2)
        .word   handle(Func3)
        .word   handle(Func4)
        .word   handle(Func5)
        .word   handle(Func6)
        .word   handle(Func7)
        .word   handle(Func8)

