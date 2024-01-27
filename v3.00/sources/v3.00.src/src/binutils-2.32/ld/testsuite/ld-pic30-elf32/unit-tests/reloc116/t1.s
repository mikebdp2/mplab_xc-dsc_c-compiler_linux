        .text      
        .global __reset
__reset:
        .word  addr_lo(symA1)
        .word  addr_lo(symA1+8)
        .word  addr_lo(symA1-8)
        .word  addr_lo(symA2)
        .word  addr_lo(symA2+8)
        .word  addr_lo(symA2-8)
        .word  addr_lo(symA3)
        .word  addr_lo(symA3+8)
        .word  addr_lo(symA3-8)
        .word  addr_lo(symA4)
        .word  addr_lo(symA4+8)
        .word  addr_lo(symA4-8)
        .word  addr_lo(symA5)
        .word  addr_lo(symA5+8)  ;; addr_hi() would roll over
        .word  addr_lo(symA5-8)
        nop
        .word  addr_lo(symB1)
        .word  addr_lo(symB1+8)
        .word  addr_lo(symB1-8)
        .word  addr_lo(symB2)
        .word  addr_lo(symB2+8)
        .word  addr_lo(symB2-8)
        .word  addr_lo(symB3)
        .word  addr_lo(symB3+8)
        .word  addr_lo(symB3-8)
        .word  addr_lo(symB4)
        .word  addr_lo(symB4+8)
        .word  addr_lo(symB4-8)
        .word  addr_lo(symB5)
        .word  addr_lo(symB5+8)  ;; addr_hi() would roll over
        .word  addr_lo(symB5-8)

        .equiv  symA1, 0
        .equiv  symA2, 0x300
        .equiv  symA3, 0x7FFF
        .equiv  symA4, 0x8000
        .equiv  symA5, 0xFFFF
                
        .global symA1,symA2,symA3,symA4,symA5
 
