        .text
.if 1
        .global __reset
__reset:
.else
        .global _main
_main:
.endif
        .pword 0xda6000

        ;; mapped DATA
        .data
        .byte 0x4a,0x4b,0x4c,0x4d,0x4e,0x4f,0x50,0x51,0x52,0x53

        ;; mapped DATA
        .section .ydata,"d"
        .byte 0x61,0x62,0x63,0x64,0x65,0x66,0x67

        ;; non-mapped BSS
        .section .b1,"b"
        .space 0xa 

        ;; mapped DATA
        .section .xdata,"d"
        .byte 0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49

        ;; mapped DATA
        .section .dconst,"d"
        .byte 0x54,0x55,0x56,0x57,0x58,0x59,0x5a

        ;; non-mapped BSS
        .section .b2,"b"
        .space 0xa
        
        ;; non-mapped DATA
        .section .d1,"d"
        .byte 0x68,0x69,0x6a,0x6b,0x6c,0x6d,0x6e,0x6f,0x70,0x71
        
        ;; non-mapped BSS
        .section .b3,"b"
        .space 0xa
        
        ;; non-mapped DATA
        .section .d2,"d"
        .byte 0x72,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7a

        ;; non-mapped CODE
        .section .x1,"x"
        .byte 0x43,0x43,0x43,0x43,0x43,0x43,0x43,0x43,0x43,0x43

        ;; mapped READONLY
        .section .const,"r"
        .byte 0x43,0x43,0x43,0x43,0x43,0x43,0x43,0x43,0x43,0x43

        ;; non-mapped READONLY
        .section .r1,"r"
        .byte 0x43,0x43,0x43,0x43,0x43,0x43,0x43,0x43,0x43,0x43
        