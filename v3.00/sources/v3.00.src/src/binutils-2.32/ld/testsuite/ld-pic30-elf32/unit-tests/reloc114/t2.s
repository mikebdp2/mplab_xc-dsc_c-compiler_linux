        .text      
        .global __t2
__t2:
        mov     #addr_lo(symA1),w0
        mov     #addr_lo(symA1+8),w0
        mov     #addr_lo(symA1-8),w0
        mov     #addr_lo(symA2),w0
        mov     #addr_lo(symA2+8),w0
        mov     #addr_lo(symA2-8),w0
        mov     #addr_lo(symA3),w0
        mov     #addr_lo(symA3+8),w0
        mov     #addr_lo(symA3-8),w0
        mov     #addr_lo(symA4),w0
        mov     #addr_lo(symA4+8),w0
        mov     #addr_lo(symA4-8),w0
        mov     #addr_lo(symA5),w0
        mov     #addr_lo(symA5+8),w0  ;; addr_hi() would roll over
        mov     #addr_lo(symA5-8),w0
        nop
        mov     #addr_lo(symB1),w0
        mov     #addr_lo(symB1+8),w0
        mov     #addr_lo(symB1-8),w0
        mov     #addr_lo(symB2),w0
        mov     #addr_lo(symB2+8),w0
        mov     #addr_lo(symB2-8),w0
        mov     #addr_lo(symB3),w0
        mov     #addr_lo(symB3+8),w0
        mov     #addr_lo(symB3-8),w0
        mov     #addr_lo(symB4),w0
        mov     #addr_lo(symB4+8),w0
        mov     #addr_lo(symB4-8),w0
        mov     #addr_lo(symB5),w0
        mov     #addr_lo(symB5+8),w0   ;; addr_hi() would roll over
        mov     #addr_lo(symB5-8),w0

        .equiv  symB1, 0
        .equiv  symB2, 0x300
        .equiv  symB3, 0x7FFF
        .equiv  symB4, 0x8000
        .equiv  symB5, 0xFFFF

        .global symB1,symB2,symB3,symB4,symB5
        
