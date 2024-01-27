        .text      
        .global __t2
__t2:
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

        .equiv  symB1, 0
        .equiv  symB2, 0x300
        .equiv  symB3, 0x7FFF
        .equiv  symB4, 0x8000
        .equiv  symB5, 0xFFFF

        .global symB1,symB2,symB3,symB4,symB5
        
