;;- Machine description for GNU compiler
;;- Microchip constraints
;;- Copyright (C) 1994-2022 Free Software Foundation, Inc.
;;- Contributed by Microchip

;; This file is part of GNU CC.

;; GNU CC is free software; you can redistribute it and/or modify
;; it under the terms of the GNU General Public License as published by
;; the Free Software Foundation; either version 1, or (at your option)
;; any later version.

;; GNU CC is distributed in the hope that it will be useful,
;; but WITHOUT ANY WARRANTY; without even the implied warranty of
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;; GNU General Public License for more details.

;; You should have received a copy of the GNU General Public License
;; along with GNU CC; see the file COPYING.  If not, write to
;; the Free Software Foundation, 59 Temple Place - Suite 330,
;; Boston, MA 02111-1307, USA.

/*
** A MD expression which defines the machine-dependent operand constraint
** letters for register classes. If char is such a letter, the value should
** be the register class corresponding to it. Otherwise, the value should be
** NO_REGS. 
**
** For the dsPIC, 
** `a' is the class of PIC-compatible accumulator registers (W0).
** `b' is the class of divide support registers (W1).
** `B' is the class of blank sink registers used in DSP instructions.
** `c' is the class of multiply support registers (W2).
** `C' is the class of multiply support registers 32bit (W2-W3).
** `d' is the class of general-purpose data registers (W1..W14).
** `D' is the class of even general-purpose data registers (W0..W12).
** `e' is the class of non-divide support general registers (W2..W14).
** `l' is the class of floating point registers (F0..F31)
** 't' is the class of very restricted product registes (W7)
** 'u' is the class of restricted product registers (W4-W6)
** 'v' is the class of AWB registers (W13).
** 'w' is the class of ACCumulator registers (A..B).
** `x' is the class of x prefetch registers (W8..W9).
** `y' is the class of y prefetch registers (W10..W11).
** `za' is the class of mac product registers (W4..W7).
** `zs' is the class of mac signed product registers (W5,W7).
** `zu' is the class of mac unsigned product registers (W4,W6).
** `E' is sink
*/

(define_register_constraint "a"
  "(pic30_isa32_target() ? W_REGS: A_REGS)"
  "PIC-compatible accumulator a register (W0)" )

(define_register_constraint "b"
  "B_REGS"
  "PIC-compatible accumulator b register (W1)" )

(define_register_constraint "B"
  "SINK_REGS"
  "Sink registers" )

(define_register_constraint "c"
  "C_REGS"
  "PIC-compatible accumulator c register (W2)" )

(define_register_constraint "C"
  "CC_REGS"
  "PIC-compatible accumulator C 32bit register (W2,W3)" )

(define_register_constraint "d"
  "D_REGS"
  "General purpose data registers (W1..W14)" )

(define_register_constraint "D"
  "ERHI_REGS"
  "General purpose even HI data registers (W0..W14)" )

(define_register_constraint "e"
  "E_REGS"
  "General purpose data registers (W0..W14)" )

(define_register_constraint "l"
  "FLP_REGS"
  "Floating Point registers (F0..F31)" )

(define_register_constraint "t"
  "VERY_RESTRICTED_PRODUCT_REGS"
  "Very restricted product registers (W7)" )

(define_register_constraint "u"
  "RESTRICTED_PRODUCT_REGS"
  "Restricted product registers (W4..W6)" )

(define_register_constraint "v"
  "AWB_REGS"
  "AWB registers (W13)" )

(define_register_constraint "w"
  "(pic30_dsp_target() ? ACCUM_REGS: NO_REGS)"
  "Accumulator registers if dsp core(A..B)" )

(define_register_constraint "x"
  "X_PREFETCH_REGS"
  "X Prefetch registers (W8..W9)" )

(define_register_constraint "y"
  "Y_PREFETCH_REGS"
  "Y Prefetch registers (W10..W11)" )

(define_register_constraint "za"
  "PRODUCT_REGS"
  "MAC product registers (W4..W7)" )

(define_register_constraint "zs"
  "PRODUCT_REGS_SIGNED"
  "MAC signed product registers (W5,W7)" )

(define_register_constraint "zu"
  "PRODUCT_REGS_UNSIGNED"
  "MAC unsigned product registers (W4,W6)" )


  /* 
  ** The letters I,J,K,... to P in an operand constraint string
  ** can be used to stand for particular ranges of immediate operands.
  ** 
  ** For   the dsPIC,                                     dsPIC33A
  ** `h'   [-15,15]                            :  ==
  ** `I'   synonym for (const_int 1)           :  ==
  ** `J'   used for positive 10-bit literals   :  positive 16-bit literals
  ** `K'   used for the shift range,           :  ==
  ** `L'   -2,-1,1,2                           :  ==
  ** `M'   used for negative 10-bit literals   :  negative 16-bit literals
  ** `N'   used for negative 5-bit literals    :  negative 7-bit literals
  ** `O'   synonym for (const_int 0).          :  ==
  ** `P'   used for positive 5-bit literals    :  positive 7-bit literals
  ** 'W'   used for -16 <= x <= 16             :  ==
  ** 'Y'   used for -6,-4,-2,2,4,6             :  ==
  ** 'Z'   signed 4-bit literals               :  signed 6-bit literals
  ** 'jq'  int that fits in 8-bits             :  ==
  ** 'jh'  int that fits in 16-bits            :  ==
  ** 'js'  do I need to spell it out?          :  ==
  */ 

(define_constraint "h"
  "Constraint value between -15 to 15"
  (and (match_code "const_int")
       (match_test "ival >= -15 && ival <=15")
  )
)

(define_constraint "I"
  "A constant 1"
  (and (match_code "const_int")
       (match_test "ival == 1")
  )
)

(define_constraint "J"
  "Positive 10-bit literal for dspic or positive 16-bit literal for 33A"
  (and (match_code "const_int")
    (ior 
       (and (match_test "!pic30_isa32_target()")
            (match_test "ival >= 0 && ival <= 1023"))
       (and (match_test "pic30_isa32_target()")
            (match_test "ival >= 0 && ival <= 65535"))
    )
  )
)

(define_constraint "jq"
  "QI constant range"
  (and (match_code "const_int")
       (ior (match_test "(ival >= 0 && ival <= 0xFF)")
            (match_test "((-ival)  >= 0 && (-ival) <= 0xFF)")
       )
  )
)

(define_constraint "jh"
  "QI constant range"
  (and (match_code "const_int")
       (ior (match_test "(ival >= 0 && ival <= 0xFFFF)")
            (match_test "((-ival)  >= 0 && (-ival) <= 0xFFFF)")
       )
  )
)

(define_constraint "js"
  "QI constant range"
  (and (match_code "const_int")
       (ior (match_test "(ival >= 0 && ival <= 0xFFFFFFFF)")
            (match_test "((-ival)  >= 0 && (-ival) <= 0xFFFFFFFF)")
       )
  )
)

(define_constraint "P"
  "Positive 5-bit literal for dspic or positive 7-bit literal for 33A"
  (and (match_code "const_int")
    (ior
       (and (match_test "!pic30_isa32_target()")
            (match_test "ival >= 0 && ival <= 31"))
       (and (match_test "pic30_isa32_target()")
            (match_test "ival >= 0 && ival <= 127"))
    )
  )
)

(define_constraint "Kq"
  "Mode1 constant range"
  (and (match_code "const_int")
       (match_test "ival >= 0 && ival <= 7")
  )
)

(define_constraint "Kh"
  "Mode1 constant range"
  (and (match_code "const_int")
       (match_test "ival >= 0 && ival <= 15")
  )
)

(define_constraint "Ks"
  "Mode1 constant range"
  (and (match_code "const_int")
       (match_test "ival >= 0 && ival <= 31")
  )
)

(define_constraint "L"
  "constant -2, -1, -1, 2"
  (and (match_code "const_int")
       (match_test "ival != 0 && ival <= 2 && ival >= -2")
  )
)


(define_constraint "M"
  "Negative 10-bit literal for dspic or negative 16-bit literal for 33A"
  (and (match_code "const_int")
    (ior
       (and (match_test "!pic30_isa32_target()")
            (match_test "ival < 0 && ival >= -1023"))
       (and (match_test "pic30_isa32_target()")
            (match_test "ival < 0 && ival >= -65535"))
    )
  )
)

(define_constraint "N"
  "Negative 5-bit literal for dspic or negative 7-bit literal for 33A"
  (and (match_code "const_int")
    (ior
       (and (match_test "!pic30_isa32_target()")
            (match_test "ival < 0 && ival >= -31"))
       (and (match_test "pic30_isa32_target()")
            (match_test "ival < 0 && ival >= -127"))
    )
  )
)

(define_constraint "O"
  "A constant 0"
  (and (match_code "const_int")
       (match_test "ival == 0")
  )
)

(define_constraint "W"
  "A constant >= -16 and <=16"
  (and (match_code "const_int")
       (match_test "ival >= -16 && ival <= 16")
  )
)

(define_constraint "Y"
  "Constants -6, -4, -2, 2, 4, 6"
  (and (match_code "const_int")
       (match_test "ival == -6 || ival == -4 || ival == -2 || 
                    ival == 2 || ival == 4 || ival == 6")
  )
)

(define_constraint "Z"
  "Signed 4 bit literals"
  (ior
     (and (match_test "!pic30_isa32_target()")
          (match_code "const_int")
          (match_test "ival >= -8 && ival <= 7"))
     (and (match_test "pic30_isa32_target()")
          (match_code "const_int")
          (match_test "ival >= -32 && ival <= 31"))
  )
)

(define_constraint "G"
  "Floating constant for 0"
  (and (match_code "const_double")
       (match_test "op == CONST0_RTX(SFmode) || op == CONST0_RTX(DFmode)")
  )
)

(define_constraint "H"
  "Floating constant for 0"
  (and (match_code "const_double")
    (and (match_test "pic30_isa32_target()")
         (match_test "(pic30_isa32_float_const(op) >= 0)")
    )
  )
)

/*
** Optional extra constraints for this machine.
**
** For the dsPIC,
** Q: memory operand consisting of a base plus displacement.
** q: memory operand that fits pic30_symbolic_address_operand
** R: memory operand consisting of a base only.
** S: memory operand consisting of a base plus index.
** T: memory operand consisting of a direct address (far).
** U: memory operand consisting of a direct address (near).
*/

(define_memory_constraint "Q"
  "@internal memory operand consisting of a base plus displacement"
  (match_test "pic30_Q_constraint(op)")
)

/*
 * synonym for 'qs'
 *
 * (define_address_constraint "q"
 *   "@internal memory operand that fits pic30_symbolic_address_operand"
 *   (match_test "!pic30_reload_in_progress() && pic30_q_constraint(op)")
 * )
 */

(define_memory_constraint "R"
  "@internal memory operand consisting of a base only"
  (and (match_code "mem")
       (match_test "pic30_R_constraint(op)")
  )
)

(define_memory_constraint "S"
  "@internal memory operand consisting of a base plus index"
  (match_test "pic30_S_constraint(op)")
)

(define_memory_constraint "T"
  "@internal memory operand consisting of a base plus index"
  (and (match_code "mem")
       (match_test "pic30_T_constraint(op,mode)")
  )
)

(define_constraint "U"
  "@internal near memory constraint"
  (and (match_code "mem")
       (match_test "pic30_U_constraint(op,mode)")
  )
)

(define_constraint "fA"
  "@internal memory operand consisting of A reg"
  (match_test "pic30_mem_constraint(\"fA\", op)")
)

(define_constraint "fB"
  "@internal memory operand consisting of B reg"
  (match_test "pic30_mem_constraint(\"fB\", op)")
)

(define_constraint "fO"
  "@internal memory operand consisting of 0"
  (match_test "pic30_mem_constraint(\"fO\", op)")
)

(define_memory_constraint "qm"
  "@internal memory operand consisting of (MEM: q)"
  (match_test "pic30_mem_constraint(\"qm\", op)")
)

(define_constraint "qs"
  "@internal memory operand consisting of symbol ref"
  (match_test "pic30_mem_constraint(\"qs\", op)")
)
