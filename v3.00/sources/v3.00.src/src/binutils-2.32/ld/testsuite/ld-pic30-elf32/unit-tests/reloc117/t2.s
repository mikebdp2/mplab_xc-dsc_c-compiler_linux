        .text      
        .global __t2
__t2:
        .word  addr_hi(symA1)
        .word  addr_hi(symA1+8)
        .word  addr_hi(symA1-8)
        .word  addr_hi(symA2)
        .word  addr_hi(symA2+8)
        .word  addr_hi(symA2-8)
        .word  addr_hi(symA3)
        .word  addr_hi(symA3+8)
        .word  addr_hi(symA3-8)
        .word  addr_hi(symA4)
        .word  addr_hi(symA4+8)
        .word  addr_hi(symA4-8)
        .word  addr_hi(symA5)
        .word  addr_hi(symA5+8)  ;; addr_hi() would roll over
        .word  addr_hi(symA5-8)
        nop
        .word  addr_hi(symB1)
        .word  addr_hi(symB1+8)
        .word  addr_hi(symB1-8)
        .word  addr_hi(symB2)
        .word  addr_hi(symB2+8)
        .word  addr_hi(symB2-8)
        .word  addr_hi(symB3)
        .word  addr_hi(symB3+8)
        .word  addr_hi(symB3-8)
        .word  addr_hi(symB4)
        .word  addr_hi(symB4+8)
        .word  addr_hi(symB4-8)
        .word  addr_hi(symB5)
        .word  addr_hi(symB5+8)  ;; addr_hi() would roll over
        .word  addr_hi(symB5-8)

        .equiv  symB1, 0x20000
        .equiv  symB2, 0x20300
        .equiv  symB3, 0x27FFF
        .equiv  symB4, 0x28000
        .equiv  symB5, 0x2FFFF

        .global symB1,symB2,symB3,symB4,symB5
        
