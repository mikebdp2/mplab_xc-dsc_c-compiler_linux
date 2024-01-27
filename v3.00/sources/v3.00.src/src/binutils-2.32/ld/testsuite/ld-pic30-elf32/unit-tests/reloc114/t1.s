        .text      
        .global __reset
__reset:
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
        mov     #addr_lo(symB5+8),w0  ;; addr_hi() would roll over
        mov     #addr_lo(symB5-8),w0

        .equiv  symA1, 0
        .equiv  symA2, 0x300
        .equiv  symA3, 0x7FFF
        .equiv  symA4, 0x8000
        .equiv  symA5, 0xFFFF
                
        .global symA1,symA2,symA3,symA4,symA5
 
