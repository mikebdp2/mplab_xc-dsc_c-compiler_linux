        .text      
        .global __t2
__t2:
        mov     #addr_hi(symA1),w0
        mov     #addr_hi(symA1+8),w0
        mov     #addr_hi(symA1-8),w0
        mov     #addr_hi(symA2),w0
        mov     #addr_hi(symA2+8),w0
        mov     #addr_hi(symA2-8),w0
        mov     #addr_hi(symA3),w0
        mov     #addr_hi(symA3+8),w0
        mov     #addr_hi(symA3-8),w0
        mov     #addr_hi(symA4),w0
        mov     #addr_hi(symA4+8),w0
        mov     #addr_hi(symA4-8),w0
        mov     #addr_hi(symA5),w0
        mov     #addr_hi(symA5+8),w0
        mov     #addr_hi(symA5-8),w0
        nop
        mov     #addr_hi(symB1),w0
        mov     #addr_hi(symB1+8),w0
        mov     #addr_hi(symB1-8),w0
        mov     #addr_hi(symB2),w0
        mov     #addr_hi(symB2+8),w0
        mov     #addr_hi(symB2-8),w0
        mov     #addr_hi(symB3),w0
        mov     #addr_hi(symB3+8),w0
        mov     #addr_hi(symB3-8),w0
        mov     #addr_hi(symB4),w0
        mov     #addr_hi(symB4+8),w0
        mov     #addr_hi(symB4-8),w0
        mov     #addr_hi(symB5),w0
        mov     #addr_hi(symB5+8),w0
        mov     #addr_hi(symB5-8),w0

        .equiv  symB1, 0x20000
        .equiv  symB2, 0x20300
        .equiv  symB3, 0x27FFF
        .equiv  symB4, 0x28000
        .equiv  symB5, 0x2FFFF

        .global symB1,symB2,symB3,symB4,symB5
        
