        .text
        .global __reset
__reset:
        nop

        .section .eedata, eedata
        .word 0,1,2,3,4,5,6,7   ; 16 bytes

