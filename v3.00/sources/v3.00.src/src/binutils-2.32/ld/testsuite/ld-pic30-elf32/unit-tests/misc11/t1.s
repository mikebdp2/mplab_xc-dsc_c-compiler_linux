        ;;
        ;; misc10
        ;;
        .text
        .global __reset
__reset:
        ;; functions defined in this file
        call    Func1              ; weak<global
        call    Func2              ; global>weak
        call    Func3              ; weak only

        ;; functions defined in other files
        call    Func4              ; weak<global
        call    Func5              ; global>weak
        call    Func6              ; weak only
        
        .weak Func1
Func1:     nop

        .global Func2
Func2:     clr w2

        .weak Func3
Func3:     clr w3
        
        .weak S0
        .global S1
        .global S2

zero:
        .pword   S0
one:    
        .pword   S1
two:    
        .pword   S2
three:
        .pword   S3
        
