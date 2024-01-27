;; Note: This macro was lifted from "p30F6014.inc"
;; and corrected as described in issue XC16-1589, item 2.
;;
        .macro config REG, VALUE
        .pushsection \REG\().sec, code
        .global \REG
        .warn "DEPRECATED. Please use #pragma config in a C file"
\REG:   .pword \VALUE
        .popsection
        .endm
        

        
