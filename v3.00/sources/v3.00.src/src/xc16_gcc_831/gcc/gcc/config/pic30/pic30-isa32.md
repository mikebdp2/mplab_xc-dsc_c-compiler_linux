;;- Machine description for GNU compiler

;;- Copyright (C) 1994, 1995, 1996, 1997 Free Software Foundation, Inc.

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

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;	This is the pattern description for the Microchip ISA32 dsPIC.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;;- instruction definitions
;;
;;- See file "rtl.def" for documentation on define_insn, match_*, et. al.
;;
;; dsPIC30 specific remarks:
;;
;; 1) BITS_PER_UNIT = 8
;;
;; 2) GCC to dsPIC30 data type mappings:
;;    QImode => char (8 bits or 1 reg).
;;    HImode => short/int (16 bits or 1 reg).
;;    SImode => long (32 bits or 2 regs).
;;    DImode => long long (64 bits or 4 regs).
;;    SFmode => single precision float (32 bits or 2 regs).
;;    DFmode => double precision float (64 bits or 4 regs).
;; 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Condition code settings.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; clobber - value of cc is unknown
;; unchanged - insn does not affect cc
;; set - set to agree with the results of the operation
;; change0 - insn does not affect cc but it does modify operand 0
;;	cc only changed if the item previously set into the condition code
;;	has been modified.
;; math - value of cc has incorrect C and OV flags
;; move - value of cc has incorrect C and OV flags
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;; ISA32 modes
(define_mode_iterator S_INT_MODES_32 [QI HI SI P32EXT])
(define_mode_iterator D_PUSH_MODES_32 [DI])
(define_mode_iterator SINGLE_REGISTER_MODES_32 [SI SF])
(define_mode_iterator DOUBLE_REGISTER_MODES_32 [DI DF])

;
; exch
;

(define_insn "exch_32"
 [(parallel [(set (match_operand:SI 0 "pic30_register_operand" "+r")
                  (match_operand:SI 1 "pic30_register_operand" "+r"))
             (set (match_dup 1)
                  (match_dup 0))]
 )]
 ""
 "*
{
  if (REGNO(operands[1]) == REGNO(operands[0])) {
    return \"; exch %0,%1\";
  } else {
    return \"exch %0,%1\";
  }
}"
 [
   (set_attr "type" "etc")
   (set_attr "op_type" "etc")
 ]
)


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;; dsp instructions
; DSP instructions
;

; ********* to support builtins

(define_insn "addac_hi_32"
  [(set (match_operand: HI          0 "pic30_accumulator_operand" "=w")
        (plus:HI (match_operand: HI 1 "pic30_accumulator_operand" "w") 
                 (match_operand: HI 2 "pic30_accumulator_operand" "w")))]
  "(REGNO(operands[1]) != REGNO(operands[2]))"
  "add.w %0"
  [
    (set_attr "cc" "unchanged")
    (set_attr "type" "etc")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "addac2_hi_32"
  [(set (match_operand: HI          0 "pic30_accumulator_operand" "=w")
        (plus:HI (match_dup 0)
                 (match_operand: HI 1 "pic30_accumulator_operand" "w")))]
  "(REGNO(operands[1]) != REGNO(operands[0]))"
  "add.w %0"
  [
    (set_attr "cc" "unchanged")
    (set_attr "type" "etc")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "addacr_shiftrt_hi_32"
  [(set (match_operand: HI      0 "pic30_accumulator_operand" "=w")
        (plus:HI
           (ashiftrt:HI 
             (match_operand: HI 1 "pic30_mode3_operand"       " rRS<>")
             (match_operand: HI 2 "immediate_operand"         " Z"))
           (match_operand:HI    3 "pic30_accumulator_operand" " 0"))
  )]
  "((INTVAL(operands[2]) >= 0) && (satisfies_constraint_Z(operands[2])))"
  "add.w %1,#%2,%0"
  [
    (set_attr "cc" "unchanged")
    (set_attr "type" "defuse")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "addacr_shiftlt_hi_32"
  [(set (match_operand: HI               0 "pic30_accumulator_operand" "=w")
        (plus:HI
           (ashift:HI (match_operand: HI 1 "pic30_mode3_operand"       " RS<>r")
                      (match_operand: HI 2 "immediate_operand"         " i"))
           (match_operand:HI    3 "pic30_accumulator_operand"          "0")))]
  "((INTVAL(operands[2]) >= 0) && (satisfies_constraint_Z(operands[2])))"
  "add.w %1,#%J2,%0"
  [
    (set_attr "cc" "unchanged")
    (set_attr "type" "defuse")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "addacr_noshift_hi_32"
  [(set (match_operand: HI    0 "pic30_accumulator_operand" "=w")
        (plus:HI
           (match_operand: HI 1 "pic30_mode3_operand"       " RS<>r")
           (match_operand:HI  2 "pic30_accumulator_operand" " 0")))]
  ""
  "add.w %1,#0,%0"
  [
    (set_attr "cc" "unchanged")
    (set_attr "type" "defuse")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "addacr_noshift1_hi_32"
  [(set (match_operand: HI    0 "pic30_accumulator_operand" "=w")
        (plus:HI
           (match_operand:HI  1 "pic30_accumulator_operand" " 0")
           (match_operand: HI 2 "pic30_mode3_operand"       " RS<>r")))]
  ""
  "add.w %2,#0,%0"
  [
    (set_attr "cc" "unchanged")
    (set_attr "type" "defuse")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "addacr_shiftrt1_hi_32"
  [(set (match_operand: HI      0 "pic30_accumulator_operand" "=w")
        (plus:HI 
           (match_dup 0)
           (ashiftrt:HI 
             (match_operand: HI 1 "pic30_mode3_operand"       " RS<>r")
             (match_operand: HI 2 "immediate_operand"         " Z"))))]
  "((INTVAL(operands[2]) >= 0) && !(pic30_errata_mask & psv_errata) && (satisfies_constraint_Z(operands[2])))"
  "add.w %1,#%2,%0"
  [
    (set_attr "cc" "unchanged")
    (set_attr "type" "defuse")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "addacr_shiftlt1_hi_32"
  [(set (match_operand: HI      0 "pic30_accumulator_operand" "=w")
        (plus:HI
           (ashift:HI 
             (match_operand: HI 1 "pic30_mode3_operand"       " RS<>r")
             (match_operand: HI 2 "immediate_operand"         " i"))
           (match_dup 0)))]
  "((INTVAL(operands[2]) >= 0) && !(pic30_errata_mask & psv_errata))"
  "add.w %1,#%J2,%0"
  [
    (set_attr "cc" "unchanged")
    (set_attr "type" "defuse")
    (set_attr "op_type" "alu")
  ]
)

;; addac_si

(define_insn "addac_si_32"
  [(set (match_operand: SI          0 "pic30_accumulator_operand" "=w")
        (plus:SI (match_operand: SI 1 "pic30_accumulator_operand" "w")
                 (match_operand: SI 2 "pic30_accumulator_operand" "w")))]
  "(REGNO(operands[1]) != REGNO(operands[2]))"
  "add.l %0"
  [
    (set_attr "cc" "unchanged")
    (set_attr "type" "etc")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "addacr_shiftrt_si_32"
  [(set (match_operand: SI      0 "pic30_accumulator_operand" "=w")
        (plus:SI
           (ashiftrt:SI 
             (match_operand: SI 1 "pic30_mode3_operand"       " rRS<>")
             (match_operand: SI 2 "immediate_operand"         " Z"))
           (match_operand:SI    3 "pic30_accumulator_operand" " 0"))
  )]
  "((INTVAL(operands[2]) >= 0) && (satisfies_constraint_Z(operands[2])))"
  "add.l %1,#%2,%0"
  [
    (set_attr "cc" "unchanged")
    (set_attr "type" "defuse")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "addacr_shiftlt_si_32"
  [(set (match_operand: SI               0 "pic30_accumulator_operand" "=w")
        (plus:SI
           (ashift:SI (match_operand: SI 1 "pic30_mode3_operand"       " RS<>r")
                      (match_operand: SI 2 "immediate_operand"         " i"))
           (match_operand:SI    3 "pic30_accumulator_operand"          "0")))]
  "((INTVAL(operands[2]) >= 0) && (satisfies_constraint_Z(operands[2])))"
  "add.l %1,#%J2,%0"
  [
    (set_attr "cc" "unchanged")
    (set_attr "type" "defuse")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "addacr_noshift_si_32"
  [(set (match_operand:SI     0 "pic30_accumulator_operand" "=w")
        (plus:SI
           (match_operand:SI  1 "pic30_mode3_operand"       " RS<>r")
           (match_operand:SI  2 "pic30_accumulator_operand" " 0")))]
  ""
  "add.l %1,#0,%0"
  [
    (set_attr "cc" "unchanged")
    (set_attr "type" "defuse")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "addacr_noshift1_si_32"
  [(set (match_operand:SI     0 "pic30_accumulator_operand" "=w")
        (plus:SI
           (match_operand:SI  1 "pic30_accumulator_operand" " 0")
           (match_operand:SI  2 "pic30_mode3_operand"       " RS<>r")))]
  ""
  "add.l %2,#0,%0"
  [
    (set_attr "cc" "unchanged")
    (set_attr "type" "defuse")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "addacr_shiftrt1_si_32"
  [(set (match_operand:SI       0 "pic30_accumulator_operand" "=w")
        (plus:SI 
           (match_dup 0)
           (ashiftrt:SI 
             (match_operand:SI  1 "pic30_mode3_operand"       " RS<>r")
             (match_operand:SI  2 "immediate_operand"         " Z"))))]
  "((INTVAL(operands[2]) >= 0) && !(pic30_errata_mask & psv_errata) && (satisfies_constraint_Z(operands[2])))"
  "add.l %1,#%2,%0"
  [
    (set_attr "cc" "unchanged")
    (set_attr "type" "defuse")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "addacr_shiftlt1_si_32"
  [(set (match_operand:SI       0 "pic30_accumulator_operand" "=w")
        (plus:SI
           (ashift:SI 
             (match_operand:SI  1 "pic30_mode3_operand"       " RS<>r")
             (match_operand:SI  2 "immediate_operand"         " i"))
           (match_dup 0)))]
  "((INTVAL(operands[2]) >= 0) && !(pic30_errata_mask & psv_errata))"
  "add.l %1,#%J2,%0"
  [
    (set_attr "cc" "unchanged")
    (set_attr "type" "defuse")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "clrac_gen_hi_32"
  [(set (match_operand: HI 0 "pic30_accumulator_operand" "=w")
        (const_int 0))
  ]
  ""
  "clr %0"
  [
    (set_attr "cc" "unchanged")
    (set_attr "type" "def")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "clrac_gen_si_32"
  [(set (match_operand: SI 0 "pic30_accumulator_operand" "=w")
        (const_int 0))
  ]
  ""
  "clr %0"
  [
    (set_attr "cc" "unchanged")
    (set_attr "type" "def")
    (set_attr "op_type" "alu")
  ]
)

;(define_insn "ed_hi_32"
; [
;  (set (match_operand:HI 0 "pic30_accumulator_operand"      "=w, w,  w")
;       (unspec:HI [
;         (match_operand:HI 1 "pic30_dsp_word_input_operand"  "r,rR,rRQ")
;         (match_operand:HI 2 "pic30_dsp_word_input_operand"  "r,rR,rRQ")
;        ] UNSPECV_DSPMULT))
; ]
; ""
; "*
;  if (which_alternative < 2) {
;     return \"ed.w %1,%2,%0\";
;  } else {
;    char buffer[120];
;    char *c = buffer;
;    int opno;
;  
;    c += sprintf(buffer, \"ed.w \");
;    for (opno = 1; opno <=2; opno++) {
;      if (GET_CODE(operands[opno]) == REG)  {
;        c += sprintf(c, \"%%%d,\", opno);
;      } else if (GET_CODE(operands[opno]) == MEM) {
;        rtx opnd;
;
;        opnd = XEXP(operands[opno],0);
;        if (GET_CODE(opnd) == REG) {
;          c += sprintf(c, \"%%%d,\",opno);
;        } else {
;          int plus = 1;
;          rtx val = NULL_RTX;
;          int offset;
;          if (GET_CODE(opnd) == PLUS) {
;            val = XEXP(opnd,1);
;          } else if (GET_CODE(opnd) == MINUS) {
;            plus=-1;
;            val = XEXP(opnd,1);
;          } else gcc_assert(0);
;          if (GET_CODE(val) != CONST_INT) gcc_assert(0);
;          offset = INTVAL(val) * plus;
;          if (offset < 0) {
;            c += sprintf(c,\"[%%r%d-=%d],\", opno, -offset);
;          } else {
;            c += sprintf(c,\"[%%r%d+=%d],\", opno, offset);
;          }
;        }
;      }
;    }
;    return buffer;
;  }
;"
; [
;   (set_attr "cc" "unchanged")
;   (set_attr "type" "defuse,defuse,defuse")
;   (set_attr "op_type" "alu")
; ]
;)

(define_insn "ed_hi_32"
 [
  (set (match_operand:HI 0 "pic30_accumulator_operand" "=w")
       (unspec:HI [
         (match_operand:HI 1 "pic30_reg_or_R_operand"   "rR")
         (match_operand:HI 2 "pic30_reg_or_R_operand"   "rR")
        ] UNSPECV_DSPMULT))
 ]
 ""
 "ed.w %1,%2,%0"
 [
   (set_attr "cc" "unchanged")
   (set_attr "type" "defuse")
   (set_attr "op_type" "alu")
 ]
)


;(define_insn "ed_si_32"
; [
;  (set (match_operand:SI 0 "pic30_accumulator_operand"      "=w, w,  w")
;       (unspec:SI [
;         (match_operand:SI 1 "pic30_dsp_word_input_operand"  "r,rR,rRQ")
;         (match_operand:SI 2 "pic30_dsp_word_input_operand"  "r,rR,rRQ")
;        ] UNSPECV_DSPMULT))
; ]
; ""
; "*
;  if (which_alternative < 2) {
;     return \"ed.l %1,%2,%0\";
;  } else {
;    char buffer[120];
;    char *c = buffer;
;    int opno;
;  
;    c += sprintf(buffer, \"ed.w \");
;    for (opno = 1; opno <=2; opno++) {
;      if (GET_CODE(operands[opno]) == REG)  {
;        c += sprintf(c, \"%%%d,\", opno);
;      } else if (GET_CODE(operands[opno]) == MEM) {
;        rtx opnd;
;
;        opnd = XEXP(operands[opno],0);
;        if (GET_CODE(opnd) == REG) {
;          c += sprintf(c, \"%%%d,\",opno);
;        } else {
;          int plus = 1;
;          rtx val = NULL_RTX;
;          int offset;
;          if (GET_CODE(opnd) == PLUS) {
;            val = XEXP(opnd,1);
;          } else if (GET_CODE(opnd) == MINUS) {
;            plus=-1;
;            val = XEXP(opnd,1);
;          } else gcc_assert(0);
;          if (GET_CODE(val) != CONST_INT) gcc_assert(0);
;          offset = INTVAL(val) * plus;
;          if (offset < 0) {
;            c += sprintf(c,\"[%%r%d-=%d],\", opno, -offset);
;          } else {
;            c += sprintf(c,\"[%%r%d+=%d],\", opno, offset);
;          }
;        }
;      }
;    }
;    return buffer;
;  }
;"
; [
;   (set_attr "cc" "unchanged")
;   (set_attr "type" "defuse,defuse,defuse")
;   (set_attr "op_type" "alu")
; ]
;)

(define_insn "ed_si_32"
 [
  (set (match_operand:SI 0 "pic30_accumulator_operand" "=w")
       (unspec:SI [
         (match_operand:SI 1 "pic30_reg_or_R_operand"   "rR")
         (match_operand:SI 2 "pic30_reg_or_R_operand"   "rR")
        ] UNSPECV_DSPMULT))
 ]
 ""
 "ed.l %1,%2,%0"
 [
   (set_attr "cc" "unchanged")
   (set_attr "type" "defuse")
   (set_attr "op_type" "alu")
 ]
)

;(define_insn "ed_si_nonononono_32"
; [
;  (set (match_operand:SI       0 "pic30_accumulator_operand"      "=w")
;       (unspec:SI [
;         (match_operand:SI     1 "pic30_reg_or_R_operand"         "%rR")
;         (mem:SI 
;           (plus:SI
;             (match_operand:SI 2 "pic30_register_operand"         "+r")
;             (match_operand:SI 3 "pic30_dsp_word_offset_operand"  " i")))
;        ] UNSPECV_DSPMULT))
;  (set (match_dup 2)
;       (plus:SI
;          (match_dup 2)
;          (match_dup 3)))
; ]
; ""
; "*
;  if (1) {
;    char buffer[120];
;    char *c = buffer;
;    int offset = INTVAL(operands[3]);
;  
;    c += sprintf(buffer, \"ed.w %%1,\");
;    if (offset < 0)
;      c += sprintf(c, \"[%%2-=%d],%%0\", -offset);
;    else 
;      c += sprintf(c, \"[%%2+=%d],%%0\", offset);
;    
;    return buffer;
;  }
;"
; [
;   (set_attr "cc" "unchanged")
;   (set_attr "type" "defuse")
;   (set_attr "op_type" "alu")
; ]
;)

(define_insn "edac_hi_32"
 [
  (set (match_operand:HI 0 "pic30_accumulator_operand" "=w,w,w,w")
       (plus: HI
         (match_operand:HI 1 "pic30_accumulator_operand" "0,0,0,0")
         (unspec:HI [
             (match_operand:HI 2 "pic30_mac_input_operand" "za,za,za,za")
             (match_dup 2)
           ] UNSPECV_DSPMULT)))
  (set (match_operand:HI 3 "pic30_mac_input_operand" "=za,za,za,za")
       (minus: HI
         (mem: HI (match_operand:HI 4 "pic30_xprefetch_operand" "x,x,x,x"))
         (mem: HI (match_operand:HI 5 "pic30_yprefetch_operand" "y,y,y,y"))))
  (set (match_operand:HI 6 "pic30_xprefetch_operand" "=4,4,B,B")
       (plus: HI
         (match_dup 6)
         (match_operand: HI 7 "immediate_operand" "Y,Y,i,i")))
  (set (match_operand:HI 8 "pic30_yprefetch_operand" "=5,B,B,5")
       (plus: HI
         (match_dup 8)
         (match_operand: HI 9 "immediate_operand" "Y,i,i,Y")))
 ]
 ""
 "@
  edac %2*%2, %0, [%4]+=%7, [%5]+=%9, %3
  edac %2*%2, %0, [%4]+=%6, [%5], %3
  edac %2*%2, %0, [%4], [%5], %3
  edac %2*%2, %0, [%4], [%5]+=%9, %3"
 [
   (set_attr "cc" "unchanged")
   (set_attr "type" "defuse,defuse,defuse,defuse")
   (set_attr "op_type" "alu")
 ]
)

(define_insn "fbcl_hi_32"
 [ 
  (set (match_operand:SI 0 "pic30_register_operand" "=r,r")
       (unspec: SI [
                     (match_operand: HI 1 "pic30_mode2_operand" "r,R<>")
                   ] UNSPECV_FBCL))
 ]
 ""
 "fbcl.w %1, %0"
 [
  (set_attr "cc" "clobber")
  (set_attr "type" "etc,defuse")
  (set_attr "op_type" "alu")
 ]
)

(define_insn "fbcl_si_32"
 [ 
  (set (match_operand:SI 0 "pic30_register_operand" "=r,r")
       (unspec: SI [
                     (match_operand: SI 1 "pic30_mode2_operand" "r,R<>")
                   ] UNSPECV_FBCL))
 ]
 ""
 "fbcl.l %1, %0"
 [
  (set_attr "cc" "clobber")
  (set_attr "type" "etc,defuse")
  (set_attr "op_type" "alu")
 ]
)

(define_insn "lac_si_32"
  [
  (set (match_operand:SI    0 "pic30_accumulator_operand" "=w")
       (unspec:SI [
         (match_operand: SI 1 "pic30_mode3_operand"       " rRS<>")
         (match_operand: SI 2 "immediate_operand"         " W")
       ] UNSPECV_LAC))
  ]
  "pic30_isa32_target()"
  "lac.l %1,#%2,%0"

  [
    (set_attr "cc" "unchanged")
    (set_attr "type" "defuse")
    (set_attr "op_type" "mov")
  ]
)

(define_insn "lac_error_32"
  [
  (set (match_operand:HI    0 "pic30_accumulator_operand" "=w")
       (unspec:HI [
         (match_operand: HI 1 "pic30_accumulator_operand" " w")
         (match_operand: HI 2 "immediate_operand"         " Z")
       ] UNSPECV_LAC))
  ]
  "satisfies_constraint_Z(operands[2])"
  "*
   {
     error(\"Argument 0 should not be an accumulator register\");
     return \"cannot generate instruction\";
   }
  "
)


(define_insn "acclhi_32"
  [
   (set (match_operand: HI   0 "pic30_register_operand"   "=r")
        (unspec:HI [
          (match_operand: HI 1 "pic30_accumulator_operand" "w")
        ] UNSPEC_ACCL))
  ]
  ""
  "mov.w %m1L,%0"
  [
    (set_attr "cc" "change0")
    (set_attr "type" "def")
    (set_attr "op_type" "mov")
  ]
)

(define_insn "acchhi_32"
  [
   (set (match_operand: HI   0 "pic30_register_operand"   "=r")
        (unspec:HI [
          (match_operand: HI 1 "pic30_accumulator_operand" "w")
        ] UNSPEC_ACCH))
  ]
  ""
  "mov.w %m1H,%0"
  [
    (set_attr "cc" "change0")
    (set_attr "type" "def")
    (set_attr "op_type" "mov")
  ]
)

(define_insn "accuhi_32"
  [
   (set (match_operand: HI   0 "pic30_register_operand"   "=r")
        (unspec:HI [
          (match_operand: HI 1 "pic30_accumulator_operand" "w")
        ] UNSPEC_ACCU))
  ]
  ""
  "mov.w %m1U,%0"
  [
    (set_attr "cc" "change0")
    (set_attr "type" "def")
    (set_attr "op_type" "mov")
  ]
)

(define_insn "sac_gen_si_32"
  [
   (set (match_operand: SI   0 "pic30_mode3_operand"      "=RS<>,r")
        (unspec:SI [
          (match_operand: SI 1 "pic30_accumulator_operand" "w,   w")
          (match_operand: SI 2 "immediate_operand"         "W,   W")
        ] UNSPECV_SAC))
  ]
  "pic30_isa32_target()"
  "sac.l %1, #%2, %0"
  [
    (set_attr "cc" "change0")
    (set_attr "type" "etc,def")
    (set_attr "op_type" "mov")
  ]
)

(define_insn "sacr_gen_si_32"
  [
   (set (match_operand: SI 0 "pic30_mode3_operand"                    "=RS<>,r")
        (unspec:SI [
                     (match_operand: SI 1 "pic30_accumulator_operand" "w,w")
		     (match_operand: SI 2 "immediate_operand"         "Z,Z")
                   ] UNSPECV_SACR))
  ]
  "pic30_isa32_target()"
  "sacr.l %1, #%2, %0"
  [
    (set_attr "cc" "change0")
    (set_attr "type" "etc,def")
    (set_attr "op_type" "mov")
  ]
)

(define_insn "mac_gen_hi_32"
  [(set (match_operand: HI 0 "pic30_accumulator_operand"  "=w,w,w,w,w,w,w,w,w")
        (plus:HI 
          (match_operand: HI 1 "pic30_accumulator_operand" "0,0,0,0,0,0,0,0,0")
          (mult:HI 
            (match_operand:HI 2 "pic30_mac_input_operand"  "za,za,za,za,za,za,za,za,za")
            (match_operand:HI 3 "pic30_mac_input_operand"  "za,za,za,za,za,za,za,za,za")
            )))
   (set (match_operand: HI 4 "pic30_mac_input_operand"    "=za,B,B,za,za,za,za,za,B")
        (mem:HI
          (match_operand: HI 5 "pic30_xprefetch_operand"   "x,B,B,x,x,x,x,x,B")))
   (set (match_operand: HI 6 "pic30_xprefetch_operand"    "=5,B,B,5,B,B,5,B,B")
        (plus:HI
          (match_dup 6)
          (match_operand: HI 7 "immediate_operand"        " Y,i,i,Y,i,i,Y,i,i")))
   (set (match_operand: HI 8 "pic30_mac_input_operand"    "=za,za,za,B,B,za,za,za,B")
        (mem:HI
          (match_operand: HI 9 "pic30_yprefetch_operand"  " y,y,y,B,B,y,y,y,B")))
   (set (match_operand: HI 10 "pic30_yprefetch_operand"   "=9,9,B,B,B,B,B,9,B")
        (plus:HI
          (match_dup 10)
          (match_operand: HI 11 "immediate_operand"      "  Y,Y,i,i,i,i,i,Y,i")))
  ]
  "" 
  "* 
   {
     static const char *mac_options[] = {
       \"mac %2*%3, %0, [%5]+=%7, %4, [%9]+=%11, %8\",  /* 0 */
       \"mac %2*%3, %0, [%9]+=%11, %8\",                /* 1 */
       \"mac %2*%3, %0, [%9], %8\",                     /* 2 */
       \"mac %2*%3, %0, [%5]+=%7, %4\",                 /* 3 */
       \"mac %2*%3, %0, [%5], %4\",                     /* 4 */
       \"mac %2*%3, %0, [%5], %4, [%9], %8\",           /* 5 */
       \"mac %2*%3, %0, [%5]+=%7, %4, [%9], %8\",       /* 6 */
       \"mac %2*%3, %0, [%5], %4, [%9]+=%11, %8\",      /* 7 */
       \"mac %2*%3, %0\",                               /* 8 */
       \"mac %3*%2, %0, [%5]+=%7, %4, [%9]+=%11, %8\",
       \"mac %3*%2, %0, [%9]+=%11, %8\",
       \"mac %3*%2, %0, [%9], %8\",
       \"mac %3*%2, %0, [%5]+=%7, %4\",
       \"mac %3*%2, %0, [%5], %4\",
       \"mac %3*%2, %0, [%5], %4, [%9], %8\",
       \"mac %3*%2, %0, [%5]+=%7, %4, [%9], %8\",
       \"mac %3*%2, %0, [%5], %4, [%9]+=%11, %8\",
       \"mac %3*%2, %0\" };

     if (REGNO(operands[1]) < REGNO(operands[2])) {
       return mac_options[which_alternative];
     } else {
       return mac_options[which_alternative+9];
     }
   }
  "
  [
    (set_attr "cc" "unchanged")
    (set_attr "type" "defuse,defuse,defuse,defuse,defuse,defuse,defuse,defuse,etc")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "store_acc_stack_32"
  [
   (parallel [
     (set (mem:SI (reg:SI SPREG))
        (match_operand: SI 0 "pic30_accumulator_operand" "w"))
     (set (reg:SI SPREG)
          (plus:SI
             (reg:SI SPREG)
             (const_int 12)))]
  )]
  "pic30_isa32_target()"
  "slac.l %0,#0,[w15++]\;suac.l %0,#0,[w15++]\;sac.l %0,#0,[w15++]"
  [
    (set_attr "cc" "unchanged")
    (set_attr "type" "defuse")
  ]    
)

(define_insn "load_acc_stack_32"
  [
   (parallel [
     (set (mem:SI (reg:SI SPREG))
        (match_operand: SI 0 "pic30_accumulator_operand" "w"))
     (set (reg:SI SPREG)
          (minus:SI
             (reg:SI SPREG)
             (const_int 12)))]
  )]
  "pic30_isa32_target()"
  "lac.l [--w15],#0,%0\;luac.l [--w15],#0,%0\;llac.l [--w15],#0,%0"
  [
    (set_attr "cc" "unchanged")
    (set_attr "type" "defuse")
  ]
)

;
; GCC had trouble respecting the pre-condition; for now restrict the
;   register choices so that u and t don't overlap... this can be improved.
;   also for msc_gen_hi and msc_awbgen_hi
;
(define_insn "macawb_gen_hi_32"
  [(set (match_operand: HI 0 "pic30_accumulator_operand"   "=w,w,w,w,w,w,w,w,w")
        (plus:HI
          (match_operand: HI 1 "pic30_accumulator_operand"  "0,0,0,0,0,0,0,0,0")
          (mult:HI
            (match_operand:HI 2 "pic30_mac_input_operand"  " u,u,u,u,u,u,u,u,u")
            (match_operand:HI 3 "pic30_mac_input_operand"  " t,t,t,t,t,t,t,t,t")
   )))
   (set (match_operand: HI 4 "pic30_mac_input_operand"     "=za,B,B,za,za,za,za,za,B")
        (mem:HI
          (match_operand: HI 5 "pic30_xprefetch_operand"   " x,B,B,x,x,x,x,x,B")
   ))
   (set (match_operand: HI 6 "pic30_xprefetch_operand"     "=5,B,B,5,B,B,5,B,B")
        (plus:HI
          (match_dup 6)
          (match_operand: HI 7 "immediate_operand"         " Y,i,i,Y,i,i,Y,i,i")
   ))
   (set (match_operand: HI 8 "pic30_mac_input_operand"     "=za,za,za,B,B,za,za,za,B")
        (mem:HI
          (match_operand: HI 9 "pic30_yprefetch_operand"   " y,y,y,B,B,y,y,y,B")
   ))
   (set (match_operand: HI 10 "pic30_yprefetch_operand"    "=9,9,B,B,B,B,B,9,B")
        (plus:HI
          (match_dup 10)
          (match_operand: HI 11 "immediate_operand"        " Y,Y,i,i,i,i,i,Y,i")
   ))
   (set (match_operand: HI 12 "pic30_awb_operand"         "=v,v,v,v,v,v,v,v,v")
        (unspec:HI [
          (match_operand:HI 13 "pic30_accumulator_operand" " w,w,w,w,w,w,w,w,w")
          (const_int 0)
        ] UNSPECV_SAC))
  ]
  "(pic30_isa32_target()) && (REGNO(operands[1]) != REGNO(operands[2]))"
  "*
   {
     const char *mac_options[] = { 
       \"mac %2*%3, %0, [%5]+=%7, %4, [%8]+=%11, %8, %12\",              /* 0 */
       \"mac %2*%3, %0, [%9]+=%11, %8, %12\",                            /* 1 */
       \"mac %2*%3, %0, [%9], %8, %12\",                                 /* 2 */
       \"mac %2*%3, %0, [%5]+=%7, %4, %12\",                             /* 3 */
       \"mac %2*%3, %0, [%5], %4, %12\",                                 /* 4 */
       \"mac %2*%3, %0, [%5], %4, [%9], %8, %12\",                       /* 5 */
       \"mac %2*%3, %0, [%5]+=%7, %4, [%9], %8, %12\",                   /* 6 */
       \"mac %2*%3, %0, [%5], %4, [%9]+=%11, %8, %12\",                  /* 7 */
       \"mac %2*%3, %0, %12\",                                           /* 8 */
       \"mac %3*%2, %0, [%5]+=%7, %4, [%9]+=%11, %8, %12\",
       \"mac %3*%2, %0, [%9]+=%11, %8, %12\",
       \"mac %3*%2, %0, [%9], %8, %12\",
       \"mac %3*%2, %0, [%5]+=%7, %4, %12\",
       \"mac %3*%2, %0, [%5], %4, %12\",
       \"mac %3*%2, %0, [%5], %4, [%9], %8, %12\",
       \"mac %3*%2, %0, [%5]+=%7, %4, [%9], %8, %12\",
       \"mac %3*%2, %0, [%5], %4, [%9]+=%11, %8, %12\",
       \"mac %3*%2, %0, %12\" };

     if (REGNO(operands[1]) < REGNO(operands[2])) {
       return mac_options[which_alternative];
     } else {
       return mac_options[which_alternative+9];
     }
   }
  "
  [
    (set_attr "cc" "unchanged")
    (set_attr "type" "defuse,defuse,defuse,defuse,defuse,defuse,defuse,defuse,etc")
    (set_attr "op_type" "alu")
  ]
)

;
;  if an AWB is not specified, then it doesn't matter which accumulator we use
;    since the accumulator portion is only there to specify which ACC is 
;    written back
;
(define_insn "movsac_gen_hi_32"
  [ (set (match_operand: HI 0 "pic30_mac_input_operand"  "=za,B,B,za,za,za,za,za,B")
        (mem:HI
          (match_operand: HI 1 "pic30_xprefetch_operand" " x,B,B,x,x,x,x,x,B")))
   (set (match_operand: HI 2 "pic30_xprefetch_operand"   "=1,B,B,1,X,X,1,B,B")
        (plus:HI
          (match_dup 2)
          (match_operand: HI 3 "immediate_operand"       " Y,i,i,Y,i,i,Y,i,i")))
   (set (match_operand: HI 4 "pic30_mac_input_operand"   "=za,za,za,B,B,za,za,za,B")
        (mem:HI
          (match_operand: HI 5 "pic30_yprefetch_operand" " y,y,y,B,B,y,y,y,B")))
   (set (match_operand: HI 6 "pic30_yprefetch_operand"   "=5,5,B,B,B,B,B,5,B")
        (plus:HI
          (match_dup 6)
          (match_operand: HI 7 "immediate_operand"       " Y,Y,i,i,i,i,i,Y,i")))
  ]
  ""
  "@
   movsac A, [%1]+=%3, %0, [%5]+=%7, %4
   movsac A, [%5]+=%7, %4
   movsac A, [%5], %4
   movsac A, [%1]+=%3, %0
   movsac A, [%1], %0
   movsac A, [%1], %0, [%5], %4
   movsac A, [%1]+=%3, %0, [%5], %4
   movsac A, [%1], %0, [%5]+=%7, %4
   movsac A" 
  [
    (set_attr "cc" "unchanged")
    (set_attr "type" "defuse,defuse,defuse,defuse,defuse,defuse,defuse,defuse,etc")
    (set_attr "op_type" "mov")
  ] 
) 

;
;  this is an odd instruction
;
(define_insn "movsacawb_gen_hi_32"
  [(set (match_operand: HI 0 "pic30_mac_input_operand"   "=za,B,B,za,za,za,za,za,B")
        (mem:HI
          (match_operand: HI 1 "pic30_xprefetch_operand" " x,B,B,x,x,x,x,x,B")))
   (set (match_operand: HI 2 "pic30_xprefetch_operand"   "=1,B,B,1,B,B,1,B,B")
        (plus:HI
          (match_dup 2)
          (match_operand: HI 3 "immediate_operand"       " Y,i,i,Y,i,i,Y,i,i")))
   (set (match_operand: HI 4 "pic30_mac_input_operand"   "=za,za,za,B,B,za,za,za,B")
        (mem:HI
          (match_operand: HI 5 "pic30_yprefetch_operand" " y,y,y,B,B,y,y,y,B")))
   (set (match_operand: HI 6 "pic30_yprefetch_operand"   "=5,5,B,B,B,B,B,5,B")
        (plus:HI
          (match_dup 6)
          (match_operand: HI 7 "immediate_operand"       " Y,Y,i,i,i,i,i,Y,i")))
   (set (match_operand: HI 8 "pic30_awb_operand"         "=v,v,v,v,v,v,v,v,v")
        (unspec:HI [
          (match_operand: HI 9 "pic30_accumulator_operand" " w,w,w,w,w,w,w,w,w")
          (const_int 0)
        ] UNSPECV_SAC))
  ]
  ""
  "@
   movsac %A9, [%1]+=%3, %0, [%5]+=%7, %4, %8
   movsac %A9, [%5]+=%7, %4, %8
   movsac %A9, [%5], %4, %8
   movsac %A9, [%1]+=%3, %0, %8
   movsac %A9, [%1], %0, %8
   movsac %A9, [%1], %0, [%5], %4, %8
   movsac %A9, [%1]+=%3, %0, [%5], %4, %8
   movsac %A9, [%1], %0, [%5]+=%7, %4, %8
   movsac %A9, %8"
  [
    (set_attr "cc" "unchanged")
    (set_attr "type" "defuse,defuse,defuse,defuse,defuse,defuse,defuse,defuse,etc")
    (set_attr "op_type" "mov")
  ]
)

(define_insn "mpy_gen_hi_32"
  [(set (match_operand: HI 0 "pic30_accumulator_operand" "=w,w,w,w,w,w,w,w,w")
        (unspec:HI [
            (match_operand:HI 1 "pic30_mac_input_operand"  " za,za,za,za,za,za,za,za,za")
            (match_operand:HI 2 "pic30_mac_input_operand"  " za,za,za,za,za,za,za,za,za")
          ] UNSPECV_DSPMULT))
   (set (match_operand: HI 3 "pic30_mac_input_operand"   "=za,B,B,za,za,za,za,za,B")
        (mem:HI
          (match_operand: HI 4 "pic30_xprefetch_operand" " x,B,B,x,x,x,x,x,B")))
   (set (match_operand: HI 5 "pic30_xprefetch_operand"   "=4,B,B,4,B,B,4,B,B")
        (plus:HI
          (match_dup 5)
          (match_operand: HI 6 "immediate_operand"       " Y,i,i,Y,i,i,Y,i,i")))
   (set (match_operand: HI 7 "pic30_mac_input_operand"   "=za,za,za,B,B,za,za,za,B")
        (mem:HI
          (match_operand: HI 8 "pic30_yprefetch_operand" " y,y,y,B,B,y,y,y,B")))
   (set (match_operand: HI 9 "pic30_yprefetch_operand"   "=8,8,B,B,B,B,B,8,B")
        (plus:HI
          (match_dup 9)
          (match_operand: HI 10 "immediate_operand"      " Y,Y,i,i,i,i,i,Y,i")))
  ]
  "" 
  "* 
   {
     const char *mpy_options[] = {
       \"mpy %1*%2, %0, [%4]+=%6, %3, [%8]+=%10, %7\", /* 0 */
       \"mpy %1*%2, %0, [%8]+=%10, %7\",               /* 1 */
       \"mpy %1*%2, %0, [%8], %7\",                    /* 2 */
       \"mpy %1*%2, %0, [%4]+=%6, %3\",                /* 3 */
       \"mpy %1*%2, %0, [%4], %3\",                    /* 4 */
       \"mpy %1*%2, %0, [%4], %3, [%8], %7\",          /* 5 */
       \"mpy %1*%2, %0, [%4]+=%6, %3, [%8], %7\",      /* 6 */
       \"mpy %1*%2, %0, [%4], %3, [%8]+=%10, %7\",     /* 7 */
       \"mpy %1*%2, %0\",                              /* 8 */
       \"mpy %2*%1, %0, [%4]+=%6, %3, [%8]+=%10, %7\",
       \"mpy %2*%1, %0, [%8]+=%10, %7\",
       \"mpy %2*%1, %0, [%8], %7\",
       \"mpy %2*%1, %0, [%4]+=%6, %3\",
       \"mpy %2*%1, %0, [%4], %3\",
       \"mpy %2*%1, %0, [%4], %3, [%8], %7\",
       \"mpy %2*%1, %0, [%4]+=%6, %3, [%8], %7\",
       \"mpy %2*%1, %0, [%4], %3, [%8]+=%10, %7\",
       \"mpy %2*%1, %0\"};

     if (REGNO(operands[1]) < REGNO(operands[2])) {
       return mpy_options[which_alternative];
     } else { 
       return mpy_options[which_alternative+9];
     }
   }
  "
  [
    (set_attr "cc" "unchanged")
    (set_attr "type" "defuse,defuse,defuse,defuse,defuse,defuse,defuse,defuse,etc")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "mpyn_gen_hi_32"
  [(set (match_operand: HI 0 "pic30_accumulator_operand" "=w,w,w,w,w,w,w,w,w")
        (unspec:HI [
           (neg:HI
             (match_operand:HI 1 "pic30_mac_input_operand"" za,za,za,za,za,za,za,za,za"))
           (match_operand:HI 2 "pic30_mac_input_operand"  " za,za,za,za,za,za,za,za,za")
          ] UNSPECV_DSPMULT))
   (set (match_operand: HI 3 "pic30_mac_input_operand"   "=za,B,B,za,za,za,za,za,B")
        (mem:HI
          (match_operand: HI 4 "pic30_xprefetch_operand" " x,B,B,x,x,x,x,x,B")))
   (set (match_operand: HI 5 "pic30_xprefetch_operand"   "=4,B,B,4,B,B,4,B,B")
        (plus:HI
          (match_dup 5)
          (match_operand: HI 6 "immediate_operand"       " Y,i,i,Y,i,i,Y,i,i")))
   (set (match_operand: HI 7 "pic30_mac_input_operand"   "=za,za,za,B,B,za,za,za,B")
        (mem:HI
          (match_operand: HI 8 "pic30_yprefetch_operand" " y,y,y,B,B,y,y,y,B")))
   (set (match_operand: HI 9 "pic30_yprefetch_operand"   "=8,8,B,B,B,B,B,8,B")
        (plus:HI
          (match_dup 9)
          (match_operand: HI 10 "immediate_operand"      " Y,Y,i,i,i,i,i,Y,i")))
  ]
  "(REGNO(operands[1]) < REGNO(operands[2]))"
  "*
   {
     const char *mpy_options[] = {
       \"mpy.n %1*%2, %0, [%4]+=%6, %3, [%8]+=%10, %7\", /* 0 */
       \"mpy.n %1*%2, %0, [%8]+=%10, %7\",               /* 1 */
       \"mpy.n %1*%2, %0, [%8], %7\",                    /* 2 */
       \"mpy.n %1*%2, %0, [%4]+=%6, %3\",                /* 3 */
       \"mpy.n %1*%2, %0, [%4], %3\",                    /* 4 */
       \"mpy.n %1*%2, %0, [%4], %3, [%8], %7\",          /* 5 */
       \"mpy.n %1*%2, %0, [%4]+=%6, %3, [%8], %7\",      /* 6 */
       \"mpy.n %1*%2, %0, [%4], %3, [%8]+=%10, %7\",     /* 7 */
       \"mpy.n %1*%2, %0\",                              /* 8 */
       \"mpy.n %2*%1, %0, [%4]+=%6, %3, [%8]+=%10, %7\",
       \"mpy.n %2*%1, %0, [%8]+=%10, %7\",
       \"mpy.n %2*%1, %0, [%8], %7\",
       \"mpy.n %2*%1, %0, [%4]+=%6, %3\",
       \"mpy.n %2*%1, %0, [%4], %3\",
       \"mpy.n %2*%1, %0, [%4], %3, [%8], %7\",
       \"mpy.n %2*%1, %0, [%4]+=%6, %3, [%8], %7\",
       \"mpy.n %2*%1, %0, [%4], %3, [%8]+=%10, %7\",
       \"mpy.n %2*%1, %0\"};

     if (REGNO(operands[1]) < REGNO(operands[2])) {
       return mpy_options[which_alternative];
     } else {
       return mpy_options[which_alternative+9];
     }
   }
  "
  [
    (set_attr "cc" "unchanged")
    (set_attr "type" "defuse,defuse,defuse,defuse,defuse,defuse,defuse,defuse,etc")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "msc_gen_hi_32"
  [(set (match_operand: HI 0 "pic30_accumulator_operand"   "=w,w,w,w,w,w,w,w,w")
        (minus:HI 
          (match_operand: HI 1 "pic30_accumulator_operand"  "0,0,0,0,0,0,0,0,0")
          (unspec:HI [
            (match_operand:HI 2 "pic30_mac_input_operand"  " u,u,u,u,u,u,u,u,u")
            (match_operand:HI 3 "pic30_mac_input_operand"  " t,t,t,t,t,t,t,t,t")
          ] UNSPECV_DSPMULT)))
   (set (match_operand: HI 4 "pic30_mac_input_operand"     "=za,B,B,za,za,za,za,za,B")
        (mem:HI
          (match_operand: HI 5 "pic30_xprefetch_operand"   " x,B,B,x,x,x,x,x,B")
   ))
   (set (match_operand: HI 6 "pic30_xprefetch_operand"     "=5,B,B,5,B,B,5,B,B")
        (plus:HI
          (match_dup 6)
          (match_operand: HI 7 "immediate_operand"         " Y,i,i,Y,i,i,Y,i,i")
   ))
   (set (match_operand: HI 8 "pic30_mac_input_operand"     "=za,za,za,B,B,za,za,za,B")
        (mem:HI
          (match_operand: HI 9 "pic30_yprefetch_operand"   " y,y,y,B,B,y,y,y,B")
   ))
   (set (match_operand: HI 10 "pic30_yprefetch_operand"    "=9,9,B,B,B,B,B,9,B")
        (plus:HI
          (match_dup 10)
          (match_operand: HI 11 "immediate_operand"        " Y,Y,i,i,i,i,i,Y,i")
   ))
  ]
  "" 
  "* 
   {
     const char *msc_options[] = {
       \"msc %2*%3, %0, [%5]+=%7, %4, [%9]+=%11, %8\",
       \"msc %2*%3, %0, [%9]+=%11, %8\",
       \"msc %2*%3, %0, [%9], %8\",
       \"msc %2*%3, %0, [%5]+=%7, %4\",
       \"msc %2*%3, %0, [%5], %4\",
       \"msc %2*%3, %0, [%5], %4, [%9], %8\",
       \"msc %2*%3, %0, [%5]+=%7, %4, [%9], %8\",
       \"msc %2*%3, %0, [%5], %4, [%9]+=%11, %8\",
       \"msc %2*%3, %0\",
       \"msc %3*%2, %0, [%5]+=%7, %4, [%9]+=%11, %8\",
       \"msc %3*%2, %0, [%9]+=%11, %8\",
       \"msc %3*%2, %0, [%9], %8\",
       \"msc %3*%2, %0, [%5]+=%7, %4\",
       \"msc %3*%2, %0, [%5], %4\",
       \"msc %3*%2, %0, [%5], %4, [%9], %8\",
       \"msc %3*%2, %0, [%5]+=%7, %4, [%9], %8\",
       \"msc %3*%2, %0, [%5], %4, [%9]+=%11, %8\",
       \"msc %3*%2, %0\"};

     if (REGNO(operands[1]) < REGNO(operands[2])) {
       return msc_options[which_alternative];
     } else { 
       return msc_options[which_alternative+9];
     }
   }
  "
  [
    (set_attr "cc" "unchanged")
    (set_attr "type" "defuse,defuse,defuse,defuse,defuse,defuse,defuse,defuse,etc")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "mscawb_gen_hi_32"
  [(set (match_operand: HI 0 "pic30_accumulator_operand"   "=w,w,w,w,w,w,w,w,w")
        (minus:HI
          (match_operand: HI 1 "pic30_accumulator_operand"  "0,0,0,0,0,0,0,0,0")
          (unspec:HI [
            (match_operand:HI 2 "pic30_mac_input_operand"  " u,u,u,u,u,u,u,u,u")
            (match_operand:HI 3 "pic30_mac_input_operand"  " t,t,t,t,t,t,t,t,t")
          ] UNSPECV_DSPMULT)))
   (set (match_operand: HI 4 "pic30_mac_input_operand"     "=za,B,B,za,za,za,za,za,B")
        (mem:HI
          (match_operand: HI 5 "pic30_xprefetch_operand"   " x,B,B,x,x,x,x,x,B")
   ))
   (set (match_operand: HI 6 "pic30_xprefetch_operand"     "=5,B,B,5,B,B,5,B,B")
        (plus:HI
          (match_dup 6)
          (match_operand: HI 7 "immediate_operand"         " Y,i,i,Y,i,i,Y,i,i")
   ))
   (set (match_operand: HI 8 "pic30_mac_input_operand"     "=za,za,za,B,B,za,za,za,B")
        (mem:HI
          (match_operand: HI 9 "pic30_yprefetch_operand"   " y,y,y,B,B,y,y,y,B")
   ))
   (set (match_operand: HI 10 "pic30_yprefetch_operand"    "=9,9,B,B,B,B,B,9,B")
        (plus:HI
          (match_dup 10)
          (match_operand: HI 11 "immediate_operand"        " Y,Y,i,i,i,i,i,Y,i")
   ))
   (set (match_operand: HI 12 "pic30_awb_operand"         "=v,v,v,v,v,v,v,v,v")
        (unspec:HI [
          (match_operand:HI 13 "pic30_accumulator_operand" " w,w,w,w,w,w,w,w,w")
          (const_int 0)
        ] UNSPECV_SAC)
   )
  ]
  ""
  "*
   {
     const char *msc_options[] = {
       \"msc %2*%3, %0, [%5]+=%7, %4, [%9]+=%11, %8, %12\",
       \"msc %2*%3, %0, [%9]+=%11, %8, %12\",
       \"msc %2*%3, %0, [%9], %8, %12\",
       \"msc %2*%3, %0, [%5]+=%7, %4, %12\",
       \"msc %2*%3, %0, [%5], %4, %12\",
       \"msc %2*%3, %0, [%5], %4, [%9], %8, %12\",
       \"msc %2*%3, %0, [%5]+=%7, %4, [%9], %8, %12\",
       \"msc %2*%3, %0, [%5], %4, [%9]+=%11, %8, %12\",
       \"msc %2*%3, %0, %12\",
       \"msc %3*%2, %0, [%5]+=%7, %4, [%9]+=%11, %8, %12\",
       \"msc %3*%2, %0, [%9]+=%11, %8, %12\",
       \"msc %3*%2, %0, [%9], %8, %12\",
       \"msc %3*%2, %0, [%5]+=%7, %4, %12\",
       \"msc %3*%2, %0, [%5], %4, %12\",
       \"msc %3*%2, %0, [%5], %4, [%9], %8, %12\",
       \"msc %3*%2, %0, [%5]+=%7, %4, [%9], %8, %12\",
       \"msc %3*%2, %0, [%5], %4, [%9]+=%11, %8, %12\",
       \"msc %3*%2, %0, %12\"};

     if (REGNO(operands[1]) < REGNO(operands[2])) {
       return msc_options[which_alternative];
     } else {
       return msc_options[which_alternative+9];
     }
   }
  " 
  [
    (set_attr "cc" "unchanged")
    (set_attr "type" "defuse,defuse,defuse,defuse,defuse,defuse,defuse,defuse,etc")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "sftac_gen_32"
  [(set (match_operand: SI 0 "pic30_accumulator_operand" "=w,w")
        (unspec:SI [
                     (match_operand: SI 1 "pic30_accumulator_operand" "0,0")
                     (match_operand: SI 2 "pic30_reg_or_imm_operand" "r,W")
                   ] UNSPECV_SFTAC)
   )]
  ""
  "@
   sftac.l %0, %2
   sftac %0,#%2"
  [
    (set_attr "cc" "unchanged")
    (set_attr "type" "etc")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "sftac_error_32"
  [(set (match_operand: HI 0 "pic30_accumulator_operand" "=w")
        (ashift:HI
           (match_operand: HI 1 "general_operand" "")
           (match_operand: HI 2 "general_operand" "")))]
  ""
  "*
   {
     error(\"Automatic generation of DSP instructions not yet supported; \"
           \"use __builtin_sftac() instead\");
     return \"cannot generate instruction\";
   }
  "
)

(define_insn "subac_hi_32"
  [(set (match_operand: HI 0 "pic30_accumulator_operand"  "=w")
        (minus:HI (match_operand: HI 1 "pic30_accumulator_operand" "w")
                  (match_operand: HI 2 "pic30_accumulator_operand" "w")))]
  "(REGNO(operands[2]) != REGNO(operands[1]))"
  "sub %0"
  [
    (set_attr "cc" "unchanged")
    (set_attr "type" "etc")
    (set_attr "op_type" "alu")
  ]
)

; ********* to support automagic generation

(define_insn "movhi_accumulator_32"
  [(set (match_operand:HI 0 "pic30_accumulator_operand" "=w")
        (match_operand:HI 1 "pic30_mode3_operand" "RS<>r"))]
  ""
  "lac.w %1, #0, %0"
)  

(define_insn "movhi_accumulator2_32"
  [(set (match_operand:HI 0 "register_operand" "")
        (match_operand:HI 1 "pic30_accumulator_operand" "w"))]
  ""
  "*
    /* sac.w %1, #0, %0 */
    {
     error(\"Automatic generation of DSP instructions not yet supported; \"
           \"use __builtin_sac() instead\");
     return \"cannot generate instruction\";
    }
  "
)

(define_insn "movsi_accumulator_32"
  [(set (match_operand:SI 0 "pic30_accumulator_operand" "=w")
        (match_operand:SI 1 "pic30_mode3_operand" "RS<>r"))]
  ""
  "lac.l %1, #0, %0"
)  

(define_insn "movsi_accumulator2_32"
  [(set (match_operand:SI 0 "register_operand" "")
        (match_operand:SI 1 "pic30_accumulator_operand" "w"))]
  ""
  "*
    /* sac.l %1, #0, %0 */
    {
     error(\"Automatic generation of DSP instructions not yet supported; \"
           \"use __builtin_sac() instead\");
     return \"cannot generate instruction\";
    }
  "
)

(define_insn "addab_error_32"
  [(set (match_operand: HI          0 "pic30_accumulator_operand" "=w")
        (plus:HI (match_operand:HI 1 "pic30_mode3_operand" "RS<>r")
                 (match_operand:HI 2 "pic30_mode3_operand" "RS<>r")))]
  ""
  "*
   {
     error(\"Automatic generation of DSP instructions not yet supported; \"
           \"use __builtin_addab() instead\");
     return \"cannot generate instruction\";
   }
  "
)

(define_insn "auto_mac_32"
  [(set (match_operand:HI 0 "pic30_accumulator_operand" "=w")
        (plus:HI
          (match_operand:HI 1 "pic30_accumulator_operand" "0")
          (subreg:HI
            (mult:SI
              (sign_extend:SI
                (match_operand: HI 2 "pic30_mac_input_operand" "za"))
              (sign_extend:SI
                (match_operand: HI 3 "pic30_mac_input_operand" "za"))) 0)))]
  ""
  "*
   {
     error(\"Automatic generation of DSP instructions not yet supported; \"
           \"use __builtin_mac() instead\");
     return \"cannot generate instruction\";
   }
  "
)

(define_insn "auto_mac1_32"
  [(set (match_operand:HI 0 "pic30_accumulator_operand" "=w")
        (plus:HI
          (subreg:HI
            (mult:SI
              (sign_extend:SI
                (match_operand: HI 1 "pic30_mac_input_operand" "za"))
              (sign_extend:SI
                (match_operand: HI 2 "pic30_mac_input_operand" "za"))) 0)
          (match_operand:HI 3 "pic30_accumulator_operand" "0")))]
  ""
  "*
   {
     error(\"Automatic generation of DSP instructions not yet supported; \"
           \"use __builtin_mac() instead\");
     return \"cannot generate instruction\";
   }
  "
)

(define_insn "auto_mpy_32"
  [(set (match_operand:HI 0 "pic30_accumulator_operand" "=w")
        (neg: HI
          (subreg:HI
            (mult:SI
              (sign_extend:SI
                (match_operand: HI 1 "pic30_mac_input_operand" "za"))
              (sign_extend:SI
                (match_operand: HI 2 "pic30_mac_input_operand" "za"))) 0)))]
  ""
  "*
   {
     error(\"Automatic generation of DSP instructions not yet supported; \"
           \"use __builtin_mpy() instead\");
     return \"cannot generate instruction\";
   }
  "
)

(define_insn "auto_msc_32"
  [(set (match_operand:HI 0 "pic30_accumulator_operand" "=w")
        (minus:HI
          (match_operand:HI 1 "pic30_accumulator_operand" "0")
          (subreg:HI
            (mult:SI
              (sign_extend:SI
                (match_operand: HI 2 "pic30_mac_input_operand" "za"))
              (sign_extend:SI
                (match_operand: HI 3 "pic30_mac_input_operand" "za"))) 0)))]
  ""
  "*
   {
     error(\"Automatic generation of DSP instructions not yet supported; \"
           \"use __builtin_msc() instead\");
     return \"cannot generate instruction\";
   }
  "
)

(define_insn "auto_msc1_32"
  [(set (match_operand:HI 0 "pic30_accumulator_operand" "=w")
        (minus:HI
          (subreg:HI
            (mult:SI
              (sign_extend:SI
                (match_operand: HI 1 "pic30_mac_input_operand" "za"))
              (sign_extend:SI
                (match_operand: HI 2 "pic30_mac_input_operand" "za"))) 0)
          (match_operand:HI 3 "pic30_accumulator_operand" "0")))]
  ""
  "*
   {
     error(\"Automatic generation of DSP instructions not yet supported; \"
           \"use __builtin_msc() instead\");
     return \"cannot generate instruction\";
   }
  "
)

(define_insn "auto_sftacr_32"
  [(set (match_operand:HI 0 "pic30_accumulator_operand" "=w")
        (ashiftrt:HI
          (match_dup 0)
          (match_operand:HI 1 "immediate_operand"       "W")))]
  "(INTVAL(operands[1]) > 0)"
  "*
   {
     /* sftac %0, #%1 */
     error(\"Automatic generation of DSP instructions not yet supported; \"
           \"use __builtin_sftac() instead\");
     return \"cannot generate instruction\";
   }
  "
)

(define_insn "auto_sftacl_32"
  [(set (match_operand:HI 0 "pic30_accumulator_operand" "=w")
        (ashift:HI 
          (match_dup 0)
          (match_operand:HI 1 "immediate_operand"       "W")))]
  "(INTVAL(operands[1]) > 0)"
  "*
   {
     error(\"Automatic generation of DSP instructions not yet supported; \"
           \"use __builtin_sftac() instead\");
     return \"cannot generate instruction\";
   }
  "
)

(define_insn "addacr_hi_32"
  [(set (match_operand: HI 0 "pic30_accumulator_operand" "=w")
        (plus:HI
           (match_operand: HI 1 "pic30_mode3_operand" "RS<>r")
           (match_operand: HI 2 "pic30_accumulator_operand" "0")))]
  ""
  "*
   {
     /* add %1, %0 */
     error(\"Automatic generation of DSP instructions not yet supported; \"
           \"use __builtin_add() instead\");
     return \"cannot generate instruction\";
   }
  "
)

(define_insn "addacr1_hi_32"
  [(set (match_operand: HI 0 "pic30_accumulator_operand" "=w")
        (plus:HI
           (match_operand: HI 1 "pic30_accumulator_operand" "0")
           (match_operand: HI 2 "pic30_mode3_operand" "RS<>r")))]
  ""
  "*
   {
     /* add %2, %0 */
     error(\"Automatic generation of DSP instructions not yet supported; \"
           \"use __builtin_add() instead\");
     return \"cannot generate instruction\";
   }
  "
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; __builtin_unique_id

(define_insn "unique_id_32"
  [(set (match_operand:SI 0 "pic30_register_operand" "=r")
        (unspec_volatile:SI [ (match_operand 1 "immediate_operand" "i")
                              (match_operand:SI 2 "immediate_operand" "i")
                            ] UNSEPCV_GENLABEL))]
  "(pic30_isa32_target())"
  "*
   {  static char buffer[80];
      char *label;

      label = (char *)(INTVAL(operands[1]));
      sprintf(buffer,\".global %s\n%s:\n\tmov.l #%ld,%%0\",
              label,label,INTVAL(operands[2]));
      return buffer;
   }
  "
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; stackpush
;; These patterns are used for passing arguments on the stack.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define_insn "pushqi1_32"
  [(set (mem:SI (post_inc:SI (reg:SI SPREG)))
                (subreg:SI (match_operand:QI 0 "register_operand" "r") 0))]
  "(pic30_isa32_target())"
  "push.l %0"
  [
    (set_attr "cc" "unchanged")
    (set_attr "type" "defuse")
  ]
)

(define_insn "pushhi1_32"
  [(set (mem:SI (post_inc:SI (reg:SI SPREG)))
                (subreg:SI (match_operand:HI 0 "register_operand" "r") 0))]
  "(pic30_isa32_target())"
  "push.l %0"
  [
    (set_attr "cc" "unchanged")
    (set_attr "type" "defuse")
  ]
)

(define_insn "pushsi1_32"
  [(set (mem:SI (post_inc:SI (reg:SI SPREG)))
                (match_operand:SI 0 "pic30_pushsi_operand" "r,R>,T"))]
  "(pic30_isa32_target())"
  "@
   push.l %0
   mov.l %0,[w15++]
   push.l %0"
  [
    (set_attr "cc" "unchanged")
    (set_attr "type" "defuse")
  ]
)
(define_insn "pushsf1_32"
  [(set (mem:SF (post_inc:SI (reg:SI SPREG)))
                (match_operand:SF 0 "pic30_float_register_operand" "l"))]
  "(pic30_isa32_target())"
  "push.l %0"
  [
    (set_attr "cc" "unchanged")
    (set_attr "type" "defuse")
  ]
)

(define_insn "pushdi1_32"
  [(set (mem:DI (post_inc:SI (reg:SI SPREG)))
                (match_operand:DI 0 "register_operand" "r"))]
  "(pic30_isa32_target())"
  "push.l %0\;push.l %d0"
  [
    (set_attr "cc" "unchanged")
    (set_attr "type" "defuse")
  ]
)

(define_insn "pushdf1_32"
  [(set (mem:DF (post_inc:SI (reg:SI SPREG)))
                (match_operand:DF 0 "pic30_float_register_operand" "l"))]
  "(pic30_isa32_target())"
  "push.l %0\;push.l %d0"
  [
    (set_attr "cc" "unchanged")
    (set_attr "type" "defuse")
  ]
)

(define_insn "push<mode>_32"
  [(set (match_operand:S_INT_MODES_32 0 "push_operand"           "=>")
        (match_operand:S_INT_MODES_32 1 "pic30_register_operand" "r"))]
  "(pic30_isa32_target())"
  "push.l %1"
  [
   (set_attr "cc" "unchanged")
   (set_attr "type" "defuse")
   (set_attr "op_type" "mov")
  ]
)

(define_insn "push<mode>_32"
  [(set (match_operand:D_PUSH_MODES_32 0 "push_operand"           "=>")
        (match_operand:D_PUSH_MODES_32 1 "pic30_register_operand" "r"))]
  "(pic30_isa32_target())"
  "push.l %1\;push.l %d1"
  [
   (set_attr "cc" "unchanged")
   (set_attr "type" "defuse")
   (set_attr "op_type" "mov")
  ]
)

(define_insn "pushsf_32"
  [(set (match_operand:SF 0 "push_operand"           "=>")
        (match_operand:SF 1 "pic30_float_register_operand" "l"))]
  "(pic30_isa32_target())"
  "push.l %1"
  [
   (set_attr "cc" "unchanged")
   (set_attr "type" "defuse")
   (set_attr "op_type" "mov")
  ]
)

(define_insn "pushdf_32"
  [(set (match_operand:DF 0 "push_operand"           "=>")
        (match_operand:DF 1 "pic30_float_register_operand" "l"))]
  "(pic30_isa32_target())"
  "push.l %1\;push.l %d1"
  [
   (set_attr "cc" "unchanged")
   (set_attr "type" "defuse")
   (set_attr "op_type" "mov")
  ]
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; stackpop
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define_insn "pop<mode>_32"
  [(set (match_operand:S_INT_MODES_32 0 "pic30_pushsi_operand" "=r,R<,T")
        (match_operand:S_INT_MODES_32 1 "pop_operand"          " <,<,<"))]
  "(pic30_isa32_target())"
  "@
   pop.l %0
   mov.l %1,%0
   pop.l %0"
  [
   (set_attr "cc" "change0")
   (set_attr "type" "defuse")
   (set_attr "op_type" "mov")
  ]
)

(define_insn "pop<mode>_32"
  [(set (match_operand:D_PUSH_MODES_32 0 "pic30_register_operand" "=r")
        (match_operand:D_PUSH_MODES_32 1 "pop_operand"            " <"))]
  "(pic30_isa32_target())"
  "pop.l %d0\;pop.l %0"
  [
   (set_attr "cc" "change0")
   (set_attr "type" "defuse")
   (set_attr "op_type" "mov")
  ]
)

(define_insn "popsf_32"
  [(set (match_operand:SF 0 "pic30_float_register_operand" "=l")
        (match_operand:SF 1 "pop_operand"            " <"))]
  "(pic30_isa32_target())"
  "pop.l %0"
  [
   (set_attr "cc" "change0")
   (set_attr "type" "defuse")
   (set_attr "op_type" "mov")
  ]
)

(define_insn "popdf_32"
  [(set (match_operand:DF 0 "pic30_float_register_operand" "=l")
        (match_operand:DF 1 "pop_operand"            " <"))]
  "(pic30_isa32_target())"
  "pop.l %d0\;pop.l %0"
  [
   (set_attr "cc" "change0")
   (set_attr "type" "defuse")
   (set_attr "op_type" "mov")
  ]
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; stackcheck
;; If STACK_CHECK_BUILTIN is zero, and you define a named pattern called
;; check_stack in your 'md' file, GCC will call that pattern with one
;; argument which is the address to compare the stack value against.
;; You must arrange for this pattern to report an error if the stack
;; pointer is out of range.
;; (define_insn "check_stack_32"
;;   [(match_operand 0 "pic30_register_operand" "r")
;;    (clobber (match_scratch:HI 1 "=r"))]
;;   ""
;;   "mov .BOS,%1\;cpsge %0,%1\;reset")

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Test operations.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define_insn "tstqi_32"
  [(set (reg:CC CC_REG)
        (compare
           (match_operand:QI 0 "pic30_near_mode2_operand" "U,r,R<>")
           (const_int 0))
   )
  ]
  "(pic30_isa32_target())"
  "cp0.b %0"
  [
   (set_attr "cc" "set")
   (set_attr "type" "etc,etc,use")
   (set_attr "op_type" "alu")
  ]
)

(define_insn "tsthi_32"
  [(set (reg:CC CC_REG)
        (compare
          (match_operand:HI 0 "pic30_near_mode2_operand" "U,r,R<>")
          (const_int 0))
   )
  ]
  "(pic30_isa32_target())"
  "cp0.w %0"
  [
   (set_attr "cc" "set")
   (set_attr "type" "etc,etc,use")
   (set_attr "op_type" "alu")
  ]
)

(define_insn "tstsi_32"
  [(set (reg:CC CC_REG)
        (compare 
          (match_operand:SI 0 "pic30_register_operand" "r")
          (const_int 0))
   )
  ]
  "(pic30_isa32_target())"
  "cp0.l %0"
  [
    (set_attr "cc" "set")
    (set_attr "type" "etc")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "tstdi_32"
  [(set (reg:CC CC_REG)
        (compare
          (match_operand:DI 0 "pic30_register_operand" "r")
          (const_int 0))
   )
  ]
  "(pic30_isa32_target())"
  "cp.l %0,#0\;cpb.l %d0,#0"
  [
    (set_attr "cc" "set")
    (set_attr "type" "etc")
    (set_attr "op_type" "alu")
  ]
)

; With dsPIC30 floats, testing the most significant word does not suffice,
; since -0.0 == 0.0

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Block moves.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Argument 0 is the destination
;; Argument 1 is the source
;; Argument 2 is the length
;; Argument 3 is the alignment
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define_expand "movstrsi_32"
  [(parallel [(set (match_operand:BLK 0 "pic30_memory_operand" "")
		   (match_operand:BLK 1 "pic30_memory_operand" ""))
	      (use (match_operand:SI 2 "const_int_operand" ""))
	      (use (match_operand:SI 3 "const_int_operand" ""))
	      (clobber (match_dup 4))
	      (clobber (match_dup 5))
   	      (clobber (reg:SI RCOUNT))
  ])]
  "(pic30_isa32_target())"
  "{
     rtx addr0;
     rtx addr1;
     int n;
     
     if (GET_CODE (operands[2]) != CONST_INT) FAIL;
     n = INTVAL(operands[2]);
     if (n < 1) FAIL;
     switch (INTVAL(operands[3]))
     {
     case 1:
       if (n > 65535) FAIL;
       break;
     case 2:
       if ((n > (65535*2)) || (n & 1)) FAIL;
       break;
     case 4:
       if ((n > (65535*4)) || (n & 1)) FAIL;
       break;
     }
     
     addr0 = copy_to_mode_reg(Pmode, XEXP (operands[0], 0));
     addr1 = copy_to_mode_reg(Pmode, XEXP (operands[1], 0));

     operands[4] = addr0;
     operands[5] = addr1;
     operands[0] = change_address(operands[0], VOIDmode, addr0);
     operands[1] = change_address(operands[1], VOIDmode, addr1);
   }"
)

;
;  This pattern should restore the src/ dest ptrs before finishing
;
;  Perhaps these patterns should be simpler (CAW)... 
;
(define_insn "movmemsi_32"
  [(set (match_operand:BLK 0 "pic30_memory_operand"  "=R,m,R,R,m,R,m")
        (match_operand:BLK 1 "pic30_memory_operand"   "R,R,m,R,R,m,m"))
   (use (match_operand:SI 2 "immediate_operand" "J,J,J,i,i,i,i"))
   (use (match_operand:SI 3 "const_int_operand" ""))
   (clobber (reg:SI RCOUNT))
   (clobber (match_scratch:SI 4  "=X,&r,&r,&r,&r,&r,&r"))
   (clobber (match_scratch:SI 5  "=X,X,X,X,&r,&r,&r"))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "*
   { /* my calculation says maximum string size is ~120 */
     static char buffer[160];
     char *c = buffer;
     const char *op0 = \"[%r0++]\";
     const char *op1 = \"[%r1++]\";
     const char *restore_with_sub_0=0;
     const char *restore_with_sub_1=0;
     const char *restore_w14=0;
     const char *sub_value = 0;  /* != 0 => use register */
     int repeat_repeat;
     int repeat_count;

     repeat_count = INTVAL(operands[2]);
     switch (which_alternative) {
     default: break;
     case 0:  /* both registers */
              /* literal <= 10 bits */
              restore_with_sub_0 = \"%r0\";
              restore_with_sub_1 = \"%r1\";
              break;
     case 1:  /* op0 is memory, take its address */
              /* literal <= 10 bits */
              if (pic30_T_constraint(operands[0],VOIDmode) ||
                  pic30_U_constraint(operands[0],VOIDmode)) {
                c += sprintf(c,\"mov.l #%%0,%%4\;\");
                op0 = \"[%4++]\";
              } else if (pic30_S_constraint(operands[0]) ||
                         pic30_Q_constraint(operands[0])) {
                /* [Wn + X] */
                rtx Wn, X;

                Wn = XEXP(XEXP(operands[0],0),0);
                X = XEXP(XEXP(operands[0],0),1);
                if (REG_P(X))
                  c += sprintf(c,\"add.l w%d,w%d,%%4\;\", REGNO(Wn), REGNO(X));
                else {
                  if (satisfies_constraint_P(X)) {
                    c += sprintf(c,\"add.l w%d,#%ld,%%4\;\",
                                 REGNO(Wn),INTVAL(X));
                  } else if (satisfies_constraint_N(X)) {
                    c += sprintf(c,\"sub.l w%d,#%ld,%%4\;\",
                                   REGNO(Wn),-1*INTVAL(X));
                  } else {
                    c += sprintf(c,\"mov.l #%ld,%%4\;\",INTVAL(X));
                    c += sprintf(c,\"add.l w%d,%%4,%%4\;\", REGNO(Wn));
                  }
                }
                op0 = \"[%4++]\";
              } else {
                gcc_assert(0);
              } 
              restore_with_sub_1 = \"%r1\";
              break;
     case 2:  /* op1 is memory, take its address */
              /* literal <= 10 bits */
              if (pic30_T_constraint(operands[1],VOIDmode) ||
                  pic30_U_constraint(operands[1],VOIDmode)) {
                c += sprintf(c,\"mov.l #%%1,%%4\;\");
                op1 = \"[%4++]\";
              } else if (pic30_S_constraint(operands[1]) ||
                         pic30_Q_constraint(operands[1])) {
                /* [Wn + X] */
                rtx Wn, X;

                Wn = XEXP(XEXP(operands[1],0),0);
                X = XEXP(XEXP(operands[1],0),1);
                if (REG_P(X))
                  c += sprintf(c,\"add.l w%d,w%d,%%4\;\", REGNO(Wn), REGNO(X));
                else {
                  if (satisfies_constraint_P(X)) {
                    c += sprintf(c,\"add.l w%d,#%ld,%%4\;\",
                                 REGNO(Wn),INTVAL(X));
                  } else if (satisfies_constraint_N(X)) {
                    c += sprintf(c,\"sub.l w%d,#%ld,%%4\;\",
                                   REGNO(Wn),-1*INTVAL(X));
                  } else {
                    c += sprintf(c,\"mov.l #%ld,%%4\;\",INTVAL(X));
                    c += sprintf(c,\"add.l w%d,%%4,%%4\;\", REGNO(Wn));
                  }
                }
                op1 = \"[%4++]\";
              } else {
                gcc_assert(0);
              } 
              restore_with_sub_0 = \"%r0\";
              break;
     case 6:  /* op0 and op1 are memory, take their addresses */
              if (pic30_T_constraint(operands[0],VOIDmode) || 
                  pic30_U_constraint(operands[0],VOIDmode)) {
                c += sprintf(c,\"mov.l #%%0,%%4\;\");
                op0 = \"[%4++]\";
              } else if (pic30_S_constraint(operands[0]) ||
                         pic30_Q_constraint(operands[0])) {
                /* [Wn + X] */
                rtx Wn, X;
  
                Wn = XEXP(XEXP(operands[0],0),0);  
                X = XEXP(XEXP(operands[0],0),1);
                if (REG_P(X)) 
                  c += sprintf(c,\"add.l w%d,w%d,%%4\;\", REGNO(Wn), REGNO(X));
                else {
                  if (satisfies_constraint_P(X)) {
                    c += sprintf(c,\"add.l w%d,#%ld,%%4\;\",
                                 REGNO(Wn),INTVAL(X));
                  } else if (satisfies_constraint_N(X)) {
                    c += sprintf(c,\"sub.l w%d,#%ld,%%4\;\",
                                   REGNO(Wn),-1*INTVAL(X));
                  } else {
                    c += sprintf(c,\"mov.l #%ld,%%4\;\",INTVAL(X));
                    c += sprintf(c,\"add.l w%d,%%4,%%4\;\", REGNO(Wn));
                  }
                }
                op0 = \"[%4++]\";
              } else {
                gcc_assert(0);
              }
              if (pic30_T_constraint(operands[1],VOIDmode) ||
                  pic30_U_constraint(operands[1],VOIDmode)) {
                c += sprintf(c,\"mov.l #%%1,%%5\;\");
                op1 = \"[%5++]\";
              }  else if (pic30_S_constraint(operands[1]) ||
                         pic30_Q_constraint(operands[1])) {
                /* [Wn + X] */
                rtx Wn, X;

                Wn = XEXP(XEXP(operands[1],0),0);
                X = XEXP(XEXP(operands[1],0),1);
                if (REG_P(X))
                  c += sprintf(c,\"add.l w%d,w%d,%%5\;\", REGNO(Wn), REGNO(X));
                else {
                  if (satisfies_constraint_P(X)) {
                    c += sprintf(c,\"add.l w%d,#%ld,%%5\;\",
                                 REGNO(Wn),INTVAL(X));
                  } else if (satisfies_constraint_N(X)) {
                    c += sprintf(c,\"sub.l w%d,#%ld,%%5\;\",
                                   REGNO(Wn),-1*INTVAL(X));
                  } else {
                    c += sprintf(c,\"mov.l #%ld,%%5\;\",INTVAL(X));
                    c += sprintf(c,\"add.l w%d,%%5,%%5\;\", REGNO(Wn));
                  }
                }
                op1 = \"[%5++]\";
              } else {
                gcc_assert(0);
              }
              break;
     case 3:  /* op0 and op1 are registers */
              /* literal > 10 bits, save repeat count in reg */
              restore_with_sub_0 = \"%r0\";
              restore_with_sub_1 = \"%r1\";
              c += sprintf(c,\"mov.l #%%2,%%4\;\");
              sub_value =\"%4\";
              break;
     case 4:  /* op0 is memory, take its address */
              /* literal > 10 bits, save repeat count in reg */
              if (pic30_T_constraint(operands[0],VOIDmode) ||
                  pic30_U_constraint(operands[0],VOIDmode)) {
                c += sprintf(c,\"mov.l #%%0,%%4\;\");
                op0 = \"[%4++]\";
              } else if (pic30_S_constraint(operands[0]) ||
                         pic30_Q_constraint(operands[0])) {
                /* [Wn + X] */
                rtx Wn, X;

                Wn = XEXP(XEXP(operands[0],0),0);
                X = XEXP(XEXP(operands[0],0),1);
                if (REG_P(X))
                  c += sprintf(c,\"add.l w%d,w%d,%%4\;\", REGNO(Wn), REGNO(X));
                else {
                  if (satisfies_constraint_P(X)) {
                    c += sprintf(c,\"add.l w%d,#%ld,%%4\;\",
                                 REGNO(Wn),INTVAL(X));
                  } else if (satisfies_constraint_N(X)) {
                    c += sprintf(c,\"sub.l w%d,#%ld,%%4\;\",
                                   REGNO(Wn),-1*INTVAL(X));
                  } else {
                    c += sprintf(c,\"mov.l #%ld,%%4\;\",INTVAL(X));
                    c += sprintf(c,\"add.l w%d,%%4,%%4\;\", REGNO(Wn));
                  }
                }
                op0 = \"[%4++]\";
              } else {
                gcc_assert(0);
              }
              if (pic30_T_constraint(operands[2],VOIDmode) ||
                  pic30_U_constraint(operands[2],VOIDmode)) {
                c += sprintf(c,\"mov.l #%%2,%%5\;\");
              } else {
                c += sprintf(c,\"mov.l %%2,%%5\;\");
              }
              sub_value = \"%5\";
              restore_with_sub_1 = \"%r1\";
              break;
     case 5:  /* op1 is memory, take its address */
              /* literal > 10 bits, save repeat count in reg */
              if (pic30_T_constraint(operands[1],VOIDmode) ||
                  pic30_U_constraint(operands[1],VOIDmode)) {
                c += sprintf(c,\"mov.l #%%1,%%4\;\");
                op1 = \"[%4++]\";
              } else if (pic30_S_constraint(operands[1]) ||
                         pic30_Q_constraint(operands[1])) {
                /* [Wn + X] */
                rtx Wn, X;

                Wn = XEXP(XEXP(operands[1],0),0);
                X = XEXP(XEXP(operands[1],0),1);
                if (REG_P(X))
                  c += sprintf(c,\"add.l w%d,w%d,%%4\;\", REGNO(Wn), REGNO(X));
                else {
                  if (satisfies_constraint_P(X)) {
                    c += sprintf(c,\"add.l w%d,#%ld,%%4\;\",
                                 REGNO(Wn),INTVAL(X));
                  } else if (satisfies_constraint_N(X)) {
                    c += sprintf(c,\"sub.l w%d,#%ld,%%4\;\",
                                   REGNO(Wn),-1*INTVAL(X));
                  } else {
                    c += sprintf(c,\"mov.l #%ld,%%4\;\",INTVAL(X));
                    c += sprintf(c,\"add.l w%d,%%4,%%4\;\", REGNO(Wn));
                  }
                }
                op1 = \"[%4++]\";
              } else {
                gcc_assert(0);
              }
              restore_with_sub_0 = \"%r0\";
              if (pic30_T_constraint(operands[2],VOIDmode) ||
                  pic30_U_constraint(operands[2],VOIDmode)) {
                c += sprintf(c,\"mov.l #%%2,%%5\;\");
              } else {
                c += sprintf(c,\"mov.l %%2,%%5\;\");
              }
              sub_value = \"%5\";
              break;
     }
     
     switch (INTVAL(operands[3]))
     {
       case 1:
         /* 
         ** Byte operation
         */
         repeat_repeat =  repeat_count - 1048574;
         if (repeat_repeat < 0) repeat_repeat = 0;
	 if (repeat_repeat) {
           c += sprintf(c,\"repeat #1048574-1\;mov.b %s,%s\;\", op1, op0);
           c += sprintf(c,\"repeat #%d-1\;mov.b %s,%s\", repeat_count,op1, op0);
         } else c += sprintf(c,\"repeat #%d-1\;mov.b %s,%s\", 
                             repeat_count, op1, op0);
         break;
     case 2: {
         int repeat_remainder;
	 /* 
	 ** 16-bit word operation
	 */
         /* repeat count is expressed in bytes */
         repeat_remainder = (repeat_count & 1);
         repeat_count = repeat_count / 2;
         repeat_repeat =  repeat_count - 1048574;
         if (repeat_repeat < 0) repeat_repeat = 0;
	 if (repeat_repeat) {
           c += sprintf(c,\"repeat #1048574-1\;mov.w %s,%s\;\", op1, op0);
           c += sprintf(c,\"repeat #%d-1\;mov.w %s,%s\", 
                        repeat_count, op1, op0);
         } else c += sprintf(c,\"repeat #%d-1\;mov.w %s,%s\", 
                             repeat_count, op1, op0);
         if (repeat_remainder) {
           if (pic30_psrd_psrd_errata(operands[1],NULL)) {
             c += sprintf(c,\"\;nop\");
           }
           c += sprintf(c,\"\;mov.b %s,%s\", op1,op0);
         } 
         break;
       }
     case 4: {
         int repeat_remainder;
         /*
         ** 32-bit word operation
         */
         /* repeat count is expressed in bytes */
         repeat_remainder = (repeat_count & 3);
         repeat_count = repeat_count / 4;
         repeat_repeat =  repeat_count - 1048574;
         if (repeat_repeat < 0) repeat_repeat = 0;
         if (repeat_repeat) {
           c += sprintf(c,\"repeat #1048574-1\;mov.l %s,%s\;\", op1, op0);
           c += sprintf(c,\"repeat #%d-1\;mov.l %s,%s\",
                        repeat_count, op1, op0);
         } else c += sprintf(c,\"repeat #%d-1\;mov.l %s,%s\",
                             repeat_count, op1, op0);
         if (repeat_remainder) {
           if (pic30_psrd_psrd_errata(operands[1],NULL)) {
             c += sprintf(c,\"\;nop\");
           }
           switch (repeat_remainder) {
             default: gcc_assert(0);
                      break;
             case 3:
             case 2: c += sprintf(c,\"\;mov.w %s,%s\", op1,op0);
                     if (repeat_remainder == 2) break;
                     /* FALLSTHROUGH */
             case 1: c += sprintf(c,\"\;mov.b %s,%s\", op1,op0);
                     break;
             case 0: break;
           }
         } 
         break;
       }
     }
     if (restore_with_sub_0) {
       if (sub_value) {
         c += sprintf(c,\"\;sub.l %s,%s,%s\", 
                      restore_with_sub_0, sub_value, restore_with_sub_0);
       } else c += sprintf(c,\"\;sub.l #%%2,%s\", restore_with_sub_0);
     }
     if (restore_with_sub_1) {
       if (sub_value) {
         c += sprintf(c,\"\;sub.l %s,%s,%s\", 
                      restore_with_sub_1, sub_value, restore_with_sub_1);
       } else c += sprintf(c,\"\;sub.l #%%2,%s\", restore_with_sub_1);
     }
     if (restore_w14) {
       c += sprintf(c, \"\;mov.l %s,w14\", restore_w14);
     }
     return buffer;
   } "
  [
    (set_attr "type" "use")
    (set_attr "op_type" "alu")
  ]
)

;; Block clear.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Argument 0 is the destination
;; Argument 1 is the length
;; Argument 2 is the alignment
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
(define_insn "setmemsi_32"
  [
   (set (match_operand:BLK 0 "pic30_memory_operand"  "=R,m,R,m,R,m,R,m,R,m,R,m")
        (match_operand     2 "pic30_reg_or_imm_operand" 
                                                     "O,O,O,O,i,i,i,i,r,r,r,r"))
   (use (match_operand:SI 1 "immediate_operand" "J,J,i,i,J,J,i,i,J,J,i,i"))
   (use (match_operand:SI 3 "const_int_operand" ""))
   (clobber (reg:SI RCOUNT))
   (clobber (match_scratch:SI 4  "=X,&r,&r,&r,X,&r,&r,&r,X,&r,&r,&r"))
   (clobber (match_scratch:SI 5  "=X,X,X,&r,X,X,X,&r,X,X,X,&r"))
   (clobber (match_scratch:SI 6  "=X,X,X,X,&r,&r,&r,&r,&r,&r,&r,&r"))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "*
   { /* my calculation says maximum string size is ~120 */
     static char buffer[160];
     char *c = buffer;
     const char *op0 = \"[%r0++]\";
     const char *op1 = \"\";
     const char *restore_with_sub_0=0;
     const char *restore_w14=0;
     const char *sub_value = 0;  /* != 0 => use register */
     unsigned char byte;
     int repeat_repeat;
     int repeat_count;
     const char *set_instr = \"mov\";
     int word_size = INTVAL(operands[3]);

     repeat_count = INTVAL(operands[1]);
     if (const_int_operand(operands[2],VOIDmode)) {
       if (INTVAL(operands[2]) == 0) {
         set_instr = \"clr\";
       } else {
         /* the middle four are the same as the bottom four, just that we don't
            need a clobber register for the value */
         gcc_assert(which_alternative > 3);
         which_alternative -= 4;
         /* repeat bytes */
         /* first convert char to unsigned char */
         /* since we are doing bit operations */
         byte = INTVAL(operands[2]) & 0xFF;
         if (INTVAL(operands[3]) == 2) {
           c += sprintf(c,\"mov.w #%d,%%6\;\", (byte << 8) + byte);
         } else if (INTVAL(operands[3]) == 4) {
           c += sprintf(c,\"mov.l #%d,%%6\;\", 
                        (byte << 24) + (byte << 16) + (byte << 8) + byte);
         } else {
           c += sprintf(c,\"mov.l #%ld,%%6\;\", INTVAL(operands[2]));
         }
         op1=\"%6,\";
       }
     } else {
       /* register already */
       /* the top four are the same as the bottom four, just that we don't
          need a clobber register for the value */
       gcc_assert(which_alternative > 7);
       which_alternative -= 8;
       op1 = \"%2,\";
       if (repeat_count < 4) {
         word_size = 1;
       } else if (word_size == 2) {
         c += sprintf(c,\"sl %%2,#8,%%6\;ior.b %%2,%%6,%%6\;\");
         op1=\"%6,\";
       } else if (word_size == 4) {
         c += sprintf(c,\"sl.l %%2,#8,%%6\;ior.b %%2,%%6,%%6\;\"
                        \"sl.l %%6,#8,%%6\;ior.b %%2,%%6,%%6\;\"
                        \"sl.l %%6,#8,%%6\;ior.b %%2,%%6,%%6\;\");
         op1=\"%6,\";
       }
     }

     switch (which_alternative) {
     default: break;
     case 0:  /* op0 is register */
              /* literal <= 10 bits */
              restore_with_sub_0 = \"%r0\";
              break;
     case 1:  /* op0 is memory, take its address */
              /* literal <= 10 bits */
              if (pic30_T_constraint(operands[0],VOIDmode) ||
                  pic30_U_constraint(operands[0],VOIDmode)) {
                c += sprintf(c,\"mov.l #%%0,%%4\;\");
                op0 = \"[%4++]\";
              } else if (pic30_S_constraint(operands[0]) ||
                         pic30_Q_constraint(operands[0])) {
                /* [Wn + X] */
                rtx Wn, X;

                Wn = XEXP(XEXP(operands[0],0),0);
                X = XEXP(XEXP(operands[0],0),1);
                {
                  if (REG_P(X))
                    c += sprintf(c,\"add.l w%d,w%d,%%4\;\", 
                                 REGNO(Wn), REGNO(X));
                  else {
                    if (satisfies_constraint_P(X)) {
                      c += sprintf(c,\"add.l w%d,#%ld,%%4\;\",
                                   REGNO(Wn),INTVAL(X));
                    } else if (satisfies_constraint_N(X)) {
                      c += sprintf(c,\"sub.l w%d,#%ld,%%4\;\",
                                     REGNO(Wn),-1*INTVAL(X));
                    } else {
                      c += sprintf(c,\"mov.l w%d,%%4\;\", REGNO(Wn));
                      if (INTVAL(X) < 0)
                        c += sprintf(c,\"sub.l #%ld,%%4\;\",-1*INTVAL(X));
                      else c += sprintf(c,\"add.l #%ld,%%4\;\",INTVAL(X));
                    }
                  }
                  op0 = \"[%4++]\";
                }
              } else {
                gcc_assert(0);
              } 
              break;
     case 2:  /* op0 is a register */
              /* literal > 10 bits, save repeat count in reg */
              restore_with_sub_0 = \"%r0\";
              c += sprintf(c,\"mov.l #%%1,%%4\;\");
              sub_value =\"%4\";
              break;
     case 3:  /* op0 is memory, take its address */
              /* literal > 10 bits, save repeat count in reg */
              if (pic30_T_constraint(operands[0],VOIDmode) ||
                  pic30_U_constraint(operands[0],VOIDmode)) {
                c += sprintf(c,\"mov.l #%%0,%%4\;\");
                op0 = \"[%4++]\";
              } else if (pic30_S_constraint(operands[0]) ||
                         pic30_Q_constraint(operands[0])) {
                /* [Wn + X] */
                rtx Wn, X;

                Wn = XEXP(XEXP(operands[0],0),0);
                X = XEXP(XEXP(operands[0],0),1);
                {
                  if (REG_P(X))
                    c += sprintf(c,\"add.l w%d,w%d,%%4\;\", 
                                 REGNO(Wn), REGNO(X));
                  else {
                    if (satisfies_constraint_P(X)) {
                      c += sprintf(c,\"add.l w%d,#%ld,%%4\;\",
                                   REGNO(Wn),INTVAL(X));
                    } else if (satisfies_constraint_N(X)) {
                      c += sprintf(c,\"sub.l w%d,#%ld,%%4\;\",
                                   REGNO(Wn),-1*INTVAL(X));
                    } else {
                      c += sprintf(c,\"mov.l w%d,%%4\;\", REGNO(Wn));
                      if (INTVAL(X) < 0)
                        c += sprintf(c,\"sub.l #%ld,%%4\;\",-1*INTVAL(X));
                      else c += sprintf(c,\"add.l #%ld,%%4\;\",INTVAL(X));
                    }
                  }
                  op0 = \"[%4++]\";
                }
              } else {
                gcc_assert(0);
              }
              break;
     }
     
     switch (word_size) 
     {
       case 1:
         /* 
         ** Byte operation
         */
         repeat_repeat =  repeat_count - 1048574;
         if (repeat_repeat < 0) repeat_repeat = 0;
	 if (repeat_repeat) {
           c += sprintf(c, \"repeat #1048574-1\;%s.b %s%s\;\", 
                           set_instr, op1, op0);
           c += sprintf(c,\"repeat #%d-1\;%s.b %s%s\", 
                           repeat_count, set_instr, op1, op0);
         } else c += sprintf(c,\"repeat #%d-1\;%s.b %s%s\", 
                             repeat_count, set_instr, op1, op0);
         break;
       case 2: {
         int repeat_remainder;
	 /* 
	 ** Word operation
	 */
         /* repeat count is expressed in bytes */
         repeat_remainder = (repeat_count & 1);
         repeat_count = repeat_count / 2;
         repeat_repeat =  repeat_count - 1048574;
         if (repeat_repeat < 0) repeat_repeat = 0;
	 if (repeat_repeat) {
           c += sprintf(c,\"repeat #1048574-1\;%s.w %s%s\;\", 
                           set_instr, op1, op0);
           c += sprintf(c,\"repeat #%d-1\;%s.w %s%s\", 
                        repeat_count, set_instr, op1, op0);
         } else c += sprintf(c,\"repeat #%d-1\;%s.w %s%s\", 
                             repeat_count, set_instr, op1, op0);
         if (repeat_remainder) 
           c += sprintf(c,\"\;%s.b %s%s\", set_instr, op1, op0);
         break;
       }
       case 4: {
         int repeat_remainder;
	 /* 
	 ** Word operation
	 */
         /* repeat count is expressed in bytes */
         repeat_remainder = (repeat_count & 3);
         repeat_count = repeat_count / 4;
         repeat_repeat =  repeat_count - 1048574;
         if (repeat_repeat < 0) repeat_repeat = 0;
	 if (repeat_repeat) {
           c += sprintf(c,\"repeat #1048574-1\;%s.l %s%s\;\", 
                           set_instr, op1, op0);
           c += sprintf(c,\"repeat #%d-1\;%s.l %s%s\", 
                        repeat_count, set_instr, op1, op0);
         } else c += sprintf(c,\"repeat #%d-1\;%s.l %s%s\", 
                             repeat_count, set_instr, op1, op0);
         switch (repeat_remainder) {
           default: gcc_assert(0);
                    break;
           case 3:
           case 2: c += sprintf(c,\"\;%s.w %s%s\", set_instr, op1, op0);
                   if (repeat_remainder == 2) break;
                   /* FALLSTHROUGH */
           case 1: c += sprintf(c,\"\;%s.b %s%s\", set_instr, op1, op0);
                   break;
           case 0: break;
         }
         break;
       }
     }
     if (restore_with_sub_0) {
       if (sub_value) {
         c += sprintf(c,\"\;sub.l %s,%s,%s\", 
                      restore_with_sub_0, sub_value, restore_with_sub_0);
       } else c += sprintf(c,\"\;sub.l #%%1,%s\", restore_with_sub_0);
     }
     if (restore_w14) {
       c += sprintf(c, \"\;mov.l %s,w14\", restore_w14);
     }
     return buffer;
   } "
  [
    (set_attr "type" "use")
    (set_attr "op_type" "alu")
  ]
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; compare instructions.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;; Define this first one so HAVE_conditional_move is defined.
(define_insn "movcc_dummy"
  [(set (match_operand 0 "" "")
       (if_then_else (match_operand 1 "" "")
                     (match_operand 2 "" "")
                     (match_operand 3 "" "")))]
  "!operands[0]"
  "")

(define_insn "maxhi2"
  [(set (match_operand:HI   0 "pic30_register_operand" "+r")
        (if_then_else:HI
          (ge 
            (match_operand:HI 1 "pic30_register_operand" " r")
            (match_dup 0))
          (match_dup 1)
          (match_dup 0)
        ))
   (clobber (reg:CC_NZ CC_REG))
  ]
  "(pic30_isa32_target())"
  "max.w %1, %0"
  [
    (set_attr "cc" "clobber")
    (set_attr "op_type" "etc")
  ]
)

(define_insn "maxsi2"
  [(set (match_operand:SI   0 "pic30_register_operand" "+r")
        (if_then_else:SI
          (ge 
            (match_operand:SI 1 "pic30_register_operand" " r")
            (match_dup 0))
          (match_dup 1) 
          (match_dup 0)
        ))
   (clobber (reg:CC_NZ CC_REG))
  ]
  "(pic30_isa32_target())"
  "max.l %1, %0"
  [
    (set_attr "cc" "clobber")
    (set_attr "op_type" "etc")
  ]
)

(define_insn "minhi2"
  [(set (match_operand:HI   0 "pic30_register_operand" "+r")
        (if_then_else:HI
          (le 
            (match_operand:HI 1 "pic30_register_operand" " r")
            (match_dup 0))
          (match_dup 1)
          (match_dup 0)
        ))
   (clobber (reg:CC_NZ CC_REG))
  ]
  "(pic30_isa32_target())"
  "min.w %1, %0"
  [
    (set_attr "cc" "clobber")
    (set_attr "op_type" "etc")
  ]
)

(define_insn "minsi2"
  [(set (match_operand:SI   0 "pic30_register_operand" "+r")
        (if_then_else:SI
          (le 
            (match_operand:SI 1 "pic30_register_operand" " r")
            (match_dup 0))
          (match_dup 1)
          (match_dup 0)
        ))
   (clobber (reg:CC_NZ CC_REG))
  ]
  "(pic30_isa32_target())"
  "min.l %1, %0"
  [
    (set_attr "cc" "clobber")
    (set_attr "op_type" "etc")
  ]
)

(define_insn "mov<mode>cc_helper"
  [(set (match_operand:S_INT_MODES_32   0 "pic30_register_operand" "=r")
        (if_then_else:S_INT_MODES_32
          (match_operator               1 "pic30_comparison_operator" 
            [(match_operand             4 "pic30_register_operand" "r")
             (match_operand             5 "pic30_reg_or_lit16"    "rJ")]) 
          (match_operand:S_INT_MODES_32 2 "pic30_register_operand" "r")
          (match_operand:S_INT_MODES_32 3 "pic30_register_operand" "r")
        ))
   (clobber (match_scratch:SI           6                         "=X"))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "* 
   return pic30_conditional_move(insn, operands);
  "
  [
    (set_attr "cc" "clobber")
    (set_attr "op_type" "etc")
  ]
)

(define_insn "mov<mode>cc_helper1"
  [(set (match_operand:S_INT_MODES_32   0 "pic30_register_operand" "=r")
        (if_then_else:S_INT_MODES_32
          (match_operator 1 "pic30_comparison_operator" [
            (reg:CC CC_REG) 
            (const_int 0)]
          )
          (match_operand:S_INT_MODES_32 2 "pic30_register_operand" "r")
          (match_operand:S_INT_MODES_32 3 "pic30_register_operand" "r")
        ))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "* 
   return pic30_conditional_move1(insn, operands);
  "
  [
    (set_attr "cc" "clobber")
    (set_attr "op_type" "etc")
  ]
)

(define_expand "mov<mode>cc"
  [(set (match_operand:S_INT_MODES_32   0 "pic30_register_operand"       "")
        (if_then_else
          (match_operand                1 "pic30_comparison_operator"          "")
          (match_operand:S_INT_MODES_32 2 "pic30_register_operand"       "")
          (match_operand:S_INT_MODES_32 3 "pic30_register_operand"       "")))
  ]
  "(pic30_isa32_target())"
  "
    rtx op0 = operands[0];
    rtx op1 = operands[1];
    rtx op2 = operands[2];
    rtx op3 = operands[3];
    rtx op4 = XEXP(operands[1],0);
    rtx op5 = XEXP(operands[1],1);
    enum rtx_code code = GET_CODE(op1);

    if (pic30_movif_condition_code_name(code,GET_MODE(op0),(int*)(0)) == 0) {
      FAIL;
    }
    if (!pic30_register_operand(op2,VOIDmode)) {
      op2 = force_reg(GET_MODE(op2),op2);
    }
    if (!pic30_register_operand(op3,VOIDmode)) {
      op3 = force_reg(GET_MODE(op3),op3);
    }
    if (!pic30_register_operand(op4,VOIDmode)) {
      op4 = force_reg(GET_MODE(op4),op4);
    }
    if (!pic30_reg_or_lit16(op5,VOIDmode)) {
      // a literal has VOIDMODE so use another op
      op5 = force_reg(GET_MODE(op4),op5);
    }

    emit_insn(
      gen_mov<mode>cc_helper(op0,op1,op2,op3,op4,op5)
    );
    DONE;
  "
)

(define_insn "any_branch_32"
  [(set (pc)
     (if_then_else 
       (match_operator 0 "comparison_operator" [
         (reg:CC CC_REG) 
         (const_int 0)]
       )
       (label_ref (match_operand 1 "" ""))
       (pc)))
  ]
  "(pic30_isa32_target())"
  "* 
   return pic30_conditional_branch(GET_CODE(operands[0]), operands[1],1,0);
  "
  [
    (set_attr "cc" "unchanged")
    (set_attr "op_type" "etc")
  ]
)

(define_insn "any_fbranch_32"
  [(set (pc)
     (if_then_else 
       (match_operator 0 "comparison_operator" [
         (reg:CC FSR)
         (const_int 0)]
       )
       (label_ref (match_operand 1 "" ""))
       (pc)))
  ]
  "(pic30_isa32_target())"
  "*
   return pic30_conditional_branch(GET_CODE(operands[0]), operands[1],1,1);
  "
  [
    (set_attr "cc" "unchanged")
    (set_attr "op_type" "etc")
  ]
)


(define_expand "cbranchp32umm4_32"
 [(set (reg:CC CC_REG)    
       (compare (match_operand:P32UMM 1 "pic30_mode1PN_operand" "")
                (match_operand:P32UMM 2 "pic30_mode1PN_operand" "")))
  (set (pc)
       (if_then_else 
         (match_operator 0 "comparison_operator" [
           (reg:CC CC_REG) 
           (const_int 0)]
         )
         (label_ref (match_operand 3 "" ""))
         (pc)))
  ] 
  "(pic30_isa32_target())"
  { 
    FAIL;
  }
) 

(define_expand "cbranch<mode>4_32"
 [(set (reg:CC CC_REG)
       (compare (match_operand:CMPMODES 1 "pic30_mode1PN_operand" "")
                (match_operand:CMPMODES 2 "pic30_mode1PN_operand" "")))
  (set (pc)
       (if_then_else
         (match_operator 0 "comparison_operator" [
          (reg:CC CC_REG) 
           (const_int 0)]
         )
         (label_ref (match_operand 3 "" ""))
         (pc)))
  ]
  "(pic30_isa32_target())"
  { /* The optimizer will coalesce this if possible, right? */
    rtx reg_1 = force_reg (GET_MODE(operands[1]), operands[1]);
    rtx reg_2 = operands[2];

    if (immediate_operand(operands[2],VOIDmode)) {
      if (GET_MODE_SIZE(GET_MODE(operands[1])) > 4) {
        if (!pic30_mode1J_operand(operands[2], GET_MODE(operands[1]))) {
          reg_2 = force_reg(GET_MODE(operands[1]), operands[2]);
        }
      }
    }
    emit(
      gen_cmp<mode>_32(reg_1,reg_2)
    );
    emit(
      gen_any_branch_32(operands[0], operands[3])
    );
    DONE;
  }
)

(define_expand "cbranch<mode>4_32"
 [(set (reg:CC FSR)
       (compare (match_operand:CMPMODES32 1 "pic30_mode1PN_operand" "")
                (match_operand:CMPMODES32 2 "pic30_mode1PN_operand" "")))
  (set (pc)
       (if_then_else 
         (match_operator 0 "comparison_operator" [
           (reg:CC FSR) 
           (const_int 0)]
         )
         (label_ref (match_operand 3 "" ""))
         (pc)))
  ]
  "(pic30_isa32_target())"
  { /* The optimizer will coalesce this if possible, right? */
    rtx reg_1 = force_reg (GET_MODE(operands[1]), operands[1]);
    rtx reg_2 = operands[2];

    if ((<MODE>mode == SFmode) || (<MODE>mode == DFmode)) {
      reg_2 = force_reg(<MODE>mode, operands[2]);
    } else if (immediate_operand(operands[2],VOIDmode)) {
      if (GET_MODE_SIZE(GET_MODE(operands[1])) > 4) {
        if (!pic30_mode1J_operand(operands[2], GET_MODE(operands[1]))) {
          reg_2 = force_reg(GET_MODE(operands[1]), operands[2]);
        }
      }
    }
    emit(
      gen_cmp<mode>_32(reg_1,reg_2)
    );
    emit(
      gen_any_fbranch_32(operands[0], operands[3])
    );
    DONE;
  }
) 

;;;;;;;;;;;;;;;;;;
;; quarter integer
;;;;;;;;;;;;;;;;;;

(define_insn "*cmpqi_imm_32"
  [(set (reg:CC CC_REG)
        (compare (match_operand:QI 0 "pic30_register_operand" "r")
                 (match_operand:QI 1 "pic30_M_operand"  "M")))
   (clobber (match_dup 0))]
  "(pic30_isa32_target())"
  "add.b #%J1,%0"
  [
    (set_attr "cc" "set")
    (set_attr "type" "etc")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "cmpqi3_sfr0_32"
  [(set (reg:CC CC_REG)
        (compare 
          (match_operand:QI 0 "pic30_reg_or_near_operand" "U,r")
          (match_operand:QI 1 "pic30_register_operand"    "r,r")))]
  "(pic30_isa32_target())"
  "@
   cp.b %0,%1
   cp.b %0,%1"
  [
    (set_attr "cc" "set,set")
    (set_attr "type" "etc,etc")
    (set_attr "op_type" "alu")
  ]
)

;(define_insn "cmpqi3_2sfr_32"
;  [(set (reg:CC CC_REG)
;        (compare 
;           (match_operand:QI 0 "pic30_wreg_or_near_operand" "?r,U, U,  r")
;           (match_operand:QI 1 "pic30_wreg_or_near_operand" "U, a, ?r, r")))
;   (clobber (match_scratch:HI 2 "=r,X,r,X"))]
;  "(pic30_isa32_target())"
;  "@
;   mov %1,%2\;cp.b %0,%2
;   cp.b %0
;   mov %0,%2\;cp.b %2,%1
;   cp.b %0,%1"
;  [
;    (set_attr "cc" "set")
;    (set_attr "type" "etc")
;    (set_attr "op_type" "alu")
;  ]
;)

;;;;;;;;;;;;;;;;;;
;; half integer
;;;;;;;;;;;;;;;;;;

(define_insn "cmphi_imm_32"
  [(set (reg:CC CC_REG)
        (compare (match_operand:HI 0 "pic30_register_operand" "r")
                 (match_operand:HI 1 "pic30_M_operand"  "M")))
   (clobber (match_dup 0))]
  "(pic30_isa32_target())"
  "add.w #%J1,%0"
  [
    (set_attr "cc" "set")
    (set_attr "type" "etc")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "cmphi_sfr0_32"
  [(set (reg:CC CC_REG)
        (compare (match_operand:HI 0 "pic30_reg_or_near_operand" "U,r")
                 (match_operand:HI 1 "pic30_register_operand"    "a,r")))]
  "(pic30_isa32_target())"
  "@
   cp.w  %0
   cp.w %0,%1"
  [
    (set_attr "cc" "set")
    (set_attr "type" "etc")
    (set_attr "op_type" "alu")
  ]
)

;        (compare (match_operand:HI 0 "pic30_reg_or_near_operand" "Ur")

(define_insn "cmpz<mode>_sfr0_32"
  [(set (reg:CC CC_REG)
        (compare 
          (match_operand:RIM 0 "pic30_mode2_or_near_operand" "r,R<>,U")
          (const_int 0)))
  ]
  "(pic30_isa32_target())"
  "@
   cp0.%v0 %0
   sub.%v0 %0,#0,[w15]
   cp0.%v0 %0"
  [
    (set_attr "cc" "set")
    (set_attr "type" "etc,use,etc")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "cmp<mode>_32"
  [(set (reg:CC CC_REG)
        (compare 
          (match_operand:RIM16 0 "pic30_mode2_operand"   "r,r,  R<>,r,r")
          (match_operand:RIM16 1 "pic30_JMmath_operand" "r,R<>,r,  J,M")))]
  "(pic30_isa32_target())"
  "@
   cp.%v0 %0,%1
   cp.%v0 %0,%1
   subr.%v0 %1,%0,[w15]
   cp.%v0 %0,#%1
   cp.%v0 %0,#%1"
  [
   (set_attr "cc" "set")
   (set_attr "type" "etc,use,use,etc,etc")
   (set_attr "op_type" "alu")
  ]
)

(define_insn "cmpsi_32"
  [(set (reg:CC CC_REG)
        (compare 
          (match_operand:SI 0 "pic30_mode2_operand"   "r,r,  R<>,r,r")
          (match_operand:SI 1 "pic30_mode1JN_operand" "r,R<>,r,  J,N")))]
  "(pic30_isa32_target())"
  "@
   cp.%v0 %0,%1
   cp.%v0 %0,%1
   subr.%v0 %1,%0,[w15]
   cp.%v0 %0,#%1
   add.%v0 %0,#%J1,[w15]"
  [
   (set_attr "cc" "set")
   (set_attr "type" "etc,use,use,etc,etc")
   (set_attr "op_type" "alu")
  ]
)

;;;;;;;;;;;;;;;;;;
;; single integer
;;;;;;;;;;;;;;;;;;

(define_insn "cmpsi_imm_32"
  [(set (reg:CC CC_REG)
        (compare (match_operand:SI 0 "pic30_register_operand"  "r")
                 (match_operand:SI 1 "immediate_operand"       "i")))
   (clobber (match_scratch:SI      2                         "=&r"))
  ]
  "((1<INTVAL(operands[1])) && 
    (INTVAL(operands[1])<65536) && 
    (pic30_isa32_target()))"
  "mov.l #%1,%2\;sub.l %0,%2,[w15]"
  [
   (set_attr "cc" "clobber")
   (set_attr "type" "etc")
   (set_attr "op_type" "alu")
  ]
)

;;;;;;;;;;;;;;;;;;
;; double integer
;;;;;;;;;;;;;;;;;;

(define_insn "cmpdi_32"
  [(set (reg:CC CC_REG)
        (compare (match_operand:DI 0 "pic30_register_operand"    "r,r,r")
                 (match_operand:DI 1 "pic30_mode1J_operand"      "r,O,J")))
  ]
  "(pic30_isa32_target())"
  "@
   cp.l %0,%1\;cpb.l %d0,%d1
   cp.l %0,#0\;cpb.l %d0,#0
   cp.l %0,#%1\;cpb.l %d0,#0"
  [
    (set_attr "cc" "set")
    (set_attr "type" "etc")
    (set_attr "op_type" "alu")
  ])

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; truncation instructions
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;; TBLMODES/EDS modes not used for this family

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; zero extension instructions
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define_insn "zero_extendqihi2_32"
  [(set (match_operand:HI   0 "pic30_register_operand" "=r,r")
        (zero_extend:HI
          (match_operand:QI 1 "pic30_mode2_operand"     "r,R<>")))
   (clobber (reg:CC_NZ CC_REG))
  ]
  "(pic30_isa32_target())"
  "@ 
   ze.bz %1,%0
   ze.bz %1,%0"
  [
   (set_attr "cc" "math")
   (set_attr "type" "def,defuse")
   (set_attr "op_type" "alu")
  ]
)

(define_insn "zero_extendqisi2_32"
  [(set (match_operand:SI   0 "pic30_register_operand" "=r,r")
        (zero_extend:SI 
          (match_operand:QI 1 "pic30_mode2_operand"    " r,R<>")))
   (clobber (reg:CC_NZ CC_REG))
  ]
  "(pic30_isa32_target())"
  "@
   ze.bz %1,%0
   ze.bz %1,%0"
  [
   (set_attr "cc" "math")
   (set_attr "type" "def,defuse")
   (set_attr "op_type" "alu")
  ]
)

(define_insn "zero_extendhisi2_32"
  [(set (match_operand:SI   0 "pic30_register_operand" "=r")
        (zero_extend:SI 
          (match_operand:HI 1 "pic30_register_operand" " r")))
   (clobber (reg:CC_NZ CC_REG))
  ]
  "(pic30_isa32_target())"
  "ze.w %1,%0"
  [
   (set_attr "cc" "change0")
   (set_attr "type" "def")
   (set_attr "op_type" "alu")
  ]
)

(define_insn "zero_extendp32extsi2_32"
  [(set (match_operand:SI       0 "register_operand" "=r")
        (zero_extend:SI 
          (match_operand:P32EXT 1 "register_operand" " 0")))]
  "(pic30_isa32_target())"
  "; nop %1,%0"
)

;(define_insn "zero_extendsip32ext2_32"
;  [(set (match_operand:P32EXT   0 "register_operand" "=r")
;        (zero_extend:P32EXT 
;          (match_operand:SI     1 "register_operand" " 0")))]
;  "(pic30_isa32_target())"
;  "; nop %1,%0"
;)

(define_insn "zero_extendqidi2_32"
  [(set (match_operand:DI   0 "pic30_register_operand" "=r,r")
        (zero_extend:DI
          (match_operand:QI 1 "pic30_mode2_operand" " r,<>R")))
  ]
  "(pic30_isa32_target())"
  "ze.bz %1,%0\;mulus.d %0,#1,%0"
  [
   (set_attr "cc" "clobber")
   (set_attr "type" "def,defuse")
   (set_attr "op_type" "alu")
  ]
)

(define_insn "zero_extendhidi2_32"
  [(set (match_operand:DI   0 "pic30_register_operand" "=r,r")
        (zero_extend:DI
          (match_operand:HI 1 "pic30_mode2_operand" " r,<>R")))
  ]
  "(pic30_isa32_target())"
  "ze.w %1,%0\;mulus.d %0,#1,%0"
  [
   (set_attr "cc" "clobber")
   (set_attr "type" "def,defuse")
   (set_attr "op_type" "alu")
  ]
)

(define_insn "zero_extendsidi2_32"
  [(set (match_operand:DI   0 "pic30_register_operand" "=r,r")
        (zero_extend:DI
          (match_operand:SI 1 "pic30_mode2_operand" " r,<>R")))
  ]
  "(pic30_isa32_target())"
  "mulus.d %1,#1,%0"
  [
   (set_attr "cc" "clobber")
   (set_attr "type" "def,defuse")
   (set_attr "op_type" "alu")
  ]
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; sign extension instructions
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define_insn "extendqihi2_32"
  [(set (match_operand:HI   0 "pic30_register_operand" "=r,r")
        (sign_extend:HI 
          (match_operand:QI 1 "pic30_mode2_operand"    " r,R<>"))
   )
   (clobber (reg:CC_NZ CC_REG))
  ]
  "(pic30_isa32_target())"
  "se.b %1,%0"
  [
   (set_attr "cc" "math")
   (set_attr "type" "def,defuse")
   (set_attr "op_type" "alu")
  ]
)

(define_insn "extendqisi2_32"
  [(set (match_operand:SI   0 "pic30_register_operand" "=r,r")
        (sign_extend:SI 
          (match_operand:QI 1 "pic30_mode2_operand"     "r,R<>")))
   (clobber (reg:CC_NZ CC_REG))
  ]
  "(pic30_isa32_target())"
  "se.b %1,%0"
  [
   (set_attr "cc" "clobber")
   (set_attr "type" "def,defuse")
   (set_attr "op_type" "alu")
  ]
)

(define_insn "extendqidi2_32"
  [(set (match_operand:DI   0 "pic30_register_operand" "=r,r")
        (sign_extend:DI 
          (match_operand:QI 1 "pic30_mode2_operand"    " r,R<>")))
   (clobber (reg:CC_NZ CC_REG))
  ]
  "(pic30_isa32_target())"
  "se.b %1,%0\;asr.l %0,#31,%d0"
  [
   (set_attr "cc" "clobber")
   (set_attr "type" "def,defuse")
   (set_attr "op_type" "alu")
  ]
)

(define_insn "extendhisi2_32"
  [(set (match_operand:SI 0 "pic30_register_operand"   "=r")
        (sign_extend:SI
           (match_operand:HI 1 "pic30_register_operand" "r"))
   )
   (clobber (reg:CC_NZ CC_REG))
  ]
  "(pic30_isa32_target())"
  "se.w %1,%0"
  [
   (set_attr "cc" "clobber")
   (set_attr "type" "def")
   (set_attr "op_type" "alu")
  ]
)

(define_insn "extendhip32ext2_32"
  [(set (match_operand:P32EXT 0 "pic30_register_operand" "=r")
        (sign_extend:P32EXT 
          (match_operand:HI   1 "pic30_register_operand" " r")))
  ]
  "(pic30_isa32_target())"
  "*
   error(\"Instruction not yet ported to ISA32\n\");
  "
  [
   (set_attr "cc" "clobber")
   (set_attr "type" "def")
   (set_attr "op_type" "alu")
  ]
)

;(define_insn "zero_extendhip32ext2_32"
;  [(set (match_operand:P32EXT 0 "pic30_register_operand" "=r")
;        (zero_extend:P32EXT 
;         (match_operand:HI   1 "pic30_register_operand" " r")))
; ]
; "(pic30_isa32_target())"
; "*
;  error(\"Instruction not yet ported to ISA32\n\");
; "
; [
;  (set_attr "cc" "clobber")
;  (set_attr "type" "def")
;  (set_attr "op_type" "alu")
; ]
;)

(define_insn "extendsip32ext2_32"
  [(set (match_operand:P32EXT 0 "pic30_register_operand" "=r")
        (sign_extend:P32EXT 
          (match_operand:SI   1 "pic30_register_operand" " r")))
  ]
  "(pic30_isa32_target())"
  "mov.l %1,%0"
  [
   (set_attr "cc" "clobber")
   (set_attr "type" "def")
   (set_attr "op_type" "alu")
  ]
)

(define_insn "zero_extendhip32ext2_32"
  [(set (match_operand:P32EXT 0 "pic30_register_operand" "=r")
        (zero_extend:P32EXT 
          (match_operand:HI   1 "pic30_register_operand" " r")))
  ]
  "(pic30_isa32_target())"
  "mov.l %1,%0"
  [
   (set_attr "cc" "clobber")
   (set_attr "type" "def")
   (set_attr "op_type" "alu")
  ]
)

(define_insn "extendhidi2_32"
  [(set (match_operand:DI 0 "pic30_register_operand"                "=r,r")
        (sign_extend:DI (match_operand:HI 1 "pic30_register_operand" "0,r")) )
   (clobber (reg:CC_NZ CC_REG))
  ]
  "(pic30_isa32_target())"
   "*
    switch (which_alternative) {
      case 0: return \"se.w %0,%0\;asr.l %0,#31,%d0\";
      case 1: return \"se.w %1,%0\;asr.l %0,#31,%d0\";
      default: return \"invalid pattern\";
    }"
  [
   (set_attr "cc" "clobber")
   (set_attr "type" "def")
   (set_attr "op_type" "alu")
  ]
)

(define_insn "extendsidi2_32"
  [(set (match_operand:DI 0 "pic30_register_operand"    "=r,r")
        (sign_extend:DI 
            (match_operand:SI 1 "pic30_register_operand" "0,r")) 
   )
   (clobber (reg:CC_NZ CC_REG))
  ]
  "(pic30_isa32_target())"
  "@
   asr.l %0,#31,%d0
   mov.l %1,%0\;asr.l %0,#31,%d0"
  [
   (set_attr "cc" "clobber")
   (set_attr "type" "def")
   (set_attr "op_type" "mov")
  ]
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Move instructions
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;; Invalid move

(define_insn "*movqi_invalid_1_32"
  [(set (match_operand:QI 0 "pic30_register_operand"  "=r")
        (match_operand:QI 1 "pic30_code_operand"       "g"))]
  "(pic30_isa32_target())"
  "mov.sl #%1,%0\;mov.bz [%0],%0"
)

(define_insn "*movqi_invalid_2_32"
  [(set (match_operand:QI 0 "pic30_code_operand" "=g")
        (match_operand:QI 1 "pic30_register_operand"    "r"))]
  "(pic30_isa32_target())"
  "*
   {
        error(\"invalid address space for operand\");
        return \"cannot generate instruction\";
   }
  "
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; 8-bit moves
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define_insn "*movqi_const0_32"
  [(set (match_operand:QI 0 "pic30_mode2_operand" "=r,R<>")
	(const_int 0))]
  "(pic30_isa32_target())"
  "@
   clr.b %0
   clr.b %0"
  [
   (set_attr "cc" "change0,change0")
   (set_attr "type" "def,use")
   (set_attr "op_type" "mov")
  ]
)

(define_insn "*movqi_const1_32"
  [(set (match_operand:QI 0 "pic30_mode2_operand" "=r,R<>")
	(const_int -1))]
  "(pic30_isa32_target())"
  "@
   movs.b #0xFF,%0
   movs.b #0xFF,%0"
  [
   (set_attr "cc" "change0,change0")
   (set_attr "type" "def,use")
   (set_attr "op_type" "mov")
  ]
)

(define_insn "*movqi_const0sfr_32"
  [(set (match_operand:QI 0 "pic30_near_operand" "=U")
	(const_int 0))]
  "(pic30_isa32_target())"
  "clr.b %0"
  [
   (set_attr "cc" "change0")
   (set_attr "type" "etc")
   (set_attr "op_type" "mov")
  ]
)

(define_insn "*movqi_const1sfr_32"
  [(set (match_operand:QI 0 "pic30_near_operand" "=U")
	(const_int -1))]
  "(pic30_isa32_target())"
  "setm.b %0"
  [
   (set_attr "cc" "change0")
   (set_attr "type" "etc")
   (set_attr "op_type" "mov")
  ]
)

(define_insn "movqi_rimm_32"
  [(set (match_operand:QI 0 "pic30_mode2_operand" "=r,R<>")
        (match_operand:QI 1 "immediate_operand" "i,i"))]
  "(pic30_isa32_target())"
  "@
   mov.bz #%1,%0
   mov.b #%1,%0"
  [
   (set_attr "cc" "change0")
   (set_attr "type" "def,defuse")
   (set_attr "op_type" "mov")
  ]
)

(define_insn "movqi_ze_32"
  [(set (match_operand:SI   0 "pic30_register_operand" "=r")
        (zero_extend:SI
          (match_operand:QI 1 "pic30_register_operand" "r")))]
  "(pic30_isa32_target())"
  "return mov.bz %1,%0"
  [
   (set_attr "cc" "change0")
   (set_attr "type" "def")
   (set_attr "op_type" "mov")
  ]
)

; general case
(define_insn "movqi_gen_32"
  [(set (match_operand:QI 0 "pic30_move_operand32"
                 "=r,<>,RS,r,  <>,  R,   RS,r,    <>,  R,   Q,r,r,U,r")
        (match_operand:QI 1 "pic30_move_operand"
                  "r,r,r, <>RS,<>RS,<>RS,r, RS<>, RS<>,RS<>,r,Q,U,r,T"))
  ]
  "(pic30_isa32_target())"
  "*
  {

     switch (which_alternative) {
       default: gcc_assert(0);
       case 0:  return \"mov.bz %1,%0\";
       case 1:  return \"mov.b %1,%0\";
       case 2:  return \"mov.b %1,%0\";
       case 3:  return \"mov.bz %1,%0\";
       case 4:  return \"mov.b %1,%0\";
       case 5:  return \"mov.b %1,%0\";
       case 6:  return \"mov.b %1,%0\";
       case 7:  return \"mov.bz %1,%0\";
       case 8:  return \"mov.b %1,%0\";
       case 9:  return \"mov.b %1,%0\";
       case 10:  return \"mov.b %1,%0\";
       case 11:  return \"mov.bz %1,%0\";
       case 12: return \"mov.bz %1,%0\";
       case 13: return \"mov.b %1,%0\";
       case 14: return \"mov.sl #%1,%0\;mov.bz [%0],%0\";
    }
  }
  "
  [
   (set_attr "cc" "change0")
   (set_attr "type"
        "def,etc,etc,defuse,use,use,etc,defuse,use,use,etc,def,def,etc,def")
   (set_attr "op_type" "mov")
  ]
)

(define_insn "movqi_gen_a_32"
  [(set (match_operand:QI 0 "pic30_move_operand32"
		"=r,R,r<>, R,   r<>,RS,r<>,RS, Q,r,U")
        (match_operand:QI 1 "pic30_move2_operand"
		 "r,r,<>RS,<>RS,r,  r, R<>,R<>,r,Q,r"))
  ]
  "(pic30_isa32_target())"
  "*
  {
     switch (which_alternative) {
       default: gcc_assert(0);
       case 0:  return \"mov.bz %1,%0\";
       case 1:  return \"mov.b %1,%0\";
       case 2:  return \"mov.b %1,%0\";
       case 3:  return \"mov.b %1,%0\";
       case 4:  return \"mov.b %1,%0\";
       case 5:  return \"mov.b %1,%0\";
       case 6:  return \"mov.b %1,%0\";
       case 7:  return \"mov.b %1,%0\";
       case 8:  return \"mov.b %1,%0\";
       case 9:  return \"mov.bz %1,%0\";
       case 10: return \"mov.b %1,%0\";
    }
  }
  "
  [
   (set_attr "cc" "change0")
   (set_attr "type"
        "def,use,defuse,use,use,use,defuse,use,etc,def,etc")
   (set_attr "op_type" "mov")
  ]
)

(define_insn "movqi_gen_b_32"
  [(set (match_operand:QI 0 "pic30_move2_operand"
		"=r,R,r<>, R,   r<>,RS,r<>,RS, Q,r,r")
        (match_operand:QI 1 "pic30_move_operand32"
		 "r,r,<>RS,<>RS,r,  r, R<>,R<>,r,Q,U"))
  ]
  "(pic30_isa32_target())"
  "*
  {  
     switch (which_alternative) {
       default: gcc_assert(0);
       case 0:  return \"mov.bz %1,%0\";
       case 1:  return \"mov.b %1,%0\";
       case 2:  return \"mov.b %1,%0\";
       case 3:  return \"mov.b %1,%0\";
       case 4:  return \"mov.b %1,%0\";
       case 5:  return \"mov.b %1,%0\";
       case 6:  return \"mov.b %1,%0\";
       case 7:  return \"mov.b %1,%0\";
       case 8:  return \"mov.b %1,%0\";
       case 9:  return \"mov.bz %1,%0\";
       case 10: return \"mov.bz %1,%0\";
    }
  }
  "
  [
   (set_attr "cc" "change0")
   (set_attr "type"
        "def,use,defuse,use,defuse,use,defuse,use,etc,def,def")
   (set_attr "op_type" "mov")
  ]
)

;; If one of the operands is immediate and the other is not a register,
;; then we should emit two insns, using a scratch register.  This will produce
;; better code in loops if the source operand is invariant, since
;; the source reload can be optimised out.  During reload we cannot
;; use change_address or force_reg which will allocate new pseudo regs.

;; Unlike most other insns, the move insns can`t be split with
;; different predicates, because register spilling and other parts of
;; the compiler, have memoized the insn number already.

(define_expand "movqi_32"
  [(set (match_operand:QI 0 "pic30_general_operand" "")
        (match_operand:QI 1 "pic30_general_operand" ""))]
  "(pic30_isa32_target())"
  "
  {
     rtx op;

     /* sometimes get here from emit_move_insn even though the rhs is
      * an add or something.  we need to handle it now that there are
      * potential clobbers of the CC
      *
      * pic30_emit_move_sequence may update operands */
     if (pic30_emit_move_sequence(operands, QImode)) DONE;
     if (pic30_near_operand(operands[0], QImode) &&
        pic30_near_operand(operands[1], QImode)) {
        rtx op1 = force_reg(QImode, operands[1]);
        emit(gen_movqi_gen_32(operands[0], op1));
        DONE;
     }

     op = operands[1];

     while (GET_CODE(op) == CONST) op = XEXP(op,0);
     switch (GET_CODE(op)) {
       default: gcc_assert(0);
       case PLUS: {
         rtx op1 = XEXP(op,0);
         rtx op2 = XEXP(op,1);
         if (GET_CODE(op1) != SYMBOL_REF) {
           if (!pic30_math_operand(op1,QImode)) {
             op1 = force_reg(QImode,op1);
           }
           if (!pic30_mode1JN_operand(op2,QImode)) {
             op2 = force_reg(QImode,op2);
           }
           emit(
             gen_addqi3_32(operands[0], op1, op2)
           );
           DONE;
         }
         /* HANDLE SYMBOL_REF below ... */
         /* FALLSTHROUGH */
       }
       case CONST_INT:
         if (GET_CODE(op) == CONST_INT) {
           if (GET_CODE(operands[0]) == REG) {
             emit(
               gen_movqi_rimm_32(operands[0],operands[1])
             );
             DONE;
           } else {
             if (can_create_pseudo_p()) {
               rtx r = force_reg(GET_MODE(operands[0]), op);
               emit(
                 gen_movqi_gen_32(operands[0],r)
               );
               DONE;
             }
           }
           break;
         }
         /* FALLSTHROUGH to SYMBOL_REF */
       case SUBREG:
       case REG:
       case LABEL_REF:
       case SYMBOL_REF:
       case MEM:
         if (pic30_reload_in_progress() ||
             (pic30_move_operand(operands[0],QImode) &&
              pic30_move_operand(operands[1],QImode))) {
           emit(
             gen_movqi_gen_32(operands[0],operands[1])
           );
           DONE;
         }
         break;
     }
  }
  ")

;; Invalid move

(define_insn "*movhi_invalid_1_32"
  [(set (match_operand:HI 0 "pic30_register_operand" "=r")
        (match_operand:HI 1 "pic30_code_operand"  "g"))]
  "(pic30_isa32_target())"
  "mov #%1,%0\;mov.w [%0],%0"
)

(define_insn "*movhi_invalid_2_32"
  [(set (match_operand:HI 0 "pic30_code_operand" "=g")
        (match_operand:HI 1 "pic30_register_operand"  "r"))]
  "(pic30_isa32_target())"
  "*
   {
        error(\"invalid address space for operand\");
        return \"cannot generate instruction\";
   }
")

(define_insn "*movhi_invalid_3_32"
  [(set (match_operand:HI 0 "pic30_move_operand32" "")
        (mem: HI (match_operand:HI 1 "pic30_invalid_address_operand"  "")))]
  "(pic30_isa32_target())"
  "*
   {
        error(\"invalid address space for operand\");
        return \"cannot generate instruction\";
   }
")

(define_insn "*movhi_invalid_4_32"
  [(set (mem (match_operand:P24PROG 0 "pic30_prog_operand" ""))
        (match_operand 1 "general_operand" ""))]
  "(pic30_isa32_target())"
  "*
   {
     error(\"invalid address space for destination\");
     return \"cannot generate instruction\";
   }
")

(define_insn "*movhi_invalid_5_32"
  [(set (mem (match_operand:P24PSV 0 "pic30_psv_operand" ""))
        (match_operand 1 "general_operand" ""))]
  "(pic30_isa32_target())"
  "*
   {
     error(\"invalid address space for destination\");
     return \"cannot generate instruction\";

   }
")

(define_insn "*movhi_invalid_6_32"
  [(set (mem (match_operand:P32DF 0 "pic30_df_operand" ""))
        (match_operand 1 "general_operand" ""))]
  "(pic30_isa32_target())"
  "*
   {
     error(\"invalid address space for destination\");
     return \"cannot generate instruction\";
   }
")

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; 16-bit moves
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;
;; immediate zero
;;;;;;;;;;;;;;;;;

(define_insn "*movhi_const0sfr_32"
  [(set (match_operand:HI 0 "pic30_near_operand" "=U")
	(const_int 0))]
  "(pic30_isa32_target())"
  "clr.w %0"
  [
   (set_attr "cc" "change0")
   (set_attr "type" "etc")
   (set_attr "op_type" "mov")
  ]
)

(define_insn "*movhi_const1sfr_32"
  [(set (match_operand:HI 0 "pic30_near_operand" "=U")
	(const_int -1))]
  "(pic30_isa32_target())"
  "setm.w %0"
  [
   (set_attr "cc" "change0")
   (set_attr "type" "etc")
   (set_attr "op_type" "mov")
  ]
)

(define_insn "movhi_const0_32"
  [(set (match_operand:HI 0 "pic30_mode2_operand" "=r,R<>")
	(const_int 0))]
  "(pic30_isa32_target())"
  "@
   movs.w #0,%0
   movs.w #0,%0"
  [
   (set_attr "cc" "change0,change0")
   (set_attr "type" "def,use")
   (set_attr "op_type" "mov")
  ]
)

(define_insn "*movhi_const1_32"
  [(set (match_operand:HI 0 "pic30_mode2_operand" "=r,R<>")
	(const_int -1))]
  "(pic30_isa32_target())"
  "@
   movs.w #0xFFFF,%0
   movs.w #0xFFFF,%0"
  [
   (set_attr "cc" "change0")
   (set_attr "type" "def,use")
   (set_attr "op_type" "mov")
  ]
)

;;;;;;;;;;;;;;;;;;;;
;; general immediate
;;;;;;;;;;;;;;;;;;;;

(define_insn "movqi_address_32"
  [(set (match_operand:QI 0 "pic30_register_operand"              "=r")
        (subreg:QI
          (match_operand:SI 1 "pic30_symbolic_address_operand"    " qs") 0))
  ]
  "(pic30_isa32_target())"
  "movs.b #%1,%0"
  [
   (set_attr "cc" "change0")
   (set_attr "type" "def")
   (set_attr "op_type" "movlit")
  ]
)

(define_insn "movhi_imm_32"
  [(set (match_operand:HI 0 "pic30_register_operand" "=r")
        (match_operand:HI 1 "immediate_operand"      " i"))
  ]
  "(pic30_isa32_target())"
  "mov.w #%1,%0"
  [
   (set_attr "cc" "change0")
   (set_attr "type" "def")
   (set_attr "op_type" "movlit")
  ]
)

;;;;;;;;;;;;;;;;;;;;;;;;;;
;; builtin move directives
;;;;;;;;;;;;;;;;;;;;;;;;;;

(define_insn "readsfr_32"
  [(set (match_operand:SI   0 "pic30_register_operand" "=r")
        (unspec_volatile:SI [
          (match_operand:SI 1 "pic30_register_operand" " r")
         ] UNSPECV_READSFR))
  ]
  "(pic30_isa32_target())"
  "mov.l [%1],%0 "
  [
   (set_attr "cc"   "change0")
   (set_attr "type" "def")
   (set_attr "op_type" "mov")
  ]
)

(define_insn "writesfr_32"
  [(unspec_volatile:SI [
     (match_operand:SI 0 "pic30_register_operand" "r")
     (match_operand:SI 1 "pic30_register_operand" "r")
  ] UNSPECV_WRITESFR)]
  "(pic30_isa32_target())"
  "mov.l %1,[%0]"
  [
   (set_attr "cc" "change0")
   (set_attr "type" "def")
   (set_attr "op_type" "mov")
  ]
)

(define_insn "dmaoffset_32"
  [(set (match_operand:SI 0 "pic30_register_operand"        "=r")
        (dmaoffset:SI 
          (match_operand  1 "pic30_symbolic_address_operand" "qs")))
  ]
  "(pic30_isa32_target())"
  "*
   error(\"Instruction not yet ported to ISA32\n\");
  "
  [
   (set_attr "cc" "change0")
   (set_attr "type" "def")
   (set_attr "op_type" "movlit")
  ]
)

(define_insn "addr_long_32"
  [(set (match_operand:SI 0 "pic30_register_operand"       "=r,r")
        (addr_long:SI
           (match_operand 1 "pic30_reg_or_symbolic_address" "qs,r")))]
  "(pic30_isa32_target())"
  "*
   error(\"Instruction not yet ported to ISA32\n\");
  "
  [
   (set_attr "cc" "change0")
   (set_attr "type" "def")
   (set_attr "op_type" "movlit,mov")
  ]
)


(define_insn "set_fsr_32"
  [(unspec_volatile [
     (match_operand:SI 0 "pic30_mode3_operand" "=rRS<>")] UNSPECV_SET_FSR)]
  "(pic30_isa32_target())"
  "mov.l %0,FSR "
  [
   (set_attr "type" "def")
   (set_attr "op_type" "mov")
  ]
)

(define_insn "get_fsr_32"
  [(set (match_operand:SI 0 "pic30_mode3_operand" "=rRS<>")
        (unspec_volatile:SI [ (const_int 0) ] UNSPECV_GET_FSR))]
  "(pic30_isa32_target())"
  "mov.l FSR,%0"
  [
   (set_attr "type" "def")
   (set_attr "op_type" "mov")
  ]
)

(define_insn "set_fcr_32"
  [(unspec_volatile [
     (match_operand:SI 0 "pic30_mode3_operand" "=rRS<>")] UNSPECV_SET_FCR)]
  "(pic30_isa32_target())"
  "mov.l %0,FCR "
  [
   (set_attr "type" "def")
   (set_attr "op_type" "mov")
  ]
)

(define_insn "get_fcr_32"
  [(set (match_operand:SI 0 "pic30_mode3_operand" "=rRS<>")
        (unspec_volatile:SI [ (const_int 0) ] UNSPECV_GET_FCR))]
  "(pic30_isa32_target())"
  "mov.l FCR,%0"
  [
   (set_attr "type" "def")
   (set_attr "op_type" "mov")
  ]
)

(define_insn "set_fear_32"
  [(unspec_volatile [
     (match_operand:SI 0 "pic30_mode3_operand" "=rRS<>")] UNSPECV_SET_FEAR)]
  "(pic30_isa32_target())"
  "mov.l %0,FEAR "
  [
   (set_attr "type" "def")
   (set_attr "op_type" "mov")
  ]
)

(define_insn "get_fear_32"
  [(set (match_operand:SI 0 "pic30_mode3_operand" "=rRS<>")
        (unspec_volatile:SI [ (const_int 0) ] UNSPECV_GET_FEAR))]
  "(pic30_isa32_target())"
  "mov.l FEAR,%0"
  [
   (set_attr "type" "def")
   (set_attr "op_type" "mov")
  ]
)

(define_insn "ior_fcr_32"
  [(unspec_volatile [
     (match_operand:HI 0 "immediate_operand" "i")] UNSPECV_IOR_FCR)]
  "pic30_isa32_target()"
  "ior #%0,FCR "
  [
   (set_attr "type" "def")
   (set_attr "op_type" "mov")
  ]
)

(define_insn "and_fcr_32"
  [(unspec_volatile [
     (match_operand:HI 0 "immediate_operand" "i")] UNSPECV_AND_FCR)]
  "pic30_isa32_target()"
  "and #%0,FCR "
  [
   (set_attr "type" "def")
   (set_attr "op_type" "mov")
  ]
)
;;;;;;;;;;;;;;;
;; general case
;;;;;;;;;;;;;;;

(define_insn "movacchi_32"
  [(set (match_operand:HI 0 "pic30_register_operand"    "=r")
        (match_operand:HI 1 "pic30_accumulator_operand" " w"))]
  "(pic30_isa32_target()) && pic30_integer_mac_support"
  "*
   error(\"Instruction not yet ported to ISA32\n\");
  "
)

(define_insn "movhiacc_32"
  [(set (match_operand:HI 0 "pic30_accumulator_operand" "=w")
        (match_operand:HI 1 "pic30_register_operand"    " r"))]
  "(pic30_isa32_target()) && pic30_integer_mac_support"
  "*
   error(\"Instruction not yet ported to ISA32\n\");
  "
)

(define_insn "movhi_gen_32"
  [(set (match_operand:HI 0 
           "pic30_move_operand32" "=r<>, R,   r<>,R,S,S,  Q,r,r,U,r")
        (match_operand:HI 1
	   "pic30_move_operand"   " RS<>,RS<>,r,  r,r,<>R,r,Q,U,r,T"))
  ]
  "(pic30_isa32_target())"
  "*
   { 
     const char *format[] = {
       \"mov.w %1,%0\",
       \"mov.w %1,%0\",
       \"mov.w %1,%0\",
       \"mov.w %1,%0\",
       \"mov.w %1,%0\",
       \"mov.w %1,%0\",
       \"mov.w %1,%0\",
       \"mov.w %1,%0\",
       \"mov.w %1,%0\",
       \"mov.w %1,%0\",
       \"mov.sl #%1,%0\;mov.w [%0],%0\",
     };

     #define IDENT_EXTERNAL(t) \
       ((t) == pic30_identExternal[0] || (t) == pic30_identExternal[1])

     if ((which_alternative == 8) || (which_alternative == 10)) {
       tree decl = 0;
       if (GET_CODE(XEXP(operands[1],0)) == SYMBOL_REF)
         decl = SYMBOL_REF_DECL(XEXP(operands[1],0));
       if (decl) {
         tree attr = pic30_lookup_attribute(IDENTIFIER_POINTER(pic30_identSpace[0]),
                                      DECL_ATTRIBUTES(decl));
         if (attr &&
             (TREE_CODE(TREE_VALUE(TREE_VALUE(attr))) == CALL_EXPR)) {
           tree id;
           id = TREE_OPERAND(CALL_EXPR_FN(TREE_VALUE(TREE_VALUE(attr))),0);
           if (IDENT_EXTERNAL(DECL_NAME(id))) return \"mov.w #%1,%0\";
         }
       }
     }
     return format[which_alternative];
   }"
  [
   (set_attr "cc" "change0")
   (set_attr "type" 
             "defuse,use,def,use,use,use,etc,def,def,etc,def")
   (set_attr "op_type" "mov")
  ]
)

;; If one of the operands is immediate and the other is not a register,
;; then we should emit two insns, using a scratch register.  This will produce
;; better code in loops if the source operand is invariant, since
;; the source reload can be optimised out.  During reload we cannot
;; use change_address or force_reg which will allocate new pseudo regs.

;; Unlike most other insns, the move insns can`t be split with
;; different predicates, because register spilling and other parts of
;; the compiler, have memoized the insn number already.

(define_expand "movhi_32"
  [(set (match_operand:HI 0 "pic30_move_operand" "")
        (match_operand:HI 1 "pic30_move_operand" ""))]
  "(pic30_isa32_target())"
  "
  {
     rtx op;

     /* sometimes get here from emit_move_insn even though the rhs is
      * an add or something.  we need to handle it now that there are
      * potential clobbers of the CC
      *
      * pic30_emit_move_sequence may update operands */
     if (pic30_emit_move_sequence(operands, HImode)) DONE;

     op = operands[1];

     while (GET_CODE(op) == CONST) op = XEXP(op,0);
     switch (GET_CODE(op)) {
       default: gcc_assert(0);
       case PLUS: {
         rtx op1 = XEXP(op,0);
         rtx op2 = XEXP(op,1);
         if (GET_CODE(op1) != SYMBOL_REF) {
           if (!pic30_JMmath_operand(op1,HImode)) {
             op1 = force_reg(HImode,op1);
           }
           if (!pic30_JMmath_operand(op2,HImode)) {
             op2 = force_reg(HImode,op2);
           }
           emit(
             gen_addhi3_32(operands[0], op1, op2)
           );
           DONE;
         }
         /* handle SYMBOL_REF below */
         /* FALLSTHROUGH */
       }
       case CONST_INT:
         if (GET_CODE(op) == CONST_INT) {
           if (GET_CODE(operands[0]) == REG) {
             emit(
               gen_movhi_imm_32(operands[0],operands[1])
             );
             DONE;
           } else {
             if (can_create_pseudo_p()) {
               rtx r = force_reg(GET_MODE(operands[0]), op);
               emit(
                 gen_movhi_gen_32(operands[0],r)
               );
               DONE;
             }
           }
           break;
         }
         /* FALLSTHROUGH to SYMBOL_REF */
       case SUBREG:
       case REG:
       case LABEL_REF:
       case SYMBOL_REF:
       case MEM:
         if (pic30_reload_in_progress() ||
             (pic30_move_operand(operands[0],HImode) &&
              pic30_move_operand(operands[1],HImode))) {
           emit(
             gen_movhi_gen_32(operands[0],operands[1])
           );
           DONE;
         }
         break;
     }
  }"
)

;;
;; Reload can generate a partial load of a larger item
;;   Recognize the instruction. - Bug in nullstone cse -O[23]
;;

(define_insn "movhi_reload_lo_32"
  [(set (match_operand:HI    0 "pic30_move_operand" "=r,RS,r,R,a,T,r")
        (subreg:HI 
           (match_operand:SI 1 "pic30_move_operand" " r,r,RS,R,U,r,T") 0))]
  "(pic30_isa32_target())"
  "@
   mov.w %1,%0
   mov.w %1,%0
   mov.w %1,%0
   mov.w %1,%0
   mov.w %1,%0
   mov.w %1,%0
   mov.w %1,%0"
  [
   (set_attr "cc" "change0,change0,change0,change0,move,change0,change0")
   (set_attr "type" "def,use,defuse,use,def,etc,def")
   (set_attr "op_type" "mov")
  ]
)

(define_insn "movhi_reload_hi_32"
  [(set (match_operand:HI 0 "pic30_reg_or_near_operand"   "=a,r")
        (subreg:HI 
           (match_operand:SI 1 "pic30_UT_operand" "U,T") 2))]
  "(pic30_isa32_target())"
  "@
   mov.w %1+2,%0
   mov.w %1+2,%0"
  [
   (set_attr "cc" "move,change0")
   (set_attr "type" "def,def")
   (set_attr "op_type" "mov")
  ]
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; 24-bit moves
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define_expand "movp32ext_address_32"
  [(set (match_operand:P32EXT 0 "pic30_move_operand32"             "=rR<>QSTU")
        (match_operand:P32EXT 1 "pic30_symbolic_address_operand" " qs"))]
  "(pic30_isa32_target())"
  "{
     if (pic30_address_of_external(operands[0],operands[1])) {
       DONE;
     } else FAIL;
   }
  "
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; 32-EXT moves
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define_expand "movp32ext_32"
  [(set (match_operand:P32EXT 0 "pic30_general_operand" "")
        (match_operand:P32EXT 1 "pic30_general_operand" ""))]
  "(pic30_isa32_target())"
  "
  {
     rtx op;

     /* sometimes get here from emit_move_insn even though the rhs is
      * an add or something.  we need to handle it now that there are
      * potential clobbers of the CC
      *
      * pic30_emit_move_sequence may update operands */
     if (pic30_emit_move_sequence(operands, P32EXTmode)) DONE;

     op = operands[1];

     while (GET_CODE(op) == CONST) op = XEXP(op,0);
     switch (GET_CODE(op)) {
       default: gcc_assert(0);
       case PLUS: {
         rtx op1 = XEXP(op,0);
         rtx op2 = XEXP(op,1);
         if (GET_CODE(op1) != SYMBOL_REF) {
           if (pic30_register_operand(operands[0],P32EXTmode) &&
               pic30_register_operand(op1,P32EXTmode)) {
             emit(
               gen_addp32ext3_32(operands[0], op1, op2)
             );
             DONE;
           }
         }
         /* handle SYMBOL_REF below ... */
         /* FALLSTHROUGH */
       }
       case CONST_INT:
         if (GET_CODE(op) == CONST_INT) {
           if (GET_CODE(operands[0]) == REG) {
             emit(
               gen_movp32ext_rimm_32(operands[0],operands[1])
             );
             DONE;
           } else {
             if (can_create_pseudo_p()) {
               rtx r = force_reg(GET_MODE(operands[0]), op);
               emit(
                 gen_movp32ext_gen_32(operands[0],r)
               );
               DONE;
             }
           }
           break;
         }
         /* FALLSTHROUGH to handle SYBMOL_REF */
       case SUBREG:
       case REG:
       case SYMBOL_REF:
       case MEM:
         if (pic30_reload_in_progress() ||
             (pic30_move_operand(operands[0],P32EXTmode) &&
              pic30_move_operand(operands[1],P32EXTmode))) {
           emit(
             gen_movp32ext_gen_32(operands[0],operands[1])
           );
           DONE;
         }
         break;
     }
  }"
)

;; rotate

(define_insn "rotl<mode>_32"
  [(set (match_operand:RIM 0 "pic30_mode2_operand"   "=r,r,  R<>,R<>")
        (rotate:RIM 
           (match_operand:RIM 1 "pic30_mode2_operand" "r,R<>,r,  R<>")
           (const_int 1)))
   (clobber (reg:CC_NZ CC_REG))
  ]
  "(pic30_isa32_target())"
  "@
   rlnc.%v0 %1,%0
   rlnc.%v0 %1,%0
   rlnc.%v0 %1,%0
   rlnc.%v0 %1,%0"
  [ 
   (set_attr "cc" "math") 
   (set_attr "type" "def,defuse,use,use")
   (set_attr "op_type" "alu")
  ]
)

(define_expand "rotl<mode>3_32"
  [(set (match_operand:RIM 0 "pic30_mode2_operand" "")
        (rotate:RIM 
          (match_operand:RIM 1 "pic30_mode2_APSV_operand" "")
          (match_operand:RIM 2 "immediate_operand" "")))]
  "(pic30_isa32_target())"
  "
{  if (immediate_operand(operands[2],VOIDmode) && (INTVAL(operands[2]) == 1)) {
     emit_insn(gen_rotl<mode>_32(operands[0], operands[1]));
     DONE;
   } else FAIL;
}")

(define_insn "rotr<mode>_32"
  [(set (match_operand:RIM 0 "pic30_mode2_operand"   "=r,r,  R<>,R<>")
        (rotatert:RIM 
           (match_operand:RIM 1 "pic30_mode2_operand" "r,R<>,r,  R<>")
           (const_int 1)))
   (clobber (reg:CC_NZ CC_REG))
  ]
  "(pic30_isa32_target())"
  "@
   rrnc.%v0 %1,%0
   rrnc.%v0 %1,%0
   rrnc.%v0 %1,%0
   rrnc.%v0 %1,%0"
  [ 
   (set_attr "cc" "math") 
   (set_attr "type" "def,defuse,use,use")
   (set_attr "op_type" "alu")
  ]
)

(define_expand "rotr<mode>3_32"
  [(set (match_operand:RIM 0 "pic30_mode2_operand" "")
        (rotatert:RIM 
          (match_operand:RIM 1 "pic30_mode2_APSV_operand" "")
          (match_operand:RIM 2 "immediate_operand" "")))]
  "(pic30_isa32_target())"
  "
{  if (immediate_operand(operands[2],VOIDmode) && (INTVAL(operands[2]) == 1)) {
     emit_insn(gen_rotr<mode>_32(operands[0], operands[1]));
     DONE;
   } else FAIL;
}")

;;;;;;;;;;;;;;;;;;;;;;; EXT
;;

(define_expand "P32EXTrd_qi_32"
   [(set (match_operand:QI 0 "pic30_move_operand"       "=r,R,rR,r,R")
         (mem:QI
           (match_operand:P32EXT 1 "pic30_ext_operand" "r,r,RQ,ST,ST")))]
   "pic30_assert(0)"
   "{
      if (pic30_read_externals(pst_8)) {
        rtx input = gen_reg_rtx(P32EXTmode);

        emit_move_insn(input, operands[1]);
        emit_library_call_value(XEXP(DECL_RTL(pic30_read_externals(pst_8)),0),
                          operands[0], LCT_NORMAL, GET_MODE(operands[0]),
                          input, P32EXTmode);
        DONE;
      } else if (pic30_read_externals(pst_any)) {
        rtx input = gen_reg_rtx(P32EXTmode);
        rtx temp = assign_stack_temp(GET_MODE(operands[0]),
                                     GET_MODE_SIZE(GET_MODE(operands[0])));
        rtx param2,param3;
  
        emit_move_insn(input, operands[1]);
        param3 = gen_reg_rtx(HImode);
        param2 = gen_reg_rtx(Pmode);
        emit_move_insn(param3, GEN_INT(1));
        emit_move_insn(param2, XEXP(temp,0));
        emit_library_call(XEXP(DECL_RTL(pic30_read_externals(pst_any)),0),
                          LCT_NORMAL, GET_MODE(operands[0]),
                          input, P32EXTmode,
                          param2, Pmode,
                          param3, HImode);
        emit_move_insn(operands[0], temp);
        DONE;
      } else {
        error(\"Cannot access external memory space;\n\"
              \"\tdeclare __read_external or __read_external8\");
        DONE;
      }
    }"
)

(define_expand "P32EXTwt_qi_32"
   [(set (mem:QI (match_operand:P32EXT 0 "pic30_ext_operand" "r,r,RQ,ST,ST"))
         (match_operand:QI 1 "pic30_move_operand"            "r,R,rR,r,R"))]
   "pic30_assert(0)"
   "{
      if (pic30_write_externals(pst_8)) {
        rtx input = gen_reg_rtx(P32EXTmode);
        rtx input1 = gen_reg_rtx(GET_MODE(operands[1]));

        emit_move_insn(input, operands[0]);
        emit_move_insn(input1, operands[1]);
        emit_library_call(XEXP(DECL_RTL(pic30_write_externals(pst_8)),0),
                          LCT_NORMAL, VOIDmode,
                          input, P32EXTmode,
                          input1, GET_MODE(operands[1]));
        DONE;
      } else if (pic30_write_externals(pst_any)) {
        rtx input = gen_reg_rtx(P32EXTmode);
        rtx temp = assign_stack_temp(GET_MODE(operands[1]),
                                     GET_MODE_SIZE(GET_MODE(operands[1])));
        rtx param2,param3;
  
        emit_move_insn(input, operands[0]);
        emit_move_insn(temp, operands[1]);
        param3 = gen_reg_rtx(HImode);
        param2 = gen_reg_rtx(Pmode);
        emit_move_insn(param3, GEN_INT(1));
        emit_move_insn(param2, XEXP(temp,0));
        emit_library_call(XEXP(DECL_RTL(pic30_write_externals(pst_any)),0),
                          LCT_NORMAL, GET_MODE(operands[0]),
                          input, P32EXTmode,
                          param2, Pmode,
                          param3, HImode);
        DONE;
      } else {
        error(\"Cannot access external memory space;\n\"
              \"\tdeclare __write_external or __write_external8\");
        DONE;
      }
    }"
)

(define_expand "P32EXTrd_<mode>_32"
   [(set (match_operand:EXT16BITRD 0 "pic30_move_operand"       "=r,R,rR,r,R")
         (mem:EXT16BITRD
           (match_operand:P32EXT 1 "pic30_ext_operand" "r,r,RQ,ST,ST")))]
   "pic30_assert(0)"
   "{
      if (pic30_read_externals(pst_16)) {
        rtx input = gen_reg_rtx(P32EXTmode);

        emit_move_insn(input, operands[1]);
        emit_library_call_value(XEXP(DECL_RTL(pic30_read_externals(pst_16)),0),
                          operands[0], LCT_NORMAL, GET_MODE(operands[0]), 
                          input, P32EXTmode);
        DONE;
      } else if (pic30_read_externals(pst_any)) {
        rtx input = gen_reg_rtx(P32EXTmode);
        rtx temp = assign_stack_temp(GET_MODE(operands[0]),
                                     GET_MODE_SIZE(GET_MODE(operands[0])));
        rtx param2,param3;
  
        emit_move_insn(input, operands[1]);
        param3 = gen_reg_rtx(HImode);
        param2 = gen_reg_rtx(Pmode);
        emit_move_insn(param3, GEN_INT(2));
        emit_move_insn(param2, XEXP(temp,0));
        emit_library_call(XEXP(DECL_RTL(pic30_read_externals(pst_any)),0),
                          LCT_NORMAL, GET_MODE(operands[0]),
                          input, P32EXTmode,
                          param2, Pmode,
                          param3, HImode);
        emit_move_insn(operands[0], temp);
        DONE;
      } else if (pic30_read_externals(pst_8)) {
        rtx input = gen_reg_rtx(P32EXTmode);

        emit_move_insn(input, operands[1]);
        emit_library_call_value(XEXP(DECL_RTL(pic30_read_externals(pst_8)),0),
          simplify_gen_subreg(QImode, operands[0], GET_MODE(operands[0]), 0), 
          LCT_NORMAL, QImode, input, P32EXTmode);
        emit_insn(
          gen_addp32ext3_imm_32(input, input, GEN_INT(1))
        );
        emit_library_call_value(XEXP(DECL_RTL(pic30_read_externals(pst_8)),0),
          simplify_gen_subreg(QImode, operands[0], GET_MODE(operands[0]), 1), 
          LCT_NORMAL, QImode, input, P32EXTmode);
        DONE;
      } else {
        error(\"Cannot access external memory space;\n\"
              \"\tdeclare __read_external or __read_external16\");
      }
    }"
)

(define_expand "P32EXTwt_<mode>_32"
   [(set (mem:EXT16BITWT
           (match_operand:P32EXT 0 "pic30_ext_operand"          "r,r,RQ,ST,ST"))
         (match_operand:EXT16BITWT 1 "pic30_move_operand"       "r,R,rR,r,R"))]
   "pic30_assert(0)"
   "{
      if (pic30_write_externals(pst_16)) {
        rtx input = gen_reg_rtx(P32EXTmode);
        rtx input1 = gen_reg_rtx(GET_MODE(operands[1]));

        emit_move_insn(input, operands[0]);
        emit_move_insn(input1, operands[1]);
        emit_library_call(XEXP(DECL_RTL(pic30_write_externals(pst_16)),0),
                          LCT_NORMAL, VOIDmode,
                          input, P32EXTmode,
                          input1, GET_MODE(operands[1]));
        DONE;
      } else if (pic30_write_externals(pst_any)) {
        rtx input = gen_reg_rtx(P32EXTmode);
        rtx temp = assign_stack_temp(GET_MODE(operands[1]),
                                     GET_MODE_SIZE(GET_MODE(operands[1])));
        rtx param2,param3;
 
        emit_move_insn(input, operands[0]);
        emit_move_insn(temp, operands[1]);
        param3 = gen_reg_rtx(HImode);
        param2 = gen_reg_rtx(Pmode);
        emit_move_insn(param3, GEN_INT(2));
        emit_move_insn(param2, XEXP(temp,0));
        emit_library_call(XEXP(DECL_RTL(pic30_write_externals(pst_any)),0),
                          LCT_NORMAL, GET_MODE(operands[0]),
                          input, P32EXTmode,
                          param2, Pmode,
                          param3, HImode);
        DONE;
      } else if (pic30_write_externals(pst_8)) {
        rtx input = gen_reg_rtx(P32EXTmode);
        rtx input1 = gen_reg_rtx(QImode);

        emit_move_insn(input, operands[0]);
        emit_move_insn(input1, 
          simplify_gen_subreg(QImode, operands[1], GET_MODE(operands[1]),0));
        emit_library_call(XEXP(DECL_RTL(pic30_write_externals(pst_8)),0),
                          LCT_NORMAL, VOIDmode,
                          input, P32EXTmode,
                          input1, QImode);
        emit_insn(
          gen_addp32ext3_imm_32(input, input, GEN_INT(1))
        );
        emit_insn(
          gen_addqi3_32(input1, input1, GEN_INT(1))
        );
        emit_library_call(XEXP(DECL_RTL(pic30_write_externals(pst_8)),0),
                          LCT_NORMAL, VOIDmode,
                          input, P32EXTmode,
                          input1, QImode);
        DONE;
      } else {
        error(\"Cannot access external memory space;\n\"
              \"\tdeclare __write_external or __write_external16\");
      }
    }"
)

(define_expand "P32EXTrd_si_32"
   [(set (match_operand:SI 0 "pic30_move_operand"       "=r,R,rR,r,R")
         (mem:SI
           (match_operand:P32EXT 1 "pic30_ext_operand" "r,r,RQ,ST,ST")))]
   "pic30_assert(0)"
   "{
      if (pic30_read_externals(pst_32)) {
        rtx input = gen_reg_rtx(P32EXTmode);

        emit_move_insn(input, operands[1]);
        emit_library_call_value(XEXP(DECL_RTL(pic30_read_externals(pst_32)),0),
                          operands[0], LCT_NORMAL, GET_MODE(operands[0]),
                          input, P32EXTmode);
        DONE;
      } else if (pic30_read_externals(pst_any)) {
        rtx input = gen_reg_rtx(P32EXTmode);
        rtx temp = assign_stack_temp(GET_MODE(operands[0]),
                                     GET_MODE_SIZE(GET_MODE(operands[0])));
        rtx param2,param3;

        emit_move_insn(input, operands[1]);
        param3 = gen_reg_rtx(HImode);
        param2 = gen_reg_rtx(Pmode);
        emit_move_insn(param3, GEN_INT(4));
        emit_move_insn(param2, XEXP(temp,0));
        emit_library_call(XEXP(DECL_RTL(pic30_read_externals(pst_any)),0),
                          LCT_NORMAL, GET_MODE(operands[0]),
                          input, P32EXTmode,
                          param2, Pmode,
                          param3, HImode);
        emit_move_insn(operands[0], temp);
        DONE;
      } else {
        error(\"Cannot access external memory space;\n\"
              \"\tdeclare __read_external or __read_external32\");
      }
    }"
)

(define_expand "P32EXTwt_si_32"
   [(set (mem:SI (match_operand:P32EXT 0 "pic30_ext_operand" "r,r,RQ,ST,ST"))
         (match_operand:SI 1 "pic30_move_operand"       "r,R,rR,r,R"))]
   ""
   "{
      if (pic30_write_externals(pst_32)) {
        rtx input = gen_reg_rtx(P32EXTmode);
        rtx input1 = gen_reg_rtx(GET_MODE(operands[1]));

        emit_move_insn(input, operands[0]);
        emit_move_insn(input1, operands[1]);
        emit_library_call(XEXP(DECL_RTL(pic30_write_externals(pst_32)),0),
                          LCT_NORMAL, VOIDmode,
                          input, P32EXTmode,
                          input1, GET_MODE(operands[1]));
        DONE;
      } else if (pic30_write_externals(pst_any)) {
        rtx input = gen_reg_rtx(P32EXTmode);
        rtx temp = assign_stack_temp(GET_MODE(operands[1]),
                                     GET_MODE_SIZE(GET_MODE(operands[1])));
        rtx param2,param3;
 
        emit_move_insn(input, operands[0]);
        emit_move_insn(temp, operands[1]);
        param3 = gen_reg_rtx(HImode);
        param2 = gen_reg_rtx(Pmode);
        emit_move_insn(param3, GEN_INT(4));
        emit_move_insn(param2, XEXP(temp,0));
        emit_library_call(XEXP(DECL_RTL(pic30_write_externals(pst_any)),0),
                          LCT_NORMAL, GET_MODE(operands[0]),
                          input, P32EXTmode,
                          param2, Pmode,
                          param3, HImode);
        DONE;
      } else {
        error(\"Cannot access external memory space;\n\"
              \"\tdeclare __write_external or __write_external32\");
      }
    }"
)

(define_expand "P32EXTrd_p32ext_32"
   [(set (match_operand:P32EXT 0 "pic30_move_operand"       "=r,R,rR,r,R")
         (mem:P32EXT
           (match_operand:P32EXT 1 "pic30_ext_operand" "r,r,RQ,ST,ST")))]
   "pic30_assert(0)"
   "{
      if (pic30_read_externals(pst_32)) {
        rtx input = gen_reg_rtx(P32EXTmode);

        emit_move_insn(input, operands[1]);
        emit_library_call_value(XEXP(DECL_RTL(pic30_read_externals(pst_32)),0),
                          operands[0], LCT_NORMAL, GET_MODE(operands[0]),
                          input, P32EXTmode);
        DONE;
      } else if (pic30_read_externals(pst_any)) {
        rtx input = gen_reg_rtx(P32EXTmode);
        rtx temp = assign_stack_temp(GET_MODE(operands[0]),
                                     GET_MODE_SIZE(GET_MODE(operands[0])));
        rtx param2,param3;

        emit_move_insn(input, operands[1]);
        param3 = gen_reg_rtx(HImode);
        param2 = gen_reg_rtx(Pmode);
        emit_move_insn(param3, GEN_INT(4));
        emit_move_insn(param2, XEXP(temp,0));
        emit_library_call(XEXP(DECL_RTL(pic30_read_externals(pst_any)),0),
                          LCT_NORMAL, GET_MODE(operands[0]),
                          input, P32EXTmode,
                          param2, Pmode,
                          param3, HImode);
        emit_move_insn(operands[0], temp);
        DONE;
      } else {
        error(\"Cannot access external memory space;\n\"
              \"\tdeclare __read_external or __read_external32\");
      }
    }"
)

(define_expand "P32EXTwt_p32ext_32"
   [(set (mem:P32EXT 
           (match_operand:P32EXT 0 "pic30_ext_operand" "r,r,RQ,ST,ST"))
         (match_operand:P32EXT 1 "pic30_move_operand"  "r,R,rR,r,R"))]
   "pic30_assert(0)"
   "{
      if (pic30_write_externals(pst_32)) {
        rtx input = gen_reg_rtx(P32EXTmode);
        rtx input1 = gen_reg_rtx(GET_MODE(operands[1]));

        emit_move_insn(input, operands[0]);
        emit_move_insn(input1, operands[1]);
        emit_library_call(XEXP(DECL_RTL(pic30_write_externals(pst_32)),0),
                          LCT_NORMAL, VOIDmode,
                          input, P32EXTmode,
                          input1, GET_MODE(operands[1]));
        DONE;
      } else if (pic30_write_externals(pst_any)) {
        rtx input = gen_reg_rtx(P32EXTmode);
        rtx temp = assign_stack_temp(GET_MODE(operands[1]),
                                     GET_MODE_SIZE(GET_MODE(operands[1])));
        rtx param2,param3;
 
        emit_move_insn(input, operands[0]);
        emit_move_insn(temp, operands[1]);
        param3 = gen_reg_rtx(HImode);
        param2 = gen_reg_rtx(Pmode);
        emit_move_insn(param3, GEN_INT(4));
        emit_move_insn(param2, XEXP(temp,0));
        emit_library_call(XEXP(DECL_RTL(pic30_write_externals(pst_any)),0),
                          LCT_NORMAL, GET_MODE(operands[0]),
                          input, P32EXTmode,
                          param2, Pmode,
                          param3, HImode);
        DONE;
      } else {
        error(\"Cannot access external memory space;\n\"
              \"\tdeclare __write_external or __write_external32\");
      }
    }"
)

(define_expand "P32EXTrd_sf_32"
   [(set (match_operand:SF 0 "pic30_move_operand"       "=r,R,rR,r,R")
         (mem:SF
           (match_operand:P32EXT 1 "pic30_ext_operand" "r,r,RQ,ST,ST")))]
   "pic30_assert(0)"
   "{
      if (pic30_read_externals(pst_32)) {
        rtx input = gen_reg_rtx(P32EXTmode);

        emit_move_insn(input, operands[1]);
        emit_library_call_value(XEXP(DECL_RTL(pic30_read_externals(pst_32)),0),
                          operands[0], LCT_NORMAL, GET_MODE(operands[0]),
                          input, P32EXTmode);
        DONE;
      } else if (pic30_read_externals(pst_any)) {
        rtx input = gen_reg_rtx(P32EXTmode);
        rtx temp = assign_stack_temp(GET_MODE(operands[0]),
                                     GET_MODE_SIZE(GET_MODE(operands[0])));
        rtx param2,param3;

        emit_move_insn(input, operands[1]);
        param3 = gen_reg_rtx(HImode);
        param2 = gen_reg_rtx(Pmode);
        emit_move_insn(param3, GEN_INT(4));
        emit_move_insn(param2, XEXP(temp,0));
        emit_library_call(XEXP(DECL_RTL(pic30_read_externals(pst_any)),0),
                          LCT_NORMAL, GET_MODE(operands[0]),
                          input, P32EXTmode,
                          param2, Pmode,
                          param3, HImode);
        emit_move_insn(operands[0], temp);
        DONE;
      } else {
        error(\"Cannot access external memory space;\n\"
              \"\tdeclare __read_external or __read_external32\");
      }
    }"
)

(define_expand "P32EXTwt_sf_32"
   [(set (mem:SF (match_operand:P32EXT 0 "pic30_ext_operand" "r,r,RQ,ST,ST"))
         (match_operand:SF 1 "pic30_move_operand"            "r,R,rR,r,R"))]
   "pic30_assert(0)"
   "{
      if (pic30_write_externals(pst_32)) {
        rtx input = gen_reg_rtx(P32EXTmode);
        rtx input1 = gen_reg_rtx(GET_MODE(operands[1]));

        emit_move_insn(input, operands[0]);
        emit_move_insn(input1, operands[1]);
        emit_library_call(XEXP(DECL_RTL(pic30_write_externals(pst_32)),0),
                          LCT_NORMAL, VOIDmode,
                          input, P32EXTmode,
                          input1, GET_MODE(operands[1]));
        DONE;
      } else if (pic30_write_externals(pst_any)) {
        rtx input = gen_reg_rtx(P32EXTmode);
        rtx temp = assign_stack_temp(GET_MODE(operands[1]),
                                     GET_MODE_SIZE(GET_MODE(operands[1])));
        rtx param2,param3;
 
        emit_move_insn(input, operands[0]);
        emit_move_insn(temp, operands[1]);
        param3 = gen_reg_rtx(HImode);
        param2 = gen_reg_rtx(Pmode);
        emit_move_insn(param3, GEN_INT(4));
        emit_move_insn(param2, XEXP(temp,0));
        emit_library_call(XEXP(DECL_RTL(pic30_write_externals(pst_any)),0),
                          LCT_NORMAL, GET_MODE(operands[0]),
                          input, P32EXTmode,
                          param2, Pmode,
                          param3, HImode);
        DONE;
      } else {
        error(\"Cannot access external memory space;\n\"
              \"\tdeclare __write_external or __write_external32\");
      }
    }"
)

(define_expand "P32EXTrd_di_32"
   [(set (match_operand:DI 0 "pic30_move_operand"       "=r,R,rR,r,R")
         (mem:DI
           (match_operand:P32EXT 1 "pic30_ext_operand" "r,r,RQ,ST,ST")))]
   "pic30_assert(0)"
   "{
      if (pic30_read_externals(pst_64)) {
        rtx input = gen_reg_rtx(P32EXTmode);

        emit_move_insn(input, operands[1]);
        emit_library_call_value(XEXP(DECL_RTL(pic30_read_externals(pst_64)),0),
                          operands[0], LCT_NORMAL, GET_MODE(operands[0]),
                          input, P32EXTmode);
        DONE;
      } else if (pic30_read_externals(pst_any)) {
        rtx input = gen_reg_rtx(P32EXTmode);
        rtx temp = assign_stack_temp(GET_MODE(operands[0]),
                                     GET_MODE_SIZE(GET_MODE(operands[0])));
        rtx param2,param3;

        emit_move_insn(input, operands[1]);
        param3 = gen_reg_rtx(HImode);
        param2 = gen_reg_rtx(Pmode);
        emit_move_insn(param3, GEN_INT(8));
        emit_move_insn(param2, XEXP(temp,0));
        emit_library_call(XEXP(DECL_RTL(pic30_read_externals(pst_any)),0),
                          LCT_NORMAL, GET_MODE(operands[0]),
                          input, P32EXTmode,
                          param2, Pmode,
                          param3, HImode);
        emit_move_insn(operands[0], temp);
        DONE;
      } else {
        error(\"Cannot access external memory space;\n\"
              \"\tdeclare __read_external or __read_external64\");
      }
    }"
)

(define_expand "P32EXTwt_di_32"
   [(set (mem:DI (match_operand:P32EXT 0 "pic30_ext_operand" "r,r,RQ,ST,ST"))
         (match_operand:DI 1 "pic30_move_operand"            "r,R,rR,r,R"))]
   "pic30_assert(0)"
   "{
      if (pic30_write_externals(pst_64)) {
        rtx input = gen_reg_rtx(P32EXTmode);
        rtx input1 = gen_reg_rtx(GET_MODE(operands[1]));

        emit_move_insn(input, operands[0]);
        emit_move_insn(input1, operands[1]);
        emit_library_call(XEXP(DECL_RTL(pic30_write_externals(pst_64)),0),
                          LCT_NORMAL, VOIDmode,
                          input, P32EXTmode,
                          input1, GET_MODE(operands[1]));
        DONE;
      } else if (pic30_write_externals(pst_any)) {
        rtx input = gen_reg_rtx(P32EXTmode);
        rtx temp = assign_stack_temp(GET_MODE(operands[1]),
                                     GET_MODE_SIZE(GET_MODE(operands[1])));
        rtx param2,param3;
 
        emit_move_insn(input, operands[0]);
        emit_move_insn(temp, operands[1]);
        param3 = gen_reg_rtx(HImode);
        param2 = gen_reg_rtx(Pmode);
        emit_move_insn(param3, GEN_INT(8));
        emit_move_insn(param2, XEXP(temp,0));
        emit_library_call(XEXP(DECL_RTL(pic30_write_externals(pst_any)),0),
                          LCT_NORMAL, GET_MODE(operands[0]),
                          input, P32EXTmode,
                          param2, Pmode,
                          param3, HImode);
        DONE;
      } else {
        error(\"Cannot access external memory space;\n\"
              \"\tdeclare __write_external or __write_external64\");
      }
    }"
)

(define_expand "P32EXTrd_df_32"
   [(set (match_operand:DF 0 "pic30_move_operand"       "=r,R,rR,r,R")
         (mem:DF
           (match_operand:P32EXT 1 "pic30_ext_operand" "r,r,RQ,ST,ST")))]
   "pic30_assert(0)"
   "{
      if (pic30_read_externals(pst_64)) {
        rtx input = gen_reg_rtx(P32EXTmode);

        emit_move_insn(input, operands[1]);
        emit_library_call_value(XEXP(DECL_RTL(pic30_read_externals(pst_64)),0),
                          operands[0], LCT_NORMAL, GET_MODE(operands[0]),
                          input, P32EXTmode);
        DONE;
      } else if (pic30_read_externals(pst_any)) {
        rtx input = gen_reg_rtx(P32EXTmode);
        rtx temp = assign_stack_temp(GET_MODE(operands[0]),
                                     GET_MODE_SIZE(GET_MODE(operands[0])));
        rtx param2,param3;

        emit_move_insn(input, operands[1]);
        param3 = gen_reg_rtx(HImode);
        param2 = gen_reg_rtx(Pmode);
        emit_move_insn(param3, GEN_INT(4));
        emit_move_insn(param2, XEXP(temp,0));
        emit_library_call(XEXP(DECL_RTL(pic30_read_externals(pst_any)),0),
                          LCT_NORMAL, GET_MODE(operands[0]),
                          input, P32EXTmode,
                          param2, Pmode,
                          param3, HImode);
        emit_move_insn(operands[0], temp);
        DONE;
      } else {
        error(\"Cannot access external memory space;\n\"
              \"\tdeclare __read_external or __read_external64\");
      }
    }"
)

(define_expand "P32EXTwt_df_32"
   [(set (mem:DF (match_operand:P32EXT 0 "pic30_ext_operand" "r,r,RQ,ST,ST"))
         (match_operand:DF 1 "pic30_move_operand"            "r,R,rR,r,R"))]
   "pic30_assert(0)"
   "{
      if (pic30_write_externals(pst_64)) {
        rtx input = gen_reg_rtx(P32EXTmode);
        rtx input1 = gen_reg_rtx(GET_MODE(operands[1]));

        emit_move_insn(input, operands[0]);
        emit_move_insn(input1, operands[1]);
        emit_library_call(XEXP(DECL_RTL(pic30_write_externals(pst_64)),0),
                          LCT_NORMAL, VOIDmode,
                          input, P32EXTmode,
                          input1, GET_MODE(operands[1]));
        DONE;
      } else if (pic30_write_externals(pst_any)) {
        rtx input = gen_reg_rtx(P32EXTmode);
        rtx temp = assign_stack_temp(GET_MODE(operands[1]),
                                     GET_MODE_SIZE(GET_MODE(operands[1])));
        rtx param2,param3;
 
        emit_move_insn(input, operands[0]);
        emit_move_insn(temp, operands[1]);
        param3 = gen_reg_rtx(HImode);
        param2 = gen_reg_rtx(Pmode);
        emit_move_insn(param3, GEN_INT(8));
        emit_move_insn(param2, XEXP(temp,0));
        emit_library_call(XEXP(DECL_RTL(pic30_write_externals(pst_any)),0),
                          LCT_NORMAL, GET_MODE(operands[0]),
                          input, P32EXTmode,
                          param2, Pmode,
                          param3, HImode);
        DONE;
      } else {
        error(\"Cannot access external memory space;\n\"
              \"\tdeclare __write_external or __write_external64\");
      }
    }"
)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; 32-bit integer moves
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Invalid move

(define_insn "*movsi_invalid_1_32"
  [(set (match_operand:SI 0 "pic30_register_operand"  "=r")
        (match_operand:SI 1 "pic30_code_operand"       "g"))]
  "(pic30_isa32_target())"
  "mov.sl #%1,%0\;mov.l [%0],%0"
)

(define_insn "*movsi_invalid_2_32"
  [(set (match_operand:SI 0 "pic30_code_operand"     "=g")
        (match_operand:SI 1 "pic30_register_operand"  "r"))]
  "(pic30_isa32_target())"
  "*
   {
     error(\"invalid address space for operand\");
     return \"cannot generate instructions\";
   }
  "
)

;; float reinterpret conversions

(define_insn "movsisf_gen_32"
  [(set (match_operand:SI   0 "pic30_floatmove_operand32"    "=r,R,<>,Q")
        (subreg:SI
          (match_operand:SF 1 "pic30_float_register_operand" " l,l,l, l") 0))]
  "(pic30_isa32_target())"
  "@
   mov.l %1,%0
   mov.l %1,%0
   mov.l %1,%0
   mov.l %1,%0"
  [
    (set_attr "cc" "unchanged")
    (set_attr "type" "def,use,use,use")
    (set_attr "op_type" "mov")
  ]
)

(define_insn "movsidf_gen_32"
  [(set (match_operand:SI   0 "pic30_floatmove_operand32"    "=r,R,<>,Q")
        (subreg:SI
          (match_operand:DF 1 "pic30_float_register_operand" " l,l,l, l") 0))]
  "(pic30_isa32_target())"
  "@
   mov.l %1,%0
   mov.l %1,%0
   mov.l %1,%0
   mov.l %1,%0"
  [
    (set_attr "cc" "unchanged")
    (set_attr "type" "def,use,use,use")
    (set_attr "op_type" "mov")
  ]
)
  
(define_insn "movsidf2_gen_32"
  [(set (match_operand:SI   0 "pic30_floatmove_operand32"    "=r,R,<>,Q")
        (subreg:SI
          (match_operand:DF 1 "pic30_float_register_operand" " l,l,l, l") 4))]
  "(pic30_isa32_target())"
  "@
   mov.l %d1,%0
   mov.l %d1,%0
   mov.l %d1,%0
   mov.l %d1,%0"
  [
    (set_attr "cc" "unchanged")
    (set_attr "type" "def,use,use,use")
    (set_attr "op_type" "mov")
  ]
)
  
(define_insn "movsfsi_gen_32"
  [(set (match_operand:SF   0 "pic30_float_register_operand" "=l,l,l, l")
        (subreg:SF
          (match_operand:SI 1 "pic30_floatmove_operand32"    " r,R,<>,Q") 0))]
  "(pic30_isa32_target())"
  "@
   mov.l %1,%0
   mov.l %1,%0
   mov.l %1,%0
   mov.l %1,%0"
  [
    (set_attr "cc" "unchanged")
    (set_attr "type" "def,defuse,defuse,defuse")
    (set_attr "op_type" "mov")
  ]
)

(define_insn "movsfdi_gen_32"
  [(set (match_operand:SF   0 "pic30_float_register_operand" "=l,l,l, l")
        (subreg:SF
          (match_operand:DI 1 "pic30_floatmove_operand32"    " r,R,<>,Q") 0))]
  "(pic30_isa32_target())"
  "@
   mov.l %1,%0
   mov.l %1,%0
   mov.l %1,%0
   mov.l %1,%0"
  [
    (set_attr "cc" "unchanged")
    (set_attr "type" "def,defuse,defuse,defuse")
    (set_attr "op_type" "mov")
  ]
)

; going through an intermediate di reg, from wierd casting
(define_insn "movsfsisf_gen_32"
  [(set (subreg:SF
          (match_operand:SI 0 "pic30_register_operand" "=r") 0)
        (match_operand:SF   1 "pic30_float_register_operand" " l"))]
  "(pic30_isa32_target())"
  "mov.l %1,%0"
  [
    (set_attr "cc" "unchanged")
    (set_attr "type" "def")
    (set_attr "op_type" "mov")
  ]
)

; going through an intermediate di reg, from wierd casting
(define_insn "movsfdisf_gen_32"
  [(set (subreg:SF
          (match_operand:DI 0 "pic30_register_operand" "=r") 0)
        (match_operand:SF   1 "pic30_float_register_operand" " l"))]
  "(pic30_isa32_target())"
  "mov.l %1,%0"
  [
    (set_attr "cc" "unchanged")
    (set_attr "type" "def")
    (set_attr "op_type" "mov")
  ]
)

;; more normal moves

(define_insn "movsi_const0_32"
  [(set (match_operand:SI 0 "pic30_move_operand32" "=r,R,<>,U")
        (const_int 0))]
  "(pic30_isa32_target())"
  "@
   movs.l #0,%0
   movs.l #0,%0
   movs.l #0,%0
   clr.l %0"
  [
   (set_attr "cc" "change0")
   (set_attr "type" "def,use,use,etc")
   (set_attr "op_type" "movlit")
  ]
)

(define_insn "movsi_const1_32"
  [(set (match_operand:SI 0 "pic30_move_operand32" "=r,R,<>,U")
	(const_int -1))]
  "(pic30_isa32_target())"
  "@
   movs.l #0xFFFF,%0
   movs.l #0xFFFF,%0
   movs.l #0xFFFF,%0
   setm.l %0"
  [
   (set_attr "cc" "change0")
   (set_attr "type" "def,use,use,etc")
   (set_attr "op_type" "movlit")
  ]
)

(define_insn "movaccsi_32"
  [(set (match_operand:SI 0 "pic30_register_operand"    "=r")
        (match_operand:SI 1 "pic30_accumulator_operand" " w"))]
  "(pic30_isa32_target()) && pic30_integer_mac_support"
  "*  
   error(\"Instruction not yet ported to ISA32\n\");
  "
  [
    (set_attr "op_type" "mov")
  ]
)

(define_insn "movsiacc_32"
  [(set (match_operand:SI 0 "pic30_accumulator_operand" "=w")
        (match_operand:SI 1 "pic30_register_operand"    " r"))]
  "(pic30_isa32_target() && pic30_integer_mac_support)"
  "*  
   error(\"Instruction not yet ported to ISA32\n\");
  "
  [
    (set_attr "op_type" "mov")
  ]
)

(define_insn "movsi_address_32"
  [(set (match_operand:SI 0 "pic30_register_operand"         "=r,r")
        (match_operand:SI 1 "pic30_reg_or_symbolic_address"  " r,qs"))
  ]
  "(pic30_isa32_target())"
  "*
{ rtx sym;

  if (pic30_symbolic_address_operand(operands[1], GET_MODE(operands[1]))) {
    /* this can be converted to a register copy, so check for symref first */
    sym = pic30_program_space_operand_p(operands[1]);
    if (sym) {
      tree fndecl = GET_CODE(sym) == SYMBOL_REF ? SYMBOL_REF_DECL(sym) : 0;
      tree fndecl_attrib;
      const char *access=0;
      int slot = 0;

      if (fndecl) {
        if ((fndecl_attrib =
              pic30_lookup_attribute(IDENTIFIER_POINTER(pic30_identBoot[0]),
                               DECL_ATTRIBUTES(fndecl)))) {
          access=\"boot\";
        } else if ((fndecl_attrib = pic30_lookup_attribute(
                                    IDENTIFIER_POINTER(pic30_identSecure[0]),
                                    DECL_ATTRIBUTES(fndecl)))) {
          access=\"secure\";
        }
      }
      if (access) {
        if (TREE_VALUE(fndecl_attrib)) {
          if (TREE_CODE(TREE_VALUE(TREE_VALUE(fndecl_attrib))) == INTEGER_CST) {
            slot = TREE_INT_CST_LOW(TREE_VALUE(TREE_VALUE(fndecl_attrib)));
          } else access = 0;
        } else access = 0;
      }
      if (access) {
        static char buffer[25];
        sprintf(buffer,\"mov.sl #%s(%d),%%0\", access, slot);
        return buffer;
      }
    }
    return \"mov.sl #%1,%0\";
  } else return \"mov.l %1,%0\";
}"
 [
  (set_attr "cc" "change0")
  (set_attr "type" "def")
  (set_attr "op_type" "mov,movlit")
 ]
)

(define_insn "movsi_imm_32"
  [(set (match_operand:SI 0 "pic30_register_operand" "=r")
        (match_operand:SI 1 "immediate_operand" "i"))]
  "(pic30_isa32_target())"
  "*
   {
     int imm = INTVAL(operands[1]);
     rtx x;

     /* check for some form of SYMBOL_REF */
     x = operands[1];
     while (x) {
       switch (GET_CODE(x)) {
         case CONST: 
           x = XEXP(x,0);
           break;
         case PLUS: 
           x = XEXP(x,0);
           break;
         case MINUS:
           x = XEXP(x,0);
           break;
         case SYMBOL_REF:
           return \"mov.sl #%1,%0\";
           break;
         default: 
           x = NULL_RTX;
           break;
       }
     }
     return pic30_output_mov32si_rimm(imm,0,0);
   }
  "
  [
   (set_attr "cc" "clobber")
   (set_attr "type" "def")
   (set_attr "op_type" "movlit")
  ]
)

(define_insn "mov<mode>_gen_32"
  [(set 
    (match_operand:M32BIT_ISA32 0 "pic30_move_operand32" "=r,r,&r,r,R,R,R,>,>,Q,&r,<,U,r,r,<")
    (match_operand:M32BIT_ISA32 1 "pic30_move_operand"    "r,R, >,Q,r,0,R,r,>,r, <,r,r,U,T,<"))
  ]
  "(pic30_isa32_target())"
  "@
   mov.l %1,%0
   mov.l %1,%0
   mov.l %1,%0
   mov.l %1,%0
   mov.l %1,%0
   \;nop
   mov.l %1,%0
   mov.l %1,%0
   mov.l %1,%0
   mov.l %1,%0
   mov.l %1,%0
   mov.l %1,%0
   mov.l %1,%0
   mov.l %1,%0
   mov.sl #%1,%0\;mov.l [%0],%0
   mov.l %1,%0"
  [
   (set_attr "cc" "clobber")
   (set_attr "type"
             "def,defuse,defuse,def,use,use,use,use,use,use,def,use,etc,def,def,use")
   (set_attr "op_type" "mov")
  ]
)

(define_insn "movp32ext_rimm_32"
  [(set (match_operand:P32EXT 0 "pic30_register_operand" "=r,r")
        (match_operand:P32EXT 1 "immediate_operand"       "O,i"))]
  "(pic30_isa32_target())"
  "*
   if (INTVAL(operands[1]) == 0) {
     return \"clr.l %0\";
   }
   if ((INTVAL(operands[1]) < 32768) && (INTVAL(operands[1]) >= -32768)) {
     return \"movs.l #%z1,%0\";
   } else {
     return \"mov.l #%1,%0\";
   }
  "
  [
   (set_attr "cc" "clobber")
   (set_attr "type" "def")
   (set_attr "op_type" "movlit")
  ]
)

;;;; can we take two instructions to write if volatile?
;;;;    volatile only means consistent at the sequence point... so yes.

(define_insn "movsi_constnsfr_32"
  [(set (match_operand:SI 0 "pic30_near_operand" "=U")
        (match_operand:SI 1 "immediate_operand"   "i"))
	(clobber (match_scratch:SI 2             "=r"))]
  "(pic30_isa32_target())"
  "*
   {
     int imm = INTVAL(operands[1]);
     int volatil = pic30_volatile_operand(operands[0], GET_MODE(operands[0]));

     if (imm == 0xFFFFFFFF) {
       return \"setm.l %0\";
     } else if ((imm == 0x000000FF) && (!volatil)) {
       return \"clr.l %0\;setm.b %0\";
     } else if (imm == 0x0000FFFF) {
       return \"clr.w %0+2\;setm.w %0\";
     } else if ((imm == 0x00FF0000) && (!volatil)) {
       return \"clr.l %0\;setm.b %0+2\";
     } else if (imm == 0xFFFF0000) {
       return \"setm %0+2\;clr %0\";
     } else if (imm == 0x00000000) {
       return \"clr.l %0\";
     } else {
       return \"mov.l #%1,%2\;mov.l %2,%0\";
     }
   }
  "
  [
   (set_attr "cc" "clobber")
   (set_attr "type" "etc")
   (set_attr "op_type" "mov")
  ]
)

(define_expand "movsi_32"
  [(set (match_operand:SI 0 "pic30_general_operand" "")
        (match_operand:SI 1 "pic30_general_operand" ""))]
  "(pic30_isa32_target())"
  "
  {
     rtx op;

     /* sometimes get here from emit_move_insn even though the rhs is
      * an add or something.  we need to handle it now that there are
      * potential clobbers of the CC
      *
      * pic30_emit_move_sequence may update operands */
     if (pic30_emit_move_sequence(operands, SImode)) DONE;

     op = operands[1];

     while (GET_CODE(op) == CONST) op = XEXP(op,0);
     switch (GET_CODE(op)) {
       default: gcc_assert(0);
       case PLUS: {
         rtx op1 = XEXP(op,0);
         rtx op2 = XEXP(op,1);
         if (GET_CODE(op1) != SYMBOL_REF) {
           if (!pic30_JMmath_operand(op1,HImode)) {
             op1 = force_reg(SImode,op1);
           }
           if (!pic30_JMmath_operand(op2,HImode)) {
             op2 = force_reg(SImode,op2);
           }
           emit(
             gen_addsi3_32(operands[0], op1, op2)
           );
           DONE;
         }
         /* handle SYMBOL_REF below */
         /* FALLSTHROUGH */
       }
       case CONST_INT:
         if (GET_CODE(op) == CONST_INT) {
           if (GET_CODE(operands[0]) == REG) {
             emit(
               gen_movsi_imm_32(operands[0],operands[1])
             );
             DONE;
           } else {
             if (can_create_pseudo_p()) {
               rtx r = force_reg(GET_MODE(operands[0]), op);
               emit(
                 gen_movsi_gen_32(operands[0],r)
               );
               DONE;
             }
           }
           break;
         }
         /* FALLSTHROUGH to SYMBOL_REF */
       case SUBREG:
       case REG:
       case LABEL_REF:
       case SYMBOL_REF:
       case MEM:
         if (pic30_reload_in_progress() ||
             (pic30_move_operand(operands[0],SImode) &&
              pic30_move_operand(operands[1],SImode))) {
           emit(
             gen_movsi_gen_32(operands[0],operands[1])
           );
           DONE;
         }
         break;
     }
  }"
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; 64-bit integer moves
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;; Invalid move

(define_insn "*movdi_invalid_1_32"
  [(set (match_operand:DI 0 "pic30_register_operand"  "=r")
        (match_operand:DI 1 "pic30_code_operand" "g"))]
  "(pic30_isa32_target())"
  "mov.sl #%1+4,%0\;mov.l [%0--],%d0\;mov.l [%0],%0"
)

(define_insn "*movdi_invalid_2_32"
  [(set (match_operand:DI 0 "pic30_code_operand" "=g")
        (match_operand:DI 1 "pic30_register_operand"    "r"))]
  "(pic30_isa32_target())"
  "*
   error(\"invalid address space for operand\");
   return \"cannot generate instructions\";
  "
)

(define_insn "movdi_rimm_32"
  [(set (match_operand:DI 0 "pic30_register_operand" "=r,r")
        (match_operand:DI 1 "immediate_operand" "O,i"))]
  "(pic30_isa32_target())"
  "*
   {
     REAL_VALUE_TYPE r;
     long l[4] = { 0 };

     switch (which_alternative) {
       case 0:
         return \"mulss.d %0,#0,%0\";
       default:
         if (GET_CODE(operands[1]) == CONST_DOUBLE) {
           r = *CONST_DOUBLE_REAL_VALUE(operands[1]);
           switch (GET_MODE(operands[1])) {
             case E_VOIDmode:
               /*
               ** Integer
               */
               if (sizeof(HOST_WIDE_INT) == 4) {
                 l[0] = CONST_DOUBLE_LOW(operands[1]);
                 l[1] = CONST_DOUBLE_HIGH(operands[1]);
               } else if (sizeof(HOST_WIDE_INT) == 8) {
                 l[0] = CONST_DOUBLE_LOW(operands[1]) & 0xFFFFFFFF;
                 l[1] = CONST_DOUBLE_LOW(operands[1]) >> 32;
               } else {
                 gcc_unreachable();
               }

               break;
             default:
               REAL_VALUE_TO_TARGET_LONG_DOUBLE(r, l);
               break;
           }
         } else {
           if (sizeof(HOST_WIDE_INT) == 4) {
             l[0] = INTVAL(operands[1]);
             l[1] = l[0] < 0 ? -1 : 0;
           } else if (sizeof(HOST_WIDE_INT) == 8) {
             l[0] = CONST_DOUBLE_LOW(operands[1]) & 0xFFFFFFFF;
             l[1] = CONST_DOUBLE_LOW(operands[1]) >> 32;
           } else {
             gcc_unreachable();
           }
         }
         (void) pic30_output_mov32si_rimm(l[0],0,0);
         return pic30_output_mov32si_rimm(l[1],0,1);
     }
   }"
  [
   (set_attr "cc" "clobber")
   (set_attr "type" "def")
   (set_attr "op_type" "movlit")
  ]
)

;
; simplify the options and make reload generate the complex ones we used to
;   support.  This should result in better code generation
;
(define_insn "movdidf_gen_32"
  [(set (match_operand:DI   0 "pic30_mode2_operand"    "=r,R,<>")
        (subreg:DI
          (match_operand:DF 1 "pic30_float_register_operand" " l,l,l") 0))]
  "(pic30_isa32_target())"
  "@
   mov.l %1,%0\;mov.l %d1,%d0
   mov.l %1,%I0\;mov.l %d1,%D0
   mov.l %1,%0\;mov.l %d1,%0"
  [
    (set_attr "cc" "unchanged")
    (set_attr "type" "def,use,use")
    (set_attr "op_type" "mov")
  ]
)

(define_insn "movdfdi_gen_32"
  [(set (match_operand:DF   0 "pic30_float_register_operand" "=l,l,l")
        (subreg:DF
          (match_operand:DI 1 "pic30_register_operand" " r,R,<>") 0))]
  "(pic30_isa32_target())"
  "@
   mov.l %1,%0\;mov.l %d1,%d0
   mov.l %I1,%0\;mov.l %D1,%d0
   mov.l %1,%0\;mov.l %1,%d0"
  [
    (set_attr "cc" "unchanged")
    (set_attr "type" "def,defuse,defuse")
    (set_attr "op_type" "mov")
  ]
)

; going through an intermediate di reg, from wierd casting
(define_insn "movdfdidf_gen_32"
  [(set (subreg:DF
          (match_operand:DI 0 "pic30_register_operand" "=r") 0)
        (match_operand:DF   1 "pic30_float_register_operand" " l"))]
  "(pic30_isa32_target())"
  "mov.l %1,%0"
  [
    (set_attr "cc" "unchanged")
    (set_attr "type" "def")
    (set_attr "op_type" "mov")
  ]
)

(define_insn "movdi_gen_32"
  [(set (match_operand:DI 0 "pic30_move_operand32"
					"=r,r,&r,R,>,&r,<,U,r,r,r,Q")
        (match_operand:DI 1 "pic30_move_operand" 
					 "r,R, >,r,r, <,r,r,T,U,Q,r"))]
  "(pic30_isa32_target())"
  "*
{
  int idSrc, idDst;
  char temp[64];
  char save[64];
  static char szInsn[64];

  szInsn[0] = 0;
  temp[0] = 0;
  save[0] = 0;

  switch (which_alternative) {
    case 0: /* r = r */
      idDst = REGNO(operands[0]);
      idSrc = REGNO(operands[1]);
      if (idDst <= idSrc) {
        return \"mov.l %1,%0\;mov.l %d1,%d0\";
      } else {
        return \"mov.l %d1,%d0\;mov.l %1,%0\";
      }
    case 1: /* r = R */
      idDst = REGNO(operands[0]);
      idSrc = REGNO(XEXP(operands[1],0));
      if (pic30_pp_modify_valid(0) == 0) {
        if ((idDst > idSrc) || ((idDst+2) <= idSrc)) {
          /*
           ** source & dest don't overlap
           */
          return \"mov.l %I1,%0\;\" 
                 \"mov.l %D1,%d0\";
        }
        if ((idDst+1) > idSrc) {
          /*
           ** [wn] -> wn+1:wn
           */
          return \"mov.l %P1,%d0\;mov.l %p1,%0\";
        } else {
          /*
           ** [wn] -> wn-2:wn-1:wn:wn+1
           */
          return \"mov.l %I1,%0\;mov.l %1,%d0\";
        } 
      } else { 
        if ((idDst > idSrc) || ((idDst + 3) <= idSrc)) {
          /*  don't significantly overlap */ 
          return \"mov.l %1,%0\;mov.l %R1,%d0\";
        }
        /* idDst <= idSrc < idDst+3 */
        switch (idDst + 4 - idSrc) {
          case 4:  /* idSrc == idDst+0 */
          case 3:  /* idSrc == idDst+1 */
            return \"mov.l %R1,%d0\;\"
                   \"mov.l %1,%0\";
          case 2:  /* idSrc == idDst+2 */
            return \"mov.l %1,%0\;mov.l %R1,%d0\";
          default: abort();
        }
      }
    case 2: /* r = > */
      if (pic30_pre_modify(operands[1]))
        return \"add.l %r1,#8,%r1\;mov.l [%r1++],%0\;mov.l [%r1--],%d0\";
      else
        return \"mov.l %1,%0\;mov.l %1,%d0\";
    case 3: /* R = r */
      return \"mov.l %1,%I0\;mov.l %d1,%D0\";
    case 4: /* > = r */
      if (pic30_pre_modify(operands[0])) {
        return \"add.l %r0,#8,%r0\;mov.l %1,%I0\;mov.l %d1,%D0\";
      } else {
        return \"mov.l %1,%0\;mov.l %d1,%0\";
      }
    case 5: /* r = < */
      if (pic30_pre_modify(operands[1])) {
        return \"sub.l %r1,#8,%r1\;mov.l [%r1++],%0\;mov.l [%r1--],%d0\";
      } else {
        return \"mov.l [%r1++],%0\;mov.l [%r1--],%d0\;sub.l %r1,#8,%r1\";
      }
    case 6: /* < = r */
      return \"mov.l %d1,%0\;mov.l %1,%0\";
    case 7: /* U = r */
      return \"mov.l %1,%0\;\"
             \"mov.l %d1,%R0\";
    case 8: /* r = T */
      return \"mov.sl #%1,%0\;\"
             \"mov.l [++%0],%d0\;\"
             \"mov.l [--%0],%0\";
    case 9: /* r = U */
      return \"mov.l %1,%0\;\"
             \"mov.l %R1,%d0\";
    case 10: /* r = Q */
      idDst = REGNO(operands[0]);
      idSrc = REGNO(XEXP(XEXP(operands[1],0),0));
      strcpy(temp, \"mov.l %1,%0\;\");
      if (idDst != idSrc)
        strcat(szInsn, temp);
      else
        strcat(save, temp);
      idDst++;
      strcpy(temp, \"mov.l %R1,%d0\;\");
      if (idDst != idSrc)
        strcat(szInsn, temp);
      else
        strcat(save, temp);
      if (save[0]) {
        save[strlen(save)-2] = 0;
        strcat(szInsn, save);
      }
      return szInsn;
    case 11: /* Q = r */
      return \"mov.l %1,%0\;mov.l %d1,%R0\";

    default: gcc_assert(0);
  }
}"
  [
    (set_attr "cc" "clobber")
    (set_attr "type"
             "def,defuse,defuse,use,use,def,use,etc,def,def,defuse,use")
    (set_attr "op_type" "mov")
  ]
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Single Float (32 bit) moves
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;; Invalid move

(define_insn "*movsf_invalid_1_32"
  [(set (match_operand:SF 0 "pic30_register_operand"  "=r")
        (match_operand:SF 1 "pic30_code_operand" "g"))]
  "(pic30_isa32_target())"
  "mov.sl #%1,%0\;mov.l [%0],%0"
)

(define_insn "*movsf_invalid_2_32"
  [(set (match_operand:SF 0 "pic30_code_operand" "=g")
        (match_operand:SF 1 "pic30_register_operand"    "r"))]
  "(pic30_isa32_target())"
  "*
   {
     error(\"invalid address space for operand\");
     return \"cannot generate instructions\";
   }"
)

(define_insn "movsf_rimm_32"
  [(set (match_operand:SF 0 "pic30_float_register_operand" "=l,l")
        (match_operand:SF 1 "immediate_operand"            " H,i"))
   (clobber 
        (match_scratch:SI 2                                "=X,r"))
  ]
  "(pic30_isa32_target())"
  "*
   {
     char *buffer;
     REAL_VALUE_TYPE r;
     long l = 0;

     if (which_alternative == 0) {
       return \"movc.s %H1,%0\";
     } 
     r = *CONST_DOUBLE_REAL_VALUE(operands[1]);
     REAL_VALUE_TO_TARGET_SINGLE(r, l);
     buffer=pic30_output_mov32si_rimm(l,2,0);
     strcat(buffer,\"\;mov.l %2,%0\");
     return buffer;
  }
  "
  [
    (set_attr "cc" "clobber")
    (set_attr "type" "def")
    (set_attr "op_type" "movlit")
  ]
)

(define_insn "movsf_reload_rimm_32"
  [(set (match_operand:SF 0 "pic30_float_register_operand" "=l")
        (match_operand:SF 1 "immediate_operand"            " i"))
   (clobber (match_operand:SI 2 "pic30_register_operand"  "=&r"))
  ]
  "(pic30_isa32_target())"
  "*
   {
     char *buffer;
     REAL_VALUE_TYPE r;
     long l = 0;

     r = *CONST_DOUBLE_REAL_VALUE(operands[1]);
     REAL_VALUE_TO_TARGET_SINGLE(r, l);
     buffer=pic30_output_mov32si_rimm(l,2,0);
     strcat(buffer,\"\;mov.l %2,%0\");
     return buffer;
   }
  "
  [
    (set_attr "cc" "clobber")
    (set_attr "type" "def")
    (set_attr "op_type" "movlit")
  ]
)

(define_insn "movsfsi_constnmem_helper_32"
  [(set (match_operand:SI 0 "pic30_register_operand" "=r")
        (subreg:SI
          (match_operand:SF 1 "immediate_operand"    " i") 0))
  ]
  "(pic30_isa32_target())"
  "*
   {
     REAL_VALUE_TYPE r;
     long l = 0;

     r = *CONST_DOUBLE_REAL_VALUE(operands[1]);
     REAL_VALUE_TO_TARGET_SINGLE(r, l);
     return pic30_output_mov32si_rimm(l,0,0);
   }"
  [
    (set_attr "cc" "clobber")
    (set_attr "type" "etc")
    (set_attr "op_type" "mov")
  ]
)

(define_insn "movsisf_helper_32"
  [(set (match_operand:SF   0 "pic30_float_mem_operand" "=URQUT")
        (subreg:SF
          (match_operand:SI 1 "pic30_register_operand"  "     r") 0))
  ]
  "(pic30_isa32_target())"
  "mov.l %1,%0"
  [
    (set_attr "cc" "clobber")
    (set_attr "type" "etc")
    (set_attr "op_type" "mov")
  ]
)

(define_expand "movsf_constnmem_32"
  [(set (match_operand:SF 0 "pic30_float_mem_operand" "=U,RQUT")
        (match_operand:SF 1 "immediate_operand"        "G,   i"))]
  "(pic30_isa32_target())"
  "{
     rtx reg = gen_reg_rtx(SImode);

     emit(
       gen_movsfsi_constnmem_helper_32(reg, operands[1])
     );
     emit(
       gen_movsisf_helper_32(operands[0],reg)
     );
     DONE;
   }"
)

(define_insn "movsf_gen_32"
  [(set
    (match_operand:SF 0 "pic30_floatmove_operand32" "=l,l,&l,R,R,R,>,>,&l,<,l,Q")
    (match_operand:SF 1 "pic30_floatmove_operand32"  "l,R, >,l,0,R,l,>, <,l,Q,l"))
  ]
  "(pic30_isa32_target())"
  "@
   mov.s %1,%0
   mov.l %1,%0
   mov.l %1,%0
   mov.l %1,%0
   \;nop 
   mov.l %1,%0
   mov.l %1,%0
   mov.l %1,%0
   mov.l %1,%0
   mov.l %1,%0
   mov.l %1,%0
   mov.l %1,%0"
  [
   (set_attr "cc" "clobber")
   (set_attr "type"
             "def,defuse,defuse,use,use,use,use,use,def,use,defuse,use")
   (set_attr "op_type" "mov")
  ]
)

(define_expand "movsf_32"
  [(set (match_operand:SF 0 "pic30_general_operand" "")
        (match_operand:SF 1 "pic30_general_operand" ""))]
  "(pic30_isa32_target())"
  "
   {
     if (pic30_emit_move_sequence(operands, SFmode)) DONE;
   }"
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Double float (64 bit) moves
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;; Invalid move

(define_insn "*movdf_invalid_1_32"
  [(set (match_operand:DF 0 "pic30_register_operand"  "=r")
        (match_operand:DF 1 "pic30_code_operand" "g"))]
  "(pic30_isa32_target())"
  "mov.sl #%1+4,%0\;mov.l [%0--],%d0\;mov.l [%0],%0"
)

(define_insn "*movdf_invalid_2_32"
  [(set (match_operand:DF 0 "pic30_code_operand" "=g")
        (match_operand:DF 1 "pic30_register_operand"    "r"))]
  "(pic30_isa32_target())"
  "*
   {
        error(\"invalid address space for operand\");
        return \"cannot generate instructions\";
   }"
)

(define_insn "movdf_rimm_32"
  [(set (match_operand:DF 0 "pic30_float_register_operand" "=l,l")
        (match_operand:DF 1 "immediate_operand"            " H,i"))
   (clobber 
        (match_scratch:DI 2                                "=X,r"))
  ]
  "(pic30_isa32_target())"
  "*
   {
      REAL_VALUE_TYPE r;
      long l[4] = { 0 };
      char *buffer;

      switch (which_alternative) {
        case 0:
          return \"movc.d %H1,%0\";
        default:
          r = *CONST_DOUBLE_REAL_VALUE(operands[1]);
          REAL_VALUE_TO_TARGET_DOUBLE(r, l);
          buffer = pic30_output_mov32si_rimm(l[0],2,0);
          buffer = pic30_output_mov32si_rimm(l[1],2,1);
          strcat(buffer, \"\;mov.l %2,%0\;mov.l %d2,%d0\");
          return buffer;
      }
   }"
  [
   (set_attr "cc" "clobber")
   (set_attr "type" "def")
   (set_attr "op_type" "mov")
  ]
)

(define_insn "movdf_reload_rimm_32"
  [(set (match_operand:DF 0 "pic30_float_register_operand" "=l")
        (match_operand:DF 1 "immediate_operand"            " i"))
   (clobber (match_operand:DI 2 "pic30_register_operand"   "=&r"))
  ]
  "(pic30_isa32_target())"
  "*
   {
      REAL_VALUE_TYPE r;
      long l[4] = { 0 };
      char *buffer;

      r = *CONST_DOUBLE_REAL_VALUE(operands[1]);
      REAL_VALUE_TO_TARGET_DOUBLE(r, l);
      buffer = pic30_output_mov32si_rimm(l[0],2,0);
      buffer = pic30_output_mov32si_rimm(l[1],2,1);
      strcat(buffer, \"\;mov.l %2,%0\;mov.l %d2,%d0\");
      return buffer;
   }"
  [
   (set_attr "cc" "clobber")
   (set_attr "type" "def")
   (set_attr "op_type" "mov")
  ]
)

(define_insn "movdfdi_constnmem_helper_32"
  [(set (match_operand:DI 0 "pic30_register_operand" "=r")
        (subreg:DI
          (match_operand:DF 1 "immediate_operand"    " i") 0))
  ]
  "(pic30_isa32_target())"
  "*
   {
      REAL_VALUE_TYPE r;
      long l[4] = { 0 };
      char *buffer;

      r = *CONST_DOUBLE_REAL_VALUE(operands[1]);
      REAL_VALUE_TO_TARGET_DOUBLE(r, l);
      buffer = pic30_output_mov32si_rimm(l[0],0,0);
      buffer = pic30_output_mov32si_rimm(l[1],0,1);
      return buffer;
   }"
  [
    (set_attr "cc" "clobber")
    (set_attr "type" "etc")
    (set_attr "op_type" "mov")
  ]
)

(define_insn "movdidf_helper_32"
  [(set (match_operand:DF   0 "pic30_float_mem_operand" "=R,Q,UT")
        (subreg:DF
          (match_operand:DI 1 "pic30_register_operand"  " r,r, r") 0))
  ]
  "(pic30_isa32_target())"
  "@
   mov.l %1,%I0\;mov.l %d1,%D0
   mov.l %1,%0\;mov.l %d1,%R0
   mov.l %1,%0\;mov.l %d1,%0+4"
  [
    (set_attr "cc" "clobber")
    (set_attr "type" "etc")
    (set_attr "op_type" "mov")
  ]
)

(define_expand "movdf_constnmem_32"
  [(set (match_operand:DF 0 "pic30_float_mem_operand" "=U,RQUT")
        (match_operand:DF 1 "immediate_operand"        "G,   i"))]
  "(pic30_isa32_target())"
  "{
     rtx reg = gen_reg_rtx(DImode);

     emit(
       gen_movdfdi_constnmem_helper_32(reg, operands[1])
     );
     emit(
       gen_movdidf_helper_32(operands[0],reg)
     );
     DONE;
   }"
)

(define_insn "movdf_w_gen_32"
  [(set (match_operand:DF 0 "pic30_float_mem_operand"       "=R,Q,UT")
        (match_operand:DF 1 "pic30_float_register_operand"  " l,l, l"))
   (clobber 
     (match_scratch:SI    2                                 "=X,X,r"))
  ]
  "(pic30_isa32_target())"
  "@
   mov.l %1,%I0\;mov.l %d1,%D0
   mov.l %1,%0\;mov.l %d1,%R0
   mov.l #%0,%2\;mov.l %1,[%2++]\;mov.l %d1,[%2--]"
  [
    (set_attr "cc" "clobber")
    (set_attr "type" "etc")
    (set_attr "op_type" "mov")
  ]
)

(define_insn "movdf_r_gen_32"
  [(set (match_operand:DF 0 "pic30_float_register_operand"  "=l,l,l")
        (match_operand:DF 1 "pic30_float_mem_operand"       " R,Q,UT"))
   (clobber 
     (match_scratch:SI    2                                 "=X,X,r"))
  ]
  "(pic30_isa32_target())"
  "@
   mov.l %I1,%0\;mov.l %D1,%d0
   mov.l %1,%0\;mov.l %R1,%d0
   mov.sl #%1,%2\;mov.l [++%2],%d0\;mov.l [--%2],%0"
  [
    (set_attr "cc" "clobber")
    (set_attr "type" "etc")
    (set_attr "op_type" "mov")
  ]
)

(define_insn "movfpdf_32"
  [(set
     (match_operand:DF 0 "pic30_float_mem_operand"        "=Q, R,U,U, T,TU")
     (match_operand:DF 1 "pic30_float_reg_or_mem_operand" " l, l,l,U, l,TU"))
   (clobber
     (match_operand:SI 2 "pic30_register_operand"         "=&r,X,X,X,&r,&r"))
  ]
  "(pic30_isa32_target())"
  "*
   if (which_alternative == 0) {
     rtx offset = XEXP(XEXP(operands[0],0),1);

     if ((INTVAL(offset) >= -127) && (INTVAL(offset) < 0)) {
       return \"sub.l %r0,#-%U0,%2\;mov.l %1,[%2++]\;mov.l %d1,[%2]\";
     } else if ((INTVAL(offset) > 0) && (INTVAL(offset) <= 127)) {
       return \"add.l %r0,#%U0,%2\;mov.l %1,[%2++]\;mov.l %d1,[%2]\";
     } else  {
       return \"mov.l #%U0,%2\;mov.l %1,[%r0+%2]\;\"
              \"add.l #4,%2\;mov.l %d1,[%r0+%2]\";
     }
   }
   switch (which_alternative) {
     default: gcc_assert(0);
     case 1:   /* R = l */
       return \"mov.l %1,%I0\;mov.l %d1,%D0\";
     case 2:   /* U = l */
       return \"push.l %1\;push.l %d1\;pop.l %0+4\;pop.l %0\";
     case 3:   /* U = U */
       return \"push.l %1\;push.l %1+4\;pop.l %0+4\;pop.l %0\";
     case 4:   /* T = l */
       return \"mov.sl #%0,%2\;mov.l %1,[%2++]\;mov.l %d1,[%2]\";
     case 5:   /* TU = TU */
       return \"mov.sl #%1,%2\;mov.l [%2++],[w15++]\;mov.l [%2],[w15++]\;\"
              \"mov.sl #%0+4,%2\;mov.l [--w15],[%2--]\;mov.l [--w15],[%2]\";
   }
  "
)

(define_insn "movdffp_32"
  [(set
     (match_operand:DF  0 "pic30_float_register_operand"  "=l, l,l, l")
     (match_operand:DF  1 "pic30_float_mem_operand"       " Q, R,U, T"))
   (clobber
     (match_operand:SI  2 "pic30_register_operand"        "=&r,X,X,&r"))
  ]
  "(pic30_isa32_target())"
  "*
   if (which_alternative == 0) {
     rtx offset = XEXP(XEXP(operands[1],0),1);

     if ((INTVAL(offset) >= -127) && (INTVAL(offset) < 0)) {
       return \"sub.l %r1,#-%U1,%2\;mov.l [%2++],%0\;mov.l [%2],%d0\";
     } else if ((INTVAL(offset) > 0) && (INTVAL(offset) <= 127)) {
       return \"add.l %r1,#%U1,%2\;mov.l [%2++],%0\;mov.l [%2],%d0\";
     } else  {
       return \"mov.l #%U1,%2\;mov.l [%r1+%2],%0\;\"
              \"add.l #4,%2\;mov.l [%r1+%2],%d0\";
     }
   }
   switch (which_alternative) {
     default: gcc_assert(0);
     case 1: /* l = R */
             return \"mov.l %I1,%0\;mov.l %D1,%d0\";
     case 2: /* l = U */
             return \"push.l %1\;push.l %1+4\;pop.l %d0\;pop.l %0\";
     case 3: /* l = T */
             return \"mov.sl #%1,%2\;mov.l [%2++],%0\;mov.l [%2],%d0\";
  }"
)

;; for moves generated during reload; until we have a real mov.l f,Q insn
;(define_insn "movdffp2_32"
;  [(set (match_operand:DF 0 "pic30_register_operand" "=l")
;        (match_operand:DF 1 "memory_operand"         " Q"))
;  ]
;  "(pic30_isa32_target())"
;  "*
;   if (REGNO(XEXP(operands[1],0)) != WR8_REGNO)
;     return \"push.l w8\;mov.l #%U1,w8\;mov.l [%r1+w8],%0\;pop.l w8\";
;   else
;     return \"push.l w9\;mov.l #%U1,w9\;mov.l [%r1+w8],%0\;pop.l w9\";
;  "
;)

(define_insn "movdfdf_32"
  [(set (match_operand:DF 0 "pic30_any_register_operand" "=rl,l,l,r")
        (match_operand:DF 1 "pic30_any_register_operand" " 0, l,r,l"))]
  "(pic30_isa32_target())"
  "@
   \; nop
   mov.d %1,%0
   mov.l %1,%0\;mov.l %d1,%d0
   mov.l %1,%0\;mov.l %d1,%d0"
)

(define_insn "movdfdfmem_32"
  [(set (match_operand:DF 0 "pic30_float_reg_or_mem_operand" "=l,l,R,Q,l,l")
        (match_operand:DF 1 "pic30_float_reg_or_mem_operand" " 0,l,l,l,R,Q"))]
  "(pic30_isa32_target())"
  "@
   \; nop
   mov.d %1,%0
   mov.l %1,%I0\;mov.l %d1,%D0
   mov.l %1,%0\;mov.l %d1,%R0
   mov.l %I1,%0\;mov.l %D1,%d0
   mov.l %1,%0\;mov.l %R1,%d0"
)

(define_expand "mov<mode>_32"
  [(set (match_operand:SM64BIT 0 "pic30_general_operand" "")
        (match_operand:SM64BIT 1 "pic30_general_operand" ""))]
  "(pic30_isa32_target())"
  "
{
   if (pic30_emit_move_sequence(operands, GET_MODE(operands[0]))) DONE;
}")

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; add instructions 
;;
;; During the instruction canonicalization phase,
;; (minus x (const_int n)) is converted to (plus x (const_int -n)).
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;
;; quarter integer
;;;;;;;;;;;;;;;;;;

(define_insn "*addqi3_incdec_32"
  [(set (match_operand:QI 0 "pic30_mode2_operand" "=r<>,R,r<>,R")
        (plus:QI 
           (match_operand:QI 1 "pic30_math_operand" "r,  r,R<>,R<>")
           (match_operand:QI 2 "immediate_operand" "i,  i,i,  i")))
   (clobber (reg:CC CC_REG))
  ]
 "(pic30_isa32_target()) && (-127<=INTVAL(operands[2])) && (INTVAL(operands[2])!=0) && (INTVAL(operands[2])<=127)"
  "*
   {
     int zero_extend = (GET_CODE(operands[0]) == REG);
     if (INTVAL(operands[2]) < 0) {
       if (zero_extend) {
         return \"sub.bz %1,#%J2,%0\";
       } else {
         return \"sub.b %1,#%J2,%0\";
       }
     } else {
       if (zero_extend) {
         return \"add.bz %1,#%2,%0\";
       } else {
         return \"add.b %1,#%2,%0\";
       }
     }
   }
  "
  [
   (set_attr "cc" "math")
   (set_attr "type" "defuse,use,defuse,use")
   (set_attr "op_type" "alu")
  ]
)

;; There is a potential register class issue here, if we cannot claim WREG
;;   then this pattern will fail.  But allowing a general reg seems to have
;;;  a slight negative impact on one particular applicaton - leaving the
;;   option open for now.  CAW

(define_insn "*addqi3_incdecsfr_32"
  [(set (match_operand:QI 0 "pic30_reg_or_near_operand"   "=U,r")
        (plus:QI (match_operand:QI 1 "pic30_near_operand" "%0,U")
                 (match_operand:QI 2 "immediate_operand"   "i,i")))
   (clobber (reg:CC CC_REG))
  ]
 "(-2<=INTVAL(operands[2]))&&(INTVAL(operands[2])!=0)&&(INTVAL(operands[2])<=2)"
  "*
{
  switch (INTVAL(operands[2])) {
    case -2: switch (which_alternative) {
               case 0: return \"dec2.b %0\";
               case 1: return \"dec2.b %1,%0\";
             }
    case -1: switch (which_alternative) {
               case 0: return \"dec.b %0\";
               case 1: return \"dec.b %1,%0\";
             }
    case 1: switch (which_alternative) {
               case 0: return \"inc.b %0\";
               case 1: return \"inc.b %1,%0\";
             }
    case 2: switch (which_alternative) {
               case 0: return \"inc2.b %0\";
               case 1: return \"inc2.b %1,%0\";
             }
    default: gcc_assert(0);
             return \"nop\";
  }
}"
  [
   (set_attr "cc" "math")
   (set_attr "type" "etc,def")
   (set_attr "op_type" "alu")
  ]
)

(define_insn "addqi3_32"
  [(set (match_operand:QI 0 "pic30_mode2_operand"
              "=r<>,r<>,R,R, r<>,R,r<>,R,r<>,R,  r<>,R,r<>,R,r,r")
        (plus:QI 
           (match_operand:QI 1 "pic30_math_operand"
              "%r,  r,  r,r, r,  r,r,  r,R<>,R<>,N,  N,P,  P,0,J")
           (match_operand:QI 2 "pic30_mode1JN_operand"
              "r,  R<>,r,R<>,N,  N,P,  P,r,  r,  r,  r,r,  r,J,0")))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "@
   add.b %1,%2,%0
   add.b %1,%2,%0
   add.b %1,%2,%0
   add.b %1,%2,%0
   sub.b %1,#%J2,%0
   sub.b %1,#%J2,%0
   add.b %1,#%2,%0
   add.b %1,#%2,%0
   add.b %2,%1,%0
   add.b %2,%1,%0
   sub.b %2,#%J1,%0
   sub.b %2,#%J1,%0
   add.b %2,#%1,%0
   add.b %2,#%1,%0
   add.b #%2,%0
   add.b #%1,%0"
  [
   (set_attr "cc" "math")
   (set_attr "type" 
             "def,defuse,use,use,def,use,def,use,defuse,use,def,use,def,use,def,def")
   (set_attr "op_type" "alu")
  ]
)

; leave this match_dup, operand 0 will not require a reload (CAW)
(define_insn "*addqi3_sfr0_32"
  [(set (match_operand:QI   0 "pic30_near_operand"     "=U")
        (plus:QI 
          (match_dup 0)
          (match_operand:QI 1 "pic30_register_operand"  "r")))
   (clobber (reg:CC CC_REG))
]
  "(pic30_isa32_target())"
  "add.b %0, %1"
  [
    (set_attr "cc" "math")
    (set_attr "type" "etc")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "*addqi3_sfr1_32"
  [(set (match_operand:QI   0 "pic30_near_operand"    "=U")
        (plus:QI 
          (match_operand:QI 1 "pic30_register_operand" "r")
          (match_dup 0)))
   (clobber (reg:CC CC_REG))
]
  "(pic30_isa32_target())"
  "add.b %0, %1"
  [
    (set_attr "cc" "math")
    (set_attr "type" "etc")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "*addqi3_sfr2_32"
  [(set (match_operand:QI 0 "pic30_register_operand"            "=r ,r")
        (plus:QI (match_operand:QI 1 "pic30_near_operand"       "%U ,U")
                 (match_operand:QI 2 "pic30_register_operand"   " 0 ,r")))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "@
   add.b %1,%0,%0
   mov.bz %2,%0\;add.b %1,%0, %0"
  [
   (set_attr "cc" "math")
   (set_attr "type" "def")
   (set_attr "op_type" "alu")
  ]
)

;;;;;;;;;;;;;;;
;; half integer
;;;;;;;;;;;;;;;
(define_insn "add<mode>3_32"
  [(set (match_operand:A16BITADD 0 "pic30_mode2_operand"
              "=r<>,R,  r<>,R,r<>,r<>,R,R,  r<>,R,r<>,R,R,r<>,R,  r<>,r<>,R,r<>,R,r,r,r,  &!?r,&!?r")
        (plus:A16BITADD 
           (match_operand:A16BITADD 1 "pic30_JMmath_operand"
              "%r<>,r<>,R,  R,r,  r,  r,r,  r,  r,r,  r,r,r,  R<>,R<>,N,  N,P,  P,0,J,NPJ,JM, r")
           (match_operand:A16BITADD 2 "pic30_JMmath_operand"
              "L,   L,  L,  L,r,  R<>,r,R<>,N,  N,P,  P,r,r,  r,  r,  r,  r,r,  r,J,0,NPJ,r,  JM"))
   )
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "*
{
   switch (which_alternative) {
     case  0:
     case  1:
     case  2:
     case  3:
       switch (INTVAL(operands[2])) {
         case -2:  return \"dec2.w %1,%0\";
         case -1:  return \"dec.w %1,%0\";
         case 1:  return \"inc.w %1,%0\";
         case 2:  return \"inc2.w %1,%0\";
         default: error(\"unknown L value: addhi3\");
                  return \"bad adhhi3\";
       }
       break;
     case  4: return \"add.w %1,%2,%0\";
     case  5: return \"add.w %1,%2,%0\";
     case  6: return \"add.w %1,%2,%0\";
     case  7: return \"add.w %1,%2,%0\";
     case  8: return \"sub.w %1,#%J2,%0\";
     case  9: return \"sub.w %1,#%J2,%0\";
     case 10: return \"add.w %1,#%2,%0\";
     case 11: return \"add.w %1,#%2,%0\";
     case 12: return \"add.w %2,%1,%0\";
     case 13: return \"add.w %2,%1,%0\";
     case 14: return \"add.w %2,%1,%0\";
     case 15: return \"add.w %2,%1,%0\";
     case 16: return \"sub.w %2,#%J1,%0\";
     case 17: return \"sub.w %2,#%J1,%0\";
     case 18: return \"add.w %2,#%1,%0\";
     case 19: return \"add.w %2,#%1,%0\";
     case 20: return \"add.w #%2,%0\";
     case 21: return \"add.w #%1,%0\";
     case 22: return \"mov.w #(%1+%2),%0\";
     case 23: if (REGNO(operands[0]) != REGNO(operands[2]))
                return \"mov.w #%1,%0\;add.w %0,%2,%0\";
              else {
                if (INTVAL(operands[1]) < 0)
                  return \"sub.w #%J1,%0\";
                else 
                  return \"add.w #%1,%0\";
              }
     case 24: if (REGNO(operands[0]) != REGNO(operands[1])) 
                return \"mov.w #%2,%0\;add.w %0,%1,%0\";
              else {
                if (INTVAL(operands[2]) < 0)
                  return \"sub.w #%J2,%0\";
                else 
                  return \"add.w #%2,%0\";
              }
     default: gcc_assert(0);
              return \"bad addhi3\";
   }
}"
  [
   (set_attr "cc" "math")
   (set_attr "type" 
              "def,use,defuse,use,def,defuse,use,use,def,use,def,use,use,def,use,defuse,def,use,def,use,def,def,def,def,def")
    (set_attr "op_type" "alu")
  ]
)

; this match_can cause issues iff operand 1 is dies in this instruction and
;   we decide to use it to reload operand 0 (CAW)
(define_insn "*addhi3_sfr0_32"
  [(set (match_operand:HI   0 "pic30_reg_or_near_operand" "+U,r")
        (plus:HI 
          (match_dup 0)
          (match_operand:HI 1 "pic30_register_operand"     "r,r")))
   (clobber (reg:CC CC_REG))
]
  "(pic30_isa32_target())"
  "@
   add %0
   add %0,%1,%0"
  [
    (set_attr "cc" "math")
    (set_attr "type" "etc,def")
    (set_attr "op_type" "alu")
  ]
)

; this match_can cause issues iff operand 1 is dies in this instruction and
;   we decide to use it to reload operand 0 (CAW)
(define_insn "*addhi3_sfr1_32"
  [(set (match_operand:HI   0 "pic30_reg_or_near_operand" "+U,r")
        (plus:HI 
          (match_operand:HI 1 "pic30_register_operand"     "r,r")
          (match_dup 0)))
   (clobber (reg:CC CC_REG))
]
  "(pic30_isa32_target())"
  "@
   add %0
   add %0,%1,%0"
  [
    (set_attr "cc" "math")
    (set_attr "type" "etc,def")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "*addhi3_sfr2_32"
  [(set (match_operand:HI   0 "pic30_register_operand" "=r,r")
        (plus:HI 
          (match_operand:HI 1 "pic30_near_operand"     "%U,U")
          (match_operand:HI 2 "pic30_register_operand" " 0,r")))
   (clobber (reg:CC CC_REG))
]
  "(pic30_isa32_target())"
  "@
   add.w %1,%0,WREG
   mov.w %2,%0\;add.w %1,%0,WREG"
  [
   (set_attr "cc" "math")
   (set_attr "type" "def")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "*addhi3_incdecsfr_32"
  [(set (match_operand:HI 0 "pic30_reg_or_near_operand"   "=U,r")
        (plus:HI (match_operand:HI 1 "pic30_near_operand" "%0,U")
                 (match_operand:HI 2 "immediate_operand"  " i,i")))
   (clobber (reg:CC CC_REG))
  ]
 "(-2<=INTVAL(operands[2]))&&(INTVAL(operands[2])!=0)&&(INTVAL(operands[2])<=2)"
  "*
{
  switch (INTVAL(operands[2])) {
    case -2: switch (which_alternative) {
               case 0: return \"dec2 %0\";
               case 1: return \"dec2 %1,%0\";
               default: abort();
             }
    case -1: switch (which_alternative) {
               case 0: return \"dec %0\";
               case 1: return \"dec %1,%0\";
               default: abort();
             }
    case 1: switch (which_alternative) {
               case 0: return \"inc %0\";
               case 1: return \"inc %1,%0\";
               default: abort();
             }
    case 2: switch (which_alternative) {
               case 0: return \"inc2 %0\";
               case 1: return \"inc2 %1,%0\";
               default: abort();
             }
    default: gcc_assert(0);
  }
}"
  [
   (set_attr "cc" "math")
   (set_attr "type" "etc,def")
    (set_attr "op_type" "alu")
  ]
)

;;;;;;;;;;;;;;;;;
;; single integer
;;;;;;;;;;;;;;;;;

(define_insn "addsihi3_32"
  [(set (match_operand:SI 0 "pic30_register_operand"      "=r")
        (plus:SI 
           (match_operand:SI 1 "pic30_register_operand"    "r")
           (zero_extend:SI 
              (match_operand:HI 2 "pic30_register_operand" "r"))))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "ze.w %2,%0\;add.l %1,%0,%0"
  [
    (set_attr "cc" "math")
    (set_attr "type" "def")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "addhisi3_ze_32"
  [(set (match_operand:SI 0 "pic30_mode2_operand"         "=r,>,>,R,R")
        (plus:SI 
           (zero_extend:SI 
              (match_operand:HI 1 "pic30_register_operand" "r,r,r,r,r"))
           (match_operand:SI 2 "pic30_mode2_operand"       "r,0,>,0,R")))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "@
   add.l %1,%2,%0
   add.l %1,%2,%0
   add.l %1,%2,%0
   add.l %1,%2,%0
   add.l %1,%2,%0"
  [
    (set_attr "cc" "math")
    (set_attr "type" "def")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "addhisi3_se_32"
  [(set (match_operand:SI 0 "pic30_mode2_operand"         "=r,>,>,R,R")
        (plus:SI
           (sign_extend:SI
              (match_operand:HI 1 "pic30_register_operand" "r,r,r,r,r"))
           (match_operand:SI 2 "pic30_mode2_operand"       "r,0,>,0,R")))
   (clobber (match_scratch:SI 3                           "=&r,&r,&r,&r,&r"))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "@
   se.w %1,%3\;add.l %3,%2,%0
   se.w %1,%3\;add.l %3,%s2,%0
   se.w %1,%3\;add.l %3,%2,%0
   se.w %1,%3\;add.l %3,%2,%0
   se.w %1,%3\;add.l %3,%2,%0"
  [
    (set_attr "cc" "math")
    (set_attr "type" "def")
    (set_attr "op_type" "alu")
  ]
)


(define_insn "addsi3_32"
 [(set (match_operand:SI   0 "pic30_mode2_operand"       "=rR<>,rR<>,rR<>")
       (plus:SI  
         (match_operand:SI 1 "pic30_mode2_operand"       "%rR<>,rR<>,rR<>")
         (match_operand:SI 2 "pic30_reg_or_PN_operand"   " r   ,P   ,N")))
  (clobber (reg:CC CC_REG))
 ]
  "(pic30_isa32_target())"
  "@
   add.l %2,%1,%0
   add.l %1,#%2,%0
   sub.l %1,#%J2,%0"
  [
    (set_attr "cc" "math,math,math")
    (set_attr "type" "defuse,defuse,defuse")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "addsi3_JM_32"
  [(set (match_operand:SI 0 "pic30_register_operand"         "=r,r")
        (plus:SI (match_operand:SI 1 "pic30_register_operand" "%0,0")
                 (match_operand:SI 2 "pic30_JM_operand"        "J,M")))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "@
   add.l #%2,%0
   sub.l #%J2,%0"
  [
   (set_attr "cc" "math")
   (set_attr "type" "def")
    (set_attr "op_type" "alu")
  ]
)

;; P32EXT

(define_insn "addp32ext3_imm_32"
  [(set (match_operand:P32EXT 0 "pic30_register_operand"          "=r,r,r,r")
        (plus:P32EXT 
                 (match_operand:P32EXT 1 "pic30_register_operand" "%r,r,0,0")
                 (match_operand:P32EXT 2 "pic30_JM_operand"       " P,N,J,M")))
   (clobber (reg:CC_NZC CC_REG))
  ]
  "(pic30_isa32_target())"
  "@
   add.l %1,#%2,%0
   sub.l %1,#%J2,%0
   add.l #%2,%0
   sub.l #%J2,%0"
  [
   (set_attr "cc" "math")
   (set_attr "type" "def")
   (set_attr "op_type" "alu")
  ]
)

(define_insn "addp32ext3_32"
 [(set (match_operand:P32EXT   0 "pic30_rR_or_near_operand" "=r,r,r,&r,r,r,r,R,R,R")
       (plus:P32EXT
         (match_operand:P32EXT 1 "pic30_rR_or_near_operand" "%r,0,r, R,r,r,0,r,r,r")
         (match_operand:P32EXT 2 "pic30_rR_or_JN_operand"    "r,r,0, r,P,N,J,r,0,R")  ))
  (clobber (reg:CC CC_REG))
 ]
  "(pic30_isa32_target())"
  "*
   {
     static const char *patterns[] = {
       \"add.l %2,%1,%0\",
       \"add.l %2,%1,%0\",
       \"add.l %2,%1,%0\",
       \"add.l %2,%1,%0\",
       \"add.l %1,#%2,%0\",
       \"sub.l %1,#%J2,%0\",
       \"add.l #%2,%0\",
       \"add.l %1,%2,%0\",
       \"add.l %1,%2,%0\",
       \"add.l %1,%2,%0\"
     };

     return patterns[which_alternative];
   }"
  [
    (set_attr "cc" "math,math,math,math,math,math,math,math,math,math")
    (set_attr "type" "def,def,def,defuse,def,def,def,use,use,use")
    (set_attr "op_type" "alu")
  ]
)

;; P32EDS

;;;;;;;;;;;;;;;;;
;; double integer
;;;;;;;;;;;;;;;;;

(define_insn "adddi3_imm_32"
  [(set (match_operand:DI          0 "pic30_register_operand" "=r,r,r,r")
        (plus:DI (match_operand:DI 1 "pic30_register_operand" "%r,r,0,0")
                 (match_operand:DI 2 "pic30_JM_operand"       " P,N,J,M")))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "@
   add.l %1,#%2,%0\;addc.l %d1,#0,%d0
   sub.l %1,#%J2,%0\;subb.l %d1,#0,%d0
   add.l #%2,%0\;addc.l #0,%d0
   sub.l #%J2,%0\;subb.l #0,%d0"
  [
    (set_attr "cc" "math")
    (set_attr "type" "def")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "addhidi3_se_32"
  [(set (match_operand:DI 0 "pic30_DI_mode2_operand"      "=r,>,>,>,&r,R,R,R,&r")
        (plus:DI
           (sign_extend:DI
              (match_operand:HI 1 "pic30_register_operand" "r,r,r,r, r,r,r,r, r"))
           (match_operand:DI 2 "pic30_DI_mode2_operand"    "r,r,0,>, >,r,0,R, R")))
   (clobber (match_scratch:SI 3                   "=&r,&r,&r,&r,&r,&r,&r,&r,&r"))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "*
{
   const char *noerrata_patterns[] = {
     /* r,r,r */          \"se.w %1,%3\;add.l %3,%2,%0\;\"
                              \"asr.l %3,#31,%3\;addc.l %3,%d2,%d0\",

     /* >,r,r */          \"se.w %1,%3\;add.l %3,%2,%0\;\"
                              \"asr.l %3,#31,%3\;addc.l %3,%d2,%0\",

     /* >,r,0 */          \"se.w %1,%3\;add.l %3,%s2,%0\;\"
                              \"asr.l %3,#31,%3\;addc.l %3,%s2,%0\",

     /* >,r,> */          \"se.w %1,%3\;add.l %3,%2,%0\;\"
                              \"asr.l %3,#31,%3\;addc.l %3,%2,%0\",

     /* r,r,> */          \"se.w %1,%3\;add.l %3,%2,%0\;\"
                              \"asr.l %3,#31,%3\;addc.l %3,%2,%d0\",

     /* R,r,r */          \"se.w %1,%3\;add.l %3,%2,%I0\;\"
                              \"asr.l %3,#31,%3\;addc.l %3,%d2,%D0\",

     /* R,r,0 */          \"se.w %1,%3\;add.l %3,%2,%I0\;\"
                              \"asr.l %3,#31,%3\;addc.l %3,%2,%D0\",

     /* R,r,R */          \"se.w %1,%3\;add.l %3,%I2,%I0\;\"
                              \"asr.l %3,#31,%3\;addc.l %3,%D2,%D0\",

     /* r,r,R */          \"se.w %1,%3\;add.l %3,%I2,%0\;\"
                              \"asr.l %3,#31,%3\;addc.l %3,%D2,%d0\",
   };

   /* no post adjustments needed for the 'R' patterns */
   return noerrata_patterns[which_alternative];
}"
   [
     (set_attr "cc" "math,math,math,math,math,math,math,math,math")
     (set_attr "type" "def")
     (set_attr "op_type" "alu")
   ]
)

(define_insn "addhidi3_ze_32"
  [(set (match_operand:DI 0 "pic30_DI_mode2_operand"      "=r,>,>,>,&r,R,R,R,&r")
        (plus:DI
           (zero_extend:DI
              (match_operand:HI 1 "pic30_register_operand" "r,r,r,r, r,r,r,r, r"))
           (match_operand:DI 2 "pic30_DI_mode2_operand"    "r,r,0,>, >,r,0,R, R")))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "*
{
  const char *noerrata_patterns[] = {
     /* r,r,r */          \"add.l %1,%2,%0\;addc.l %d2,#0,%d0\",

     /* >,r,r */          \"add.l %1,%2,%0\;addc.l %d2,#0,%0\",

     /* >,r,0 */          \"add.l %1,%s2,%0\;addc.l %s2,#0,%0\",

     /* >,r,> */          \"add.l %1,%2,%0\;addc.l %2,#0,%0\",

     /* r,r,> */          \"add.l %1,%2,%0\;addc.l %2,#0,%d0\",

     /* R,r,r */          \"add.l %1,%2,%I0\;addc.l %d2,#0,%D0\",

     /* R,r,0 */          \"add.l %1,%2,%I0\;addc.l %2,#0,%D0\",

     /* R,r,R */          \"add.l %1,%I2,%I0\;addc.l %D2,#0,%D0\",

     /* r,r,R */          \"add.l %1,%I2,%0\;addc.l %D2,#0,%d0\",
  };

  /* no post adjustments needed for the 'R' patterns */
  return noerrata_patterns[which_alternative];
}"
   [
     (set_attr "cc" "math,math,math,math,math,clobber,clobber,clobber,clobber")
     (set_attr "type" "def")
     (set_attr "op_type" "alu")
   ]
)

(define_insn "addsidi3_se_32"
  [(set (match_operand:DI 0 "pic30_DI_mode2_operand"      "=r,>,>,>,&r,R,R,R,&r")
        (plus:DI
           (sign_extend:DI
              (match_operand:SI 1 "pic30_register_operand" "r,r,r,r,r,r,r,r,r"))
           (match_operand:DI 2 "pic30_DI_mode2_operand"    "r,r,0,>,>,r,0,R,R")))
   (clobber (match_scratch:SI 3                   "=&r,&r,&r,&r,&r,&r,&r,&r,&r"))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "*
{
  const char *noerrata_patterns[] = {
     /* r,r,r */          \"asr.l %1,#31,%3\;\"
                              \"add.l %1,%2,%0\;addc.l %3,%d2,%d0\",

     /* >,r,r */          \"asr.l %1,#31,%3\;\"
                              \"add.l %1,%2,%0\;addc.l %3,%d2,%0\",

     /* >,r,0 */          \"asr.l %1,#31,%3\;\"
                              \"add.l %1,%s2,%0\;addc.l %3,%s2,%0\",

     /* >,r,> */          \"asr.l %1,#31,%3\;\"
                              \"add.l %1,%2,%0\;addc.l %3,%2,%0\",

     /* r,r,> */          \"asr.l %1,#31,%3\;\"
                              \"add.l %1,%2,%0\;addc.l %3,%2,%d0\",

     /* R,r,r */          \"asr.l %1,#31,%3\;\"
                              \"add.l %1,%2,%I0\;addc.l %3,%d2,%I0\",

     /* R,r,0 */          \"asr.l %1,#31,%3\;\"
                              \"add.l %1,%2,%I0\;addc.l %3,%2,%I0\",

     /* R,r,R */          \"asr.l %1,#31,%3\;\"
                              \"add.l %1,%I2,%I0\;addc.l %3,%I2,%I0\",

     /* r,r,R */          \"asr.l %1,#31,%3\;\"
                              \"add.l %1,%I2,%0\;addc.l %3,%I2,%d0\",
  };
 
  /* no post adjustments needed for the 'R' patterns */
  return noerrata_patterns[which_alternative];
}"
   [
     (set_attr "cc" "math,math,math,math,math,clobber,clobber,clobber,clobber")
     (set_attr "type" "def")
     (set_attr "op_type" "alu")
   ]
)

(define_insn "addsidi3_ze_32"
  [(set (match_operand:DI 0 "pic30_DI_mode2_operand"      "=r,>,>,>,&r,R,R,R,&r")
        (plus:DI
           (zero_extend:DI
              (match_operand:SI 1 "pic30_register_operand" "r,r,r,r, r,r,r,r, r"))
           (match_operand:DI 2 "pic30_DI_mode2_operand"    "r,r,0,>, >,r,0,R, R")))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "*
{
   const char *noerrata_patterns[] = {
     /* r,r,r */          \"add.l %1,%2,%0\;addc.l %d2,#0,%d0\",

     /* >,r,r */          \"add.l %1,%2,%0\;addc.l %d2,#0,%0\",

     /* >,r,0 */          \"add.l %1,%s2,%0\;addc.l %s2,#0,%0\",

     /* >,r,> */          \"add.l %1,%2,%0\;addc.l %2,#0,%0\",

     /* r,r,> */          \"add.l %1,%2,%0\;addc.l %2,#0,%d0\",

     /* R,r,r */          \"add.l %1,%2,%I0\;addc.l %d2,#0,%D0\",

     /* R,r,0 */          \"add.l %1,%2,%I0\;addc.l %2,#0,%D0\",

     /* R,r,R */          \"add.l %1,%I2,%I0\;addc.l %D2,#0,%D0\",

     /* r,r,R */          \"add.l %1,%I2,%0\;addc.l %D2,#0,%d0\",
  };

  /* no post adjustments needed for the 'R' patterns */
  return noerrata_patterns[which_alternative];

}"
   [
     (set_attr "cc" "math,math,math,math,math,clobber,clobber,clobber,clobber")
     (set_attr "type" "def")
     (set_attr "op_type" "alu")
   ]
)

(define_insn "adddihi3_se_32"
  [(set (match_operand:DI 0 "pic30_DI_mode2_operand"      "=r,>,>,>,&r,R,R,R,&r")
        (plus:DI
           (match_operand:DI 1 "pic30_DI_mode2_operand"    "r,r,0,>, >,r,0,R, R")
           (sign_extend:DI
              (match_operand:HI 2 "pic30_register_operand" "r,r,r,r, r,r,r,r, r"))
        )
   )
   (clobber (match_scratch:SI 3                   "=&r,&r,&r,&r,&r,&r,&r,&r,&r"))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "*
{
   const char *noerrata_patterns[] = {
     /* r,r,r */          \"se.w %2,%3\;add.l %1,%3,%0\;\"
                              \"asr.l %3,#31,%3\;addc.l %d1,%3,%d0\",

     /* >,r,r */          \"se.w %2,%3\;add.l %1,%3,%0\;\"
                              \"asr.l %3,#31,%3\;addc.l %d1,%3,%0\",

     /* >,0,r */          \"se.w %2,%3\;add.l %s1,%3,%0\;\"
                              \"asr.l %3,#31,%3\;addc.l %s1,%3,%0\",

     /* >,>,r */          \"se.w %2,%3\;add.l %1,%3,%0\;\"
                              \"asr.l %3,#31,%3\;addc.l %1,%3,%0\",

     /* r,>,r */          \"se.w %2,%3\;add.l %1,%3,%0\;\"
                              \"asr.l %3,#31,%3\;addc.l %1,%3,%d0\",

     /* R,r,r */          \"se.w %2,%3\;add.l %1,%3,%I0\;\"
                              \"asr.l %3,#31,%3\;addc.l %d1,%3,%D0\",

     /* R,0,r */          \"se.w %2,%3\;add.l %s1,%3,%I0\;\"
                              \"asr.l %3,#31,%3\;addc.l %s1,%3,%D0\",

     /* R,R,r */          \"se.w %2,%3\;add.l %I1,%3,%I0\;\"
                              \"asr.l %3,#31,%3\;addc.l %D1,%3,%D0\",

     /* r,R,r */          \"se.w %2,%3\;add.l %I1,%3,%0\;\"
                              \"asr.l %3,#31,%3\;addc.l %D1,%3,%d0\",
  };


  /* no post adjustments needed for the 'R' patterns */
  return noerrata_patterns[which_alternative];
}"
   [
     (set_attr "cc" "math,math,math,math,math,clobber,clobber,clobber,clobber")
     (set_attr "type" "def")
     (set_attr "op_type" "alu")
   ]
)

(define_insn "adddihi3_ze_32"
  [(set (match_operand:DI 0 "pic30_DI_mode2_operand"         "=r,>,>,>,&r,R,R,R,&r")
        (plus:DI
           (match_operand:DI 1 "pic30_DI_mode2_operand"       "r,r,0,>, >,r,0,R, R")
           (zero_extend:DI
              (match_operand:HI 2 "pic30_register_operand" "r,r,r,r, r,r,r,r, r"))
        )
   )
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "*
{
   const char *noerrata_patterns[] = {
     /* r,r,r */          \"add.l %1,%2,%0\;addc.l %d1,#0,%d0\;\"

     /* >,r,r */          \"add.l %1,%2,%0\;addc.l %d1,#0,%0\;\"

     /* >,0,r */          \"add.l %s1,%2,%0\;addc.l %s1,#0,%0\;\"

     /* >,>,r */          \"add.l %2,%1,%0\;addc.l %1,#0,%0\;\"

     /* r,>,r */          \"add.l %2,%1,%0\;addc.l %1,30,%d0\;\"

     /* R,r,r */          \"add.l %1,%2,%I0\;addc.l %d1,#0,%D0\;\"

     /* R,0,r */          \"add.l %2,%1,%I0\;addc.l %1,#0,%D0\;\"

     /* R,R,r */          \"add.l %2,%I1,%I0\;addc.l %D1,#0,%D0\;\"

     /* r,R,r */          \"add.l %2,%I1,%0\;addc.l %D1,#0,%d0\;\"
  };

  /* no post adjustments needed for the 'R' patterns */
  return noerrata_patterns[which_alternative];

}"
   [
     (set_attr "cc" "math,math,math,math,math,clobber,clobber,clobber,clobber")
     (set_attr "type" "def")
     (set_attr "op_type" "alu")
   ]
)

(define_insn "adddisi3_se_32"
  [(set (match_operand:DI 0 "pic30_DI_mode2_operand"      "=r,>,>,>,&r,R,R,R,&r")
        (plus:DI
           (match_operand:SI 1 "pic30_DI_mode2_operand"    "r,r,0,>, >,r,0,R, R")
           (sign_extend:DI
              (match_operand:SI 2 "pic30_register_operand" "r,r,r,r, r,r,r,r, r"))
        )
   )
   (clobber (match_scratch:SI 3                   "=&r,&r,&r,&r,&r,&r,&r,&r,&r"))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "*
{
   const char *noerrata_patterns[] = {
     /* r,r,r */          \"asr.l %2,#31,%3\;\"
                              \"add.l %1,%2,%0\;addc.l %3,%d1,%d0\",

     /* >,r,r */          \"asr.l %2,#31,%3\;\"
                              \"add.l %1,%2,%0\;addc.l %3,%d1,%0\",

     /* >,0,r */          \"asr.l %2,#31,%3\;\"
                              \"add.l %s1,%2,%0\;addc.l %3,%s1,%0\",

     /* >,>,r */          \"asr.l %2,#31,%3\;\"
                              \"add.l %1,%2,%0\;addc.l %3,%1,%0\",

     /* r,>,r */          \"asr.l %2,#31,%3\;\"
                              \"add.l %1,%2,%0\;addc.l %3,%1,%d0\",

     /* R,r,r */          \"asr.l %2,#31,%3\;\"
                              \"add.l %1,%2,%I0\;addc.l %3,%d1,%D0\",

     /* R,0,r */          \"asr.l %2,#31,%3\;\"
                              \"add.l %2,%1,%I0\;addc.l %3,%1,%D0\",

     /* R,R,r */          \"asr.l %2,#31,%3\;\"
                              \"add.l %2,%I1,%I0\;addc.l %3,%D1,%D0\",

     /* r,R,r */          \"asr.l %2,#31,%3\;\"
                              \"add.l %2,%I1,%0\;addc.l %3,%D1,%d0\",

  };

  /* no post adjustments needed for the 'R' patterns */
  return noerrata_patterns[which_alternative];

}"
   [
     (set_attr "cc" "math,math,math,math,math,clobber,clobber,clobber,clobber")
     (set_attr "type" "def")
     (set_attr "op_type" "alu")
   ]
)

(define_insn "adddisi3_ze_32"
  [(set (match_operand:DI 0 "pic30_DI_mode2_operand"      "=r,>,>,>,&r,R,R,R,&r")
        (plus:DI
           (match_operand:DI 1 "pic30_DI_mode2_operand"    "r,r,0,>, >,r,0,R, R")
           (zero_extend:DI
              (match_operand:SI 2 "pic30_register_operand" "r,r,r,r, r,r,r,r, r"))
        )
   )
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "*
{
   const char *noerrata_patterns[] = {
     /* r,r,r */          \"add.l %1,%2,%0\;addc.l %d1,#0,%d0\",

     /* >,r,r */          \"add.l %1,%2,%0\;addc.l %d1,#0,%0\",

     /* >,0,r */          \"add.l %2,%s1,%0\;addc.l %s1,#0,%0\",

     /* >,>,r */          \"add.l %2,%1,%0\;addc.l %1,#0,%0\",

     /* r,>,r */          \"add.l %2,%1,%0\;addc.l %1,#0,%d0\",

     /* R,r,r */          \"add.l %1,%2,%I0\;addc.l %d1,#0,%I0\",

     /* R,0,r */          \"add.l %2,%1,%I0\;addc.l %1,#0,%D0\",

     /* R,R,r */          \"add.l %2,%I1,%I0\;addc.l %D1,#0,%D0\",

     /* r,R,r */          \"add.l %2,%I1,%0\;addc.l %D1,#0,%d0\",
  };

  /* no post adjustments needed for the 'R' patterns */
  return noerrata_patterns[which_alternative];

}"
   [
     (set_attr "cc" "math,math,math,math,math,clobber,clobber,clobber,clobber")
     (set_attr "type" "def")
     (set_attr "op_type" "alu")
   ]
)

(define_insn "adddi3_32"
  [(set (match_operand:DI 0 "pic30_DI_mode2_operand"      "=r,>,>,>,&r,R,R,R,&r")
        (plus:DI
           (match_operand:DI 1 "pic30_register_operand"    "r,r,r,r, r,r,r,r, r")
           (match_operand:DI 2 "pic30_DI_mode2_operand"    "r,r,0,>, >,r,0,R, R")))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "*
{
   const char *noerrata_patterns[] = {
     /* r,r,r */          \"add.l %1,%2,%0\;addc.l %d1,%d2,%d0\",

     /* >,r,r */          \"add.l %1,%2,%0\;addc.l %d1,%d2,%0\",

     /* >,r,0 */          \"add.l %1,%s2,%0\;addc.l %d1,%s2,%0\",

     /* >,r,> */          \"add.l %1,%2,%0\;addc.l %d1,%2,%0\",

     /* r,r,> */          \"add.l %1,%2,%0\;addc.l %d1,%2,%d0\",

     /* R,r,r */          \"add.l %1,%2,%I0\;addc.l %d1,%d2,%D0\",

     /* R,r,0 */          \"add.l %1,%2,%I0\;addc.l %d1,%2,%D0\",

     /* R,r,R */          \"add.l %1,%I2,%I0\;addc.l %d1,%D2,%D0\",

     /* r,r,R */          \"add.l %1,%I2,%0\;addc.l %d1,%D2,%d0\"
  };

  /* no post adjustments needed for the 'R' patterns */
  return noerrata_patterns[which_alternative];
}"
   [
     (set_attr "cc" "math,math,math,math,math,clobber,clobber,clobber,clobber")
     (set_attr "type" "def")
     (set_attr "op_type" "alu")
   ]
)

;;;;;;;;
;; float
;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; subtract instructions
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;
;; quarter integer
;;;;;;;;;;;;;;;;;;

(define_insn "*subqi3_imm_32"
  [(set (match_operand:QI 0 "pic30_register_operand"          "=r,r")
        (minus:QI (match_operand:QI 1 "pic30_register_operand" "0,0")
                  (match_operand:QI 2 "pic30_JM_operand" "J,M")))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "@
   sub.b #%2,%0
   add.b #%J2,%0"
  [
   (set_attr "cc" "math")
   (set_attr "type" "def")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "subqi3_32"
  [(set (match_operand:QI 0 "pic30_mode2_operand"     "=r<>,r<>,R,R,  r<>,R,r<>,R")
        (minus:QI 
          (match_operand:QI 1 "pic30_register_operand" "r,  r,  r,r,  r,  r,r,  r")
          (match_operand:QI 2 "pic30_mode1PN_operand"  "r,  R<>,r,R<>,N,  N,P,  P")))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "@
   sub.b %1,%2,%0
   sub.b %1,%2,%0
   sub.b %1,%2,%0
   sub.b %1,%2,%0
   add.b %1,#%J2,%0
   add.b %1,#%J2,%0
   sub.b %1,#%2,%0
   sub.b %1,#%2,%0"
  [
   (set_attr "cc" "math")
   (set_attr "type" "def,defuse,use,use,def,use,def,use")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "*subqi3_sfr1_32"
  [(set (match_operand:QI 0 "pic30_register_operand"             "=r,r")
        (minus:QI (match_operand:QI 1 "pic30_near_operand"       " U,U")
                  (match_operand:QI 2 "pic30_register_operand"   " 0,r")))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "@
   sub.b %1,%0, WREG
   mov.bz %1,%0\;sub.b %2,%0,%0"
  [
   (set_attr "cc" "math")
   (set_attr "type" "def")
    (set_attr "op_type" "alu")
  ]
)

;;;;;;;;;;;;;;;
;; half integer
;;;;;;;;;;;;;;;

; leave this match_dup, operand 1 cannot interfere with reload (CAW)
(define_insn "*subhi3_imm_32"
  [(set (match_operand:HI 0 "pic30_register_operand"          "=r,r")
        (minus:HI (match_dup 0)
                  (match_operand:HI 1 "pic30_JM_operand" "J,M")))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "@
   sub.w #%1,%0
   add.w #%J1,%0"
  [
   (set_attr "cc" "math")
   (set_attr "type" "def")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "subhi3_32"
  [(set (match_operand:HI 0 "pic30_mode2_operand"
             "=r<>,R,r<>,R,  r<>,R,r<>,R,  r<>,R,r<>,R,r<>,R")
        (minus:HI 
           (match_operand:HI 1 "pic30_mode1P_operand"
              "r,  r,r,  r,  r,  r,R<>,R<>,r,  r,r,  r,P,  P")
           (match_operand:HI 2 "pic30_mode1PN_operand"
              "r,  r,R<>,R<>,r,  r,r,  r,  N,  N,P,  P,r,  r")))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "@
   sub.w %1,%2,%0
   sub.w %1,%2,%0
   sub.w %1,%2,%0
   sub.w %1,%2,%0
   subr.w %2,%1,%0
   subr.w %2,%1,%0
   subr.w %2,%1,%0
   subr.w %2,%1,%0
   add.w %1,#%J2,%0
   add.w %1,#%J2,%0
   sub.w %1,#%2,%0
   sub.w %1,#%2,%0
   subr.w %2,#%1,%0
   subr.w %2,#%1,%0"
  [
     (set_attr "cc" "math")
     (set_attr "type"
  	 "def,use,defuse,use,def,use,defuse,use,def,use,def,use,def,use")
    (set_attr "op_type" "alu")
  ]
)

; this match_can cause issues iff operand 1 is dies in this instruction and
;   we decide to use it to reload operand 0 (CAW)
(define_insn "subhi3_sfr0_32"
  [(set (match_operand:HI   0 "pic30_reg_or_near_operand" "+U,r")
        (minus:HI 
          (match_dup 0)
          (match_operand:HI 1 "pic30_register_operand"     "r,r")))
   (clobber (reg:CC CC_REG))
]
  "(pic30_isa32_target())"
  "@
   sub.w %0
   sub.w %0,%1,%0"
  [
    (set_attr "cc" "math")
    (set_attr "type" "etc,def")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "*subhi3_sfr1_32"
  [(set (match_operand:HI   0 "pic30_register_operand"   "=r,r")
        (minus:HI 
          (match_operand:HI 1 "pic30_near_operand"       " U,U")
          (match_operand:HI 2 "pic30_register_operand"   " 0,r")))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "@
   sub.w %1,%0,WREG
   mov.w %2,%0\;sub.w %1,%0,WREG"          
  [
   (set_attr "cc" "math")
   (set_attr "type" "def")
    (set_attr "op_type" "alu")
  ]
)

;;;;;;;;;;;;;;;;;
;; single integer
;;;;;;;;;;;;;;;;;

(define_insn "*subsihi3_32"
  [(set (match_operand:SI 0 "pic30_register_operand"          "=r")
        (minus:SI 
          (match_operand:SI 1 "pic30_register_operand"         "r")
          (zero_extend:SI 
            (match_operand:HI 2 "pic30_register_operand"       "r"))))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "ze.w %2,%0\;sub.l %1,%0,%0"
  [
   (set_attr "cc" "math")
   (set_attr "type" "def")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "*subsi3_imm0_32"
  [(set (match_operand:SI 0 "pic30_register_operand"          "=r,r")
        (minus:SI (match_operand:SI 1 "pic30_register_operand" "r,r")
                  (match_operand:SI 2 "pic30_PN_operand"       "N,P")))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "@
   add.l %1,#%J2,%0
   sub.l %1,#%2,%0"
  [
    (set_attr "cc" "math")
    (set_attr "type" "def")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "*subsi3_imm1_32"
  [(set (match_operand:SI 0 "pic30_register_operand"          "=r")
        (minus:SI (match_operand:SI 1 "pic30_P_operand"        "P")
                  (match_operand:SI 2 "pic30_register_operand" "r")))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "subr.l %2,#%1,%0"
  [
    (set_attr "cc" "math")
    (set_attr "type" "def")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "subsi3_imm2_32"
  [(set (match_operand:SI 0 "pic30_register_operand"          "=r,r")
        (minus:SI (match_operand:SI 1 "pic30_register_operand" "0,0")
                  (match_operand:SI 2 "pic30_JM_operand"       "J,M")))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "@
   sub.l #%2,%0
   add.l #%J2,%0"
  [
    (set_attr "cc" "math")
    (set_attr "type" "def")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "subsi3_32"
  [(set (match_operand:SI   0 "pic30_reg_or_R_operand" "=r,r,r,&r,R,R,R,&r,R,R")
        (minus:SI 
          (match_operand:SI 1 "pic30_reg_or_R_operand" "r,0,r, r,r,r,r,  R,0,R")
          (match_operand:SI 2 "pic30_reg_or_R_operand" "r,r,0, R,r,0,R, r,r,r")
   ))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "@
   sub.l %1,%2,%0
   sub.l %1,%2,%0
   sub.l %1,%2,%0
   sub.l %1,%2,%0
   sub.l %1,%2,%0
   sub.l %1,%2,%0
   sub.l %1,%2,%0
   subr.l %2,%1,%0
   subr.l %2,%1,%0
   subr.l %2,%1,%0"
  [
    (set_attr "cc" "math")
    (set_attr "type" "def,def,def,defuse,use,use,use,defuse,use,use")
    (set_attr "op_type" "alu")
  ]
)

;;;;;;;;;;;;;;;;;
;; double integer
;;;;;;;;;;;;;;;;;

(define_insn "subdi3_imm_32"
  [(set (match_operand:DI 0 "pic30_register_operand"           "=r,r")
        (minus:DI (match_operand:DI 1 "pic30_register_operand" " 0,0")
                  (match_operand:DI 2 "pic30_JM_operand"       " J,M")))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "@
   sub.l #%2,%0,#%2\;subb.l #0,%d0
   add.l #%J2,%0\;addc.l #0,%d0"
  [
    (set_attr "cc" "math")
    (set_attr "type" "def")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "subhidi3_se_32"
  [(set (match_operand:DI 0 "pic30_DI_mode2_operand"      "=r,>,>,>,&r,R,R,R,&r")
        (minus:DI
           (sign_extend:DI
              (match_operand:HI 1 "pic30_register_operand" "r,r,r,r, r,r,r,r, r"))
           (match_operand:DI 2 "pic30_DI_mode2_operand"    "r,r,0,>, >,r,0,R, R")))
   (clobber (match_scratch:SI 3                   "=&r,&r,&r,&r,&r,&r,&r,&r,&r"))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "*
{
   const char *noerrata_patterns[] = {
     /* r,r,r */          \"se.w %1,%3\;sub.l %3,%2,%0\;\"
                              \"asr.l %3,#31,%3\;subb.l %3,%d2,%d0\",

     /* >,r,r */          \"se.w %1,%3\;sub.l %3,%2,%0\;\"
                              \"asr.l %3,#31,%3\;subb.l %3,%d2,%0\",

     /* >,r,0 */          \"se.w %1,%3\;sub.l %3,%s2,%0\;\"
                              \"asr.l %3,#31,%3\;subb.l %3,%s2,%0\",

     /* >,r,> */          \"se.w %1,%3\;sub.l %3,%2,%0\;\"
                              \"asr.l %3,#31,%3\;subb.l %3,%2,%0\",

     /* r,r,> */          \"se.w %1,%3\;sub.l %3,%2,%0\;\"
                              \"asr.l %3,#31,%3\;subb.l %3,%2,%d0\",

     /* R,r,r */          \"se.w %1,%3\;sub.l %3,%2,%I0\;\"
                              \"asr.l %3,#31,%3\;subb.l %3,%2,%D0\",

     /* R,r,0 */          \"se.w %1,%3\;sub.l %3,%2,%I0\;\"
                              \"asr.l %3,#31,%3\;subb.l %3,%2,%D0\",

     /* R,r,R */          \"se.w %1,%3\;sub.l %3,%I2,%I0\;\"
                              \"asr.l %3,#31,%3\;subb.l %3,%D2,%D0\",

     /* r,r,R */          \"se.w %1,%3\;sub.l %3,%I2,%0\;\"
                              \"asr.l %3,#31,%3\;subb.l %3,%D2,%d0\",
   };

   /* no post adjustments needed for the 'R' patterns */
   return noerrata_patterns[which_alternative];
}"
   [
     (set_attr "cc" "math,math,math,math,math,clobber,clobber,clobber,clobber")
     (set_attr "type" "def")
     (set_attr "op_type" "alu")
   ]
)

(define_insn "subhidi3_ze_32"
  [(set (match_operand:DI 0 "pic30_DI_mode2_operand"      "=r,>,>,>,&r,R,R,R,&r")
        (minus:DI
           (zero_extend:DI
              (match_operand:HI 1 "pic30_register_operand" "r,r,r,r, r,r,r,r, r"))
           (match_operand:DI 2 "pic30_DI_mode2_operand"    "r,r,0,>, >,r,0,R, R")))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "*
{
   const char *noerrata_patterns[] = {
     /* r,r,r */          \"sub.l %1,%2,%0\;subbr.l %d2,#0,%d0\",

     /* >,r,r */          \"sub.l %1,%2,%0\;subbr.l %d2,#0,%0\",

     /* >,r,0 */          \"sub.l %1,%s2,%0\;subbr.l %s2,#0,%0\",

     /* >,r,> */          \"sub.l %1,%2,%0\;subbr.l %2,#0,%0\",

     /* r,r,> */          \"sub.l %1,%2,%0\;subbr.l %2,#0,%d0\",

     /* R,r,r */          \"sub.l %1,%2,%I0\;subbr.l %d2,#0,%D0\",

     /* R,r,0 */          \"sub.l %1,%2,%I0\;subbr.l %2,#0,%D0\",

     /* R,r,R */          \"sub.l %1,%I2,%I0\;subbr.l %D2,#0,%D0\",

     /* r,r,R */          \"sub.l %1,%I2,%0\;subbr.l %D2,#0,%d0\"
  };

  /* no post adjustments needed for the 'R' patterns */
  return noerrata_patterns[which_alternative]; 
}"
   [
     (set_attr "cc" "math,math,math,math,math,clobber,clobber,clobber,clobber")
     (set_attr "type" "def")
     (set_attr "op_type" "alu")
   ]
)

(define_insn "subsidi3_se_32"
  [(set (match_operand:DI 0 "pic30_DI_mode2_operand"      "=r,>,>,>,&r,R,R,R,&r")
        (minus:DI
           (sign_extend:DI
              (match_operand:SI 1 "pic30_register_operand" "r,r,r,r, r,r,r,r, r"))
           (match_operand:DI 2 "pic30_DI_mode2_operand"    "r,r,0,>, >,r,0,R, R")))
   (clobber (match_scratch:SI 3                   "=&r,&r,&r,&r,&r,&r,&r,&r,&r"))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "*
{
   const char *noerrata_patterns[] = {
     /* r,r,r */          \"se.w %1,%3\;sub.l %1,%2,%0\;subb.l %3,%d2,%d0\",

     /* >,r,r */          \"se.w %1,%3\;sub.l %1,%2,%0\;subb.l %3,%d2,%0\",

     /* >,r,0 */          \"se.w %1,%3\;sub.l %1,%s2,%0\;subb.l %3,%s2,%0\",

     /* >,r,> */          \"se.w %1,%3\;sub.l %1,%2,%0\;subb.l %3,%2,%0\",

     /* r,r,> */          \"se.w %1,%3\;sub.l %1,%2,%0\;subb.l %3,%2,%d0\",

     /* R,r,r */          \"se.w %1,%3\;sub.l %1,%2,%I0\;subb.l %3,%d2,%D0\",

     /* R,r,0 */          \"se.w %1,%3\;sub.l %1,%2,%I0\;subb.l %3,%2,%D0\",

     /* R,r,R */          \"se.w %1,%3\;sub.l %1,%I2,%I0\;subb.l %3,%D2,%D0\",

     /* r,r,R */          \"se.w %1,%3\;sub.l %1,%I2,%0\;subb.l %3,%D2,%d0\",
  };

  /* no post adjustments needed for the 'R' patterns */
  return noerrata_patterns[which_alternative];
}"
   [
     (set_attr "cc" "math,math,math,math,math,clobber,clobber,clobber,clobber")
     (set_attr "type" "def")
     (set_attr "op_type" "alu")
   ]
)

(define_insn "subsidi3_ze_32"
  [(set (match_operand:DI 0 "pic30_DI_mode2_operand"      "=r,>,>,>,&r,R,R,R,&r")
        (minus:DI
           (zero_extend:DI
              (match_operand:SI 1 "pic30_register_operand" "r,r,r,r, r,r,r,r, r"))
           (match_operand:DI 2 "pic30_DI_mode2_operand"    "r,r,0,>, >,r,0,R, R")))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "*
{
   const char *noerrata_patterns[] = {
     /* r,r,r */          \"sub.l %1,%2,%0\;subbr.l %d2,#0,%d0\",

     /* >,r,r */          \"sub.l %1,%2,%0\;subbr.l %d2,#0,%0\",

     /* >,r,0 */          \"sub.l %1,%s2,%0\;subbr.l %s2,#0,%0\",

     /* >,r,> */          \"sub.l %1,%2,%0\;subbr.l %2,#0,%0\",

     /* r,r,> */          \"sub.l %1,%2,%0\;subbr.l %2,#0,%d0\",

     /* R,r,r */          \"sub.l %1,%2,%I0\;subbr.l %d2,#0,%I0\",

     /* R,r,0 */          \"sub.l %1,%2,%I0\;subbr.l %2,#0,%I0\",

     /* R,r,R */          \"sub.l %1,%I2,%I0\;subbr.l %I2,#0,%I0\",

     /* r,r,R */          \"sub.l %1,%I2,%0\;subbr.l %I2,#0,%d0\",
  };

  /* no post adjustments needed for the 'R' patterns */
  return noerrata_patterns[which_alternative];
}"
   [
     (set_attr "cc" "math,math,math,math,math,clobber,clobber,clobber,clobber")
     (set_attr "type" "def")
     (set_attr "op_type" "alu")
   ]
)

(define_insn "subdihi3_se_32"
  [(set (match_operand:DI 0 "pic30_DI_mode2_operand"      "=r,>,>,>,&r,R,R,R,&r")
        (minus:DI
           (match_operand:DI 1 "pic30_DI_mode2_operand"    "r,r,0,>, >,r,0,R, R")
           (sign_extend:DI
              (match_operand:HI 2 "pic30_register_operand" "r,r,r,r, r,r,r,r, r"))
))
   (clobber (match_scratch:SI 3                   "=&r,&r,&r,&r,&r,&r,&r,&r,&r"))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "*
{
   const char *noerrata_patterns[] = {
     /* r,r,r */          \"se.w %2,%3\;sub.l %1,%3,%0\;\"
                              \"asr.l %3,#31,%3\;subb.l %d1,%3,%d0\",

     /* >,r,r */          \"se.w %2,%3\;sub.l %1,%3,%0\;\"
                              \"asr.l %3,#31,%3\;subb.l %d1,%3,%0\",

     /* >,0,r */          \"se.w %2,%3\;subr.l %3,%s1,%0\;\"
                              \"asr.l %3,#31,%3\;subb.l %3,%s1,%0\",

     /* >,>,r */          \"se.w %2,%3\;subr.l %3,%1,%0\;\"
                              \"asr.l %3,#31,%3\;subb.l %3,%1,%0\",

     /* r,>,r */          \"se.w %2,%3\;subr.l %3,%1,%0\;\"
                              \"asr.l %3,#31,%3\;subb.l %3,%1,%d0\",

     /* R,r,r */          \"se.w %2,%3\;subr.l %3,%1,%I0\;\"
                              \"asr.l %3,#31,%3\;subb.l %3,%1,%D0\",

     /* R,0,r */          \"se.w %2,%3\;subr.l %3,%1,%I0\;\"
                              \"asr.l %3,#31,%3\;subb.l %3,%1,%D0\",

     /* R,R,r */          \"se.w %2,%3\;subr.l %3,%I1,%I0\;\"
                              \"asr.l %3,#31,%3\;subb.l %3,%D1,%D0\",

     /* r,R,r */          \"se.w %2,%3\;subr.l %3,%I1,%0\;\"
                              \"asr.l %3,#31,%3\;subb.l %3,%D1,%d0\",
  };

  /* no post adjustments needed for the 'R' patterns */
  return noerrata_patterns[which_alternative];
}"
   [
     (set_attr "cc" "math,math,math,math,math,clobber,clobber,clobber,clobber")
     (set_attr "type" "def")
     (set_attr "op_type" "alu")
   ]
)

(define_insn "subdihi3_ze_32"
  [(set (match_operand:DI 0 "pic30_DI_mode2_operand"      "=r,>,>,>,&r,R,R,R,&r")
        (minus:DI
           (match_operand:DI 1 "pic30_DI_mode2_operand"    "r,r,0,>, >,r,0,R, R")
           (zero_extend:DI
              (match_operand:HI 2 "pic30_register_operand" "r,r,r,r, r,r,r,r, r"))
))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "*
{
   const char *noerrata_patterns[] = {
     /* r,r,r */          \"sub.l %1,%2,%0\;subb.l %d1,#0,%d0\",

     /* >,r,r */          \"sub.l %1,%2,%0\;subb.l %d1,#0,%0\",

     /* >,0,r */          \"subr.l %2,%s1,%0\;subb.l %s1,#0,%0\",

     /* >,>,r */          \"subr.l %2,%1,%0\;subb.l %1,#0,%0\",

     /* r,>,r */          \"subr.l %2,%1,%0\;subb.l %1,#0,%d0\",

     /* R,r,r */          \"sub.l %1,%2,%I0\;subb.l %d1,#0,%I0\",

     /* R,0,r */          \"subr.l %2,%1,%I0\;subb.l %1,#0,%I0\",

     /* R,R,r */          \"subr.l %2,%I1,%I0\;subb.l %I1,#0,%I0\",

     /* r,R,r */          \"subr.l %2,%I1,%0\;subb.l %I1,#0,%d0\"
  };

  /* no post adjustments needed for the 'R' patterns */
  return noerrata_patterns[which_alternative];
}"
   [
     (set_attr "cc" "math,math,math,math,math,clobber,clobber,clobber,clobber")
     (set_attr "type" "def")
     (set_attr "op_type" "alu")
   ]
)

(define_insn "subdisi3_se_32"
  [(set (match_operand:DI 0 "pic30_DI_mode2_operand"      "=r,>,>,>,&r,R,R,R,&r")
        (minus:DI
           (match_operand:DI 1 "pic30_DI_mode2_operand"    "r,r,0,>, >,r,0,R, R")
           (sign_extend:DI
              (match_operand:SI 2 "pic30_register_operand" "r,r,r,r, r,r,r,r, r"))
))
   (clobber (match_scratch:SI 3                   "=&r,&r,&r,&r,&r,&r,&r,&r,&r"))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "*
{
   const char *noerrata_patterns[] = {
     /* r,r,r */     \"asr.l %2,#31,%3\;sub.l %1,%2,%0\;subb.l %d1,%3,%d0\",
             
     /* >,r,r */     \"asr.l %2,#31,%3\;sub.l %1,%2,%0\;subb.l %d1,%3,%0\",
             
     /* >,0,r */     \"asr.l %2,#31,%3\;subr.l %2,%s1,%0\;subbr.l %3,%s1,%0\",
             
     /* >,>,r */     \"asr.l %2,#31,%3\;subr.l %2,%1,%0\;subbr.l %3,%1,%0\",
             
     /* r,>,r */     \"asr.l %2,#31,%3\;subr.l %2,%1,%0\;subbr.l %3,%1,%d0\",
             
     /* R,r,r */     \"asr.l %2,#31,%3\;subr.l %2,%1,%I0\;subbr.l %3,%1,%D0\",
             
     /* R,0,r */     \"asr.l %2,#31,%3\;subr.l %2,%1,%I0\;subbr.l %3,%1,%D0\",
             
     /* R,R,r */     \"asr.l %2,#31,%3\;subr.l %2,%I1,%I0\;subbr.l %3,%D1,%D0\",
             
     /* r,R,r */     \"asr.l %2,#31,%3\;subr.l %2,%I1,%0\;subbr.l %3,%D1,%d0\",
  };

  /* no post adjustments needed for the 'R' patterns */
  return noerrata_patterns[which_alternative];
}"
   [
     (set_attr "cc" "math,math,math,math,math,clobber,clobber,clobber,clobber")
     (set_attr "type" "def")
     (set_attr "op_type" "alu")
   ]
)

(define_insn "subdisi3_ze_32"
  [(set (match_operand:DI 0 "pic30_DI_mode2_operand"      "=r,>,>,>,&r,R,R,R,&r")
        (minus:DI
           (match_operand:DI 1 "pic30_DI_mode2_operand"    "r,r,0,>, >,r,0,R, R")
           (zero_extend:DI
              (match_operand:SI 2 "pic30_register_operand" "r,r,r,r, r,r,r,r, r"))
))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "*
{
   const char *noerrata_patterns[] = {
     /* r,r,r */          \"sub.l %1,%2,%0\;subb.l %d1,#0,%d0\",

     /* >,r,r */          \"sub.l %1,%2,%0\;subb.l %d1,#0,%0\",
             
     /* >,0,r */          \"subr.l %2,%s1,%0\;subb.l %s1,#0,%0\",
             
     /* >,>,r */          \"subr.l %2,%1,%0\;subb.l %1,#0,%0\",
             
     /* r,>,r */          \"subr.l %2,%1,%0\;subb.l %1,#0,%d0\",
             
     /* R,r,r */          \"subr.l %2,%1,%I0\;subb.l %1,#0,%D0\",
             
     /* R,0,r */          \"subr.l %2,%1,%I0\;subb.l %1,#0,%D0\",
             
     /* R,R,r */          \"subr.l %2,%I1,%I0\;subb.l %D1,#0,%D0\",
             
     /* r,R,r */          \"subr.l %2,%I1,%0\;subb.l %D1,#0,%d0\",
  };

  /* no post adjustments needed for the 'R' patterns */
  return noerrata_patterns[which_alternative];
}"
   [
     (set_attr "cc" "math,math,math,math,math,clobber,clobber,clobber,clobber")
     (set_attr "type" "def")
     (set_attr "op_type" "alu")
   ]
)

(define_insn "subdi3_32"
  [(set (match_operand:DI 0 "pic30_DI_mode2_operand"      "=r,>,>,>,&r,R,R,R,&r")
        (minus:DI
           (match_operand:DI 1 "pic30_register_operand"    "r,r,r,r, r,r,r,r, r")
           (match_operand:DI 2 "pic30_DI_mode2_operand"    "r,r,0,>, >,r,0,R, R")))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "*
{
   const char *noerrata_patterns[] = {
     /* r,r,r */          \"sub.l %1,%2,%0\;subb.l %d1,%d2,%d0\",

     /* >,r,r */          \"sub.l %1,%2,%0\;subb.l %d1,%d2,%0\",

     /* >,r,0 */          \"sub.l %1,%s2,%0\;subb.l %d1,%s2,%0\",

     /* >,r,> */          \"sub.l %1,%2,%0\;subb.l %d1,%2,%0\",

     /* r,r,> */          \"sub.l %1,%2,%0\;subb.l %d1,%2,%d0\",

     /* R,r,r */          \"sub.l %1,%2,%I0\;subb.l %d1,%d2,%D0\",

     /* R,r,0 */          \"sub.l %1,%2,%I0\;subb.l %d1,%2,%D0\",

     /* R,r,R */          \"sub.l %1,%I2,%I0\;subb.l %d1,%D2,%D0\",

     /* r,r,R */          \"sub.l %1,%I2,%0\;subb.l %d1,%D2,%d0\"
  };

  /* no post adjustments needed for the 'R' patterns */
  return noerrata_patterns[which_alternative];

}"
   [
     (set_attr "cc" "math")
     (set_attr "type" "def")
     (set_attr "op_type" "alu")
   ]
)


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; multiply instructions
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; 16-bit product

;; 8 x 8 => 16 (unsigned)

(define_expand "umulqihi3_32"
  [(set (match_operand:HI 0 "pic30_register_operand" "")
    (mult:HI 
      (zero_extend:HI (match_operand:QI 1 "pic30_register_operand" ""))
      (zero_extend:HI (match_operand:QI 2 "pic30_reg_imm_or_near_operand" ""))))
  ]
;; NULLSTONE CSE fails when enabled
  "0"
  "
  {
    if (GET_CODE(operands[2]) == CONST_INT) {
      int pow2 = INTVAL(operands[2]);
      if (pic30_one_bit_set_p(pow2)) {
        emit_insn(gen_umulqihi3pow2(operands[0], operands[1], operands[2]));
      } else {
        emit_insn(gen_umulqihi3imm(operands[0], operands[1], operands[2]));
      }
    } else {
      emit_insn(gen_umulqihi3gen(operands[0], operands[1], operands[2]));
    }
    DONE;
  }"
)

(define_insn "umulqihi3gen_32"
  [(set (match_operand:HI     0 "pic30_creg_operand"      "=c")
        (mult:HI 
          (zero_extend:HI 
            (match_operand:QI 1 "pic30_register_operand"  "%r"))
          (zero_extend:HI 
            (match_operand:QI 2 "pic30_near_operand"      " U"))))]
  "(pic30_isa32_target())"
  "mul.b %2,%1"
  [
   (set_attr "cc" "change0")
   (set_attr "type" "def")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "umulqihi3pow2_32"
  [(set (match_operand:HI     0 "pic30_register_operand" "=r")
        (mult:HI 
          (zero_extend:HI 
            (match_operand:QI 1 "pic30_register_operand" "%r"))
          (match_operand:HI   2 "immediate_operand"       "i")))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_one_bit_set(GET_MODE(operands[0]),operands[2],1))"
  "ze %1,%0\;sl %0,#%b2,%0"
  [
   (set_attr "cc" "clobber")
   (set_attr "type" "def")
    (set_attr "op_type" "alu")
  ]
)

;; 16 x 16 => 16

;; this expansion is helpful in constant propagation
;; (but in general, it leads to code expansion, since
;; it causes all integer multiplication to be inline)

(define_expand "mulhi3_32"
  [(set (match_operand:HI 0 "pic30_register_operand" "")
        (mult:HI (match_operand:HI 1 "pic30_register_operand" "")
                 (match_operand:HI 2 "pic30_reg_or_imm_operand" "")))]
  "(TARGET_ISA32)"
  "
{ rtx temp_reg = 0, temp_opnd1 = 0, temp_opnd2 = 0;
  int valid_immediate = 0;

  if (immediate_operand(operands[2], VOIDmode)) {
    if (INTVAL(operands[2]) == 2) {
      emit_insn(gen_mulhi3imm_32(operands[0], operands[1], operands[2]));
      DONE;
    } else if (pic30_P_operand(operands[2],VOIDmode)) {
      valid_immediate = 1;
    } else {
      temp_opnd2  = gen_reg_rtx(HImode);

      emit_move_insn(temp_opnd2, operands[2]);
    }
  }
  temp_reg = gen_reg_rtx(SImode);
  if (valid_immediate) {
    emit_insn(
      gen_mulhisi3_imm_32(temp_reg,
                       temp_opnd1 ? temp_opnd1 : operands[1],
                       temp_opnd2 ? temp_opnd2 : operands[2])
    );
  } else {
    emit_insn(
      gen_mulhisi3_32(temp_reg,
                   temp_opnd1 ? temp_opnd1 : operands[1],
                   temp_opnd2 ? temp_opnd2 : operands[2])
    );
  }
  convert_move(operands[0], temp_reg, 0);
  DONE;
}")

(define_insn "mulhi3imm_32"
  [(set (match_operand:HI 0 "pic30_mode2_operand"         "=r,r<>,r<>,R,R")
        (mult:HI 
            (match_operand:HI 1 "pic30_near_mode2_operand" "U,r,  R<>,r,R<>")
            (match_operand:HI 2 "immediate_operand"        "i,i,  i,  i,i")))
   (clobber (reg:CC_NZC CC_REG))
  ]
  "(pic30_isa32_target()) && (INTVAL(operands[2]) == 2)"
  "@
     sl.w %1,%0
     sl.w %1,%0
     sl.w %1,%0
     sl.w %1,%0
     sl.w %1,%0"
  [
   (set_attr "cc" "math")
   (set_attr "type" "def,def,defuse,use,use")
    (set_attr "op_type" "alu")
  ]
)

; 32-bit product
; /* *_extend of an immediate_operand is illegal, apparantly 
;    so need two separate sequences */

(define_insn "umulhisi3imm_32"
  [(set (match_operand:SI 0 "pic30_register_operand"      "=r")
        (mult:SI 
          (zero_extend:SI 
             (match_operand:HI 1 "pic30_register_operand" "%r"))
          (match_operand:SI 2 "pic30_P_operand"            "P")))]
  "(pic30_isa32_target())"
  "muluu.w %1,#%2,%0"
  [
   (set_attr "cc" "change0")
   (set_attr "type" "def")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "umulhisi3_32"
  [(set (match_operand:SI     0 "pic30_register_operand" "=r,r")
        (mult:SI 
          (zero_extend:SI 
            (match_operand:HI 1 "pic30_register_operand" "%r,r"))
          (zero_extend:SI 
            (match_operand:HI 2 "pic30_mode2_operand"    "r,R<>"))))]
  "(pic30_isa32_target())"
  "muluu.w %1,%2,%0"
  [
   (set_attr "cc" "change0")
   (set_attr "type" "def,defuse")
    (set_attr "op_type" "alu")
  ]
)

;; 16 x 16 => 32 (signed)

;; /* *_extend of an immediate_operand is illegal, apparantly -
;;   so need separate patterns */

(define_insn "mulhisi3_imm_32"
  [(set (match_operand:SI 0 "pic30_register_operand"     "=r")
        (mult:SI 
          (sign_extend:SI 
            (match_operand:HI 1 "pic30_register_operand" "%r"))
            (match_operand:SI 2 "immediate_operand"       "i")))]
  "(pic30_isa32_target() && 
   (INTVAL(operands[2]) < 256) && (INTVAL(operands[2]) > 0))"
  "mulsu.w %1,#%2,%0"
  [
   (set_attr "cc" "change0")
   (set_attr "type" "def")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "mulhisi3_32"
  [(set (match_operand:SI     0 "pic30_register_operand" "=r,r")
        (mult:SI 
          (sign_extend:SI 
            (match_operand:HI 1 "pic30_register_operand" "%r,r"))
          (sign_extend:SI 
            (match_operand:HI 2 "pic30_mode2_operand"    "r,R<>"))))]
  "(pic30_isa32_target())"
  "mulss.w %1,%2,%0"
  [
   (set_attr "cc" "change0")
   (set_attr "type" "def,defuse")
    (set_attr "op_type" "alu")
  ]
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; builtin 16x16->32 instructions
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define_insn "mulsu_32"
  [(set (match_operand:SI 0 "pic30_register_operand"               "=r,r,  r,r")
        (unspec:SI [
                    (match_operand:HI 1 "pic30_mode2_or_P_operand" "r ,r,  r,P")
                    (match_operand:HI 2 "pic30_mode2_or_P_operand" "r ,R<>,P,r")
        ] UNSPECV_MULSU))
                   ]
  "(pic30_isa32_target())"
  "@
   mulsu.w %1,%2,%0
   mulsu.w %1,%2,%0
   mulsu.w %1,#%2,%0
   muluu.w %2,#%1,%0"
  [
   (set_attr "cc" "change0")
   (set_attr "type" "def,defuse,def,def")
    (set_attr "op_type" "alu")
  ]
)

;
; For the purpose of RAW stalls, an accumulator register can never
;   be used as a pointer
;
(define_insn "mulsu_acc_32"
  [(set (match_operand:HI 0 "pic30_accumulator_operand" "=w,w,w,w")
        (subreg:HI 
          (unspec:SI [
                    (match_operand:HI 1 "pic30_mode2_or_P_operand" "r,r,  r,P")
                    (match_operand:HI 2 "pic30_mode2_or_P_operand" "r,R<>,P,r")
          ] UNSPECV_MULSU) 0))
                   ]
  "(pic30_isa32_target())"
  "@
   mulsu.w %1,%2,%0
   mulsu.w %1,%2,%0
   mulsu.w %1,#%2,%0
   muluu.w %2,#%1,%0"
  [
   (set_attr "cc" "change0")
   (set_attr "type" "etc,use,etc,etc")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "muluu_32"
  [(set (match_operand:SI 0 "pic30_register_operand"               "=r,r,  r,r")
        (unspec:SI [
                    (match_operand:HI 1 "pic30_mode2_or_P_operand" "r ,r,  r,P")
                    (match_operand:HI 2 "pic30_mode2_or_P_operand" "r ,R<>,P,r")
        ] UNSPECV_MULUU))
  ]
  "(pic30_isa32_target())"
  "@
   muluu.w %1,%2,%0
   muluu.w %1,%2,%0
   muluu.w %1,#%2,%0
   muluu.w %2,#%1,%0"
  [
   (set_attr "cc" "change0")
   (set_attr "type" "def,defuse,def,def")
    (set_attr "op_type" "alu")
  ]
)

;
; For the purpose of RAW stalls, an accumulator register can never
;   be used as a pointer
;
(define_insn "muluu_acc_32"
  [(set (match_operand:HI 0 "pic30_accumulator_operand"  "=w,w,w,w")
        (subreg:HI 
          (unspec:SI [
                    (match_operand:HI 1 "pic30_mode2_or_P_operand" "r,r,  r,P")
                    (match_operand:HI 2 "pic30_mode2_or_P_operand" "r,R<>,P,r")
          ] UNSPECV_MULUU) 0))
  ]
  "(pic30_isa32_target())"
  "@
   muluu.w %1,%2,%0
   muluu.w %1,%2,%0
   muluu.w %1,#%2,%0
   muluu.w %2,#%1,%0"
  [
   (set_attr "cc" "change0")
   (set_attr "type" "etc,use,etc,etc")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "mulus_32"
  [(set (match_operand:SI 0 "pic30_register_operand"               "=r,r")
        (unspec:SI [
	            (match_operand:HI 1 "pic30_register_operand"    "r,r")
	            (match_operand:HI 2 "pic30_mode2_operand" "r,R<>")
	] UNSPECV_MULUS))
  ]
  "(pic30_isa32_target())"
  "mulus.w %1,%2,%0"
  [
   (set_attr "cc" "change0")
   (set_attr "type" "def,defuse")
    (set_attr "op_type" "alu")
  ]
)

;
; For the purpose of RAW stalls, an accumulator register can never
;   be used as a pointer
;
(define_insn "mulus_acc_32"
  [(set (match_operand:HI 0 "pic30_accumulator_operand" "=w,w")
        (subreg:HI 
          (unspec:SI [
                    (match_operand:HI 1 "pic30_register_operand"    "r,r")
                    (match_operand:HI 2 "pic30_mode2_operand" "r,R<>")
        ] UNSPECV_MULUS) 0))
  ]
  "(pic30_isa32_target())"
  "mul.us %1,%2,%0"
  [
   (set_attr "cc" "change0")
   (set_attr "type" "etc,use")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "mulss_32"
  [(set (match_operand:SI 0 "pic30_register_operand"               "=r,r")
        (unspec:SI [
	            (match_operand:HI 1 "pic30_register_operand"    "r,r")
	            (match_operand:HI 2 "pic30_mode2_operand" "r,R<>")
	] UNSPECV_MULSS)) 
  ]
  "(pic30_isa32_target())"
  "mulss.w %1,%2,%0"
  [
   (set_attr "cc" "change0")
   (set_attr "type" "def,defuse")
    (set_attr "op_type" "alu")
  ]
)

; For the purpose of RAW stalls, an accumulator register can never
;   be used as a pointer
;
(define_insn "mulss_acc_32"
  [(set (match_operand:HI 0 "pic30_accumulator_operand" "=w,w")
        (subreg:HI  
          (unspec:SI [
            (match_operand:HI 1 "pic30_register_operand" "r,r")
            (match_operand:HI 2 "pic30_mode2_operand" "r,R<>")
          ] UNSPECV_MULSS) 0))
  ]
  "(pic30_isa32_target())"
  "mulss.w %1,%2,%0"
  [
   (set_attr "cc" "change0")
   (set_attr "type" "etc,use")
    (set_attr "op_type" "alu")
  ]
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; 32 x 32 => 32 (signed / unsigned) ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;
;;  For e-core machine we don't want to use the normal mechanism because
;;    the normal mulhi wants an even register (which is better for)
;;    16-bit mul but not for composing 32-bit muls
;;


(define_insn "mulsi3"
  [(set (match_operand:SI   0 "pic30_register_operand" "=r")
        (mult:SI 
          (match_operand:SI 1 "pic30_register_operand"  "r")
          (match_operand:SI 2 "pic30_register_operand"  "r")))
  ]
  "pic30_isa32_target()"
  "mulss.l %1,%2,%0"
  [
    (set_attr "op_type" "alu")
  ]
)

(define_insn "mulsidi3"
  [(set (match_operand:DI     0 "pic30_register_operand" "=r")
        (mult:DI
          (sign_extend:DI
            (match_operand:SI 1 "pic30_register_operand" " r"))
          (sign_extend:DI
            (match_operand:SI 2 "pic30_register_operand" " r"))))
  ]
  "pic30_isa32_target()"
  "mulss.d %1,%2,%0"
  [
    (set_attr "op_type" "alu")
  ]
)

(define_insn "umulsidi3"
  [(set (match_operand:DI     0 "pic30_register_operand" "=r")
        (mult:DI
          (zero_extend:DI
            (match_operand:SI 1 "pic30_register_operand" " r"))
          (zero_extend:DI
            (match_operand:SI 2 "pic30_register_operand" " r"))))
  ]
  "pic30_isa32_target()"
  "muluu.d %1,%2,%0"
  [
    (set_attr "op_type" "alu")
  ]
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; divide instructions
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; builtin 32/16 instructions
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define_insn "divmodhi2_32"
  [(set (match_operand:DI 0 "pic30_register_operand"  "=&r,!!&r")
        (unspec:DI [
          (match_operand:HI 1 "pic30_register_operand" "0,r")
          (match_operand:HI 2 "pic30_register_operand" "r,r")
        ] UNSPEC_DIVMOD))
   (clobber (reg:HI RCOUNT))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "@
   repeat #__TARGET_DIVIDE_CYCLES16\;divs.w %0,%2
   mov.w %1,%0\;repeat #__TARGET_DIVIDE_CYCLES16\;divs.w %0,%2"
  [
    (set_attr "cc" "clobber")  ; should have new CC of div
    (set_attr "type" "def")
    (set_attr "op_type" "alu")
  ]
)

(define_expand "divmodhi4_32"
  [(set (match_operand:HI 0 "pic30_register_operand" "=r")
        (div:HI
          (match_operand:HI 1 "pic30_register_operand"  "r")
          (match_operand:HI 2 "pic30_register_operand"  "r")))
   (set (match_operand:HI 3  "pic30_register_operand" "=r")
        (mod:HI
          (match_dup 1)
          (match_dup 2)))
   (clobber (reg:HI RCOUNT))
  ]
  "(pic30_isa32_target())"
  "
  {  
    rtx result = gen_reg_rtx(DImode);

    emit(
      gen_divmodhi2_32(result, operands[1], operands[2])
    );
    emit_move_insn(operands[0], gen_rtx_SUBREG(HImode, result, 0));
    emit_move_insn(operands[3], gen_rtx_SUBREG(HImode, result, 4));
    DONE;
  }"
)

(define_insn "divmodsihi2_32"
  [(set (match_operand:DI 0 "pic30_register_operand"  "=&r,!!&r")
        (unspec:DI [
          (match_operand:SI 1 "pic30_register_operand" "0,r")
          (match_operand:HI 2 "pic30_register_operand" "r,r")
        ] UNSPECV_DIVMODSD))
   (clobber (reg:HI RCOUNT))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "@
   repeat #__TARGET_DIVIDE_CYCLES16\;divs.l %0,%2
   mov.w %1,%0\;repeat #__TARGET_DIVIDE_CYCLES16\;divs.l %0,%2"
  [
    (set_attr "cc" "clobber")  ; should have new CC of div
    (set_attr "type" "def")
    (set_attr "op_type" "alu")
  ]
)

(define_expand "sdivsihi_32"
  [(set (match_operand:HI 0 "pic30_register_operand" "=r")
        (unspec:HI [
          (match_operand:SI 1 "pic30_register_operand"  "r")
          (match_operand:HI 2 "pic30_register_operand"  "r")
         ] UNSPECV_DIVSD))
   (clobber (reg:HI RCOUNT))
  ]
  "(pic30_isa32_target())"
  "
  {
    rtx result = gen_reg_rtx(DImode);

    emit(
      gen_divmodsihi2_32(result, operands[1], operands[2])
    );
    emit_move_insn(operands[0], gen_rtx_SUBREG(HImode, result, 0));
    DONE;
  }"
)

(define_expand "smodsihi_32"
  [(set (match_operand:HI 0 "pic30_register_operand" "=r")
        (unspec:HI [
          (match_operand:SI 1 "pic30_register_operand"  "r")
          (match_operand:HI 2 "pic30_register_operand"  "r")
         ] UNSPECV_MODSD))
   (clobber (reg:HI RCOUNT))
  ]
  "(pic30_isa32_target())"
  "
  {
    rtx result = gen_reg_rtx(DImode);

    emit(
      gen_divmodsihi2_32(result, operands[1], operands[2])
    );
    emit_move_insn(operands[0], gen_rtx_SUBREG(HImode, result, 4));
    DONE;
  }"
)

(define_expand "sdivmodsihi_32"
  [(set (match_operand:HI 0 "pic30_register_operand" "=r")
        (unspec:HI [
          (match_operand:SI 1 "pic30_register_operand"  "r")
          (match_operand:HI 2 "pic30_register_operand"  "r")
         ] UNSPECV_DIVMODSD))
   (set (mem:HI
          (match_operand:SI 3  "pic30_register_operand" "r"))
        (unspec:HI [
          (match_dup 1)
          (match_dup 2)
         ] UNSPECV_DIVMODSD))
   (clobber (reg:HI RCOUNT))
  ]
  "(pic30_isa32_target())"
  "
  {  
    rtx result = gen_reg_rtx(DImode);

    emit(
      gen_divmodsihi2_32(result, operands[1], operands[2])
    );
    emit_move_insn(operands[0], gen_rtx_SUBREG(HImode, result, 0));
    emit_move_insn(gen_rtx_MEM(HImode,operands[3]), 
                   gen_rtx_SUBREG(HImode, result, 4));
    DONE;
  }"
)

(define_insn "udivmodhi2_32"
  [(set (match_operand:DI   0 "pic30_register_operand" "=&r,!!&r")
        (unspec:DI [
          (match_operand:HI 1 "pic30_register_operand" " 0,r")
          (match_operand:HI 2 "pic30_register_operand" " r,r")
        ] UNSPEC_UDIVMOD))
   (clobber (reg:HI RCOUNT))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "@
   repeat #__TARGET_DIVIDE_CYCLES16\;divu.w %0,%2
   mov.w %1,%0\;repeat #__TARGET_DIVIDE_CYCLES16\;divu.w %0,%2"
  [
    (set_attr "cc" "clobber")  ; should have new CC of div
    (set_attr "type" "def")
    (set_attr "op_type" "alu")
  ]
)

(define_expand "udivmodhi4_32"
  [(set (match_operand:HI 0 "pic30_register_operand" "=r")
        (udiv:HI
          (match_operand:HI 1 "pic30_register_operand"  "r")
          (match_operand:HI 2 "pic30_register_operand"  "r")))
   (set (match_operand:HI 3  "pic30_register_operand" "=r")
        (mod:SI 
          (match_dup 1)
          (match_dup 2)))
   (clobber (reg:HI RCOUNT))
  ]
  "(pic30_isa32_target())"
  "
  {
    rtx result = gen_reg_rtx(DImode);
          
    emit(
      gen_udivmodhi2_32(result, operands[1], operands[2])
    );
    emit_move_insn(operands[0], gen_rtx_SUBREG(HImode, result, 0));
    emit_move_insn(operands[3], gen_rtx_SUBREG(HImode, result, 4));
    DONE;
  }"
)

(define_insn "udivmodsihi2_32"
  [(set (match_operand:DI   0 "pic30_register_operand" "=&r,!!&r")
        (unspec:DI [
          (match_operand:SI 1 "pic30_register_operand" " 0,r")
          (match_operand:HI 2 "pic30_register_operand" " r,r")
        ] UNSPECV_DIVMODUD))
   (clobber (reg:HI RCOUNT))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "@
   repeat #__TARGET_DIVIDE_CYCLES16\;divu.l %0,%2
   mov.w %1,%0\;repeat #__TARGET_DIVIDE_CYCLES16\;divu.l %0,%2"
  [
    (set_attr "cc" "clobber")  ; should have new CC of div
    (set_attr "type" "def")
    (set_attr "op_type" "alu")
  ]
)

(define_expand "udivsihi_32"
  [(set (match_operand:HI 0 "pic30_register_operand" "=r")
        (unspec:HI [
          (match_operand:SI 1 "pic30_register_operand"  "r")
          (match_operand:HI 2 "pic30_register_operand"  "r")
         ] UNSPECV_DIVUD))
   (clobber (reg:HI RCOUNT))
  ]
  "(pic30_isa32_target())"
  "
  {
    rtx result = gen_reg_rtx(DImode);

    emit(
      gen_udivmodsihi2_32(result, operands[1], operands[2])
    );
    emit_move_insn(operands[0], gen_rtx_SUBREG(HImode, result, 0));
    DONE;
  }"
)

(define_expand "umodsihi_32"
  [(set (match_operand:HI 0 "pic30_register_operand" "=r")
        (unspec:HI [
          (match_operand:SI 1 "pic30_register_operand"  "r")
          (match_operand:HI 2 "pic30_register_operand"  "r")
         ] UNSPECV_MODUD))
   (clobber (reg:HI RCOUNT))
  ]
  "(pic30_isa32_target())"
  "
  {
    rtx result = gen_reg_rtx(DImode);

    emit(
      gen_udivmodsihi2_32(result, operands[1], operands[2])
    );
    emit_move_insn(operands[0], gen_rtx_SUBREG(HImode, result, 4));
    DONE;
  }"
)

(define_expand "udivmodsihi_32"
  [(set (match_operand:HI 0 "pic30_register_operand" "=r")
        (unspec:HI [
          (match_operand:SI 1 "pic30_register_operand"  "r")
          (match_operand:HI 2 "pic30_register_operand"  "r")
         ] UNSPECV_DIVMODUD))
   (set (mem:HI
          (match_operand:SI 3  "pic30_register_operand" "r"))
        (unspec:HI [
          (match_dup 1)
          (match_dup 2)
         ] UNSPECV_DIVMODUD))
   (clobber (reg:HI RCOUNT))
  ]
  "(pic30_isa32_target())"
  "
  {
    rtx result = gen_reg_rtx(DImode);

    emit(
      gen_udivmodsihi2_32(result, operands[1], operands[2])
    );
    emit_move_insn(operands[0], gen_rtx_SUBREG(HImode, result, 0));
    emit_move_insn(gen_rtx_MEM(HImode,operands[3]), 
                   gen_rtx_SUBREG(HImode, result, 4));
    DONE;
  }"
)

(define_insn "divmodsi2_helper_32"
  [(set (subreg:SI
           (match_operand:DI 0 "pic30_register_operand"  "=&r,!!&r") 0)
        (unspec_volatile:SI [
           (match_operand:SI 1  "pic30_register_operand" " 0,r")
           (match_operand:SI 2  "pic30_register_operand"  "r,r")
         ] UNSPEC_DIVMOD)) 
   (set (subreg:SI (match_dup 0) 4)
        (unspec_volatile:SI [
           (match_dup 1)
           (match_dup 2)
         ] UNSPEC_DIVMOD))
   (clobber (reg:HI RCOUNT))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "@
   repeat #__TARGET_DIVIDE_CYCLES32\;divsl %0,%2
   mov.l %1,%0\;repeat #__TARGET_DIVIDE_CYCLES32\;divsl %0,%2"
  [
    (set_attr "cc" "clobber")  ; should have new CC of div
    (set_attr "type" "def")
    (set_attr "op_type" "alu")
  ]
)

(define_expand "divmodsi4_32"
  [(set (match_operand:SI 0 "pic30_register_operand" "=r")
        (div:SI
          (match_operand:SI 1 "pic30_register_operand"  "r")
          (match_operand:SI 2 "pic30_register_operand"  "r")))
   (set (match_operand:SI 3  "pic30_register_operand" "=r")
        (mod:SI
          (match_dup 1)
          (match_dup 2)))
   (clobber (reg:HI RCOUNT))
  ]
  "(pic30_isa32_target())"
  "
  {  rtx result = gen_reg_rtx(DImode);

     emit(
       gen_divmodsi2_helper_32(result, operands[1], operands[2])
     );
     emit_move_insn(operands[0], gen_rtx_SUBREG(SImode, result, 0));
     emit_move_insn(operands[3], gen_rtx_SUBREG(SImode, result, 4));
     DONE;
  }"
)

(define_insn "udivmodsi2_helper_32"
  [(set (match_operand:DI 0 "pic30_register_operand"  "=&r,!!&r")
        (unspec:DI [
          (match_operand:SI 1 "pic30_register_operand" "0,r")
          (match_operand:SI 2 "pic30_register_operand" "r,r")
        ] UNSPEC_UDIVMOD))
   (clobber (reg:HI RCOUNT))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "@
   repeat #__TARGET_DIVIDE_CYCLES32\;divul %0,%2
   mov.l %1,%0\;repeat #__TARGET_DIVIDE_CYCLES32\;divul %0,%2"
  [
    (set_attr "cc" "clobber")  ; should have new CC of div
    (set_attr "type" "def")
    (set_attr "op_type" "alu")
  ]
)

(define_expand "udivmodsi4_32"
  [(set (match_operand:SI 0 "pic30_register_operand" "=r")
        (udiv:SI
          (match_operand:SI 1 "pic30_register_operand"  "r")
          (match_operand:SI 2 "pic30_register_operand"  "r")))
   (set (match_operand:SI 3  "pic30_register_operand" "=r")
        (mod:SI 
          (match_dup 1)
          (match_dup 2)))
   (clobber (reg:HI RCOUNT))
  ]
  "(pic30_isa32_target())"
  "
  {  rtx result = gen_reg_rtx(DImode);

     emit(
       gen_udivmodsi2_helper_32(result, operands[1], operands[2])
     );
     emit_move_insn(operands[0], gen_rtx_SUBREG(SImode, result, 0));
     emit_move_insn(operands[3], gen_rtx_SUBREG(SImode, result, 4));
     DONE;
  }"
)

(define_insn "divfmodhi2_32"
  [(set (match_operand:DI 0 "pic30_register_operand"  "=&r,!!&r")
        (unspec:DI [
          (match_operand:HI 1 "pic30_register_operand" "0,r")
          (match_operand:HI 2 "pic30_register_operand" "r,r")
        ] UNSPECV_DIVF))
   (clobber (reg:HI RCOUNT))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "@
   repeat #__TARGET_DIVIDE_CYCLES16\;divf.w %0,%2
   mov.w %1,%0\;repeat #__TARGET_DIVIDE_CYCLES16\;divf.w %0,%2"
  [
    (set_attr "cc" "clobber")  ; should have new CC of div
    (set_attr "type" "def")
    (set_attr "op_type" "alu")
  ]
)

(define_expand "divf_32"
  [(set (match_operand:HI 0 "pic30_register_operand" "=r")
        (div:HI
          (match_operand:HI 1 "pic30_register_operand"  "r")
          (match_operand:HI 2 "pic30_register_operand"  "r")))
   (clobber (reg:HI RCOUNT))
  ]
  "(pic30_isa32_target())"
  "
  {
    rtx result = gen_reg_rtx(DImode);

    emit(
      gen_divfmodhi2_32(result, operands[1], operands[2])
    );
    emit_move_insn(operands[0], gen_rtx_SUBREG(HImode, result, 0));
    DONE;
  }"
)

(define_expand "divmodf_32"
  [(set (match_operand:HI 0 "pic30_register_operand" "=r")
        (unspec:HI [
          (match_operand:HI 1 "pic30_register_operand"  "r")
          (match_operand:HI 2 "pic30_register_operand"  "r")
         ] UNSPEC_DIVMODF))
   (set (match_operand:HI 3  "pic30_register_operand" "=r")
        (unspec:HI [
          (match_dup 1)
          (match_dup 2)
         ] UNSPEC_DIVMODF))
   (clobber (reg:HI RCOUNT))
  ]
  "(pic30_isa32_target())"
  "
  {
    rtx result = gen_reg_rtx(DImode);

    emit(
      gen_divfmodhi2_32(result, operands[1], operands[2])
    );
    emit_move_insn(operands[0], gen_rtx_SUBREG(HImode, result, 0));
    emit_move_insn(operands[3], gen_rtx_SUBREG(HImode, result, 4));
    DONE;
  }"
)

(define_insn "divfmodsihi2_32"
  [(set (match_operand:DI 0 "pic30_register_operand"  "=&r,!!&r")
        (unspec:DI [
          (match_operand:SI 1 "pic30_register_operand" "0,r")
          (match_operand:HI 2 "pic30_register_operand" "r,r")
        ] UNSPECV_DIVF))
   (clobber (reg:HI RCOUNT))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "@
   repeat #__TARGET_DIVIDE_CYCLES16\;divf.l %0,%2
   mov.w %1,%0\;repeat #__TARGET_DIVIDE_CYCLES16\;divf.l %0,%2"
  [
    (set_attr "cc" "clobber")  ; should have new CC of div
    (set_attr "type" "def")
    (set_attr "op_type" "alu")
  ]
)

(define_expand "divfsihi_32"
  [(set (match_operand:HI 0 "pic30_register_operand" "=r")
        (div:HI
          (match_operand:SI 1 "pic30_register_operand"  "r")
          (match_operand:HI 2 "pic30_register_operand"  "r")))
   (clobber (reg:HI RCOUNT))
  ]
  "(pic30_isa32_target())"
  "
  {
    rtx result = gen_reg_rtx(DImode);

    emit(
      gen_divfmodsihi2_32(result, operands[1], operands[2])
    );
    emit_move_insn(operands[0], gen_rtx_SUBREG(HImode, result, 0));
    DONE;
  }"
)

(define_expand "divmodfsihi_32"
  [(set (match_operand:HI 0 "pic30_register_operand" "=r")
        (unspec:HI [
          (match_operand:SI 1 "pic30_register_operand"  "r")
          (match_operand:HI 2 "pic30_register_operand"  "r")
         ] UNSPEC_DIVMODF))
   (set (match_operand:HI 3  "pic30_register_operand" "=r")
        (unspec:HI [
          (match_dup 1)
          (match_dup 2)
         ] UNSPEC_DIVMODF))
   (clobber (reg:HI RCOUNT))
  ]
  "(pic30_isa32_target())"
  "
  {
    rtx result = gen_reg_rtx(DImode);

    emit(
      gen_divfmodsihi2_32(result, operands[1], operands[2])
    );
    emit_move_insn(operands[0], gen_rtx_SUBREG(HImode, result, 0));
    emit_move_insn(operands[3], gen_rtx_SUBREG(HImode, result, 4));
    DONE;
  }"
)

(define_insn "divfmodsi2_32"
  [(set (match_operand:DI 0 "pic30_register_operand"  "=&r,!!&r")
        (unspec:DI [
          (match_operand:SI 1 "pic30_register_operand" "0,r")
          (match_operand:SI 2 "pic30_register_operand" "r,r")
        ] UNSPECV_DIVF))
   (clobber (reg:HI RCOUNT))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "@
   repeat #__TARGET_DIVIDE_CYCLES32\;divfl %0,%2
   mov.w %1,%0\;repeat #__TARGET_DIVIDE_CYCLES32\;divfl %0,%2"
  [
    (set_attr "cc" "clobber")  ; should have new CC of div
    (set_attr "type" "def")
    (set_attr "op_type" "alu")
  ]
)

(define_expand "divfsi_32"
  [(set (match_operand:SI 0 "pic30_register_operand" "=r")
        (div:SI
          (match_operand:SI 1 "pic30_register_operand"  "r")
          (match_operand:SI 2 "pic30_register_operand"  "r")))
   (clobber (reg:HI RCOUNT))
  ]
  "(pic30_isa32_target())"
  "
  {
    rtx result = gen_reg_rtx(DImode);

    emit(
      gen_divfmodsi2_32(result, operands[1], operands[2])
    );
    emit_move_insn(operands[0], gen_rtx_SUBREG(SImode, result, 0));
    DONE;
  }"
)

(define_expand "divmodfsi_32"
  [(set (match_operand:SI 0 "pic30_register_operand" "=r")
        (unspec:SI [
          (match_operand:SI 1 "pic30_register_operand"  "r")
          (match_operand:SI 2 "pic30_register_operand"  "r")
         ] UNSPEC_DIVMODF))
   (set (match_operand:HI 3  "pic30_register_operand" "=r")
        (unspec:SI [
          (match_dup 1)
          (match_dup 2)
         ] UNSPEC_DIVMODF))
   (clobber (reg:HI RCOUNT))
  ]
  "(pic30_isa32_target())"
  "
  {
    rtx result = gen_reg_rtx(DImode);

    emit(
      gen_divfmodsi2_32(result, operands[1], operands[2])
    );
    emit_move_insn(operands[0], gen_rtx_SUBREG(SImode, result, 0));
    emit_move_insn(operands[3], gen_rtx_SUBREG(SImode, result, 4));
    DONE;
  }"
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; square root instructions
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Other arithmetic instructions:
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Absolute value
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define_insn "absqi2_32"
   [(set (match_operand:QI 0 "pic30_register_operand" "=r")
        (abs:QI
          (match_operand:QI 1 "pic30_register_operand" "r")))
    (clobber (reg:CC CC_REG))
   ]
   "(pic30_isa32_target())"
   "add.b %1,#0,%0\;bra nn,.LSKP%=\;neg.b %1,%0\;.LSKP%=:"
   [
     (set_attr "cc" "clobber")
     (set_attr "type" "def")
     (set_attr "op_type" "alu")
   ]
)

;;;;;;;;;;;;;;;;;;
;; Half integer
;;;;;;;;;;;;;;;;;;

(define_insn "abshi2_32"
   [(set (match_operand:HI 0 "pic30_register_operand" "=r")
        (abs:HI 
          (match_operand:HI 1 "pic30_register_operand" "r")))
    (clobber (reg:CC CC_REG))
   ]
   "(pic30_isa32_target())"
   "add.w %1,#0,%0\;bra nn,.LSKP%=\;neg.w %1,%0\;.LSKP%=:"
   [
     (set_attr "cc" "clobber")
     (set_attr "type" "def")
     (set_attr "op_type" "alu")
   ]
)

(define_insn "abssi2_32"
   [(set (match_operand:SI 0 "pic30_register_operand"  "=r")
         (abs:SI 
           (match_operand:SI 1 "pic30_register_operand" "r")))
    (clobber (reg:CC CC_REG))
   ]
   "(pic30_isa32_target())"
   "add.l %1,#0,%0\;bra nn,.LSKP%=\;neg.l %1,%0\;.LSKP%=:"
   [
     (set_attr "cc" "clobber")
     (set_attr "type" "def")
     (set_attr "op_type" "alu")
   ]
)

;;;;;;;;;;;;;;;;;;
;; Single float
;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;
;; Double float
;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Negation
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;
;; Quarter integer 
;;;;;;;;;;;;;;;;;;

(define_insn "negqi2_32"
  [(set (match_operand:QI 0 "pic30_mode2_operand"        "=r<>,r<>,R,R")
        (neg:QI (match_operand:QI 1 "pic30_mode2_operand" "r,  R<>,r,R<>")))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "neg.b %1,%0"
  [
   (set_attr "cc" "set")
   (set_attr "type" "def,defuse,use,use")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "*negqi2_sfr0_32"
  [(set (match_operand:QI 0 "pic30_register_operand"    "=r")
        (neg:QI (match_operand:QI 1 "pic30_near_operand" "U")))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "neg.b %1,%0"
  [
   (set_attr "cc" "set")
   (set_attr "type" "def")
    (set_attr "op_type" "alu")
  ]
)

; leave this match_dup, operand 0 will not require a reload (CAW)
(define_insn "*negqi2_sfr1_32"
  [(set (match_operand:QI 0 "pic30_near_operand"        "=U")
        (neg:QI (match_dup 0)))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "neg.b %0"
  [
    (set_attr "cc" "set")
    (set_attr "op_type" "alu")
  ]
)

;;;;;;;;;;;;;;;;;;
;; Half integer 
;;;;;;;;;;;;;;;;;;

(define_insn "neghi2_32"
  [(set (match_operand:HI 0 "pic30_mode2_operand"        "=r<>,r<>,R,R")
        (neg:HI (match_operand:HI 1 "pic30_mode2_operand" "r,  R<>,r,R<>")))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "neg %1,%0"
  [
   (set_attr "cc" "set")
   (set_attr "type" "def,defuse,use,use")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "*neghi2_sfr0_32"
  [(set (match_operand:HI 0 "pic30_register_operand"            "=r")
        (neg:HI (match_operand:HI 1 "pic30_reg_or_near_operand" " Ur")))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "neg %1,%0"
  [
   (set_attr "cc" "set")
   (set_attr "type" "def")
   (set_attr "op_type" "alu")
  ]
)

; leave this match_dup, there is no operand 2 to interfere with a reload (CAW)
(define_insn "*neghi2_sfr1_32"
  [(set (match_operand:HI 0 "pic30_reg_or_near_operand"        "=U,r")
        (neg:HI (match_dup 0)))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "@
   neg %0
   neg %0,%0"
  [
   (set_attr "cc" "set")
   (set_attr "type" "etc,def")
   (set_attr "op_type" "alu")
  ]
)

;;;;;;;;;;;;;;;;;;
;; Single integer 
;;;;;;;;;;;;;;;;;;

(define_insn "negsi2_32"
  [(set (match_operand:SI 0 "pic30_register_operand"  "=r")
        (neg:SI 
          (match_operand:SI 1 "pic30_register_operand" "r")))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "neg.l %1,%0"
  [
    (set_attr "cc" "set")
    (set_attr "type" "def")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "negdi2_32"
  [(set (match_operand:DI 0 "pic30_register_operand"  "=r")
        (neg:DI 
          (match_operand:DI 1 "pic30_register_operand" "r")))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "subr.l %1,#0,%0\;subbr.l %d1,#0,%d0"
  [
    (set_attr "cc" "set")
    (set_attr "type" "def")
    (set_attr "op_type" "alu")
  ]
)

;;;;;;;;;;;;;;;;;;
;; Single float
;;;;;;;;;;;;;;;;;;

; leave this match_dup, operand 0 will not require a reload (CAW)
(define_insn "*negsf2sfr_32"
  [(set (match_operand:SF 0        "pic30_near_operand" "=U")
        (neg:SF (match_dup 0)))]
  "(pic30_isa32_target())"
  "btg.b %0+3,#7"
  [
    (set_attr "cc" "clobber")
    (set_attr "op_type" "alu")
  ]
)

;;;;;;;;;;;;;;;;;;
;; Double float
;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; bit-logical instructions
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;
;; Quarter integer 
;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;
;; Set Bit ;;
;;;;;;;;;;;;;

(define_insn "bitsetqi_32"
  [(set (match_operand:QI 0 "pic30_mode2_operand"        "=r,R<>")
        (ior:QI (match_operand:QI 1 "pic30_mode2_operand" "0,0")
                (match_operand 2 "const_int_operand"      "i,i")))]
  "(pic30_one_bit_set_p(INTVAL(operands[2])&0x00ff))"
  "bset.b %0,#%b2"
  [
   (set_attr "type" "def,etc")
    (set_attr "op_type" "alu")
  ]
)

; leave this match_dup, operand 1 cannot interfere with reload (CAW)
(define_insn "bitsetqi_sfr_32"
  [(set (match_operand:QI 0 "pic30_near_operand"        "=U")
        (ior:QI  (match_dup 0)
                 (match_operand 1 "const_int_operand" "i")))]
  "pic30_one_bit_set_p(INTVAL(operands[1]) & 0x00ff)"
  "bset.b %0,#%b1"
  [
    (set_attr "type" "etc")
    (set_attr "op_type" "alu")
  ]
)

;;;;;;;;;;;;;;;
;; Reset Bit ;;
;;;;;;;;;;;;;;;

(define_insn "bitclrqi_32"
  [(set (match_operand:QI 0 "pic30_mode2_operand"        "=r,R,<>")
        (and:QI (match_operand:QI 1 "pic30_mode2_operand" "0,0,0")
                (match_operand 2 "const_int_operand"      "i,i,i")))]
  "(pic30_one_bit_set_p((~INTVAL(operands[2])&0xff)))"
  "bclr.b %0,#%B2"
  [
   (set_attr "type" "def,use,defuse")
    (set_attr "op_type" "alu")
  ]
)

; leave this match_dup, operand 1 cannot interfere with reload (CAW)
(define_insn "bitclrqi_sfr_32"
  [(set (match_operand:QI 0 "pic30_near_operand"        "=U")
        (and:QI  (match_dup 0)
                 (match_operand 1 "const_int_operand" "i")))]
  "(pic30_one_bit_set_p((~INTVAL (operands[1])) & 0x00ff))"
  "bclr.b %0,#%B1"
  [
    (set_attr "type" "use")
    (set_attr "op_type" "alu")
  ]
)


;;;;;;;;;;;;;;;;
;; Toggle Bit ;;
;;;;;;;;;;;;;;;;

(define_insn "bittogqi_32"
  [(set (match_operand:QI 0 "pic30_mode2_operand"        "=r,R,<>")
        (xor:QI (match_operand:QI 1 "pic30_mode2_operand" "0,0,0")
                (match_operand 2 "const_int_operand"      "i,i,i")))]
  "(TARGET_ISA32) && (pic30_one_bit_set_p(INTVAL(operands[2])&0xffff))"
  "btg.b %0,#%b2"
  [
   (set_attr "type" "def,use,defuse")
    (set_attr "op_type" "alu")
  ]
)

; leave this match_dup, operand 1 cannot interfere with reload (CAW)
(define_insn "bittogqi_sfr_32"
  [(set (match_operand:QI 0 "pic30_near_operand"        "=U")
        (xor:QI  (match_dup 0)
                 (match_operand 1 "const_int_operand" "i")))]
  "(TARGET_ISA32) && (pic30_one_bit_set_p(INTVAL(operands[1]) & 0x00ff))"
  "btg.b %0,#%b1"
  [
    (set_attr "type" "use")
    (set_attr "op_type" "alu")
  ]
)

;;;;;;;;;;;;;;
;; Test Bit ;;
;;;;;;;;;;;;;;

(define_insn "*bittstqi_and_32"
  [(set (reg:CC_Z CC_REG)
        (and (match_operand:QI 0 "pic30_reg_or_near_operand" "r,U")
             (match_operand 1 "immediate_operand" "i,i")))
  ]
  "(pic30_isa32_target()) &&
   (pic30_one_bit_set(GET_MODE(operands[0]),operands[1],1))"
  "@
   btst.bz %0,#%b1
   btst.b %0,#%b1"
  [
    (set_attr "cc" "set")
    (set_attr "type" "etc,use")
    (set_attr "op_type" "alu")
  ]
)
 
(define_insn "newbittstqi_32"
  [(set (reg:CC_Z CC_REG)
        (compare
           (zero_extract 
             (match_operand 0 "pic30_mode2_or_near_operand" "R<>r,U")
             (const_int 1)
             (match_operand 1 "const_int_operand"           "i,i"))
           (const_int 0))
    )]
  "(INTVAL(operands[1]) < 8) && (pic30_isa32_target())"
  "@
   btst.bz %0,#%1
   btst.b  %0,#%1"
  [
    (set_attr "cc" "set")
    (set_attr "type" "use,etc")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "newbittstqi_r_32"
  [(set (reg:CC_Z CC_REG)
        (compare
           (zero_extract 
             (match_operand 0 "pic30_mode2_or_near_operand" "R<>r,U")
             (const_int 1)
             (match_operand 1 "pic30_register_operand"       "r,r"))
           (const_int 0))
    )
  ]
  "(pic30_isa32_target())"
  "@
   btst.bz %0,%1
   btst.b %0,%1"
  [
    (set_attr "cc" "set")
    (set_attr "type" "use,etc")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "*bittstqi_32"
  [(set (reg:CC_Z CC_REG)
        (compare 
          (zero_extract (match_operand:QI 0 "pic30_reg_or_near_operand" "r,U")
		        (const_int 1)
		        (match_operand 1 "const_int_operand" "i,i"))
          (const_int 0))
   )
  ]
  "(pic30_isa32_target())"
  "@
   btst.bz %0,#%1
   btst.b %0,#%1"
  [
    (set_attr "cc" "set")
    (set_attr "type" "etc,use")
    (set_attr "op_type" "alu")
  ]
)

;;;;;;;;;;;;;;;;;;
;; Half integer ;;
;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;
;; Set Bit ;;
;;;;;;;;;;;;;

;; the pre/post modify modes have been removed due to errata in Rev A silicon
;;   the pre/post modify modes are valid for REV_A3

(define_insn "bitsethi_32"
  [(set (match_operand:HI 0 "pic30_mode2_operand"         "=r,R,<>")
        (ior:HI  (match_operand:HI 1 "pic30_mode2_operand" "0,0,0")
                 (match_operand 2 "const_int_operand"      "i,i,i")))]
  "(pic30_one_bit_set(GET_MODE(operands[0]),operands[2],1))"
  "bset %0,#%b2"
  [
   (set_attr "type" "def,use,defuse")
    (set_attr "op_type" "alu")
  ]
)

; leave this match_dup, operand 1 cannot interfere with reload (CAW)
(define_insn "bitsethi_sfr_32"
  [(set (match_operand:HI 0 "pic30_near_operand"         "=U")
        (ior:HI  (match_dup 0)
                 (match_operand 1 "const_int_operand"  "i")))]
  "(pic30_one_bit_set(GET_MODE(operands[0]),operands[1],1))"
  "*
   {
     if ((INTVAL(operands[1])&0xffff) >= 256)
       return \"bset.b %0+1,#%b1-8\";
     else
       return \"bset.b %0,#%b1\";
   }"
  [
    (set_attr "type" "use")
    (set_attr "op_type" "alu")
  ]
)

;;;;;;;;;;;;;;;
;; Reset Bit ;;
;;;;;;;;;;;;;;;

;; the pre/post modify modes have been removed due to errata in Rev A silicon
;;   the pre/post modify modes are valid for REV_A3

(define_insn "bitclrhi_32"
  [(set (match_operand:HI 0 "pic30_mode2_operand"         "=r,R,<>")
        (and:HI  (match_operand:HI 1 "pic30_mode2_operand" "0,0,0")
                 (match_operand 2 "const_int_operand"      "i,i,i")))]
  "(pic30_one_bit_set_p((~INTVAL(operands[2])) & 0xffff))"
  "bclr %0,#%B2"
  [
   (set_attr "type" "def,use,defuse")
    (set_attr "op_type" "alu")
  ]
)

; leave this match_dup, operand 1 cannot interfere with reload (CAW)
;        (and:HI  (match_operand:HI 1 "pic30_near_operand"  "0")
(define_insn "bitclrhi_sfr_32"
  [(set (match_operand:HI 0 "pic30_near_operand"           "=U")
        (and:HI  (match_dup 0)
                 (match_operand 1 "const_int_operand" "i")))]
  "(pic30_one_bit_set_p((~INTVAL(operands[1])) & 0xffff))"
  "*
   {
     if (pic30_which_bit(~INTVAL(operands[1])) > 7)
       return \"bclr.b %0+1,#%B1-8\";
     else
       return \"bclr.b %0,#%B1\";
   }"
  [
    (set_attr "type" "use")
    (set_attr "op_type" "alu")
  ]
)

;;;;;;;;;;;;;;;;
;; Toggle Bit ;;
;;;;;;;;;;;;;;;;

;; the pre/post modify modes have been removed due to errata in Rev A silicon
;;   the pre/post modify modes are valid for REV_A3

(define_insn "bittoghi_32"
  [(set (match_operand:HI 0 "pic30_mode2_operand"         "=r,R,<>")
        (xor:HI  (match_operand:HI 1 "pic30_mode2_operand" "0,0,0")
                 (match_operand 2 "const_int_operand"      "i,i,i")))]
  "(TARGET_ISA32) && (pic30_one_bit_set(GET_MODE(operands[0]),operands[2],1))"
  "btg.w %0,#%b2"
  [
   (set_attr "type" "def,use,defuse")
    (set_attr "op_type" "alu")
  ]
)

; leave this match_dup, operand 1 cannot interfere with reload (CAW)
(define_insn "bittoghi_sfr_32"
  [(set (match_operand:HI 0 "pic30_near_operand"           "=U")
        (xor:HI  (match_dup 0)
                 (match_operand 1 "const_int_operand" "i")))]
  "(TARGET_ISA32) && (pic30_one_bit_set(GET_MODE(operands[0]),operands[1],1))"
  "*
   {
     if ((INTVAL(operands[1])&0xffff) >= 256)
       return \"btg.b %0+1,#%b1-8\";
     else
       return \"btg.b %0,#%b1\";
   }"
  [
    (set_attr "type" "use")
    (set_attr "op_type" "alu")
  ]
)

;;;;;;;;;;;;;;
;; Test Bit ;;
;;;;;;;;;;;;;;

(define_insn "*bittsthi_and_32"
  [(set (reg:CC_Z CC_REG)
        (compare
          (and (match_operand:HI 0 "pic30_near_mode2_operand" "r,R,<>,U")
               (match_operand 1 "immediate_operand"           "i,i,i, i"))
          (const_int 0))
   )
  ]
  "(pic30_isa32_target()) &&
   (pic30_one_bit_set(GET_MODE(operands[0]),operands[1],1))"
  "@
   btst.wz %0,#%b1
   btst.wz %0,#%b1
   btst.wz %0,#%b1
   btst.wz %0,#%b1"
  [
   (set_attr "cc" "set")
   (set_attr "type" "etc,use,use,etc")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "*bittsthi_32"
  [(set (reg:CC_Z CC_REG) 
        (compare
          (zero_extract:HI
            (match_operand:HI 0 "pic30_near_mode2_operand" "r,R,<>,U")
            (const_int 1)
            (match_operand 1 "const_int_operand"           "i,i,i, i"))
          (const_int 0))
   )
  ]
  "(pic30_isa32_target())"
  "@
   btst.wz %0,#%1
   btst.wz %0,#%1
   btst.wz %0,#%1
   btst.wz %0,#%1"
  [
   (set_attr "cc" "set")
   (set_attr "type" "etc,use,use,etc")
   (set_attr "op_type" "alu")
  ]
)

(define_insn "*newbittsthi_32"
  [(set (reg:CC_Z CC_REG) 
        (compare 
          (zero_extract
            (match_operand:HI 0 "pic30_near_mode2_operand" "r,R,<>,U")
            (const_int 1)
            (match_operand 1 "const_int_operand"           "i,i,i, i"))
          (const_int 0))
   )]
  "(pic30_isa32_target())"
  "@
   btst.wz %0,#%1
   btst.wz %0,#%1
   btst.wz %0,#%1
   btst.wz %0,#%1"
  [
   (set_attr "cc" "set")
   (set_attr "type" "etc,use,use,etc")
   (set_attr "op_type" "alu")
  ]
)

(define_insn "*bittsthi_r_32"
  [(set (reg:CC_Z CC_REG) 
        (compare
          (zero_extract:HI
            (match_operand:HI 0 "pic30_mode2_operand" "r,R,<>")
            (const_int 1)
            (match_operand 1 "pic30_register_operand" "r,r,r"))
          (const_int 0))
   )
  ]
  "(pic30_isa32_target())"
  "@
   btst.wz %0,%1
   btst.wz %0,%1
   btst.wz %0,%1"
  [
   (set_attr "cc" "set")
   (set_attr "type" "etc,use,use")
   (set_attr "op_type" "alu")
  ]
)

(define_insn "*newbittsthi_r_32"
  [(set (reg:CC_Z CC_REG) 
        (compare 
          (zero_extract
            (match_operand:HI 0 "pic30_mode2_operand"    "r,R,<>")
            (const_int 1)
            (match_operand:HI 1 "pic30_register_operand" "r,r,r"))
          (const_int 0))
   )]
  "(pic30_isa32_target())"
  "@
   btst.wz %0,%1
   btst.wz %0,%1
   btst.wz %0,%1"
  [
   (set_attr "cc" "set")
   (set_attr "type" "etc,use,use")
   (set_attr "op_type" "alu")
  ]
)

;;;;;;;;;;;;;;;;;;;;
;; Single integer ;;
;;;;;;;;;;;;;;;;;;;;

(define_insn "*bittstsi_and_32"
  [(set (reg:CC_Z CC_REG)
        (compare
          (and (match_operand:SI 0 "pic30_near_mode2_operand" "r,R,<>,U")
               (match_operand 1 "immediate_operand"           "i,i,i, i"))
          (const_int 0))
   )
  ]
  "(pic30_isa32_target()) &&
   (pic30_one_bit_set(GET_MODE(operands[0]),operands[1],1))"
  "@
   btst.lz %0,#%b1
   btst.lz %0,#%b1
   btst.lz %0,#%b1
   btst.lz %0,#%b1"
  [
   (set_attr "cc" "set")
   (set_attr "type" "etc,use,use,etc")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "*bittstsi_32"
  [(set (reg:CC_Z CC_REG) 
        (compare
          (zero_extract:SI
            (match_operand:SI 0 "pic30_near_mode2_operand" "r,R,<>,U")
            (const_int 1)
            (match_operand 1 "const_int_operand"           "i,i,i, i"))
          (const_int 0))
   )
  ]
  "(pic30_isa32_target())"
  "@
   btst.lz %0,#%1
   btst.lz %0,#%1
   btst.lz %0,#%1
   btst.lz %0,#%1"
  [
   (set_attr "cc" "set")
   (set_attr "type" "etc,use,use,etc")
   (set_attr "op_type" "alu")
  ]
)

(define_insn "*newbittstsi_32"
  [(set (reg:CC_Z CC_REG) 
        (compare 
          (zero_extract
            (match_operand:SI 0 "pic30_near_mode2_operand" "r,R,<>,U")
            (const_int 1)
            (match_operand 1 "const_int_operand"           "i,i,i, i"))
          (const_int 0))
   )]
  "(pic30_isa32_target())"
  "@
   btst.lz %0,#%1
   btst.lz %0,#%1
   btst.lz %0,#%1
   btst.b %0+%1/8,#%1%%8"
  [
   (set_attr "cc" "set")
   (set_attr "type" "etc,use,use,etc")
   (set_attr "op_type" "alu")
  ]
)

(define_insn "*bittstsi_r_32"
  [(set (reg:CC CC_REG) 
        (compare
          (zero_extract:SI
            (match_operand:SI 0 "pic30_mode2_operand" "r,R,<>")
            (const_int 1)
            (match_operand 1 "pic30_register_operand" "r,r,r"))
          (const_int 0))
   )
  ]
  "(pic30_isa32_target())"
  "@
   btst.lz %0,%1
   btst.lz %0,%1
   btst.lz %0,%1"
  [
   (set_attr "cc" "set")
   (set_attr "type" "etc,use,use")
   (set_attr "op_type" "alu")
  ]
)

(define_insn "*newbittstsi_r_32"
  [(set (reg:CC_Z CC_REG) 
        (compare 
          (zero_extract
            (match_operand:SI 0 "pic30_mode2_operand" "r,R,<>")
            (const_int 1)
            (match_operand 1 "pic30_register_operand" "r,r,r"))
          (const_int 0))
   )]
  "(pic30_isa32_target())"
  "@
   btst.lz %0,%1
   btst.lz %0,%1
   btst.lz %0,%1"
  [
   (set_attr "cc" "set")
   (set_attr "type" "etc,use,use")
   (set_attr "op_type" "alu")
  ]
)

;;;;;;;;;;;;;
;; Set Bit ;;
;;;;;;;;;;;;;

(define_insn "bitsetsi_32"
  [(set (match_operand:SI 0 "pic30_mode2_operand"      "=r,R<>")
        (ior:SI  
          (match_operand:SI 1 "pic30_mode2_operand"    "0,0")
          (match_operand 2 "const_int_operand"         "i,i")))]
  "(pic30_isa32_target() && 
    pic30_one_bit_set(GET_MODE(operands[0]),operands[2],1))"
  "@
   bset.l %0,#%b2
   bset.l %0,#%b2"
  [
    (set_attr "type" "def,use")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "bitsetsir_32"
  [(set (match_operand:SI 0 "pic30_register_operand"  "=r")
        (ior:SI  
          (match_operand:SI 1 "pic30_register_operand" "0")
          (match_operand 2 "const_int_operand"         "i")))]
  "((pic30_isa32_target()) && 
    pic30_one_bit_set(GET_MODE(operands[0]),operands[2],1))"
  "bset.l %0,#%b2"
  [
    (set_attr "type" "def")
    (set_attr "op_type" "alu")
  ]
)

; leave this match_dup, operand 1 cannot interfere with reload (CAW)
(define_insn "bitsetsi_sfr_32"
  [(set (match_operand:SI 0 "pic30_near_operand"         "=U")
        (ior:SI  (match_dup 0)
                 (match_operand 1 "const_int_operand"  "i")))]
  "(pic30_isa32_target()) &&
   (pic30_one_bit_set(GET_MODE(operands[0]),operands[1],1))"
  "*
   {
     unsigned int n = INTVAL(operands[1]);

     if (n >= 0x1000000)
       return \"bset.b %0+3,#%b1-24\";
     else if (n >= 0x10000)
       return \"bset.b %0+2,#%b1-16\";
     else if (n >= 0x0100)
       return \"bset.b %0+1,#%b1-8\";
     else return \"bset.b %0+0,#%b1\";
   }"
  [
    (set_attr "type" "etc")
    (set_attr "op_type" "alu")
  ]
)

;;;;;;;;;;;;;;;
;; Reset Bit ;;
;;;;;;;;;;;;;;;

; leave this match_dup, operand 1 cannot interfere with reload (CAW)
(define_insn "bitclrsi_sfr_32"
  [(set (match_operand:SI 0 "pic30_near_operand"         "=U")
        (and:SI  (match_dup 0)
                 (match_operand 1 "const_int_operand"  "i")))]
  "(pic30_isa32_target()) &&
   (pic30_one_bit_set(GET_MODE(operands[0]),operands[1],1))"
  "*
   {
     int n = pic30_which_bit(~INTVAL(operands[1]));

     if (n > 23)
       return \"bclr.b %0+3,#%B1-24\";
     else if (n > 15)
       return \"bclr.b %0+2,#%B1-16\";
     else if (n > 7)
       return \"bclr.b %0+1,#%B1-8\";
     else return \"bclr.b %0+0,#%B1\";
   }"
  [
    (set_attr "type" "etc")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "bitclrsi_32"
  [(set (match_operand:SI 0 "pic30_register_operand"  "=r")
        (and:SI  
          (match_operand:SI 1 "pic30_register_operand" "0")
          (match_operand 2 "const_int_operand"         "i")))]
  "(pic30_isa32_target()) &&
   (pic30_one_bit_set(GET_MODE(operands[0]),operands[2],0))"
  "bclr.l %0,#%B2"
  [
    (set_attr "type" "def")
    (set_attr "op_type" "alu")
  ]
)

;;;;;;;;;;;;;;;;
;; Toggle Bit ;;
;;;;;;;;;;;;;;;;

(define_insn "bittogsi_32"
  [(set (match_operand:SI 0 "pic30_mode2_operand"     "=r,R,<>")
        (xor:SI  
          (match_operand:SI 1 "pic30_mode2_operand"    "0,0,0")
          (match_operand 2 "const_int_operand"         "i,i,i")))]
  "(pic30_isa32_target()) && 
   (pic30_one_bit_set(GET_MODE(operands[0]),operands[2],1))"
  "btg.l %0,#%b2"
  [
    (set_attr "type" "def")
    (set_attr "op_type" "alu")
  ]
)

; leave this match_dup, operand 1 cannot interfere with reload (CAW)
(define_insn "bittogsi_sfr_32"
  [(set (match_operand:SI 0 "pic30_near_operand"         "=U")
        (xor:SI  (match_dup 0)
                 (match_operand 1 "const_int_operand"  "i")))]
  "(pic30_isa32_target()) &&
   (pic30_one_bit_set(GET_MODE(operands[0]),operands[1],1))"
  "*
   {
     unsigned int n = INTVAL(operands[1]);
     if (n >= 0x01000000)
       return \"btg.b %0+3,#%b1-24\";
     else if (n >= 0x00010000)
       return \"btg.b %0+2,#%b1-16\";
     else if (n >= 0x00000100)
       return \"btg.b %0+1,#%b1-8\";
     else return \"btg.b %0+0,#%b1-0\";
   }"
  [
    (set_attr "type" "etc")
    (set_attr "op_type" "alu")
  ]
)

;;;;;;;;;;;;;;
;; Test Bit ;;
;;;;;;;;;;;;;;

(define_insn "bittstsir_and_32"
  [(set (reg:CC_Z CC_REG) 
        (compare
          (and 
            (match_operand:SI 0 "pic30_register_operand"  "r")
            (match_operand    1 "const_int_operand"       "i"))
          (const_int 0))
   )
  ]
  "(pic30_isa32_target()) && 
   (pic30_one_bit_set(GET_MODE(operands[0]),operands[1],1))"
  "btst.l %0,#%b1"
  [
    (set_attr "cc" "set")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "*bittstsi_sfr_and_32"
  [(set (reg:CC_Z CC_REG) 
        (compare 
          (and (match_operand:SI 0 "pic30_near_operand" "U")
               (match_operand    1 "const_int_operand"  "i"))
          (const_int 0))
   )
  ]
  "(pic30_isa32_target()) &&
   (pic30_one_bit_set(GET_MODE(operands[0]),operands[1],1))"
  "btst.b %0+%b1/8,#%b1%%8"
  [
    (set_attr "cc" "set")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "bittstsiR_32"
  [(set (reg:CC_Z CC_REG)
        (compare 
          (zero_extract 
            (match_operand:SI 0 "pic30_reg_or_R_operand" "r,R")
            (const_int 1)
            (match_operand    1 "const_int_operand"      "i,i"))
          (const_int 0))
   )
  ]
  "(pic30_isa32_target())"
  "btst.l %0,#%1"
  [
    (set_attr "cc" "set")
    (set_attr "type" "etc,use")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "bittstsiR_and_32"
  [(set (reg:CC_Z CC_REG)
        (compare 
          (and 
            (match_operand:SI 0 "pic30_reg_or_R_operand"  "r,R")
            (match_operand    1 "const_int_operand"       "i,i"))
          (const_int 0))
   )
  ]
  "(pic30_isa32_target()) &&
   (pic30_one_bit_set(GET_MODE(operands[0]),operands[1],1))"
  "btst.l %0,#%b1"
  [
    (set_attr "cc" "set")
    (set_attr "type" "etc,use")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "bittstsir_32"
  [(set (reg:CC_Z CC_REG) 
        (compare
          (zero_extract 
            (match_operand:SI 0 "pic30_register_operand" "r")
            (const_int 1)
            (match_operand    1    "const_int_operand"   "i"))
          (const_int 0))
   )
  ]
  "(pic30_isa32_target())"
  "btst.l %0,#%1"
  [
    (set_attr "cc" "set")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "*bittstsi_sfr_32"
  [(set (reg:CC CC_REG) 
        (compare 
          (zero_extract (match_operand:SI 0 "pic30_near_operand" "U")
                        (const_int 1)
                        (match_operand    1 "const_int_operand"  "i"))
          (const_int 0))
   )
  ]
  "(pic30_isa32_target())"
  "btst.b %0+%1/8,#%1%%8"
  [
    (set_attr "cc" "set")
    (set_attr "op_type" "alu")
  ]
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; AND
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;
;; quarter integer
;;;;;;;;;;;;;;;;;;

(define_insn "andqi3_helper_32"
  [(set (match_operand:QI 0 "pic30_mode2_operand"
               "=r<>,r<>,R,R,  rR<>,rR<>,r<>,R,  rR<>,rR<>")
        (and:QI 
           (match_operand:QI 1 "pic30_mode1PN_operand"
               "%r,  r,  r,r,  rR<>,rR<>,R<>,R<>,P,   N")
           (match_operand:QI 2 "pic30_mode1PN_operand"
               "r,   R<>,r,R<>,P,   N,   r,  r,  rR<>,rR<>")))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "@
   and.b %1,%2,%0
   and.b %1,%2,%0
   and.b %1,%2,%0
   and.b %1,%2,%0
   and.b %1,#(%H2&0x7F),%0
   and1.b %1,#(%2&0x7F),%0
   and.b %2,%1,%0
   and.b %2,%1,%0
   and.b %2,#(%H1&0x7F),%0
   and1.b %2,#(%1&0x7F),%0"
  [
   (set_attr "cc" "math")
   (set_attr "type" "def,defuse,use,use,def,use,defuse,use,def,use")
    (set_attr "op_type" "alu")
  ]
)

(define_expand "andqi3_32"
  [(set (match_operand:QI 0 "pic30_register_operand" "")
        (and:QI 
          (match_operand:QI 1 "pic30_reg_or_imm_operand" "")
          (match_operand:QI 2 "pic30_reg_or_imm_operand" "")))]
  "(pic30_isa32_target())"
  " 
   if (!pic30_mode2_operand(operand0,GET_MODE(operand0))) {
     operand0 = force_reg(GET_MODE(operand0),operand0);
   }
   if (!pic30_mode1PN_operand(operand1,GET_MODE(operand0))) {
     operand1 = force_reg(GET_MODE(operand0),operand1);
   }
   if (!pic30_mode1PN_operand(operand2,GET_MODE(operand0))) {
     operand2 = force_reg(GET_MODE(operand0),operand1);
   }
   emit(
     gen_andqi3_helper_32(operand0,operand1,operand2)
   );
   DONE;
  "
)

(define_insn "*andqi3_sfr0_32"
  [(set (match_operand:QI   0 "pic30_register_operand" "=r,r")
        (and:QI 
          (match_operand:QI 1 "pic30_register_operand" "%0,r")
          (match_operand:QI 2 "pic30_near_operand"     " U,U")))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "@
   and.b %2,%0
   mov.bz %1,%0\;and.b %2,%0"
  [
   (set_attr "cc" "set")
   (set_attr "type" "def")
   (set_attr "op_type" "alu")
  ]
)

(define_insn "*andqi3_sfr2_32"
  [(set (match_operand:QI   0 "pic30_near_operand"     "=U")
        (and:QI 
          (match_operand:QI 1 "pic30_register_operand" "%r")
          (match_operand:QI 2 "pic30_near_operand"     "0")))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "and.b %0,%1,WREG"
  [
   (set_attr "cc" "set")
   (set_attr "type" "etc")
   (set_attr "op_type" "alu")
  ]
)

;;;;;;;;;;;;;;;
;; half integer
;;;;;;;;;;;;;;;

(define_insn "andhi3_helper_32"
  [(set (match_operand:HI 0 "pic30_mode2_operand"
              "=r<>,r<>,R,R,  R<>, r,rR<>,R,  r<>,R,   r<>, rR<>")
        (and:HI 
          (match_operand:HI 1 "pic30_mode1PN_operand"
              "%r,  r,  r,r,  rR<>,r,rR<>,R<>,R<>,P,   P,   N")
          (match_operand:HI 2 "pic30_mode1PN_operand"
              "r,   R<>,r,R<>,P,   P,N,   r,  r,  rR<>,rR<>,rR<>")))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "@
   and.w %1,%2,%0
   and.w %1,%2,%0
   and.w %1,%2,%0
   and.w %1,%2,%0
   and.w %1,#(%H2&0x7F),%0
   and.w %1,#(%H2&0x7F),%0
   and1.w %1,#(%2&0x7F),%0
   and.w %2,%1,%0
   and.w %2,%1,%0
   and.w %2,#(%H1&0x7F),%0
   and.w %2,#(%H1&0x7F),%0
   and1.w %2,#(%1&0x7F),%0"
  [
   (set_attr "cc" "math")
   (set_attr "type" "def,defuse,use,use,def,use,etc,use,defuse,use,def,etc")
    (set_attr "op_type" "alu")
  ]
)

(define_expand "andhi3_32"
  [(set (match_operand:HI 0 "pic30_mode2_operand"
              "=r<>,r<>,R,R,  R<>,r,R,  r<>,R,r<>")
        (and:HI
          (match_operand:HI 1 "pic30_mode1P_or_1bitclr_APSV_operand"
              "%r,  r,  r,r,  r,  r,R<>,R<>,P,P")
          (match_operand:HI 2 "pic30_mode1P_or_1bitclr_APSV_operand"
              "r,  R<>,r,R<>,P,  P,r,  r,  r,r")))]
  "(pic30_isa32_target())"
  "
{
  if (pic30_one_bit_set(GET_MODE(operands[0]),operands[1],0)) {
    if (!pic30_mode2_operand(operands[2], GET_MODE(operands[2]))) {
      operands[2] = force_reg(GET_MODE(operands[2]), operands[2]);
    }
    emit(
      gen_bitclrhi_32(operands[0], operands[2], operands[1])
    );
  } else if (pic30_one_bit_set(GET_MODE(operands[0]),operands[2],0)) {
    if (!pic30_mode2_operand(operands[1], GET_MODE(operands[1]))) {
      operands[1] = force_reg(GET_MODE(operands[1]), operands[1]);
    }
    emit(
      gen_bitclrhi_32(operands[0], operands[1], operands[2])
    );
  } else if (pic30_mode1P_operand(operands[1],GET_MODE(operands[1])) &&
             pic30_mode1P_operand(operands[2],GET_MODE(operands[2]))) {
    emit(
      gen_andhi3_helper_32(operands[0],operands[1],operands[2])
    );
  } else if (immediate_operand(operands[1],VOIDmode) && 
             satisfies_constraint_J(operands[1])) {
    int intval = INTVAL(operands[1]) & 0xFFFF;
    emit(
      gen_andhi3_J_32(operands[0],operands[2],GEN_INT(intval))
    );
  } else if (immediate_operand(operands[2],VOIDmode) && 
             satisfies_constraint_J(operands[2])) {
    int intval = INTVAL(operands[2]) & 0xFFFF;
    emit(
      gen_andhi3_J_32(operands[0],operands[1],GEN_INT(intval))
    );
  } else if (pic30_reg_or_near_operand(operands[1],GET_MODE(operands[0])) &&
             pic30_reg_or_near_operand(operands[2],GET_MODE(operands[1])) &&
             pic30_register_operand(operands[2],GET_MODE(operands[2]))) {
    emit(
      gen_andhi3_sfr_32(operands[0],operands[1],operands[2])
    );
  } else {
    rtx xop1, xop2;

    xop1 = force_reg(GET_MODE(operands[0]),operands[1]);
    xop2 = force_reg(GET_MODE(operands[0]),operands[2]);
    emit (
      gen_andhi3_helper_32(operands[0],xop1, xop2)
    );
  }
  DONE;
}")

(define_insn "andhi3_sfr_32"
  [(set (match_operand:HI   0 "pic30_reg_or_near_operand" "=U,U,r,r,r")
        (and:HI
          (match_operand:HI 1 "pic30_reg_or_near_operand" "%0,r,U,U,r")
          (match_operand:HI 2 "pic30_register_operand"    " r,r,0,r,r"))
   )
   (clobber 
          (match_scratch:HI 3                             "=X,r,X,r,X"))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "@
   and.w %0,%2
   and.w %1,%2,%3\;mov.w %3,%0
   and.w %1,%0,%0
   mov.w %1,%3\;and.w %2,%3,%0
   and.w %1,%2,%0"
  [
    (set_attr "cc" "math")
    (set_attr "type" "etc,etc,def,def,def")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "andhi3_J_32"
  [(set (match_operand:HI 0 "pic30_register_operand"         "=r")
        (and:HI (match_operand:HI 1 "pic30_register_operand" "%0")
                (match_operand:HI 2 "pic30_J_operand"        "J")))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "and.w #(%H2&0xFFFF),%0"
  [
   (set_attr "cc" "math")
   (set_attr "type" "def")
    (set_attr "op_type" "alu")
  ]
)

;;;;
;;    gcc will FORCE a reload for a memory operation if it is a SUBREG -
;;    if the pattern doesn't have a register option, then reload will fail
;;    even though some machines (like dsPIC) can read a byte or hi mode just
;;    fine
;;;;
(define_insn "*andhi3_sfr2_32"
  [(set (match_operand:HI         0 "pic30_register_operand"  "=r,  r, r")
        (and:HI (match_operand:HI 1 "pic30_register_operand"  " 0,  r, r")
                (match_operand:HI 2 "pic30_reg_or_near_operand"
                                                              " U,  U, r")))
                (clobber (match_scratch:HI 3                  "=X,  &r,X"))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "@
   and %2,%0,WREG
   mov %2,%3\;and %3,%1,WREG
   and %1,%2,WREG"
  [
   (set_attr "cc" "set")
   (set_attr "type" "def")
    (set_attr "op_type" "alu")
  ]
)

;;;;;;;;;;;;;;;;;
;; single integer
;;;;;;;;;;;;;;;;;

(define_expand "andsi3_32"
  [(set (match_operand:SI 0 "pic30_mode2_operand"
              "=r<>,r<>,R,R,  R<>,r,R,  r<>,R,r<>")
        (and:SI
          (match_operand:SI 1 "pic30_mode1P_or_1bitclr_APSV_operand"
              "%r,  r,  r,r,  r,  r,R<>,R<>,P,P")
          (match_operand:SI 2 "pic30_mode1P_or_1bitclr_APSV_operand"
              "r,  R<>,r,R<>,P,  P,r,  r,  r,r")))]
  "(pic30_isa32_target())"
  "
{
 if (pic30_one_bit_set(GET_MODE(operands[0]),operands[1],0)) {
    rtx r0,r1;

    r0 = force_reg(E_SImode, operands[0]);
    r1 = force_reg(E_SImode, operands[2]);
    emit(
      gen_bitclrsi_32(r0, r1, operands[1])
    );
    emit_move_insn(operands[0],r0);
  } else if (pic30_one_bit_set(GET_MODE(operands[0]),operands[2],0)) {
    rtx r0,r1;

    r0 = force_reg(E_SImode, operands[0]);
    r1 = force_reg(E_SImode, operands[1]);
    emit(
      gen_bitclrsi_32(r0, r1, operands[2])
    );
    emit_move_insn(operands[0],r0);
  } else if (pic30_mode1P_operand(operands[1],GET_MODE(operands[1])) &&
             pic30_mode1P_operand(operands[2],GET_MODE(operands[2]))) {
    emit(
      gen_andsi3_helper_32(operands[0],operands[1],operands[2])
    );
  } else if (pic30_mode1P_operand(operands[2],GET_MODE(operands[2])) &&
             immediate_operand(operands[1],VOIDmode) &&
             satisfies_constraint_J(operands[1])) {
    emit(     
      gen_andsi3_J_32(operands[0],operands[2],operands[1])
    );        
  } else if (pic30_mode1P_operand(operands[1],GET_MODE(operands[2])) &&
             immediate_operand(operands[2],VOIDmode) &&
             satisfies_constraint_J(operands[2])) {
    emit(
      gen_andsi3_J_32(operands[0],operands[1],operands[2])
    );
  } else if (pic30_reg_or_near_operand(operands[1],GET_MODE(operands[1])) &&
             pic30_reg_or_near_operand(operands[2],GET_MODE(operands[2])))  {
    emit(
      gen_andsi3_sfr_32(operands[0],operands[1],operands[2])
    );
  } else {
    rtx xop1, xop2;
      
    xop1 = force_reg(GET_MODE(operands[0]),operands[1]);
    xop2 = force_reg(GET_MODE(operands[0]),operands[2]);
    emit (
      gen_andsi3_helper_32(operands[0],xop1, xop2)
    );
  }
  DONE;
}")

(define_insn "andsi3_helper_32"
  [(set (match_operand:SI 0 "pic30_mode2_operand"
              "=r<>,r<>,R,R,  R<>,r,R,  r<>,R,r<>")
        (and:SI
          (match_operand:SI 1 "pic30_mode1P_operand"
              "%r,  r,  r,r,  r,  r,R<>,R<>,P,P")
          (match_operand:SI 2 "pic30_mode1P_operand"
              "r,  R<>,r,R<>,P,  P,r,  r,  r,r")))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "@
   and.l %1,%2,%0
   and.l %1,%2,%0
   and.l %1,%2,%0
   and.l %1,%2,%0
   and.l %1,#(%H2&0x7F),%0
   and.l %1,#(%H2&0x7F),%0
   and.l %2,%1,%0
   and.l %2,%1,%0
   and.l %2,#(%H1&0x7F),%0
   and.l %2,#(%H1&0x7F),%0"
  [
   (set_attr "cc" "math")
   (set_attr "type" "def,defuse,use,use,def,use,use,defuse,use,def")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "andsi3_sfr_32"
  [(set (match_operand:SI   0 "pic30_reg_or_near_operand" "=U,U,r,r,r")
        (and:SI
          (match_operand:SI 1 "pic30_reg_or_near_operand" "%0,r,U,U,r")
          (match_operand:SI 2 "pic30_register_operand"    " r,r,0,r,r"))
   )
   (clobber
          (match_scratch:HI 3                             "=X,r,X,X,X"))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "@
   and.l %0,%2
   and.l %1,%2,%3\;mov.l %3,%0
   and.l %1,%0,WREG
   mov.l %1,%0\;and.l %2,%0,%0
   and.l %1,%2,%0"
  [
    (set_attr "cc" "math")
    (set_attr "type" "etc,etc,def,def,def")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "andsi3_J_32"
  [(set (match_operand:SI 0 "pic30_register_operand"         "=r")
        (and:SI (match_operand:SI 1 "pic30_register_operand" "%0")
                (match_operand:SI 2 "pic30_J_operand"        "J")))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "and.l #(%H2&0xFFFF),%0"
  [
   (set_attr "cc" "math")
   (set_attr "type" "def")
    (set_attr "op_type" "alu")
  ]
)


;;;;;;;;;;;;;;;;;
;; double integer
;;;;;;;;;;;;;;;;;

(define_insn "andhidi3_se_32"
  [(set (match_operand:DI 0 "pic30_DI_mode2_operand"      "=r,>,>,>,&r,R,R,R,&r")
        (and:DI
           (sign_extend:DI
              (match_operand:HI 1 "pic30_register_operand" "r,r,r,r, r,r,r,r, r"))
           (match_operand:DI 2 "pic30_DI_mode2_operand"    "r,r,0,>, >,r,0,R, R")))
   (clobber (match_scratch:SI 3                   "=&r,&r,&r,&r,&r,&r,&r,&r,&r"))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "*
{
   const char *noerrata_patterns[] = {
     /* r,r,r */          \"se.w %1,%3\;and.l %2,%3,%0\;\"
                              \"asr %3,#31,%3\;and.l %3,%d2,%d0\",

     /* >,r,r */          \"se.w %1,%3\;and.l %2,%3,%0\;\"
                              \"asr %3,#31,%3\;and.l %3,%d2,%0\",

     /* >,r,0 */          \"se.w %1,%3\;and.l %3,%s2,%0\;\"
                              \"asr %3,#31,%3\;and.l %3,%s2,%0\",

     /* >,r,> */          \"se.w %1,%3\;and.l %3,%2,%0\;\"
                              \"asr %3,#31,%3\;and.l %3,%2,%0\",

     /* r,r,> */          \"se.w %1,%3\;and.l %3,%2,%0\;\"
                              \"asr %3,#31,%3\;and.l %3,%2,%d0\",

     /* R,r,r */          \"se.w %1,%3\;and.l %3,%2,%I0\;\"
                              \"asr %3,#31,%3\;and.l %3,%d2,%D0\",

     /* R,r,0 */          \"se.w %1,%3\;and.l %3,%2,%I0\;\"
                              \"asr %3,#31,%3\;and.l %3,%2,%D0\",

     /* R,r,R */          \"se.w %1,%3\;and.l %3,%I2,%I0\;\"
                              \"asr %3,#31,%3\;and.l %3,%D2,%D0\",

     /* r,r,R */          \"se.w %1,%3\;and.l %3,%I2,%0\;\"
                              \"asr %3,#31,%3\;and.l %3,%D2,%d0\"
  };

  /* increasing the patterns, means increasing this number too */
  const char **patterns;

  patterns = noerrata_patterns;
  return patterns[which_alternative];
}"
   [
     (set_attr "cc" "math,math,math,math,math,clobber,clobber,clobber,clobber")
     (set_attr "type" "def")
     (set_attr "op_type" "alu")
  ]
)

(define_insn "andhidi3_ze_32"
  [(set (match_operand:DI 0 "pic30_DI_mode2_operand"      "=r,>,>,>,&r,R,R,R,&r")
        (and:DI
           (zero_extend:DI
              (match_operand:HI 1 "pic30_register_operand" "r,r,r,r, r,r,r,r, r"))
           (match_operand:DI 2 "pic30_DI_mode2_operand"    "r,r,0,>, >,r,0,R, R"))
   )
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "*
{
   const char *noerrata_patterns[] = {
     /* r,r,r */          \"and.l %1,%2,%0\;clr.l %d0\",

     /* >,r,r */          \"and.l %1,%2,%0\;clr.l %0\",

     /* >,r,0 */          \"and.l %1,%s2,%0\;clr.l %0\",

     /* >,r,> */          \"and.l %1,%2,%0\;clr.l %0\",

     /* r,r,> */          \"and.l %1,%2,%0\;clr.l %d0\",

     /* R,r,r */          \"and.l %1,%2,%I0\;clr.l %D0\",

     /* R,r,0 */          \"and.l %1,%2,%I0\;clr.l %D0\",

     /* R,r,R */          \"and.l %1,%2,%I0\;clr.l %D0\",

     /* r,r,R */          \"and.l %1,%2,%0\;clr.l %d0\"
  };

  /* increasing the patterns, means increasing this number too */
  static char szInsns[180];
  const char **patterns;
  rtx x;

  patterns = noerrata_patterns;

  switch (which_alternative) {
     default: return patterns[which_alternative];

     case 3:  /* >,r,> */
              strcpy(szInsns, patterns[which_alternative]);

              x = XEXP(operands[2],0);
	      if (!pic30_dead_or_set_p(NEXT_INSN(insn), x) &&
                  !find_regno_note(insn, REG_UNUSED, REGNO(x))) {
                 strcat(szInsns,\"\;add.l #4,%r2\");
              }
              return szInsns;

     case 4:  /* r,r,> */
              strcpy(szInsns, patterns[which_alternative]);

              x = XEXP(operands[2],0);
	      if (!pic30_dead_or_set_p(NEXT_INSN(insn), x) &&
                  !find_regno_note(insn, REG_UNUSED, REGNO(x))) {
                 strcat(szInsns,\"\;add.l #4,%r2\");
              }
              return szInsns;
   }
}"
   [
     (set_attr "cc" "math,math,math,math,math,clobber,clobber,clobber,clobber")
     (set_attr "type" "def")
     (set_attr "op_type" "alu")
  ]
)

(define_insn "andsidi3_se_32"
  [(set (match_operand:DI 0 "pic30_DI_mode2_operand"      "=r,>,>,>,&r,R,R,R,&r")
        (and:DI
           (sign_extend:DI
              (match_operand:SI 1 "pic30_register_operand" "r,r,r,r, r,r,r,r, r"))
           (match_operand:DI 2 "pic30_DI_mode2_operand"    "r,r,0,>, >,r,0,R, R")))
   (clobber (match_scratch:SI 3                   "=&r,&r,&r,&r,&r,&r,&r,&r,&r"))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "*
{
  const char *noerrata_patterns[] = {
     /* r,r,r */        \"asr.l %1,#31,%3\;and.l %1,%2,%0\;and.l %3,%d2,%d0\",

     /* >,r,r */        \"asr.l %1,#31,%3\;and.l %1,%2,%0\;and.l %3,%d2,%0\",

     /* >,r,0 */        \"asr.l %1,#31,%3\;and.l %1,%s2,%0\;and.l %3,%s2,%0\",

     /* >,r,> */        \"asr.l %1,#31,%3\;and.l %1,%2,%0\;and.l %3,%2,%0\",

     /* r,r,> */        \"asr.l %1,#31,%3\;and.l %1,%2,%0\;and.l %3,%2,%d0\",

     /* R,r,r */        \"asr.l %1,#31,%3\;and.l %1,%2,%I0\;and.l %3,%d2,%D0\",

     /* R,r,0 */        \"asr.l %1,#31,%3\;and.l %1,%2,%I0\;and.l %3,%2,%D0\",

     /* R,r,R */        \"asr.l %1,#31,%3\;and.l %1,%I2,%I0\;and.l %3,%D2,%D0\",

     /* r,r,R */        \"asr.l %1,#31,%3\;and.l %1,%I2,%0\;and.l %3,%D2,%d0\"
  };

  /* increasing the patterns, means increasing this number too */
  const char **patterns;

  patterns = noerrata_patterns;
  return patterns[which_alternative];
}"
   [
     (set_attr "cc" "math,math,math,math,math,clobber,clobber,clobber,clobber")
     (set_attr "type" "def")
     (set_attr "op_type" "alu")
  ]
)

(define_insn "andsidi3_ze_32"
  [(set (match_operand:DI 0 "pic30_DI_mode2_operand"      "=r,>,>,>,&r,R,R,R,&r")
        (and:DI
           (zero_extend:DI
              (match_operand:SI 1 "pic30_register_operand" "r,r,r,r, r,r,r,r, r"))
           (match_operand:DI 2 "pic30_DI_mode2_operand"    "r,r,0,>, >,r,0,R, R"))
   )
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "*
{
   const char *noerrata_patterns[] = {
     /* r,r,r */          \"and.l %1,%2,%0\;clr.l %d0\",

     /* >,r,r */          \"and.l %1,%2,%0\;clr.l %0\",

     /* >,r,0 */          \"and.l %1,%s2,%0\;clr.l %0\",

     /* >,r,> */          \"and.l %1,%2,%0\;clr.l %0\",

     /* r,r,> */          \"and.l %1,%2,%0\;clr.l %d0\",

     /* R,r,r */          \"and.l %1,%2,%I0\;clr.l %D0\",

     /* R,r,0 */          \"and.l %1,%2,%I0\;clr.l %D0\",

     /* R,r,R */          \"and.l %1,%I2,%I0\;clr.l %D0\",

     /* r,r,R */          \"and.l %1,%I2,%0\;clr.l %d0\"
  };

  /* increasing the patterns, means increasing this number too */
  static char szInsns[180];
  const char **patterns;
  rtx x;

  patterns = noerrata_patterns;

  switch (which_alternative) {
     default: return patterns[which_alternative];

     case 3:  /* >,r,> */
              strcpy(szInsns, patterns[which_alternative]);

              x = XEXP(operands[2],0);
              if (!pic30_dead_or_set_p(NEXT_INSN(insn), x) &&
                  !find_regno_note(insn, REG_UNUSED, REGNO(x))) {
                 strcat(szInsns,\"\;add.l #4,%r2\");
              }
              return szInsns;

     case 4:  /* r,r,> */
              strcpy(szInsns, patterns[which_alternative]);

              x = XEXP(operands[2],0);
              if (!pic30_dead_or_set_p(NEXT_INSN(insn), x) &&
                  !find_regno_note(insn, REG_UNUSED, REGNO(x))) {
                 strcat(szInsns,\"\;add.l #4,%r2\");
              }
              return szInsns;

   }
}"
   [
     (set_attr "cc" "math,math,math,math,math,clobber,clobber,clobber,clobber")
     (set_attr "type" "def")
     (set_attr "op_type" "alu")
  ]
)

(define_insn "anddihi3_se_32"
  [(set (match_operand:DI 0 "pic30_DI_mode2_operand"      "=r,>,>,>,&r,R,R,R,&r")
        (and:DI
           (match_operand:DI 1 "pic30_DI_mode2_operand"    "r,r,0,>, >,r,0,R, R")
           (sign_extend:DI
              (match_operand:HI 2 "pic30_register_operand" "r,r,r,r, r,r,r,r, r"))
        )
    )
   (clobber (match_scratch:SI 3                   "=&r,&r,&r,&r,&r,&r,&r,&r,&r"))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "*
{
   const char *noerrata_patterns[] = {
     /* r,r,r */          \"se.w %2,%3\;and.l %3,%1,%0\;\"
                              \"asr.l %3,#31,%3\;and.l %3,%d1,%d0\",

     /* >,r,r */          \"se.w %2,%3\;and.l %3,%1,%0\;\"
                              \"asr.l %3,#31,%3\;and.l %3,%d1,%0\",

     /* >,0,r */          \"se.w %2,%3\;and.l %3,%s1,%0\;\"
                              \"asr.l %3,#31,%3\;and.l %3,%s1,%0\",

     /* >,>,r */          \"se.w %2,%3\;and.l %3,%1,%0\;\"
                              \"asr.l %3,#31,%3\;and.l %3,%1,%0\",

     /* r,>,r */          \"se.w %2,%3\;and.l %3,%1,%0\;\"
                              \"asr.l %3,#31,%3\;and.l %3,%1,%d0\",

     /* R,r,r */          \"se.w %2,%3\;and.l %3,%1,%I0\;\"
                              \"asr.l %3,#31,%3\;and.l %3,%d1,%D0\",

     /* R,0,r */          \"se.w %2,%3\;and.l %3,%1,%I0\;\"
                              \"asr.l %3,#31,%3\;and.l %3,%d1,%D0\",

     /* R,R,r */          \"se.w %2,%3\;and.l %3,%I1,%I0\;\"
                              \"asr.l %3,#31,%3\;and.l %3,%D1,%D0\",

     /* r,R,r */          \"se.w %2,%3\;and.l %3,%I1,%0\;\"
                              \"asr.l %3,#31,%3\;and.l %3,%D1,%d0\"
  };

  const char **patterns;

  patterns = noerrata_patterns;
  return patterns[which_alternative];
}"
   [
     (set_attr "cc" "clobber")
     (set_attr "type" "def")
     (set_attr "op_type" "alu")
  ]
)

(define_insn "anddihi3_ze_32"
  [(set (match_operand:DI 0 "pic30_DI_mode2_operand"      "=r,>,>,>,&r,R,R,R,&r")
        (and:DI
           (match_operand:DI 1 "pic30_DI_mode2_operand"    "r,r,0,>, >,r,0,R, R")
           (zero_extend:DI
              (match_operand:HI 2 "pic30_register_operand" "r,r,r,r, r,r,r,r, r"))
        )
   )
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "*
{
   const char *patterns[] = {
     /* r,r,r */          \"and.l %1,%2,%0\;clr.l %d0\",

     /* >,r,r */          \"and.l %1,%2,%0\;clr.l %0\",

     /* >,0,r */          \"and.l %2,%s1,%0\;clr.l %0\",

     /* >,>,r */          \"and.l %2,%1,%0\;clr.l %0\",

     /* r,>,r */          \"and.l %2,%1,%0\;clr.l %d0\",

     /* R,r,r */          \"and.l %1,%2,%I0\;clr.l %D0\",

     /* R,0,r */          \"and.l %2,%1,%I0\;clr.l %D0\",

     /* R,R,r */          \"and.l %2,%1,%I0\;clr.l %D0\",

     /* r,R,r */          \"and.l %2,%1,%0\;clr.l %d0\",
  };

  /* increasing the patterns, means increasing this number too */
  static char szInsns[160];
  rtx x;

  switch (which_alternative) {
     default: return patterns[which_alternative];

     case 3:  /* >,>,r */
              strcpy(szInsns, patterns[which_alternative]);

              x = XEXP(operands[2],0);
	      if (!pic30_dead_or_set_p(NEXT_INSN(insn), x) &&
                  !find_regno_note(insn, REG_UNUSED, REGNO(x))) {
                 strcat(szInsns,\"\;add.l #4,%r1\");
              }
              return szInsns;

     case 4:  /* r,>,r */
              strcpy(szInsns, patterns[which_alternative]);

              x = XEXP(operands[2],0);
	      if (!pic30_dead_or_set_p(NEXT_INSN(insn), x) &&
                  !find_regno_note(insn, REG_UNUSED, REGNO(x))) {
                 strcat(szInsns,\"\;add.l #4,%r1\");
              }
              return szInsns;
   }
}"
   [
     (set_attr "cc" "clobber")
     (set_attr "type" "def")
     (set_attr "op_type" "alu")
  ]
)

(define_insn "anddisi3_se_32"
  [(set (match_operand:DI 0 "pic30_DI_mode2_operand"      "=r,>,>,>,&r,R,R,R,&r")
        (and:DI
           (match_operand:DI 1 "pic30_DI_mode2_operand"    "r,r,0,>, >,r,0,R, R")
           (sign_extend:DI
              (match_operand:SI 2 "pic30_register_operand" "r,r,r,r, r,r,r,r, r"))
        )
   )
   (clobber (match_scratch:SI 3                   "=&r,&r,&r,&r,&r,&r,&r,&r,&r"))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "*
{
   const char *noerrata_patterns[] = {
     /* r,r,r */    \"asr.l %2,#31,%3\;and.l %2,%1,%0\;and.l %3,%d1,%d0\",

     /* >,r,r */    \"asr.l %2,#31,%3\;and.l %2,%1,%0\;and.l %3,%d1,%0\",

     /* >,0,r */    \"asr.l %2,#31,%3\;and.l %2,%s1,%0\;and.l %3,%s1,%0\",

     /* >,>,r */    \"asr.l %2,#31,%3\;and.l %2,%1,%0\;and.l %3,%1,%0\",

     /* r,>,r */    \"asr.l %2,#31,%3\;and.l %2,%1,%0\;and.l %3,%1,%d0\",

     /* R,r,r */    \"asr.l %2,#31,%3\;and.l %2,%1,%I0\;and.l %3,%d1,%D0\",

     /* R,0,r */    \"asr.l %2,#31,%3\;and.l %2,%1,%I0\;and.l %3,%1,%D0\",

     /* R,R,r */    \"asr.l %2,#31,%3\;and.l %2,%I1,%I0\;and.l %3,%D1,%D0\",

     /* r,R,r */    \"asr.l %2,#31,%3\;and.l %2,%I1,%0\;and.l %3,%D1,%d0\"
  };

  /* increasing the patterns, means increasing this number too */
  const char **patterns;

  patterns = noerrata_patterns;
  return patterns[which_alternative];
}"
   [
     (set_attr "cc" "clobber")
     (set_attr "type" "def")
     (set_attr "op_type" "alu")
  ]
)

(define_insn "anddisi3_ze_32"
  [(set (match_operand:DI 0 "pic30_DI_mode2_operand"         "=r,>,>,>,&r,R,R,R,&r")
        (and:DI
           (match_operand:DI 1 "pic30_DI_mode2_operand"    "r,r,0,>, >,r,0,R, R")
           (zero_extend:DI
              (match_operand:SI 2 "pic30_register_operand" "r,r,r,r, r,r,r,r, r"))
        )
   )
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "*
{
  const char *noerrata_patterns[] = {
     /* r,r,r */          \"and.l %1,%2,%0\;clr.l %d0\",
  
     /* >,r,r */          \"and.l %1,%2,%0\;clr.l %0\",

     /* >,0,r */          \"and.l %2,%s1,%0\;clr.l %0\",
                              
     /* >,>,r */          \"and.l %2,%1,%0\;clr.l %0\",
     
     /* r,>,r */          \"and.l %2,%1,%0\;clr.l %d0\",
     
     /* R,r,r */          \"and.l %1,%2,%I0\;clr.l %D0\",
                              
     /* R,0,r */          \"and.l %2,%1,%I0\;clr.l %D0\",
     
     /* R,R,r */          \"and.l %2,%1,%I0\;clr.l %D0\",
                          
     /* r,R,r */          \"and.l %2,%1,%0\;clr.l %d0\"
  };

  /* increasing the patterns, means increasing this number too */
  static char szInsns[170];
  const char **patterns;
  rtx x;

  patterns = noerrata_patterns;

  switch (which_alternative) {
     default: return patterns[which_alternative];

     case 3:  /* >,>,r */
              strcpy(szInsns, patterns[which_alternative]);

              x = XEXP(operands[1],0);
              if (!pic30_dead_or_set_p(NEXT_INSN(insn), x) &&
                  !find_regno_note(insn, REG_UNUSED, REGNO(x))) {
                 strcat(szInsns,\"\;add.l #4,%r1\");
              }
              return szInsns;

     case 4:  /* r,>,r */
              strcpy(szInsns, patterns[which_alternative]);

              x = XEXP(operands[1],0);
              if (!pic30_dead_or_set_p(NEXT_INSN(insn), x) &&
                  !find_regno_note(insn, REG_UNUSED, REGNO(x))) {
                 strcat(szInsns,\"\;add.l #4,%r1\");
              }
              return szInsns;

   }
}"
   [
     (set_attr "cc" "clobber")
     (set_attr "type" "def")
     (set_attr "op_type" "alu")
  ]
)

(define_insn "anddi3_32"
  [(set (match_operand:DI 0 "pic30_DI_mode2_operand"       "=r,>,>,>,&r,R,R,R,&r")
        (and:DI
           (match_operand:DI 1 "pic30_register_operand"    "r,r,r,r, r,r,r,r, r")
           (match_operand:DI 2 "pic30_DI_mode2_operand"    "r,r,0,>, >,r,0,R, R")))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "*
{
   const char *noerrata_patterns[] = {
     /* r,r,r */          \"and.l %1,%2,%0\;and.l %d1,%d2,%d0\",

     /* >,r,r */          \"and.l %1,%2,%0\;and.l %d1,%d2,%0\",

     /* >,r,0 */          \"and.l %1,%s2,%0\;and.l %d1,%s2,%0\",

     /* >,r,> */          \"and.l %1,%2,%0\;and.l %d1,%2,%0\",

     /* r,r,> */          \"and.l %1,%2,%0\;and.l %d1,%2,%d0\",

     /* R,r,r */          \"and.l %1,%2,%I0\;and.l %d1,%d2,%D0\",

     /* R,r,0 */          \"and.l %1,%2,%I0\;and.l %d1,%2,%D0\",

     /* R,r,R */          \"and.l %1,%I2,%I0\;and.l %d1,%D2,%D0\",

     /* r,r,R */          \"and.l %1,%I2,%0\;and.l %d1,%D2,%d0\"
  };

  /* increasing the patterns, means increasing this number too */
  const char **patterns;
 
  patterns = noerrata_patterns;
  return patterns[which_alternative];
}"
   [
     (set_attr "cc" "clobber")
     (set_attr "type" "def")
     (set_attr "op_type" "alu")
  ]
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; OR
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;
;; quarter integer
;;;;;;;;;;;;;;;;;;

(define_insn "*iorqi3_imm_32"
  [(set (match_operand:QI 0 "pic30_register_operand"         "=r")
        (ior:QI (match_operand:QI 1 "pic30_register_operand" "%0")
                (match_operand:QI 2 "pic30_J_operand"   "J")))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "ior.b #(%H2&0xFF),%0"
  [
   (set_attr "cc" "math")
   (set_attr "type" "def")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "iorqi3_gen_32"
  [(set (match_operand:QI 0 "pic30_mode2_operand"
              "=r<>,r<>,R,R,  r<>,R,r<>,R,  r<>,R")
        (ior:QI 
           (match_operand:QI 1 "pic30_mode1P_operand"
              "%r,  r,  r,r,  r,  r,R<>,R<>,P,  P")
           (match_operand:QI 2 "pic30_mode1P_operand"
              "r,  R<>,r,R<>,P,  P,r,  r,  r,  r")))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "@
   ior.b %1,%2,%0
   ior.b %1,%2,%0
   ior.b %1,%2,%0
   ior.b %1,%2,%0
   ior.b %1,#(%H2&0x7F),%0
   ior.b %1,#(%H2&0x7F),%0
   ior.b %2,%1,%0
   ior.b %2,%1,%0
   ior.b %2,#(%H1&0x7F),%0
   ior.b %2,#(%H1&0x7F),%0"
  [
   (set_attr "cc" "math")
   (set_attr "type" "def,defuse,use,use,def,use,defuse,use,def,use")
    (set_attr "op_type" "alu")
  ]
)

(define_expand "iorqi3_32"
  [(set (match_operand:QI 0 "pic30_mode2_operand"
              "=r<>,r<>,R,R,  r<>,R,r<>,R,  r<>,R")
        (ior:QI
           (match_operand:QI 1 "pic30_mode1P_or_1bit_APSV_operand"
              "%r,  r,  r,r,  r,  r,R<>,R<>,P,  P")
           (match_operand:QI 2 "pic30_mode1P_or_1bit_APSV_operand"
              "r,  R<>,r,R<>,P,  P,r,  r,  r,  r")))]
  "(pic30_isa32_target())"
  "
{
  if (pic30_one_bit_set(GET_MODE(operands[0]),operands[1],1)) {
    emit(
      gen_bitsetqi_32(operands[0], operands[2], operands[1])
    );
  } else if (pic30_one_bit_set(GET_MODE(operands[0]),operands[2],1)) {
    emit(
      gen_bitsetqi_32(operands[0], operands[1], operands[2])
    );
  } else { 
    rtx op1 = operands[1];
    rtx op2 = operands[2];

    if (!pic30_mode1P_operand(op1,GET_MODE(op1))) {
       op1 = force_reg(GET_MODE(operands[0]), op1);
    }
    if (!pic30_mode1P_operand(op2,GET_MODE(op1))) {
       op2 = force_reg(GET_MODE(operands[0]), op2);
    }
    emit(
      gen_iorqi3_gen_32(operands[0],op1,op2)
    );
  }
  DONE;
}")

(define_insn "*iorqi3_sfr0_32"
  [(set (match_operand:QI 0 "pic30_register_operand"           "=r,&r")
        (ior:QI (match_operand:QI 1 "pic30_register_operand"   "%0,r")
                (match_operand:QI 2 "pic30_near_operand"       " U,U")))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "@
   ior.b %2,%0,WREG
   mov.bz %2,%0\;ior.b %1,%0,%0"
  [
   (set_attr "cc" "math")
   (set_attr "type" "def")
    (set_attr "op_type" "alu")
  ]
)

; leave this match_dup, operand 1 cannot interfere with reload (CAW)
 (define_insn "*iorqi3_sfr1_32"
   [(set (match_operand:QI   0 "pic30_near_operand"    "=U")
         (ior:QI 
           (match_dup        0)
           (match_operand:QI 1 "pic30_register_operand" "r")))
    (clobber (reg:CC CC_REG))
   ]
   "(pic30_isa32_target())"
   "ior.b %0, %1"
  [
    (set_attr "cc" "math")
    (set_attr "op_type" "alu")
  ]
)

;;;;;;;;;;;;;;;
;; half integer
;;;;;;;;;;;;;;;

; this match_can cause issues iff operand 1 is dies in this instruction and
;   we decide to use it to reload operand 0 (CAW)
(define_insn "iorhi3_sfr0_32"
  [(set (match_operand:HI 0         "pic30_reg_or_near_operand" "=U,r,r")
        (ior:HI (match_operand:HI 1 "pic30_register_operand"    "%r,0,r")
                (match_operand:HI 2 "pic30_reg_or_near_operand" " 0,U,r")))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "@
   ior.w %0,%1
   ior.w %2,%0,WREG
   ior.w %2,%1,%0"
  [
    (set_attr "cc" "math")
    (set_attr "type" "etc,def,def")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "iorhi3_helper_32"
  [(set (match_operand:HI 0 "pic30_mode2_operand"
              "=r<>,r<>,R,  R,r<>,R,r<>,R,  r<>,R")
        (ior:HI 
           (match_operand:HI 1 "pic30_mode1P_operand"
              "%r,  r,  r,  r,r,  r,R<>,R<>,P,  P")
           (match_operand:HI 2 "pic30_mode1P_operand"
              "R<>,r,  R<>,r,P,  P,r,  r,  r,  r")))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "@
   ior.w %1,%2,%0
   ior.w %1,%2,%0
   ior.w %1,%2,%0
   ior.w %1,%2,%0
   ior.w %1,#(%H2&0x7F),%0
   ior.w %1,#(%H2&0x7F),%0
   ior.w %2,%1,%0
   ior.w %2,%1,%0
   ior.w %2,#(%H1&0x7F),%0
   ior.w %2,#(%H1&0x7F),%0"
  [
   (set_attr "cc" "math")
   (set_attr "type" "defuse,def,use,use,def,use,defuse,use,def,use")
    (set_attr "op_type" "alu")
  ]
)

(define_expand "iorhi3_32"
  [(set (match_operand:HI 0 "pic30_mode2_operand"
              "=r<>,r<>,R,R,  R<>,r,R,  r<>,R,r<>")
        (ior:HI
          (match_operand:HI 1 "pic30_mode1P_or_1bitclr_APSV_operand"
              "%r,  r,  r,r,  r,  r,R<>,R<>,P,P")
          (match_operand:HI 2 "pic30_mode1P_or_1bitclr_APSV_operand"
              "r,  R<>,r,R<>,P,  P,r,  r,  r,r")))]
  "(pic30_isa32_target())"
  "
{ 
  if (pic30_one_bit_set(GET_MODE(operands[0]),operands[1],1)) {
    emit(
      gen_bitsethi_32(operands[0], operands[2], operands[1])
    );
  } else if (pic30_one_bit_set(GET_MODE(operands[0]),operands[2],1)) {
    emit(
      gen_bitsethi_32(operands[0], operands[1], operands[2])
    );
  } else if (pic30_mode1P_operand(operands[1],GET_MODE(operands[1])) &&
             pic30_mode1P_operand(operands[2],GET_MODE(operands[2]))) {
    emit(
      gen_iorhi3_helper_32(operands[0],operands[1],operands[2])
    );
  } else if (immediate_operand(operands[1],VOIDmode) && 
             satisfies_constraint_J(operands[1])) {
    int intval = INTVAL(operands[1]) & 0xFFFF;
    emit(
      gen_iorhi3_J_32(operands[0],operands[2],GEN_INT(intval))
    );
  } else if (immediate_operand(operands[2],VOIDmode) && 
             satisfies_constraint_J(operands[2])) {
    int intval = INTVAL(operands[2]) & 0xFFFF;
    emit(
      gen_iorhi3_J_32(operands[0],operands[1],GEN_INT(intval))
    );
  } else if (pic30_reg_or_near_operand(operands[1],GET_MODE(operands[1])) &&
             pic30_reg_or_near_operand(operands[2],GET_MODE(operands[2])))  {
    emit(
      gen_iorhi3_sfr_32(operands[0],operands[1],operands[2])
    );
  } else {
    rtx xop1, xop2;

    xop1 = force_reg(GET_MODE(operands[0]),operands[1]);
    xop2 = force_reg(GET_MODE(operands[0]),operands[2]);
    emit (
      gen_iorhi3_helper_32(operands[0],xop1, xop2)
    );
  }
  DONE;
}")

(define_insn "iorhi3_sfr_32"
  [(set (match_operand:HI   0 "pic30_reg_or_near_operand" "=U,U,r,&r,r")
        (ior:HI
          (match_operand:HI 1 "pic30_reg_or_near_operand" "%0,r,U, U,r")
          (match_operand:HI 2 "pic30_register_operand"    " r,r,0, r,r"))
   )
   (clobber
          (match_scratch:HI 3                             "=X,r,X, X,X"))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "@
   ior.w %0,%2
   ior.w %1,%2,%3\;mov.w %3,%0
   ior.w %1,%0,WREG
   mov.w %1,%0\;ior.w %2,%0,%0
   ior.w %1,%2,%0"
  [
    (set_attr "cc" "math")
    (set_attr "type" "etc,etc,def,def,def")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "iorhi3_J_32"
  [(set (match_operand:HI 0 "pic30_register_operand"         "=r,&r")
        (ior:HI (match_operand:HI 1 "pic30_register_operand" "%0,r")
                (match_operand:HI 2 "pic30_J_operand"        " J,J")))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "@
   ior.w #(%H2&0xFFFF),%0
   mov.w #(%H2&0xFFFF),%0\;ior.w %1,%0,%0"
  [
   (set_attr "cc" "math")
   (set_attr "type" "def")
   (set_attr "op_type" "alu")
  ]
)


;;;;;;;;;;;;;;;;;
;; single integer
;;;;;;;;;;;;;;;;;

(define_expand "iorsi3_32"
  [(set (match_operand:SI 0 "pic30_mode2_operand"
              "=r<>,r<>,R,R,  R<>,r,R,  r<>,R,r<>")
        (ior:SI
          (match_operand:SI 1 "pic30_mode1P_or_1bitclr_APSV_operand"
              "%r,  r,  r,r,  r,  r,R<>,R<>,P,P")
          (match_operand:SI 2 "pic30_mode1P_or_1bitclr_APSV_operand"
              "r,  R<>,r,R<>,P,  P,r,  r,  r,r")))
  ]
  "(pic30_isa32_target())"
  "
{
  if (pic30_one_bit_set(GET_MODE(operands[0]),operands[1],1)) {
    emit(
      gen_bitsetsi_32(operands[0], operands[2], operands[1])
    );
  } else if (pic30_one_bit_set(GET_MODE(operands[0]),operands[2],1)) {
    emit(
      gen_bitsetsi_32(operands[0], operands[1], operands[2])
    );
  } else if (pic30_mode1P_operand(operands[1],GET_MODE(operands[1])) &&
             pic30_mode1P_operand(operands[2],GET_MODE(operands[2]))) {
    emit(
      gen_iorsi3_helper_32(operands[0],operands[1],operands[2]) 
    );
  } else if (immediate_operand(operands[1],VOIDmode) &&
             satisfies_constraint_J(operands[1])) {
    emit(
      gen_iorsi3_J_32(operands[0],operands[2],operands[1])
    );
  } else if (immediate_operand(operands[2],VOIDmode) &&
             satisfies_constraint_J(operands[2])) {
    emit(
      gen_iorsi3_J_32(operands[0],operands[1],operands[2])
    );
  } else if (pic30_reg_or_near_operand(operands[1],GET_MODE(operands[1])) &&
             pic30_reg_or_near_operand(operands[2],GET_MODE(operands[2])))  {
    emit(
      gen_iorsi3_sfr_32(operands[0],operands[1],operands[2])
    );
  } else {
    rtx xop1, xop2;

    xop1 = force_reg(GET_MODE(operands[0]),operands[1]);
    xop2 = force_reg(GET_MODE(operands[0]),operands[2]);
    emit (
      gen_iorsi3_helper_32(operands[0],xop1, xop2)
    );
  }
  DONE;
}")


(define_insn "iorsi3_helper_32"
 [(set (match_operand:SI 0 "pic30_mode2_operand"
             "=r,&r, R,R,R, r,R,<>,<>,R,<>")
       (ior:SI 
          (match_operand:SI 1 "pic30_mode1P_operand"
             "%r, r, r,r,r, 0,r, r, r, r, r")
          (match_operand:SI 2 "pic30_mode1P_operand"
             " r, R, 0,R,r, P,P, <>,R,<>,P")))
  (clobber (reg:CC CC_REG))
 ]
 "(pic30_isa32_target())"
 "@
  ior.l %1,%2,%0
  ior.l %1,%2,%0
  ior.l %1,%2,%0
  ior.l %1,%2,%0
  ior.l %1,%2,%0
  ior.l %1,#(%H2&0x7F),%0
  ior.l %1,#(%H2&0x7F),%0
  ior.l %1,%2,%0
  ior.l %1,%2,%0
  ior.l %1,%2,%0
  ior.l %1,#(%H2&0x7F),%0"
 [
    (set_attr "cc" "clobber")
    (set_attr "type" "def,defuse,use,use,use,def,use,use,use,use,use")
    (set_attr "op_type" "alu")
 ]
)

;
; (CAW) it looks like '%' commutative operands cannot be inside
;       a parallel.  At least, the recog() code for this one (with the
;       % did not have path from start to return the alternate.  :(
;
;       Getting rid of the rest of them
;       this failed xc16-1988 -Os -merrata
;
;       Will delay making more changes, but will
;          comment more like this (CAW) %
;

(define_insn "iorsi3_sfr_32"
  [(set (match_operand:SI   0 "pic30_reg_or_near_operand" "=U,U,r,&r,r")
        (ior:SI
          (match_operand:SI 1 "pic30_reg_or_near_operand" " 0,r,U,U,r")
          (match_operand:SI 2 "pic30_register_operand"    " r,r,0,r,r"))
   )
   (clobber
          (match_scratch:SI 3                             "=X,r,X,X,X"))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "@
   ior.l %0,%2
   ior.l %1,%2,%3\;mov.l %3,%0
   ior.l %1,%0,WREG
   mov.l %1,%0\;ior.l %2,%0,%0
   ior.l %1,%2,%0"
  [
    (set_attr "cc" "math")
    (set_attr "type" "etc,etc,def,def,def")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "iorsi3_sfr_b_32"
  [(set (match_operand:SI   0 "pic30_reg_or_near_operand" "=U,U,r,&r,r")
        (ior:SI
          (match_operand:SI 1 "pic30_register_operand"    " r,r,0,r,r") 
          (match_operand:SI 2 "pic30_reg_or_near_operand" " 0,r,U,U,r"))
   )
   (clobber
          (match_scratch:SI 3                             "=X,r,X,X,X"))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "@
   ior.l %0,%1
   ior.l %2,%1,%3\;mov.l %3,%0
   ior.l %2,%0,WREG
   mov.l %2,%0\;ior.l %1,%0,%0
   ior.l %2,%1,%0"
  [
    (set_attr "cc" "math")
    (set_attr "type" "etc,etc,def,def,def")
    (set_attr "op_type" "alu")
  ]
)

; (CAW) %

(define_insn "iorsi3_J_32"
  [(set (match_operand:SI 0 "pic30_register_operand"         "=r,&r")
        (ior:SI (match_operand:SI 1 "pic30_register_operand" " 0,r")
                (match_operand:SI 2 "pic30_J_operand"         "J,J")))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "@
   ior.l #(%H2&0xFFFF),%0
   mov.sl #(%H2&0xFFFF),%0\;ior.l %1,%0,%0"
  [
   (set_attr "cc" "math")
   (set_attr "type" "def")
   (set_attr "op_type" "alu")
  ]
)

(define_insn "iorsi3_J_b_32"
  [(set (match_operand:SI 0 "pic30_register_operand"         "=r,&r")
        (ior:SI 
           (match_operand:SI 1 "pic30_J_operand"         "J,J")
           (match_operand:SI 2 "pic30_register_operand" " 0,r")))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "@
   ior.l #(%H1&0xFFFF),%0
   mov.sl #(%H1&0xFFFF),%0\;ior.l %2,%0,%0"
  [
   (set_attr "cc" "math")
   (set_attr "type" "def")
   (set_attr "op_type" "alu")
  ]
)



;;;;;;;;;;;;;;;;;
;; double integer
;;;;;;;;;;;;;;;;;

(define_insn "*iorhidi3_se_32"
  [(set (match_operand:DI 0 "pic30_DI_mode2_operand"         "=r,>,>,>,&r,R,R,R,&r")
        (ior:DI
           (sign_extend:DI
              (match_operand:HI 1 "pic30_register_operand" "r,r,r,r, r,r,r,r, r"))
           (match_operand:DI 2 "pic30_DI_mode2_operand"       "r,r,0,>, >,r,0,R, R")
        )
   )
   (clobber (match_scratch:HI 3                   "=&r,&r,&r,&r,&r,&r,&r,&r,&r"))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "@
   se.w %1,%3\;ior.l %3,%2,%0\;asr.l %3,#31,%3\;ior.l %3,%d2,%d0
   se.w %1,%3\;ior.l %3,%2,%0\;asr.l %3,#31,%3\;ior.l %3,%d2,%0
   se.w %1,%3\;ior.l %3,%2,%0\;asr.l %3,#31,%3\;ior %3,%s2,%0
   se.w %1,%3\;ior.l %3,%2,%0\;asr.l %3,#31,%3\;ior %3,%2,%0
   se.w %1,%3\;ior.l %3,%2,%0\;asr.l %3,#31,%3\;ior %3,%2,%d0
   se.w %1,%3\;ior.l %3,%2,%I0\;asr.l %3,#31,%3\;ior %3,%d2,%D0
   se.w %1,%3\;ior.l %3,%2,%I0\;asr.l %3,#31,%3\;ior %3,%2,%D0
   se.w %1,%3\;ior.l %3,%I2,%I0\;asr.l %3,#31,%3\;ior %3,%I2,%D0
   se.w %1,%3\;ior.l %1,%I2,%0\;asr.l %3,#31,%3\;ior %3,%D2,%d0"
   [
     (set_attr "cc" "math,math,math,math,math,clobber,clobber,clobber,clobber")
     (set_attr "type" "def")
    (set_attr "op_type" "alu")
  ]
)

; CAW - 16-bit register writes clear the upper bits
;
(define_insn "*iorhidi3_ze_32"
  [(set (match_operand:DI 0 "pic30_DI_mode2_operand"         "=r,> ,>, >,&r, R,R, R,&r")
        (ior:DI
           (zero_extend:DI
              (match_operand:HI 1 "pic30_register_operand"    "r,r ,r, r, r, r,r, r, r"))
           (match_operand:DI 2 "pic30_DI_mode2_operand"       "r,r ,0, >, >, r,0, R, R")
        )
   )
   (clobber (match_scratch:HI 3                              "=X,&r,X,&r, X,&r,X,&r, X"))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "@
   ior.l %1,%2,%0\;mov.l %d2,%d0
   ze.w %1,%3\;ior.l %2,%2,%0\;mov.l %d2,%0
   ior.l %1,%s2,%s0\;add.l #8,%r0
   ze.w %1,%3\;ior.l %3,%2,%0\;mov.l %2,%0
   ze.w %1,%0\;ior.l %0,%2,%0\;mov.l %2,%d0
   ze.w %1,%3\;ior.l %3,%2,%0\;mov %d2,%R0
   ior.l %1,%2,%0
   ze.w %1,%3\;ior.l %3,%I2,%I0\;mov.l %I2,%I0
   ze.w %1,%0\;ior.l %0,%2,%0\;mov %R2,%d0"
   [
     (set_attr "cc" "math,math,math,math,math,clobber,clobber,clobber,clobber")
     (set_attr "type" "def")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "*iordihi3_se_32"
  [(set (match_operand:DI 0 "pic30_DI_mode2_operand"         "=r,>,>,>,&r,R,R,R,&r")
        (ior:DI
           (match_operand:DI 1 "pic30_DI_mode2_operand"       "r,r,0,>, >,r,0,R, R")
           (sign_extend:DI
              (match_operand:HI 2 "pic30_register_operand" "r,r,r,r, r,r,r,r, r"))
        )
   )
   (clobber (match_scratch:HI 3                   "=&r,&r,&r,&r,&r,&r,&r,&r,&r"))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "@
   se.w %2,%3\;ior.l %3,%1,%0\;asr.l %3,#31,%3\;ior.l %3,%d1,%d0
   se.w %2,%3\;ior.l %3,%1,%0\;asr.l %3,#31,$3\;ior.l %3,%d1,%0
   se.w %2,%3\;ior.l %3,%s1,%0\;asr.l %3,#31,%3\;ior.l %3,%s1,%0
   se.w %2,%3\;ior.l %3,%1,%0\;asr.l %3,#31,%3\;ior.l %3,%1,%0
   se.w %2,%3\;ior.l %3,%1,%0\;asr.l %3,#31,%3\;ior.l %3,%1,%d0
   se.w %2,%3\;ior.l %3,%1,%I0\;asr.l %3,#31,%3\;ior.l %3,%d1,%D0
   se.w %2,%3\;ior.l %3,%1,%I0\;asr.l %3,#31,%3\;ior.l %3,%1,%D0
   se.w %2,%3\;ior.l %3,%I1,%I0\;asr.l %3,#31,%3\;ior.l %3,%D1,%D0
   se.w %2,%3\;ior.l %3,%I1,%0\;asr.l %3,#31,%3\;ior.l %3,%D1,%d0"
   [
     (set_attr "cc" "clobber")
     (set_attr "type" "def")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "*iordihi3_ze_32"
  [(set (match_operand:DI 0 "pic30_DI_mode2_operand"         "=r,>,>,>,&r,R,R,R,&r")
        (ior:DI
           (match_operand:DI 1 "pic30_DI_mode2_operand"       "r,r,0,>, >,r,0,R, R")
           (zero_extend:DI
              (match_operand:HI 2 "pic30_register_operand" "r,r,r,r, r,r,r,r, r"))
        )
   )
   (clobber (match_scratch:HI 3                   "=X,&r,X,&r,X,&r,X,&r,X"))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "@
   ior.l %1,%2,%0\;mov.l %d1,%d0
   ze.w %2,%3\;ior.l %1,%3,%0\;mov.l %d1,%0
   ior.w %2,%s1,%s0\;add.l #8,%r0
   ze.w %2,%3\;ior.l %3,%s1,%s0\;mov.l %1,%0
   ze.w %2,%0\;ior.l %0,%1,%0\;mov.l %1,%d0
   ze.w %2,%3\;ior.l %1,%3,%0\;mov.l %d1,%R0
   ior.w %2,%1,%0
   ze.w %2,%3\;ior.l %3,%I1,%I0\;mov.l %D1,%D0
   ze.w %2,%0\;ior.l %0,%1,%0\;mov.l %R1,%d0"
   [
     (set_attr "cc" "clobber")
     (set_attr "type" "def")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "*iordisi3_se_32"
  [(set (match_operand:DI 0 "pic30_DI_mode2_operand"         "=r,>,>,>,&r,R,R,R,&r")
        (ior:DI
           (match_operand:DI 1 "pic30_DI_mode2_operand"       "r,r,0,>, >,r,0,R, R")
           (sign_extend:DI
              (match_operand:SI 2 "pic30_register_operand" "r,r,r,r, r,r,r,r, r"))
        )
   )
   (clobber (match_scratch:HI 3                   "=&r,&r,&r,&r,&r,&r,&r,&r,&r"))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "@
   asr.l %2,#31,%3\;ior.l %2,%1,%0\;ior.l %3,%d1,%d0
   asr.l %2,#31,%3\;ior.l %2,%1,%0\;ior.l %3,%d1,%0
   asr.l %2,#31,%3\;ior.l %2,%s1,%0\;ior.l %3,%s1,%0
   asr.l %2,#31,%3\;ior.l %2,%1,%0\;ior.l %3,%1,%0
   asr.l %2,#31,%3\;ior.l %2,%1,%0\;ior.l %3,%1,%d0
   asr.l %2,#31,%3\;ior.l %2,%1,%I0\;ior.l %3,%d1,%D0
   asr.l %2,#31,%3\;ior.l %2,%1,%I0\;ior.l %3,%1,%D0
   asr.l %2,#31,%3\;ior.l %2,%I1,%I0\;ior.l %3,%D1,%D0
   asr.l %2,#31,%3\;ior.l %2,%I1,%0\;ior.l %3,%D1,%d0"
   [
     (set_attr "cc" "clobber")
     (set_attr "type" "def")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "*iordisi3_ze_32"
  [(set (match_operand:DI 0 "pic30_DI_mode2_operand"         "=r,>,>,>,&r,R,R,R,&r")
        (ior:DI
           (match_operand:DI 1 "pic30_DI_mode2_operand"       "r,r,0,>, >,r,0,R, R")
           (zero_extend:DI
              (match_operand:SI 2 "pic30_register_operand" "r,r,r,r, r,r,r,r, r"))
        )
   )
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "@
   ior.l %1,%2,%0\;mov.l %d1,%d0
   ior.l %1,%2,%0\;mov.l %d1,%0
   ior.l %2,%s1,%0\;add.l #4,%r0
   ior.l %2,%1,%0\;mov.l %1,%0
   ior.l %2,%1,%0\;mov.l %1,%d0
   ior.l %2,%1,%I0\;mov.l %d2,%d1,%D0
   ior.l %2,%1,%0
   ior.l %2,%I1,%I0\;mov.l %D1,%D0
   ior.l %2,%1,%0\;mov %R1,%d0"
   [
     (set_attr "cc" "clobber")
     (set_attr "type" "def")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "iordi3_32"
  [(set (match_operand:DI 0 "pic30_DI_mode2_operand"         "=r,>,>,>,&r,R,R,R,&r")
        (ior:DI
           (match_operand:DI 1 "pic30_register_operand"    "r,r,r,r, r,r,r,r, r")
           (match_operand:DI 2 "pic30_DI_mode2_operand"       "r,r,0,>, >,r,0,R, R")))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "@
   ior.l %1,%2,%0\;ior.l %d1,%d2,%d0
   ior.l %1,%2,%0\;ior.l %d1,%d2,%0
   ior.l %1,%s2,%0\;ior.l %d1,%s2,%0
   ior.l %1,%2,%0\;ior.l %d1,%2,%0
   ior.l %1,%2,%0\;ior.l %d1,%2,%d0
   ior.l %1,%2,%I0\;ior.l %d1,%d2,%D0
   ior.l %1,%2,%I0\;ior.l %d1,%2,%D0
   ior.l %1,%I2,%I0\;ior.l %d1,%D2,%D0
   ior.l %1,%I2,%0\;ior.l %d1,%D2,%d0"
   [
     (set_attr "cc" "clobber")
     (set_attr "type" "def")
    (set_attr "op_type" "alu")
  ]
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; XOR
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;
;; quarter integer
;;;;;;;;;;;;;;;;;;

(define_insn "*xorqi3_imm_32"
  [(set (match_operand:QI         0 "pic30_register_operand" "=r")
        (xor:QI (match_dup 0)
                (match_operand:QI 1 "pic30_J_operand"        " J")))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "xor.b #(%H1&0xFF),%0"
  [
   (set_attr "cc" "math")
   (set_attr "type" "def")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "*xorqi3_imm_b_32"
  [(set (match_operand:QI         0 "pic30_register_operand" "=r")
        (xor:QI 
                (match_operand:QI 1 "pic30_J_operand"        " J")
                (match_dup 0)))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "xor.b #(%H1&0xFF),%0"
  [
   (set_attr "cc" "math")
   (set_attr "type" "def")
    (set_attr "op_type" "alu")
  ]
)


(define_insn "xorqi3_gen_32"
  [(set (match_operand:QI 0 "pic30_mode2_operand"
              "=r<>,r<>,R,R,  r<>,R,r<>,R,  r<>,R")
        (xor:QI 
           (match_operand:QI 1 "pic30_mode1P_operand"
              "%r,  r,  r,r,  r,  r,R<>,R<>,P,  P")
           (match_operand:QI 2 "pic30_mode1P_operand"
              " r,  R<>,r,R<>,P,  P,r,  r,  r,  r")))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "@
   xor.b %1,%2,%0
   xor.b %1,%2,%0
   xor.b %1,%2,%0
   xor.b %1,%2,%0
   xor.b %1,#(%H2&0x7F),%0
   xor.b %1,#(%H2&0x7F),%0
   xor.b %2,%1,%0
   xor.b %2,%1,%0
   xor.b %2,#(%H1&0x7F),%0
   xor.b %2,#(%H1&0x7F),%0"
  [
   (set_attr "cc" "math")
   (set_attr "type" "def,defuse,use,use,def,use,defuse,use,def,use")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "*xorqi3_sfr0_32"
  [(set (match_operand:QI   0 "pic30_register_operand"   "=r,&r")
        (xor:QI 
          (match_operand:QI 1 "pic30_register_operand"   "%0,r")
          (match_operand:QI 2 "pic30_near_operand"       " U,U")))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "@
   xor.b %2,%0,WREG
   mov.bz %2,%0\;xor.b %1,%0,%0"
  [
   (set_attr "cc" "math")
   (set_attr "type" "def")
   (set_attr "op_type" "alu")
  ]
)

; leave this match_dup, operand 0 will not require a reload (CAW)
(define_insn "*xorqi3_sfr1_32"
  [(set (match_operand:QI   0 "pic30_near_operand"     "=U")
        (xor:QI 
          (match_dup 0)
          (match_operand:QI 1 "pic30_register_operand"  "r")))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "xor.b %0,%1"
  [
    (set_attr "cc" "math")
    (set_attr "type" "etc")
    (set_attr "op_type" "alu")
  ]
)

;;;;;;;;;;;;;;;
;; half integer
;;;;;;;;;;;;;;;

(define_insn "xorhi3J_32"
  [(set (match_operand:HI    0 "pic30_register_operand" "=r")
        (xor:HI 
           (match_operand:HI 1 "pic30_register_operand" "%0")
           (match_operand:HI 2 "immediate_operand"     " i")))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "xor.w #(%H2&0xFFFF),%0"
  [
   (set_attr "cc" "math")
   (set_attr "type" "def")
   (set_attr "op_type" "alu")
  ]
)

(define_insn "xorhi3_gen_32"
  [(set (match_operand:HI 0 "pic30_mode2_operand"
              "=r<>,r<>, R,R,  r<>,R,r<>,R,  r<>,R")
        (xor:HI 
           (match_operand:HI 1 "pic30_mode1P_operand"
              "%r,  r,   r,r,  r,  r,R<>,R<>,P,  P")
           (match_operand:HI 2 "pic30_mode1P_operand"
              "r,  R<>, r,R<>,P,  P,r,  r,  r,  r")))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "@
   xor.w %1,%2,%0
   xor.w %1,%2,%0
   xor.w %1,%2,%0
   xor.w %1,%2,%0
   xor.w %1,#(%H2&0xFFFF),%0
   xor.w %1,#(%H2&0xFFFF),%0
   xor.w %2,%1,%0
   xor.w %2,%1,%0
   xor.w %2,#(%H1&0xFFFF),%0
   xor.w %2,#(%H1&0xFFFF),%0"
  [
   (set_attr "cc" "math")
   (set_attr "type" "def,defuse,use,use,def,use,defuse,use,def,use")
   (set_attr "op_type" "alu")
  ]
)

(define_insn "xorhi3_sfr0_32"
  [(set (match_operand:HI 0         "pic30_reg_or_near_operand" "=U,r,r")
        (xor:HI (match_operand:HI 1 "pic30_register_operand"    "%r,0,r")
                (match_operand:HI 2 "pic30_reg_or_near_operand" " 0,U,r")))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "@
   xor.w %0,%1
   xor.w %2,%0,WREG
   xor.w %2,%1,%0"
  [
    (set_attr "cc" "math")
    (set_attr "type" "etc,def,def")
    (set_attr "op_type" "alu")
  ]
)

; leave this match_dup, operand 0 will not require a reload (CAW)
(define_insn "xorhi3_sfr2_32"
  [(set (match_operand:HI 0 "pic30_near_operand"            "=U")
        (xor:HI 
          (match_dup 0)
          (match_operand:HI 1 "pic30_immediate_1bit_operand" "i")))
  ]
  "(pic30_isa32_target())"
  "btg.w %0,#%b1"
  [
    (set_attr "cc" "clobber")
    (set_attr "type" "etc")
    (set_attr "op_type" "alu")
  ]
)

; leave this match_dup, operand 0 will not require a reload (CAW)
(define_insn "xorhi3_sfr3_32"
  [(set (match_operand:HI   0 "pic30_near_operand"          "=U")
        (xor:HI 
          (match_operand:HI 1 "pic30_immediate_1bit_operand" "i")
          (match_dup 0)))
  ]
  "(pic30_isa32_target())"
  "btg.w %0,#%b1"
  [
    (set_attr "cc" "clobber")
    (set_attr "type" "etc")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "*xorhi3_imm_32"
  [(set (match_operand:HI 0 "pic30_register_operand"         "=r")
        (xor:HI (match_operand:HI 1 "pic30_register_operand" "%0")
                (match_operand:HI 2 "pic30_J_operand"   "J")))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "xor.w #(%H2&0xFFFF),%0"
  [
   (set_attr "cc" "math")
   (set_attr "type" "def")
    (set_attr "op_type" "alu")
  ]
)

; this match_can cause issues iff operand 1 is dies in this instruction and
;   we decide to use it to reload operand 0 (CAW)
(define_insn "*xorhi3_sfr0_32"
  [(set (match_operand:HI 0 "pic30_reg_or_near_operand"   "+r,U")
        (xor:HI 
          (match_operand:HI 1 "pic30_reg_or_near_operand" "%U,r")
          (match_operand:HI 2 "pic30_reg_or_near_operand" "0,0")))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "@
   xor.w %1,%0,WREG
   xor.w %0,%1"
  [
    (set_attr "cc" "clobber")
    (set_attr "type" "def,etc")
    (set_attr "op_type" "alu")
  ]
)

;;;;;;;;;;;;;;;;;
;; single integer
;;;;;;;;;;;;;;;;;

(define_insn "xorsi3_gen_32"
 [(set 
    (match_operand:SI 0 "pic30_mode2_operand" 
                                                 "=r,&r,R,R,R, r,R,<>,<>,R,<>")
      (xor:SI 
        (match_operand:SI 1 "pic30_register_operand" 
                                                 "%r, r,r,r,r, 0,r, r, r, r, r")
        (match_operand:SI 2 "pic30_mode1P_operand" 
                                                 " r, R,0,R,r, P,P, <>,R,<>,P")
  ))
  (clobber (reg:CC CC_REG))
 ]
 "(pic30_isa32_target())"
 "@
  xor.l %1,%2,%0
  xor.l %1,%2,%0
  xor.l %1,%2,%0
  xor.l %1,%2,%0
  xor.l %1,%2,%0
  xor.l %1,#(%H2&0x7F),%0
  xor.l %1,#(%H2&0x7F),%0
  xor.l %1,%2,%0
  xor.l %1,%2,%0
  xor.l %1,%2,%0
  xor.l %1,#(%H2&0x7F),%0"
 [
  (set_attr "cc" "clobber")
  (set_attr "type" "def,defuse,use,use,use,def,use,use,use,use,use")
  (set_attr "op_type" "alu")
 ]
)

(define_expand "xor<mode>3_32"
 [(set 
    (match_operand:RESTRICTED_INT_MODES   0 "pic30_register_operand"   "") 
    (xor:RESTRICTED_INT_MODES 
      (match_operand:RESTRICTED_INT_MODES 1 "pic30_register_operand"   "")
      (match_operand:RESTRICTED_INT_MODES 2 "pic30_reg_or_imm_operand" "")))
 ]
 "pic30_isa32_target()"
 "{
    rtx op2 = operands[2];

    if (!pic30_mode1P_operand(op2,GET_MODE(op2))) {
      op2 = force_reg(GET_MODE(operand0),op2);
    }
    emit(
      gen_xor<mode>3_gen_32(operands[0],operands[1], op2)
    );
    DONE;
  }"
)

;;;;;;;;;;;;;;;;;
;; double integer
;;;;;;;;;;;;;;;;;

/* not yet ported */
(define_insn "*xorhidi3_se_32"
  [(set (match_operand:DI 0 "pic30_DI_mode2_operand"         "=r,>,>,>,&r,R,R,R,&r")
        (xor:DI
           (sign_extend:DI
              (match_operand:HI 1 "pic30_register_operand" "r,r,r,r, r,r,r,r, r"))
           (match_operand:DI 2 "pic30_DI_mode2_operand"       "r,r,0,>, >,r,0,R, R")))
   (clobber (match_scratch:HI 3                   "=&r,&r,&r,&r,&r,&r,&r,&r,&r"))
   (clobber (reg:CC CC_REG))
  ]
  "0"
  "*
{
  const char *noerrata_patterns[] = {
     /* r,r,r */          \"asr %1,#15,%3\;xor %1,%2,%0\;xor %3,%d2,%d0\;\"
                              \"xor %3,%t2,%t0\;xor %3,%q2,%q0\",

     /* >,r,r */          \"asr %1,#15,%3\;xor %1,%2,%0\;xor %3,%d2,%0\;\"
                              \"xor %3,%t2,%0\;xor %3,%q2,%0\",

     /* >,r,0 */          \"asr %1,#15,%3\;xor %1,%s2,%0\;xor %3,%s2,%0\;\"
                              \"xor %3,%s2,%0\;xor %3,%s2,%0\",

     /* >,r,> */          \"asr %1,#15,%3\;xor %1,%2,%0\;xor %3,%2,%0\;\"
                              \"xor %3,%2,%0\;xor %3,%2,%0\",

     /* r,r,> */          \"asr %1,#15,%3\;xor %1,%2,%0\;xor %3,%2,%d0\;\"
                              \"xor %3,%2,%t0\;xor %3,%2,%q0\",

     /* R,r,r */          \"asr %1,#15,%3\;xor %1,%2,%I0\;xor %3,%d2,%I0\;\"
                              \"xor %3,%t2,%I0\;xor %3,%q2,%0\",

     /* R,r,0 */          \"asr %1,#15,%3\;xor %1,%2,%I0\;xor %3,%2,%I0\;\"
                              \"xor %3,%2,%I0\;xor %3,%2,%0\",

     /* R,r,R */          \"asr %1,#15,%3\;xor %1,%I2,%I0\;xor %3,%I2,%I0\;\"
                              \"xor %3,%I2,%I0\;xor %3,%2,%0\",

     /* r,r,R */          \"asr %1,#15,%3\;xor %1,%I2,%0\;xor %3,%I2,%d0\;\"
                              \"xor %3,%I2,%t0\;xor %3,%2,%q0\",
  };

  const char *psv_psv_patterns[] = {
     /* r,r,r */          \"asr %1,#15,%3\;xor %1,%2,%0\;xor %3,%d2,%d0\;\"
                              \"xor %3,%t2,%t0\;xor %3,%q2,%q0\",

     /* >,r,r */          \"asr %1,#15,%3\;xor %1,%2,%0\;xor %3,%d2,%0\;\"
                              \"xor %3,%t2,%0\;xor %3,%q2,%0\",

     /* >,r,0 */          \"asr %1,#15,%3\;xor %1,%s2,%0\;xor %3,%s2,%0\;\"
                              \"xor %3,%s2,%0\;xor %3,%s2,%0\",

     /* >,r,> */          \"asr %1,#15,%3\;\"
                          \"xor %1,%2,%0\;\"
                          \"nop\;\"
                          \"xor %3,%2,%0\;\"
                          \"nop\;\"
                          \"xor %3,%2,%0\;\"
                          \"nop\;\"
                          \"xor %3,%2,%0\",

     /* r,r,> */          \"asr %1,#15,%3\;\"
                          \"xor %1,%2,%0\;\"
                          \"nop\;\"
                          \"xor %3,%2,%d0\;\"
                          \"nop\;\"
                          \"xor %3,%2,%t0\;\"
                          \"nop\;\"
                          \"xor %3,%2,%q0\",

     /* R,r,r */          \"asr %1,#15,%3\;xor %1,%2,%I0\;xor %3,%d2,%I0\;\"
                              \"xor %3,%t2,%I0\;xor %3,%q2,%0\",

     /* R,r,0 */          \"asr %1,#15,%3\;\"
                          \"xor %1,%2,%I0\;\"
                          \"nop\;\"
                          \"xor %3,%2,%I0\;\"
                          \"nop\;\"
                          \"xor %3,%2,%I0\;\"
                          \"nop\;\"
                          \"xor %3,%2,%0\",

     /* R,r,R */          \"asr %1,#15,%3\;\"
                          \"xor %1,%I2,%I0\;\"
                          \"nop\;\"
                          \"xor %3,%I2,%I0\;\"
                          \"nop\;\"
                          \"xor %3,%I2,%I0\;\"
                          \"nop\;\"
                          \"xor %3,%2,%0\",

     /* r,r,R */          \"asr %1,#15,%3\;\"
                          \"xor %1,%I2,%0\;\"
                          \"nop\;\"
                          \"xor %3,%I2,%d0\;\"
                          \"nop\;\"
                          \"xor %3,%I2,%t0\;\"
                          \"nop\;\"
                          \"xor %3,%2,%q0\",
  };

  /* increasing the patterns, means increasing this number too */
  static char szInsns[180];
  int regno;
  rtx x;
  const char **patterns;

  if (pic30_psrd_psrd_errata(NULL,operands[2])) {
    pic30_rtx_nops += 3;
    patterns = psv_psv_patterns;
  } else {
    patterns = noerrata_patterns;
  }

  switch (which_alternative) {
     default: return patterns[which_alternative];

     case 5:  /* R,r,r */
              strcpy(szInsns, patterns[which_alternative]);

              x = XEXP(operands[0],0);
	      if (!pic30_dead_or_set_p(NEXT_INSN(insn), x) &&
                  !find_regno_note(insn, REG_UNUSED, REGNO(x))) {
                 strcat(szInsns,\"\;sub #6,%r0\");
              }
              return szInsns;

     case 6:  /* R,r,0 */
              strcpy(szInsns, patterns[which_alternative]);

              x = XEXP(operands[0],0);
	      if (!pic30_dead_or_set_p(NEXT_INSN(insn), x) &&
                  !find_regno_note(insn, REG_UNUSED, REGNO(x))) {
                 strcat(szInsns,\"\;sub #6,%r0\");
              }
              return szInsns;

     case 7:  /* R,r,R */
              strcpy(szInsns, patterns[which_alternative]);

              x = XEXP(operands[0],0);
	      if (!pic30_dead_or_set_p(NEXT_INSN(insn), x) &&
                  !find_regno_note(insn, REG_UNUSED, REGNO(x))) {
                 strcat(szInsns,\"\;sub #6,%r0\");
              }

              x = XEXP(operands[2],0);
	      if (!pic30_dead_or_set_p(NEXT_INSN(insn), x) &&
                  !find_regno_note(insn, REG_UNUSED, REGNO(x))) {
                 strcat(szInsns,\"\;sub #6,%r2\");
              }
              return szInsns;

     case 8:  /* r,r,R */
              strcpy(szInsns, patterns[which_alternative]);

              x = XEXP(operands[2],0);
              if (!pic30_dead_or_set_p(NEXT_INSN(insn), x) &&
                  !find_regno_note(insn, REG_UNUSED, REGNO(x))) {
                 strcat(szInsns,\"\;sub #6,%r2\");
              }
              return szInsns;
   }
}"
   [
     (set_attr "cc" "math,math,math,math,math,clobber,clobber,clobber,clobber")
     (set_attr "type" "def")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "*xorhidi3_ze_32"
  [(set (match_operand:DI 0 "pic30_DI_mode2_operand"         "=r,>,>,>,&r,R,R,R,&r")
        (xor:DI
           (zero_extend:DI
              (match_operand:HI 1 "pic30_register_operand" "r,r,r,r, r,r,r,r, r"))
           (match_operand:DI 2 "pic30_DI_mode2_operand"       "r,r,0,>, >,r,0,R, R")))
   (clobber (match_scratch:HI 3                   "=X,X,&r,&r,&r,X,&r,&r,&r"))
   (clobber (reg:CC CC_REG))
  ]
  "0"
  "*
{
  const char *noerrata_patterns[] = {
     /* r,r,r */          \"xor %1,%2,%0\;mov %d2,%d0\;mov.d %t2,%t0\",

     /* >,r,r */          \"xor %1,%2,%0\;mov %d2,%0\;mov.d %t2,%0\",

     /* >,r,0 */          \"xor %1,%s2,%s0\;add #8,%r0\",

     /* >,r,> */          \"xor %1,%2,%0\;mov %2,%0\;mov %2,%0\;mov %2,%0\",

     /* &r,r,> */         \"xor %1,%2,%0\;mov %2,%d0\;mov.d %2,%t0\",

     /* R,r,r */          \"xor %1,%2,%0\;mov %d2,%Q0\;mov %t2,%R0\;mov %q2,%S0\",

     /* R,r,0 */          \"xor %1,%2,%0\",

     /* R,r,R */          \"xor %1,%I2,%I0\;mov %I2,%I0\;mov %I2,%I0\;mov %2,%0\",

     /* r,r,R */          \"xor %1,%2,%0\;mov %Q2,%d0\;mov %R2,%t0\;mov %S2,%q0\"
  };

  const char *psv_psv_patterns[] = {
     /* r,r,r */          \"xor %1,%2,%0\;mov %d2,%d0\;mov.d %t2,%t0\",

     /* >,r,r */          \"xor %1,%2,%0\;mov %d2,%0\;mov.d %t2,%0\",

     /* >,r,0 */          \"xor %1,%s2,%s0\;add #8,%r0\",

     /* >,r,> */          \"xor %1,%2,%0\;\"
                          \"nop\;\"
                          \"mov %2,%0\;\"
                          \"nop\;\"
                          \"mov %2,%0\;mov %2,%0\",

     /* r,r,> */          \"xor %1,%2,%0\;\"
                          \"nop\;\"
                          \"mov %2,%d0\;\"
                          \"nop\;\"
                          \"mov.d %2,%t0\",

     /* R,r,r */          \"xor %1,%2,%0\;mov %d2,%Q0\;mov %t2,%R0\;mov %q2,%S0\",

     /* R,r,0 */          \"xor %1,%2,%0\",

     /* R,r,R */          \"xor %1,%I2,%I0\;\"
                          \"nop\;\"
                          \"mov %I2,%I0\;\"
                          \"nop\;\"
                          \"mov %I2,%I0\;\"
                          \"nop\;\"
                          \"mov %2,%0\",

     /* r,r,R */          \"xor %1,%2,%0\;\"
                          \"nop\;\"
                          \"mov %Q2,%d0\;\"
                          \"nop\;\"
                          \"mov %R2,%t0\;\"
                          \"nop\;\"
                          \"mov %S2,%q0\"
  };
  
  const char *psv_psv_movd_patterns[] = {
     /* r,r,r */          \"xor %1,%2,%0\;mov %d2,%d0\;mov.d %t2,%t0\",

     /* >,r,r */          \"xor %1,%2,%0\;mov %d2,%0\;mov.d %t2,%0\",

     /* >,r,0 */          \"xor %1,%s2,%s0\;add #8,%r0\",

     /* >,r,> */          \"xor %1,%2,%0\;\"
                          \"mov %2,%0\;\"
                          \"mov %2,%0\;\"
                          \"mov %2,%0\",

     /* r,r,> */          \"xor %1,%2,%0\;\"
                          \"mov %2,%d0\;\"
                          \"mov %2,%t0\;\"
                          \"mov %2,%q0\",

     /* R,r,r */          \"xor %1,%2,%0\;\"
                          \"mov %d2,%Q0\;\"
                          \"mov %t2,%R0\;\"
                          \"mov %q2,%S0\",

     /* R,r,0 */          \"xor %1,%2,%0\",

     /* R,r,R */          \"xor %1,%I2,%I0\;\"
                          \"mov %I2,%I0\;\"
                          \"mov %I2,%I0\;\"
                          \"mov %2,%0\",

     /* r,r,R */          \"xor %1,%2,%0\;\"
                          \"mov %Q2,%d0\;\"
                          \"mov %R2,%t0\;\"
                          \"mov %S2,%q0\"
  };
  
  /* increasing the patterns, means increasing this number too */
  static char szInsns[180];
  int regno;
  rtx x;
  const char **patterns;

  if (pic30_psrd_psrd_errata_movd(NULL,operands[2])) {
    patterns = psv_psv_movd_patterns;
  } else if (pic30_psrd_psrd_errata(NULL,operands[2])) {
    pic30_rtx_nops += 3;
    patterns = psv_psv_patterns;
  } else {
    patterns = noerrata_patterns;
  }

  switch (which_alternative) {
     default: return patterns[which_alternative];

     case 7:  /* R,r,R */
              strcpy(szInsns, patterns[which_alternative]);

              x = XEXP(operands[0],0);
	      if (!pic30_dead_or_set_p(NEXT_INSN(insn), x) &&
                  !find_regno_note(insn, REG_UNUSED, REGNO(x))) {
                 strcat(szInsns,\"\;sub #6,%r0\");
              }

              x = XEXP(operands[2],0);
	      if (!pic30_dead_or_set_p(NEXT_INSN(insn), x) &&
                  !find_regno_note(insn, REG_UNUSED, REGNO(x))) {
                 strcat(szInsns,\"\;sub #6,%r2\");
              }
              return szInsns;
              }
}"
   [
     (set_attr "cc" "math,math,math,math,math,clobber,clobber,clobber,clobber")
     (set_attr "type" "def")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "*xorsidi3_se_32"
  [(set (match_operand:DI 0 "pic30_DI_mode2_operand"         "=r,>,>,>,&r,R,R,R,&r")
        (xor:DI
           (sign_extend:DI
              (match_operand:SI 1 "pic30_register_operand" "r,r,r,r, r,r,r,r, r"))
           (match_operand:DI 2 "pic30_DI_mode2_operand"       "r,r,0,>, >,r,0,R, R")))
   (clobber (match_scratch:HI 3                   "=&r,&r,&r,&r,&r,&r,&r,&r,&r"))
   (clobber (reg:CC CC_REG))
  ]
  "0"
  "*
{
  const char *noerrata_patterns[] = {
     /* r,r,r */          \"asr %d1,#15,%3\;xor %1,%2,%0\;xor %d1,%d2,%d0\;\"
                              \"xor %3,%t2,%t0\;xor %3,%q2,%q0\",

     /* >,r,r */          \"asr %d1,#15,%3\;xor %1,%2,%0\;xor %d1,%d2,%0\;\"
                              \"xor %3,%t2,%0\;xor %3,%q2,%0\",

     /* >,r,0 */          \"asr %d1,#15,%3\;xor %1,%s2,%0\;xor %d1,%s2,%0\;\"
                              \"xor %3,%s2,%0\;xor %3,%s2,%0\",

     /* >,r,> */          \"asr %d1,#15,%3\;xor %1,%2,%0\;xor %d1,%2,%0\;\"
                              \"xor %3,%2,%0\;xor %3,%2,%0\",

     /* r,r,> */          \"asr %d1,#15,%3\;xor %1,%2,%0\;xor %d1,%2,%d0\;\"
                              \"xor %3,%2,%t0\;xor %3,%2,%q0\",

     /* R,r,r */          \"asr %d1,#15,%3\;xor %1,%2,%I0\;xor %d1,%d2,%I0\;\"
                              \"xor %3,%t2,%I0\;xor %3,%q2,%0\",

     /* R,r,0 */          \"asr %d1,#15,%3\;xor %1,%2,%I0\;xor %d1,%2,%I0\;\"
                              \"xor %3,%2,%I0\;xor %3,%2,%0\",

     /* R,r,R */          \"asr %d1,#15,%3\;xor %1,%I2,%I0\;xor %d1,%I2,%I0\;\"
                              \"xor %3,%I2,%I0\;xor %3,%2,%0\",

     /* r,r,R */          \"asr %d1,#15,%3\;xor %1,%I2,%0\;xor %d1,%I2,%d0\;\"
                              \"xor %3,%I2,%t0\;xor %3,%2,%q0\",
  };

  const char *psv_psv_patterns[] = {
     /* r,r,r */          \"asr %d1,#15,%3\;xor %1,%2,%0\;xor %d1,%d2,%d0\;\"
                              \"xor %3,%t2,%t0\;xor %3,%q2,%q0\",

     /* >,r,r */          \"asr %d1,#15,%3\;xor %1,%2,%0\;xor %d1,%d2,%0\;\"
                              \"xor %3,%t2,%0\;xor %3,%q2,%0\",

     /* >,r,0 */          \"asr %d1,#15,%3\;xor %1,%s2,%0\;xor %d1,%s2,%0\;\"
                              \"xor %3,%s2,%0\;xor %3,%s2,%0\",

     /* >,r,> */          \"asr %d1,#15,%3\;\"
                          \"xor %1,%2,%0\;\"
                          \"nop\;\"
                          \"xor %d1,%2,%0\;\"
                          \"nop\;\"
                          \"xor %3,%2,%0\;\"
                          \"nop\;\"
                          \"xor %3,%2,%0\",

     /* r,r,> */          \"asr %d1,#15,%3\;\"
                          \"xor %1,%2,%0\;\"
                          \"nop\;\"
                          \"xor %d1,%2,%d0\;\"
                          \"nop\;\"
                          \"xor %3,%2,%t0\;\"
                          \"nop\;\"
                          \"xor %3,%2,%q0\",

     /* R,r,r */          \"asr %d1,#15,%3\;xor %1,%2,%I0\;xor %d1,%d2,%I0\;\"
                              \"xor %3,%t2,%I0\;xor %3,%q2,%0\",

     /* R,r,0 */          \"asr %d1,#15,%3\;xor %1,%2,%I0\;xor %d1,%2,%I0\;\"
                              \"xor %3,%2,%I0\;xor %3,%2,%0\",

     /* R,r,R */          \"asr %d1,#15,%3\;\"
                          \"xor %1,%I2,%I0\;\"
                          \"nop\;\"
                          \"xor %d1,%I2,%I0\;\"
                          \"nop\;\"
                          \"xor %3,%I2,%I0\;\"
                          \"nop\;\"
                          \"xor %3,%2,%0\",

     /* r,r,R */          \"asr %d1,#15,%3\;\"
                          \"xor %1,%I2,%0\;\"
                          \"nop\;\"
                          \"xor %d1,%I2,%d0\;\"
                          \"nop\;\"
                          \"xor %3,%I2,%t0\;\"
                          \"nop\;\"
                          \"xor %3,%2,%q0\",
  };

  /* increasing the patterns, means increasing this number too */
  static char szInsns[180];
  int regno;
  rtx x;
  const char **patterns;

  if (pic30_psrd_psrd_errata(NULL,operands[2])) {
    pic30_rtx_nops += 3;
    patterns = psv_psv_patterns;
  } else {
    patterns = noerrata_patterns;
  }

  switch (which_alternative) {
     default: return patterns[which_alternative];

     case 5:  /* R,r,r */
              strcpy(szInsns, patterns[which_alternative]);

              x = XEXP(operands[0],0);
	      if (!pic30_dead_or_set_p(NEXT_INSN(insn), x) &&
                  !find_regno_note(insn, REG_UNUSED, REGNO(x))) {
                 strcat(szInsns,\"\;sub #6,%r0\");
              }
              return szInsns;

     case 6:  /* R,r,0 */
              strcpy(szInsns, patterns[which_alternative]);

              x = XEXP(operands[0],0);
	      if (!pic30_dead_or_set_p(NEXT_INSN(insn), x) &&
                  !find_regno_note(insn, REG_UNUSED, REGNO(x))) {
                 strcat(szInsns,\"\;sub #6,%r0\");
              }
              return szInsns;

     case 7:  /* R,r,R */
              strcpy(szInsns, patterns[which_alternative]);

              x = XEXP(operands[0],0);
	      if (!pic30_dead_or_set_p(NEXT_INSN(insn), x) &&
                  !find_regno_note(insn, REG_UNUSED, REGNO(x))) {
                 strcat(szInsns,\"\;sub #6,%r0\");
              }

              x = XEXP(operands[2],0);
	      if (!pic30_dead_or_set_p(NEXT_INSN(insn), x) &&
                  !find_regno_note(insn, REG_UNUSED, REGNO(x))) {
                 strcat(szInsns,\"\;sub #6,%r2\");
              }
              return szInsns;

     case 8:  /* r,r,R */
              strcpy(szInsns, patterns[which_alternative]);

              x = XEXP(operands[2],0);
              if (!pic30_dead_or_set_p(NEXT_INSN(insn), x) &&
                  !find_regno_note(insn, REG_UNUSED, REGNO(x))) {
                 strcat(szInsns,\"\;sub #6,%r2\");
              }
              return szInsns;
   }
}"
   [
     (set_attr "cc" "math,math,math,math,math,clobber,clobber,clobber,clobber")
     (set_attr "type" "def")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "*xorsidi3_ze_32"
  [(set (match_operand:DI 0 "pic30_DI_mode2_operand"         "=r,>,>,>,&r,R,R,R,&r")
        (xor:DI
           (zero_extend:DI
              (match_operand:SI 1 "pic30_register_operand" "r,r,r,r, r,r,r,r, r"))
           (match_operand:DI 2 "pic30_DI_mode2_operand"       "r,r,0,>, >,r,0,R, R")))
   (clobber (reg:CC CC_REG))
  ]
  "0"
  "*
{
  const char *noerrata_patterns[] = {
     /* r,r,r */          \"xor %1,%2,%0\;xor %d1,%d2,%d0\;mov.d %t2,%t0\",

     /* >,r,r */          \"xor %1,%2,%0\;xor %d1,%d2,%0\;mov.d %t2,%0\",

     /* >,r,0 */          \"xor %1,%s2,%0\;xor %d1,%s2,%0\;add #4,%0\",

     /* >,r,> */          \"xor %1,%2,%0\;xor %d1,%2,%0\;\"
                              \"mov %2,%0\;mov %2,%0\",

     /* r,r,> */          \"xor %1,%2,%0\;xor %d1,%2,%d0\;mov.d %2,%t0\",

     /* R,r,r */          \"xor %1,%2,%I0\;xor %d1,%d2,%D0\;mov %t2,%R0\;mov %q2,%S0\",

     /* R,r,0 */          \"xor %1,%2,%I0\;xor %d1,%2,%D0\",

     /* R,r,R */          \"xor %1,%I2,%I0\;xor %d1,%I2,%I0\;mov %I2,%I0\;mov %2,%0\",

     /* r,r,R */          \"xor %1,%I2,%0\;xor %d1,%D2,%d0\;mov %R2,%t0\;mov %S2,%q0\"
  };

  const char *psv_psv_patterns[] = {
     /* r,r,r */          \"xor %1,%2,%0\;xor %d1,%d2,%d0\;mov.d %t2,%t0\",

     /* >,r,r */          \"xor %1,%2,%0\;xor %d1,%d2,%0\;mov.d %t2,%0\",

     /* >,r,0 */          \"xor %1,%s2,%0\;xor %d1,%s2,%0\;add #4,%0\",

     /* >,r,> */          \"xor %1,%2,%0\;\"
                          \"nop\;\"
                          \"xor %d1,%2,%0\;\"
                          \"nop\;\"
                          \"mov %2,%0\;\"
                          \"nop\;\"
                          \"mov %2,%0\",

     /* r,r,> */          \"xor %1,%2,%0\;\"
                          \"nop\;\"
                          \"xor %d1,%2,%d0\;\"
                          \"nop\;\"
                          \"mov.d %2,%t0\",

     /* R,r,r */          \"xor %1,%2,%I0\;xor %d1,%d2,%D0\;mov %t2,%R0\;mov %q2,%S0\",

     /* R,r,0 */          \"xor %1,%2,%I0\;xor %d1,%2,%D0\",

     /* R,r,R */          \"xor %1,%I2,%I0\;\"
                          \"nop\;\"
                          \"xor %d1,%I2,%I0\;\"
                          \"nop\;\"
                          \"mov %I2,%I0\;\"
                          \"nop\;\"
                          \"mov %2,%0\",

     /* r,r,R */          \"xor %1,%I2,%0\;\"
                          \"nop\;\"
                          \"xor %d1,%D2,%d0\;\"
                          \"nop\;\"
                          \"mov %R2,%t0\;\"
                          \"nop\;\"
                          \"mov %S2,%q0\"
  };

  const char *psv_psv_movd_patterns[] = {
     /* r,r,r */          \"xor %1,%2,%0\;xor %d1,%d2,%d0\;mov.d %t2,%t0\",

     /* >,r,r */          \"xor %1,%2,%0\;xor %d1,%d2,%0\;mov.d %t2,%0\",

     /* >,r,0 */          \"xor %1,%s2,%0\;xor %d1,%s2,%0\;add #4,%0\",

     /* >,r,> */          \"xor %1,%2,%0\;\"
                          \"xor %d1,%2,%0\;\"
                          \"mov %2,%0\;\"
                          \"mov %2,%0\",

     /* r,r,> */          \"xor %1,%2,%0\;\"
                          \"xor %d1,%2,%d0\;\"
                          \"mov %2,%t0\;\"
                          \"mov %2,%q0\",

     /* R,r,r */          \"xor %1,%2,%I0\;\"
                          \"xor %d1,%d2,%D0\;\"
                          \"mov %t2,%R0\;\"
                          \"mov %q2,%S0\",

     /* R,r,0 */          \"xor %1,%2,%I0\;xor %d1,%2,%D0\",

     /* R,r,R */          \"xor %1,%I2,%I0\;\"
                          \"xor %d1,%I2,%I0\;\"
                          \"mov %I2,%I0\;\"
                          \"mov %2,%0\",

     /* r,r,R */          \"xor %1,%I2,%0\;\"
                          \"xor %d1,%D2,%d0\;\"
                          \"mov %R2,%t0\;\"
                          \"mov %S2,%q0\"
  };

  /* increasing the patterns, means increasing this number too */
  static char szInsns[180];
  int regno;
  rtx x;
  const char **patterns;

  if (pic30_psrd_psrd_errata_movd(NULL,operands[2])) {
    patterns = psv_psv_movd_patterns;
  } else if (pic30_psrd_psrd_errata(NULL,operands[2])) {
    pic30_rtx_nops += 3;
    patterns = psv_psv_patterns;
  } else {
    patterns = noerrata_patterns;
  }

  switch (which_alternative) {
     default: return patterns[which_alternative];


     case 7:  /* R,r,R */
              strcpy(szInsns, patterns[which_alternative]);

              x = XEXP(operands[0],0);
	      if (!pic30_dead_or_set_p(NEXT_INSN(insn), x) &&
                  !find_regno_note(insn, REG_UNUSED, REGNO(x))) {
                 strcat(szInsns,\"\;sub #6,%r0\");
              }

              x = XEXP(operands[2],0);
	      if (!pic30_dead_or_set_p(NEXT_INSN(insn), x) &&
                  !find_regno_note(insn, REG_UNUSED, REGNO(x))) {
                 strcat(szInsns,\"\;sub #6,%r2\");
              }
              return szInsns;

   }
}"
   [
     (set_attr "cc" "math,math,math,math,math,clobber,clobber,clobber,clobber")
     (set_attr "type" "def")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "*xordihi3_se_32"
  [(set (match_operand:DI 0 "pic30_DI_mode2_operand"         "=r,>,>,>,&r,R,R,R,&r")
        (xor:DI
           (match_operand:DI 1 "pic30_DI_mode2_operand"       "r,r,0,>, >,r,0,R, R")
           (sign_extend:DI
              (match_operand:HI 2 "pic30_register_operand" "r,r,r,r, r,r,r,r, r"))
        )
   )
   (clobber (match_scratch:HI 3                   "=&r,&r,&r,&r,&r,&r,&r,&r,&r"))
   (clobber (reg:CC CC_REG))
  ]
  "0"
  "*
{
  const char *noerrata_patterns[] = {
     /* r,r,r */          \"asr %2,#15,%3\;xor %2,%1,%0\;xor %3,%d1,%d0\;\"
                              \"xor %3,%t1,%t0\;xor %3,%q1,%q0\",

     /* >,r,r */          \"asr %2,#15,%3\;xor %2,%1,%0\;xor %3,%d1,%0\;\"
                              \"xor %3,%t1,%0\;xor %3,%q1,%0\",

     /* >,0,r */          \"asr %2,#15,%3\;xor %2,%s1,%0\;xor %3,%s1,%0\;\"
                              \"xor %3,%s1,%0\;xor %3,%s1,%0\",

     /* >,>,r */          \"asr %2,#15,%3\;xor %2,%1,%0\;xor %3,%1,%0\;\"
                              \"xor %3,%1,%0\;xor %3,%1,%0\",

     /* r,>,r */          \"asr %2,#15,%3\;xor %2,%1,%0\;xor %3,%1,%d0\;\"
                              \"xor %3,%1,%t0\;xor %3,%1,%q0\",

     /* R,r,r */          \"asr %2,#15,%3\;xor %2,%1,%I0\;xor %3,%d1,%I0\;\"
                              \"xor %3,%t1,%I0\;xor %3,%q1,%0\",

     /* R,0,r */          \"asr %2,#15,%3\;xor %2,%1,%I0\;xor %3,%1,%I0\;\"
                              \"xor %3,%1,%I0\;xor %3,%1,%0\",

     /* R,R,r */          \"asr %2,#15,%3\;xor %2,%I1,%I0\;xor %3,%I1,%I0\;\"
                              \"xor %3,%I1,%I0\;xor %3,%1,%0\",

     /* r,R,r */          \"asr %2,#15,%3\;xor %2,%I1,%0\;xor %3,%I1,%d0\;\"
                              \"xor %3,%I1,%t0\;xor %3,%1,%q0\",
  };

  const char *psv_psv_patterns[] = {
     /* r,r,r */          \"asr %2,#15,%3\;xor %2,%1,%0\;xor %3,%d1,%d0\;\"
                              \"xor %3,%t1,%t0\;xor %3,%q1,%q0\",

     /* >,r,r */          \"asr %2,#15,%3\;xor %2,%1,%0\;xor %3,%d1,%0\;\"
                              \"xor %3,%t1,%0\;xor %3,%q1,%0\",

     /* >,0,r */          \"asr %2,#15,%3\;xor %2,%s1,%0\;xor %3,%s1,%0\;\"
                              \"xor %3,%s1,%0\;xor %3,%s1,%0\",

     /* >,>,r */          \"asr %2,#15,%3\;\"
                          \"xor %2,%1,%0\;\"
                          \"nop\;\"
                          \"xor %3,%1,%0\;\"
                          \"nop\;\"
                          \"xor %3,%1,%0\;\"
                          \"nop\;\"
                          \"xor %3,%1,%0\",

     /* r,>,r */          \"asr %2,#15,%3\;\"
                          \"xor %2,%1,%0\;\"
                          \"nop\;\"
                          \"xor %3,%1,%d0\;\"
                          \"nop\;\"
                          \"xor %3,%1,%t0\;\"
                          \"nop\;\"
                          \"xor %3,%1,%q0\",

     /* R,r,r */          \"asr %2,#15,%3\;xor %2,%1,%I0\;xor %3,%d1,%I0\;\"
                              \"xor %3,%t1,%I0\;xor %3,%q1,%0\",

     /* R,0,r */          \"asr %2,#15,%3\;xor %2,%1,%I0\;xor %3,%1,%I0\;\"
                              \"xor %3,%1,%I0\;xor %3,%1,%0\",

     /* R,R,r */          \"asr %2,#15,%3\;\"
                          \"xor %2,%I1,%I0\;\"
                          \"nop\;\"
                          \"xor %3,%I1,%I0\;\"
                          \"nop\;\"
                          \"xor %3,%I1,%I0\;\"
                          \"nop\;\"
                          \"xor %3,%1,%0\",

     /* r,R,r */          \"asr %2,#15,%3\;\"
                          \"xor %2,%I1,%0\;\"
                          \"nop\;\"
                          \"xor %3,%I1,%d0\;\"
                          \"nop\;\"
                          \"xor %3,%I1,%t0\;\"
                          \"nop\;\"
                          \"xor %3,%1,%q0\",
  };

  /* increasing the patterns, means increasing this number too */
  static char szInsns[180];
  int regno;
  rtx x;
  const char **patterns;

  if (pic30_psrd_psrd_errata(NULL,operands[2])) {
    pic30_rtx_nops += 3;
    patterns = psv_psv_patterns;
  } else {
    patterns = noerrata_patterns;
  }

  switch (which_alternative) {
     default: return patterns[which_alternative];

     case 5:  /* R,r,r */
              strcpy(szInsns, patterns[which_alternative]);

              x = XEXP(operands[0],0);
	      if (!pic30_dead_or_set_p(NEXT_INSN(insn), x) &&
                  !find_regno_note(insn, REG_UNUSED, REGNO(x))) {
                 strcat(szInsns,\"\;sub #6,%r0\");
              }
              return szInsns;

     case 6:  /* R,0,r */
              strcpy(szInsns, patterns[which_alternative]);

              x = XEXP(operands[0],0);
	      if (!pic30_dead_or_set_p(NEXT_INSN(insn), x) &&
                  !find_regno_note(insn, REG_UNUSED, REGNO(x))) {
                 strcat(szInsns,\"\;sub #6,%r0\");
              }
              return szInsns;

     case 7:  /* R,R,r */
              strcpy(szInsns, patterns[which_alternative]);

              x = XEXP(operands[0],0);
	      if (!pic30_dead_or_set_p(NEXT_INSN(insn), x) &&
                  !find_regno_note(insn, REG_UNUSED, REGNO(x))) {
                 strcat(szInsns,\"\;sub #6,%r0\");
              }

              x = XEXP(operands[1],0);
	      if (!pic30_dead_or_set_p(NEXT_INSN(insn), x) &&
                  !find_regno_note(insn, REG_UNUSED, REGNO(x))) {
                 strcat(szInsns,\"\;sub #6,%r1\");
              }
              return szInsns;

     case 8:  /* r,R,r */
              strcpy(szInsns, patterns[which_alternative]);

              x = XEXP(operands[1],0);
              if (!pic30_dead_or_set_p(NEXT_INSN(insn), x) &&
                  !find_regno_note(insn, REG_UNUSED, REGNO(x))) {
                 strcat(szInsns,\"\;sub #6,%r1\");
              }
              return szInsns;
   }
}"
   [
     (set_attr "cc" "clobber")
     (set_attr "type" "def")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "*xordihi3_ze_32"
  [(set (match_operand:DI 0 "pic30_DI_mode2_operand"         "=r,>,>,>,&r,R,R,R,&r")
        (xor:DI
           (match_operand:DI 1 "pic30_DI_mode2_operand"       "r,r,0,>, >,r,0,R, R")
           (zero_extend:DI
              (match_operand:HI 2 "pic30_register_operand" "r,r,r,r, r,r,r,r, r"))
        )
   )
   (clobber (reg:CC CC_REG))
  ]
  "0"
  "*
{
  const char *noerrata_patterns[] = {
     /* r,r,r */          \"xor %1,%2,%0\;mov %d1,%d0\;mov.d %t1,%t0\",

     /* >,r,r */          \"xor %1,%2,%0\;mov %d1,%0\;mov.d %t1,%0\",

     /* >,0,r */          \"xor %2,%s1,%s0\;add #8,%r0\",

     /* >,>,r */          \"xor %2,%1,%0\;mov %1,%0\;mov %1,%0\;mov %1,%0\",

     /* r,>,r */          \"xor %2,%1,%0\;mov %1,%d0\;mov.d %1,%t0\",

     /* R,r,r */          \"xor %2,%1,%0\;mov %d1,%Q0\;mov %t1,%R0\;mov %q1,%S0\",

     /* R,0,r */          \"xor %2,%1,%0\",

     /* R,R,r */          \"xor %2,%I1,%I0\;mov %I1,%I0\;mov %I1,%I0\;mov %1,%0\",

     /* r,R,r */          \"xor %2,%1,%0\;mov %Q1,%d0\;mov %R1,%t0\;mov %S1,%q0\"
  };

  const char *psv_psv_patterns[] = {
     /* r,r,r */          \"xor %1,%2,%0\;mov %d1,%d0\;mov.d %t1,%t0\",

     /* >,r,r */          \"xor %1,%2,%0\;mov %d1,%0\;mov.d %t1,%0\",

     /* >,0,r */          \"xor %2,%s1,%s0\;add #8,%r0\",

     /* >,>,r */          \"xor %2,%1,%0\;\"
                          \"nop\;\"
                          \"mov %1,%0\;\"
                          \"nop\;\"
                          \"mov %1,%0\;\"
                          \"nop\;\"
                          \"mov %1,%0\",

     /* r,>,r */          \"xor %2,%1,%0\;\"
                          \"nop\;\"
                          \"mov %1,%d0\;\"
                          \"nop\;\"
                          \"mov.d %1,%t0\",

     /* R,r,r */          \"xor %2,%1,%0\;mov %d1,%Q0\;mov %t1,%R0\;mov %q1,%S0\",

     /* R,0,r */          \"xor %2,%1,%0\",

     /* R,R,r */          \"xor %2,%I1,%I0\;\"
                          \"nop\;\"
                          \"mov %I1,%I0\;\"
                          \"nop\;\"
                          \"mov %I1,%I0\;\"
                          \"nop\;\"
                          \"mov %1,%0\",

     /* r,R,r */          \"xor %2,%1,%0\;\"
                          \"nop\;\"
                          \"mov %Q1,%d0\;\"
                          \"nop\;\"
                          \"mov %R1,%t0\;\"
                          \"nop\;\"
                          \"mov %S1,%q0\"
  };

  const char *psv_psv_movd_patterns[] = {
     /* r,r,r */          \"xor %1,%2,%0\;mov %d1,%d0\;mov.d %t1,%t0\",

     /* >,r,r */          \"xor %1,%2,%0\;mov %d1,%0\;mov.d %t1,%0\",

     /* >,0,r */          \"xor %2,%s1,%s0\;add #8,%r0\",

     /* >,>,r */          \"xor %2,%1,%0\;\"
                          \"mov %1,%0\;\"
                          \"mov %1,%0\;\"
                          \"mov %1,%0\",

     /* r,>,r */          \"xor %2,%1,%0\;\"
                          \"mov %1,%d0\;\"
                          \"mov %1,%t0\;\"
                          \"mov %1,%q0\",

     /* R,r,r */          \"xor %2,%1,%0\;\"
                          \"mov %d1,%Q0\;\"
                          \"mov %t1,%R0\;\"
                          \"mov %q1,%S0\",

     /* R,0,r */          \"xor %2,%1,%0\",

     /* R,R,r */          \"xor %2,%I1,%I0\;\"
                          \"mov %I1,%I0\;\"
                          \"mov %I1,%I0\;\"
                          \"mov %1,%0\",

     /* r,R,r */          \"xor %2,%1,%0\;\"
                          \"mov %Q1,%d0\;\"
                          \"mov %R1,%t0\;\"
                          \"mov %S1,%q0\"
  };

  /* increasing the patterns, means increasing this number too */
  static char szInsns[180];
  int regno;
  rtx x;
  const char **patterns;

  if (pic30_psrd_psrd_errata_movd(NULL,operands[2])) {
    patterns = psv_psv_movd_patterns;
  } else if (pic30_psrd_psrd_errata(NULL,operands[2])) {
    pic30_rtx_nops += 3;
    patterns = psv_psv_patterns;
  } else {
    patterns = noerrata_patterns;
  }

  switch (which_alternative) {
     default: return patterns[which_alternative];

     case 7:  /* R,r,R */
              strcpy(szInsns, patterns[which_alternative]);

              x = XEXP(operands[0],0);
	      if (!pic30_dead_or_set_p(NEXT_INSN(insn), x) &&
                  !find_regno_note(insn, REG_UNUSED, REGNO(x))) {
                 strcat(szInsns,\"\;sub #6,%r0\");
              }

              x = XEXP(operands[2],0);
	      if (!pic30_dead_or_set_p(NEXT_INSN(insn), x) &&
                  !find_regno_note(insn, REG_UNUSED, REGNO(x))) {
                 strcat(szInsns,\"\;sub #6,%r2\");
              }
              return szInsns;

              }
}"
   [
     (set_attr "cc" "clobber")
     (set_attr "type" "def")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "*xordisi3_se_32"
  [(set (match_operand:DI 0 "pic30_DI_mode2_operand"         "=r,>,>,>,&r,R,R,R,&r")
        (xor:DI
           (match_operand:DI 1 "pic30_DI_mode2_operand"       "r,r,0,>, >,r,0,R, R")
           (sign_extend:DI
              (match_operand:SI 2 "pic30_register_operand" "r,r,r,r, r,r,r,r, r"))
        )
   )
   (clobber (match_scratch:HI 3                   "=&r,&r,&r,&r,&r,&r,&r,&r,&r"))
   (clobber (reg:CC CC_REG))
  ]
  "0"
  "*
{
  const char *noerrata_patterns[] = {
     /* r,r,r */          \"asr %d2,#15,%3\;xor %2,%1,%0\;xor %d2,%d1,%d0\;\"
                              \"xor %3,%t1,%t0\;xor %3,%q1,%q0\",

     /* >,r,r */          \"asr %d2,#15,%3\;xor %2,%1,%0\;xor %d2,%d1,%0\;\"
                              \"xor %3,%t1,%0\;xor %3,%q1,%0\",

     /* >,0,r */          \"asr %d2,#15,%3\;xor %2,%s1,%0\;xor %d2,%s1,%0\;\"
                              \"xor %3,%s1,%0\;xor %3,%s1,%0\",

     /* >,>,r */          \"asr %d2,#15,%3\;xor %2,%1,%0\;xor %d2,%1,%0\;\"
                              \"xor %3,%1,%0\;xor %3,%1,%0\",

     /* r,>,r */          \"asr %d2,#15,%3\;xor %2,%1,%0\;xor %d2,%1,%d0\;\"
                              \"xor %3,%1,%t0\;xor %3,%1,%q0\",

     /* R,r,r */          \"asr %d2,#15,%3\;xor %2,%1,%I0\;xor %d2,%d1,%I0\;\"
                              \"xor %3,%t1,%I0\;xor %3,%q1,%0\",

     /* R,0,r */          \"asr %d2,#15,%3\;xor %2,%1,%I0\;xor %d2,%1,%I0\;\"
                              \"xor %3,%1,%I0\;xor %3,%1,%0\",

     /* R,R,r */          \"asr %d2,#15,%3\;xor %2,%I1,%I0\;xor %d2,%I1,%I0\;\"
                              \"xor %3,%I1,%I0\;xor %3,%1,%0\",

     /* r,R,r */          \"asr %d2,#15,%3\;xor %2,%I1,%0\;xor %d2,%I1,%d0\;\"
                              \"xor %3,%I1,%t0\;xor %3,%1,%q0\",
  };

  const char *psv_psv_patterns[] = {
     /* r,r,r */          \"asr %d2,#15,%3\;xor %2,%1,%0\;xor %d2,%d1,%d0\;\"
                              \"xor %3,%t1,%t0\;xor %3,%q1,%q0\",

     /* >,r,r */          \"asr %d2,#15,%3\;xor %2,%1,%0\;xor %d2,%d1,%0\;\"
                              \"xor %3,%t1,%0\;xor %3,%q1,%0\",

     /* >,0,r */          \"asr %d2,#15,%3\;xor %2,%s1,%0\;xor %d2,%s1,%0\;\"
                              \"xor %3,%s1,%0\;xor %3,%s1,%0\",

     /* >,>,r */          \"asr %d2,#15,%3\;\"
                          \"xor %2,%1,%0\;\"
                          \"nop\;\"
                          \"xor %d2,%1,%0\;\"
                          \"nop\;\"
                          \"xor %3,%1,%0\;\"
                          \"nop\;\"
                          \"xor %3,%1,%0\",

     /* r,>,r */          \"asr %d2,#15,%3\;\"
                          \"xor %2,%1,%0\;\"
                          \"nop\;\"
                          \"xor %d2,%1,%d0\;\"
                          \"nop\;\"
                          \"xor %3,%1,%t0\;\"
                          \"nop\;\"
                          \"xor %3,%1,%q0\",

     /* R,r,r */          \"asr %d2,#15,%3\;xor %2,%1,%I0\;xor %d2,%d1,%I0\;\"
                              \"xor %3,%t1,%I0\;xor %3,%q1,%0\",

     /* R,0,r */          \"asr %d2,#15,%3\;xor %2,%1,%I0\;xor %d2,%1,%I0\;\"
                              \"xor %3,%1,%I0\;xor %3,%1,%0\",

     /* R,R,r */          \"asr %d2,#15,%3\;\"
                          \"xor %2,%I1,%I0\;\"
                          \"nop\;\"
                          \"xor %d2,%I1,%I0\;\"
                          \"nop\;\"
                          \"xor %3,%I1,%I0\;\"
                          \"nop\;\"
                          \"xor %3,%1,%0\",

     /* r,R,r */          \"asr %d2,#15,%3\;\"
                          \"xor %2,%I1,%0\;\"
                          \"nop\;\"
                          \"xor %d2,%I1,%d0\;\"
                          \"nop\;\"
                          \"xor %3,%I1,%t0\;\"
                          \"nop\;\"
                          \"xor %3,%1,%q0\",
  };

  /* increasing the patterns, means increasing this number too */
  static char szInsns[180];
  int regno;
  rtx x;
  const char **patterns;

  if (pic30_psrd_psrd_errata(NULL,operands[2])) {
    pic30_rtx_nops += 3;
    patterns = psv_psv_patterns;
  } else {
    patterns = noerrata_patterns;
  }

  switch (which_alternative) {
     default: return patterns[which_alternative];

     case 5:  /* R,r,r */
              strcpy(szInsns, patterns[which_alternative]);

              x = XEXP(operands[0],0);
	      if (!pic30_dead_or_set_p(NEXT_INSN(insn), x) &&
                  !find_regno_note(insn, REG_UNUSED, REGNO(x))) {
                 strcat(szInsns,\"\;sub #6,%r0\");
              }
              return szInsns;

     case 6:  /* R,0,r */
              strcpy(szInsns, patterns[which_alternative]);

              x = XEXP(operands[0],0);
	      if (!pic30_dead_or_set_p(NEXT_INSN(insn), x) &&
                  !find_regno_note(insn, REG_UNUSED, REGNO(x))) {
                 strcat(szInsns,\"\;sub #6,%r0\");
              }
              return szInsns;

     case 7:  /* R,R,r */
              strcpy(szInsns, patterns[which_alternative]);

              x = XEXP(operands[0],0);
	      if (!pic30_dead_or_set_p(NEXT_INSN(insn), x) &&
                  !find_regno_note(insn, REG_UNUSED, REGNO(x))) {
                 strcat(szInsns,\"\;sub #6,%r0\");
              }

              x = XEXP(operands[1],0);
	      if (!pic30_dead_or_set_p(NEXT_INSN(insn), x) &&
                  !find_regno_note(insn, REG_UNUSED, REGNO(x))) {
                 strcat(szInsns,\"\;sub #6,%r1\");
              }
              return szInsns;

     case 8:  /* r,R,r */
              strcpy(szInsns, patterns[which_alternative]);

              x = XEXP(operands[1],0);
              if (!pic30_dead_or_set_p(NEXT_INSN(insn), x) &&
                  !find_regno_note(insn, REG_UNUSED, REGNO(x))) {
                 strcat(szInsns,\"\;sub #6,%r1\");
              }
              return szInsns;
   }
}"
   [
     (set_attr "cc" "clobber")
     (set_attr "type" "def")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "*xordisi3_ze_32"
  [(set (match_operand:DI 0 "pic30_DI_mode2_operand"         "=r,>,>,>,&r,R,R,R,&r")
        (xor:DI
           (match_operand:DI 2 "pic30_DI_mode2_operand"       "r,r,0,>, >,r,0,R, R")
           (zero_extend:DI
              (match_operand:SI 1 "pic30_register_operand" "r,r,r,r, r,r,r,r, r"))
        )
   )
   (clobber (reg:CC CC_REG))
  ]
  "0"
  "*
{
  const char *noerrata_patterns[] = {
     /* r,r,r */          \"xor %1,%2,%0\;xor %d1,%d2,%d0\;mov.d %t1,%t0\",

     /* >,r,r */          \"xor %1,%2,%0\;xor %d1,%d2,%0\;mov.d %t1,%0\",

     /* >,0,r */          \"xor %2,%s1,%0\;xor %d2,%s1,%0\;add #4,%r0\",

     /* >,>,r */          \"xor %2,%1,%0\;xor %d2,%1,%0\;\"
                              \"mov %1,%0\;mov %1,%0\",

     /* r,>,r */          \"xor %2,%1,%0\;xor %d2,%1,%d0\;mov.d %1,%t0\",

     /* R,r,r */          \"xor %2,%1,%I0\;xor %d2,%d1,%D0\;mov %t1,%R0\;mov %q1,%S0\",

     /* R,0,r */          \"xor %2,%1,%I0\;xor %d2,%1,%D0\",

     /* R,R,r */          \"xor %2,%I1,%I0\;xor %d2,%I1,%I0\;mov %I1,%I0\;mov %1,%0\",

     /* r,R,r */          \"xor %2,%I1,%0\;xor %d2,%D1,%d0\;mov %R1,%t0\;mov %S1,%q0\"
  };

  const char *psv_psv_patterns[] = {
     /* r,r,r */          \"xor %1,%2,%0\;xor %d1,%d2,%d0\;mov.d %t1,%t0\",

     /* >,r,r */          \"xor %1,%2,%0\;xor %d1,%d2,%0\;mov.d %t1,%0\",

     /* >,0,r */          \"xor %2,%s1,%0\;xor %d2,%s1,%0\;add #4,%r0\",

     /* >,>,r */          \"xor %2,%1,%0\;\"
                          \"nop\;\"
                          \"xor %d2,%1,%0\;\"
                          \"nop\;\"
                          \"mov %1,%0\;\"
                          \"nop\;\"
                          \"mov %1,%0\",

     /* r,>,r */          \"xor %2,%1,%0\;\"
                          \"nop\;\"
                          \"xor %d2,%1,%d0\;\"
                          \"nop\;\"
                          \"mov.d %1,%t0\",

     /* R,r,r */          \"xor %2,%1,%I0\;xor %d2,%d1,%D0\;mov %t1,%R0\;mov %q1,%S0\",

     /* R,0,r */          \"xor %2,%1,%I0\;xor %d2,%1,%D0\",

     /* R,R,r */          \"xor %2,%I1,%I0\;\"
                          \"nop\;\"
                          \"xor %d2,%I1,%I0\;\"
                          \"nop\;\"
                          \"mov %I1,%I0\;\"
                          \"nop\;\"
                          \"mov %1,%0\",

     /* r,R,r */          \"xor %2,%I1,%0\;\"
                          \"nop\;\"
                          \"xor %d2,%D1,%d0\;\"
                          \"nop\;\"
                          \"mov %R1,%t0\;\"
                          \"nop\;\"
                          \"mov %S1,%q0\"
  };

  const char *psv_psv_movd_patterns[] = {
     /* r,r,r */          \"xor %1,%2,%0\;xor %d1,%d2,%d0\;mov.d %t1,%t0\",

     /* >,r,r */          \"xor %1,%2,%0\;xor %d1,%d2,%0\;mov.d %t1,%0\",

     /* >,0,r */          \"xor %2,%s1,%0\;xor %d2,%s1,%0\;add #4,%r0\",

     /* >,>,r */          \"xor %2,%1,%0\;\"
                          \"xor %d2,%1,%0\;\"
                          \"mov %1,%0\;\"
                          \"mov %1,%0\",

     /* r,>,r */          \"xor %2,%1,%0\;\"
                          \"xor %d2,%1,%d0\;\"
                          \"mov %1,%t0\;\"
                          \"mov %1,%q0\",

     /* R,r,r */          \"xor %2,%1,%I0\;\"
                          \"xor %d2,%d1,%D0\;\"
                          \"mov %t1,%R0\;\"
                          \"mov %q1,%S0\",

     /* R,0,r */          \"xor %2,%1,%I0\;xor %d2,%1,%D0\",

     /* R,R,r */          \"xor %2,%I1,%I0\;\"
                          \"xor %d2,%I1,%I0\;\"
                          \"mov %I1,%I0\;\"
                          \"mov %1,%0\",

     /* r,R,r */          \"xor %2,%I1,%0\;\"
                          \"xor %d2,%D1,%d0\;\"
                          \"mov %R1,%t0\;\"
                          \"mov %S1,%q0\"
  };

  /* increasing the patterns, means increasing this number too */
  static char szInsns[180];
  int regno;
  rtx x;
  const char **patterns;

  if (pic30_psrd_psrd_errata_movd(NULL,operands[2])) {
    patterns = psv_psv_movd_patterns;
  } else if (pic30_psrd_psrd_errata(NULL,operands[2])) {
    pic30_rtx_nops += 3;
    patterns = psv_psv_patterns;
  } else {
    patterns = noerrata_patterns;
  }

  switch (which_alternative) {
     default: return patterns[which_alternative];

     case 7:  /* R,r,R */
              strcpy(szInsns, patterns[which_alternative]);

              x = XEXP(operands[0],0);
	      if (!pic30_dead_or_set_p(NEXT_INSN(insn), x) &&
                  !find_regno_note(insn, REG_UNUSED, REGNO(x))) {
                 strcat(szInsns,\"\;sub #6,%r0\");
              }

              x = XEXP(operands[2],0);
	      if (!pic30_dead_or_set_p(NEXT_INSN(insn), x) &&
                  !find_regno_note(insn, REG_UNUSED, REGNO(x))) {
                 strcat(szInsns,\"\;sub #6,%r2\");
              }
              return szInsns;
              }
}"
   [
     (set_attr "cc" "clobber")
     (set_attr "type" "def")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "xordi3_32"
  [(set (match_operand:DI 0 "pic30_DI_mode2_operand"         "=r,>,>,>,&r,R,R,R,&r")
        (xor:DI
           (match_operand:DI 1 "pic30_register_operand"    "r,r,r,r, r,r,r,r, r")
           (match_operand:DI 2 "pic30_DI_mode2_operand"       "r,r,0,>, >,r,0,R, R")))
   (clobber (reg:CC CC_REG))
  ]
  "0"
  "*
{
  const char *noerrata_patterns[] = {
     /* r,r,r */          \"xor %1,%2,%0\;xor %d1,%d2,%d0\;\"
                              \"xor %t1,%t2,%t0\;xor %q1,%q2,%q0\",

     /* >,r,r */          \"xor %1,%2,%0\;xor %d1,%d2,%0\;\"
                              \"xor %t1,%t2,%0\;xor %q1,%q2,%0\",

     /* >,r,0 */          \"xor %1,%s2,%0\;xor %d1,%s2,%0\;\"
                              \"xor %t1,%s2,%0\;xor %q1,%s2,%0\",

     /* >,r,> */          \"xor %1,%s2,%0\;xor %d1,%2,%0\;\"
                              \"xor %t1,%2,%0\;xor %q1,%2,%0\",

     /* r,r,> */          \"xor %1,%2,%0\;xor %d1,%2,%d0\;\"
                              \"xor %t1,%2,%t0\;xor %q1,%2,%q0\",

     /* R,r,r */          \"xor %1,%2,%I0\;xor %d1,%d2,%I0\;\"
                              \"xor %t1,%t2,%I0\;xor %q1,%q2,%0\",

     /* R,r,0 */          \"xor %1,%2,%I0\;xor %d1,%2,%I0\;\"
                              \"xor %t1,%2,%I0\;xor %q1,%2,%0\",

     /* R,r,R */          \"xor %1,%I2,%I0\;xor %d1,%I2,%I0\;\"
                              \"xor %t1,%I2,%I0\;xor %q1,%2,%0\",

     /* r,r,R */          \"xor %1,%I2,%0\;xor %d1,%I2,%d0\;\"
                              \"xor %t1,%I2,%t0\;xor %q1,%2,%q0\",
  };

  const char *psv_psv_patterns[] = {
     /* r,r,r */          \"xor %1,%2,%0\;xor %d1,%d2,%d0\;\"
                              \"xor %t1,%t2,%t0\;xor %q1,%q2,%q0\",

     /* >,r,r */          \"xor %1,%2,%0\;xor %d1,%d2,%0\;\"
                              \"xor %t1,%t2,%0\;xor %q1,%q2,%0\",

     /* >,r,0 */          \"xor %1,%s2,%0\;xor %d1,%s2,%0\;\"
                              \"xor %t1,%s2,%0\;xor %q1,%s2,%0\",

     /* >,r,> */          \"xor %1,%s2,%0\;\"
                          \"nop\;\"
                          \"xor %d1,%2,%0\;\"
                          \"nop\;\"
                          \"xor %t1,%2,%0\;\"
                          \"nop\;\"
                          \"xor %q1,%2,%0\",

     /* r,r,> */          \"xor %1,%2,%0\;\"
                          \"nop\;\"
                          \"xor %d1,%2,%d0\;\"
                          \"nop\;\"
                          \"xor %t1,%2,%t0\;\"
                          \"nop\;\"
                          \"xor %q1,%2,%q0\",

     /* R,r,r */          \"xor %1,%2,%I0\;xor %d1,%d2,%I0\;\"
                              \"xor %t1,%t2,%I0\;xor %q1,%q2,%0\",

     /* R,r,0 */          \"xor %1,%2,%I0\;xor %d1,%2,%I0\;\"
                              \"xor %t1,%2,%I0\;xor %q1,%2,%0\",

     /* R,r,R */          \"xor %1,%I2,%I0\;\"
                          \"nop\;\"
                          \"xor %d1,%I2,%I0\;\"
                          \"nop\;\"
                          \"xor %t1,%I2,%I0\;\"
                          \"nop\;\"
                          \"xor %q1,%2,%0\",

     /* r,r,R */          \"xor %1,%I2,%0\;\"
                          \"nop\;\"
                          \"xor %d1,%I2,%d0\;\"
                          \"nop\;\"
                          \"xor %t1,%I2,%t0\;\"
                          \"nop\;\"
                          \"xor %q1,%2,%q0\",
  };

  /* increasing the patterns, means increasing this number too */
  static char szInsns[180];
  int regno;
  rtx x;
  const char **patterns;

  if (pic30_psrd_psrd_errata(NULL,operands[2])) {
    pic30_rtx_nops += 3;
    patterns = psv_psv_patterns;
  } else {
    patterns = noerrata_patterns;
  }

  switch (which_alternative) {
     default: return patterns[which_alternative];

     case 5:  /* R,r,r */
              strcpy(szInsns, patterns[which_alternative]);

              x = XEXP(operands[0],0);
	      if (!pic30_dead_or_set_p(NEXT_INSN(insn), x) &&
                  !find_regno_note(insn, REG_UNUSED, REGNO(x))) {
                 strcat(szInsns,\"\;sub #6,%r0\");
              }
              return szInsns;

     case 6:  /* R,r,0 */
              strcpy(szInsns, patterns[which_alternative]);

              x = XEXP(operands[0],0);
	      if (!pic30_dead_or_set_p(NEXT_INSN(insn), x) &&
                  !find_regno_note(insn, REG_UNUSED, REGNO(x))) {
                 strcat(szInsns,\"\;sub #6,%r0\");
              }
              return szInsns;

     case 7:  /* R,r,R */
              strcpy(szInsns, patterns[which_alternative]);

              x = XEXP(operands[0],0);
	      if (!pic30_dead_or_set_p(NEXT_INSN(insn), x) &&
                  !find_regno_note(insn, REG_UNUSED, REGNO(x))) {
                 strcat(szInsns,\"\;sub #6,%r0\");
              }

              x = XEXP(operands[2],0);
	      if (!pic30_dead_or_set_p(NEXT_INSN(insn), x) &&
                  !find_regno_note(insn, REG_UNUSED, REGNO(x))) {
                 strcat(szInsns,\"\;sub #6,%r2\");
              }
              return szInsns;

     case 8:  /* r,r,R */
              strcpy(szInsns, patterns[which_alternative]);

              x = XEXP(operands[2],0);
              if (!pic30_dead_or_set_p(NEXT_INSN(insn), x) &&
                  !find_regno_note(insn, REG_UNUSED, REGNO(x))) {
                 strcat(szInsns,\"\;sub #6,%r2\");
              }
              return szInsns;
   }
}"
   [
     (set_attr "cc" "clobber")
     (set_attr "type" "def")
    (set_attr "op_type" "alu")
  ]
)


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; NOT
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;
;; QImode ;;
;;;;;;;;;;;;

(define_insn "one_cmplqi2_32"
  [(set (match_operand:QI 0 "pic30_mode2_operand"        "=r<>,r<>,R,R")
        (not:QI (match_operand:QI 1 "pic30_mode2_operand" "r,  R<>,r,R<>")))
   (clobber (reg:CC_NZ CC_REG))
  ]
  "(pic30_isa32_target())"
  "com.b %1,%0"
  [
   (set_attr "cc" "math")
   (set_attr "type" "def,defuse,use,use")
    (set_attr "op_type" "alu")
  ]
)

; leave this match_dup, operand 0 will not require a reload (CAW)
(define_insn "*one_cmplqi2_sfr_32"
  [(set (match_operand:QI 0 "pic30_near_operand"        "=U")
        (not:QI (match_dup 0)))
   (clobber (reg:CC_NZ CC_REG))
  ]
  "(pic30_isa32_target())"
  "com.b %0"
  [
    (set_attr "cc" "math")
    (set_attr "type" "etc")
    (set_attr "op_type" "alu")
  ]
)

;;;;;;;;;;;;
;; HImode ;;
;;;;;;;;;;;;

(define_insn "one_cmplhi2_32"
  [(set (match_operand:HI 0 "pic30_mode2_operand"        "=r<>,r<>,R,R")
        (not:HI 
           (match_operand:HI 1 "pic30_mode2_operand" "r,  R<>,r,R<>")))
   (clobber (reg:CC_NZ CC_REG))
  ]
  "(pic30_isa32_target())"
  "com.w %1,%0"
  [
   (set_attr "cc" "math")
   (set_attr "type" "def,defuse,use,use")
    (set_attr "op_type" "alu")
  ]
)

; leave this match_dup, operand 0 will not require a reload (CAW)
(define_insn "*one_cmplhi2_sfr_32"
  [(set (match_operand:HI 0 "pic30_near_operand"        "=U")
        (not:HI (match_dup 0)))
   (clobber (reg:CC_NZ CC_REG))
  ]
  "(pic30_isa32_target())"
  "com.w %0"
  [
    (set_attr "cc" "math")
    (set_attr "type" "etc")
    (set_attr "op_type" "alu")
  ]
)

;;;;;;;;;;;;
;; SImode ;;
;;;;;;;;;;;;

(define_insn "one_cmplsi2_32"
  [(set (match_operand:SI 0 "pic30_register_operand"        "=r")
        (not:SI (match_operand:SI 1 "pic30_register_operand" "r")))
   (clobber (reg:CC_NZ CC_REG))
  ]
  "(pic30_isa32_target())"
  "com.l %1,%0"
  [
   (set_attr "cc" "clobber")
   (set_attr "type" "def")
    (set_attr "op_type" "alu")
  ]
)

; leave this match_dup, operand 0 will not require a reload (CAW)
(define_insn "*one_cmplsi2_sfr_32"
  [(set (match_operand:SI 0 "pic30_near_operand"        "=U")
        (not:SI (match_dup 0)))
   (clobber (reg:CC_NZ CC_REG))
  ]
  "(pic30_isa32_target())"
  "com.l %0"
  [
    (set_attr "cc" "clobber")
    (set_attr "type" "etc")
    (set_attr "op_type" "alu")
  ]
)

;; DImode

(define_insn "one_cmpldi2_32"
  [(set (match_operand:DI 0 "pic30_register_operand"        "=r")
        (not:DI (match_operand:DI 1 "pic30_register_operand" "r")))
   (clobber (reg:CC_NZ CC_REG))
  ]
  "(pic30_isa32_target())"
  "com.l %1,%0\;com.l %d1,%d0"
  [
   (set_attr "cc" "clobber")
   (set_attr "type" "def")
    (set_attr "op_type" "alu")
  ]
)

; leave this match_dup, operand 0 will not require a reload (CAW)
(define_insn "*one_cmpldi2_sfr_32"
  [(set (match_operand:DI 0 "pic30_near_operand"        "=U")
        (not:DI (match_dup 0)))
   (clobber (reg:CC_NZ CC_REG))
  ]
  "(pic30_isa32_target())"
  "com.l %0\;com.l %0+4"
  [
    (set_attr "cc" "clobber")
    (set_attr "op_type" "alu")
  ]
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Find first one
;;
;; Represents one plus the index of the least significant 1-bit in X,
;; represented as an integer of mode M.  (The value is zero if X is
;; zero.)  The mode of X need not be M; depending on the target
;; machine, various mode combinations may be valid.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define_insn "ffshi2_32"
  [(set (match_operand:SI 0 "pic30_register_operand"           "=r,r")
        (unspec:SI [
           (match_operand:HI 1 "pic30_mode2_operand" "r,R<>")
         ] UNSPEC_FF1R))
   (clobber (reg:CC_C CC_REG))
  ]
  "(pic30_isa32_target())"
  "ff1r.w %1,%0"
  [
   (set_attr "cc" "clobber")
   (set_attr "type" "def,defuse")
   (set_attr "op_type" "alu")
  ]
)

(define_insn "ffssi2_32"
  [(set (match_operand:SI 0 "pic30_register_operand"           "=r,r")
        (ffs:SI
           (match_operand:SI 1 "pic30_mode2_operand" "r,R<>")))
   (clobber (reg:CC_C CC_REG))
  ]
  "(pic30_isa32_target())"
  "ff1r.l %1,%0"
  [
   (set_attr "cc" "clobber")
   (set_attr "type" "def,defuse")
   (set_attr "op_type" "alu")
  ]
)

(define_insn "ff1lhi2_32"
  [(set (match_operand:SI 0 "pic30_register_operand"           "=r,r")
        (unspec:SI [
          (match_operand:HI 1 "pic30_mode2_operand" "r,R<>")
        ] UNSPEC_FF1L))
   (clobber (reg:CC_C CC_REG))
  ]
  "(pic30_isa32_target())"
  "ff1l.w %1,%0"
  [
   (set_attr "cc" "clobber")
   (set_attr "type" "def,defuse")
   (set_attr "op_type" "alu")
  ]
)

(define_insn "ff1lsi2_32"
  [(set (match_operand:SI 0 "pic30_register_operand"           "=r,r")
        (unspec:SI [
          (match_operand:SI 1 "pic30_mode2_operand" "r,R<>")
        ] UNSPEC_FF1L))
   (clobber (reg:CC_C CC_REG))
  ]
  "(pic30_isa32_target())"
  "ff1l.l %1,%0"
  [
   (set_attr "cc" "clobber")
   (set_attr "type" "def,defuse")
   (set_attr "op_type" "alu")
  ]
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Shift instructions
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Arithmetic Shift Left instructions
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;
;; QImode ;;
;;;;;;;;;;;;

(define_insn "*ashlqi3_gen_1_32"
  [(set (match_operand:QI 0 "pic30_mode2_operand"           "=r<>,r<>,R,R")
        (ashift:QI (match_operand:QI 1 "pic30_mode2_operand" "r,  R<>,r,R<>")
                   (match_operand:QI 2 "pic30_I_operand"     "I,  I,  I,I")))
   (clobber (reg:CC_NZC CC_REG))
  ]
  "(pic30_isa32_target())"
  "sl.b %1,%0"
  [
   (set_attr "cc" "math")
   (set_attr "type" "def,defuse,use,use")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "*ashlqi3_sfr_1_32"
  [(set (match_operand:QI 0 "pic30_reg_or_near_operand"    "=r,U")
        (ashift:QI (match_operand:QI 1 "pic30_near_operand" "U,0")
                   (match_operand:QI 2 "pic30_I_operand"    "I,I")))
   (clobber (reg:CC_NZC CC_REG))
  ]
  "(pic30_isa32_target())"
  "@
   sl.b %1,%0
   sl.b %0"
  [
   (set_attr "cc" "math")
   (set_attr "type" "def,etc")
    (set_attr "op_type" "alu")
  ]
)

;; For shift by n, we operate in word mode.
;; This is ok, since we are shifting left
;; (zeroes are inserted from the right).
;; However, the condition codes are not useful.

;; for register sl, must first mask off upper bits because
;;   if it was loaded with a mov.b (likely) into a dirty register (possible)
;;   then the word-sized shift value will saturate

(define_insn "ashlqi3_unused_32"
  [(set (match_operand:QI 0 "pic30_register_operand"                 "=r,r")
        (ashift:QI (match_operand:QI 1 "pic30_register_operand"       "r,r")
                   (match_operand:QI 2 "pic30_reg_or_P_operand" "r,P")))
   (clobber (reg:CC_NZC CC_REG))
  ]
  "0"
  "@
    and.w #0xFF,%2\;sl %1,%2,%0
    sl %1,#%2,%0"
  [
   (set_attr "cc" "clobber")
   (set_attr "type" "def")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "ashlqiqi3_32"
  [(set (match_operand:QI 0 "pic30_register_operand"            "=r,r")
        (ashift:QI (match_operand:QI 1 "pic30_register_operand"  "r,r")
                   (match_operand:QI 2 "pic30_reg_or_P_operand"  "r,P")))
   (clobber (reg:CC_NZC CC_REG))
  ]
  "(pic30_isa32_target())"
  "@
   sl.b %1,%2,%0
   sl %1,#%2,%0"
  [
   (set_attr "cc" "clobber")
   (set_attr "type" "def")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "ashlqihi3_32"
  [(set (match_operand:QI 0 "pic30_register_operand"            "=r,r")
        (ashift:QI (match_operand:QI 1 "pic30_register_operand"  "r,r")
                   (match_operand:HI 2 "pic30_reg_or_P_operand"  "r,P")))
   (clobber (reg:CC_NZC CC_REG))
  ]
  "(pic30_isa32_target())"
  "@
   sl.b %1,%2,%0
   sl %1,#%2,%0"
  [
   (set_attr "cc" "clobber")
   (set_attr "type" "def")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "ashlqisi3_32"
  [(set (match_operand:QI 0 "pic30_register_operand"            "=r,r")
        (ashift:QI (match_operand:QI 1 "pic30_register_operand"  "r,r")
                   (match_operand:SI 2 "pic30_reg_or_P_operand"  "r,P")))
   (clobber (reg:CC_NZC CC_REG))
  ]
  "(pic30_isa32_target())"
  "@
   sl.b %1,%2,%0
   sl %1,#%2,%0"
  [
   (set_attr "cc" "clobber")
   (set_attr "type" "def")
    (set_attr "op_type" "alu")
  ]
)

(define_expand "ashlqi3_32"
  [(set (match_operand:QI   0 "pic30_register_operand"   "=r,r,r")
        (ashift:SI
          (match_operand:QI 1 "pic30_register_operand"    "r,r,r")
          (match_operand:HI 2 "pic30_reg_or_imm_operand"  "Kq,i,r")))
  ]
  "(pic30_isa32_target())"
  "{ rtx op2 = operands[2];

     if (GET_CODE(op2) == SUBREG) {
       op2 = XEXP(op2,0);
     }
     if (GET_MODE(op2) == E_QImode) {
       emit(
         gen_ashlqiqi3_32(operands[0],operands[1],op2)
       );
     } else if (GET_MODE(op2) == E_HImode) {
       emit(
         gen_ashlqihi3_32(operands[0],operands[1],op2)
       );
     } else {
       // voidmode, simode, or subreg of some other mode
       emit(
         gen_ashlqisi3_32(operands[0],operands[1],operands[2])
       );
     }
     DONE;
   }"
)


;;;;;;;;;;;;
;; HImode ;;
;;;;;;;;;;;;

(define_insn "*ashlhi3_gen_1_32"
  [(set (match_operand:HI 0 "pic30_mode2_operand"           "=r<>,r<>,R,R")
        (ashift:HI (match_operand:HI 1 "pic30_mode2_operand" "r,  R<>,r,R<>")
                   (match_operand:HI 2 "pic30_I_operand"     "I,  I,  I,I")
        )
   )
   (clobber (reg:CC_NZC CC_REG))
  ]
  "(pic30_isa32_target())"
  "@
   sl.w %1,%0
   sl.w %1,%0
   sl.w %1,%0
   sl.w %1,%0"
 [
  (set_attr "cc" "math")
  (set_attr "type" "def,defuse,use,use")
    (set_attr "op_type" "alu")
 ]
)

(define_insn "*ashlhi3_sfr0_32"
  [(set (match_operand:HI 0 "pic30_register_operand"       "=r")
        (ashift:HI (match_operand:HI 1 "pic30_near_operand" "U")
                   (match_operand:HI 2 "pic30_I_operand"    "I")))
   (clobber (reg:CC_NZC CC_REG))
  ]
  "(pic30_isa32_target())"
  "sl.w %1,%0"
  [
   (set_attr "cc" "math")
   (set_attr "type" "def")
   (set_attr "op_type" "alu")
  ]
)

; leave this match_dup, operand 0 will not require a reload (CAW)
(define_insn "*ashlhi3_sfr1_32"
  [(set (match_operand:HI 0 "pic30_near_operand"           "=U")
        (ashift:HI (match_dup 0)
                   (match_operand:HI 1 "pic30_I_operand"    "I")))
   (clobber (reg:CC_NZC CC_REG))
  ]
  "(pic30_isa32_target())"
  "sl.w %0"
  [
    (set_attr "cc" "math")
  ]
)

(define_insn "ashlhi3_32"
  [(set (match_operand:HI 0 "pic30_register_operand"              "=r,r,r")
        (ashift:HI (match_operand:HI 1 "pic30_register_operand"   "r,r,r")
                   (match_operand:HI 2 "pic30_reg_or_imm_operand" "Kh,i,r")))
   (clobber (reg:CC_NZC CC_REG))
  ]
  "(pic30_isa32_target())"
  "*
{
   switch (which_alternative) {
     case 0:  
       return \"sl.w %1,#%2,%0\";
     case 1: 
       if (INTVAL(operands[2]) < 0)
         return \"asr.w %1,#%J2%%16,%0\";
       else
         return \"sl.w %1,#%2%%16,%0\";
     case 2:
       return \"sl.w %1,%2,%0\";
     default:
       gcc_assert(0);
   }
}"
  [
   (set_attr "cc" "math")
   (set_attr "type" "def")
    (set_attr "op_type" "alu")
  ]
)

;;;;;;;;;;;;
;; SImode ;;
;;;;;;;;;;;;

(define_insn "ashlsihi3_32"
  [(set (match_operand:SI 0 "pic30_register_operand"              "=r,r,r")
        (ashift:SI (match_operand:SI 1 "pic30_register_operand"   "r,r,r")
                   (match_operand:HI 2 "pic30_reg_or_imm_operand" "Ks,i,r")))
   (clobber (reg:CC_NZC CC_REG))
  ]
  "(pic30_isa32_target())"
  "*
{
   switch (which_alternative) {
     case 0:
       return \"sl.l %1,#%2,%0\";
     case 1:
       if (INTVAL(operands[2]) < 0)
         return \"asr.l %1,#%J2%%16,%0\";
       else
         return \"sl.l %1,#%2%%16,%0\";
     case 2:
       return \"sl.l %1,%2,%0\";
     default:
       gcc_assert(0);
   }
}"
  [
   (set_attr "cc" "math")
   (set_attr "type" "def")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "ashlsisi3_32"
  [(set (match_operand:SI 0 "pic30_register_operand"              "=r,r,r")
        (ashift:SI (match_operand:SI 1 "pic30_register_operand"   "r,r,r")
                   (match_operand:SI 2 "pic30_reg_or_imm_operand" "Ks,i,r")))
   (clobber (reg:CC_NZC CC_REG))
  ]
  "(pic30_isa32_target())"
  "*
{
   switch (which_alternative) {
     case 0:
       return \"sl.l %1,#%2,%0\";
     case 1:
       if (INTVAL(operands[2]) < 0)
         return \"asr.l %1,#%J2%%16,%0\";
       else
         return \"sl.l %1,#%2%%16,%0\";
     case 2:
       return \"sl.l %1,%2,%0\";
     default:
       gcc_assert(0);
   }
}"
  [
   (set_attr "cc" "math")
   (set_attr "type" "def")
    (set_attr "op_type" "alu")
  ]
)

(define_expand "ashlsi3_32"
  [(set (match_operand:SI   0 "pic30_register_operand"   "=r,r,r")
        (ashift:SI 
          (match_operand:SI 1 "pic30_register_operand"    "r,r,r")
          (match_operand:HI 2 "pic30_reg_or_imm_operand"  "Ks,i,r")))
  ]
  "(pic30_isa32_target())"
  "{ rtx op2 = operands[2];

     if (GET_CODE(op2) == SUBREG) {
       op2 = XEXP(op2,0);
     }
     if (GET_MODE(op2) == E_SImode) {
       emit(
         gen_ashlsihi3_32(operands[0],operands[1],op2)
       );
     } else {
       // voidmode, himode, or subreg of some other mode
       emit(
         gen_ashlsihi3_32(operands[0],operands[1],operands[2])
       );
     } 
     DONE;
   }"
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; DImode arithmetic shift left.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define_expand "ashldi3_32"
  [(set (match_operand:DI 0 "pic30_register_operand"            "")
        (ashift:DI (match_operand:DI 1 "pic30_register_operand"  "")
                   (match_operand:HI 2 "pic30_reg_or_imm_operand" "")))]
  "(pic30_isa32_target())"
  "
{
  if (GET_CODE(operands[2]) == CONST_INT) {
    if (INTVAL(operands[2]) == 0) {
      emit_insn(
        gen_movdi_32(operands[0], operands[1])
      );
    } else if (INTVAL(operands[2]) == 1) {
      emit_insn(
        gen_ashldi3_imm1_32(operands[0], operands[1], operands[2])
      );
    } else if (INTVAL(operands[2]) >= 2 && INTVAL(operands[2]) <= 31) {
      emit_insn(
        gen_ashldi3_immn_32(operands[0], operands[1], operands[2])
      );
    } else if (INTVAL(operands[2]) >= 32 && INTVAL(operands[2]) <= 63) {
      emit_insn(
        gen_ashldi3_immN_32(operands[0], operands[1], operands[2])
      );
    } else {
      emit_insn(
        gen_movdi_32(operands[0], const0_rtx)
      );
    }
  } else {
    emit_insn(
      gen_ashldi3_reg_32(operands[0],operands[1],operands[2])
    );
  }
  DONE;
}")

(define_insn "ashldi3_imm1_32"
  [(set (match_operand:DI            0 "pic30_register_operand" "=r")
        (ashift:DI (match_operand:DI 1 "pic30_register_operand" " r")
                   (match_operand:HI 2 "immediate_operand"      " i")))
   (clobber (reg:CC_NZC CC_REG))
  ]
  "(pic30_isa32_target() && (INTVAL(operands[2])==1))"
  "sl.l %1,%0\;rlc.l %d1,%d0"
  [
   (set_attr "cc" "clobber")
   (set_attr "type" "def")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "ashldi3_immn_32"
  [(set (match_operand:DI            0 "pic30_register_operand" "=r")
        (ashift:DI (match_operand:DI 1 "pic30_register_operand" " 0")
                   (match_operand:HI 2 "immediate_operand"      " i")))
   (clobber (reg:CC_NZC CC_REG))
  ]
  "(pic30_isa32_target() && (INTVAL(operands[2])<=31))"
  "sl.l %d1,#%2,%d0\;slm %1,#%2,%0"
  [
   (set_attr "cc" "math")
   (set_attr "type" "def")
   (set_attr "op_type" "alu")
  ]
)

(define_insn "ashldi3_immN_32"
  [(set (match_operand:DI            0 "pic30_register_operand" "=r")
        (ashift:DI (match_operand:DI 1 "pic30_register_operand" " 0")
                   (match_operand:HI 2 "immediate_operand"      " i")))
   (clobber (reg:CC_NZC CC_REG))
  ]
  "(pic30_isa32_target() && (INTVAL(operands[2]) >= 32) &&
   (INTVAL(operands[2])<=63))"
  "sl.l %1,#%K2,%d0\;clr.l %0"
  [
   (set_attr "cc" "math")
   (set_attr "type" "def")
   (set_attr "op_type" "alu")
  ]
)

(define_insn "ashldi3_reg_32"
  [(set (match_operand:DI            0 "pic30_register_operand" "=r")
        (ashift:DI (match_operand:DI 1 "pic30_register_operand" " r")
                   (match_operand:HI 2 "pic30_register_operand" " r")))
   (clobber (reg:CC_NZC CC_REG))
  ]
  "(pic30_isa32_target())"
  "sl.l %d1,%2,%d0\;slm.l %1,%2,%0"
  [
   (set_attr "cc" "clobber")
   (set_attr "type" "etc")
    (set_attr "op_type" "alu")
  ]
)


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Logical Shift Right instructions
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;
;; QImode ;;
;;;;;;;;;;;;

(define_insn "lshrqi3_32"
  [(set (match_operand:QI   0 "pic30_mode2_operand" "=r<>,r<>,R,R")
        (lshiftrt:QI 
          (match_operand:QI 1 "pic30_mode2_operand"  "r,  R<>,r,R<>")
          (match_operand:HI 2 "pic30_I_operand"      "I,  I,  I,I")))
   (clobber (reg:CC_NZC CC_REG))
  ]
  "(pic30_isa32_target())"
  "lsr.b %1,%0"
  [
    (set_attr "cc" "math")
    (set_attr "type" "def,defuse,use,use")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "*lshrqi3_sfr0_32"
  [(set (match_operand:QI 0 "pic30_register_operand"         "=r")
        (lshiftrt:QI (match_operand:QI 1 "pic30_near_operand" "U")
                     (match_operand:HI 2 "pic30_I_operand"    "I")))
   (clobber (reg:CC_NZC CC_REG))
  ]
  "(pic30_isa32_target())"
  "lsr.b %1,%0"
  [
    (set_attr "cc" "math")
    (set_attr "type" "def")
    (set_attr "op_type" "alu")
  ]
)

; leave this match_dup, operand 0 will not require a reload (CAW)
(define_insn "*lshrqi3_sfr1_32"
  [(set (match_operand:QI 0 "pic30_near_operand"          "=U")
        (lshiftrt:QI (match_dup 0)
                     (match_operand:HI 1 "pic30_I_operand" "I")))
   (clobber (reg:CC_NZC CC_REG))
  ]
  "(pic30_isa32_target())"
  "lsr.b %0"
  [
    (set_attr "cc" "math")
    (set_attr "op_type" "alu")
  ]
)

;;;;;;;;;;;;
;; HImode ;;
;;;;;;;;;;;;

(define_insn "*lshrhi3_one_DATA_32"
  [(set (match_operand:HI 0 "pic30_mode2_operand"             "=r<>,r<>,R,R")
        (lshiftrt:HI (match_operand:HI 1 "pic30_mode2_operand" "r,  R<>,r,R<>")
                     (match_operand:HI 2 "pic30_I_operand"     "I,  I,  I,I")))
   (clobber (reg:CC_NZC CC_REG))
  ]
  "(pic30_isa32_target())"
  "lsr.w %1,%0"
  [
   (set_attr "cc" "math")
   (set_attr "type" "def,defuse,use,use")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "lshrhi3_32"
 [(set (match_operand:HI   0 "pic30_mode2_operand"       "=rR<>,rR<>")
       (lshiftrt:HI
         (match_operand:HI 1 "pic30_mode2_operand"       " rR<>,rR<>")
         (match_operand:HI 2 "pic30_reg_or_PN_operand"   " r   ,P")))
   (clobber (reg:CC_NZC CC_REG))
 ]
  "(pic30_isa32_target())"
  "@
   lsr.w %1,%2,%0
   lsr.w %1,#%2,%0"
  [ 
    (set_attr "cc" "math,math")
    (set_attr "type" "def,def")
    (set_attr "op_type" "alu")
  ] 
) 

(define_insn "*lshrhi3_sfr0_32"
  [(set (match_operand:HI 0 "pic30_register_operand"          "=r,r")
        (lshiftrt:HI (match_operand:HI 1 "pic30_data_operand" " U,T")
                     (match_operand:HI 2 "pic30_I_operand"    " I,I")))
   (clobber (reg:CC_NZC CC_REG))
  ]
  "(pic30_isa32_target())"
  "@
   lsr.w %1,WREG
   mov.w %1,%0\;lsr.w %0,%0"
  [
   (set_attr "cc" "math")
   (set_attr "type" "def")
    (set_attr "op_type" "alu")
  ]
)

; leave this match_dup, operand 0 will not require a reload (CAW)
(define_insn "*lshrhi3_sfr1_32"
  [(set (match_operand:HI 0 "pic30_near_operand"          "=U")
        (lshiftrt:HI (match_dup 0)
                     (match_operand:HI 1 "pic30_I_operand" "I")))
   (clobber (reg:CC_NZC CC_REG))
  ]
  "(pic30_isa32_target())"
  "lsr.w %0"
  [(set_attr "cc" "math")])

(define_insn "lshrhi3_32_old"
  [(set (match_operand:HI 0 "pic30_register_operand"                "=r,r,r")
        (lshiftrt:HI (match_operand:HI 1 "pic30_register_operand"   "r,r,r")
                     (match_operand:HI 2 "pic30_reg_or_imm_operand" "Kh,i,r")))
   (clobber (reg:CC_NZC CC_REG))
  ]
  "(pic30_isa32_target())"
  "@
   lsr.w %1,#%2,%0
   clr.w %0
   lsr.w %1,%2,%0"
  [
   (set_attr "cc" "math,change0,math")
   (set_attr "type" "def")
    (set_attr "op_type" "alu")
  ]
)

;; P32EXT
(define_insn "hipart_p32ext_32"
  [(set (match_operand:HI       0 "pic30_register_operand")
        (unspec:HI [
          (match_operand:P32EXT 1 "pic30_register_operand")
        ] UNSPEC_EXTHI))]
  "(pic30_isa32_target())"
  "lsr.l %1,#15,%0"
)

;;;;;;;;;;;;
;; SImode ;;
;;;;;;;;;;;;

(define_expand "lshrsi3_32"
  [(set (match_operand:SI 0 "pic30_register_operand"              "")
        (lshiftrt:SI (match_operand:SI 1 "pic30_register_operand" "")
                     (match_operand:HI 2 "pic30_reg_or_imm_operand"   "")))]
  "(pic30_isa32_target())"
  "
{
  if (!pic30_register_operand(operands[1],GET_MODE(operands[0])))
    FAIL;
  if (!pic30_register_operand(operands[0],GET_MODE(operands[0])))
    FAIL;
  if (GET_CODE(operands[2]) == CONST_INT) {
    if (INTVAL(operands[2]) == 0) {
      emit_insn(
        gen_movsi_32(operands[0], operands[1])
      );
    } else if (INTVAL(operands[2]) >= 1 && INTVAL(operands[2]) <= 31) {
      emit_insn(
        gen_lshrsi3_imm_32(operands[0], operands[1], operands[2])
      );
    } else {
      emit_insn(
        gen_movsi(operands[0], const0_rtx)
      );
    }
  } else {
    emit_insn(
      gen_lshrsi3_reg_32(operands[0],operands[1],operands[2])
    );
  }
  DONE;
}")

(define_insn "lshrsi3_imm_32"
  [(set (match_operand:SI 0 "pic30_register_operand"             "=r,r")
        (lshiftrt:SI (match_operand:SI 1 "pic30_register_operand" "r,r")
                     (match_operand:HI 2 "immediate_operand"      "Ks,i")))
   (clobber (reg:CC_NZC CC_REG))
  ]
  "(pic30_isa32_target())"
  "@
   lsr.l %1,#%2,%0
   clr.l %0"
  [
   (set_attr "cc" "clobber")
   (set_attr "type" "def")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "lshrsi3_reg_32"
  [(set (match_operand:SI 0 "pic30_register_operand"             "=r")
        (lshiftrt:SI (match_operand:SI 1 "pic30_register_operand" "r")
                     (match_operand:HI 2 "pic30_register_operand" "r"))
   )
   (clobber (reg:CC_NZC CC_REG))
  ]
  "(pic30_isa32_target())"
  "lsr.l %1,%2,%0"
  [
   (set_attr "cc" "clobber")
   (set_attr "type" "etc")
    (set_attr "op_type" "alu")
  ]
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; DImode logical shift right.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define_insn "lshrdi3_32"
  [(set (match_operand:DI 0 "pic30_register_operand"               "=r,r")
        (lshiftrt:DI (match_operand:DI 1 "pic30_register_operand"   "r,r")
                     (match_operand:HI 2 "pic30_reg_or_imm_operand" "i,r")))
   (clobber (reg:CC_NZC CC_REG))
  ]
  "(pic30_isa32_target())"
  "*
   if (which_alternative == 0) {
     if (INTVAL(operands[2]) < 32) {
       return \"lsr.l %1,#%2,%0\;lsrm.l %d1,#%2,%d0\";
     } else {
       return \"lsr.l %d1,#%K2,%0\;movs.l #0,%d0\";
     }
   } else {
     return \"lsr.l %1,%2,%0\;lsrm.l %d1,%2,%d0\";
   }
  "
  [
    (set_attr "cc" "clobber")
    (set_attr "type" "def")
    (set_attr "op_type" "alu")
  ]
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Arithmetic shift right.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; QImode arithmetic shift right.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define_insn "ashrqi3_32"
  [(set 
     (match_operand:QI   0 "pic30_mode2_operand" "=r,r,  R<>,R<>,&r")
     (ashiftrt:QI 
       (match_operand:QI 1 "pic30_mode2_operand"  "r,R<>,r,  R<>,r")
       (match_operand:QI 2 "immediate_operand"    "I,I,  I,  I,  i")))
   (clobber (reg:CC_NZC CC_REG))
  ]
  "(pic30_isa32_target())"
  "@
   asr.bz %1,%0
   asr.bz %1,%0
   asr.b %1,%0
   asr.b %1,%0
   mov.bz #2,%0\;asr.b %1,%0,%0"
  [
   (set_attr "cc" "math")
   (set_attr "type" "def,defuse,use,use,def")
   (set_attr "op_type" "alu")
  ]
)

(define_insn "*ashrqi3_one_APSV_32"
  [(set (match_operand:QI 0 "pic30_mode2_operand"             "=r<>,r<>,R,R")
        (ashiftrt:QI 
           (match_operand:QI 1 "pic30_mode2_APSV_operand" "r,  R<>,r,R<>")
           (match_operand:QI 2 "pic30_I_operand"       "I,  I,  I,I")))
   (clobber (reg:CC_NZC CC_REG))
  ]
  "pic30_assert(0)"
  "asr.b %1,%0"
  [
   (set_attr "cc" "math")
   (set_attr "type" "def,defuse,use,use")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "*ashrqi3_sfr_32"
  [(set (match_operand:QI 0 "pic30_reg_or_near_operand"      "=r,U")
        (ashiftrt:QI (match_operand:QI 1 "pic30_near_operand" "U,0")
                     (match_operand:QI 2 "pic30_I_operand"    "I,I")))
   (clobber (reg:CC_NZC CC_REG))
  ]
  "(pic30_isa32_target())"
  "@
   asr.b %1,%0
   asr.b %0"
  [
   (set_attr "cc" "math")
   (set_attr "type" "def,etc")
    (set_attr "op_type" "alu")
  ]
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; HImode arithmetic shift right.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define_insn "*ashrhi3_one_DATA_32"
  [(set (match_operand:HI 0 "pic30_mode2_operand"             "=r<>,r<>,R,R")
        (ashiftrt:HI (match_operand:HI 1 "pic30_mode2_operand" "r,  R<>,r,R<>")
                     (match_operand:HI 2 "pic30_I_operand"     "I,  I,  I,I")))
   (clobber (reg:CC_NZC CC_REG))
  ]
  "(pic30_isa32_target())"
  "asr.w %1,%0"
  [
   (set_attr "cc" "math")
   (set_attr "type" "def,defuse,use,use")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "*ashrhi3_one_APSV_32"
  [(set (match_operand:HI 0 "pic30_mode2_operand"        "=r<>,r<>,R,R")
        (ashiftrt:HI 
           (match_operand:HI 1 "pic30_mode2_APSV_operand" "r,  R<>,r,R<>")
           (match_operand:HI 2 "pic30_I_operand"          "I,  I,  I,I")))
   (clobber (reg:CC_NZC CC_REG))
  ]
  "pic30_assert(0)"
  "asr.w %1,%0"
  [
   (set_attr "cc" "math")
   (set_attr "type" "def,defuse,use,use")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "*ashrhi3_sfr_32"
  [(set (match_operand:HI 0 "pic30_reg_or_near_operand"      "=r,U")
        (ashiftrt:HI (match_operand:HI 1 "pic30_near_operand" "U,0")
                     (match_operand:HI 2 "pic30_I_operand"    "I,I")))
   (clobber (reg:CC_NZC CC_REG))
  ]
  "(pic30_isa32_target())"
  "@
   asr.w %1,%0
   asr.w %0"
  [
   (set_attr "cc" "math")
   (set_attr "type" "def,etc")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "ashrhi3_32"
  [(set (match_operand:HI 0 "pic30_register_operand"               "=r,r,r")
        (ashiftrt:HI (match_operand:HI 1 "pic30_register_operand"   "r,r,r")
                     (match_operand:HI 2 "pic30_reg_or_imm_operand" "Kh,i,r")))
   (clobber (reg:CC_NZC CC_REG))
  ]
  "(pic30_isa32_target())"
  "*
   {
     switch (which_alternative) {
       case 0: return \"asr.w %1,#%2,%0\";
       case 1: if (INTVAL(operands[2]) < 0)
                 return \"sl.w %1,#%J2%%16,%0\";
               else return \"asr.w %1,#%2%%16,%0\";
       case 2: return \"asr.w %1,%2,%0\";
       default: gcc_assert(0);
     }
   }"
  [
   (set_attr "cc" "math")
   (set_attr "type" "def")
   (set_attr "op_type" "alu")
  ]
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; SImode arithmetic shift right.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define_insn "ashrsi3_32"
  [(set 
     (match_operand:SI   0 "pic30_mode2_operand"      "=rR<>,rR<>")
     (ashiftrt:SI 
       (match_operand:SI 1 "pic30_mode2_operand"      " rR<>,rR<>")
       (match_operand:HI 2 "pic30_reg_or_imm_operand" " i,   r")))
   (clobber (reg:CC_NZC CC_REG))
  ]
  "(pic30_isa32_target())"
  "@
   asr.l %1,#%2,%0
   asr.l %1,%2,%0"
  [
    (set_attr "cc" "math")
    (set_attr "type" "def")
    (set_attr "op_type" "alu")
  ]
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; DImode arithmetic shift right.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define_insn "ashrdi3_32"
  [(set (match_operand:DI 0 "pic30_register_operand"               "=r,r")
        (ashiftrt:DI (match_operand:DI 1 "pic30_register_operand"   "r,r")
                     (match_operand:HI 2 "pic30_reg_or_imm_operand" "i,r")))
   (clobber (reg:CC_NZC CC_REG))
  ]
  "(pic30_isa32_target())"
  "*
   if (which_alternative == 0) {
     if (INTVAL(operands[2]) < 32) {
       return \"lsr.l %1,#%2,%0\;asrm.l %d1,#%2,%d0\";
     } else {
       return \"lsr.l %d1,#%K2,%0\;asr.l %d1,#31,%d0\";
     }
   } else {
     return \"lsr.l %1,%2,%0\;asrm.l %d1,%2,%d0\";
   }
  "
  [
    (set_attr "cc" "clobber")
    (set_attr "type" "def")
    (set_attr "op_type" "alu")
  ]
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; rotate instructions
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; rotate left
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;
;; QImode ;;
;;;;;;;;;;;;

(define_insn "*rotlqi3_one_DATA_32"
  [(set (match_operand:QI 0 "pic30_mode2_operand"    "=r<>,r<>,R,R")
        (rotate:QI 
           (match_operand:QI 1 "pic30_mode2_operand" " r,  R<>,r,R<>")
           (match_operand:QI 2 "pic30_I_operand"     " I,  I,  I,I")))
   (clobber (reg:CC_NZ CC_REG))
  ]
  "(pic30_isa32_target())"
  "rlnc.b %1,%0"
  [
   (set_attr "cc" "math")
   (set_attr "type" "def,defuse,use,use")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "*rotlqi3_one_APSV_32"
  [(set (match_operand:QI 0 "pic30_mode2_operand"         "=r<>,r<>,R,R")
        (rotate:QI 
           (match_operand:QI 1 "pic30_mode2_APSV_operand" " r,  R<>,r,R<>")
           (match_operand:QI 2 "pic30_I_operand"          " I,  I,  I,I")))
   (clobber (reg:CC_NZ CC_REG))
  ]
  "pic30_assert(0)"
  "rlnc.b %1,%0"
  [
   (set_attr "cc" "math")
   (set_attr "type" "def,defuse,use,use")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "*rotlqi3_sfr_32"
  [(set (match_operand:QI 0 "pic30_reg_or_near_operand"    "=r,U")
        (rotate:QI (match_operand:QI 1 "pic30_near_operand" "U,0")
                   (match_operand:QI 2 "pic30_I_operand"    "I,I")))
   (clobber (reg:CC_NZ CC_REG))
  ]
  "(pic30_isa32_target())"
  "@
   rlnc.b %1,%0
   rlnc.b %0"
  [
   (set_attr "cc" "math")
   (set_attr "type" "def,etc")
    (set_attr "op_type" "alu")
  ]
)

;;;;;;;;;;;;
;; HImode ;;
;;;;;;;;;;;;

(define_insn "*rotlhi3_one_DATA_32"
  [(set (match_operand:HI 0 "pic30_mode2_operand"           "=r<>,r<>,R,R")
        (rotate:HI (match_operand:HI 1 "pic30_mode2_operand" "r,  R<>,r,R<>")
                   (match_operand:HI 2 "pic30_I_operand"     "I,  I,  I,I")))
   (clobber (reg:CC_NZ CC_REG))
  ]
  "(pic30_isa32_target())"
  "rlnc.w %1,%0"
  [
   (set_attr "cc" "math")
   (set_attr "type" "def,defuse,use,use")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "*rotlhi3_one_APSV_32"
  [(set (match_operand:HI 0 "pic30_mode2_operand"           "=r<>,r<>,R,R")
        (rotate:HI 
           (match_operand:HI 1 "pic30_mode2_APSV_operand"   " r,  R<>,r,R<>")
           (match_operand:HI 2 "pic30_I_operand"            " I,  I,  I,I")))
   (clobber (reg:CC_NZ CC_REG))
  ]
  ""
  "rlnc.w %1,%0"
  [
   (set_attr "cc" "math")
   (set_attr "type" "def,defuse,use,use")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "*rotlhi3_sfr_32"
  [(set (match_operand:HI 0 "pic30_reg_or_near_operand"    "=r,U")
        (rotate:HI (match_operand:HI 1 "pic30_near_operand" "U,0")
                   (match_operand:HI 2 "pic30_I_operand"    "I,I")))
   (clobber (reg:CC_NZ CC_REG))
  ]
  "(pic30_isa32_target())"
  "@
   rlnc %1,%0
   rlnc %1"
  [
   (set_attr "cc" "math")
   (set_attr "type" "def,etc")
    (set_attr "op_type" "alu")
  ]
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; rotate right
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;
;; QImode ;;
;;;;;;;;;;;;

(define_insn "*rotrqi3_one_32"
  [(set (match_operand:QI 0 "pic30_mode2_operand"   "=r<>,r<>,R,R")
        (rotatert:QI 
          (match_operand:QI 1 "pic30_mode2_operand"  "r,  R<>,r,R<>")
          (match_operand:QI 2 "pic30_I_operand"      "I,  I,  I,I")))
   (clobber (reg:CC_NZ CC_REG))
  ]
  "(pic30_isa32_target())"
  "rrnc.b %1,%0"
  [
    (set_attr "cc" "math")
    (set_attr "type" "def,defuse,use,use")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "*rotrqi3_sfr_32"
  [(set (match_operand:QI 0 "pic30_reg_or_near_operand"      "=r,U")
        (rotatert:QI (match_operand:QI 1 "pic30_near_operand" "U,0")
                     (match_operand:QI 2 "pic30_I_operand"    "I,I")))
   (clobber (reg:CC_NZ CC_REG))
  ]
  "(pic30_isa32_target())"
  "@
   rrnc.b %1,%0
   rrnc.b %1"
  [
    (set_attr "cc" "math")
    (set_attr "type" "def,etc")
    (set_attr "op_type" "alu")
  ]
)

;;;;;;;;;;;;
;; HImode ;;
;;;;;;;;;;;;

(define_insn "*rotrhi3_one_DATA_32"
  [(set (match_operand:HI 0 "pic30_mode2_operand"             "=r<>,r<>,R,R")
        (rotatert:HI (match_operand:HI 1 "pic30_mode2_operand" "r,  R<>,r,R<>")
                     (match_operand:HI 2 "pic30_I_operand"     "I,  I,  I,I")))
   (clobber (reg:CC_NZ CC_REG))
  ]
  "(pic30_isa32_target())"
  "rrnc.w %1,%0"
  [
   (set_attr "cc" "math")
   (set_attr "type" "def,defuse,use,use")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "*rotrhi3_one_32"
  [(set (match_operand:HI 0 "pic30_mode2_operand"         "=r<>,r<>,R,R")
        (rotatert:HI 
           (match_operand:HI 1 "pic30_mode2_APSV_operand" " r,  R<>,r,R<>")
           (match_operand:HI 2 "pic30_I_operand"           "I,  I,  I,I")))
   (clobber (reg:CC_NZ CC_REG))
  ]
  "(pic30_isa32_target())"
  "rrnc.w %1,%0"
  [
   (set_attr "cc" "math")
   (set_attr "type" "def,defuse,use,use")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "*rotrhi3_sfr_32"
  [(set (match_operand:HI 0 "pic30_reg_or_near_operand"      "=r,U")
        (rotatert:HI (match_operand:HI 1 "pic30_near_operand" "U,0")
                     (match_operand:HI 2 "pic30_I_operand"    "I,I")))
   (clobber (reg:CC_NZ CC_REG))
  ]
  "(pic30_isa32_target())"
  "@
   rrnc.w %1,%0
   rrnc.w %1"
  [
   (set_attr "cc" "math")
   (set_attr "type" "def,etc")
    (set_attr "op_type" "alu")
  ]
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Special cases of bit-field insns which we should
;; recognize in preference to the general case.
;; These handle aligned 8-bit and 16-bit fields,
;; which can usually be done with move instructions.
;    dsPIC30: t.b.d.
;********************

;; Bit field instructions, general cases.
;; "o,d" constraint causes a nonoffsettable memref to match the "o"
;; so that its address is reloaded.

(define_insn "bfins_32"
  [(set (zero_extract:SI
          (match_operand:SI 0 "pic30_mode2_or_near_operand" "+U, Rr,Rr,U")
          (match_operand:SI 1 "immediate_operand"           " i, i, i, i")
          (match_operand:SI 2 "immediate_operand"           " i, i, i, i"))
        (match_operand:SI 3 "pic30_reg_or_lit8"             " r, r, i, ???i"))
   (clobber (match_scratch:SI 4                             "=X, X, X, r"))
  ]
  "(pic30_isa32_target())"
  "*
{
  switch (which_alternative)
  {
    case 1: return \"bfins.l #%2,#%1,%3,%0\";
    case 2: return \"bfins.l #%2,#%1,#%3,%0\";
    case 3:
    case 0:  { /* U, i, i, r */
      /* Check to see if it is near+offset */
      if (GET_CODE(XEXP(operands[0],0)) == CONST) {
        if(GET_CODE(XEXP(XEXP(operands[0],0),0)) == PLUS) {
          int offset;
#if 0
          /* funky code for classic devices... */
          offset = INTVAL(XEXP(XEXP(XEXP(operands[0],0),0),1)); 
          if (offset & 1) {
            if (which_alternative == 3) {
              return \"mov #%3,%4\;bfins #%2+8,#%1,%4,%0-1\";
            } else {
              return \"bfins #%2+8,#%1,%3,%0-1\";
            }
          } else {
            if (which_alternative == 3) {
              return \"mov #%3,%4\;bfins #%2,#%1,%4,%0\";   
            } else {
              return \"bfins #%2,#%1,%3,%0\";
            }
          }
#else 
          if (which_alternative == 3) {
            return \"mov.sl #%3,%4\;bfins.l #%2,#%1,%4,%0\";
          } else {
            return \"bfins.l #%2,#%1,%4,%0\";
          }
#endif

        } else {
          gcc_assert(0);
        }
      } else {
        return \"bfins.l #%2,#%1,%3,%0\";
      } 
    }
    default:
      gcc_assert(0);
  }       
}" 
) 


;(define_insn "insv_32"
;  [(set (zero_extract:SI    
;          (match_operand    0 "pic30_mode2_or_near_operand" "+Rr,U")
;          (match_operand:SI 1 "immediate_operand"           " i, i")
;          (match_operand:SI 2 "immediate_operand"           " i, i"))
;        (match_operand:SI 3 "pic30_register_operand"        " r, r"))
;  ]
;  "(pic30_isa32_target())"
;  "bfins.l #%2,#%1,%3,%0"
;)

(define_expand "insv_32"
  [(set (zero_extract:SI
          (match_operand:SI 0 "pic30_mode2_or_near_operand" "+RrU")
          (match_operand:SI 1 "immediate_operand"           "i")
          (match_operand:SI 2 "immediate_operand"           "i"))
        (match_operand:SI 3 "immediate_operand"             "i"))]
  "(pic30_isa32_target())"
  "
{ int n;
  enum machine_mode mode;
  if ((pic30_register_operand(operands[0], VOIDmode) ||
       pic30_mode2_or_near_operand(operands[0], HImode)) &&
      (INTVAL(operands[1]) > 1) &&
      pic30_reg_or_lit8(operands[3], VOIDmode) &&
      (INTVAL(operands[1])+INTVAL(operands[2])<33)
     ) {
    rtx op0 = operands[0];

    if (!pic30_mode2_or_near_operand(op0,VOIDmode)) {
      op0 = force_reg(GET_MODE(op0),op0);
    }
    emit(
      gen_bfins_32(op0,operands[1],operands[2],operands[3])
    );
    if (op0 != operands[0]) {
      emit_move_insn(operands[0], op0);
    }
    DONE;
  }

  n = 4;
  mode = GET_MODE(operands[0]);
  if (pic30_volatile_operand(operands[0], mode)) n = 1;
  if ((INTVAL(operands[1]) <= n) && (GET_CODE(operands[3]) == CONST_INT) &&
      /* pic30_reg_or_near_operand is already called, but if optimization is
         on, constant addresses are forced into a register so that 'cse can
         get a chance to see them' (in fn memory_address)... thats okay, cse
         will turn them back to constants we hope */
      (pic30_reg_or_near_operand(operands[0],VOIDmode)) &&
      ((mode == SImode) || (mode == HImode) || (mode == QImode)))
  {  int mask;
     rtx mask_rtx;
     int i;
     rtx modifybit = operands[0];

     /* actually operand [3] does not necessarily fit into our bitfield...
        *SIGH* */

     for (i = 0; i < INTVAL(operands[1]); i++) {
       mask =  (1 << (INTVAL(operands[2]) +i));
       if ((INTVAL(operands[3]) & (1 << i)) == 0) {
          if (pic30_mode2_operand(operands[0],VOIDmode)) {
            mask_rtx = gen_rtx_CONST_INT(SImode, ~mask);
            emit(
              gen_bitclrsi_16(operands[0], operands[0], mask_rtx)
            );
          } else {
            if (mode == SImode) {
              mask_rtx = gen_rtx_CONST_INT(SImode, ~mask);
              emit(
                gen_bitclrsi_sfr_16(operands[0], mask_rtx)
              );
            } else if (mode == HImode) {
              mask_rtx = gen_rtx_CONST_INT(HImode, ~mask);
              emit(
                gen_bitclrhi_sfr_16(operands[0], mask_rtx)
              );
            } else if (mode == QImode) {
              if (mask > 0x80) {
                FAIL;
                break;
              }
              mask_rtx = gen_rtx_CONST_INT(HImode, ~mask);
              emit(
                gen_bitclrqi_sfr_16(modifybit, mask_rtx)
              );
            }
          }
       } else {
          if (pic30_mode2_operand(operands[0],VOIDmode)) {
            mask_rtx = gen_rtx_CONST_INT(HImode, mask);
            emit(
              gen_bitsetsi_16(operands[0], operands[0], mask_rtx)
            );
          } else {
            if (mode == SImode) {
              mask_rtx = gen_rtx_CONST_INT(SImode, mask);
              emit(
                gen_bitsetsi_sfr_16(operands[0], mask_rtx)
              );
            } else if (mode == HImode) {
              mask_rtx = gen_rtx_CONST_INT(HImode, mask);
              emit(
                gen_bitsethi_sfr_16(operands[0], mask_rtx)
              );
            } else {
              if (mask > 0x80) {
                FAIL;
                break;
              }
              mask_rtx = gen_rtx_CONST_INT(HImode, mask);
              emit(
                gen_bitsetqi_sfr_16(modifybit, mask_rtx)
              );
            }
          }
       }
     }
     DONE;
  }
  else FAIL;
}"
)


;; (define_insn "extv_32" ...


(define_insn "extzvqi_32"
  [(set (match_operand:QI   0 "pic30_register_operand"      "=r,r")
        (zero_extract:QI  
          (match_operand:QI 1 "pic30_mode2_or_near_operand" "Rr,U")
          (match_operand    2 "immediate_operand"           " i,i")
          (match_operand    3 "immediate_operand"           " i,i")))
   (clobber (reg:CC_NZ CC_REG))
  ]
  "((pic30_isa32_target()) && (INTVAL(operands[2]) + INTVAL(operands[3]) == 8))"
  "mov.w #%3,%0\;asr.b %1,%0,%0"
)

(define_insn "extzvhisi_32"
  [(set (subreg:SI 
          (match_operand:HI   0 "pic30_register_operand"      "=r,r") 0)
        (zero_extract:SI
          (subreg:SI 
            (match_operand:HI 1 "pic30_mode2_or_near_operand" "R<>r,U") 0)
          (match_operand      2 "immediate_operand"           " i,i")
          (match_operand      3 "immediate_operand"           " i,i")))]
  "(pic30_isa32_target())"
  "*
{
  switch(which_alternative)
  {
    case 0:  return \"bfext.w #%3,#%2,%1,%0\";
    case 1:  {
#if 0
      /* Check to see if the near operand has an offset */
      if (GET_CODE(XEXP(operands[1],0)) == CONST) {
        if(GET_CODE(XEXP(XEXP(operands[1],0),0)) == PLUS) {
          int bit_offset,offset;
          offset = INTVAL(XEXP(XEXP(XEXP(operands[1],0),0),1));
          bit_offset = INTVAL(operands[3]);
          if (offset & 1) {
            bit_offset = (offset*8 + bit_offset) & 15;
            return \"bfext.w #%3+8,#%2,%1-1,%0\";
          } else {
            return \"bfext.w #%3,#%2,%1,%0\";
          }
        }
#else
      // not sure why we are doing the above at all...
      if (0) {
#endif
      } else {
        return \"bfext.w #%3,#%2,%1,%0\";
      }
    }
    default:
      gcc_assert(0);
  }
}"
)

(define_insn "extzv_32"
  [(set (match_operand:SI   0 "pic30_register_operand"      "=r,r")
        (zero_extract:SI  
          (match_operand:SI 1 "pic30_mode2_or_near_operand" "Rr,U")
          (match_operand:SI 2 "immediate_operand"           " i,i")
          (match_operand:SI 3 "immediate_operand"           " i,i")))]
  "(pic30_isa32_target())"
  "*
{
  static char buffer[128];

  switch(which_alternative)
  {
    case 0:  return \"bfext.l #%3,#%2,%1,%0\";
    case 1:  {
#if 0
      /* Check to see if the near operand has an offset */
      if (GET_CODE(XEXP(operands[1],0)) == CONST) {
        if(GET_CODE(XEXP(XEXP(operands[1],0),0)) == PLUS) {
          int bit_offset,offset;
          offset = INTVAL(XEXP(XEXP(XEXP(operands[1],0),0),1));
          bit_offset = INTVAL(operands[3]);
          if (offset != 0) {
            bit_offset = (offset*8 + bit_offset) & 31;
            sprintf(buffer,\"bfext.l #%d,#%%2,%%1-%d,%%0\",
                    bit_offset, offset);
            return buffer;
          } else {
            return \"bfext.l #%3,#%2,%1,%0\";
          }
        }
#else
      // not sure why we are doing the above at all...
      if (0) {
#endif
      } else {
        return \"bfext.l #%3,#%2,%1,%0\";
      }
    }
    default:
      gcc_assert(0);
  }
}"
)

;; Now recognize bit field insns that operate on registers
;; (or at least were intended to do so).
;[unnamed only]

;; Special patterns for optimizing bit-field instructions.
;**************************************

; cc status test ops n.a. on dsPIC30 ......... e.g. "sleu" on 68k:
;  [(set (match_operand:QI 0 "pic30_general_operand" "=d")
;        (leu (cc0) (const_int 0)))]
;  ""
;  "* cc_status = cc_prev_status;
;     return \"sls %0\"; ")


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Basic conditional jump instructions.
;; Every machine description must have a named pattern for each of the
;; conditional branch names "bcond"
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define_insn "beq_32"
  [(set (pc)
        (if_then_else (eq (reg:CC CC_REG)
                          (const_int 0))
                      (label_ref (match_operand 0 "" ""))
                      (pc)))]
  "(pic30_isa32_target())"
  "* return pic30_conditional_branch(EQ, operands[0],0,0);"
  [
    (set_attr "cc" "unchanged")
    (set_attr "op_type" "etc")
  ]
)

(define_insn "bne_32"
  [(set (pc)
        (if_then_else (ne (reg:CC CC_REG)
                          (const_int 0))
                      (label_ref (match_operand 0 "" ""))
                      (pc)))]
  "(pic30_isa32_target())"
  "* return pic30_conditional_branch(NE, operands[0],0,0);"
  [
    (set_attr "cc" "unchanged")
    (set_attr "op_type" "etc")
  ]
)

(define_insn "bgt_32"
  [(set (pc)
        (if_then_else (gt (reg:CC CC_REG)
                          (const_int 0))
                      (label_ref (match_operand 0 "" ""))
                      (pc)))]
  "(pic30_isa32_target())"
  "* return pic30_conditional_branch(GT, operands[0],0,0);"
  [
    (set_attr "cc" "unchanged")
    (set_attr "op_type" "etc")
  ]
)

(define_insn "blt_32"
  [(set (pc)
        (if_then_else (lt (reg:CC CC_REG)
                          (const_int 0))
                      (label_ref (match_operand 0 "" ""))
                      (pc)))]
  "(pic30_isa32_target())"
  "* return pic30_conditional_branch(LT, operands[0],0,0);"
  [
    (set_attr "cc" "unchanged")
    (set_attr "op_type" "etc")
  ]
)

(define_insn "bge_32"
  [(set (pc)
        (if_then_else (ge (reg:CC CC_REG)
                          (const_int 0))
                      (label_ref (match_operand 0 "" ""))
                      (pc)))]
  "(pic30_isa32_target())"
  "* return pic30_conditional_branch(GE, operands[0],0,0);"
  [
    (set_attr "cc" "unchanged")
    (set_attr "op_type" "etc")
  ]
)

(define_insn "ble_32"
  [(set (pc)
        (if_then_else (le (reg:CC CC_REG)
                          (const_int 0))
                      (label_ref (match_operand 0 "" ""))
                      (pc)))]
  "(pic30_isa32_target())"
  "* return pic30_conditional_branch(LE, operands[0],0,0);"
  [
    (set_attr "cc" "unchanged")
    (set_attr "op_type" "etc")
  ]
)


; unsigned branches:

(define_insn "bgtu_32"
  [(set (pc)
        (if_then_else (gtu (reg:CC CC_REG)
                          (const_int 0))
                      (label_ref (match_operand 0 "" ""))
                      (pc)))]
  "(pic30_isa32_target())"
  "* return pic30_conditional_branch(GTU, operands[0],0,0);"
  [
    (set_attr "cc" "unchanged")
    (set_attr "op_type" "etc")
  ]
)

(define_insn "bltu_32"
  [(set (pc)
        (if_then_else (ltu (reg:CC CC_REG)
                          (const_int 0))
                      (label_ref (match_operand 0 "" ""))
                      (pc)))]
  "(pic30_isa32_target())"
  "* return pic30_conditional_branch(LTU, operands[0],0,0);"
  [
    (set_attr "cc" "unchanged")
    (set_attr "op_type" "etc")
  ]
)

(define_insn "bgeu_32"
  [(set (pc)
        (if_then_else (geu (reg:CC CC_REG)
                          (const_int 0))
                      (label_ref (match_operand 0 "" ""))
                      (pc)))]
  "(pic30_isa32_target())"
  "* return pic30_conditional_branch(GEU, operands[0],0,0);"
  [
    (set_attr "cc" "unchanged")
    (set_attr "op_type" "etc")
  ]
)

(define_insn "bleu_32"
  [(set (pc)
        (if_then_else (leu (reg:CC CC_REG)
                          (const_int 0))
                      (label_ref (match_operand 0 "" ""))
                      (pc)))]
  "(pic30_isa32_target())"
  "* return pic30_conditional_branch(LEU, operands[0],0,0);"
  [
    (set_attr "cc" "unchanged")
    (set_attr "op_type" "etc")
  ]
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Every machine description must have an anonymous pattern for each of
;; the possible reverse-conditional branches.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define_insn "*bccreverse_32"
  [(set (pc)
        (if_then_else (match_operator 1 "comparison_operator" 
			[(reg:CC CC_REG) (const_int 0)])
                      (pc)
                      (label_ref (match_operand 0 "" ""))))]
  "(pic30_isa32_target())"
  "*
   {
     return pic30_conditional_branch(
		reverse_condition(GET_CODE(operands[1])), operands[0],0,0);
   }"
  [
    (set_attr "cc" "unchanged")
    (set_attr "op_type" "etc")
  ]
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; An instruction to jump to an address which is operand zero.
;; This pattern name is mandatory on all machines.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define_insn "indirect_jump_32"
 [(set (pc) 
       (match_operand:HI 0 "pic30_register_operand" "r"))
 ]
 "(pic30_isa32_target())"
 "goto %0"
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Instruction to jump to a variable address. This is a low-level capability
;; which can be used to implement a dispatch table when there is no 'casesi'
;; pattern. This pattern requires two operands: the address or offset, and a
;; label which should immediately precede the jump table. If the macro
;; CASE_VECTOR_PC_RELATIVE evaluates to a nonzero value then the first operand
;; is an offset which counts from the address of the table; otherwise, it is
;; an absolute address to jump to. In either case, the first operand has mode
;; Pmode. The 'tablejump' insn is always the last insn before the jump table
;; it uses. Its assembler code normally has no need to use the second operand,
;; but you should incorporate it in the RTL pattern so that the jump optimizer
;; will not delete the table as unreachable code.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; for rev .11 and later
;
(define_insn "tablejump_32"
  [(set (pc) 
     (match_operand:HI 0 "pic30_register_operand" "r"))
   (use (label_ref (match_operand 1 "" "")))
   (clobber (match_dup 0))
   (clobber (reg:CC_NZC CC_REG))
  ]
  "(pic30_isa32_target())"
  "sl %0,%0\;.align 4\;neop\;bra %0")


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Subroutines of "casesi".
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;; Operand 0 is index
;; operand 1 is the minimum bound
;; operand 2 is the maximum bound - minimum bound + 1
;; operand 3 is CODE_LABEL for the table;
;; operand 4 is the CODE_LABEL to go to if index out of range.

(define_expand "casesi_32"
  [(match_operand:SI 0 "pic30_general_operand" "")
   (match_operand:SI 1 "immediate_operand" "")
   (match_operand:SI 2 "immediate_operand" "")
   (match_operand 3 "" "")
   (match_operand 4 "" "")]
  "(pic30_isa32_target())"
  "
{
  if (INTVAL(operands[2]) >= 32767) {
    error(\"Too many case statements in switch table\");
    FAIL;
  }
  if (operands[1] != const0_rtx) {
    rtx reg = gen_reg_rtx(SImode);

    operands[1] = GEN_INT(-INTVAL(operands[1]));
    operands[1] = force_reg(SImode, operands[1]);
    emit_insn(gen_addsi3(reg, operands[0], operands[1]));
    operands[0] = reg;
  }
  operands[0] = force_reg(SImode, operands[0]);
  operands[2] = force_reg(SImode, operands[2]);
  emit_insn(gen_cmpsi(operands[0], operands[2]));
  emit_jump_insn(gen_bgtu(operands[4]));
  emit_jump_insn(gen_casesi0(operands[0], operands[3]));
  DONE;
}")

(define_insn "casesi0_32"
  [(set (pc) (plus:SI
     (mem:SI (plus:SI (pc)
               (match_operand:SI 0 "pic30_mode2_operand" "r,R")))
     (label_ref (match_operand 1 "" ""))))
     (clobber (match_scratch:HI 2  "=X,r"))
     (clobber (match_dup 0))
     (clobber (reg:CC_NZC CC_REG))
  ]
  "(pic30_isa32_target())"
  "*
   {
     /*
     ** See if there is anything between us and the jump table
     ** If we could be sure there never was, then the 'clobber'
     ** of operand[0] could be removed.
     */

     /* for rev .10 and earlier */

     rtx_insn *p;
     int fDisjoint = FALSE;
     for (p = NEXT_INSN (insn); 
          p != (as_a <rtx_insn *> (operands[1])); 
          p = NEXT_INSN (p)) {
       if (INSN_P(p)) {
         fDisjoint = TRUE;
         break;
       }
     }
     if (fDisjoint) {
       if (which_alternative == 0)
         return \"sl.l %0,%0\;\"
                \"add.l #(%1-$)/4,%0\;\"
                \".align 4\;\"
                \"neop\;\"
                \"bra %0\";
       else
         return \"mov.l [%0], %2\;\"
                \"sl.l %2,%2\;\"
                \"add.l #(%1-$)/4,%2\;\"
                \".align 4\;\"
                \"neop\;\"
                \"bra %2\";
     } else {
       if (which_alternative == 0)
         return \"sl.l %0,%0\;\"
                \".align 4\;\"
                \"neop\;\"
                \"bra %0\";
       else
         return \"mov.l %0, %2\;\"
                \"sl.l %2,%2\;\"
                \".align 4\;\"
                \"neop\;\"
                \"bra %2\";
     }
   }"
  [
    (set_attr "type" "etc,use")
    (set_attr "op_type" "etc")
  ]
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Unconditional jump
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define_insn "jump_32"
  [(set (pc)
        (label_ref (match_operand 0 "" "")))]
  "(pic30_isa32_target())"
  "bra %0"
  [
    (set_attr "cc" "unchanged")
    (set_attr "op_type" "etc")
  ]
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Call subroutine, returning value in operand 0
;; (which must be a hard register).
;; Operand 1 is the function to call
;; Operand 2 is the number of bytes of arguments pushed
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define_expand "call_value_32"
  [(set (match_operand 0 "pic30_register_operand"        "=rl,rl,rl")
        (call (match_operand    1 "memory_operand"  "R,QS,m")
              (match_operand:HI 2 "pic30_general_operand" "")))]
  ;; Operand 2 not really used for dsPIC30.
  "(pic30_isa32_target())"
  "
{
     struct saved_list *save_list = 0;
     tree save_variable_list;

     if ((GET_CODE(operands[1]) == MEM) &&
         (GET_CODE(XEXP(operands[1],0)) == SYMBOL_REF)) {
       if (!pic30_program_space_operand_p(XEXP(operands[1],0))) {
         error(\"Cannot call non-program symbol\");
       }
     }

     save_variable_list = pic30_get_save_variable_list();
     if (save_variable_list) {
       tree v;

       for (v = save_variable_list; v; v = TREE_CHAIN(v)) {
         struct saved_list *save_item;
         tree decl = pic30_get_save_variable_decl(v,NULL,NULL);

         save_item = (struct saved_list *)xmalloc(sizeof(struct saved_list));
         gcc_assert(save_item);
         save_item->saved_value = gen_reg_rtx(DECL_MODE(decl));
	 save_item->decl = decl;
         save_item->next = save_list;
         save_list = save_item;
         emit_move_insn(save_item->saved_value, DECL_RTL(save_item->decl));
       }
     }
 
     emit(
       gen_call_value_helper_32(operands[0], operands[1], operands[2])
     );
     
     if (save_variable_list) {
       struct saved_list *l;

       for (l = save_list; l; ) {
         struct saved_list *ll = l->next;
         emit_move_insn(DECL_RTL(l->decl),l->saved_value);
         free(l);
         l = ll;
       }
     }

     DONE;
}
  "
)

(define_insn "call_value_helper_32"
  [(set (match_operand 0 "pic30_any_register_operand"  "=rl,rl ,rl")
        (call (match_operand    1 "memory_operand"     " R, QS, m")
              (match_operand:HI 2 "pic30_general_operand" "")))]
  ;; Operand 2 not really used for dsPIC30.
  "(pic30_isa32_target() && (pic30_check_for_conversion(insn)))"
  "*
   static char szInsn[88];
   char *security;
   int slot = 0;
   int set_psv;
   char *this_insn = szInsn;

   pic30_clear_fn_list = 1;
   pic30_valid_call_address_operand(operands[0], Pmode);
#ifdef __C30_BETA__
   if (TARGET_ABI_CHECK) {
    this_insn += sprintf(this_insn,\"call __c30_abi_push\n\t\");
   }
#endif
   switch (which_alternative)
   {
     case 0:
       this_insn += sprintf(this_insn,\"call %s\", reg_names[REGNO(XEXP(operands[1],0))]);
       break;
     case 1:
       /*
        ** Casts of &(int x) to function ptrs, etc.
        */
       error(\"invalid function call\");
     case 2:
       security = pic30_boot_secure_access(operands[1],&slot,&set_psv);
       if (security) {
         this_insn += sprintf(this_insn, \"%s %s(%d)\",
                         pic30_near_function_p(operands[1]) ? 
                           \"rcall\" : \"call\", security,slot);
       } else 
         this_insn += sprintf(this_insn, \"%s %%1\",
                         pic30_near_function_p(operands[1]) ? 
                           \"rcall\" : \"call\");
         break;
      default:
         gcc_assert(0);
   }
#ifdef __C30_BETA__
   if (TARGET_ABI_CHECK) {
     this_insn += sprintf(this_insn,\"\n\tcall __c30_abi_pop\");
   }
#endif
   return szInsn ;
")

;; Call subroutine with no return value.
(define_expand "call_void_32"
  [ (call (match_operand:QI 0 "memory_operand"  "R,QS,m")
          (match_operand:HI 1 "pic30_general_operand" ""))]
  ;; Operand 1 not really used for dsPIC30.
  "(pic30_isa32_target())"
  "
{
     struct saved_list *save_list = 0;
     tree save_variable_list;

     if ((GET_CODE(operands[0]) == MEM) &&
         (GET_CODE(XEXP(operands[0],0)) == SYMBOL_REF)) {
       if (!pic30_program_space_operand_p(XEXP(operands[0],0))) {
         error(\"Cannot call non-program symbol\");
       }
     }

     save_variable_list = pic30_get_save_variable_list();
     if (save_variable_list) {
       tree v;

       for (v = save_variable_list; v; v = TREE_CHAIN(v)) {
         struct saved_list *save_item;
         tree decl = pic30_get_save_variable_decl(v,NULL,NULL);

         save_item = (struct saved_list *)xmalloc(sizeof(struct saved_list));
         gcc_assert(save_item);
         save_item->saved_value = gen_reg_rtx(DECL_MODE(decl));
         save_item->decl = decl;
         save_item->next = save_list;
         save_list = save_item;
         emit_move_insn(save_item->saved_value, DECL_RTL(save_item->decl));
       }
     }

     emit(
       gen_call_void_helper_32(operands[0], operands[1])
     );

     if (save_variable_list) {
       struct saved_list *l;

       for (l = save_list; l; ) {
         struct saved_list *ll = l->next;
         emit_move_insn(DECL_RTL(l->decl),l->saved_value);
         free(l);
         l = ll;
       }
     }

     DONE;
}
  "
)

(define_insn "call_void_helper_32"
  [(call (match_operand:QI 0 "memory_operand" "R,QS,m")
         (match_operand:HI 1 "pic30_general_operand" ""))]
  "(pic30_isa32_target()) && (pic30_check_for_conversion(insn))"
  "*
{
   static char szInsn[88];
   char *security;
   int slot = 0;
   char *this_insn = szInsn;

   pic30_clear_fn_list = 1;
   pic30_valid_call_address_operand(operands[0], Pmode);
#ifdef __C30_BETA__
   if (TARGET_ABI_CHECK) {
    this_insn += sprintf(this_insn,\"call __c30_abi_push\n\t\");
   }
#endif
   switch (which_alternative)
   {
     case 0:
       this_insn += sprintf(this_insn,\"call %s\", reg_names[REGNO(XEXP(operands[0],0))]);
       break;
     case 1:
       /*
        ** Casts of &(int x) to function ptrs, etc.
        */
       error(\"invalid function call\");
     case 2:
       security = pic30_boot_secure_access(operands[0],&slot,0);
       if (security) {
         this_insn += sprintf(this_insn, \"%s %s(%d)\",
                         pic30_near_function_p(operands[0]) ? 
                           \"rcall\" : \"call\",
                         security, slot);
       } else
         this_insn += sprintf(this_insn, \"%s %%0\",
                         pic30_near_function_p(operands[0]) ? 
                           \"rcall\" : \"call\");
         break;
     default:
       gcc_assert(0);
   }
#ifdef __C30_BETA__
   if (TARGET_ABI_CHECK) {
     this_insn += sprintf(this_insn,\"\n\tcall __c30_abi_pop\");
   }
#endif
   return szInsn ;
}")

;; Call subroutine with no return value.
;; This handles intrinsics, such as bcopy.

(define_expand "call_32"
 [(call (match_operand:QI 0 "" "")
        (match_operand:HI 1 "" ""))]
 "(pic30_isa32_target())"
 "
{
  struct saved_list *save_list = 0;
  tree save_variable_list;
  if (GET_CODE (operands[0]) == MEM && 
      !pic30_call_address_operand (XEXP (operands[0], 0), FN_Pmode))
    operands[0] = gen_rtx_MEM (GET_MODE (operands[0]),
			       force_reg (FN_Pmode, XEXP (operands[0], 0)));

  if ((GET_CODE(operands[0]) == MEM) && 
      (GET_CODE(XEXP(operands[0],0)) == SYMBOL_REF)) {
    if (!pic30_program_space_operand_p(XEXP(operands[0],0))) {
      error(\"Cannot call non-program symbol\");
    }
  }

  save_variable_list = pic30_get_save_variable_list();
  if (save_variable_list) {
    tree v;

    for (v = save_variable_list; v; v = TREE_CHAIN(v)) {
      struct saved_list *save_item;
      tree decl = pic30_get_save_variable_decl(v,NULL,NULL);

      save_item = (struct saved_list *)xmalloc(sizeof(struct saved_list));
      gcc_assert(save_item);
      save_item->saved_value = gen_reg_rtx(DECL_MODE(decl));
      save_item->decl = decl;
      save_item->next = save_list;
      save_list = save_item;
      emit_move_insn(save_item->saved_value, DECL_RTL(save_item->decl));
    }
  }

  emit(
    gen_call_void_helper_32(operands[0], operands[1])
  );
  if (save_variable_list) {
     struct saved_list *l;

     for (l = save_list; l; ) {
       struct saved_list *ll = l->next;
       emit_move_insn(DECL_RTL(l->decl),l->saved_value);
       free(l);
       l = ll;
     }
   }

  DONE;
}")

(define_insn "*call_32"
  [(call (mem:QI (match_operand:QI 0 "pic30_call_address_operand" ""))
         (match_operand:HI 1 "pic30_general_operand" ""))]
  "(pic30_isa32_target()) && (pic30_check_for_conversion(insn))"
  "*
  {
    static char szInsn[88];
    char *security;
    int slot = 0;
    char *insn = szInsn;

    pic30_clear_fn_list = 1;
    pic30_valid_call_address_operand(operands[0], FN_Pmode);
    security = pic30_boot_secure_access(operands[0],&slot,0);
#ifdef __C30_BETA__
    if (TARGET_ABI_CHECK) {
     insn += sprintf(insn,\"call __c30_abi_push\n\t\");
    }
#endif
    if (security) {
      insn += sprintf(insn, \"call %s(%d)\",security, slot);
    } else sprintf(szInsn,\"call %%0\");
#ifdef __C30_BETA__
    if (TARGET_ABI_CHECK) {
      insn += sprintf(insn,\"\n\tcall __c30_abi_pop\");
    }
#endif
    return szInsn;
  }"
)

;;
;; return
;;
(define_insn "return_32"
  [(return)]
  "pic30_null_epilogue_p(0)"
  "*
{
	pic30_set_function_return(TRUE);

	return \"return\";
}"
  [
    (set_attr "cc" "clobber")
    (set_attr "op_type" "etc")
  ]
)

(define_insn "return_from_epilogue_32"
  [(return)]
  "!pic30_null_epilogue_p(0) && reload_completed"
  "*
  { extern tree current_function_decl;
    if (pic30_interrupt_function_p(current_function_decl))
    {
      return \"retfie\";
    }
    else if (pic30_noreturn_function(current_function_decl))
    {
      return \"reset\";
    }
    else
    {
      return \"return\";
    }
  }"
  [
    (set_attr "cc" "clobber")
    (set_attr "op_type" "etc")
  ]
)

;;
;; link
;;
(define_insn "lnk_32"
 [
    (set (mem:SI (reg:SI SPREG))
         (reg:SI FPREG))
    (set (reg:SI FPREG)
         (plus:SI (reg:SI SPREG) (const_int 4)))
    (set (reg:SI SPREG)
         (plus:SI (reg:SI SPREG)
                  (plus:SI (match_operand 0 "immediate_operand" "i")
                           (const_int 4))))
 ]
 "reload_completed && (pic30_isa32_target())"
  "*
   return \"lnk #%0\";
  "
 [
  (set_attr "cc" "change0")
  (set_attr "type" "def")
  (set_attr "op_type" "etc")
 ]
)

;;
;; unlink
;;
(define_insn "ulnk_32"
  [
     (set (reg:HI SPREG)
          (minus:HI (reg:HI FPREG) (const_int 4)))
     (set (reg:HI FPREG)
          (mem:HI (reg:HI SPREG)))
  ] 
  "reload_completed && (pic30_isa32_target())"
  "*
  {
    return \"ulnk\";
  }
  "
  [
   (set_attr "cc" "change0")
   (set_attr "type" "def")
   (set_attr "op_type" "etc")
  ]
)

  

;;
;; disi
;;
(define_insn "disi_32"
  [(unspec_volatile [
     (match_operand 0 "pic30_reg_or_imm_operand" "r,i")
   ] UNSPECV_DISI)]
  "(TARGET_ISA32)"
  "@
   disictl %0
   disictl #%0"
)

(define_insn "disiwb_32"
  [(unspec_volatile [
     (match_operand 0 "pic30_reg_or_imm_operand" "r,i")
     (match_operand 1 "pic30_mode2_operand"      "rR<>,rR<>")
   ] UNSPECV_DISI)]
  "(TARGET_ISA32)"
  "@
   disictl %0,%1
   disictl #%0,%1"
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; prologue/epilogue
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define_expand "prologue_32"
  [(const_int 1)]
  "(pic30_isa32_target())"                           
  "
  {
     pic30_expand_prologue();
     DONE;
  }"
)

(define_expand "epilogue_32"
  [(const_int 1)]
  "(pic30_isa32_target())"
  "
  {
     pic30_expand_epilogue();
     DONE;
  }"
)

(define_insn "profile_enter_32"
  [(unspec_volatile [(const_int 0)] UNSPECV_ENTER)]
  "pic30_assert(0)"
  "call __function_level_profiling_long"
)

(define_insn "profile_exit_32"
  [(unspec_volatile [(const_int 0)] UNSPECV_EXIT)]
  "pic30_assert(0)"
  "goto __function_level_profiling_long_zero"
)

(define_insn "profile_exit_call_32"
  [(unspec_volatile [(const_int 1)] UNSPECV_EXIT)]
  "pic30_assert(0)"
  "call __function_level_profiling_long_zero"
)

;; UNSPEC_VOLATILE is considered to use and clobber all hard registers and
;; all of memory.  This blocks insns from being moved across this point.

(define_insn "blockage_32"
  [(unspec_volatile [(const_int 0)] UNSPECV_BLOCKAGE)]
  ""
  "")

(define_insn "pa_32"
  [(unspec_volatile [(match_operand 0 "immediate_operand" "i")] UNSPECV_PA)]
  ""
  ".set ___PA___,%0")

(define_insn "iv_32"
  [(unspec_volatile [(match_operand 0 "immediate_operand" "i")] UNSPECV_IV)]
  "(pic30_isa32_target())"
  {
   static char szInsn[96];
	char szVector[32];
	int nVectorID;
			
	nVectorID = INTVAL(operands[0]);
	if (nVectorID < 0)
	{
		nVectorID = -nVectorID-1;
		sprintf(szVector,"__AltInterrupt%d",nVectorID);
	}
	else
	{
		sprintf(szVector, "__Interrupt%d", nVectorID);
	}
	sprintf(szInsn, ".global\t%s\n%s:", szVector, szVector);

	return szInsn;
  }
)
(define_insn "pp_32"
  [(unspec_volatile [(const_int 0)] UNSPECV_PP)]
  "(pic30_isa32_target())"
  {
  	return pic30_interrupt_preprologue();
  }
)

;
; CAW - for RAW scheduling we could split these instructions after the
;   mov #_addr,%0
;
(define_insn "write_oscconl_32"
  [(set (match_operand:HI 0 "pic30_register_operand" "=&r")
        (unspec_volatile [
                     (match_operand 1 "pic30_register_operand" "r")
                     (match_operand 2 "pic30_register_operand" "r")
                     (match_operand 3 "pic30_register_operand" "r")]
           UNSPECV_WRITEOSCCONL)
   )]
  "pic30_assert(0)"
  "mov #_OSCCON,%0\;mov.b %1,[%0]\;mov.b %2,[%0]\;mov.b %3,[%0]"
)

(define_insn "write_oscconh_32"
  [(set (match_operand:HI 0 "pic30_register_operand" "=&r")
        (unspec_volatile [
                     (match_operand 1 "pic30_register_operand" "r")
                     (match_operand 2 "pic30_register_operand" "r")
                     (match_operand 3 "pic30_register_operand" "r")]
    UNSPECV_WRITEOSCCONH)
   )]
  "pic30_assert(0)"
  "mov #_OSCCON+1,%0\;mov.b %1,[%0]\;mov.b %2,[%0]\;mov.b %3,[%0]"
)

(define_insn "write_disicnt_32"
  [ (unspec_volatile [
      (match_operand 0 "pic30_register_operand" "r")] UNSPECV_WRITEDISICNT)
  ]
  "pic30_assert(0)"
  "mov %0,_DISICNT\;mov w0,_WREG0"
)

(define_insn "write_pwmsfr_32"
  [ (unspec_volatile [
      (match_operand:HI 0 "pic30_near_operand"     "U,U")
      (match_operand:HI 1 "pic30_register_operand" "a,r")
      (match_operand:HI 2 "pic30_register_operand" "r,r")
      (match_operand:HI 3 "pic30_register_operand" "r,r")
      (match_operand:HI 4 "pic30_R_operand"        "R,R")
    ] UNSPECV_WRITEPWMCON)
    (clobber (match_scratch: HI  5                "=X,&r"))
  ]
  "pic30_assert(0)"
  "@
   mov %3,%4\;mov %2,%4\;mov %1,%0
   mov #%0,%5\;mov %3,%4\;mov %2,%4\;mov %1,[%5]"
  [
    (set_attr "cc" "change0")
    (set_attr "type" "use,etc")
    (set_attr "op_type" "mov")
  ]
)

(define_insn "write_nvm_secure_32"
  [
    (unspec_volatile [ 
      (match_operand:HI 0 "pic30_register_operand" "+r") 
      (match_operand:HI 1 "pic30_register_operand" "+r") 
    ] UNSPECV_WRITENVM)
  ]
  "pic30_assert(0)"
  ".set ___PA___,0\;mov %0,_NVMKEY\;nop\;mov %1,_NVMKEY\;bset _NVMCON,#15\;clr %0\;clr %1\;.set __PA___,1"
  [
    (set_attr "type" "etc")
    (set_attr "op_type" "movlit")
  ]
)

(define_insn "write_nvm_32"
  [(set (match_operand:HI 0 "pic30_register_operand" "=r")
        (unspec_volatile [ (const_int 0) ] UNSPECV_WRITENVM))]
  "pic30_assert(0)"
  ".set ___PA___,0\;mov #0x55,%0\;mov %0,_NVMKEY\;mov #0xAA,%0\;mov %0,_NVMKEY\;bset _NVMCON,#15\;nop\;nop\;.set ___PA___,1"
  [
    (set_attr "type" "etc")
    (set_attr "op_type" "etc")
  ]
)

(define_insn "write_rtcwen_32"
  [(set (match_operand:HI 0 "pic30_register_operand" "=&r")
        (unspec_volatile [
           (match_operand:HI 1 "pic30_register_operand" "0")
         ] UNSPECV_WRITERTCWEN))]
  "pic30_assert(0)"
  ".set ___PA___,0\;mov #0x55,%0\;mov %0,_NVMKEY\;mov #0xAA,%0\;mov %0,_NVMKEY\;bset _RCFGCAL,#13\;.set ___PA___,1"
  [
    (set_attr "type" "etc")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "write_wrlock_32"
  [(set (match_operand:HI 0 "pic30_register_operand" "=&r")
        (unspec_volatile [
           (match_operand:HI 1 "pic30_register_operand" "0")
         ] UNSPECV_WRITEWRLOCK))]
  "pic30_assert(0)"
  ".set ___PA___,0\;mov #0x55,%0\;mov %0,_NVMKEY\;mov #0xAA,%0\;mov %0,_NVMKEY\;bclr _RTCCON1L,#11\;.set ___PA___,1"
  [
    (set_attr "type" "etc")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "write_cryotp_32"
  [(set (match_operand:HI 0 "pic30_register_operand" "=&r")
        (unspec_volatile [ (const_int 0) ] UNSPECV_WRITECRTOTP))]
  "pic30_assert(0)"
  ".set ___PA___,0\;mov #0x55,%0\;mov %0,_NVMKEY\;mov #0xAA,%0\;mov %0,_NVMKEY\;nop\;bset _CRYOTP,#0\;.set ___PA___,1"
  [
    (set_attr "type" "etc")
    (set_attr "op_type" "alu")
  ]
)

(define_insn "write_dataflash_32"
  [(set (match_operand:HI 0 "pic30_register_operand" "=r")
        (unspec_volatile [ (const_int 0) ] UNSPECV_WRITEDATAFLASH))
   (clobber 
        (match_scratch:HI 1                          "=&r"))
  ]
  "pic30_assert(0)"
  ".set ___PA___,0\;mov _DFCON,%1\;bset %1,#7\;mov #0xEDB7,%0\;mov %0,_DFKEY\;mov #0x1248,%0\;mov %0,_DFKEY\;mov %1,_DFCON\;.set ___PA___,1"
)

(define_insn "write_dataflash_secure_32"
  [
    (unspec_volatile [ 
      (match_operand:HI 0 "pic30_register_operand" "+r") 
      (match_operand:HI 1 "pic30_register_operand" "+r") 
    ] UNSPECV_WRITEDATAFLASH)
    (clobber 
         (match_scratch:HI 2                          "=&r"))
  ]
  "pic30_assert(0)"
  ".set ___PA___,0\;mov _DFCON,%2\;bset %2,#7\;mov %0,_DFKEY\;mov %1,_DFKEY\;mov %2,_DFCON\;clr %0\;clr %1\;.set ___PA___,1"
)



;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; nop
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define_insn "bifnop_32"
  [(unspec_volatile [(const_int 0)] UNSPECV_NOP)]
  "(pic30_isa32_target())"
  "nop"
  [
    (set_attr "cc" "unchanged")
    (set_attr "op_type" "etc")
  ]
)

(define_insn "pwrsav_32"
  [
   (unspec_volatile 
     [(match_operand:HI 0 "immediate_operand" "i")]
    UNSPECV_PWRSAV)
  ]
  "(pic30_isa32_target())"
  "pwrsav #%0"
  [
    (set_attr "cc" "unchanged")
    (set_attr "op_type" "etc")
  ]
)

(define_insn "clrwdt_32"
  [
   (unspec_volatile
     [(const_int 0)]
   UNSPECV_CLRWDT)
  ]
  "(pic30_isa32_target())"
  "clrwdt"
  [
    (set_attr "cc" "unchanged")
    (set_attr "op_type" "etc")
  ]
)

;
;; misc
;

(define_insn "section_begin_32"
  [(set (match_operand:SI 0 "pic30_register_operand" "=r")
        (unspec:SI [ 
          (match_operand 1 "immediate_operand" "i")
        ] UNSPEC_SECTION_BEGIN))]
  "(pic30_isa32_target())"
  "*
  { static char *buffer = 0;
    static int buffer_size;
    int len;

    tree t = (tree)(INTVAL(operands[1]));
    const char *section_name = TREE_STRING_POINTER(t);
    len = sizeof (\"mov.l #.startof.(),%0\");
    len += strlen(section_name)*2;
    len++;
    if (buffer_size < len) {
      if (buffer) free(buffer);
      buffer = (char *)xmalloc(len+50);
      buffer_size = len+50;
    }
    sprintf(buffer, \"mov.l #.startof.(%s),%%0\", section_name, section_name);
    return buffer;
  }"
  [
    (set_attr "type" "def")
    (set_attr "op_type" "movlit")
  ]
)

(define_insn "section_end_32"
  [(set (match_operand:SI 0 "pic30_register_operand" "=r")
        (unspec:SI [ 
          (match_operand 1 "immediate_operand" "i")
        ] UNSPEC_SECTION_END))]
  "(pic30_isa32_target())"
  "*
  { static char *buffer = 0;
    static int buffer_size;
    int len;

    tree t = (tree)(INTVAL(operands[1]));
    const char *section_name = TREE_STRING_POINTER(t);
    len = sizeof (\"mov.l #.endof.(),%0\");
    len += strlen(section_name)*2;
    len++;
    if (buffer_size < len) {
      if (buffer) free(buffer);
      buffer = (char *)xmalloc(len+50);
      buffer_size = len+50;
    }
    sprintf(buffer, \"mov.l #.endof.(%s),%%0\", section_name, section_name);
    return buffer;
  }"
  [
    (set_attr "type" "def")
    (set_attr "op_type" "movlit")
  ]
)

(define_insn "section_size_32"
  [(set (match_operand:SI 0 "pic30_register_operand" "=r")
        (unspec:SI [ 
          (match_operand 1 "immediate_operand" "i")
        ] UNSPEC_SECTION_SIZE))]
  "(pic30_isa32_target())"
  "*
  { static char *buffer = 0;
    static int buffer_size;
    int len;

    tree t = (tree)(INTVAL(operands[1]));
    const char *section_name = TREE_STRING_POINTER(t);
    len = sizeof (\"mov.l #.sizeof.(),%0\");
    len += strlen(section_name)*2;
    len++;
    if (buffer_size < len) {
      if (buffer) free(buffer);
      buffer = (char *)xmalloc(len+50);
      buffer_size = len+50;
    }
    sprintf(buffer, \"mov.l #.sizeof.(%s),%%0\", section_name, section_name);
    return buffer;
  }"
  [
    (set_attr "type" "def")
    (set_attr "op_type" "movlit")
  ]
)

; define [sg]et_isr_state as an expand, in so that we can generate less
;   code if we don't actually 'use' the value (apart from to re-set it)
;   if unless we say if (__builtin_get_isr_state() == n) there is no need
;   to shift the value

(define_insn "get_isr_state_helper_32"
  [(set (match_operand:SI 0 "pic30_register_operand" "=r")
        (unspec_volatile:SI [ (const_int 0) ] UNSPEC_GET_ISR_STATE))]
  "(pic30_isa32_target())"
  "mov.l SR,%0"
  [
    (set_attr "type" "def")
    (set_attr "op_type" "mov")
  ]
)

(define_insn "extract_gie_32"
  [(set (match_operand:SI 0 "pic30_register_operand" "=r")
        (unspec_volatile:SI [ (match_dup 0) ] UNSPEC_EXTRACT_GIE))]
  "(pic30_isa32_target())"
  "cp0.w _INTCON1\;bra nn,.L1_%=\;bset %0,#3\n.L1_%=:"
)

(define_expand "get_isr_state_32"
 [(set (match_operand:SI 0 "pic30_register_operand" "=r")
       (unspec_volatile:SI [(const_int 0)] UNSPEC_GET_ISR_STATE))]
  "(pic30_isa32_target())"
  "{ rtx reg = operands[0];
     if (!pic30_register_operand(operands[0], SImode)) {
       reg = force_reg (SImode, operands[0]);
     }
     emit_insn(
       gen_get_isr_state_helper_32(reg)
     );
     emit_insn(
       gen_ashrsi3(reg, reg, GEN_INT(5))
     );
     emit_insn(
       gen_andsi3(reg, reg, GEN_INT(7))
     );
     if (pic30_device_has_gie()) {
       emit_insn(
         gen_extract_gie_32(reg)
       );
     }
     if (reg != operands[0]) {
       emit_move_insn(operands[0], reg);
     }
     DONE;
   }")

(define_insn "set_isr_state_helper_32"
  [(unspec_volatile [
      (match_operand:SI 0 "pic30_register_operand" "r")
    ] UNSPECV_SET_ISR_STATE)
   (clobber (match_scratch:SI 1                    "=&r"))
   (clobber (match_scratch:SI 2                    "=&r"))]
  "(pic30_isa32_target())"
  "mov SR,%1\;and.b #0x1F,%1\;ior %1,%0,%1\;mov %1,SR"
  [
    (set_attr "cc" "clobber")
    (set_attr "op_type" "mov")
  ]
)

(define_insn "insert_gie_32"
  [(unspec_volatile [ 
      (match_operand:SI 0 "pic30_register_operand" "r")] UNSPEC_INSERT_GIE)
   (clobber (match_scratch:SI 1                    "=r"))]
  "(pic30_isa32_target())"
  "sl.w %0,#12,%1\;cp0.w %1\;bra nn,.L1_%=\;bset.w _INTCON1,#15\n.L1_%=:"
)


(define_expand "set_isr_state_32"
 [(unspec_volatile [ 
     (match_operand:SI 0 "pic30_register_operand" "=r")] UNSPECV_SET_ISR_STATE)]
  "(pic30_isa32_target())"
  "{ rtx reg = operands[0];
     rtx reg2 = gen_reg_rtx(SImode);

     if (!pic30_register_operand(operands[0], SImode)) {
       reg = force_reg (SImode, operands[0]);
     }
     emit_insn(
       gen_andsi3(reg2, reg, GEN_INT(7))
     );
     emit_insn(
       gen_ashlsi3(reg2, reg2, GEN_INT(5))
     );
     emit_insn(
       gen_set_isr_state_helper_32(reg2)
     );
     if (pic30_device_has_gie()) {
       emit_insn(
         gen_insert_gie_32(reg)
       );
     }
     DONE;
   }")

(define_insn "disable_isr_gie_32"
  [(unspec_volatile [(const_int 0)] UNSPECV_DISABLE_ISR_GIE)]
  "(pic30_isa32_target())"
  "*
   {
     if (pic30_device_has_gie()) {
       /* clear the GIE */
       return \"bclr.w _INTCON1,#15\";
     } else {
       error(\"This device has no GIE\");
       return \"; This device has no GIE\";
     }
   }"
)

(define_insn "enable_isr_gie_32"
  [(unspec_volatile [(const_int 0)] UNSPECV_ENABLE_ISR_GIE)]
  "(pic30_isa32_target())"
  "*
   {
     if (pic30_device_has_gie()) {
       /* set the GIE */
       return \"bset.w _INTCON1,#15\";
     } else {
       error(\"This device has no GIE\");
       return \"; This device has no GIE\";
     }
   }"
)  

(define_insn "disable_isr_ipl_32"
  [(unspec_volatile [(const_int 0)] UNSPECV_DISABLE_ISR)
   (clobber
      (match_scratch:HI 0  "=a,r"))
   (clobber
      (match_scratch:HI 1  "=r,r"))]
  "pic30_assert(0)"
  "*
   {
     if (which_alternative == 0) {
       return \"mov _DISICNT,%1\;disi #0x3FFF\;mov #0xE0,%0\;ior %0,SR\;mov %1,_DISICNT\;mov w0,_WREG0\";
     } else {
      return \"mov _DISICNT,%1\;disi #0x3FFF\;mov SR,%1\;ior #0xE0,%1\;mov %1,SR\;mov %1,_DISICNT\;mov w0,_WREG0\";
     }
   }"
)

(define_insn "enable_isr_ipl_32"
  [(unspec_volatile [(const_int 0)] UNSPECV_ENABLE_ISR)
   (clobber
      (match_scratch:HI 0  "=r"))
   (clobber
      (match_scratch:HI 1  "=r"))]
  "pic30_assert(0)"
  "mov _DISICNT,%1\;disi #0x3FFF\;mov SR,%0\;and.b #0x1F,%0\;mov %0,SR\;mov %1,_DISICNT\;mov w0,_WREG0"
)

(define_expand "disable_isr_32"
  [(const_int 0)]
  "pic30_assert(0)"
  "{
     if (pic30_device_has_gie()) {
       emit_insn(
         gen_disable_isr_gie()
       );
     } else {
       emit_insn(
         gen_disable_isr_ipl()
       );
     }
     DONE;
   }"
)

(define_expand "enable_isr_32"
  [(const_int 0)]
  "pic30_assert(0)"
  "{
     if (pic30_device_has_gie()) {
       emit_insn(
         gen_enable_isr_gie()
       );
     } else {
       emit_insn(
         gen_enable_isr_ipl()
       );
     }
     DONE;
   }"
)

(define_insn "software_break_32"
  [(unspec_volatile [(const_int 0)] UNSPECV_SOFTWARE_BREAK)]
  "(pic30_isa32_target())"
  "break"
)
 
;; Fixed Point

;; Use mode iterators alot here, because they are "new" and these are new
;;   patterns (and because most of our Fract/Accum types are the same size

;; *** helper patterns

; for use in the prologue to set the right initial value
(define_insn "setCORCON_32"
  [(set (reg:HI CORCON)
        (match_operand:HI 0 "pic30_register_operand" "r"))]
  "pic30_assert(0)"
  "mov %0,_CORCON"
)

(define_insn "abs<mode>2_32"
  [(set (match_operand:Q15   0 "pic30_register_operand" "=r")
        (abs:Q15 
          (match_operand:Q15 1 "pic30_register_operand"  "0")))
   (clobber (reg:CC CC_REG))
  ]
  "(pic30_isa32_target())"
  "btsc %1,#15\;neg %1,%0"
  [
    (set_attr "cc" "clobber")
    (set_attr "type" "def")
  ]
)

(define_insn "bsetCORCON_32"
  [(set (reg:HI CORCON)
        (ior:HI (reg:HI CORCON)
                (match_operand 0 "const_int_operand"      "i,i,i")))]
  " (pic30_one_bit_set_p(INTVAL(operands[0])&0xffff))"
  "bset _CORCON,#%b0"
)

(define_insn "bclrCORCON_32"
  [(set (reg:HI CORCON)
        (and:HI (reg:HI CORCON)
                (match_operand 0 "const_int_operand"      "i,i,i")))]
  " (pic30_one_bit_set_p(((~INTVAL(operands[0]))&0xffff)))"
  "bclr.b _CORCON,#%B0"
)

;; *** load/store

(define_insn "mov<mode>_rimm_32"
   [(set (match_operand:QUQ15 0 "pic30_register_operand" "=r")
         (match_operand:QUQ15 1 "immediate_operand" "i"))]
   ""
   "mov #%1,%0"
  [
    (set_attr "type" "def")
  ]
)

(define_insn "mov<mode>_gen_32"
  [
    (set 
       (match_operand:QUQ15 0 "pic30_move_operand" "=rR<>, Q,r, TU,r, !TUrR<>,w,Q,w,TU,w,r,w,   R<>")
       (match_operand:QUQ15 1 "pic30_move_operand" " rR<>, r,Q, r,TU, TUrR<>, w,w,Q,w,TU,w,rR<>,w"))
  ]
  ""
  "@
   mov %1,%0
   mov %1,%0
   mov %1,%0
   mov %1,%0
   mov %1,%0
   push %1\;pop %0
   clr %0\;add %0
   mov %m1H,%0
   lac %1,%0
   push %m1H\;pop %0
   clr %0\;push %1\;pop %m0H
   mov %m1H,%0
   lac %1,%0
   push %m1H\;pop %0"
  [
    (set_attr "type" "defuse,use,defuse,etc,def,defuse,etc,use,use,etc,etc,def,use,use")
  ]
)

(define_expand "mov<mode>_32"
  [(set (match_operand:QUQ15 0 "pic30_move_operand" "")
        (match_operand:QUQ15 1 "pic30_move_operand" ""))]
  ""
  "
{ int result;

  result = pic30_emit_move_sequence(operands, <MODE>mode);

  if (result > 0) DONE;
  if (immediate_operand(operands[1],VOIDmode)) {
    rtx reg = operands[0];

    if (!pic30_register_operand(operands[0],<MODE>mode)) {
      reg = gen_reg_rtx(<MODE>mode);
      emit(
        gen_mov<mode>_rimm(reg,operands[1])
      );
      emit_move_insn(operands[0], reg);
      DONE;
    }
  }
}")

;;
;; larger moves cannot use the 'k'

(define_insn "mov<mode>_rimm_32"
  [
    (set (match_operand:QUQ31 0 "pic30_register_operand" "=r")
         (match_operand:QUQ31 1 "immediate_operand" "i"))
  ]
  ""
  "mov #%1,%0\;mov #%y1,%d0"
  [
    (set_attr "type" "def")
  ]
)

(define_insn "mov<mode>_gen_32"
  [
    (set 
      (match_operand:QUQ31 0 "pic30_move_operand" "=rR<>, r,   Q, r, TU,r, TU,R, w,TU,w,r,w,R,w")
      (match_operand:QUQ31 1 "pic30_move_operand" " r,    R<>, r, Q, r,TU, R,TU, w,w,TU,w,r,w,R"))
  ]
  "(pic30_psrd_psrd_errata(operands[0],operands[1]) == 0) &&
   (pic30_psrd_psrd_errata_movd(operands[0],operands[1]) == 0)"
  "*
   {
     const char *patterns[] = {
       \"mov.d %1,%0\",
       \"mov.d %1,%0\",
       \"mov.w %1,%0\;mov.w %d1,%Q0\",
       \"mov.w %1,%0\;mov.w %Q1,%d0\",               /* =r,Q */
       \"push.d %1\;pop %0+2\;pop %0\",
       \"push %1\;push %1+2\;pop.d %0\",
       \"push %I1\;push %D1\;pop %0+2\;pop %0\",
       \"push %1\;push %1+2\;pop %P0\;pop %p0\",
       \"clr %0\;add %0\",
       \"push %m1L\;push %m1H\;pop %0+2\;pop %0\",
       \"push %1\;push %1+2\;pop %m0H\;pop %m0L\",
       \"mov %m1L,%0\;mov %m1H,%d0\",
       \"mov %1,%m0L\;mov %d1,%m0H\",
       \"push %m1H\;push %m1L\;pop %I0\;pop %D0\",
       \"push %I1\;push %D1\;pop %m0H\;pop %m0L\"
     };

     /* check for early clobber */
     int src_regno,dst_regno;
     int overlap;

     dst_regno = REGNO(operands[0]);

     switch (which_alternative) {
       default: /* no overlap possible */
                break;
       case 3: {
         /* mem ( plus ( reg, */
         rtx inner = XEXP(operands[1],0);
         src_regno = REGNO(XEXP(inner,0));
         overlap = src_regno-dst_regno;
         switch (overlap) {
           default: /* no overlap */
             break;
           case 0:  /* src == dst */
             return \"mov.w %Q1,%d0\;mov.w %1,%0\";
             break;
         }
         break;
       }
     }
     return patterns[which_alternative];
   }"
  [
    (set_attr "type" "def,defuse,use,use,etc,def,use,use,etc,etc,etc,def,etc,use,use")
  ]
)

(define_expand "mov<mode>_32"
  [(set (match_operand:QUQ31 0 "pic30_move_d_operand" "")
        (match_operand:QUQ31 1 "pic30_move_d_operand" ""))]
  ""
  "
{ int result = pic30_emit_move_sequence(operands, <MODE>mode);

  if (result > 0) DONE;
  
  if (immediate_operand(operands[1],VOIDmode)) {
    rtx reg = operands[0];

    if (!pic30_register_operand(operands[0],<MODE>mode)) {
      reg = gen_reg_rtx(<MODE>mode);
      emit(
        gen_mov<mode>_rimm_32(reg,operands[1])
      );
      emit_move_insn(operands[0], reg);
      DONE;
    }
  }

  /* we may need a clobber here */
  if (pic30_move_d_operand(operands[1],<MODE>mode)) {
    emit(
      gen_mov<mode>_gen_32(operands[0], operands[1])
    );
    DONE;
  }
 
}")

;; *** add/sub
;  16-bit
(define_insn "add<mode>3_32"
  [(set (match_operand:QUQ15 0 "pic30_register_operand" "=r")
        (plus:QUQ15
           (match_operand:QUQ15 1 "pic30_register_operand" "r")
           (match_operand:QUQ15 2 "pic30_register_operand" "r")))]
  " (pic30_isa32_target())"
  "add.w %1,%2,%0"
  [
    (set_attr "cc" "math")
    (set_attr "type" "etc")
  ]
)

(define_insn "sub<mode>3_32"
  [(set (match_operand:QUQ15 0 "pic30_register_operand" "=r")
        (minus:QUQ15
           (match_operand:QUQ15 1 "pic30_register_operand" "r")
           (match_operand:QUQ15 2 "pic30_register_operand" "r")))]
  " (pic30_isa32_target())"
  "sub.w %1,%2,%0"
  [
    (set_attr "cc" "math")
    (set_attr "type" "etc")
  ]
)

;  signed saturating
(define_insn "ssadd<mode>3_32"
  [(set (match_operand:QUQ15 0 "pic30_register_operand" "=&r")
        (ss_plus:QUQ15
           (match_operand:QUQ15 1 "pic30_register_operand" "r")
           (match_operand:QUQ15 2 "pic30_register_operand" "r")))]
  " pic30_fp_inline_p()"
  "add %1,%2,%0\;bra nov,.L1_%=\;mov #0x7FFF,%0\;btsc %2,#15\;com %0,%0\n.L1_%=:\;"
  [
    (set_attr "cc" "clobber")
    (set_attr "type" "etc")
  ]
)

(define_insn "sssub<mode>3_32"
  [(set (match_operand:QUQ15 0 "pic30_register_operand" "=&r")
        (ss_minus:QUQ15
           (match_operand:QUQ15 1 "pic30_register_operand" "r")
           (match_operand:QUQ15 2 "pic30_register_operand" "r")))]
  " pic30_fp_inline_p()"
  "sub %1,%2,%0\;bra nov,.L1_%=\;mov #0x7FFF,%0\;btss %2,#15\;com %0,%0\n.L1_%=:\;"
  [
    (set_attr "cc" "clobber")
    (set_attr "type" "etc")
  ]
)

;  unsigned saturating - our unsigned mode really is the same width as signed
(define_insn "usadd<mode>3_32"
  [(set (match_operand:QUQ15 0 "pic30_register_operand" "=&r")
        (us_plus:QUQ15
           (match_operand:QUQ15 1 "pic30_register_operand" "r")
           (match_operand:QUQ15 2 "pic30_register_operand" "r")))]
  ""
  "add %1,%2,%0\;btsc SR,#2\;mov #0x7FFF,%0"
  [
    (set_attr "cc" "clobber")
    (set_attr "type" "etc")
  ]
)

(define_insn "ussub<mode>3_32"
  [(set (match_operand:QUQ15 0 "pic30_register_operand" "=&r")
        (us_minus:QUQ15
           (match_operand:QUQ15 1 "pic30_register_operand" "r")
           (match_operand:QUQ15 2 "pic30_register_operand" "r")))]
  ""
  "sub %1,%2,%0\;btsc SR,#3\;clr %0"
  [
    (set_attr "cc" "clobber")
    (set_attr "type" "etc")
  ]
)

;  32-bit
(define_insn "add<mode>3_32"
  [(set (match_operand:QUQ31 0 "pic30_register_operand" "=r")
        (plus:QUQ31
           (match_operand:QUQ31 1 "pic30_register_operand" "r")
           (match_operand:QUQ31 2 "pic30_register_operand" "r")))]
  ""
  "add %1,%2,%0\;addc %d1,%d2,%d0"
  [
    (set_attr "cc" "math")
    (set_attr "type" "etc")
  ]
)

(define_insn "sub<mode>3_32"
  [(set (match_operand:QUQ31 0 "pic30_register_operand" "=r")
        (minus:QUQ31
           (match_operand:QUQ31 1 "pic30_register_operand" "r")
           (match_operand:QUQ31 2 "pic30_register_operand" "r")))]
  ""
  "sub %1,%2,%0\;subb %d1,%d2,%d0"
  [
    (set_attr "cc" "math")
    (set_attr "type" "etc")
  ]
)

;  signed saturating
(define_insn "ssadd<mode>3_32"
  [(set (match_operand:QUQ31 0 "pic30_register_operand" "=&r")
        (ss_plus:QUQ31
           (match_operand:QUQ31 1 "pic30_register_operand" "r")
           (match_operand:QUQ31 2 "pic30_register_operand" "r")))]
  " pic30_fp_inline_p()"
  "*
   return \"add %1,%2,%0\;\"
          \"addc %d1,%d2,%d0\;\"
          \"bra nov,.L1_%=\;\"
          \"setm %0\;\"
          \"mov #0x7FFF,%d0\;\"
          \"btst.c %d2,#15\;\"
          \"bra nc,.L1_%=\;\"
          \"com %d0,%d0\;\"
          \"com %0,%0\n\"
          \".L1_%=:\";
  "
  [
    (set_attr "cc" "clobber")
    (set_attr "type" "etc")
  ]
)

(define_insn "sssub<mode>3_32"
  [(set (match_operand:QUQ31 0 "pic30_register_operand" "=&r")
        (ss_minus:QUQ31
           (match_operand:QUQ31 1 "pic30_register_operand" "r")
           (match_operand:QUQ31 2 "pic30_register_operand" "r")))]
  " pic30_fp_inline_p()"
  "*
   return \"sub %1,%2,%0\;\"
          \"subb %d1,%d2,%d0\;\"
          \"bra nov,.L1_%=\;\"
          \"setm %0\;\"
          \"mov #0x7FFF,%d0\;\"
          \"btst.c %d2,#15\;\"
          \"bra c,.L1_%=\;\"
          \"com %d0,%d0\;\"
          \"com %0,%0\n\"
          \".L1_%=:\";
  "
  [
    (set_attr "cc" "clobber")
    (set_attr "type" "etc")
  ]
)

;  unsigned saturating - our unsigned mode really is the same width as signed
(define_insn "usadd<mode>3_32"
  [(set (match_operand:QUQ31 0 "pic30_register_operand" "=&r")
        (us_plus:QUQ31
           (match_operand:QUQ31 1 "pic30_register_operand" "r")
           (match_operand:QUQ31 2 "pic30_register_operand" "r")))]
  " pic30_fp_inline_p()"
  "*
   return \"add %1,%2,%0\;\"
          \"addc %d1,%d2,%d0\;\"
          \"bra nov,.L1_%=\;\"
          \"setm %0\;\"
          \"mov #0x7FFF,%d0\n\"
          \".L1_%=:\";
  "
  [
    (set_attr "cc" "clobber")
    (set_attr "type" "etc")
  ]
)

(define_insn "ussub<mode>3_32"
  [(set (match_operand:QUQ31 0 "pic30_register_operand" "=&r")
        (us_minus:QUQ31
           (match_operand:QUQ31 1 "pic30_register_operand" "r")
           (match_operand:QUQ31 2 "pic30_register_operand" "r")))]
  " pic30_fp_inline_p()"
  "*
   return \"sub %1,%2,%0\;\"
          \"subb %d1,%d2,%d0\;\"
          \"btsc SR,#3\;\"
          \"mul.uu %0,#0,%0\";
  "
  [
    (set_attr "cc" "clobber")
    (set_attr "type" "etc")
  ]
)

;; *** multiply
;  16-bit

/*
 * do not know if this will be successful
 */
(define_insn "mul<mode>3_helper_trunc_dsp_A_32"
  [(set (match_operand:QUQ15 0 "pic30_move_operand"       "=r")
        (mult:QUQ15 
          (match_operand:QUQ15 1 "pic30_register_operand" " za")
          (match_operand:QUQ15 2 "pic30_register_operand" " za")))
   (clobber (reg:SQ A_REGNO))
   (use (reg:HI CORCON))
  ]
  "((pic30_fp_round_p() == pic30_truncation) || 
    (pic30_fp_round_p() == pic30_fastest))"
  "mpy %1,%2,A\;sac A,#0,%0"
)

(define_insn "mul<mode>3_helper_trunc_dsp_B_32"
  [(set (match_operand:QUQ15 0 "pic30_move_operand"       "=r")
        (mult:QUQ15 
          (match_operand:QUQ15 1 "pic30_register_operand" " za")
          (match_operand:QUQ15 2 "pic30_register_operand" " za")))
   (clobber (reg:SQ B_REGNO))
   (use (reg:HI CORCON))
  ]
  "((pic30_fp_round_p() == pic30_truncation) || 
    (pic30_fp_round_p() == pic30_fastest))"
  "mpy %1,%2,B\;sac B,#0,%0"
)

(define_insn "mul<mode>3_helper_rnd_dsp_A_32"
  [(set (match_operand:QUQ15 0 "pic30_move_operand"       "=r")
        (mult:QUQ15
          (match_operand:QUQ15 1 "pic30_register_operand" " za")
          (match_operand:QUQ15 2 "pic30_register_operand" " za")))
   (clobber (reg:SQ A_REGNO))
   (use (reg:HI CORCON))
  ]
  "((pic30_fp_round_p() == pic30_convergent) ||
    (pic30_fp_round_p() == pic30_conventional))"
  "mpy %1,%2,A\;sac.r A,#0,%0"
)

(define_insn "mul<mode>3_helper_rnd_dsp_B_32"
  [(set (match_operand:QUQ15 0 "pic30_move_operand"       "=r")
        (mult:QUQ15
          (match_operand:QUQ15 1 "pic30_register_operand" " za")
          (match_operand:QUQ15 2 "pic30_register_operand" " za")))
   (clobber (reg:SQ B_REGNO))
   (use (reg:HI CORCON))
  ]
  "((pic30_fp_round_p() == pic30_convergent) ||
    (pic30_fp_round_p() == pic30_conventional))"
  "mpy %1,%2,B\;sac.r B,#0,%0"
)
/* If it is, we should remove the middle alternative here */

(define_insn "mul<mode>3_helper_32"
  [(set (match_operand:QUQ15 0 "pic30_move_operand"       "=rR<>,r,QST")
        (mult:QUQ15 
          (match_operand:QUQ15 1 "pic30_register_operand" " r,za,r")
          (match_operand:QUQ15 2 "pic30_register_operand" " r,za,r")))
   (clobber (match_scratch:SQ  3                         "=&r,&w,&r"))
  ]
  ""
  "*
   { 
     if ((pic30_fp_round_p() == pic30_truncation) ||
         (pic30_fp_round_p() == pic30_fastest)) {
       static const char *patterns[] = {
         \"mul.ss %1,%2,%3\;sl %3,%3\;rlc %d3,%0\",
         \"mpy %1,%2,%3\;sac %3,#0,%0\",
         \"mul.ss %1,%2,%3\;sl %3,%3\;rlc %d3,%d3\;mov %d3,%0\"
       };

       return patterns[which_alternative];
     } else if (pic30_fp_round_p() == pic30_conventional) {
       static const char *patterns[] = {
         \"mul.ss %1,%2,%3\;\"
             \"sl %3,%3\;\"
             \"rlc %d3,%d3\;\"
             \"sl %3,[w15]\;\"
             \"addc %d3,#0,%0\",

         \"mpy %1,%2,%3\;\"
             \"sac.r %3,#0,%0\",

         \"mul.ss %1,%2,%3\;\"
             \"sl %3,%3\;\"
             \"rlc %d3,%d3\;\"
             \"sl %3,[w15]\;\"
             \"addc %d3,#0,%d3\;\"
             \"mov %d3,%0\"
       };

       return patterns[which_alternative];
     } else if (pic30_fp_round_p() == pic30_convergent) {
       /* convergent rounding, doing it without branches appears to be
        * simpler. And quicker, especially on EP parts;
        *
        * For rounding, we do the add and if the remainer is 0 we mask of the
        * low bit.  Ie:
        *  res rem
        *  xx1 100 will round to xx1+1
        *  xx0 100 will round to xx0+1 (xx1) then mask back to (xx0) 
        *
        * Listen very carefully, I shall say this only wance
        */
       static const char *patterns[] = {
         \"mul.ss %1,%2,%3\;\"     /* multipy */
           \"sl %3,%3\;\"          /* shift 32-bit result left to reposition */
           \"rlc %d3,%d3\;\"       /*   binary point */
           \"sl %3,%3\;\"          /* round shift remainder up, like convgnt */
           \"bra nc,.L1_%=\;\"     /* if no carry, finish */
           \"btsc SR,#1\;\"       /* if round word == 0 */
           \"btst.c %d3,#0\;\"     /* mov low result bit to C */
                                   /*  1 + 1 = 0 (even) */
           \"addc %d3,#0,%d3\n\"
           \".L1_%=:\;\"
           \"mov %d3,%0\",

         \"mpy %1,%2,%0\;\"        /* ensure CORCONbits.US == 0? */
           \"sac.r %3,#0,%0\",

         \"mul.ss %1,%2,%3\;\"
           \"sl %3,%3\;\"
           \"rlc %d3,%d3\;\"
           \"sl %3,%3\;\"          /* round */
           \"bra nc,.L1_%=\;\"     /* if no carry, finish */
           \"btsc SR,#1\;\"       /* if round word == 0 */
           \"btst.c %d3,#0\;\"     /* mov low result bit to C */
           \"addc %d3,#0,%d3\;\"
           \".L1_%=:\;\"
           \"mov %d3,%0\" 
       };

       return patterns[which_alternative];
     } else return \"Cannot generate pattern\";
   }
  "
  [
    (set_attr "cc" "clobber")
  ]
)

(define_expand "mul<mode>3_32"
  [(set (match_operand:QUQ15 0 "pic30_move_operand" "=rRQST<>")
        (mult:QUQ15 
          (match_operand:QUQ15 1 "pic30_register_operand" "=r")
          (match_operand:QUQ15 2 "pic30_register_operand" "=r")))]
  ""
  "
  {  rtx ra,rb;

     /* for no fp inline big patterns; lets fail and call the routine */
     if (!pic30_fp_inline_p() && (pic30_fp_round_p() != pic30_truncation) &&
         (pic30_fp_round_p() != pic30_fastest))
       FAIL;
 
     /* set round mode and saturation mode 
      *   the optimizer should discard the things we don't use - ie
      *   an accumulator is not used or the saturation is already correct
      */
     ra = operands[1];
     rb = operands[2];
     if (!pic30_register_operand(ra,<MODE>mode)) 
       ra = force_reg(<MODE>mode, ra);
     if (!pic30_register_operand(rb,<MODE>mode)) 
       rb = force_reg(<MODE>mode, rb);
     emit(
       gen_mul<mode>3_helper_32(operands[0],ra,rb)
     );
     DONE;
  }"
)

/*
 * do not know if this will be successful
 */
(define_insn "ssmul<mode>3_helper_trunc_dsp_A_32"
  [(set (match_operand:QUQ15 0 "pic30_move_operand"       "=r")
        (ss_mult:QUQ15
          (match_operand:QUQ15 1 "pic30_register_operand" " za")
          (match_operand:QUQ15 2 "pic30_register_operand" " za")))
   (clobber (reg:SQ A_REGNO))
   (use (reg:HI CORCON))
  ]
  "((pic30_fp_round_p() == pic30_truncation) ||
    (pic30_fp_round_p() == pic30_fastest))"
  "mpy %1,%2,A\;sac A,#0,%0"
)

(define_insn "ssmul<mode>3_helper_trunc_dsp_B_32"
  [(set (match_operand:QUQ15 0 "pic30_move_operand"       "=r")
        (ss_mult:QUQ15
          (match_operand:QUQ15 1 "pic30_register_operand" " za")
          (match_operand:QUQ15 2 "pic30_register_operand" " za")))
   (clobber (reg:SQ B_REGNO))
   (use (reg:HI CORCON))
  ]
  "((pic30_fp_round_p() == pic30_truncation) ||
    (pic30_fp_round_p() == pic30_fastest))"
  "mpy %1,%2,B\;sac B,#0,%0"
)

(define_insn "ssmul<mode>3_helper_rnd_dsp_A_32"
  [(set (match_operand:QUQ15 0 "pic30_move_operand"       "=r")
        (ss_mult:QUQ15
          (match_operand:QUQ15 1 "pic30_register_operand" " za")
          (match_operand:QUQ15 2 "pic30_register_operand" " za")))
   (clobber (reg:SQ A_REGNO))
   (use (reg:HI CORCON))
  ]
  "((pic30_fp_round_p() == pic30_convergent) ||
    (pic30_fp_round_p() == pic30_conventional))"
  "mpy %1,%2,A\;sac.r A,#0,%0"
)

(define_insn "ssmul<mode>3_helper_rnd_dsp_B_32"
  [(set (match_operand:QUQ15 0 "pic30_move_operand"       "=r")
        (ss_mult:QUQ15
          (match_operand:QUQ15 1 "pic30_register_operand" " za")
          (match_operand:QUQ15 2 "pic30_register_operand" " za")))
   (clobber (reg:SQ B_REGNO))
   (use (reg:HI CORCON))
  ]
  "((pic30_fp_round_p() == pic30_convergent) ||
    (pic30_fp_round_p() == pic30_conventional))"
  "mpy %1,%2,B\;sac.r B,#0,%0"
)
/* If it is, we should remove the middle alternative here */

(define_insn "ssmul<mode>3_helper_32"
  [(set (match_operand:QUQ15 0 "pic30_move_operand" "=&r,r,RQST<>")
        (ss_mult:QUQ15 
          (match_operand:QUQ15 1 "pic30_register_operand" " r,za,r")
          (match_operand:QUQ15 2 "pic30_register_operand" " r,za,r")))
   (clobber (match_scratch:SQ  3                         "=&r,w,&r"))
  ]
  ""
  "*
   { 
     if ((pic30_fp_round_p() == pic30_truncation) ||
         (pic30_fp_round_p() == pic30_fastest)) {
       static const char *patterns[] = {
         \"mul.ss %1,%2,%3\;\"
           \"sl %3,%3\;\"
           \"rlc %d3,%0\;\"
           \"xor %1,%2,%3\;\"
           \"bra n,.L1_%=\;\"
           \"btsc %0,#15\;\"
           \"com %0,%0\n\"
           \".L1_%=:\",

         \"mpy %1,%2,%3\;sac %3,#0,%0\",  /* ensure CORCONbits.US == 0? */

         \"mul.ss %1,%2,%3\;\"
           \"sl %3,%3\;\"
           \"rlc %d3,%d3\;\"
           \"xor %1,%2,%3\;\"
           \"bra n,.L1_%=\;\"
           \"btsc %d3,#15\;\"
           \"com %d3,%d3\n\"
           \".L1_%=:\;\"
           \"mov %d3,%0\"
       };

       return patterns[which_alternative];
     } else if (pic30_fp_round_p() == pic30_conventional) {
       static const char *patterns[] = {
         \"mul.ss %1,%2,%3\;\"
           \"sl %3,%3\;\"
           \"rlc %d3,%d3\;\"
           \"sl %3,[w15]\;\"
           \"addc %d3,#0,%0\;\"
           \"xor %1,%2,%3\;\"
           \"bra n,.L1_%=\;\"
           \"btsc %0,#15\;\"
           \"com %0,%0\n\"
           \".L1_%=:\",

         \"mpy %1,%2,%0\;sac.r %3,#0,%0\",  /* ensure CORCONbits.US == 0? */

         \"mul.ss %1,%2,%3\;\"
           \"sl %3,%3\;\"
           \"rlc %d3,%d3\;\"
           \"sl %3,[w15]\;\"
           \"addc %d3,#0,%d3\;\"
           \"xor %1,%2,%3\;\"
           \"bra n,.L1_%=\;\"
           \"btsc %d3,#15\;\"
           \"com %d3,%d3\n\"
           \".L1_%=:\;\"
           \"mov %d3,%0\"
       };

       return patterns[which_alternative];
     } else if (pic30_fp_round_p() == pic30_convergent) {
       static const char *patterns[] = {
         \"mul.ss %1,%2,%3\;\"                    /* multiply */
           \"sl %3,%3\;\"                         /* fix binary point */
           \"rlc %d3,%0\;\"
           \"sl %3,%3\;\"                         /* move round into carry */
           \"bra nc,.L1_%=\;\"                    /* if no carry, finish */
           \"btsc SR,#1\;\"                      /* if round word == 0 */
           \"btst.c %0,#0\;\"                     /* mov low result bit to C */
                                                  /*  1 + 1 = 0 (even) */
           \"addc %0,#0,%0\n\"                    /* add carry */
           \".L1_%=:\;\"
           \"xor %1,%2,%3\;\"
           \"bra n,.L2_%=\;\"
           \"btsc %0,#15\;\"
           \"com %0,%0\n\"
           \".L2_%=:\",

         \"mpy %1,%2,%0\;sac.r %3,#0,%0\",  /* ensure CORCONbits.US == 0? */

         \"mul.ss %1,%2,%3\;\"
           \"sl %3,%3\;\"
           \"rlc %d3,%d3\;\"
           \"sl %3,%3\;\"                         /* round */
           \"bra nc,.L1_%=\;\"                    /* if no carry, finish */
           \"btsc SR,#1\;\"                      /* if round word == 0 */
           \"btst.c %d3,#0\;\"                    /* mov low result bit to C */
                                                  /*  1 + 1 = 0 (even) */
           \"addc %d3,#0,%d3\n\"
           \".L1_%=:\;\"
           \"xor %1,%2,%3\;\"
           \"bra n,.L2_%=\;\"
           \"btsc %d3,#15\;\"
           \"com %d3,%d3\n\"
           \".L2_%=:\;\"
           \"mov %d3,%0\"
       };

       return patterns[which_alternative];
     } else return \"Cannot generate pattern\";
   }
  "
  [
    (set_attr "cc" "clobber")
  ]
)

(define_expand "ssmul<mode>3_32"
  [(set (match_operand:QUQ15 0 "pic30_move_operand" "=rRQST<>")
        (ss_mult:QUQ15 
          (match_operand:QUQ15 1 "pic30_register_operand" "=r")
          (match_operand:QUQ15 2 "pic30_register_operand" "=r")))]
  ""
  "
  {  rtx ra,rb;

     /* for no fp inline big patterns; lets fail and call the routine */
     if (!pic30_fp_inline_p() && (pic30_fp_round_p() != pic30_truncation) &&
         (pic30_fp_round_p() != pic30_fastest))
       FAIL;

     /* set round mode and saturation mode
      *   the optimizer should discard the things we don't use - ie
      *   an accumulator is not used or the saturation is already correct
      */
     ra = operands[1];
     rb = operands[2];
     if (!pic30_register_operand(ra,<MODE>mode)) 
       ra = force_reg(<MODE>mode, ra);
     if (!pic30_register_operand(rb,<MODE>mode)) 
       rb = force_reg(<MODE>mode, rb);
     emit(
       gen_ssmul<mode>3_helper_32(operands[0],ra,rb)
     );
     DONE;
  }"
)

(define_expand "usmul<mode>3_32"
  [(set (match_operand:QUQ15 0 "pic30_move_operand" "=rRQST<>")
        (us_mult:QUQ15
          (match_operand:QUQ15 1 "pic30_register_operand" "=r")
          (match_operand:QUQ15 2 "pic30_register_operand" "=r")))]
  ""
  "
  {  rtx ra,rb;

     /* for no fp inline big patterns; lets fail and call the routine */
     if (!pic30_fp_inline_p() && (pic30_fp_round_p() != pic30_truncation) &&
         (pic30_fp_round_p() != pic30_fastest))
       FAIL;

     /* set round mode and saturation mode
      *   the optimizer should discard the things we don't use - ie
      *   an accumulator is not used or the saturation is already correct
      */

     /* unsigned multiply on this device cannot over-flow
        so we don't need to do saturation - translate it to the unsaturating
        signed multiply */
     ra = operands[1];
     rb = operands[2];
     if (!pic30_register_operand(ra,<MODE>mode))
       ra = force_reg(<MODE>mode, ra);
     if (!pic30_register_operand(rb,<MODE>mode))
       rb = force_reg(<MODE>mode, rb);
     emit(
       gen_mul<mode>3_helper_32(operands[0],ra,rb)
     );
     DONE;
  }"
)

;; *** multiply
;  32-bit
; handled by helper routines in libfx

;; *** divide
;  16-bit

(define_insn "udiv<mode>3_helper_32"
  [(set (match_operand:UQ15 0 "pic30_register_operand"    "=a,?b,??&e")
        (udiv:UQ15 
          (match_operand:UQ15 1 "pic30_register_operand"   "r, r,  r")
          (match_operand:UQ15 2 "pic30_ereg_operand"       "e, e,  e")))
   (clobber (reg:HI RCOUNT))
  ]
  ""
  "*
  {
     rtx w0 = gen_rtx_REG(HImode, WR0_REGNO);
     rtx w1 = gen_rtx_REG(HImode, WR1_REGNO);
     if ((pic30_fp_round_p() == pic30_truncation) ||
         (pic30_fp_round_p() == pic30_fastest)) {
       switch (which_alternative) {
         case 0:  /*
                ** wm/wn -> w0
                */
                  if (pic30_dead_or_set_p(insn, w1)) {
                    return \"repeat #__TARGET_DIVIDE_CYCLES\;\"
                           \"divf %1,%2\";
                  } else {
                      return \"mov w1,[w15++]\;\"
                             \"repeat #__TARGET_DIVIDE_CYCLES\;\"
                             \"divf %1,%2\;\"
                             \"mov [--w15],w1\";
                }
         case 1:  /*
                  ** wm/wn -> w1
                  */
                  if (pic30_dead_or_set_p(insn, w0)) {
                    return \"repeat #__TARGET_DIVIDE_CYCLES\;\"
                           \"divf %1,%2\;\"
                           \"mov w0,%0\";
                  } else {
                    return \"mov w0,[w15++]\;\"
                           \"repeat #__TARGET_DIVIDE_CYCLES\;\"
                           \"divf %1,%2\;\"
                           \"mov w0,%0\;\"
                           \"mov [--w15],w0\";
                  }
         case 2:  /*
                  ** wm/wn -> we
                  */
                  if (pic30_dead_or_set_p(insn, w0) &&
                      pic30_dead_or_set_p(insn, w1)) {
                    return \"repeat #__TARGET_DIVIDE_CYCLES\;\"
                           \"divf %1,%2\;\"
                           \"mov w0,%0\";
                  } else {
                    return \"mov.d w0,[w15++]\;\"
                           \"repeat #__TARGET_DIVIDE_CYCLES\;\"
                           \"divf %1,%2\;\"
                           \"mov w0,%0\;\"
                           \"mov.d [--w15],w0\";
                  }
         default: gcc_assert(0);
       }
     } else if (pic30_fp_round_p() == pic30_conventional) {
       switch (which_alternative) {
         case 0:  /*
                ** wm/wn -> w0
                */
                  if (pic30_dead_or_set_p(insn, w1)) {
                                                     /* all follow sim. flow */
                    return 
                           \"repeat #__TARGET_DIVIDE_CYCLES\;\"
                           \"divf %1,%2\;\"
                           \"sl w1,w1\;\"
                           \"cp w1,%2\;\"            /* compare 2*remainder */
                           \"addc %0,#0,%0\";        /* inc if carry */
                  } else {
                      return \"mov w1,[w15++]\;\"
                             \"repeat #__TARGET_DIVIDE_CYCLES\;\"
                             \"divf %1,%2\;\"
                             \"sl w1,w1\;\"
                             \"cp w1,%2\;\"
                             \"addc %0,#0,%0\;\"
                             \"mov [--w15],w1\";
                }
         case 1:  /*
                  ** wm/wn -> w1
                  */
                  if (pic30_dead_or_set_p(insn, w0)) {
                    return 
                           \"repeat #__TARGET_DIVIDE_CYCLES\;\"
                           \"divf %1,%2\;\"
                           \"sl w1,w1\;\"
                           \"mov w0,%0\;\"
                           \"cp w1,%2\;\"
                           \"addc %0,#0,%0\;\";
                  } else {
                    return \"mov w0,[w15++]\;\"
                           \"repeat #__TARGET_DIVIDE_CYCLES\;\"
                           \"divf %1,%2\;\"
                           \"sl w1,w1\;\"
                           \"mov w0,%0\;\"
                           \"cp w1,%2\;\"
                           \"addc %0,#0,%0\;\"
                           \"mov [--w15],w0\";
                  }
         case 2:  /*
                  ** wm/wn -> we
                  */
                  if (pic30_dead_or_set_p(insn, w0) &&
                      pic30_dead_or_set_p(insn, w1)) {
                    return 
                           \"repeat #__TARGET_DIVIDE_CYCLES\;\"
                           \"divf %1,%2\;\"
                           \"sl w1,w1\;\"
                           \"cp w1,%2\;\"
                           \"mov w0,%0\;\"
                           \"addc %0,#0,%0\";
                  } else {
                    return \"mov.d w0,[w15++]\;\"
                           \"repeat #__TARGET_DIVIDE_CYCLES\;\"
                           \"divf %1,%2\;\"
                           \"sl w1,w1\;\"
                           \"mov w0,%0\;\"
                           \"cp w1,%2\;\"
                           \"addc %0,#0,%0\;\" 
                           \"mov.d [--w15],w0\";
                  }
         default: gcc_assert(0);
       }
     } else if (pic30_fp_round_p() == pic30_convergent) {
       switch (which_alternative) {
         case 0:  /*
                  ** wm/wn -> w0
                  */
                  if (pic30_dead_or_set_p(insn, w1)) {
                                                     /* convergent follows this
                                                        form */
                    return 
                           \"repeat #__TARGET_DIVIDE_CYCLES\;\"
                           \"divf %1,%2\;\"
                           \"sl w1,w1\;\"
                           \"cp w1,%2\;\"            /* cp divisor w/ 2*rem */
                           \"btsc SR,#1\;\"         /* if zero ... */
                           \"btst.c w0,#0\;\"        /*   copy low bit to c */
                                                     /*   ie if low bit == 0 */
                                                     /*      don't add */
                           \"addc.w w0,#0,%0\";      /* add carry */
                  } else {
                      return \"mov w1,[w15++]\;\"
                             \"repeat #__TARGET_DIVIDE_CYCLES\;\"
                             \"divf %1,%2\;\"
                             \"sl w1,w1\;\"
                             \"cp w1,%2\;\"
                             \"btsc SR,#1\;\"
                             \"btst.c w0,#0\;\"
                             \"addc.w w0,#0,%0\;\" 
                             \"mov [--w15],w1\";
                  }
         case 1:  /*
                  ** wm/wn -> w1
                  */
                  if (pic30_dead_or_set_p(insn, w0)) {
                    return 
                           \"repeat #__TARGET_DIVIDE_CYCLES\;\"
                           \"divf %1,%2\;\"
                           \"sl w1,w1\;\"
                           \"cp w1,%2\;\"
                           \"btsc SR,#1\;\"
                           \"btst.c w0,#0\;\"
                           \"addc.w w0,#0,%0\";
                  } else {
                    return \"mov w0,[w15++]\;\"
                           \"sl %2,[w15++]\;\"
                           \"repeat #__TARGET_DIVIDE_CYCLES\;\"
                           \"divf %1,%2\;\"
                           \"sl w1,w1\;\"
                           \"cp w1,[--w15]\;\"
                           \"btsc SR,#1\;\"
                           \"btst.c w0,#0\;\"
                           \"addc.w w0,#0,%0\;\" 
                           \"mov [--w15],w0\";
                  }
         case 2 : /*
                  ** wm/wn -> we
                  */
                  if (pic30_dead_or_set_p(insn, w0) &&
                      pic30_dead_or_set_p(insn, w1)) {
                    return \"repeat #__TARGET_DIVIDE_CYCLES\;\"
                           \"divf %1,%2\;\"
                           \"sl w1,w1\;\"
                           \"cp w1,%2\;\"
                           \"btsc SR,#1\;\"
                           \"btst.c w0,#0\;\"
                           \"addc.w w0,#0,%0\";
                  } else {
                    return \"mov.d w0,[w15++]\;\"
                           \"repeat #__TARGET_DIVIDE_CYCLES\;\"
                           \"divf %1,%2\;\"
                           \"sl w1,w1\;\"
                           \"cp w1,%2\;\"
                           \"btsc SR,#1\;\"
                           \"btst.c w0,#0\;\"
                           \"addc.w w0,#0,%0\;\" 
                           \"mov.d [--w15],w0\";
                  }
         default: gcc_assert(0);
       }
     }
     gcc_assert(0);
     return \"nop\";
  } 
  "
  [
    (set_attr "cc" "clobber")
    (set_attr "type" "def")
  ]
)

; check for == and return 0x8000 in all cases (if sign is positive then 
;   its an valid invalid answer)

(define_insn "div<mode>3_helper_32"
  [(set (match_operand:QUQ15   0 "pic30_register_operand"  "=a,?b,??&e")
        (div:QUQ15 
          (match_operand:QUQ15 1 "pic30_register_operand"   "r, r,  r")
          (match_operand:QUQ15 2 "pic30_ereg_operand"       "e, e,  e")))
   (match_operand:HI           3 "pic30_register_operand"   "e, e,  e")
   (clobber (match_scratch:HI  4                          "=&e,&e, &e"))
   (clobber (reg:HI RCOUNT))
  ]
  ""
  "*
  {
     rtx w0 = gen_rtx_REG(HImode, WR0_REGNO);
     rtx w1 = gen_rtx_REG(HImode, WR1_REGNO);
     if ((pic30_fp_round_p() == pic30_truncation) ||
         (pic30_fp_round_p() == pic30_fastest)) {
       switch (which_alternative) {
         case 0:  /*
                ** wm/wn -> w0
                */
                  if (pic30_dead_or_set_p(insn, w1)) {
                    return \"sub %1,%2,[w15]\;\"
                           \"mov #0x8000,%0\;\"
                           \"bra z,.LE%=\;\"
                           \"repeat #__TARGET_DIVIDE_CYCLES\;\"
                           \"divf %1,%2\;\"
                           \"com w0,%4\;\"              /* complement in case
                                                            of negate */
                           \"ff1r w1,w1\;\"             /* C = w1 == 0 */
                           \"btsc %3,#15\;\"            /* if negative, neg */
                           \"addc %4,#0,%0\n\"
                           \".LE%=:\";
                  } else {
                    return \"sub %1,%2,[w15]\;\"
                           \"mov #0x8000,%0\;\"
                           \"bra z,.LE%=\;\"
                           \"mov w1,[w15++]\;\"
                           \"repeat #__TARGET_DIVIDE_CYCLES\;\"
                           \"divf %1,%2\;\"
                           \"com w0,%4\;\"              /* complement in case
                                                            of negate */
                           \"ff1r w1,w1\;\"             /* C = w1 == 0 */
                           \"btsc %3,#15\;\"          /* if negative, neg */
                           \"addc %4,#0,%0\;\"
                           \"mov [--w15],w1\n\"
                           \".LE%=:\";
                }
         case 1:  /*
                  ** wm/wn -> w1
                  */
                  if (pic30_dead_or_set_p(insn, w0)) {
                    return \"sub %1,%2,[w15]\;\"
                           \"mov #0x8000,%0\;\"
                           \"bra z,.LE%=\;\"
                           \"repeat #__TARGET_DIVIDE_CYCLES\;\"
                           \"divf %1,%2\;\"
                           \"com w0,%4\;\"              /* complement in case
                                                            of negate */
                           \"ff1r w1,w1\;\"             /* C = w1 == 0 */
                           \"btsc %3,#15\;\"            /* if negative, neg */
                           \"addc %4,#0,w0\;\"
                           \"mov w0,%0\n\"
                           \".LE%=:\";
                  } else {
                    return \"sub %1,%2,[w15]\;\"
                           \"mov #0x8000,%0\;\"
                           \"bra z,.LE%=\;\"
                           \"mov w0,[w15++]\;\"
                           \"repeat #__TARGET_DIVIDE_CYCLES\;\"
                           \"divf %1,%2\;\"
                           \"com w0,%4\;\"              /* complement in case
                                                            of negate */
                           \"ff1r w1,w1\;\"             /* C = w1 == 0 */
                           \"btsc %3,#15\;\"            /* if negative, neg */
                           \"addc %4,#0,w0\;\"
                           \"mov w0,%0\;\"
                           \"mov [--w15],w0\n\"
                           \".LE%=:\";
                  }
         case 2 : /*
                  ** wm/wn -> we
                  */
                  if (pic30_dead_or_set_p(insn, w0) &&
                      pic30_dead_or_set_p(insn, w1)) {
                    return \"sub %1,%2,[w15]\;\"
                           \"mov #0x8000,%0\;\"
                           \"bra z,.LE%=\;\"
                           \"repeat #__TARGET_DIVIDE_CYCLES\;\"
                           \"divf %1,%2\;\"
                           \"com w0,%4\;\"              /* complement in case
                                                            of negate */
                           \"ff1r w1,w1\;\"             /* C = w1 == 0 */
                           \"btsc %3,#15\;\"            /* if negative, neg */
                           \"addc %4,#0,w0\;\"
                           \"mov w0,%0\n\"
                           \".LE%=\";
                  } else {
                    return \"sub %1,%2,[w15]\;\"
                           \"mov #0x8000,%0\;\"
                           \"bra z,.LE%=\;\"
                           \"mov.d w0,[w15++]\;\"
                           \"repeat #__TARGET_DIVIDE_CYCLES\;\"
                           \"divf %1,%2\;\"
                           \"com w0,%4\;\"              /* complement in case
                                                            of negate */
                           \"ff1r w1,w1\;\"             /* C = w1 == 0 */
                           \"btsc %3,#15\;\"            /* if negative, neg */
                           \"addc %4,#0,w0\;\"
                           \"mov w0,%0\;\"
                           \"mov.d [--w15],w0\n\"
                           \".LE%=:\";
                  }
         default: gcc_assert(0);
       }
     } else if (pic30_fp_round_p() == pic30_conventional) {
       switch (which_alternative) {
         case 0:  /*
                ** wm/wn -> w0
                */
                  if (pic30_dead_or_set_p(insn, w1)) {
                                                     /* all follow . flow */
                    return \"sub %1,%2,[w15]\;\"
                           \"mov #0x8000,%0\;\"
                           \"bra z,.LE%=\;\"
                           \"repeat #__TARGET_DIVIDE_CYCLES\;\"
                           \"divf %1,%2\;\"
                           \"sl w1,%4\;\"
                           \"sub %4,%2,%4\;\"        /* compare 2*remainder */
                           \"btss SR,#0\;\"         /* carry set, use new val*/
                           \"mov %4,w1\;\" 
                           \"rlc w0,w0\;\"           /* put R in low bit */
                           \"ff1r w1,%4\;\"          /* C = w1 == 0 */
                           \"com w0,%4\;\"           /* complement in case
                                                        of negate */
                           \"btsc %3,#15\;\"         /* if negative, neg */
                           \"addc %4,#0,w0\;\"
                           \"rlc %3,[w15]\;\"        /* get sign in carry */
                           \"rrc w0,w0\;\"           /* div by 2 again */
                           \"addc w0,#0,%0\n\"       /* add in round */
                           \".LE%=:\";
                  } else {
                    return \"sub %1,%2,[w15]\;\"
                           \"mov #0x8000,%0\;\"
                           \"bra z,.LE%=\;\"
                           \"mov w1,[w15++]\;\"
                           \"repeat #__TARGET_DIVIDE_CYCLES\;\"
                           \"divf %1,%2\;\"
                           \"sl w1,%4\;\"
                           \"sub %4,%2,%4\;\"        /* compare 2*remainder */
                           \"btss SR,#0\;\"         /* carry set, use new val*/
                           \"mov %4,w1\;\"
                           \"rlc w0,w0\;\"           /* put R in low bit */
                           \"ff1r w1,%4\;\"          /* C = w1 == 0 */
                           \"com w0,%4\;\"           /* complement in case
                                                        of negate */
                           \"btsc %3,#15\;\"         /* if negative, neg */
                           \"addc %4,#0,w0\;\"
                           \"rlc %3,[w15]\;\"        /* get sign in carry */
                           \"rrc w0,w0\;\"           /* div by 2 again */
                           \"addc w0,#0,%0\;\"       /* add in round */
                           \"mov [--w15],w1\n\"
                           \".LE%=:\";
                }
         case 1:  /*
                  ** wm/wn -> w1
                  */
                  if (pic30_dead_or_set_p(insn, w0)) {
                    return \"sub %1,%2,[w15]\;\"
                           \"mov #0x8000,%0\;\"
                           \"bra z,.LE%=\;\"
                           \"repeat #__TARGET_DIVIDE_CYCLES\;\"
                           \"divf %1,%2\;\"
                           \"ff1r w1,%4\;\"          /* C = w1 == 0 */
                           \"sl w1,%4\;\"
                           \"sub %4,%2,%4\;\"        /* compare 2*remainder */
                           \"btss SR,#0\;\"         /* carry set, use new val*/
                           \"mov %4,w1\;\"
                           \"rlc w0,w0\;\"           /* put R in low bit */
                           \"com w0,%4\;\"           /* complement in case
                                                        of negate */
                           \"btsc %3,#15\;\"         /* if negative, neg */
                           \"addc %4,#0,w0\;\"
                           \"rlc %3,[w15]\;\"        /* get sign in carry */
                           \"rrc w0,w0\;\"           /* div by 2 again */
                           \"addc w0,#0,%0\n\"       /* add in round */
                           \".LE%=:\";
                  } else {
                    return \"sub %1,%2,[w15]\;\"
                           \"mov #0x8000,%0\;\"
                           \"bra z,.LE%=\;\"
                           \"mov w0,[w15++]\;\"
                           \"repeat #__TARGET_DIVIDE_CYCLES\;\"
                           \"divf %1,%2\;\"
                           \"sl w1,%4\;\"
                           \"sub %4,%2,%4\;\"        /* compare 2*remainder */
                           \"btss SR,#0\;\"         /* carry set, use new val*/
                           \"mov %4,w1\;\"
                           \"rlc w0,w0\;\"           /* put R in low bit */
                           \"ff1r w1,%4\;\"          /* C = w1 == 0 */
                           \"com w0,%4\;\"           /* complement in case
                                                        of negate */
                           \"btsc %3,#15\;\"         /* if negative, neg */
                           \"addc %4,#0,w0\;\"
                           \"rlc %3,[w15]\;\"        /* get sign in carry */
                           \"rrc w0,w0\;\"           /* div by 2 again */
                           \"addc w0,#0,%0\;\"       /* add in round */
                           \"mov [--w15],w0\n\"
                           \".LE%=:\";
                  }
         case 2:  /*
                  ** wm/wn -> we
                  */
                  if (pic30_dead_or_set_p(insn, w0) &&
                      pic30_dead_or_set_p(insn, w1)) {
                    return \"sub %1,%2,[w15]\;\"
                           \"mov #0x8000,%0\;\"
                           \"bra z,.LE%=\;\"
                           \"repeat #__TARGET_DIVIDE_CYCLES\;\"
                           \"divf %1,%2\;\"
                           \"sl w1,%4\;\"
                           \"sub %4,%2,%4\;\"        /* compare 2*remainder */
                           \"btss SR,#0\;\"         /* carry set, use new val*/
                           \"mov %4,w1\;\"
                           \"rlc w0,w0\;\"           /* put R in low bit */
                           \"ff1r w1,%4\;\"          /* C = w1 == 0 */
                           \"com w0,%4\;\"           /* complement in case
                                                        of negate */
                           \"btsc %3,#15\;\"         /* if negative, neg */
                           \"addc %4,#0,w0\;\"
                           \"rlc %3,[w15]\;\"        /* get sign in carry */
                           \"rrc w0,w0\;\"           /* div by 2 again */
                           \"addc w0,#0,%0\n\"       /* add in round */
                           \".LE%=:\";
                  } else {
                    return \"sub %1,%2,[w15]\;\"
                           \"mov #0x8000,%0\;\"
                           \"bra z,.LE%=\;\"
                           \"mov.d w0,[w15++]\;\"
                           \"repeat #__TARGET_DIVIDE_CYCLES\;\"
                           \"divf %1,%2\;\"
                           \"sl w1,%4\;\"
                           \"sub %4,%2,%4\;\"        /* compare 2*remainder */
                           \"btss SR,#0\;\"         /* carry set, use new val*/
                           \"mov %4,w1\;\"
                           \"rlc w0,w0\;\"           /* put R in low bit */
                           \"ff1r w1,%4\;\"          /* C = w1 == 0 */
                           \"com w0,%4\;\"           /* complement in case
                                                        of negate */
                           \"btsc %3,#15\;\"         /* if negative, neg */
                           \"addc %4,#0,w0\;\"
                           \"rlc %3,[w15]\;\"        /* get sign in carry */
                           \"rrc w0,w0\;\"           /* div by 2 again */
                           \"addc w0,#0,%0\;\"       /* add in round */
                           \"mov.d [--w15],w0\n\"
                           \".LE%=:\";
                  }
         default: gcc_assert(0);
       }
     } else if (pic30_fp_round_p() == pic30_convergent) {
       switch (which_alternative) {
         case 0:  /*
                  ** wm/wn -> w0
                  */
                  if (pic30_dead_or_set_p(insn, w1)) {
                                                     /* convergent follows this
                                                        form */
                    return \"sub %1,%2,[w15]\;\"
                           \"mov #0x8000,%0\;\"
                           \"bra z,.LE%=\;\"
                           \"repeat #__TARGET_DIVIDE_CYCLES\;\"
                           \"divf %1,%2\;\"
                           \"sl w1,%4\;\"
                           \"sub %4,%2,%4\;\"        /* compare 2*remainder */
                           \"btss SR,#0\;\"         /* carry set, use new val*/
                           \"mov %4,w1\;\"
                           \"btsc SR,#1\;\"
                           \"btst.c w0,#0\;\"        /*   copy low bit to c */
                                                     /*   ie if low bit == 0 */
                                                     /*      don't add */
                           \"rlc w0,w0\;\"           /* put R in low bit */

                           \"ff1r w1,%4\;\"          /* C = w1 == 0 */
                           \"com w0,%4\;\"           /* complement in case
                                                        of negate */
                           \"btsc %3,#15\;\"         /* if negative, neg */
                           \"addc %4,#0,w0\;\"
                           \"rlc %3,[w15]\;\"        /* get sign in carry */
                           \"rrc w0,w0\;\"           /* div by 2 again */
                           \"addc.w w0,#0,%0\n\"     /* add carry */
                           \".LE%=:\";
                  } else {
                    return \"sub %1,%2,[w15]\;\"
                           \"mov #0x8000,%0\;\"
                           \"bra z,.LE%=\;\"
                           \"mov w1,[w15++]\;\"
                           \"repeat #__TARGET_DIVIDE_CYCLES\;\"
                           \"divf %1,%2\;\"
                           \"ff1r w1,%4\;\"          /* C = w1 == 0 */
                           \"sl w1,%4\;\"
                           \"sub %4,%2,%4\;\"        /* compare 2*remainder */
                           \"btss SR,#0\;\"         /* carry set, use new val*/
                           \"mov %4,w1\;\"
                           \"btsc SR,#1\;\"
                           \"btst.c w0,#0\;\"        /*   copy low bit to c */
                                                     /*   ie if low bit == 0 */
                                                     /*      don't add */
                           \"rlc w0,w0\;\"           /* put R in low bit */
                           \"com w0,%4\;\"           /* complement in case
                                                        of negate */
                           \"btsc %3,#15\;\"         /* if negative, neg */
                           \"addc %4,#0,w0\;\"
                           \"rlc %3,[w15]\;\"        /* get sign in carry */
                           \"rrc w0,w0\;\"           /* div by 2 again */
                           \"addc.w w0,#0,%0\;\"     /* add carry */
                           \"mov [--w15],w1\n\"
                           \".LE%=:\";
                  }
         case 1:  /*
                  ** wm/wn -> w1
                  */
                  if (pic30_dead_or_set_p(insn, w0)) {
                    return \"sub %1,%2,[w15]\;\"
                           \"mov #0x8000,%0\;\"
                           \"bra z,.LE%=\;\"
                           \"repeat #__TARGET_DIVIDE_CYCLES\;\"
                           \"divf %1,%2\;\"
                           \"sl w1,%4\;\"
                           \"sub %4,%2,%4\;\"        /* compare 2*remainder */
                           \"btss SR,#0\;\"         /* carry set, use new val*/
                           \"mov %4,w1\;\"
                           \"btsc SR,#1\;\"
                           \"btst.c w0,#0\;\"        /*   copy low bit to c */
                                                     /*   ie if low bit == 0 */
                                                     /*      don't add */
                           \"rlc w0,w0\;\"           /* put R in low bit */

                           \"ff1r w1,%4\;\"          /* C = w1 == 0 */
                           \"com w0,%4\;\"           /* complement in case
                                                        of negate */
                           \"btsc %3,#15\;\"         /* if negative, neg */
                           \"addc %4,#0,w0\;\"
                           \"sl w1,w1\;\"
                           \"rlc %3,[w15]\;\"        /* get sign in carry */
                           \"rrc w0,w0\;\"           /* div by 2 again */
                           \"addc.w w0,#0,%0\n\"     /* add carry */
                           \".LE%=:\";
                  } else {
                    return \"sub %1,%2,[w15]\;\"
                           \"mov #0x8000,%0\;\"
                           \"bra z,.LE%=\;\"
                           \"mov w0,[w15++]\;\"
                           \"repeat #__TARGET_DIVIDE_CYCLES\;\"
                           \"divf %1,%2\;\"
                           \"sl w1,%4\;\"
                           \"sub %4,%2,%4\;\"        /* compare 2*remainder */
                           \"btss SR,#0\;\"         /* carry set, use new val*/
                           \"mov %4,w1\;\"
                           \"btsc SR,#1\;\"
                           \"btst.c w0,#0\;\"        /*   copy low bit to c */
                                                     /*   ie if low bit == 0 */
                                                     /*      don't add */
                           \"rlc w0,w0\;\"           /* put R in low bit */

                           \"ff1r w1,%4\;\"          /* C = w1 == 0 */
                           \"com w0,%4\;\"           /* complement in case
                                                        of negate */
                           \"btsc %3,#15\;\"         /* if negative, neg */
                           \"addc %4,#0,w0\;\"
                           \"rlc %3,[w15]\;\"        /* get sign in carry */
                           \"rrc w0,w0\;\"           /* div by 2 again */
                           \"addc.w w0,#0,%0\;\"     /* add carry */
                           \"mov [--w15],w0\n\"
                           \".LE%=:\";
                  }
         case 2:  /*
                  ** wm/wn -> we
                  */
                  if (pic30_dead_or_set_p(insn, w0) &&
                      pic30_dead_or_set_p(insn, w1)) {
                    return \"sub %1,%2,[w15]\;\"
                           \"mov #0x8000,%0\;\"
                           \"bra z,.LE%=\;\"
                           \"repeat #__TARGET_DIVIDE_CYCLES\;\"
                           \"divf %1,%2\;\"
                           \"sl w1,%4\;\"
                           \"sub %4,%2,%4\;\"        /* compare 2*remainder */
                           \"btss SR,#0\;\"         /* carry set, use new val*/
                           \"mov %4,w1\;\"
                           \"btsc SR,#1\;\"
                           \"btst.c w0,#0\;\"        /*   copy low bit to c */
                                                     /*   ie if low bit == 0 */
                                                     /*      don't add */
                           \"rlc w0,w0\;\"           /* put R in low bit */

                           \"ff1r w1,%4\;\"          /* C = w1 == 0 */
                           \"com w0,%4\;\"           /* complement in case
                                                        of negate */
                           \"btsc %3,#15\;\"         /* if negative, neg */
                           \"addc %4,#0,w0\;\"
                           \"rlc %3,[w15]\;\"        /* get sign in carry */
                           \"rrc w0,w0\;\"           /* div by 2 again */
                           \"addc.w w0,#0,%0\n\"     /* add carry */
                           \".LE%=:\";
                  } else {
                    return \"sub %1,%2,[w15]\;\"
                           \"mov #0x8000,%0\;\"
                           \"bra z,.LE%=\;\"
                           \"mov.d w0,[w15++]\;\"
                           \"repeat #__TARGET_DIVIDE_CYCLES\;\"
                           \"divf %1,%2\;\"
                           \"sl w1,%4\;\"
                           \"sub %4,%2,%4\;\"        /* compare 2*remainder */
                           \"btss SR,#0\;\"         /* carry set, use new val*/
                           \"mov %4,w1\;\"
                           \"btsc SR,#1\;\"
                           \"btst.c w0,#0\;\"        /*   copy low bit to c */
                                                     /*   ie if low bit == 0 */
                                                     /*      don't add */
                           \"rlc w0,w0\;\"           /* put R in low bit */

                           \"ff1r w1,%4\;\"          /* C = w1 == 0 */
                           \"com w0,%4\;\"           /* complement in case
                                                        of negate */
                           \"btsc %3,#15\;\"         /* if negative, neg */
                           \"addc %4,#0,w0\;\"
                           \"sl w1,w1\;\"
                           \"rlc %3,[w15]\;\"        /* get sign in carry */
                           \"rrc w0,w0\;\"           /* div by 2 again */
                           \"addc.w w0,#0,%0\;\"     /* add carry */
                           \"mov.d [--w15],w0\n\"
                           \".LE%=:\";
                  }
         default: gcc_assert(0);
       }
     }
     gcc_assert(0);
     return \"nop\";
  } 
  "
  [
    (set_attr "cc" "clobber")
    (set_attr "type" "def")
  ]
)

(define_insn "usatdiv<mode>3_helper_32"
  [(set (match_operand:UQ15 0 "pic30_register_operand"    "=a,?b,??&e")
        (us_div:UQ15 
          (match_operand:UQ15 1 "pic30_register_operand"   "r, r,  r")
          (match_operand:UQ15 2 "pic30_ereg_operand"       "e, e,  e")))
   (clobber (reg:HI RCOUNT))
  ]
  ""
  "*
  {
     rtx w0 = gen_rtx_REG(HImode, WR0_REGNO);
     rtx w1 = gen_rtx_REG(HImode, WR1_REGNO);
     if ((pic30_fp_round_p() == pic30_truncation) ||
         (pic30_fp_round_p() == pic30_fastest)) {
       switch (which_alternative) {
         case 0:  /*
                ** wm/wn -> w0
                */
                  if (pic30_dead_or_set_p(insn, w1)) {
                    return \"repeat #__TARGET_DIVIDE_CYCLES\;\"
                           \"divf %1,%2\;\"
                           \"btsc SR,#2\;\"
                           \"mov #0x7FFF,%0\";
                  } else {
                      return \"mov w1,[w15++]\;\"
                             \"repeat #__TARGET_DIVIDE_CYCLES\;\"
                             \"divf %1,%2\;\"
                             \"mov [--w15],w1\;\"
                             \"btsc SR,#2\;\"
                             \"mov #0x7FFF,%0\";
                }
         case 1:  /*
                  ** wm/wn -> w1
                  */
                  if (pic30_dead_or_set_p(insn, w0)) {
                    return \"repeat #__TARGET_DIVIDE_CYCLES\;\"
                           \"divf %1,%2\;\"
                           \"mov w0,%0\;\"
                           \"btsc SR,#2\;\"
                           \"mov #0x7FFF,%0\";
                  } else {
                    return \"mov w0,[w15++]\;\"
                           \"repeat #__TARGET_DIVIDE_CYCLES\;\"
                           \"divf %1,%2\;\"
                           \"mov w0,%0\;\"
                           \"mov [--w15],w0\;\"
                           \"btsc SR,#2\;\"
                           \"mov #0x7FFF,%0\";
                  } 
         case 2:  /*
                  ** wm/wn -> we
                  */
                  if (pic30_dead_or_set_p(insn, w0) &&
                      pic30_dead_or_set_p(insn, w1)) {
                    return \"repeat #__TARGET_DIVIDE_CYCLES\;\"
                           \"divf %1,%2\;\"
                           \"mov w0,%0\;\"
                           \"btsc SR,#2\;\"
                           \"mov #0x7FFF,%0\";
                  } else {
                    return \"mov.d w0,[w15++]\;\"
                           \"repeat #__TARGET_DIVIDE_CYCLES\;\"
                           \"divf %1,%2\;\"
                           \"mov w0,%0\;\"
                           \"mov.d [--w15],w0\;\"
                           \"btsc SR,#2\;\"
                           \"mov #0x7FFF,%0\";
                  }
         default: gcc_assert(0);
       }
     } else if (pic30_fp_round_p() == pic30_conventional) {
       switch (which_alternative) {
         case 0:  /*
                ** wm/wn -> w0
                */
                  if (pic30_dead_or_set_p(insn, w1)) {
                                                     /* all follow sim. flow */
                    return 
                           \"repeat #__TARGET_DIVIDE_CYCLES\;\"
                           \"divf %1,%2\;\"
                           \"bra ov,.OV%=\;\"
                           \"sl w1,w1\;\"
                           \"cp w1,%2\;\"            /* compare 2*remainder */
                           \"addc %0,#0,%0\n\"           /* inc if carry */
                           \".OV%=:\;\"
                           \"btsc SR,#2\;\"
                           \"mov #0x7FFF,%0\";
                  } else {
                      return \"mov w1,[w15++]\;\"
                             \"repeat #__TARGET_DIVIDE_CYCLES\;\"
                             \"divf %1,%2\;\"
                             \"bra ov,.OV%=\;\"
                             \"sl w1,w1\;\"
                             \"cp w1,%2\;\"
                             \"addc %0,#0,%0\n\"
                             \".OV%=:\;\"
                             \"mov [--w15],w1\;\"
                             \"btsc SR,#2\;\"
                             \"mov #0x7FFF,%0\";
                }
         case 1:  /*
                  ** wm/wn -> w1
                  */
                  if (pic30_dead_or_set_p(insn, w0)) {
                    return 
                           \"repeat #__TARGET_DIVIDE_CYCLES\;\"
                           \"divf %1,%2\;\"
                           \"bra ov,.OV%=\;\"
                           \"sl w1,w1\;\"
                           \"mov w0,%0\;\"
                           \"cp w1,%2\;\"
                           \"addc %0,#0,%0\n\"
                           \".OV%=:\;\"
                           \"btsc SR,#2\;\"
                           \"mov #0x7FFF,%0\";
                  } else {
                    return \"mov w0,[w15++]\;\"
                           \"repeat #__TARGET_DIVIDE_CYCLES\;\"
                           \"divf %1,%2\;\"
                           \"bra ov,.OV%=\;\"
                           \"sl w1,w1\;\"
                           \"mov w0,%0\;\"
                           \"cp w1,%2\;\"
                           \"addc %0,#0,%0\n\"
                           \".OV%=:\;\"
                           \"mov [--w15],w0\;\"
                           \"btsc SR,#2\;\"
                           \"mov #0x7FFF,%0\";
                  }
         case 2:  /*
                  ** wm/wn -> we
                  */
                  if (pic30_dead_or_set_p(insn, w0) &&
                      pic30_dead_or_set_p(insn, w1)) {
                    return 
                           \"repeat #__TARGET_DIVIDE_CYCLES\;\"
                           \"divf %1,%2\;\"
                           \"bra ov,.OV%=\;\"
                           \"sl w1,w1\;\"
                           \"mov w0,%0\;\"
                           \"cp w1,%2\;\"
                           \"addc %0,#0,%0\n\"
                           \".OV%=:\;\"
                           \"btsc SR,#2\;\"
                           \"mov #0x7FFF,%0\";
                  } else {
                    return \"mov.d w0,[w15++]\;\"
                           \"repeat #__TARGET_DIVIDE_CYCLES\;\"
                           \"divf %1,%2\;\"
                           \"bra ov,.OV%=\;\"
                           \"sl w1,w1\;\"
                           \"mov w0,%0\;\"
                           \"cp w1,%2\;\"
                           \"addc %0,#0,%0\n\" 
                           \".OV%=:\;\"
                           \"mov.d [--w15],w0\;\"
                           \"btsc SR,#2\;\"
                           \"mov #0x7FFF,%0\";
                  }
         default: gcc_assert(0);
       }
     } else if (pic30_fp_round_p() == pic30_convergent) {
       switch (which_alternative) {
         case 0:  /*
                  ** wm/wn -> w0
                  */
                  if (pic30_dead_or_set_p(insn, w1)) {
                                                     /* convergent follows this
                                                        form */
                    return 
                           \"repeat #__TARGET_DIVIDE_CYCLES\;\"
                           \"divf %1,%2\;\"
                           \"bra ov,.OV%=\;\"
                           \"sl w1,w1\;\"
                           \"cp w1,%2\;\"            /* cp divisor w/ 2*rem */
                           \"btsc SR,#1\;\"         /* if zero ... */
                           \"btst.c w0,#0\;\"        /*   copy low bit to c */
                                                     /*   ie if low bit == 0 */
                                                     /*      don't add */
                           \"addc.w w0,#0,%0\n\"     /* add carry */
                           \".OV%=:\;\"
                           \"btsc SR,#2\;\"
                           \"mov #0x7FFF,%0\";
                  } else {
                      return \"mov w1,[w15++]\;\"
                             \"repeat #__TARGET_DIVIDE_CYCLES\;\"
                             \"divf %1,%2\;\"
                             \"bra ov,.OV%=\;\"
                             \"sl w1,w1\;\"
                             \"cp w1,%2\;\"
                             \"btsc SR,#1\;\"
                             \"btst.c w0,#0\;\"
                             \"addc.w w0,#0,%0\n\" 
                             \".OV%=:\;\"
                             \"mov [--w15],w1\;\"
                             \"btsc SR,#2\;\"
                             \"mov #0x7FFF,%0\";
                  }
         case 1:  /*
                  ** wm/wn -> w1
                  */
                  if (pic30_dead_or_set_p(insn, w0)) {
                    return 
                           \"repeat #__TARGET_DIVIDE_CYCLES\;\"
                           \"divf %1,%2\;\"
                           \"bra ov,.OV%=\;\"
                           \"sl w1,w1\;\"
                           \"cp w1,%2\;\"
                           \"btsc SR,#1\;\"
                           \"btst.c w0,#0\;\"
                           \"addc.w w0,#0,%0\n\"
                           \".OV%=:\;\"
                           \"btsc SR,#2\;\"
                           \"mov #0x7FFF,%0\";
                  } else {
                    return \"mov w0,[w15++]\;\"
                           \"repeat #__TARGET_DIVIDE_CYCLES\;\"
                           \"divf %1,%2\;\"
                           \"bra ov,.OV%=\;\"
                           \"sl w1,w1\;\"
                           \"cp w1,%2\;\"
                           \"btsc SR,#1\;\"
                           \"btst.c w0,#0\;\"
                           \"addc.w w0,#0,%0\n\" 
                           \".OV%=:\;\"
                           \"mov [--w15],w0\;\"
                           \"btsc SR,#2\;\"
                           \"mov #0x7FFF,%0\";
                  }
         case 2:  /*
                  ** wm/wn -> we
                  */
                  if (pic30_dead_or_set_p(insn, w0) &&
                      pic30_dead_or_set_p(insn, w1)) {
                    return \"repeat #__TARGET_DIVIDE_CYCLES\;\"
                           \"divf %1,%2\;\"
                           \"bra ov,.OV%=\;\"
                           \"sl w1,w1\;\"
                           \"cp w1,%2\;\"
                           \"btsc SR,#1\;\"
                           \"btst.c w0,#0\;\"
                           \"addc.w w0,#0,%0\n\"
                           \".OV%=:\;\"
                           \"btsc SR,#2\;\"
                           \"mov #0x7FFF,%0\";
                  } else {
                    return \"mov.d w0,[w15++]\;\"
                           \"repeat #__TARGET_DIVIDE_CYCLES\;\"
                           \"divf %1,%2\;\"
                           \"bra ov,.OV%=\;\"
                           \"sl w1,w1\;\"
                           \"cp w1,%2\;\"
                           \"btsc SR,#1\;\"
                           \"btst.c w0,#0\;\"
                           \"addc.w w0,#0,%0\n\" 
                           \".OV%=:\;\"
                           \"mov.d [--w15],w0\;\"
                           \"btsc SR,#2\;\"
                           \"mov #0x7FFF,%0\";
                  }
         default: gcc_assert(0);
       }
     }
     gcc_assert(0);
     return \"nop\";
  } 
  "
  [
    (set_attr "cc" "clobber")
    (set_attr "type" "def")
  ]
)

(define_insn "ssatdiv<mode>3_helper_32"
  [(set (match_operand:Q15    0 "pic30_register_operand"  "=a,?b,??&e")
        (ss_div:Q15 
          (match_operand:Q15  1 "pic30_register_operand"   "r, r,  r")
          (match_operand:Q15  2 "pic30_ereg_operand"       "e, e,  e")))
   (match_operand:HI          3 "pic30_register_operand"   "e, e,  e")
   (clobber (match_scratch:HI 4                          "=&e,&e, &e"))
   (clobber (reg:HI RCOUNT))
  ]
  ""
  "*
  {
     rtx w0 = gen_rtx_REG(HImode, WR0_REGNO);
     rtx w1 = gen_rtx_REG(HImode, WR1_REGNO);
     if ((pic30_fp_round_p() == pic30_truncation) ||
         (pic30_fp_round_p() == pic30_fastest)) {
       switch (which_alternative) {
         case 0:  /*
                ** wm/wn -> w0
                */
                  if (pic30_dead_or_set_p(insn, w1)) {
                    return \"repeat #__TARGET_DIVIDE_CYCLES\;\"
                           \"divf %1,%2\;\"
                           \"bra nov,.NOV%=\;\"
                           \"mov #0x7FFF,%0\;\"
                           \"setm w1\n\"                /* force C == 0 */
                           \".NOV%=:\;\"
                           \"ff1r w1,%4\;\"             /* C = w1 == 0 */
                           \"com w0,%4\;\"              /* complement in case
                                                            of negate */
                           \"btsc %3,#15\;\"            /* if negative, neg */
                           \"addc %4,#0,w0\";
                  } else {
                      return \"mov w1,[w15++]\;\"
                             \"repeat #__TARGET_DIVIDE_CYCLES\;\"
                             \"divf %1,%2\;\"
                             \"bra nov,.NOV%=\;\"
                             \"mov #0x7FFF,%0\;\"
                             \"setm w1\n\"              /* force C == 0 */
                             \".NOV%=:\;\"
                             \"ff1r w1,%4\;\"           /* C = w1 == 0 */
                             \"com w0,%4\;\"            /* complement in case
                                                            of negate */
                             \"btsc %3,#15\;\"          /* if negative, neg */
                             \"addc %4,#0,w0\;\"
                             \"mov [--w15],w1\";
                }
         case 1:  /*
                  ** wm/wn -> w1
                  */
                  if (pic30_dead_or_set_p(insn, w0)) {
                    return \"repeat #__TARGET_DIVIDE_CYCLES\;\"
                           \"divf %1,%2\;\"
                           \"bra nov,.NOV%=\;\"
                           \"mov #0x7FFF,%0\;\"
                           \"setm w1\n\"                /* force C == 0 */
                           \".NOV%=:\;\"
                           \"ff1r w1,%4\;\"             /* C = w1 == 0 */
                           \"com w0,%4\;\"              /* complement in case
                                                            of negate */
                           \"btsc %3,#15\;\"            /* if negative, neg */
                           \"addc %4,#0,w0\;\"
                           \"mov  w0,%0\";

                  } else {
                    return \"mov w0,[w15++]\;\"
                           \"repeat #__TARGET_DIVIDE_CYCLES\;\"
                           \"divf %1,%2\;\"
                           \"bra nov,.NOV%=\;\"
                           \"mov #0x7FFF,%0\;\"
                           \"setm w1\n\"                /* force C == 0 */
                           \".NOV%=:\;\"
                           \"ff1r w1,%4\;\"             /* C = w1 == 0 */
                           \"com w0,%4\;\"              /* complement in case
                                                            of negate */
                           \"btsc %3,#15\;\"            /* if negative, neg */
                           \"addc %4,#0,w0\;\"
                           \"mov w0,%0\;\"
                           \"mov [--w15],w0\";
                  } 
         case 2:  /*
                  ** wm/wn -> we
                  */
                  if (pic30_dead_or_set_p(insn, w0) &&
                      pic30_dead_or_set_p(insn, w1)) {
                    return \"repeat #__TARGET_DIVIDE_CYCLES\;\"
                           \"divf %1,%2\;\"
                           \"bra nov,.NOV%=\;\"
                           \"mov #0x7FFF,%0\"
                           \"setm w1\n\"                /* force C == 0 */
                           \".NOV%=:\;\"
                           \"ff1r w1,%4\;\"             /* C = w1 == 0 */
                           \"com w0,%4\;\"              /* complement in case
                                                            of negate */
                           \"btsc %3,#15\;\"            /* if negative, neg */
                           \"addc %4,#0,w0\;\"
                           \"mov w0,%0\";
                  } else {
                    return \"mov.d w0,[w15++]\;\"
                           \"repeat #__TARGET_DIVIDE_CYCLES\;\"
                           \"divf %1,%2\;\"
                           \"bra nov,.NOV%=\;\"
                           \"mov #0x7FFF,%0\;\"
                           \"setm w1\n\"                /* force C == 0 */
                           \".NOV%=:\;\"
                           \"ff1r w1,%4\;\"             /* C = w1 == 0 */
                           \"com w0,%4\;\"              /* complement in case
                                                            of negate */
                           \"btsc %3,#15\;\"            /* if negative, neg */
                           \"addc %4,#0,w0\;\"
                           \"mov w0,%0\;\"
                           \"mov.d [--w15],w0\";
                  }
         default: gcc_assert(0);
       }
     } else if (pic30_fp_round_p() == pic30_conventional) {
       switch (which_alternative) {
         case 0:  /*
                ** wm/wn -> w0
                */
                  if (pic30_dead_or_set_p(insn, w1)) {
                                                     /* all follow sim. flow */
                    return 
                           \"repeat #__TARGET_DIVIDE_CYCLES\;\"
                           \"divf %1,%2\;\"
                           \"bra ov,.OV%=\;\"
                           \"sl w1,%4\;\"
                           \"sub %4,%2,%4\;\"        /* compare 2*remainder */
                           \"btss SR,#0\;\"         /* carry set, use new val*/
                           \"mov %4,w1\;\"
                           \"rlc w0,w0\;\"           /* put R in low bit */

                           \"ff1r w1,%4\;\"          /* C = w1 == 0 */
                           \"com w0,%4\;\"           /* complement in case
                                                        of negate */
                           \"btsc %3,#15\;\"         /* if negative, neg */
                           \"addc %4,#0,w0\;\"
                           \"rlc %3,[w15]\;\"        /* get sign in carry */
                           \"rrc w0,w0\;\"           /* div by 2 again */
                           \"addc w0,#0,%0\;\"       /* add in round */
                           \"bra nov,.NOV%=\n\"
                           \".OV%=:\;\"
                           \"mov #0x7FFF,%0\;\"
                           \"btsc %3,#15\;\"
                           \"com %0,%0\n\"
                           \".NOV%=:\";
                  } else {
                      return \"mov w1,[w15++]\;\"
                             \"repeat #__TARGET_DIVIDE_CYCLES\;\"
                             \"divf %1,%2\;\"
                             \"bra ov,.OV%=\;\"
                             \"sl w1,%4\;\"
                             \"sub %4,%2,%4\;\"      /* compare 2*remainder */
                             \"btss SR,#0\;\"       /* carry set, use new val*/
                             \"mov %4,w1\;\"
                             \"rlc w0,w0\;\"         /* put R in low bit */
                             \"ff1r w1,%4\;\"        /* C = w1 == 0 */ 
                             \"com w0,%4\;\"         /* complement in case
                                                        of negate */
                             \"btsc %3,#15\;\"       /* if negative, neg */ 
                             \"addc %4,#0,w0\;\"
                             \"rlc %3,[w15]\;\"        /* get sign in carry */
                             \"rrc w0,w0\;\"           /* div by 2 again */
                             \"addc w0,#0,%0\;\"       /* add in round */
                             \"bra nov,.NOV%=\n\"
                             \".OV%=:\;\"
                             \"mov #0x7FFF,%0\;\"
                             \"btsc %3,#15\;\"
                             \"com %0,%0\n\"
                             \".NOV%=:\;\"
                             \"mov [--w15],w1\";
                }
         case 1:  /*
                  ** wm/wn -> w1
                  */
                  if (pic30_dead_or_set_p(insn, w0)) {
                    return 
                           \"repeat #__TARGET_DIVIDE_CYCLES\;\"
                           \"divf %1,%2\;\"
                           \"bra ov,.OV%=\;\"
                           \"sl w1,%4\;\"
                           \"sub %4,%2,%4\;\"        /* compare 2*remainder */
                           \"btss SR,#0\;\"         /* carry set, use new val*/
                           \"mov %4,w1\;\"
                           \"rlc w0,w0\;\"           /* put R in low bit */
                           \"ff1r w1,%4\;\"          /* C = w1 == 0 */ 
                           \"com w0,%4\;\"           /* complement in case
                                                        of negate */
                           \"btsc %3,#15\;\"         /* if negative, neg */ 
                           \"addc %4,#0,w0\;\"
                           \"rlc %3,[w15]\;\"        /* get sign in carry */
                           \"rrc w0,w0\;\"           /* div by 2 again */
                           \"addc w0,#0,%0\;\"       /* add in round */
                           \"bra nov,.NOV%=\n\"
                           \".OV%=:\;\"
                           \"mov #0x7FFF,%0\;\"
                           \"btsc %3,#15\;\"
                           \"com %0,%0\n\"
                           \".NOV%=:\";
                  } else {
                    return \"mov w0,[w15++]\;\"
                           \"repeat #__TARGET_DIVIDE_CYCLES\;\"
                           \"divf %1,%2\;\"
                           \"bra ov,.OV%=\;\"
                           \"sl w1,%4\;\"
                           \"sub %4,%2,%4\;\"        /* compare 2*remainder */
                           \"btss SR,#0\;\"         /* carry set, use new val*/
                           \"mov %4,w1\;\"
                           \"rlc w0,w0\;\"           /* put R in low bit */
                           \"ff1r w1,%4\;\"          /* C = w1 == 0 */ 
                           \"com w0,%4\;\"           /* complement in case
                                                        of negate */
                           \"btsc %3,#15\;\"         /* if negative, neg */ 
                           \"addc %4,#0,w0\;\"
                           \"rlc %3,[w15]\;\"        /* get sign in carry */
                           \"rrc w0,w0\;\"           /* div by 2 again */
                           \"addc w0,#0,%0\;\"       /* add in round */
                           \"bra nov,.NOV%=\n\"
                           \".OV%=:\;\"
                           \"mov #0x7FFF,%0\;\"
                           \"btsc %3,#15\;\"
                           \"com %0,%0\n\"
                           \".NOV%=:\;\"
                           \"mov [--w15],w0\";
                  }
         case 2:  /*
                  ** wm/wn -> we
                  */
                  if (pic30_dead_or_set_p(insn, w0) &&
                      pic30_dead_or_set_p(insn, w1)) {
                    return 
                           \"repeat #__TARGET_DIVIDE_CYCLES\;\"
                           \"divf %1,%2\;\"
                           \"bra ov,.OV%=\;\"
                           \"sl w1,%4\;\"
                           \"sub %4,%2,%4\;\"        /* compare 2*remainder */
                           \"btss SR,#0\;\"         /* carry set, use new val*/
                           \"mov %4,w1\;\"
                           \"rlc w0,w0\;\"           /* put R in low bit */
                           \"ff1r w1,%4\;\"          /* C = w1 == 0 */ 
                           \"com w0,%4\;\"           /* complement in case
                                                        of negate */
                           \"btsc %3,#15\;\"         /* if negative, neg */ 
                           \"addc %4,#0,w0\;\"
                           \"rlc %3,[w15]\;\"        /* get sign in carry */
                           \"rrc w0,w0\;\"           /* div by 2 again */
                           \"addc w0,#0,%0\;\"       /* add in round */
                           \"bra nov,.NOV%=\n\"
                           \".OV%=:\;\"
                           \"mov #0x7FFF,%0\;\"
                           \"btsc %3,#15\;\"
                           \"com %0,%0\n\"
                           \".NOV%=:\";
                  } else {
                    return \"mov.d w0,[w15++]\;\"
                           \"repeat #__TARGET_DIVIDE_CYCLES\;\"
                           \"divf %1,%2\;\"
                           \"bra ov,.OV%=\;\"
                           \"sl w1,%4\;\"
                           \"sub %4,%2,%4\;\"        /* compare 2*remainder */
                           \"btss SR,#0\;\"         /* carry set, use new val*/
                           \"mov %4,w1\;\"
                           \"rlc w0,w0\;\"           /* put R in low bit */
                           \"ff1r w1,%4\;\"          /* C = w1 == 0 */ 
                           \"com w0,%4\;\"           /* complement in case
                                                        of negate */
                           \"btsc %3,#15\;\"         /* if negative, neg */ 
                           \"addc %4,#0,w0\;\"
                           \"rlc %3,[w15]\;\"        /* get sign in carry */
                           \"rrc w0,w0\;\"           /* div by 2 again */
                           \"addc w0,#0,%0\;\"       /* add in round */
                           \"bra nov,.NOV%=\n\"
                           \".OV%=:\;\"
                           \"mov #0x7FFF,%0\;\"
                           \"btsc %3,#15\;\"
                           \"com %0,%0\n\"
                           \".NOV%=:\;\"
                           \"mov.d [--w15],w0\";
                  }
         default: gcc_assert(0);
       }
     } else if (pic30_fp_round_p() == pic30_convergent) {
       switch (which_alternative) {
         case 0:  /*
                  ** wm/wn -> w0
                  */
                  if (pic30_dead_or_set_p(insn, w1)) {
                                                     /* convergent follows this
                                                        form */
                    return 
                           \"repeat #__TARGET_DIVIDE_CYCLES\;\"
                           \"divf %1,%2\;\"
                           \"bra ov,.OV%=\;\"
                           \"sl w1,%4\;\"
                           \"sub %4,%2,%4\;\"        /* compare 2*remainder */
                           \"btss SR,#0\;\"         /* carry set, use new val*/
                           \"mov %4,w1\;\"
                           \"btsc SR,#1\;\"
                           \"btst.c w0,#0\;\"        /*   copy low bit to c */
                                                     /*   ie if low bit == 0 */
                                                     /*      don't add */
                           \"rlc w0,w0\;\"           /* put R in low bit */
                           \"ff1r w1,%4\;\"          /* C = w1 == 0 */ 
                           \"com w0,%4\;\"           /* complement in case
                                                        of negate */
                           \"btsc %3,#15\;\"         /* if negative, neg */ 
                           \"addc %4,#0,w0\;\"
                           \"rlc %3,[w15]\;\"        /* get sign in carry */
                           \"rrc w0,w0\;\"           /* div by 2 again */
                           \"addc.w w0,#0,%0\;\"     /* add carry */
                           \"bra nov,.NOV%=\n\"
                           \".OV%=:\;\"
                           \"mov #0x7FFF,%0\;\"
                           \"btsc %3,#15\;\"
                           \"com %0,%0\n\"
                           \".NOV%=:\";
                  } else {
                      return \"mov w1,[w15++]\;\"
                             \"repeat #__TARGET_DIVIDE_CYCLES\;\"
                             \"divf %1,%2\;\"
                             \"bra ov,.OV%=\;\"
                             \"sl w1,%4\;\"
                             \"sub %4,%2,%4\;\"      /* compare 2*remainder */
                             \"btss SR,#0\;\"       /* carry set, use new val*/
                             \"mov %4,w1\;\"
                             \"btsc SR,#1\;\"
                             \"btst.c w0,#0\;\"      /*   copy low bit to c */
                                                     /*   ie if low bit == 0 */
                                                     /*      don't add */
                             \"rlc w0,w0\;\"         /* put R in low bit */
                             \"ff1r w1,%4\;\"        /* C = w1 == 0 */ 
                             \"com w0,%4\;\"         /* complement in case
                                                        of negate */
                             \"btsc %3,#15\;\"       /* if negative, neg */ 
                             \"addc %4,#0,w0\;\"
                             \"rlc %3,[w15]\;\"      /* get sign in carry */
                             \"rrc w0,w0\;\"         /* div by 2 again */
                             \"addc.w w0,#0,%0\;\" 
                             \"bra nov,.NOV%=\n\"
                             \".OV%=:\;\"
                             \"mov #0x7FFF,%0\;\"
                             \"btsc %3,#15\;\"
                             \"com %0,%0\n\"
                             \".NOV%=:\;\"
                             \"mov [--w15],w1\";
                  }
         case 1:  /*
                  ** wm/wn -> w1
                  */
                  if (pic30_dead_or_set_p(insn, w0)) {
                    return 
                           \"repeat #__TARGET_DIVIDE_CYCLES\;\"
                           \"divf %1,%2\;\"
                           \"bra ov,.OV%=\;\"
                           \"sl w1,%4\;\"
                           \"sub %4,%2,%4\;\"        /* compare 2*remainder */
                           \"btss SR,#0\;\"         /* carry set, use new val*/
                           \"mov %4,w1\;\"
                           \"btsc SR,#1\;\"
                           \"btst.c w0,#0\;\"        /*   copy low bit to c */
                                                     /*   ie if low bit == 0 */
                                                     /*      don't add */
                           \"rlc w0,w0\;\"           /* put R in low bit */
                           \"ff1r w1,%4\;\"          /* C = w1 == 0 */ 
                           \"com w0,%4\;\"           /* complement in case
                                                        of negate */
                           \"btsc %3,#15\;\"         /* if negative, neg */ 
                           \"addc %4,#0,w0\;\"
                           \"rlc %3,[w15]\;\"        /* get sign in carry */
                           \"rrc w0,w0\;\"           /* div by 2 again */
                           \"addc.w w0,#0,%0\;\"
                           \"bra nov,.NOV%=\n\"
                           \".OV%=:\;\"
                           \"mov #0x7FFF,%0\;\"
                           \"btsc %3,#15\;\"
                           \"com %0,%0\n\"
                           \".NOV%=:\";
                  } else {
                    return \"mov w0,[w15++]\;\"
                           \"repeat #__TARGET_DIVIDE_CYCLES\;\"
                           \"divf %1,%2\;\"
                           \"bra ov,.OV%=\;\"
                           \"sl w1,%4\;\"
                           \"sub %4,%2,%4\;\"        /* compare 2*remainder */
                           \"btss SR,#0\;\"         /* carry set, use new val*/
                           \"mov %4,w1\;\"
                           \"btsc SR,#1\;\"
                           \"btst.c w0,#0\;\"        /*   copy low bit to c */
                                                     /*   ie if low bit == 0 */
                                                     /*      don't add */
                           \"rlc w0,w0\;\"           /* put R in low bit */
                           \"ff1r w1,%4\;\"          /* C = w1 == 0 */ 
                           \"com w0,%4\;\"           /* complement in case
                                                        of negate */
                           \"btsc %3,#15\;\"         /* if negative, neg */ 
                           \"addc %4,#0,w0\;\"
                           \"rlc %3,[w15]\;\"        /* get sign in carry */
                           \"rrc w0,w0\;\"           /* div by 2 again */
                           \"addc.w w0,#0,%0\;\" 
                           \"bra nov,.NOV%=\n\"
                           \".OV%=:\;\"
                           \"mov #0x7FFF,%0\;\"
                           \"btsc %3,#15\;\"
                           \"com %0,%0\n\"
                           \".NOV%=:\;\"
                           \"mov [--w15],w0\";
                  }
         case 2:  /*
                  ** wm/wn -> we
                  */
                  if (pic30_dead_or_set_p(insn, w0) &&
                      pic30_dead_or_set_p(insn, w1)) {
                    return \"repeat #__TARGET_DIVIDE_CYCLES\;\"
                           \"divf %1,%2\;\"
                           \"bra ov,.OV%=\;\"
                           \"sl w1,%4\;\"
                           \"sub %4,%2,%4\;\"        /* compare 2*remainder */
                           \"btss SR,#0\;\"         /* carry set, use new val*/
                           \"mov %4,w1\;\"
                           \"btsc SR,#1\;\"
                           \"btst.c w0,#0\;\"        /*   copy low bit to c */
                                                     /*   ie if low bit == 0 */
                                                     /*      don't add */
                           \"rlc w0,w0\;\"           /* put R in low bit */
                           \"ff1r w1,%4\;\"          /* C = w1 == 0 */ 
                           \"com w0,%4\;\"           /* complement in case
                                                        of negate */
                           \"btsc %3,#15\;\"         /* if negative, neg */ 
                           \"addc %4,#0,w0\;\"
                           \"rlc %3,[w15]\;\"        /* get sign in carry */
                           \"rrc w0,w0\;\"           /* div by 2 again */
                           \"addc.w w0,#0,%0\;\"
                           \"bra nov,.NOV%=\n\"
                           \".OV%=:\;\"
                           \"mov #0x7FFF,%0\;\"
                           \"btsc %3,#15\;\"
                           \"com %0,%0\n\"
                           \".NOV%=:\";
                  } else {
                    return \"mov.d w0,[w15++]\;\"
                           \"repeat #__TARGET_DIVIDE_CYCLES\;\"
                           \"divf %1,%2\;\"
                           \"sl w1,%4\;\"
                           \"sub %4,%2,%4\;\"        /* compare 2*remainder */
                           \"btss SR,#0\;\"         /* carry set, use new val*/
                           \"mov %4,w1\;\"
                           \"btsc SR,#1\;\"
                           \"btst.c w0,#0\;\"        /*   copy low bit to c */
                                                     /*   ie if low bit == 0 */
                                                     /*      don't add */
                           \"rlc w0,w0\;\"           /* put R in low bit */
                           \"bra ov,.OV%=\;\"
                           \"ff1r w1,%4\;\"          /* C = w1 == 0 */ 
                           \"com w0,%4\;\"           /* complement in case
                                                        of negate */
                           \"btsc %3,#15\;\"         /* if negative, neg */ 
                           \"addc %4,#0,w0\;\"
                           \"rlc %3,[w15]\;\"        /* get sign in carry */
                           \"rrc w0,w0\;\"           /* div by 2 again */
                           \"addc.w w0,#0,%0\;\" 
                           \"bra nov,.NOV%=\n\"
                           \".OV%=:\;\"
                           \"mov #0x7FFF,%0\;\"
                           \"btsc %3,#15\;\"
                           \"com %0,%0\n\"
                           \".NOV%=:\;\"
                           \"mov.d [--w15],w0\";
                  }
         default: gcc_assert(0);
       }
     }
     gcc_assert(0);
     return \"nop\";
  } 
  "
  [
    (set_attr "cc" "clobber")
    (set_attr "type" "def")
  ]
)

(define_expand "udiv<mode>3_32"
  [(set (match_operand:UQ15 0 "pic30_register_operand"    "=a,?b,??e")
        (udiv:UQ15
          (match_operand:UQ15 1 "pic30_register_operand"   "r, r,  r")
          (match_operand:UQ15 2 "pic30_ereg_operand"       "e, e,  e")))]
  ""
  "
  {  rtx ra,rb;

     /* divf is not valid on 24F targets */
     if (!pic30_dsp_target()) FAIL;
     ra = operands[1];
     rb = operands[2];
     if (!pic30_register_operand(ra,<MODE>mode))
       ra = force_reg(<MODE>mode, ra);
     if (!pic30_register_operand(rb,<MODE>mode))
       rb = force_reg(<MODE>mode, rb);
     emit(
       gen_udiv<mode>3_helper_32(operands[0],ra,rb)
     );
     DONE;
  }"
)

(define_expand "div<mode>3_32"
  [(set (match_operand:Q15 0 "pic30_register_operand"    "=a,?b,??e")
        (div:Q15
          (match_operand:Q15 1 "pic30_register_operand"   "r, r,  r")
          (match_operand:Q15 2 "pic30_ereg_operand"       "e, e,  e")))]
  ""
  "
  {  rtx ra,rb;
     rtx ura,urb;
     rtx sign, hi1,hi2;

     /* divf is not valid on 24F targets */
     if (!pic30_dsp_target()) FAIL;
     ra = operands[1];
     rb = operands[2];
     if (!pic30_register_operand(ra,<MODE>mode))
       ra = force_reg(<MODE>mode, ra);
     if (!pic30_register_operand(rb,<MODE>mode))
       rb = force_reg(<MODE>mode, rb);
     sign = gen_reg_rtx(HImode);
     hi1 = gen_rtx_SUBREG(HImode, ra, 0);
     hi2 = gen_rtx_SUBREG(HImode, rb, 0);
     emit(
       gen_xorhi3(sign,hi1,hi2)
     );
     ura = gen_reg_rtx(<MODE>mode);
     urb = gen_reg_rtx(<MODE>mode);
     emit(
       gen_abs<mode>2_32(ura,ra)
     );
     emit(
       gen_abs<mode>2_32(urb,rb)
     );
     emit(
       gen_div<mode>3_helper_32(operands[0],ura,urb,sign)
     );
     DONE;
  }"
)

(define_insn "sssaturate<mode>_32"
  [(set (match_operand:QUQ15    0 "pic30_register_operand" "=&r")
        (unspec:QUQ15 [
           (match_operand:QUQ15 1 "pic30_register_operand" "0")
           (match_operand:HI    2 "pic30_register_operand" "r")
         ] UNSPEC_SATURATE)
   ) 
   (clobber (match_scratch:HI 3                        "=&r"))
  ]
  ""
  "*
   return \"xor %1,%2,%3\;\"    /* test result sign with expected */
          \"btss %3,#15\;\"
          \"bra .SAT%=\;\"        /* if they are the same we are done */
          \"mov #0x8000,%0\;\"  /* otherwise saturate to the expected sign */
          \".SAT%=:\";
  "
)

(define_expand "usdiv<mode>3_32"
  [(set (match_operand:UQ15 0 "pic30_register_operand"    "=a,?b,??e")
        (us_div:UQ15
          (match_operand:UQ15 1 "pic30_register_operand"   "r, r,  r")
          (match_operand:UQ15 2 "pic30_ereg_operand"       "e, e,  e")))]
  ""
  "
  {  rtx ra,rb;

     /* divf is not valid on 24F targets */
     if (!pic30_dsp_target()) FAIL;
     ra = operands[1];
     rb = operands[2];
     if (!pic30_register_operand(ra,<MODE>mode))
       ra = force_reg(<MODE>mode, ra);
     if (!pic30_register_operand(rb,<MODE>mode))
       rb = force_reg(<MODE>mode, rb);
     emit(
       gen_usatdiv<mode>3_helper_32(operands[0],ra,rb)
     );
     DONE;
  }"
)

(define_expand "ssdiv<mode>3_32"
  [(set (match_operand:Q15    0 "pic30_register_operand"  "=a,?b,??e")
        (ss_div:Q15
          (match_operand:Q15  1 "pic30_register_operand"   "r, r,  r")
          (match_operand:Q15  2 "pic30_ereg_operand"       "e, e,  e")))
   (clobber (match_scratch:HI 3                           "=r, r,  r"))
  ]
  ""
  "
  {  rtx ra,rb;
     rtx ura,urb;
     rtx sign,hi1,hi2;

     /* divf is not valid on 24F targets */
     if (!pic30_dsp_target()) FAIL;
     ra = operands[1];
     rb = operands[2];
     if (!pic30_register_operand(ra,<MODE>mode))
       ra = force_reg(<MODE>mode, ra);
     if (!pic30_register_operand(rb,<MODE>mode))
       rb = force_reg(<MODE>mode, rb);
     hi1 = gen_rtx_SUBREG(HImode, ra, 0);
     hi2 = gen_rtx_SUBREG(HImode, rb, 0);
     sign = gen_reg_rtx(HImode);
     emit(
       gen_xorhi3_32(sign,hi1, hi2)
     );
     ura = gen_reg_rtx(<MODE>mode);
     urb = gen_reg_rtx(<MODE>mode);
     emit(
       gen_abs<mode>2_32(ura,ra)
     );
     emit(
       gen_abs<mode>2_32(urb,rb)
     );
     emit(
       gen_ssatdiv<mode>3_helper_32(operands[0],ura,urb,sign)
     );
     DONE;
  }"
)

;; *** multiply
;  32-bit
; handled by helper routines in libfx

;; *** conversions
; 16-bit

;  [sat]fract[uns]mn2
;
;    These patterns handle modes m,n where m,n are any combination of
;      fixed-point, floating-point, [un]signed integer
;  
;    One of the modes is fixed.
;

; integer (IMODE) modes
;  from Q15

(define_insn "fract<mode>qi2_helper_32"
  [(set (match_operand:QI    0 "pic30_register_operand"  "=&r")
        (fract_convert:QI
          (match_operand:QUQ15 1 "pic30_register_operand"  "r")))]
  ""
  "*
  {
     gcc_assert(GET_MODE_FBIT(<MODE>mode) == 15);
     if ((pic30_fp_round_p() == pic30_truncation) ||
         (pic30_fp_round_p() == pic30_fastest)) {
       /* chop the low order bits, signed results are either 0 or -1 */
       return \"asr %1,#15,%0\"; 
     } else if (pic30_fp_round_p() == pic30_conventional) {
       /* [-1,-.5] round to -1, (-.5,.5) round to 0, otherwise 1? */
       return \"asr %1,#14,%0\;btss %0,#0\;clr.b %0\";
     } else {
       error(\"Invalid fixed-point rounding mode specified\");
     }
     gcc_assert(0);
     return \"nop\";

  }"
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "fract<mode>qi2_convergent_32"
  [(set (match_operand:QI      0 "pic30_register_operand" "=&r")
        (fract_convert:QI
          (match_operand:QUQ15   1 "pic30_register_operand"   "r")))
   (clobber (match_scratch:QUQ15 2                           "=r"))
  ]
  ""
  "*
  {
     gcc_assert(GET_MODE_FBIT(<MODE>mode) == 15);
     if (pic30_fp_round_p() == pic30_convergent) {
       return \"asr %1,#14,%0\;\"  /* as conventional */
              \"btss %0,#0\;\"
              \"clr.b %0\;\"
              \"sl %1,#2,%2\;\"    /* x.yz13 where z13 == 0 */
              \"btsc SR,#1\;\"    /*   always rounds to even 0 */
              \"clr.b %0\";
     } else {
       error(\"Invalid fixed-point rounding mode specified\");
     }
     gcc_assert(0);
     return \"nop\";

  }"
  [
    (set_attr "cc" "clobber")
  ]
)

(define_expand "fract<mode>qi2_32"
  [(set (match_operand:QI    0 "pic30_register_operand" "=r")
        (fract_convert:QI
          (match_operand:QUQ15 1 "pic30_register_operand"  "r")))]
  ""
  "
  { rtx r0,r1;

    if (!pic30_fp_inline_p() && (pic30_fp_round_p() == pic30_convergent))
      FAIL;
    r0 = operands[0];
    r1 = operands[1];
    if (!pic30_register_operand(r0,QImode)) {
      r0 = gen_reg_rtx(QImode);
    }
    if (!pic30_register_operand(r1,VOIDmode)) {
      r1 = force_reg(<MODE>mode, r1);
    }
    if (pic30_fp_round_p() == pic30_convergent) {
      emit(
        gen_fract<mode>qi2_convergent_32(r0,r1)
      );
    } else {
      emit(
        gen_fract<mode>qi2_helper_32(r0,r1)
      );
    }
    if (r0 != operands[0]) {
      emit_move_insn(operands[0],r0);
    }
    DONE;
  }
  "
)

(define_insn "fract<mode>hi2_helper_32"
  [(set (match_operand:HI    0 "pic30_register_operand"  "=&r")
        (fract_convert:HI
          (match_operand:QUQ15 1 "pic30_register_operand"  "r")))]
  ""
  "*
  {
     gcc_assert(GET_MODE_FBIT(<MODE>mode) == 15);
     if ((pic30_fp_round_p() == pic30_truncation) ||
         (pic30_fp_round_p() == pic30_fastest)) {
       /* chop the low order bits, signed results are either 0 or -1 */
       return \"asr %1,#15,%0\"; 
     } else if (pic30_fp_round_p() == pic30_conventional) {
       /* [-1,-.5] round to -1, (-.5,.5) round to 0, otherwise 1? */
       return \"asr %1,#14,%0\;btss %0,#0\;clr %0\";
     } else {
       error(\"Invalid fixed-point rounding mode specified\");
     }
     gcc_assert(0);
     return \"nop\";

  }"
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "fract<mode>hi2_convergent_32"
  [(set (match_operand:HI    0 "pic30_register_operand"  "=&r")
        (fract_convert:HI
          (match_operand:QUQ15 1 "pic30_register_operand"  "r")))
   (clobber (match_scratch:QUQ15 2                         "=r"))
  ]
  ""
  "*
  {
     gcc_assert(GET_MODE_FBIT(<MODE>mode) == 15);
     if (pic30_fp_round_p() == pic30_convergent) {
       return \"asr %1,#14,%0\;\"  /* as conventional */
              \"btss %0,#0\;\"
              \"clr %0\;\"
              \"sl %1,#2,%2\;\"    /* x.yz13 where z13 == 0 */
              \"btsc SR,#1\;\"    /*   always rounds to even 0 */
              \"clr %0\";
     } else {
       error(\"Invalid fixed-point rounding mode specified\");
     }
     gcc_assert(0);
     return \"nop\";

  }"
  [
    (set_attr "cc" "clobber")
  ]
)

(define_expand "fract<mode>hi2_32"
  [(set (match_operand:HI    0 "pic30_register_operand"  "=r")
        (fract_convert:HI
          (match_operand:QUQ15 1 "pic30_register_operand"  "r")))]
  ""
  "
  { rtx r0,r1;

    if (!pic30_fp_inline_p() && (pic30_fp_round_p() == pic30_convergent))
      FAIL;
    r0 = operands[0];
    r1 = operands[1];
    if (!pic30_register_operand(r0,HImode)) {
      r0 = gen_reg_rtx(HImode);
    }
    if (!pic30_register_operand(r1,VOIDmode)) {
      r1 = force_reg(<MODE>mode, r1);
    }
    if (pic30_fp_round_p() == pic30_convergent) {
      emit(
        gen_fract<mode>hi2_convergent_32(r0,r1)
      );
    } else {
      emit(
        gen_fract<mode>hi2_helper_32(r0,r1)
      );
    }
    if (r0 != operands[0]) {
      emit_move_insn(operands[0],r0);
    }
    DONE;
  }
  "
)

(define_insn "fract<mode>si2_helper_32"
  [(set (match_operand:SI    0 "pic30_register_operand"  "=&r")
        (fract_convert:SI
          (match_operand:QUQ15 1 "pic30_register_operand"  "r")))]
  ""
  "*
  {
     gcc_assert(GET_MODE_FBIT(<MODE>mode) == 15);
     if ((pic30_fp_round_p() == pic30_truncation) ||
         (pic30_fp_round_p() == pic30_fastest)) {
       /* chop the low order bits, signed results are either 0 or -1 */
       return \"asr %1,#15,%d0\;asr %1,#15,%0\";
     } else if (pic30_fp_round_p() == pic30_conventional) {
       /* [-1,-.5] round to -1, (-.5,.5) round to 0, otherwise 1? */
       return \"asr %1,#14,%0\;btss %0,#0\;clr %0\;asr %0,#15,%d0\";
     } else {
       error(\"Invalid fixed-point rounding mode specified\");
     }
     gcc_assert(0);
     return \"nop\";
  }"
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "fract<mode>si2_convergent_32"
  [(set (match_operand:SI    0 "pic30_register_operand"  "=&r")
        (fract_convert:SI
          (match_operand:QUQ15 1 "pic30_register_operand"  "r")))
   (clobber (match_scratch:QUQ15 2                         "=r"))
  ]
  ""
  "*
  {
     gcc_assert(GET_MODE_FBIT(<MODE>mode) == 15);
     if (pic30_fp_round_p() == pic30_convergent) {
       return \"asr %1,#14,%0\;\" /* as conventional */
              \"btss %0,#0\;\"
              \"clr  %0\;\"
              \"asr %0,#15,%d0\;\"
              \"sl %1,#2,%2\;\"    /* x.yz13 where z13 == 0 */
              \"btsc SR,#1\;\"    /*   always rounds to even 0 */
              \"mul.uu %0,#0,%0\";
     } else {
       error(\"Invalid fixed-point rounding mode specified\");
     }
     gcc_assert(0);
     return \"nop\";

  }"
  [
    (set_attr "cc" "clobber")
  ]
)

(define_expand "fract<mode>si2_32"
  [(set (match_operand:SI    0 "pic30_register_operand"  "=r")
        (fract_convert:SI
          (match_operand:QUQ15 1 "pic30_register_operand"  "r")))]
  ""
  "
  { rtx r0,r1;

    if (!pic30_fp_inline_p() && (pic30_fp_round_p() == pic30_convergent))
      FAIL;
    r0 = operands[0];
    r1 = operands[1];
    if (!pic30_register_operand(r0,SImode)) {
      r0 = gen_reg_rtx(SImode);
    }
    if (!pic30_register_operand(r1,VOIDmode)) {
      r1 = force_reg(<MODE>mode, r1);
    }
    if (pic30_fp_round_p() == pic30_convergent) {
      emit(
        gen_fract<mode>si2_convergent_32(r0,r1)
      );
    } else {
      emit(
        gen_fract<mode>si2_helper_32(r0,r1)
      );
    }
    if (r0 != operands[0]) {
      emit_move_insn(operands[0],r0);
    }
    DONE;
  }
  "
)

(define_insn "fract<mode>di2_helper_32"
  [(set (match_operand:DI    0 "pic30_register_operand"  "=&r")
        (fract_convert:DI
          (match_operand:QUQ15 1 "pic30_register_operand"  "r")))]
  ""
  "*
  {
     gcc_assert(GET_MODE_FBIT(<MODE>mode) == 15);
     if ((pic30_fp_round_p() == pic30_truncation) ||
         (pic30_fp_round_p() == pic30_fastest)) {
       /* chop the low order bits, signed results are either 0 or -1 */
       return \"asr %1,#15,%q0\;asr %1,#15,%t0\;mul.su %t0,#1,%0\";
     } else if (pic30_fp_round_p() == pic30_conventional) {
       /* [-1,-.5] round to -1, (-.5,.5) round to 0, otherwise 1? */
       return \"asr %1,#14,%0\;\"
              \"btss %0,#0\;\"
              \"clr %0\;\"
              \"asr %0,#15,%d0\;\"
              \"mul.su %d0,#1,%t0\";
     } else {
       error(\"Invalid fixed-point rounding mode specified\");
     }
     gcc_assert(0);
     return \"nop\";
  }"
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "fract<mode>di2_convergent_32"
  [(set (match_operand:DI    0 "pic30_register_operand"  "=&r")
        (fract_convert:DI
          (match_operand:QUQ15 1 "pic30_register_operand"  "r")))
   (clobber (match_scratch:QUQ15 2                         "=r"))
  ]
  ""
  "*
  {
     gcc_assert(GET_MODE_FBIT(<MODE>mode) == 15);
     if (pic30_fp_round_p() == pic30_convergent) {
       return \"asr %1,#14,%0\;\"  /* as conventional */
              \"btss %0,#0\;\"
              \"clr  %0\;\"
              \"asr %0,#15,%d0\;\"
              \"sl %1,#2,%2\;\"    /* x.yz13 where z13 == 0 */
              \"btsc SR,#1\;\"    /*   always rounds to even 0 */
              \"mul.uu %0,#0,%0\;\"
              \"mul.su %d0,#1,%t0\";
     } else {
       error(\"Invalid fixed-point rounding mode specified\");
     }
     gcc_assert(0);
     return \"nop\";

  }"
  [
    (set_attr "cc" "clobber")
  ]
)

(define_expand "fract<mode>di2_32"
  [(set (match_operand:DI    0 "pic30_register_operand"  "=r")
        (fract_convert:DI
          (match_operand:QUQ15 1 "pic30_register_operand"  "r")))]
  ""
  "
  { rtx r0,r1;

    if (!pic30_fp_inline_p() && 
        ((pic30_fp_round_p() == pic30_convergent) ||
         (pic30_fp_round_p() == pic30_conventional)))
      FAIL;
    r0 = operands[0];
    r1 = operands[1];
    if (!pic30_register_operand(r0,DImode)) {
      r0 = gen_reg_rtx(DImode);
    }
    if (!pic30_register_operand(r1,VOIDmode)) {
      r1 = force_reg(<MODE>mode, r1);
    }
    if (pic30_fp_round_p() == pic30_convergent) {
      emit(
        gen_fract<mode>di2_convergent_32(r0,r1)
      );
    } else {
      emit(
        gen_fract<mode>di2_helper_32(r0,r1)
      );
    }
    if (r0 != operands[0]) {
      emit_move_insn(operands[0],r0);
    }
    DONE;
  }
  "
)

; from Q31

(define_insn "fract<mode>qi2_helper_32"
  [(set (match_operand:QI    0 "pic30_register_operand"  "=&r")
        (fract_convert:QI
          (match_operand:QUQ31 1 "pic30_register_operand"  "r")))]
  ""
  "*
  {
     gcc_assert(GET_MODE_FBIT(<MODE>mode) == 31);
     if ((pic30_fp_round_p() == pic30_truncation) ||
         (pic30_fp_round_p() == pic30_fastest)) {
       /* chop the low order bits, signed results are either 0 or -1 */
       return \"asr %d1,#15,%0\"; 
     } else if (pic30_fp_round_p() == pic30_conventional) {
       /* [-1,-.5] round to -1, (-.5,.5) round to 0, otherwise 1? */
       return \"asr %d1,#14,%0\;btss %0,#0\;clr.b %0\";
     } else {
       error(\"Invalid fixed-point rounding mode specified\");
     }
     gcc_assert(0);
     return \"nop\";

  }"
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "fract<mode>qi2_convergent_32"
  [(set (match_operand:QI    0 "pic30_register_operand"  "=&r")
        (fract_convert:QI
          (match_operand:QUQ31 1 "pic30_register_operand"  "r")))
   (clobber (match_scratch:QUQ31 2                         "=&r"))
  ]
  ""
  "*
  {
     gcc_assert(GET_MODE_FBIT(<MODE>mode) == 31);
     if (pic30_fp_round_p() == pic30_convergent) {
       return \"asr %d1,#14,%0\;\"  /* as conventional */
              \"btss %0,#0\;\"
              \"clr.b %0\;\"
              \"sl %d1,#2,%2\;\"    /* x.yz13 where z13 == 0 */
              \"btsc SR,#1\;\"     /*   always rounds to even 0 */
              \"cp0 %1\;\"  
              \"btsc SR,#1\;\"
              \"clr %0\";
     } else {
       error(\"Invalid fixed-point rounding mode specified\");
     }
     gcc_assert(0);
     return \"nop\";

  }"
  [
    (set_attr "cc" "clobber")
  ]
)

(define_expand "fract<mode>qi2_32"
  [(set (match_operand:QI    0 "pic30_register_operand"  "=r")
        (fract_convert:QI
          (match_operand:QUQ31 1 "pic30_register_operand"  "r")))]
  ""
  "
  { rtx r0,r1;

    if (!pic30_fp_inline_p() && (pic30_fp_round_p() == pic30_convergent))
      FAIL;
    r0 = operands[0];
    r1 = operands[1];
    if (!pic30_register_operand(r0,QImode)) {
      r0 = gen_reg_rtx(QImode);
    }
    if (!pic30_register_operand(r1,VOIDmode)) {
      r1 = force_reg(<MODE>mode, r1);
    }
    if (pic30_fp_round_p() == pic30_convergent) {
      emit(
        gen_fract<mode>qi2_convergent_32(r0,r1)
      );
    } else {
      emit(
        gen_fract<mode>qi2_helper_32(r0,r1)
      );
    }
    if (r0 != operands[0]) {
      emit_move_insn(operands[0],r0);
    }
    DONE;
  }
  "
)

(define_insn "fract<mode>hi2_helper_32"
  [(set (match_operand:HI    0 "pic30_register_operand"  "=&r")
        (fract_convert:HI
          (match_operand:QUQ31 1 "pic30_register_operand"  "r")))]
  ""
  "*
  {
     gcc_assert(GET_MODE_FBIT(<MODE>mode) == 31);
     if ((pic30_fp_round_p() == pic30_truncation) ||
         (pic30_fp_round_p() == pic30_fastest)) {
       /* chop the low order bits, signed results are either 0 or -1 */
       return \"asr %d1,#15,%0\"; 
     } else if (pic30_fp_round_p() == pic30_conventional) {
       /* [-1,-.5] round to -1, (-.5,.5) round to 0, otherwise 1? */
       return \"asr %d1,#14,%0\;btss %0,#0\;clr %0\";
     } else {
       error(\"Invalid fixed-point rounding mode specified\");
     }
     gcc_assert(0);
     return \"nop\";

  }"
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "fract<mode>hi2_convergent_32"
  [(set (match_operand:HI    0 "pic30_register_operand"  "=&r")
        (fract_convert:HI
          (match_operand:QUQ31 1 "pic30_register_operand"  "r")))
   (clobber (match_scratch:QUQ31 2                         "=&r"))
  ]
  ""
  "*
  {
     gcc_assert(GET_MODE_FBIT(<MODE>mode) == 31);
     if (pic30_fp_round_p() == pic30_convergent) {
       return \"asr %d1,#14,%0\;\"  /* as conventional */
              \"btss %0,#0\;\"
              \"clr %0\;\"
              \"sl %d1,#2,%2\;\"    /* x.yz13 where z13 == 0 */
              \"btsc SR,#1\;\"     /*   always rounds to even 0 */
              \"cp0 %1\;\"  
              \"btsc SR,#1\;\"
              \"clr %0\";
     } else {
       error(\"Invalid fixed-point rounding mode specified\");
     }
     gcc_assert(0);
     return \"nop\";

  }"
  [
    (set_attr "cc" "clobber")
  ]
)

(define_expand "fract<mode>hi2_32"
  [(set (match_operand:HI    0 "pic30_register_operand"  "=r")
        (fract_convert:HI
          (match_operand:QUQ31 1 "pic30_register_operand"  "r")))]
  ""
  "
  { rtx r0,r1;

    if (!pic30_fp_inline_p() && (pic30_fp_round_p() == pic30_convergent))
      FAIL;
    r0 = operands[0];
    r1 = operands[1];
    if (!pic30_register_operand(r0,HImode)) {
      r0 = gen_reg_rtx(HImode);
    }
    if (!pic30_register_operand(r1,VOIDmode)) {
      r1 = force_reg(<MODE>mode, r1);
    }
    if (pic30_fp_round_p() == pic30_convergent) {
      emit(
        gen_fract<mode>hi2_convergent_32(r0,r1)
      );
    } else {
      emit(
        gen_fract<mode>hi2_helper_32(r0,r1)
      );
    }
    if (r0 != operands[0]) {
      emit_move_insn(operands[0],r0);
    }
    DONE;
  }
  "
)

(define_insn "fract<mode>si2_helper_32"
  [(set (match_operand:SI    0 "pic30_register_operand"  "=&r")
        (fract_convert:SI
          (match_operand:QUQ31 1 "pic30_register_operand"  "r")))]
  ""
  "*
  {
     gcc_assert(GET_MODE_FBIT(<MODE>mode) == 31);
     if ((pic30_fp_round_p() == pic30_truncation) ||
         (pic30_fp_round_p() == pic30_fastest)) {
       /* chop the low order bits, signed results are either 0 or -1 */
       return \"asr %d1,#15,%d0\;asr %d1,#15,%0\";
     } else if (pic30_fp_round_p() == pic30_conventional) {
       /* [-1,-.5] round to -1, (-.5,.5) round to 0, otherwise 1? */
       return \"asr %d1,#14,%0\;\"
              \"btss %0,#0\;\"
              \"clr %0\;\"
              \"asr %0,#15,%d0\";
     } else {
       error(\"Invalid fixed-point rounding mode specified\");
     }
     gcc_assert(0);
     return \"nop\";
  }"
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "fract<mode>si2_convergent_32"
  [(set (match_operand:SI    0 "pic30_register_operand"  "=&r")
        (fract_convert:SI
          (match_operand:QUQ31 1 "pic30_register_operand"  "r")))
   (clobber (match_scratch:QUQ31 2                         "=&r"))
  ]
  ""
  "*
  {
     gcc_assert(GET_MODE_FBIT(<MODE>mode) == 31);
     if (pic30_fp_round_p() == pic30_convergent) {
       return \"asr %d1,#14,%0\;\"  /* as conventional */
              \"btss %0,#0\;\"
              \"clr  %0\;\"
              \"asr %0,#15,%d0\;\"
              \"sl %d1,#2,%2\;\"    /* x.yz13 where z13 == 0 */
              \"btsc SR,#1\;\"     /*   always rounds to even 0 */
              \"cp0 %1\;\"
              \"btsc SR,#1\;\"
              \"mul.uu %0,#0,%0\";
     } else {
       error(\"Invalid fixed-point rounding mode specified\");
     }
     gcc_assert(0);
     return \"nop\";

  }"
  [
    (set_attr "cc" "clobber")
  ]
)

(define_expand "fract<mode>si2_32"
  [(set (match_operand:SI    0 "pic30_register_operand"  "=r")
        (fract_convert:SI
          (match_operand:QUQ31 1 "pic30_register_operand"  "r")))]
  ""
  "
  { rtx r0,r1;

    if (!pic30_fp_inline_p() && (pic30_fp_round_p() == pic30_convergent))
      FAIL;
    r0 = operands[0];
    r1 = operands[1];
    if (!pic30_register_operand(r0,SImode)) {
      r0 = gen_reg_rtx(SImode);
    }
    if (!pic30_register_operand(r1,VOIDmode)) {
      r1 = force_reg(<MODE>mode, r1);
    }
    if (pic30_fp_round_p() == pic30_convergent) {
      emit(
        gen_fract<mode>si2_convergent_32(r0,r1)
      );
    } else {
      emit(
        gen_fract<mode>si2_helper_32(r0,r1)
      );
    }
    if (r0 != operands[0]) {
      emit_move_insn(operands[0],r0);
    }
    DONE;
  }
  "
)

(define_insn "fract<mode>di2_helper_32"
  [(set (match_operand:DI    0 "pic30_register_operand"  "=&r")
        (fract_convert:DI
          (match_operand:QUQ31 1 "pic30_register_operand"  "r")))]
  ""
  "*
  {
     gcc_assert(GET_MODE_FBIT(<MODE>mode) == 31);
     if ((pic30_fp_round_p() == pic30_truncation) ||
         (pic30_fp_round_p() == pic30_fastest)) {
       /* chop the low order bits, signed results are either 0 or -1 */
       return \"asr %d1,#15,%q0\;asr %d1,#15,%t0\;mul.su %t0,#1,%0\";
     } else if (pic30_fp_round_p() == pic30_conventional) {
       /* [-1,-.5] round to -1, (-.5,.5) round to 0, otherwise 1? */
       return \"asr %d1,#14,%0\;\"
              \"btss %0,#0\;\"
              \"clr %0\;\"
              \"asr %0,#15,%d0\;\"
              \"mul.su %d0,#1,%t0\";
     } else {
       error(\"Invalid fixed-point rounding mode specified\");
     }
     gcc_assert(0);
     return \"nop\";
  }"
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "fract<mode>di2_convergent_32"
  [(set (match_operand:DI    0 "pic30_register_operand"  "=&r")
        (fract_convert:DI
          (match_operand:QUQ31 1 "pic30_register_operand"  "r")))
   (clobber (match_scratch:QUQ31 2                         "=&r"))
  ]
  ""
  "*
  {
     gcc_assert(GET_MODE_FBIT(<MODE>mode) == 31);
     if (pic30_fp_round_p() == pic30_convergent) {
       return \"asr %d1,#14,%0\;\"  /* as conventional */
              \"btss %0,#0\;\"
              \"clr  %0\;\"
              \"asr %0,#15,%d0\;\"
              \"sl %d1,#2,%2\;\"    /* x.yz13 where z13 == 0 */
              \"btsc SR,#1\;\"     /*   always rounds to even 0 */
              \"cp0 %1\;\"
              \"btsc SR,#1\;\"
              \"mul.uu %0,#0,%0\;\"
              \"mul.su %d0,#1,%t0\";
     } else {
       error(\"Invalid fixed-point rounding mode specified\");
     }
     gcc_assert(0);
     return \"nop\";

  }"
  [
    (set_attr "cc" "clobber")
  ]
)

(define_expand "fract<mode>di2_32"
  [(set (match_operand:DI    0 "pic30_register_operand"  "=r")
        (fract_convert:DI
          (match_operand:QUQ31 1 "pic30_register_operand"  "r")))]
  ""
  "
  { rtx r0,r1;

    if (!pic30_fp_inline_p() && 
         ((pic30_fp_round_p() == pic30_convergent) ||
          (pic30_fp_round_p() == pic30_convergent)))
      FAIL;
    r0 = operands[0];
    r1 = operands[1];
    if (!pic30_register_operand(r0,DImode)) {
      r0 = gen_reg_rtx(DImode);
    }
    if (!pic30_register_operand(r1,VOIDmode)) {
      r1 = force_reg(<MODE>mode, r1);
    }
    if (pic30_fp_round_p() == pic30_convergent) {
      emit(
        gen_fract<mode>di2_convergent_32(r0,r1)
      );
    } else {
      emit(
        gen_fract<mode>di2_helper_32(r0,r1)
      );
    }
    if (r0 != operands[0]) {
      emit_move_insn(operands[0],r0);
    }
    DONE;
  }
  "
)

; from ACC

(define_insn "fract<mode>qi2_helper_32"
  [(set (match_operand:QI    0 "pic30_register_operand"   "=&r,r")
        (fract_convert:QI
          (match_operand:ACC 1 "pic30_accum_or_reg_operand" "r,w")))
  ]
  ""
  "*
  {
     gcc_assert(GET_MODE_FBIT(<MODE>mode) == 31);
     if ((pic30_fp_round_p() == pic30_truncation) ||
         (pic30_fp_round_p() == pic30_fastest)) {
       /* chop the low order bits; remember with 9.31 the lowest order  */
       /*  integer bit is in the 15th bit of 2nd word */
       if (which_alternative == 0)
         return \"sl %d1,[w15]\;\"
                \"rlc %t1,%0\";
       else 
         return \"mov %m1U,%0\;\"
                \"sl %0,%0\;\"
                \"btsc %m1H,#15\;\"
                \"inc %0,%0\";
     } else if (pic30_fp_round_p() == pic30_conventional) {
       /* [-1,-.5] round to -1, (-.5,.5) round to 0, otherwise 1? */
       if (which_alternative == 0)
         return \"sl %d1,[w15]\;\"
                \"rlc %t1,%0\;\"
                \"btst.c %d1,#14\;\"
                \"addc.b %0,#0,%0\";
       else 
         return \"mov %m1U,%0\;\"
                \"sl %0,%0\;\"
                \"btsc %m1H,#15\;\"
                \"inc %0,%0\;\"
                \"btsc %m1H,#14\;\"
                \"inc %0,%0\;\";
     } else {
       error(\"Invalid fixed-point rounding mode specified\");
     }
     gcc_assert(0);
     return \"nop\";
  }"
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "fract<mode>qi2_convergent_32"
  [(set (match_operand:QI     0 "pic30_register_operand" "=&r")
        (fract_convert:QI
          (match_operand:ACC  1 "pic30_register_operand"   "r")))
   (clobber (match_scratch:HI 2                           "=&r"))
  ]
  ""
  "*
   return \"sl %d1,%2\;\"
          \"rlc.b %t1,%0\;\"
          \"sl %2,%2\;\"
          \"ior %2,%1,[w15]\;\"
          \"btsc SR,#0\;\"   /* carry clear? */
          \"bset SR,#0\;\"
          \"btsc SR,#1\;\"
          \"btst.c %0,#0\;\"
          \"addc.b %0,#0,%0\";

  "
  [
    (set_attr "cc" "clobber")
  ]
)

(define_expand "fract<mode>qi2_32"
  [(set (match_operand:QI    0 "pic30_register_operand"    "=r")
        (fract_convert:QI
          (match_operand:ACC 1 "pic30_accum_or_reg_operand" "rw")))]
  ""
  "
  { rtx r0,r1;

    r0 = operands[0];
    r1 = operands[1];
    if (!pic30_register_operand(r0,QImode)) {
      r0 = gen_reg_rtx(QImode);
    }
    if (pic30_fp_round_p() == pic30_convergent) {
      if (!pic30_accum_or_reg_operand(r1,<MODE>mode)) {
        r1 = force_reg(<MODE>mode, r1);
      }
      emit(
        gen_fract<mode>qi2_convergent_32(r0,r1)
      );
    } else {
      if (!pic30_register_operand(r1,VOIDmode)) {
        r1 = force_reg(<MODE>mode, r1);
      }
      emit(
        gen_fract<mode>qi2_helper_32(r0,r1)
      );
    }
    if (r0 != operands[0]) {
      emit_move_insn(operands[0],r0);
    }
    DONE;
  }
  "
)

(define_insn "fract<mode>hi2_helper_32"
  [(set (match_operand:HI    0 "pic30_register_operand"   "=&r,r")
        (fract_convert:HI
          (match_operand:ACC 1 "pic30_accum_or_reg_operand" "r,w")))
  ]
  ""
  "*
  {
     gcc_assert(GET_MODE_FBIT(<MODE>mode) == 31);
     if ((pic30_fp_round_p() == pic30_truncation) ||
         (pic30_fp_round_p() == pic30_fastest)) {
       /* chop the low order bits; remember with 9.31 the lowest order  */
       /*  integer bit is in the 15th bit of 2nd word */
       if (which_alternative == 0)
         return \"se %t1,%0\;\"
                \"sl %d1,[w15]\;\"
                \"rlc %0,%0\;\";
       else 
         return \"mov %m1U,%0\;\"
                \"se %0,%0\;\"
                \"sl %0,%0\;\"
                \"btsc %m1H,#15\;\"
                \"inc %0,%0\";
     } else if (pic30_fp_round_p() == pic30_conventional) {
       /* [-1,-.5] round to -1, (-.5,.5) round to 0, otherwise 1? */
       if (which_alternative == 0)
         return \"se %t1,%0\;\"
                \"sl %d1,[w15]\;\"
                \"rlc %0,%0\;\"
                \"btst.c %d1,#14\;\"
                \"addc %0,#0,%0\";
       else 
         return \"mov %m1U,%0\;\"
                \"se %0,%0\;\"
                \"sl %0,%0\;\"
                \"btsc %m1H,#15\;\"
                \"inc %0,%0\;\"
                \"btsc %m1H,#14\;\"
                \"inc %0,%0\;\";
     } else {
       error(\"Invalid fixed-point rounding mode specified\");
     }
     gcc_assert(0);
     return \"nop\";
  }"
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "fract<mode>hi2_convergent_32"
  [(set (match_operand:HI     0 "pic30_register_operand" "=&r")
        (fract_convert:HI
          (match_operand:ACC  1 "pic30_register_operand"   "r")))
   (clobber (match_scratch:HI 2                           "=&r"))
  ]
  ""
  "*
   return \"se %t1,%0\;\"
          \"sl %d1,%2\;\"
          \"rlc %0,%0\;\"
          \"sl %2,%2\;\"
          \"ior %2,%1,[w15]\;\"
          \"btsc SR,#0\;\"   /* carry clear? */
          \"bset SR,#0\;\"
          \"btsc SR,#1\;\"
          \"btst.c %0,#0\;\"
          \"addc %0,#0,%0\";
  "
  [
    (set_attr "cc" "clobber")
  ]
)

(define_expand "fract<mode>hi2_32"
  [(set (match_operand:HI    0 "pic30_register_operand"    "=r")
        (fract_convert:HI
          (match_operand:ACC 1 "pic30_accum_or_reg_operand" "rw")))]
  ""
  "
  { rtx r0,r1;

    r0 = operands[0];
    r1 = operands[1];
    if (!pic30_register_operand(r0,HImode)) {
      r0 = gen_reg_rtx(HImode);
    }
    if (pic30_fp_round_p() == pic30_convergent) {
      if (!pic30_accum_or_reg_operand(r1,<MODE>mode)) {
        r1 = force_reg(<MODE>mode, r1);
      }
      emit(
        gen_fract<mode>hi2_convergent_32(r0,r1)
      );
    } else {
      if (!pic30_register_operand(r1,VOIDmode)) {
        r1 = force_reg(<MODE>mode, r1);
      }
      emit(
        gen_fract<mode>hi2_helper_32(r0,r1)
      );
    }
    if (r0 != operands[0]) {
      emit_move_insn(operands[0],r0);
    }
    DONE;
  }
  "
)

(define_insn "fract<mode>qi2_helper_32"
  [(set (match_operand:QI     0 "pic30_register_operand"   "=&r,r")
        (fract_convert:QI
          (match_operand:UACC 1 "pic30_accum_or_reg_operand" "r,w")))
  ]
  ""
  "*
  {
     gcc_assert(GET_MODE_FBIT(<MODE>mode) == 31);
     if ((pic30_fp_round_p() == pic30_truncation) ||
         (pic30_fp_round_p() == pic30_fastest)) {
       /* chop the low order bits; remember with 9.31 the lowest order  */
       /*  integer bit is in the 15th bit of 2nd word */
       if (which_alternative == 0)
         return \"sl %d1,[w15]\;\"
                \"rlc %t1,%0\";
       else 
         return \"mov %m1U,%0\;\"
                \"sl %0,%0\;\"
                \"btsc %m1H,#15\;\"
                \"inc %0,%0\";
     } else if (pic30_fp_round_p() == pic30_conventional) {
       /* [-1,-.5] round to -1, (-.5,.5) round to 0, otherwise 1? */
       if (which_alternative == 0)
         return \"sl %d1,[w15]\;\"
                \"rlc %t1,%0\;\"
                \"btst.c %d1,#14\;\"
                \"addc.b %0,#0,%0\";
       else 
         return \"mov %m1U,%0\;\"
                \"sl %0,%0\;\"
                \"btsc %m1H,#15\;\"
                \"inc %0,%0\;\"
                \"btsc %m1H,#14\;\"
                \"inc %0,%0\;\";
     } else {
       error(\"Invalid fixed-point rounding mode specified\");
     }
     gcc_assert(0);
     return \"nop\";
  }"
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "fract<mode>qi2_convergent_32"
  [(set (match_operand:QI      0 "pic30_register_operand" "=&r")
        (fract_convert:QI
          (match_operand:UACC  1 "pic30_register_operand"   "r")))
   (clobber (match_scratch:HI 2                           "=&r"))
  ]
  ""
  "sl %d1,%2\;rlc %t1,%0\;sl %2,%2\;ior %2,%1,[w15]\;btst.c %d1,#14\;btsc SR,#1\;bclr SR,#0\;addc %0,#0,%0"
  [
    (set_attr "cc" "clobber")
  ]
)

(define_expand "fract<mode>qi2_32"
  [(set (match_operand:QI     0 "pic30_register_operand"    "=r")
        (fract_convert:QI
          (match_operand:UACC 1 "pic30_accum_or_reg_operand" "rw")))]
  ""
  "
  { rtx r0,r1;

    r0 = operands[0];
    r1 = operands[1];
    if (!pic30_register_operand(r0,QImode)) {
      r0 = gen_reg_rtx(QImode);
    }
    if (pic30_fp_round_p() == pic30_convergent) {
      if (!pic30_accum_or_reg_operand(r1,<MODE>mode)) {
        r1 = force_reg(<MODE>mode, r1);
      }
      emit(
        gen_fract<mode>qi2_convergent_32(r0,r1)
      );
    } else {
      if (!pic30_register_operand(r1,VOIDmode)) {
        r1 = force_reg(<MODE>mode, r1);
      }
      emit(
        gen_fract<mode>qi2_helper_32(r0,r1)
      );
    }
    if (r0 != operands[0]) {
      emit_move_insn(operands[0],r0);
    }
    DONE;
  }
  "
)

(define_insn "fract<mode>hi2_helper_32"
  [(set (match_operand:HI     0 "pic30_register_operand"   "=&r,r")
        (fract_convert:HI
          (match_operand:UACC 1 "pic30_accum_or_reg_operand" "r,w")))
  ]
  ""
  "*
  {
     gcc_assert(GET_MODE_FBIT(<MODE>mode) == 31);
     if ((pic30_fp_round_p() == pic30_truncation) ||
         (pic30_fp_round_p() == pic30_fastest)) {
       /* chop the low order bits; remember with 9.31 the lowest order  */
       /*  integer bit is in the 15th bit of 2nd word */
       if (which_alternative == 0)
         return \"ze %t1,%0\;\"
                \"sl %d1,[w15]\;\"
                \"rlc %t1,%0\";
       else 
         return \"mov %m1U,%0\;\"
                \"ze %0,%0\;\"
                \"sl %0,%0\;\"
                \"btsc %m1H,#15\;\"
                \"inc %0,%0\";
     } else if (pic30_fp_round_p() == pic30_conventional) {
       /* [-1,-.5] round to -1, (-.5,.5) round to 0, otherwise 1? */
       if (which_alternative == 0)
         return \"ze %t0,%0\;\"
                \"sl %d1,[w15]\;\"
                \"rlc %0,%0\;\"
                \"btst.c %d1,#14\;\"
                \"addc %0,#0,%0\";
       else 
         return \"mov %m1U,%0\;\"
                \"ze %0,%0\;\"
                \"sl %0,%0\;\"
                \"btsc %m1H,#15\;\"
                \"inc %0,%0\;\"
                \"btsc %m1H,#14\;\"
                \"inc %0,%0\";
     } else {
       error(\"Invalid fixed-point rounding mode specified\");
     }
     gcc_assert(0);
     return \"nop\";
  }"
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "fract<mode>hi2_convergent_32"
  [(set (match_operand:HI      0 "pic30_register_operand" "=&r")
        (fract_convert:HI
          (match_operand:UACC  1 "pic30_register_operand"   "r")))
   (clobber (match_scratch:HI  2                           "=&r"))
  ]
  ""
  "*
   return \"ze %t1,%0\;\"
          \"sl %d1,%2\;\"
          \"rlc %0,%0\;\"
          \"sl %2,%2\;\"
          \"ior %2,%1,[w15]\;\"
          \"btsc SR,#0\;\"   /* carry clear? */
          \"bset SR,#0\;\"
          \"btsc SR,#1\;\"
          \"btst.c %0,#0\;\"
          \"addc %0,#0,%0\";
  "
  [
    (set_attr "cc" "clobber")
  ]
)

(define_expand "fract<mode>hi2_32"
  [(set (match_operand:HI     0 "pic30_register_operand"    "=r")
        (fract_convert:HI
          (match_operand:UACC 1 "pic30_accum_or_reg_operand" "rw")))]
  ""
  "
  { rtx r0,r1;

    r0 = operands[0];
    r1 = operands[1];
    if (!pic30_register_operand(r0,HImode)) {
      r0 = gen_reg_rtx(HImode);
    }
    if (pic30_fp_round_p() == pic30_convergent) {
      if (!pic30_accum_or_reg_operand(r1,<MODE>mode)) {
        r1 = force_reg(<MODE>mode, r1);
      }
      emit(
        gen_fract<mode>hi2_convergent_32(r0,r1)
      );
    } else {
      if (!pic30_register_operand(r1,VOIDmode)) {
        r1 = force_reg(<MODE>mode, r1);
      }
      emit(
        gen_fract<mode>hi2_helper_32(r0,r1)
      );
    }
    if (r0 != operands[0]) {
      emit_move_insn(operands[0],r0);
    }
    DONE;
  }
  "
)

; To SI and DI done by function only

; to Q15

(define_insn "fractqi<mode>2_32"
  [(set (match_operand:Q15  0 "pic30_register_operand"  "=&r")
        (fract_convert:Q15
          (match_operand:QI 1 "pic30_register_operand"  "r")))]
  "pic30_fp_inline_p()"
  "*
   {  
      return 
             \"cp0.b %1\;\"        /* compare input with 0 */
             \"mov #0x8000,%0\;\"  /* assume -1 */
             \"btsc SR,#0\;\"     /* positive => 0 or .99 */
             \"mov #0x7FFF,%0\;\"  /*   load .99... */
             \"btsc SR,#1\;\"     /* if zero */
             \"clr %0\";           /*   load zero */
   }
  "
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "fracthi<mode>2_32"
  [(set (match_operand:Q15  0 "pic30_register_operand"  "=&r")
        (fract_convert:Q15
          (match_operand:HI 1 "pic30_register_operand"  "r")))]
  "pic30_fp_inline_p()"
  "*
   {
      return 
             \"cp0  %1\;\"         /* compare input with 0 */
             \"mov #0x8000,%0\;\"  /* assume -1 */
             \"btsc SR,#0\;\"     /* positive => 0 or .99 */
             \"mov #0x7FFF,%0\;\"  /*   load .99... */
             \"btsc SR,#1\;\"     /* if zero */
             \"clr %0\";           /*   load zero */
   }
  "
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "fractsi<mode>2_32"
  [(set (match_operand:Q15  0 "pic30_register_operand"  "=&r")
        (fract_convert:Q15
          (match_operand:SI 1 "pic30_register_operand"  "r")))]
  "pic30_fp_inline_p()"
  "*
   {  
      return 
             \"sub %1,#0,[w15]\;\"   /* compare input with 0 */
             \"subb %d1,#0,[w15]\;\" /* compare input with 0 */
             \"mov #0x8000,%0\;\"    /* assume -1 */
             \"btsc SR,#0\;\"       /* positive => 0 or .99 */
             \"mov #0x7FFF,%0\;\"    /*   load .99... */
             \"btsc SR,#1\;\"       /* if zero */
             \"clr %0\";             /*   load zero */
   }
  "
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "fractdi<mode>2_32"
  [(set (match_operand:Q15  0 "pic30_register_operand"  "=&r")
        (fract_convert:Q15
          (match_operand:DI 1 "pic30_register_operand"  "r")))]
  "pic30_fp_inline_p()"
  "*
   {  
      return 
             \"sub %1,#0,[w15]\;\"   /* compare input with 0 */
             \"subb %d1,#0,[w15]\;\" /* compare input with 0 */
             \"subb %t1,#0,[w15]\;\" /* compare input with 0 */
             \"subb %q1,#0,[w15]\;\" /* compare input with 0 */
             \"mov #0x8000,%0\;\"    /* assume -1 */
             \"btsc SR,#0\;\"       /* positive => 0 or .99 */
             \"mov #0x7FFF,%0\;\"    /*   load .99... */
             \"btsc SR,#1\;\"       /* if zero */
             \"clr %0\";             /*   load zero */
   }
  "
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "fractqi<mode>2_32"
  [(set (match_operand:UQ15  0 "pic30_register_operand"  "=&r")
        (fract_convert:UQ15
          (match_operand:QI  1 "pic30_register_operand"  "r")))]
  ""
  "*
   {  
      return 
             \"cp0.b %1\;\"        /* compare input with 0 */
             \"mov #0x7FFF,%0\;\"  /* assume ~1 */
             \"btsc SR,#1\;\"     /* if zero */
             \"clr %0\";           /*   load zero */
   }
  "
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "fracthi<mode>2_32"
  [(set (match_operand:UQ15  0 "pic30_register_operand"  "=&r")
        (fract_convert:UQ15
          (match_operand:HI  1 "pic30_register_operand"  "r")))]
  ""
  "*
   {
      return 
             \"cp0.b %1\;\"        /* compare input with 0 */
             \"mov #0x7FFF,%0\;\"  /* assume ~1 */
             \"btsc SR,#1\;\"     /* if zero */
             \"clr %0\";           /*   load zero */
   }
  "
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "fractsi<mode>2_32"
  [(set (match_operand:UQ15  0 "pic30_register_operand"  "=&r")
        (fract_convert:UQ15
          (match_operand:SI  1 "pic30_register_operand"  "r")))]
  ""
  "*
   {  
      return 
             \"sub %1,#0,[w15]\;\"   /* compare input with 0 */
             \"subb %d1,#0,[w15]\;\" /* compare input with 0 */
             \"mov #0x7FFF,%0\;\"    /* assume ~1 */
             \"btsc SR,#1\;\"       /* if zero */
             \"clr %0\";             /*   load zero */
   }
  "
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "fractdi<mode>2_32"
  [(set (match_operand:UQ15  0 "pic30_register_operand"  "=&r")
        (fract_convert:UQ15
          (match_operand:DI  1 "pic30_register_operand"  "r")))]
  ""
  "*
   {  
      return 
             \"sub %1,#0,[w15]\;\"   /* compare input with 0 */
             \"subb %d1,#0,[w15]\;\" /* compare input with 0 */
             \"subb %t1,#0,[w15]\;\" /* compare input with 0 */
             \"subb %q1,#0,[w15]\;\" /* compare input with 0 */
             \"mov #0x7FFF,%0\;\"    /*   load .99... */
             \"btsc SR,#1\;\"       /* if zero */
             \"clr %0\";             /*   load zero */
   }
  "
  [
    (set_attr "cc" "clobber")
  ]
)

; to Q31

(define_insn "fractqi<mode>2_32"
  [(set (match_operand:Q31  0 "pic30_register_operand"  "=&r")
        (fract_convert:Q31
          (match_operand:QI 1 "pic30_register_operand"  "r")))]
  " pic30_fp_inline_p()"
  "*
   {  
      return 
             \"cp0.b %1\;\"         /* compare input with 0 */
             \"mov #0x8000,%d0\;\"  /* assume -1 */
             \"btsc SR,#0\;\"      /* positive => 0 or .99 */
             \"mov #0x7FFF,%d0\;\"  /*   load .99... */
             \"btsc SR,#1\;\"      /* if zero */
             \"clr %d0\;\"          /*   load zero */
             \"sl  %d0,%0\;\"       /* form low word by propagating non sign */
             \"asr %0,#15,%0\";     /*   bits into low word */
   }
  "
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "fracthi<mode>2_32"
  [(set (match_operand:Q31  0 "pic30_register_operand"  "=&r")
        (fract_convert:Q31
          (match_operand:HI 1 "pic30_register_operand"  "r")))]
  " pic30_fp_inline_p()"
  "*
   {
      return 
             \"cp0.b %1\;\"         /* compare input with 0 */
             \"mov #0x8000,%d0\;\"  /* assume -1 */
             \"btsc SR,#0\;\"      /* positive => 0 or .99 */
             \"mov #0x7FFF,%d0\;\"  /*   load .99... */
             \"btsc SR,#1\;\"      /* if zero */
             \"clr %d0\;\"          /*   load zero */
             \"sl  %d0,%0\;\"       /* form low word by propagating non sign */
             \"asr %0,#15,%0\";     /*   bits into low word */
   }
  "
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "fractsi<mode>2_32"
  [(set (match_operand:Q31  0 "pic30_register_operand"  "=&r")
        (fract_convert:Q31
          (match_operand:SI 1 "pic30_register_operand"  "r")))]
  " pic30_fp_inline_p()"
  "*
   {  
      return 
             \"sub %1,#0,[w15]\;\"   /* compare input with 0 */
             \"subb %d1,#0,[w15]\;\" /* compare input with 0 */
             \"mov #0x8000,%d0\;\"   /* assume -1 */
             \"btsc SR,#0\;\"       /* positive => 0 or .99 */
             \"mov #0x7FFF,%d0\;\"   /*   load .99... */
             \"btsc SR,#1\;\"       /* if zero */
             \"clr %d0\;\"           /*   load zero */
             \"sl  %d0,%0\;\"        /* form low word by propagating non sign */
             \"asr %0,#15,%0\";      /*   bits into low word */
   }
  "
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "fractdi<mode>2_32"
  [(set (match_operand:Q31  0 "pic30_register_operand"  "=&r")
        (fract_convert:Q31
          (match_operand:DI 1 "pic30_register_operand"  "r")))]
  " pic30_fp_inline_p()"
  "*
   {  
      return 
             \"sub %1,#0,[w15]\;\"   /* compare input with 0 */
             \"subb %d1,#0,[w15]\;\" /* compare input with 0 */
             \"subb %t1,#0,[w15]\;\" /* compare input with 0 */
             \"subb %q1,#0,[w15]\;\" /* compare input with 0 */
             \"mov #0x8000,%d0\;\"   /* assume -1 */
             \"btsc SR,#0\;\"       /* positive => 0 or .99 */
             \"mov #0x7FFF,%d0\;\"   /*   load .99... */
             \"btsc SR,#1\;\"       /* if zero */
             \"clr %d0\;\"           /*   load zero */
             \"sl  %d0,%0\;\"        /* form low word by propagating non sign */
             \"asr %0,#15,%0\";      /*   bits into low word */
   }
  "
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "fractqi<mode>2_32"
  [(set (match_operand:UQ31  0 "pic30_register_operand"  "=&r")
        (fract_convert:UQ31
          (match_operand:QI  1 "pic30_register_operand"  "r")))]
  ""
  "*
   {  
      return 
             \"cp0.b %1\;\"         /* compare input with 0 */
             \"mov #0x7FFF,%d0\;\"  /*   load .99... */
             \"setm %0\;\"
             \"btsc SR,#1\;\"      /* if zero */
             \"mul.uu %0,#0,%0\";   /*   load zero */
   }
  "
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "fracthi<mode>2_32"
  [(set (match_operand:UQ31  0 "pic30_register_operand"  "=&r")
        (fract_convert:UQ31
          (match_operand:HI  1 "pic30_register_operand"  "r")))]
  ""
  "*
   {
      return 
             \"cp0.b %1\;\"         /* compare input with 0 */
             \"mov #0x7FFF,%d0\;\"  /*   load .99... */
             \"setm %0\;\"
             \"btsc SR,#1\;\"      /* if zero */
             \"mul.uu %0,#0,%0\";   /*   load zero */
   }
  "
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "fractsi<mode>2_32"
  [(set (match_operand:UQ31  0 "pic30_register_operand"  "=&r")
        (fract_convert:UQ31
          (match_operand:SI  1 "pic30_register_operand"  "r")))]
  ""
  "*
   {  
      return 
             \"sub %1,#0,[w15]\;\"   /* compare input with 0 */
             \"subb %d1,#0,[w15]\;\" /* compare input with 0 */
             \"mov #0x7FFF,%d0\;\"   /*   load .99... */
             \"setm %0\;\"
             \"btsc SR,#1\;\"       /* if zero */
             \"mul.uu %0,#0,%0\";    /*   load zero */
   }
  "
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "fractdi<mode>2_32"
  [(set (match_operand:UQ31  0 "pic30_register_operand"  "=&r")
        (fract_convert:UQ31
          (match_operand:DI  1 "pic30_register_operand"  "r")))]
  ""
  "*
   {  
      return 
             \"sub %1,#0,[w15]\;\"   /* compare input with 0 */
             \"subb %d1,#0,[w15]\;\" /* compare input with 0 */
             \"subb %t1,#0,[w15]\;\" /* compare input with 0 */
             \"subb %q1,#0,[w15]\;\" /* compare input with 0 */
             \"mov #0x7FFF,%d0\;\"   /*   load .99... */
             \"setm %0\;\"
             \"btsc SR,#1\;\"       /* if zero */
             \"mul.uu %0,#0,%0\";    /*   load zero */
   }
  "
  [
    (set_attr "cc" "clobber")
  ]
)

; to ACC

(define_insn "fractqi<mode>2_32"
  [(set (match_operand:ACC  0 "pic30_register_operand"  "=&r")
        (fract_convert:ACC
          (match_operand:QI 1 "pic30_register_operand"    "r")))]
  "pic30_fp_inline_p()"
  "*
   {  
      return 
             \"mul.uu %0,#0,%0\;\"  /* clear low bits */
             \"mov.b %1,%t0\;\"     /* copy integer portion into upper byte */
             \"asr.b %t0,%t0\;\"    /* shift it into position */
             \"rrc %d0,%d0\";
   }
  "
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "fracthi<mode>2_32"
  [(set (match_operand:ACC  0 "pic30_register_operand"  "=&r")
        (fract_convert:ACC
          (match_operand:HI 1 "pic30_register_operand"  "r")))]
  "pic30_fp_inline_p()"
  "*
   {
      return 
             \"mul.uu %0,#0,%0\;\"  /* clear low bits */
             \"mov %1,%t0\;\"       /* copy low integer portion to upper byte */
             \"asr.b %t0,%t0\;\"    /* shift it into position */
             \"rrc %d0,%d0\";
   }
  "
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "fractsi<mode>2_32"
  [(set (match_operand:ACC  0 "pic30_register_operand"  "=&r")
        (fract_convert:ACC
          (match_operand:SI 1 "pic30_register_operand"  "r")))]
  "pic30_fp_inline_p()"
  "*
   {  
      return 
             \"mul.uu %0,#0,%0\;\"  /* clear low bits */
             \"mov %1,%t0\;\"       /* copy low integer portion to upper byte */
             \"asr.b %t0,%t0\;\"    /* shift it into position */
             \"rrc %d0,%d0\";
   }
  "
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "fractdi<mode>2_32"
  [(set (match_operand:ACC  0 "pic30_register_operand"  "=&r")
        (fract_convert:ACC
          (match_operand:DI 1 "pic30_register_operand"  "r")))]
  "pic30_fp_inline_p()"
  "*
   {  
      return 
             \"mul.uu %0,#0,%0\;\"  /* clear low bits */
             \"mov %1,%t0\;\"       /* copy low integer portion to upper byte */
             \"asr.b %t0,%t0\;\"    /* shift it into position */
             \"rrc %d0,%d0\";
   }
  "
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "fractqi<mode>2_32"
  [(set (match_operand:UACC  0 "pic30_register_operand"  "=&r")
        (fract_convert:UACC
          (match_operand:QI  1 "pic30_register_operand"  "r")))]
  ""
  "*
   {  
      return 
             \"mul.uu %0,#0,%0\;\"  /* clear low bits */
             \"mov.b %1,%t0\;\"     /* copy integer portion into upper byte */
             \"asr.b %t0,%t0\;\"    /* shift it into position */
             \"rrc %d0,%d0\";
   }
  "
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "fracthi<mode>2_32"
  [(set (match_operand:UACC  0 "pic30_register_operand"  "=&r")
        (fract_convert:UACC
          (match_operand:HI  1 "pic30_register_operand"  "r")))]
  ""
  "*
   {
      return 
             \"mul.uu %0,#0,%0\;\"  /* clear low bits */
             \"mov %1,%t0\;\"       /* copy low integer portion to upper byte */
             \"asr.b %t0,%t0\;\"    /* shift it into position */
             \"rrc %d0,%d0\";
   }
  "
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "fractsi<mode>2_32"
  [(set (match_operand:UACC  0 "pic30_register_operand"  "=&r")
        (fract_convert:UACC
          (match_operand:SI  1 "pic30_register_operand"  "r")))]
  ""
  "*
   {  
      return 
             \"mul.uu %0,#0,%0\;\"  /* clear low bits */
             \"mov %1,%t0\;\"       /* copy low integer portion to upper byte */
             \"asr.b %t0,%t0\;\"    /* shift it into position */
             \"rrc %d0,%d0\";
   }
  "
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "fractdi<mode>2_32"
  [(set (match_operand:UACC  0 "pic30_register_operand"  "=&r")
        (fract_convert:UACC
          (match_operand:DI  1 "pic30_register_operand"  "r")))]
  ""
  "*
   {  
      return 
             \"mul.uu %0,#0,%0\;\"  /* clear low bits */
             \"mov %1,%t0\;\"       /* copy low integer portion to upper byte */
             \"asr.b %t0,%t0\;\"    /* shift it into position */
             \"rrc %d0,%d0\";
   }
  "
  [
    (set_attr "cc" "clobber")
  ]
)

; unsigned integer (IMODE) modes
;  from Q15

(define_insn "fractuns<mode>qi2_helper_32"
  [(set (match_operand:QI    0 "pic30_register_operand"  "=&r")
        (unsigned_fract_convert:QI
          (match_operand:QUQ15 1 "pic30_register_operand"  "r")))]
  ""
  "*
  {
     gcc_assert(GET_MODE_FBIT(<MODE>mode) == 15);
     if ((pic30_fp_round_p() == pic30_truncation) ||
         (pic30_fp_round_p() == pic30_fastest)) {
       return \"asr %1,#15,%0\"; 
     } else if (pic30_fp_round_p() == pic30_conventional) {
       return \"asr %1,#14,%0\;btss %0,#0\;clr.b %0\";
     } else {
       error(\"Invalid fixed-point rounding mode specified\");
     }
     gcc_assert(0);
     return \"nop\";

  }"
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "fractuns<mode>qi2_convergent_32"
  [(set (match_operand:QI      0 "pic30_register_operand" "=&r")
        (unsigned_fract_convert:QI
          (match_operand:QUQ15   1 "pic30_register_operand"   "r")))
   (clobber (match_scratch:QUQ15 2                           "=r"))
  ]
  ""
  "*
  {
     gcc_assert(GET_MODE_FBIT(<MODE>mode) == 15);
     if (pic30_fp_round_p() == pic30_convergent) {
       return \"asr %1,#14,%0\;\"  /* as conventional */
              \"btss %0,#0\;\"
              \"clr.b %0\;\"
              \"sl %1,#2,%2\;\"    /* x.yz13 where z13 == 0 */
              \"btsc SR,#1\;\"    /*   always rounds to even 0 */
              \"clr.b %0\";
     } else {
       error(\"Invalid fixed-point rounding mode specified\");
     }
     gcc_assert(0);
     return \"nop\";

  }"
  [
    (set_attr "cc" "clobber")
  ]
)

(define_expand "fractuns<mode>qi2_32"
  [(set (match_operand:QI    0 "pic30_register_operand" "=r")
        (unsigned_fract_convert:QI
          (match_operand:QUQ15 1 "pic30_register_operand"  "r")))]
  ""
  "
  { rtx r0,r1;

    if (!pic30_fp_inline_p() && (pic30_fp_round_p() == pic30_convergent))
      FAIL;
    r0 = operands[0];
    r1 = operands[1];
    if (!pic30_register_operand(r0,QImode)) {
      r0 = gen_reg_rtx(QImode);
    }
    if (!pic30_register_operand(r1,VOIDmode)) {
      r1 = force_reg(<MODE>mode, r1);
    }
    if (pic30_fp_round_p() == pic30_convergent) {
      emit(
        gen_fractuns<mode>qi2_convergent_32(r0,r1)
      );
    } else {
      emit(
        gen_fractuns<mode>qi2_helper_32(r0,r1)
      );
    }
    if (r0 != operands[0]) {
      emit_move_insn(operands[0],r0);
    }
    DONE;
  }
  "
)

(define_insn "fractuns<mode>hi2_helper_32"
  [(set (match_operand:HI    0 "pic30_register_operand"  "=&r")
        (unsigned_fract_convert:HI
          (match_operand:QUQ15 1 "pic30_register_operand"  "r")))]
  ""
  "*
  {
     gcc_assert(GET_MODE_FBIT(<MODE>mode) == 15);
     if ((pic30_fp_round_p() == pic30_truncation) ||
         (pic30_fp_round_p() == pic30_fastest)) {
       /* chop the low order bits, signed results are either 0 or -1 */
       return \"asr %1,#15,%0\"; 
     } else if (pic30_fp_round_p() == pic30_conventional) {
       /* [-1,-.5] round to -1, (-.5,.5) round to 0, otherwise 1? */
       return \"asr %1,#14,%0\;btss %0,#0\;clr %0\";
     } else {
       error(\"Invalid fixed-point rounding mode specified\");
     }
     gcc_assert(0);
     return \"nop\";

  }"
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "fractuns<mode>hi2_convergent_32"
  [(set (match_operand:HI    0 "pic30_register_operand"  "=&r")
        (unsigned_fract_convert:HI
          (match_operand:QUQ15 1 "pic30_register_operand"  "r")))
   (clobber (match_scratch:QUQ15 2                         "=r"))
  ]
  ""
  "*
  {
     gcc_assert(GET_MODE_FBIT(<MODE>mode) == 15);
     if (pic30_fp_round_p() == pic30_convergent) {
       return \"asr %1,#14,%0\;\"  /* as conventional */
              \"btss %0,#0\;\"
              \"clr %0\;\"
              \"sl %1,#2,%2\;\"    /* x.yz13 where z13 == 0 */
              \"btsc SR,#1\;\"    /*   always rounds to even 0 */
              \"clr %0\";
     } else {
       error(\"Invalid fixed-point rounding mode specified\");
     }
     gcc_assert(0);
     return \"nop\";

  }"
  [
    (set_attr "cc" "clobber")
  ]
)

(define_expand "fractuns<mode>hi2_32"
  [(set (match_operand:HI    0 "pic30_register_operand"  "=r")
        (unsigned_fract_convert:HI
          (match_operand:QUQ15 1 "pic30_register_operand"  "r")))]
  ""
  "
  { rtx r0,r1;

    if (!pic30_fp_inline_p() && (pic30_fp_round_p() == pic30_convergent))
      FAIL;
    r0 = operands[0];
    r1 = operands[1];
    if (!pic30_register_operand(r0,HImode)) {
      r0 = gen_reg_rtx(HImode);
    }
    if (!pic30_register_operand(r1,VOIDmode)) {
      r1 = force_reg(<MODE>mode, r1);
    }
    if (pic30_fp_round_p() == pic30_convergent) {
      emit(
        gen_fractuns<mode>hi2_convergent_32(r0,r1)
      );
    } else {
      emit(
        gen_fractuns<mode>hi2_helper_32(r0,r1)
      );
    }
    if (r0 != operands[0]) {
      emit_move_insn(operands[0],r0);
    }
    DONE;
  }
  "
)

(define_insn "fractuns<mode>si2_helper_32"
  [(set (match_operand:SI    0 "pic30_register_operand"  "=&r")
        (unsigned_fract_convert:SI
          (match_operand:QUQ15 1 "pic30_register_operand"  "r")))]
  ""
  "*
  {
     gcc_assert(GET_MODE_FBIT(<MODE>mode) == 15);
     if ((pic30_fp_round_p() == pic30_truncation) ||
         (pic30_fp_round_p() == pic30_fastest)) {
       /* chop the low order bits, signed results are either 0 or -1 */
       return \"asr %1,#15,%d0\;asr %1,#15,%0\";
     } else if (pic30_fp_round_p() == pic30_conventional) {
       /* [-1,-.5] round to -1, (-.5,.5) round to 0, otherwise 1? */
       return \"asr %1,#14,%0\;btss %0,#0\;clr %0\;asr %0,#15,%d0\";
     } else {
       error(\"Invalid fixed-point rounding mode specified\");
     }
     gcc_assert(0);
     return \"nop\";
  }"
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "fractuns<mode>si2_convergent_32"
  [(set (match_operand:SI    0 "pic30_register_operand"  "=&r")
        (unsigned_fract_convert:SI
          (match_operand:QUQ15 1 "pic30_register_operand"  "r")))
   (clobber (match_scratch:QUQ15 2                         "=r"))
  ]
  ""
  "*
  {
     gcc_assert(GET_MODE_FBIT(<MODE>mode) == 15);
     if (pic30_fp_round_p() == pic30_convergent) {
       return \"asr %1,#14,%0\;\" /* as conventional */
              \"btss %0,#0\;\"
              \"clr  %0\;\"
              \"asr %0,#15,%d0\;\"
              \"sl %1,#2,%2\;\"    /* x.yz13 where z13 == 0 */
              \"btsc SR,#1\;\"    /*   always rounds to even 0 */
              \"mul.uu %0,#0,%0\";
     } else {
       error(\"Invalid fixed-point rounding mode specified\");
     }
     gcc_assert(0);
     return \"nop\";

  }"
  [
    (set_attr "cc" "clobber")
  ]
)

(define_expand "fractuns<mode>si2_32"
  [(set (match_operand:SI    0 "pic30_register_operand"  "=r")
        (unsigned_fract_convert:SI
          (match_operand:QUQ15 1 "pic30_register_operand"  "r")))]
  ""
  "
  { rtx r0,r1;

    if (!pic30_fp_inline_p() && (pic30_fp_round_p() == pic30_convergent))
      FAIL;
    r0 = operands[0];
    r1 = operands[1];
    if (!pic30_register_operand(r0,SImode)) {
      r0 = gen_reg_rtx(SImode);
    }
    if (!pic30_register_operand(r1,VOIDmode)) {
      r1 = force_reg(<MODE>mode, r1);
    }
    if (pic30_fp_round_p() == pic30_convergent) {
      emit(
        gen_fractuns<mode>si2_convergent_32(r0,r1)
      );
    } else {
      emit(
        gen_fractuns<mode>si2_helper_32(r0,r1)
      );
    }
    if (r0 != operands[0]) {
      emit_move_insn(operands[0],r0);
    }
    DONE;
  }
  "
)

(define_insn "fractuns<mode>di2_helper_32"
  [(set (match_operand:DI    0 "pic30_register_operand"  "=&r")
        (unsigned_fract_convert:DI
          (match_operand:QUQ15 1 "pic30_register_operand"  "r")))]
  ""
  "*
  {
     gcc_assert(GET_MODE_FBIT(<MODE>mode) == 15);
     if ((pic30_fp_round_p() == pic30_truncation) ||
         (pic30_fp_round_p() == pic30_fastest)) {
       /* chop the low order bits, signed results are either 0 or -1 */
       return \"asr %1,#15,%q0\;asr %1,#15,%t0\;mul.su %t0,#1,%0\";
     } else if (pic30_fp_round_p() == pic30_conventional) {
       /* [-1,-.5] round to -1, (-.5,.5) round to 0, otherwise 1? */
       return \"asr %1,#14,%0\;\"
              \"btss %0,#0\;\"
              \"clr %0\;\"
              \"asr %0,#15,%d0\;\"
              \"mul.su %d0,#1,%t0\";
     } else {
       error(\"Invalid fixed-point rounding mode specified\");
     }
     gcc_assert(0);
     return \"nop\";
  }"
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "fractuns<mode>di2_convergent_32"
  [(set (match_operand:DI    0 "pic30_register_operand"  "=&r")
        (unsigned_fract_convert:DI
          (match_operand:QUQ15 1 "pic30_register_operand"  "r")))
   (clobber (match_scratch:QUQ15 2                         "=r"))
  ]
  ""
  "*
  {
     gcc_assert(GET_MODE_FBIT(<MODE>mode) == 15);
     if (pic30_fp_round_p() == pic30_convergent) {
       return \"asr %1,#14,%0\;\"  /* as conventional */
              \"btss %0,#0\;\"
              \"clr  %0\;\"
              \"asr %0,#15,%d0\;\"
              \"sl %1,#2,%2\;\"    /* x.yz13 where z13 == 0 */
              \"btsc SR,#1\;\"    /*   always rounds to even 0 */
              \"mul.uu %0,#0,%0\;\"
              \"mul.su %d0,#1,%t0\";
     } else {
       error(\"Invalid fixed-point rounding mode specified\");
     }
     gcc_assert(0);
     return \"nop\";

  }"
  [
    (set_attr "cc" "clobber")
  ]
)

(define_expand "fractuns<mode>di2_32"
  [(set (match_operand:DI    0 "pic30_register_operand"  "=r")
        (unsigned_fract_convert:DI
          (match_operand:QUQ15 1 "pic30_register_operand"  "r")))]
  ""
  "
  { rtx r0,r1;

    if (!pic30_fp_inline_p() && 
        ((pic30_fp_round_p() == pic30_convergent) ||
         (pic30_fp_round_p() == pic30_conventional)))
      FAIL;
    r0 = operands[0];
    r1 = operands[1];
    if (!pic30_register_operand(r0,DImode)) {
      r0 = gen_reg_rtx(DImode);
    }
    if (!pic30_register_operand(r1,VOIDmode)) {
      r1 = force_reg(<MODE>mode, r1);
    }
    if (pic30_fp_round_p() == pic30_convergent) {
      emit(
        gen_fractuns<mode>di2_convergent_32(r0,r1)
      );
    } else {
      emit(
        gen_fractuns<mode>di2_helper_32(r0,r1)
      );
    }
    if (r0 != operands[0]) {
      emit_move_insn(operands[0],r0);
    }
    DONE;
  }
  "
)

; from Q31

(define_insn "fractuns<mode>qi2_helper_32"
  [(set (match_operand:QI    0 "pic30_register_operand"  "=&r")
        (unsigned_fract_convert:QI
          (match_operand:QUQ31 1 "pic30_register_operand"  "r")))]
  ""
  "*
  {
     gcc_assert(GET_MODE_FBIT(<MODE>mode) == 31);
     if ((pic30_fp_round_p() == pic30_truncation) ||
         (pic30_fp_round_p() == pic30_fastest)) {
       /* chop the low order bits, signed results are either 0 or -1 */
       return \"asr %d1,#15,%0\"; 
     } else if (pic30_fp_round_p() == pic30_conventional) {
       /* [-1,-.5] round to -1, (-.5,.5) round to 0, otherwise 1? */
       return \"asr %d1,#14,%0\;btss %0,#0\;clr.b %0\";
     } else {
       error(\"Invalid fixed-point rounding mode specified\");
     }
     gcc_assert(0);
     return \"nop\";

  }"
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "fractuns<mode>qi2_convergent_32"
  [(set (match_operand:QI    0 "pic30_register_operand"  "=&r")
        (unsigned_fract_convert:QI
          (match_operand:QUQ31 1 "pic30_register_operand"  "r")))
   (clobber (match_scratch:QUQ31 2                         "=&r"))
  ]
  ""
  "*
  {
     gcc_assert(GET_MODE_FBIT(<MODE>mode) == 31);
     if (pic30_fp_round_p() == pic30_convergent) {
       return \"asr %d1,#14,%0\;\"  /* as conventional */
              \"btss %0,#0\;\"
              \"clr.b %0\;\"
              \"sl %d1,#2,%2\;\"    /* x.yz13 where z13 == 0 */
              \"btsc SR,#1\;\"     /*   always rounds to even 0 */
              \"cp0 %1\;\"  
              \"btsc SR,#1\;\"
              \"clr %0\";
     } else {
       error(\"Invalid fixed-point rounding mode specified\");
     }
     gcc_assert(0);
     return \"nop\";

  }"
  [
    (set_attr "cc" "clobber")
  ]
)

(define_expand "fractuns<mode>qi2_32"
  [(set (match_operand:QI    0 "pic30_register_operand"  "=r")
        (unsigned_fract_convert:QI
          (match_operand:QUQ31 1 "pic30_register_operand"  "r")))]
  ""
  "
  { rtx r0,r1;

    if (!pic30_fp_inline_p() && (pic30_fp_round_p() == pic30_convergent))
      FAIL;
    r0 = operands[0];
    r1 = operands[1];
    if (!pic30_register_operand(r0,QImode)) {
      r0 = gen_reg_rtx(QImode);
    }
    if (!pic30_register_operand(r1,VOIDmode)) {
      r1 = force_reg(<MODE>mode, r1);
    }
    if (pic30_fp_round_p() == pic30_convergent) {
      emit(
        gen_fractuns<mode>qi2_convergent_32(r0,r1)
      );
    } else {
      emit(
        gen_fractuns<mode>qi2_helper_32(r0,r1)
      );
    }
    if (r0 != operands[0]) {
      emit_move_insn(operands[0],r0);
    }
    DONE;
  }
  "
)

(define_insn "fractuns<mode>hi2_helper_32"
  [(set (match_operand:HI    0 "pic30_register_operand"  "=&r")
        (unsigned_fract_convert:HI
          (match_operand:QUQ31 1 "pic30_register_operand"  "r")))]
  ""
  "*
  {
     gcc_assert(GET_MODE_FBIT(<MODE>mode) == 31);
     if ((pic30_fp_round_p() == pic30_truncation) ||
         (pic30_fp_round_p() == pic30_fastest)) {
       /* chop the low order bits, signed results are either 0 or -1 */
       return \"asr %d1,#15,%0\"; 
     } else if (pic30_fp_round_p() == pic30_conventional) {
       /* [-1,-.5] round to -1, (-.5,.5) round to 0, otherwise 1? */
       return \"asr %d1,#14,%0\;btss %0,#0\;clr %0\";
     } else {
       error(\"Invalid fixed-point rounding mode specified\");
     }
     gcc_assert(0);
     return \"nop\";

  }"
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "fractuns<mode>hi2_convergent_32"
  [(set (match_operand:HI    0 "pic30_register_operand"  "=&r")
        (unsigned_fract_convert:HI
          (match_operand:QUQ31 1 "pic30_register_operand"  "r")))
   (clobber (match_scratch:QUQ31 2                         "=&r"))
  ]
  ""
  "*
  {
     gcc_assert(GET_MODE_FBIT(<MODE>mode) == 31);
     if (pic30_fp_round_p() == pic30_convergent) {
       return \"asr %d1,#14,%0\;\"  /* as conventional */
              \"btss %0,#0\;\"
              \"clr %0\;\"
              \"sl %d1,#2,%2\;\"    /* x.yz13 where z13 == 0 */
              \"btsc SR,#1\;\"     /*   always rounds to even 0 */
              \"cp0 %1\;\"  
              \"btsc SR,#1\;\"
              \"clr %0\";
     } else {
       error(\"Invalid fixed-point rounding mode specified\");
     }
     gcc_assert(0);
     return \"nop\";

  }"
  [
    (set_attr "cc" "clobber")
  ]
)

(define_expand "fractuns<mode>hi2_32"
  [(set (match_operand:HI    0 "pic30_register_operand"  "=r")
        (unsigned_fract_convert:HI
          (match_operand:QUQ31 1 "pic30_register_operand"  "r")))]
  ""
  "
  { rtx r0,r1;

    if (!pic30_fp_inline_p() && (pic30_fp_round_p() == pic30_convergent))
      FAIL;
    r0 = operands[0];
    r1 = operands[1];
    if (!pic30_register_operand(r0,HImode)) {
      r0 = gen_reg_rtx(HImode);
    }
    if (!pic30_register_operand(r1,VOIDmode)) {
      r1 = force_reg(<MODE>mode, r1);
    }
    if (pic30_fp_round_p() == pic30_convergent) {
      emit(
        gen_fractuns<mode>hi2_convergent_32(r0,r1)
      );
    } else {
      emit(
        gen_fractuns<mode>hi2_helper_32(r0,r1)
      );
    }
    if (r0 != operands[0]) {
      emit_move_insn(operands[0],r0);
    }
    DONE;
  }
  "
)

(define_insn "fractuns<mode>si2_helper_32"
  [(set (match_operand:SI    0 "pic30_register_operand"  "=&r")
        (unsigned_fract_convert:SI
          (match_operand:QUQ31 1 "pic30_register_operand"  "r")))]
  ""
  "*
  {
     gcc_assert(GET_MODE_FBIT(<MODE>mode) == 31);
     if ((pic30_fp_round_p() == pic30_truncation) ||
         (pic30_fp_round_p() == pic30_fastest)) {
       /* chop the low order bits, signed results are either 0 or -1 */
       return \"asr %d1,#15,%d0\;asr %d1,#15,%0\";
     } else if (pic30_fp_round_p() == pic30_conventional) {
       /* [-1,-.5] round to -1, (-.5,.5) round to 0, otherwise 1? */
       return \"asr %d1,#14,%0\;\"
              \"btss %0,#0\;\"
              \"clr %0\;\"
              \"asr %0,#15,%d0\";
     } else {
       error(\"Invalid fixed-point rounding mode specified\");
     }
     gcc_assert(0);
     return \"nop\";
  }"
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "fractuns<mode>si2_convergent_32"
  [(set (match_operand:SI    0 "pic30_register_operand"  "=&r")
        (unsigned_fract_convert:SI
          (match_operand:QUQ31 1 "pic30_register_operand"  "r")))
   (clobber (match_scratch:QUQ31 2                         "=&r"))
  ]
  ""
  "*
  {
     gcc_assert(GET_MODE_FBIT(<MODE>mode) == 31);
     if (pic30_fp_round_p() == pic30_convergent) {
       return \"asr %d1,#14,%0\;\"  /* as conventional */
              \"btss %0,#0\;\"
              \"clr  %0\;\"
              \"asr %0,#15,%d0\;\"
              \"sl %d1,#2,%2\;\"    /* x.yz13 where z13 == 0 */
              \"btsc SR,#1\;\"     /*   always rounds to even 0 */
              \"cp0 %1\;\"
              \"btsc SR,#1\;\"
              \"mul.uu %0,#0,%0\";
     } else {
       error(\"Invalid fixed-point rounding mode specified\");
     }
     gcc_assert(0);
     return \"nop\";

  }"
  [
    (set_attr "cc" "clobber")
  ]
)

(define_expand "fractuns<mode>si2_32"
  [(set (match_operand:SI    0 "pic30_register_operand"  "=r")
        (unsigned_fract_convert:SI
          (match_operand:QUQ31 1 "pic30_register_operand"  "r")))]
  ""
  "
  { rtx r0,r1;

    if (!pic30_fp_inline_p() && (pic30_fp_round_p() == pic30_convergent))
      FAIL;
    r0 = operands[0];
    r1 = operands[1];
    if (!pic30_register_operand(r0,SImode)) {
      r0 = gen_reg_rtx(SImode);
    }
    if (!pic30_register_operand(r1,VOIDmode)) {
      r1 = force_reg(<MODE>mode, r1);
    }
    if (pic30_fp_round_p() == pic30_convergent) {
      emit(
        gen_fractuns<mode>si2_convergent_32(r0,r1)
      );
    } else {
      emit(
        gen_fractuns<mode>si2_helper_32(r0,r1)
      );
    }
    if (r0 != operands[0]) {
      emit_move_insn(operands[0],r0);
    }
    DONE;
  }
  "
)

(define_insn "fractuns<mode>di2_helper_32"
  [(set (match_operand:DI    0 "pic30_register_operand"  "=&r")
        (unsigned_fract_convert:DI
          (match_operand:QUQ31 1 "pic30_register_operand"  "r")))]
  ""
  "*
  {
     gcc_assert(GET_MODE_FBIT(<MODE>mode) == 31);
     if ((pic30_fp_round_p() == pic30_truncation) ||
         (pic30_fp_round_p() == pic30_fastest)) {
       /* chop the low order bits, signed results are either 0 or -1 */
       return \"asr %d1,#15,%q0\;asr %d1,#15,%t0\;mul.su %t0,#1,%0\";
     } else if (pic30_fp_round_p() == pic30_conventional) {
       /* [-1,-.5] round to -1, (-.5,.5) round to 0, otherwise 1? */
       return \"asr %d1,#14,%0\;\"
              \"btss %0,#0\;\"
              \"clr %0\;\"
              \"asr %0,#15,%d0\;\"
              \"mul.su %d0,#1,%t0\";
     } else {
       error(\"Invalid fixed-point rounding mode specified\");
     }
     gcc_assert(0);
     return \"nop\";
  }"
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "fractuns<mode>di2_convergent_32"
  [(set (match_operand:DI    0 "pic30_register_operand"  "=&r")
        (unsigned_fract_convert:DI
          (match_operand:QUQ31 1 "pic30_register_operand"  "r")))
   (clobber (match_scratch:QUQ31 2                         "=&r"))
  ]
  ""
  "*
  {
     gcc_assert(GET_MODE_FBIT(<MODE>mode) == 31);
     if (pic30_fp_round_p() == pic30_convergent) {
       return \"asr %d1,#14,%0\;\"  /* as conventional */
              \"btss %0,#0\;\"
              \"clr  %0\;\"
              \"asr %0,#15,%d0\;\"
              \"sl %d1,#2,%2\;\"    /* x.yz13 where z13 == 0 */
              \"btsc SR,#1\;\"     /*   always rounds to even 0 */
              \"cp0 %1\;\"
              \"btsc SR,#1\;\"
              \"mul.uu %0,#0,%0\;\"
              \"mul.su %d0,#1,%t0\";
     } else {
       error(\"Invalid fixed-point rounding mode specified\");
     }
     gcc_assert(0);
     return \"nop\";

  }"
  [
    (set_attr "cc" "clobber")
  ]
)

(define_expand "fractuns<mode>di2_32"
  [(set (match_operand:DI    0 "pic30_register_operand"  "=r")
        (unsigned_fract_convert:DI
          (match_operand:QUQ31 1 "pic30_register_operand"  "r")))]
  ""
  "
  { rtx r0,r1;

    if (!pic30_fp_inline_p() && 
         ((pic30_fp_round_p() == pic30_convergent) ||
          (pic30_fp_round_p() == pic30_convergent)))
      FAIL;
    r0 = operands[0];
    r1 = operands[1];
    if (!pic30_register_operand(r0,DImode)) {
      r0 = gen_reg_rtx(DImode);
    }
    if (!pic30_register_operand(r1,VOIDmode)) {
      r1 = force_reg(<MODE>mode, r1);
    }
    if (pic30_fp_round_p() == pic30_convergent) {
      emit(
        gen_fractuns<mode>di2_convergent_32(r0,r1)
      );
    } else {
      emit(
        gen_fractuns<mode>di2_helper_32(r0,r1)
      );
    }
    if (r0 != operands[0]) {
      emit_move_insn(operands[0],r0);
    }
    DONE;
  }
  "
)

; from ACC

(define_insn "fractuns<mode>qi2_helper_32"
  [(set (match_operand:QI    0 "pic30_register_operand"   "=&r,r")
        (unsigned_fract_convert:QI
          (match_operand:ACC 1 "pic30_accum_or_reg_operand" "r,w")))
  ]
  ""
  "*
  {
     gcc_assert(GET_MODE_FBIT(<MODE>mode) == 31);
     if ((pic30_fp_round_p() == pic30_truncation) ||
         (pic30_fp_round_p() == pic30_fastest)) {
       /* chop the low order bits; remember with 9.31 the lowest order  */
       /*  integer bit is in the 15th bit of 2nd word */
       if (which_alternative == 0)
         return \"sl %d1,[w15]\;\"
                \"rlc %t1,%0\";
       else 
         return \"mov %m1U,%0\;\"
                \"sl %0,%0\;\"
                \"btsc %m1H\;\"
                \"inc %0,%0\";
     } else if (pic30_fp_round_p() == pic30_conventional) {
       /* [-1,-.5] round to -1, (-.5,.5) round to 0, otherwise 1? */
       if (which_alternative == 0)
         return \"sl %d1,[w15]\;\"
                \"rlc %t1,%0\;\"
                \"btst.c %d1,#14\;\"
                \"addc.b %0,#0,%0\";
       else 
         return \"mov %m1U,%0\;\"
                \"sl %0,%0\;\"
                \"btsc %m1H,#15\;\"
                \"inc %0,%0\;\"
                \"btsc %m1H,#14\;\"
                \"inc %0,%0\;\";
     } else {
       error(\"Invalid fixed-point rounding mode specified\");
     }
     gcc_assert(0);
     return \"nop\";
  }"
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "fractuns<mode>qi2_convergent_32"
  [(set (match_operand:QI     0 "pic30_register_operand" "=&r")
        (unsigned_fract_convert:QI
          (match_operand:ACC  1 "pic30_register_operand"   "r")))
   (clobber (match_scratch:HI 2                           "=&r"))
  ]
  ""
  "*
   return \"sl %d1,%2\;\"
          \"rlc.b %t1,%0\;\"
          \"sl %2,%2\;\"
          \"ior %2,%1,[w15]\;\"
          \"btsc SR,#0\;\"   /* carry clear? */
          \"bset SR,#0\;\"
          \"btsc SR,#1\;\"
          \"btst.c %0,#0\;\"
          \"addc.b %0,#0,%0\";
  "
  [
    (set_attr "cc" "clobber")
  ]
)

(define_expand "fractuns<mode>qi2_32"
  [(set (match_operand:QI    0 "pic30_register_operand"    "=r")
        (unsigned_fract_convert:QI
          (match_operand:ACC 1 "pic30_accum_or_reg_operand" "rw")))]
  ""
  "
  { rtx r0,r1;

    r0 = operands[0];
    r1 = operands[1];
    if (!pic30_register_operand(r0,QImode)) {
      r0 = gen_reg_rtx(QImode);
    }
    if (pic30_fp_round_p() == pic30_convergent) {
      if (!pic30_accum_or_reg_operand(r1,<MODE>mode)) {
        r1 = force_reg(<MODE>mode, r1);
      }
      emit(
        gen_fractuns<mode>qi2_convergent_32(r0,r1)
      );
    } else {
      if (!pic30_register_operand(r1,VOIDmode)) {
        r1 = force_reg(<MODE>mode, r1);
      }
      emit(
        gen_fractuns<mode>qi2_helper_32(r0,r1)
      );
    }
    if (r0 != operands[0]) {
      emit_move_insn(operands[0],r0);
    }
    DONE;
  }
  "
)

(define_insn "fractuns<mode>hi2_helper_32"
  [(set (match_operand:HI    0 "pic30_register_operand"   "=&r,r")
        (unsigned_fract_convert:HI
          (match_operand:ACC 1 "pic30_accum_or_reg_operand" "r,w")))
  ]
  ""
  "*
  {
     gcc_assert(GET_MODE_FBIT(<MODE>mode) == 31);
     if ((pic30_fp_round_p() == pic30_truncation) ||
         (pic30_fp_round_p() == pic30_fastest)) {
       /* chop the low order bits; remember with 9.31 the lowest order  */
       /*  integer bit is in the 15th bit of 2nd word */
       if (which_alternative == 0)
         return \"ze %t1,%0\;\"
                \"sl %d1,[w15]\;\"
                \"rlc %0,%0\";
       else 
         return \"mov %m1U,%0\;\"
                \"ze %0,%0\;\"
                \"sl %0,%0\;\"
                \"btsc %m1H\;\"
                \"inc %0,%0\;\";
     } else if (pic30_fp_round_p() == pic30_conventional) {
       /* [-1,-.5] round to -1, (-.5,.5) round to 0, otherwise 1? */
       if (which_alternative == 0)
         return \"ze %t1,%0\;\"
                \"sl %d1,[w15]\;\"
                \"rlc %0,%0\;\"
                \"btst.c %d1,#14\;\"
                \"addc %0,#0,%0\";
       else 
         return \"mov %m1U,%0\;\"
                \"ze %0,%0\;\"
                \"sl %0,%0\;\"
                \"btsc %m1H,#15\;\"
                \"inc %0,%0\;\"
                \"btsc %m1H,#14\;\"
                \"inc %0,%0\;\";
     } else {
       error(\"Invalid fixed-point rounding mode specified\");
     }
     gcc_assert(0);
     return \"nop\";
  }"
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "fractuns<mode>hi2_convergent_32"
  [(set (match_operand:HI     0 "pic30_register_operand" "=&r")
        (unsigned_fract_convert:HI
          (match_operand:ACC  1 "pic30_register_operand"   "r")))
   (clobber (match_scratch:HI 2                           "=&r"))
  ]
  ""
  "*
   return \"ze %t1,%0\;\"
          \"sl %d1,%2\;\"
          \"rlc %0,%0\;\"
          \"sl %2,%2\;\"
          \"ior %2,%1,[w15]\;\"
          \"btsc SR,#0\;\"   /* carry clear? */
          \"bset SR,#0\;\"
          \"btsc SR,#1\;\"
          \"btst.c %0,#0\;\"
          \"addc %0,#0,%0\";
  "
  [
    (set_attr "cc" "clobber")
  ]
)

(define_expand "fractuns<mode>hi2_32"
  [(set (match_operand:HI    0 "pic30_register_operand"    "=r")
        (unsigned_fract_convert:HI
          (match_operand:ACC 1 "pic30_accum_or_reg_operand" "rw")))]
  ""
  "
  { rtx r0,r1;

    r0 = operands[0];
    r1 = operands[1];
    if (!pic30_register_operand(r0,HImode)) {
      r0 = gen_reg_rtx(HImode);
    }
    if (pic30_fp_round_p() == pic30_convergent) {
      if (!pic30_accum_or_reg_operand(r1,<MODE>mode)) {
        r1 = force_reg(<MODE>mode, r1);
      }
      emit(
        gen_fractuns<mode>hi2_convergent_32(r0,r1)
      );
    } else {
      if (!pic30_register_operand(r1,VOIDmode)) {
        r1 = force_reg(<MODE>mode, r1);
      }
      emit(
        gen_fractuns<mode>hi2_helper_32(r0,r1)
      );
    }
    if (r0 != operands[0]) {
      emit_move_insn(operands[0],r0);
    }
    DONE;
  }
  "
)

(define_insn "fractuns<mode>qi2_helper_32"
  [(set (match_operand:QI     0 "pic30_register_operand"   "=&r,r")
        (unsigned_fract_convert:QI
          (match_operand:UACC 1 "pic30_accum_or_reg_operand" "r,w")))
  ]
  ""
  "*
  {
     gcc_assert(GET_MODE_FBIT(<MODE>mode) == 31);
     if ((pic30_fp_round_p() == pic30_truncation) ||
         (pic30_fp_round_p() == pic30_fastest)) {
       /* chop the low order bits; remember with 9.31 the lowest order  */
       /*  integer bit is in the 15th bit of 2nd word */
       if (which_alternative == 0)
         return \"sl %d1,[w15]\;\"
                \"rlc %t1,%0\";
       else 
         return \"mov %m1U,%0\;\"
                \"sl %0,%0\;\"
                \"btsc %m1H\;\"
                \"inc %0,%0\";
     } else if (pic30_fp_round_p() == pic30_conventional) {
       /* [-1,-.5] round to -1, (-.5,.5) round to 0, otherwise 1? */
       if (which_alternative == 0)
         return \"sl %d1,[w15]\;\"
                \"rlc %t1,%0\;\"
                \"btst.c %d1,#14\;\"
                \"addc.b %0,#0,%0\";
       else 
         return \"mov %m1U,%0\;\"
                \"sl %0,%0\;\"
                \"btsc %m1H,#15\;\"
                \"inc %0,%0\;\"
                \"btsc %m1H,#14\;\"
                \"inc %0,%0\;\";
     } else {
       error(\"Invalid fixed-point rounding mode specified\");
     }
     gcc_assert(0);
     return \"nop\";
  }"
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "fractuns<mode>qi2_convergent_32"
  [(set (match_operand:QI      0 "pic30_register_operand" "=&r")
        (unsigned_fract_convert:QI
          (match_operand:UACC  1 "pic30_register_operand"   "r")))
   (clobber (match_scratch:HI 2                           "=&r"))
  ]
  ""
  "sl %d1,%2\;rlc %t1,%0\;sl %2,%2\;ior %2,%1,[w15]\;btst.c %d1,#14\;btsc SR,#1\;bclr _SR,#0\;addc %0,#0,%0"
  [
    (set_attr "cc" "clobber")
  ]
)

(define_expand "fractuns<mode>qi2_32"
  [(set (match_operand:QI     0 "pic30_register_operand"    "=r")
        (unsigned_fract_convert:QI
          (match_operand:UACC 1 "pic30_accum_or_reg_operand" "rw")))]
  ""
  "
  { rtx r0,r1;

    r0 = operands[0];
    r1 = operands[1];
    if (!pic30_register_operand(r0,QImode)) {
      r0 = gen_reg_rtx(QImode);
    }
    if (pic30_fp_round_p() == pic30_convergent) {
      if (!pic30_accum_or_reg_operand(r1,<MODE>mode)) {
        r1 = force_reg(<MODE>mode, r1);
      }
      emit(
        gen_fractuns<mode>qi2_convergent_32(r0,r1)
      );
    } else {
      if (!pic30_register_operand(r1,VOIDmode)) {
        r1 = force_reg(<MODE>mode, r1);
      }
      emit(
        gen_fractuns<mode>qi2_helper_32(r0,r1)
      );
    }
    if (r0 != operands[0]) {
      emit_move_insn(operands[0],r0);
    }
    DONE;
  }
  "
)

(define_insn "fractuns<mode>hi2_helper_32"
  [(set (match_operand:HI     0 "pic30_register_operand"   "=&r,r")
        (unsigned_fract_convert:HI
          (match_operand:UACC 1 "pic30_accum_or_reg_operand" "r,w")))
  ]
  ""
  "*
  {
     gcc_assert(GET_MODE_FBIT(<MODE>mode) == 31);
     if ((pic30_fp_round_p() == pic30_truncation) ||
         (pic30_fp_round_p() == pic30_fastest)) {
       /* chop the low order bits; remember with 9.31 the lowest order  */
       /*  integer bit is in the 15th bit of 2nd word */
       if (which_alternative == 0)
         return \"ze %t1,%0\;\"
                \"sl %d1,[w15]\;\"
                \"rlc %0,%0\;\";
       else 
         return \"mov %m1U,%0\;\"
                \"ze %0,%0\;\"
                \"sl %0,%0\;\"
                \"btsc %m1H\;\"
                \"inc %0,%0\;\";
     } else if (pic30_fp_round_p() == pic30_conventional) {
       /* [-1,-.5] round to -1, (-.5,.5) round to 0, otherwise 1? */
       if (which_alternative == 0)
         return \"ze %t1,%0\;\"
                \"sl %d1,[w15]\;\"
                \"rlc %0,%0\;\"
                \"btst.c %d1,#14\;\"
                \"addc %0,#0,%0\";
       else 
         return \"mov %m1U,%0\;\"
                \"ze %0,%0\;\"
                \"sl %0,%0\;\"
                \"btsc %m1H,#15\;\"
                \"inc %0,%0\;\"
                \"btsc %m1H,#14\;\"
                \"inc %0,%0\;\";
     } else {
       error(\"Invalid fixed-point rounding mode specified\");
     }
     gcc_assert(0);
     return \"nop\";
  }"
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "fractuns<mode>hi2_convergent_32"
  [(set (match_operand:HI      0 "pic30_register_operand" "=&r")
        (unsigned_fract_convert:HI
          (match_operand:UACC  1 "pic30_register_operand"   "r")))
   (clobber (match_scratch:HI  2                           "=&r"))
  ]
  ""
  "*
   return \"ze %t1,%0\;\"
          \"sl %d1,%2\;\"
          \"rlc %0,%0\;\"
          \"sl %2,%2\;\"
          \"ior %2,%1,[w15]\;\"
          \"btsc SR,#0\;\"   /* carry clear? */
          \"bset SR,#0\;\"
          \"btsc SR,#1\;\"
          \"btst.c %0,#0\;\"
          \"addc %0,#0,%0\";
  "
  [
    (set_attr "cc" "clobber")
  ]
)

(define_expand "fractuns<mode>hi2_32"
  [(set (match_operand:HI     0 "pic30_register_operand"    "=r")
        (unsigned_fract_convert:HI
          (match_operand:UACC 1 "pic30_accum_or_reg_operand" "rw")))]
  ""
  "
  { rtx r0,r1;

    r0 = operands[0];
    r1 = operands[1];
    if (!pic30_register_operand(r0,HImode)) {
      r0 = gen_reg_rtx(HImode);
    }
    if (pic30_fp_round_p() == pic30_convergent) {
      if (!pic30_accum_or_reg_operand(r1,<MODE>mode)) {
        r1 = force_reg(<MODE>mode, r1);
      }
      emit(
        gen_fractuns<mode>hi2_convergent_32(r0,r1)
      );
    } else {
      if (!pic30_register_operand(r1,VOIDmode)) {
        r1 = force_reg(<MODE>mode, r1);
      }
      emit(
        gen_fractuns<mode>hi2_helper_32(r0,r1)
      );
    }
    if (r0 != operands[0]) {
      emit_move_insn(operands[0],r0);
    }
    DONE;
  }
  "
)

; To SI and DI done by function only

; to Q15

(define_insn "fractunsqi<mode>2_32"
  [(set (match_operand:Q15  0 "pic30_register_operand"  "=&r")
        (unsigned_fract_convert:Q15
          (match_operand:QI 1 "pic30_register_operand"  "r")))]
  " pic30_fp_inline_p()"
  "*
   {  
      return 
             \"cp0.b %1\;\"        /* compare input with 0 */
             \"mov #0x8000,%0\;\"  /* assume -1 */
             \"btsc SR,#0\;\"     /* positive => 0 or .99 */
             \"mov #0x7FFF,%0\;\"  /*   load .99... */
             \"btsc SR,#1\;\"     /* if zero */
             \"clr %0\";           /*   load zero */
   }
  "
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "fractunshi<mode>2_32"
  [(set (match_operand:Q15  0 "pic30_register_operand"  "=&r")
        (unsigned_fract_convert:Q15
          (match_operand:HI 1 "pic30_register_operand"  "r")))]
  " pic30_fp_inline_p()"
  "*
   {
      return 
             \"cp0  %1\;\"         /* compare input with 0 */
             \"mov #0x8000,%0\;\"  /* assume -1 */
             \"btsc SR,#0\;\"     /* positive => 0 or .99 */
             \"mov #0x7FFF,%0\;\"  /*   load .99... */
             \"btsc SR,#1\;\"     /* if zero */
             \"clr %0\";           /*   load zero */
   }
  "
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "fractunssi<mode>2_32"
  [(set (match_operand:Q15  0 "pic30_register_operand"  "=&r")
        (unsigned_fract_convert:Q15
          (match_operand:SI 1 "pic30_register_operand"  "r")))]
  " pic30_fp_inline_p()"
  "*
   {  
      return 
             \"sub %1,#0,[w15]\;\"   /* compare input with 0 */
             \"subb %d1,#0,[w15]\;\" /* compare input with 0 */
             \"mov #0x8000,%0\;\"    /* assume -1 */
             \"btsc SR,#0\;\"       /* positive => 0 or .99 */
             \"mov #0x7FFF,%0\;\"    /*   load .99... */
             \"btsc SR,#1\;\"       /* if zero */
             \"clr %0\";             /*   load zero */
   }
  "
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "fractunsdi<mode>2_32"
  [(set (match_operand:Q15  0 "pic30_register_operand"  "=&r")
        (unsigned_fract_convert:Q15
          (match_operand:DI 1 "pic30_register_operand"  "r")))]
  " pic30_fp_inline_p()"
  "*
   {  
      return 
             \"sub %1,#0,[w15]\;\"   /* compare input with 0 */
             \"subb %d1,#0,[w15]\;\" /* compare input with 0 */
             \"subb %t1,#0,[w15]\;\" /* compare input with 0 */
             \"subb %q1,#0,[w15]\;\" /* compare input with 0 */
             \"mov #0x8000,%0\;\"    /* assume -1 */
             \"btsc SR,#0\;\"       /* positive => 0 or .99 */
             \"mov #0x7FFF,%0\;\"    /*   load .99... */
             \"btsc SR,#1\;\"       /* if zero */
             \"clr %0\";             /*   load zero */
   }
  "
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "fractunsqi<mode>2_32"
  [(set (match_operand:UQ15  0 "pic30_register_operand"  "=&r")
        (unsigned_fract_convert:UQ15
          (match_operand:QI  1 "pic30_register_operand"  "r")))]
  ""
  "*
   {  
      return 
             \"cp0.b %1\;\"        /* compare input with 0 */
             \"mov #0x7FFF,%0\;\"  /* assume ~1 */
             \"btsc SR,#1\;\"     /* if zero */
             \"clr %0\";           /*   load zero */
   }
  "
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "fractunshi<mode>2_32"
  [(set (match_operand:UQ15  0 "pic30_register_operand"  "=&r")
        (unsigned_fract_convert:UQ15
          (match_operand:HI  1 "pic30_register_operand"  "r")))]
  ""
  "*
   {
      return 
             \"cp0.b %1\;\"        /* compare input with 0 */
             \"mov #0x7FFF,%0\;\"  /* assume ~1 */
             \"btsc SR,#1\;\"     /* if zero */
             \"clr %0\";           /*   load zero */
   }
  "
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "fractunssi<mode>2_32"
  [(set (match_operand:UQ15  0 "pic30_register_operand"  "=&r")
        (unsigned_fract_convert:UQ15
          (match_operand:SI  1 "pic30_register_operand"  "r")))]
  ""
  "*
   {  
      return 
             \"sub %1,#0,[w15]\;\"   /* compare input with 0 */
             \"subb %d1,#0,[w15]\;\" /* compare input with 0 */
             \"mov #0x7FFF,%0\;\"    /* assume ~1 */
             \"btsc SR,#1\;\"       /* if zero */
             \"clr %0\";             /*   load zero */
   }
  "
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "fractunsdi<mode>2_32"
  [(set (match_operand:UQ15  0 "pic30_register_operand"  "=&r")
        (unsigned_fract_convert:UQ15
          (match_operand:DI  1 "pic30_register_operand"  "r")))]
  ""
  "*
   {  
      return 
             \"sub %1,#0,[w15]\;\"   /* compare input with 0 */
             \"subb %d1,#0,[w15]\;\" /* compare input with 0 */
             \"subb %t1,#0,[w15]\;\" /* compare input with 0 */
             \"subb %q1,#0,[w15]\;\" /* compare input with 0 */
             \"mov #0x7FFF,%0\;\"    /*   load .99... */
             \"btsc SR,#1\;\"       /* if zero */
             \"clr %0\";             /*   load zero */
   }
  "
  [
    (set_attr "cc" "clobber")
  ]
)

; to Q31

(define_insn "fractunsqi<mode>2_32"
  [(set (match_operand:Q31  0 "pic30_register_operand"  "=&r")
        (unsigned_fract_convert:Q31
          (match_operand:QI 1 "pic30_register_operand"  "r")))]
  " pic30_fp_inline_p()"
  "*
   {  
      return 
             \"cp0.b %1\;\"         /* compare input with 0 */
             \"mov #0x8000,%d0\;\"  /* assume -1 */
             \"btsc SR,#0\;\"      /* positive => 0 or .99 */
             \"mov #0x7FFF,%d0\;\"  /*   load .99... */
             \"btsc SR,#1\;\"      /* if zero */
             \"clr %d0\;\"          /*   load zero */
             \"sl  %d0,%0\;\"       /* form low word by propagating non sign */
             \"asr %0,#15,%0\";     /*   bits into low word */
   }
  "
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "fractunshi<mode>2_32"
  [(set (match_operand:Q31  0 "pic30_register_operand"  "=&r")
        (unsigned_fract_convert:Q31
          (match_operand:HI 1 "pic30_register_operand"  "r")))]
  " pic30_fp_inline_p()"
  "*
   {
      return 
             \"cp0.b %1\;\"         /* compare input with 0 */
             \"mov #0x8000,%d0\;\"  /* assume -1 */
             \"btsc SR,#0\;\"      /* positive => 0 or .99 */
             \"mov #0x7FFF,%d0\;\"  /*   load .99... */
             \"btsc SR,#1\;\"      /* if zero */
             \"clr %d0\;\"          /*   load zero */
             \"sl  %d0,%0\;\"       /* form low word by propagating non sign */
             \"asr %0,#15,%0\";     /*   bits into low word */
   }
  "
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "fractunssi<mode>2_32"
  [(set (match_operand:Q31  0 "pic30_register_operand"  "=&r")
        (unsigned_fract_convert:Q31
          (match_operand:SI 1 "pic30_register_operand"  "r")))]
  " pic30_fp_inline_p()"
  "*
   {  
      return 
             \"sub %1,#0,[w15]\;\"   /* compare input with 0 */
             \"subb %d1,#0,[w15]\;\" /* compare input with 0 */
             \"mov #0x8000,%d0\;\"   /* assume -1 */
             \"btsc SR,#0\;\"       /* positive => 0 or .99 */
             \"mov #0x7FFF,%d0\;\"   /*   load .99... */
             \"btsc SR,#1\;\"       /* if zero */
             \"clr %d0\;\"           /*   load zero */
             \"sl  %d0,%0\;\"        /* form low word by propagating non sign */
             \"asr %0,#15,%0\";      /*   bits into low word */
   }
  "
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "fractunsdi<mode>2_32"
  [(set (match_operand:Q31  0 "pic30_register_operand"  "=&r")
        (unsigned_fract_convert:Q31
          (match_operand:DI 1 "pic30_register_operand"  "r")))]
  " pic30_fp_inline_p()"
  "*
   {  
      return 
             \"sub %1,#0,[w15]\;\"   /* compare input with 0 */
             \"subb %d1,#0,[w15]\;\" /* compare input with 0 */
             \"subb %t1,#0,[w15]\;\" /* compare input with 0 */
             \"subb %q1,#0,[w15]\;\" /* compare input with 0 */
             \"mov #0x8000,%d0\;\"   /* assume -1 */
             \"btsc SR,#0\;\"       /* positive => 0 or .99 */
             \"mov #0x7FFF,%d0\;\"   /*   load .99... */
             \"btsc SR,#1\;\"       /* if zero */
             \"clr %d0\;\"           /*   load zero */
             \"sl  %d0,%0\;\"        /* form low word by propagating non sign */
             \"asr %0,#15,%0\";      /*   bits into low word */
   }
  "
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "fractunsqi<mode>2_32"
  [(set (match_operand:UQ31  0 "pic30_register_operand"  "=&r")
        (unsigned_fract_convert:UQ31
          (match_operand:QI  1 "pic30_register_operand"  "r")))]
  ""
  "*
   {  
      return 
             \"cp0.b %1\;\"         /* compare input with 0 */
             \"mov #0x7FFF,%d0\;\"  /*   load .99... */
             \"setm %0\;\"
             \"btsc SR,#1\;\"      /* if zero */
             \"mul.uu %0,#0,%0\";   /*   load zero */
   }
  "
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "fractunshi<mode>2_32"
  [(set (match_operand:UQ31  0 "pic30_register_operand"  "=&r")
        (unsigned_fract_convert:UQ31
          (match_operand:HI  1 "pic30_register_operand"  "r")))]
  ""
  "*
   {
      return 
             \"cp0.b %1\;\"         /* compare input with 0 */
             \"mov #0x7FFF,%d0\;\"  /*   load .99... */
             \"setm %0\;\"
             \"btsc SR,#1\;\"      /* if zero */
             \"mul.uu %0,#0,%0\";   /*   load zero */
   }
  "
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "fractunssi<mode>2_32"
  [(set (match_operand:UQ31  0 "pic30_register_operand"  "=&r")
        (unsigned_fract_convert:UQ31
          (match_operand:SI  1 "pic30_register_operand"  "r")))]
  ""
  "*
   {  
      return 
             \"sub %1,#0,[w15]\;\"   /* compare input with 0 */
             \"subb %d1,#0,[w15]\;\" /* compare input with 0 */
             \"mov #0x7FFF,%d0\;\"   /*   load .99... */
             \"setm %0\;\"
             \"btsc SR,#1\;\"       /* if zero */
             \"mul.uu %0,#0,%0\";    /*   load zero */
   }
  "
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "fractunsdi<mode>2_32"
  [(set (match_operand:UQ31  0 "pic30_register_operand"  "=&r")
        (unsigned_fract_convert:UQ31
          (match_operand:DI  1 "pic30_register_operand"  "r")))]
  ""
  "*
   {  
      return 
             \"sub %1,#0,[w15]\;\"   /* compare input with 0 */
             \"subb %d1,#0,[w15]\;\" /* compare input with 0 */
             \"subb %t1,#0,[w15]\;\" /* compare input with 0 */
             \"subb %q1,#0,[w15]\;\" /* compare input with 0 */
             \"mov #0x7FFF,%d0\;\"   /*   load .99... */
             \"setm %0\;\"
             \"btsc SR,#1\;\"       /* if zero */
             \"mul.uu %0,#0,%0\";    /*   load zero */
   }
  "
  [
    (set_attr "cc" "clobber")
  ]
)

; to ACC

(define_insn "fractunsqi<mode>2_32"
  [(set (match_operand:ACC  0 "pic30_register_operand"  "=&r")
        (unsigned_fract_convert:ACC
          (match_operand:QI 1 "pic30_register_operand"    "r")))]
  " pic30_fp_inline_p()"
  "*
   {  
      return 
             \"mul.uu %0,#0,%0\;\"  /* clear low bits */
             \"mov.b %1,%t0\;\"     /* copy integer portion into upper byte */
             \"asr.b %t0,%t0\;\"    /* shift it into position */
             \"rrc %d0,%d0\";
   }
  "
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "fractunshi<mode>2_32"
  [(set (match_operand:ACC  0 "pic30_register_operand"  "=&r")
        (unsigned_fract_convert:ACC
          (match_operand:HI 1 "pic30_register_operand"  "r")))]
  " pic30_fp_inline_p()"
  "*
   {
      return 
             \"mul.uu %0,#0,%0\;\"  /* clear low bits */
             \"mov %1,%t0\;\"       /* copy low integer portion to upper byte */
             \"asr.b %t0,%t0\;\"    /* shift it into position */
             \"rrc %d0,%d0\";
   }
  "
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "fractunssi<mode>2_32"
  [(set (match_operand:ACC  0 "pic30_register_operand"  "=&r")
        (unsigned_fract_convert:ACC
          (match_operand:SI 1 "pic30_register_operand"  "r")))]
  " pic30_fp_inline_p()"
  "*
   {  
      return 
             \"mul.uu %0,#0,%0\;\"  /* clear low bits */
             \"mov %1,%t0\;\"       /* copy low integer portion to upper byte */
             \"asr.b %t0,%t0\;\"    /* shift it into position */
             \"rrc %d0,%d0\";
   }
  "
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "fractunsdi<mode>2_32"
  [(set (match_operand:ACC  0 "pic30_register_operand"  "=&r")
        (unsigned_fract_convert:ACC
          (match_operand:DI 1 "pic30_register_operand"  "r")))]
  " pic30_fp_inline_p()"
  "*
   {  
      return 
             \"mul.uu %0,#0,%0\;\"  /* clear low bits */
             \"mov %1,%t0\;\"       /* copy low integer portion to upper byte */
             \"asr.b %t0,%t0\;\"    /* shift it into position */
             \"rrc %d0,%d0\";
   }
  "
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "fractunsqi<mode>2_32"
  [(set (match_operand:UACC  0 "pic30_register_operand"  "=&r")
        (unsigned_fract_convert:UACC
          (match_operand:QI  1 "pic30_register_operand"  "r")))]
  ""
  "*
   {  
      return 
             \"mul.uu %0,#0,%0\;\"  /* clear low bits */
             \"mov.b %1,%t0\;\"     /* copy integer portion into upper byte */
             \"asr.b %t0,%t0\;\"    /* shift it into position */
             \"rrc %d0,%d0\";
   }
  "
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "fractunshi<mode>2_32"
  [(set (match_operand:UACC  0 "pic30_register_operand"  "=&r")
        (unsigned_fract_convert:UACC
          (match_operand:HI  1 "pic30_register_operand"  "r")))]
  ""
  "*
   {
      return 
             \"mul.uu %0,#0,%0\;\"  /* clear low bits */
             \"mov %1,%t0\;\"       /* copy low integer portion to upper byte */
             \"asr.b %t0,%t0\;\"    /* shift it into position */
             \"rrc %d0,%d0\";
   }
  "
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "fractunssi<mode>2_32"
  [(set (match_operand:UACC  0 "pic30_register_operand"  "=&r")
        (unsigned_fract_convert:UACC
          (match_operand:SI  1 "pic30_register_operand"  "r")))]
  ""
  "*
   {  
      return 
             \"mul.uu %0,#0,%0\;\"  /* clear low bits */
             \"mov %1,%t0\;\"       /* copy low integer portion to upper byte */
             \"asr.b %t0,%t0\;\"    /* shift it into position */
             \"rrc %d0,%d0\";
   }
  "
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "fractunsdi<mode>2_32"
  [(set (match_operand:UACC  0 "pic30_register_operand"  "=&r")
        (unsigned_fract_convert:UACC
          (match_operand:DI  1 "pic30_register_operand"  "r")))]
  ""
  "*
   {  
      return 
             \"mul.uu %0,#0,%0\;\"  /* clear low bits */
             \"mov %1,%t0\;\"       /* copy low integer portion to upper byte */
             \"asr.b %t0,%t0\;\"    /* shift it into position */
             \"rrc %d0,%d0\";
   }
  "
  [
    (set_attr "cc" "clobber")
  ]
)


;; conversions back and forth to floating point or double will be done by
;;  out of line functions only

; conversions to and from fractional types
;; awkward - we can only move through one iterator, these are all 
;;   coppies of each other with a different source mode

(define_insn "fractsq<mode>2_helper_32"
  [(set (match_operand:QUQ15  0 "pic30_register_operand" "=&r")
        (fract_convert:QUQ15
          (match_operand:SQ 1 "pic30_register_operand" "r")))]
  ""
  "*
  {
    if ((pic30_fp_round_p() == pic30_truncation) ||
        (pic30_fp_round_p() == pic30_fastest)) {
      return \"mov %d1,%0\";
    } else if (pic30_fp_round_p() == pic30_conventional) {
      return \"sl %1,[w15]\;\"
             \"addc %d1,#0,%0\";
    } else {
       error(\"Invalid fixed-point rounding mode specified\");
    }
     gcc_assert(0);
     return \"nop\";
  }
  "
  [
    (set_attr "cc" "math")
  ]
)

(define_insn "fractsq<mode>2_convergent_32"
  [(set (match_operand:QUQ15  0 "pic30_register_operand" "=&r")
        (fract_convert:QUQ15
          (match_operand:SQ   1 "pic30_register_operand" "r")))
   (clobber (match_scratch:HI 2                          "=&r"))
  ]
  ""
  "*
  {
    if (pic30_fp_round_p() == pic30_convergent) {
      return \"setm %2\;\"
             \"sl %1,[w15]\;\"          /* get top bit into carry */
             \"btsc SR,#1\;\"          /* if the result is 0 */
             \"mov #0xFFFE,%2\;\"       /*   mask off low bit */
             \"addc %d1,#0,%0\;\"       /* add in 'overflow' */
             \"and %0,%2,%0\";          /* maybe mask off low bit */
    } else {
       error(\"Invalid fixed-point rounding mode specified\");
    }
     gcc_assert(0);
     return \"nop\";
  }
  "
  [
    (set_attr "cc" "math")
  ]
)

(define_expand "fractsq<mode>2_32"
  [(set (match_operand:QUQ15 0 "pic30_register_operand"  "=r")
        (fract_convert:QUQ15
          (match_operand:SQ  1 "pic30_register_operand"  "r")))]
  ""
  "
  { rtx r0,r1;

    r0 = operands[0];
    r1 = operands[1];
    if (!pic30_register_operand(r0,SImode)) {
      r0 = gen_reg_rtx(<MODE>mode);
    }
    if (!pic30_register_operand(r1,VOIDmode)) {
      r1 = force_reg(<MODE>mode, r1);
    }
    if (pic30_fp_round_p() == pic30_convergent) {
      emit(
        gen_fractsq<mode>2_convergent_32(r0,r1)
      );
    } else {
      emit(
        gen_fractsq<mode>2_helper_32(r0,r1)
      );
    }
    if (r0 != operands[0]) {
      emit_move_insn(operands[0],r0);
    }
    DONE;
  }
  "
)

(define_insn "fractusq<mode>2_helper_32"
  [(set (match_operand:QUQ15 0 "pic30_register_operand" "=&r")
        (fract_convert:QUQ15
          (match_operand:USQ 1 "pic30_register_operand" "r")))]
  ""
  "*
  {
    if ((pic30_fp_round_p() == pic30_truncation) ||
        (pic30_fp_round_p() == pic30_fastest)) {
      return \"mov %d1,%0\";
    } else if (pic30_fp_round_p() == pic30_conventional) {
      return \"sl %1,[w15]\;\"
             \"addc %d1,#0,%0\";
    } else {
       error(\"Invalid fixed-point rounding mode specified\");
    }
     gcc_assert(0);
     return \"nop\";
  }
  "
  [
    (set_attr "cc" "math")
  ]
)

(define_insn "fractusq<mode>2_convergent_32"
  [(set (match_operand:QUQ15  0 "pic30_register_operand" "=&r")
        (fract_convert:QUQ15
          (match_operand:USQ  1 "pic30_register_operand" "r")))
   (clobber (match_scratch:HI 2                          "=&r"))
  ]
  ""
  "*
  {
    if (pic30_fp_round_p() == pic30_convergent) {
      return \"setm %2\;\"
             \"sl %1,[w15]\;\"          /* get top bit into carry */
             \"btsc SR,#1\;\"          /* if the result is 0 */
             \"mov #0xFFFE,%2\;\"       /*   mask off low bit */
             \"addc %d1,#0,%0\;\"       /* add in 'overflow' */
             \"and %0,%2,%0\";          /* maybe mask off low bit */
    } else {
       error(\"Invalid fixed-point rounding mode specified\");
    }
     gcc_assert(0);
     return \"nop\";
  }
  "
  [
    (set_attr "cc" "math")
  ]
)

(define_expand "fractusq<mode>2_32"
  [(set (match_operand:QUQ15 0 "pic30_register_operand"  "=r")
        (fract_convert:QUQ15
          (match_operand:USQ 1 "pic30_register_operand"  "r")))]
  ""
  "
  { rtx r0,r1;

    r0 = operands[0];
    r1 = operands[1];
    if (!pic30_register_operand(r0,SImode)) {
      r0 = gen_reg_rtx(<MODE>mode);
    }
    if (!pic30_register_operand(r1,VOIDmode)) {
      r1 = force_reg(<MODE>mode, r1);
    }
    if (pic30_fp_round_p() == pic30_convergent) {
      emit(
        gen_fractusq<mode>2_convergent_32(r0,r1)
      );
    } else {
      emit(
        gen_fractusq<mode>2_helper_32(r0,r1)
      );
    }
    if (r0 != operands[0]) {
      emit_move_insn(operands[0],r0);
    }
    DONE;
  }
  "
)

(define_insn "fracttq<mode>2_helper_32"
  [(set (match_operand:QUQ15 0 "pic30_register_operand" "=&r")
        (fract_convert:QUQ15
          (match_operand:TQ  1 "pic30_register_operand" "r")))]
  ""
  "*
  {
    if ((pic30_fp_round_p() == pic30_truncation) ||
        (pic30_fp_round_p() == pic30_fastest)) {
      return \"mov %d1,%0\";
    } else if (pic30_fp_round_p() == pic30_conventional) {
      return \"sl %1,[w15]\;\"
             \"addc %d1,#0,%0\";
    } else {
       error(\"Invalid fixed-point rounding mode specified\");
    }
     gcc_assert(0);
     return \"nop\";
  }
  "
  [
    (set_attr "cc" "math")
  ]
)

(define_insn "fracttq<mode>2_convergent_32"
  [(set (match_operand:QUQ15  0 "pic30_register_operand" "=&r")
        (fract_convert:QUQ15
          (match_operand:TQ  1 "pic30_register_operand" "r")))
   (clobber (match_scratch:HI 2                          "=&r"))
  ]
  ""
  "*
  {
    if (pic30_fp_round_p() == pic30_convergent) {
      return \"setm %2\;\"
             \"sl %1,[w15]\;\"          /* get top bit into carry */
             \"btsc SR,#1\;\"          /* if the result is 0 */
             \"mov #0xFFFE,%2\;\"       /*   mask off low bit */
             \"addc %d1,#0,%0\;\"       /* add in 'overflow' */
             \"and %0,%2,%0\";          /* maybe mask off low bit */
    } else {
       error(\"Invalid fixed-point rounding mode specified\");
    }
     gcc_assert(0);
     return \"nop\";
  }
  "
  [
    (set_attr "cc" "math")
  ]
)

(define_expand "fracttq<mode>2_32"
  [(set (match_operand:QUQ15 0 "pic30_register_operand"  "=r")
        (fract_convert:QUQ15
          (match_operand:TQ 1 "pic30_register_operand"  "r")))]
  ""
  "
  { rtx r0,r1;

    if (!pic30_fp_inline_p() && (pic30_fp_round_p() == pic30_convergent))
      FAIL;
    r0 = operands[0];
    r1 = operands[1];
    if (!pic30_register_operand(r0,SImode)) {
      r0 = gen_reg_rtx(<MODE>mode);
    }
    if (!pic30_register_operand(r1,VOIDmode)) {
      r1 = force_reg(<MODE>mode, r1);
    }
    if (pic30_fp_round_p() == pic30_convergent) {
      emit(
        gen_fracttq<mode>2_convergent_32(r0,r1)
      );
    } else {
      emit(
        gen_fracttq<mode>2_helper_32(r0,r1)
      );
    }
    if (r0 != operands[0]) {
      emit_move_insn(operands[0],r0);
    }
    DONE;
  }
  "
)

(define_insn "fractutq<mode>2_helper_32"
  [(set (match_operand:QUQ15 0 "pic30_register_operand" "=&r")
        (fract_convert:QUQ15
          (match_operand:UTQ 1 "pic30_register_operand" "r")))]
  ""
  "*
  {
    if ((pic30_fp_round_p() == pic30_truncation) ||
        (pic30_fp_round_p() == pic30_fastest)) {
      return \"mov %d1,%0\";
    } else if (pic30_fp_round_p() == pic30_conventional) {
      return \"sl %1,[w15]\;\"
             \"addc %d1,#0,%0\";
    } else {
       error(\"Invalid fixed-point rounding mode specified\");
    }
     gcc_assert(0);
     return \"nop\";
  }
  "
  [
    (set_attr "cc" "math")
  ]
)

(define_insn "fractutq<mode>2_convergent_32"
  [(set (match_operand:QUQ15  0 "pic30_register_operand" "=&r")
        (fract_convert:QUQ15
          (match_operand:UTQ  1 "pic30_register_operand" "r")))
   (clobber (match_scratch:HI 2                          "=&r"))
  ]
  ""
  "*
  {
    if (pic30_fp_round_p() == pic30_convergent) {
      return \"setm %2\;\"
             \"sl %1,[w15]\;\"          /* get top bit into carry */
             \"btsc SR,#1\;\"          /* if the result is 0 */
             \"mov #0xFFFE,%2\;\"       /*   mask off low bit */
             \"addc %d1,#0,%0\;\"       /* add in 'overflow' */
             \"and %0,%2,%0\";          /* maybe mask off low bit */
    } else {
       error(\"Invalid fixed-point rounding mode specified\");
    }
     gcc_assert(0);
     return \"nop\";
  }
  "
  [
    (set_attr "cc" "math")
  ]
)

(define_expand "fractutq<mode>2_32"
  [(set (match_operand:QUQ15 0 "pic30_register_operand"  "=r")
        (fract_convert:QUQ15
          (match_operand:UTQ 1 "pic30_register_operand"  "r")))]
  ""
  "
  { rtx r0,r1;

    if (!pic30_fp_inline_p() && (pic30_fp_round_p() == pic30_convergent))
      FAIL;
    r0 = operands[0];
    r1 = operands[1];
    if (!pic30_register_operand(r0,SImode)) {
      r0 = gen_reg_rtx(<MODE>mode);
    }
    if (!pic30_register_operand(r1,VOIDmode)) {
      r1 = force_reg(<MODE>mode, r1);
    }
    if (pic30_fp_round_p() == pic30_convergent) {
      emit(
        gen_fracttq<mode>2_convergent_32(r0,r1)
      );
    } else {
      emit(
        gen_fracttq<mode>2_helper_32(r0,r1)
      );
    }
    if (r0 != operands[0]) {
      emit_move_insn(operands[0],r0);
    }
    DONE;
  }
  "
)

(define_insn "fractdq<mode>2_helper_32"
  [(set (match_operand:QUQ15 0 "pic30_register_operand" "=&r")
        (fract_convert:QUQ15
          (match_operand:DQ  1 "pic30_register_operand" "r")))]
  ""
  "*
  {
    if ((pic30_fp_round_p() == pic30_truncation) ||
        (pic30_fp_round_p() == pic30_fastest)) {
      return \"mov %d1,%0\";
    } else if (pic30_fp_round_p() == pic30_conventional) {
      return \"sl %1,[w15]\;\"
             \"addc %d1,#0,%0\";
    } else {
       error(\"Invalid fixed-point rounding mode specified\");
    }
     gcc_assert(0);
     return \"nop\";
  }
  "
  [
    (set_attr "cc" "math")
  ]
)

(define_insn "fractdq<mode>2_convergent_32"
  [(set (match_operand:QUQ15  0 "pic30_register_operand" "=&r")
        (fract_convert:QUQ15
          (match_operand:DQ   1 "pic30_register_operand" "r")))
   (clobber (match_scratch:HI 2                          "=&r"))
  ]
  ""
  "*
  {
    if (pic30_fp_round_p() == pic30_convergent) {
      return \"setm %2\;\"
             \"sl %1,[w15]\;\"          /* get top bit into carry */
             \"btsc SR,#1\;\"          /* if the result is 0 */
             \"mov #0xFFFE,%2\;\"       /*   mask off low bit */
             \"addc %d1,#0,%0\;\"       /* add in 'overflow' */
             \"and %0,%2,%0\";          /* maybe mask off low bit */
    } else {
       error(\"Invalid fixed-point rounding mode specified\");
    }
     gcc_assert(0);
     return \"nop\";
  }
  "
)

(define_expand "fractdq<mode>2_32"
  [(set (match_operand:QUQ15 0 "pic30_register_operand"  "=r")
        (fract_convert:QUQ15
          (match_operand:DQ  1 "pic30_register_operand"  "r")))]
  ""
  "
  { rtx r0,r1;

    if (!pic30_fp_inline_p() && (pic30_fp_round_p() == pic30_convergent))
      FAIL;
    r0 = operands[0];
    r1 = operands[1];
    if (!pic30_register_operand(r0,SImode)) {
      r0 = gen_reg_rtx(<MODE>mode);
    }
    if (!pic30_register_operand(r1,VOIDmode)) {
      r1 = force_reg(<MODE>mode, r1);
    }
    if (pic30_fp_round_p() == pic30_convergent) {
      emit(
        gen_fractdq<mode>2_convergent_32(r0,r1)
      );
    } else {
      emit(
        gen_fractdq<mode>2_helper_32(r0,r1)
      );
    }
    if (r0 != operands[0]) {
      emit_move_insn(operands[0],r0);
    }
    DONE;
  }
  "
)

(define_insn "fractudq<mode>2_helper_32"
  [(set (match_operand:QUQ15 0 "pic30_register_operand" "=&r")
        (fract_convert:QUQ15
          (match_operand:UDQ 1 "pic30_register_operand" "r")))]
  ""
  "*
  {
    if ((pic30_fp_round_p() == pic30_truncation) ||
        (pic30_fp_round_p() == pic30_fastest)) {
      return \"mov %d1,%0\";
    } else if (pic30_fp_round_p() == pic30_conventional) {
      return \"sl %1,[w15]\;\"
             \"addc %d1,#0,%0\";
    } else {
       error(\"Invalid fixed-point rounding mode specified\");
    }
     gcc_assert(0);
     return \"nop\";
  }
  "
  [
    (set_attr "cc" "math")
  ]
)

(define_insn "fractudq<mode>2_convergent_32"
  [(set (match_operand:QUQ15  0 "pic30_register_operand" "=&r")
        (fract_convert:QUQ15
          (match_operand:UDQ  1 "pic30_register_operand" "r")))
   (clobber (match_scratch:HI 2                          "=&r"))
  ]
  ""
  "*
  {
    if (pic30_fp_round_p() == pic30_convergent) {
      return \"setm %2\;\"
             \"sl %1,[w15]\;\"          /* get top bit into carry */
             \"btsc SR,#1\;\"          /* if the result is 0 */
             \"mov #0xFFFE,%2\;\"       /*   mask off low bit */
             \"addc %d1,#0,%0\;\"       /* add in 'overflow' */
             \"and %0,%2,%0\";          /* maybe mask off low bit */
    } else {
       error(\"Invalid fixed-point rounding mode specified\");
    }
     gcc_assert(0);
     return \"nop\";
  }
  "
  [
    (set_attr "cc" "math")
  ]
)

(define_expand "fractudq<mode>2_32"
  [(set (match_operand:QUQ15 0 "pic30_register_operand"  "=r")
        (fract_convert:QUQ15
          (match_operand:UDQ 1 "pic30_register_operand"   "r")))]
  ""
  "
  { rtx r0,r1;

    if (!pic30_fp_inline_p() && (pic30_fp_round_p() == pic30_convergent))
      FAIL;
    r0 = operands[0];
    r1 = operands[1];
    if (!pic30_register_operand(r0,SImode)) {
      r0 = gen_reg_rtx(<MODE>mode);
    }
    if (!pic30_register_operand(r1,VOIDmode)) {
      r1 = force_reg(<MODE>mode, r1);
    }
    if (pic30_fp_round_p() == pic30_convergent) {
      emit(
        gen_fractudq<mode>2_convergent_32(r0,r1)
      );
    } else {
      emit(
        gen_fractudq<mode>2_helper_32(r0,r1)
      );
    }
    if (r0 != operands[0]) {
      emit_move_insn(operands[0],r0);
    }
    DONE;
  }
  "
)

(define_insn "fractqq<mode>2_32"
  [(set (match_operand:QUQ31 0 "pic30_register_operand" "=r")
        (fract_convert:QUQ31
          (match_operand:QQ  1 "pic30_register_operand"  "r")))]
  ""
  "mov %1,%d0\;clr %0"
  [
    (set_attr "cc" "change0")
  ]
)

(define_insn "fractuqq<mode>2_32"
  [(set (match_operand:QUQ31 0 "pic30_register_operand" "=r")
        (fract_convert:QUQ31
          (match_operand:UQQ 1 "pic30_register_operand"  "r")))]
  ""
  "mov %1,%d0\;clr %0"
  [
    (set_attr "cc" "change0")
  ]
)

(define_insn "fracthq<mode>2_32"
  [(set (match_operand:QUQ31 0 "pic30_register_operand" "=r")
        (fract_convert:QUQ31
          (match_operand:HQ 1 "pic30_register_operand"   "r")))]
  ""
  "mov %1,%d0\;clr %0"
  [
    (set_attr "cc" "change0")
  ]
)

(define_insn "fractuhq<mode>2_32"
  [(set (match_operand:QUQ31 0 "pic30_register_operand" "=r")
        (fract_convert:QUQ31
          (match_operand:UHQ 1 "pic30_register_operand"  "r")))]
  ""
  "mov %1,%d0\;clr %0"
  [
    (set_attr "cc" "change0")
  ]
)

; conversions between signed/unsigned 'self'

(define_insn "fractqquqq2_32"
  [(set (match_operand:UQQ  0 "register_operand" "=r")
       (unsigned_fract_convert:UQQ
         (match_operand:QQ 1 "register_operand" "0")))]
  ""
  "; nop"
)

(define_insn "fractuqqqq2_32"
  [(set (match_operand:QQ    0 "register_operand" "=r")
       (fract_convert:QQ
         (match_operand:UQQ 1 "register_operand" "0")))]
  ""
  "; nop"
)

(define_insn "fracthquhq2_32"
  [(set (match_operand:UHQ  0 "register_operand" "=r")
       (unsigned_fract_convert:UHQ
         (match_operand:HQ 1 "register_operand" "0")))]
  ""
  "; nop"
)

(define_insn "fractuhqhq2_32"
  [(set (match_operand:HQ    0 "register_operand" "=r")
       (fract_convert:HQ
         (match_operand:UHQ 1 "register_operand" "0")))]
  ""
  "; nop"
)

(define_insn "fractsqusq2_32"
  [(set (match_operand:USQ  0 "register_operand" "=r")
       (unsigned_fract_convert:USQ
         (match_operand:SQ 1 "register_operand" "0")))]
  ""
  "; nop"
)

(define_insn "fractusqsq2_32"
  [(set (match_operand:SQ    0 "register_operand" "=r")
       (fract_convert:SQ
         (match_operand:USQ 1 "register_operand" "0")))]
  ""
  "; nop"
)

(define_insn "fractdqudq2_32"
  [(set (match_operand:UDQ  0 "register_operand" "=r")
       (unsigned_fract_convert:UDQ
         (match_operand:DQ 1 "register_operand" "0")))]
  ""
  "; nop"
)

(define_insn "fractudqdq2_32"
  [(set (match_operand:DQ    0 "register_operand" "=r")
       (fract_convert:DQ
         (match_operand:UDQ 1 "register_operand" "0")))]
  ""
  "; nop"
)

(define_insn "fracttqutq2_32"
  [(set (match_operand:UTQ  0 "register_operand" "=r")
       (unsigned_fract_convert:UTQ
         (match_operand:TQ 1 "register_operand" "0")))]
  ""
  "; nop"
)

(define_insn "fractutqtq2_32"
  [(set (match_operand:TQ    0 "register_operand" "=r")
       (fract_convert:TQ
         (match_operand:UTQ 1 "register_operand" "0")))]
  ""
  "; nop"
)

(define_insn "fracthauha2_32"
  [(set (match_operand:UHA  0 "register_operand" "=r")
       (unsigned_fract_convert:UHA
         (match_operand:HA 1 "register_operand" "0")))]
  ""
  "; nop"
)

(define_insn "fractuhaha2_32"
  [(set (match_operand:HA    0 "register_operand" "=r")
       (fract_convert:HA
         (match_operand:UHA 1 "register_operand" "0")))]
  ""
  "; nop"
)

(define_insn "fractsausa2_32"
  [(set (match_operand:USA  0 "register_operand" "=r")
       (unsigned_fract_convert:USA
         (match_operand:SA 1 "register_operand" "0")))]
  ""
  "; nop"
)

(define_insn "fractusasa2_32"
  [(set (match_operand:SA    0 "register_operand" "=r")
       (fract_convert:SA
         (match_operand:USA 1 "register_operand" "0")))]
  ""
  "; nop"
)

(define_insn "fractdauda2_32"
  [(set (match_operand:UDA  0 "register_operand" "=r")
       (unsigned_fract_convert:UDA
         (match_operand:DA 1 "register_operand" "0")))]
  ""
  "; nop"
)

(define_insn "fractudada2_32"
  [(set (match_operand:DA    0 "register_operand" "=r")
       (fract_convert:DA
         (match_operand:UDA 1 "register_operand" "0")))]
  ""
  "; nop"
)

(define_insn "fracttauta2_32"
  [(set (match_operand:UTA  0 "register_operand" "=r")
       (unsigned_fract_convert:UTA
         (match_operand:TA 1 "register_operand" "0")))]
  ""
  "; nop"
)

(define_insn "fractutata2_32"
  [(set (match_operand:TA    0 "register_operand" "=r")
       (fract_convert:TA
         (match_operand:UTA 1 "register_operand" "0")))]
  ""
  "; nop"
)

; saturating conversions
;   not all conversions saturate (but we seem to need them all)
;   many of these are copies of the "non-saturating" versions

(define_insn "satfract<mode>qi2_helper_32"
  [(set (match_operand:QI    0 "pic30_register_operand"  "=&r")
        (sat_fract:QI
          (match_operand:QUQ15 1 "pic30_register_operand"  "r")))]
  ""
  "*
  {
     gcc_assert(GET_MODE_FBIT(<MODE>mode) == 15);
     if ((pic30_fp_round_p() == pic30_truncation) ||
         (pic30_fp_round_p() == pic30_fastest)) {
       return \"asr %1,#15,%0\"; 
     } else if (pic30_fp_round_p() == pic30_conventional) {
       /* [-1,-.5] round to -1, (-.5,.5) round to 0, otherwise 1? */
       return \"asr %1,#14,%0\;btss %0,#0\;clr.b %0\";
     } else {
       error(\"Invalid fixed-point rounding mode specified\");
     }
     gcc_assert(0);
     return \"nop\";

  }"
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "satfract<mode>qi2_convergent_32"
  [(set (match_operand:QI    0 "pic30_register_operand"  "=&r")
        (sat_fract:QI
          (match_operand:QUQ15 1 "pic30_register_operand"  "r")))
   (clobber (match_scratch:QUQ15 2                         "=r"))
  ]
  ""
  "*
  {
     gcc_assert(GET_MODE_FBIT(<MODE>mode) == 15);
     if (pic30_fp_round_p() == pic30_convergent) {
       return \"asr %1,#14,%0\;\"  /* as conventional */
              \"btss %0,#0\;\"
              \"clr.b %0\;\"
              \"sl %1,#2,%2\;\"    /* x.yz13 where z13 == 0 */
              \"btsc SR,#1\;\"    /*   always rounds to even 0 */
              \"clr.b %0\";
     } else {
       error(\"Invalid fixed-point rounding mode specified\");
     }
     gcc_assert(0);
     return \"nop\";

  }"
  [
    (set_attr "cc" "clobber")
  ]
)

(define_expand "satfract<mode>qi2_32"
  [(set (match_operand:QI    0 "pic30_register_operand"  "=r")
        (sat_fract:QI
          (match_operand:QUQ15 1 "pic30_register_operand"  "r")))]
  ""
  "
  { rtx r0,r1;

    if (!pic30_fp_inline_p() && (pic30_fp_round_p() == pic30_convergent))
      FAIL;
    r0 = operands[0];
    r1 = operands[1];
    if (!pic30_register_operand(r0,QImode)) {
      r0 = gen_reg_rtx(QImode);
    }
    if (!pic30_register_operand(r1,VOIDmode)) {
      r1 = force_reg(<MODE>mode, r1);
    }
    if (pic30_fp_round_p() == pic30_convergent) {
      emit(
        gen_satfract<mode>qi2_convergent_32(r0,r1)
      );
    } else {
      emit(
        gen_satfract<mode>qi2_helper_32(r0,r1)
      );
    }
    if (r0 != operands[0]) {
      emit_move_insn(operands[0],r0);
    }
    DONE;
  }
  "
)

(define_insn "satfract<mode>hi2_helper_32"
  [(set (match_operand:HI    0 "pic30_register_operand"  "=&r")
        (sat_fract:HI
          (match_operand:QUQ15 1 "pic30_register_operand"  "r")))]
  ""
  "*
  {
     gcc_assert(GET_MODE_FBIT(<MODE>mode) == 15);
     if ((pic30_fp_round_p() == pic30_truncation) ||
         (pic30_fp_round_p() == pic30_fastest)) {
       /* chop the low order bits, signed results are either 0 or -1 */
       return \"asr %1,#15,%0\"; 
     } else if (pic30_fp_round_p() == pic30_conventional) {
       /* [-1,-.5] round to -1, (-.5,.5) round to 0, otherwise 1? */
       return \"asr %1,#14,%0\;btss %0,#0\;clr %0\";
     } else {
       error(\"Invalid fixed-point rounding mode specified\");
     }
     gcc_assert(0);
     return \"nop\";

  }"
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "satfract<mode>hi2_convergent_32"
  [(set (match_operand:HI    0 "pic30_register_operand"  "=&r")
        (sat_fract:HI
          (match_operand:QUQ15 1 "pic30_register_operand"  "r")))
   (clobber (match_scratch:QUQ15 2                         "=r"))
  ]
  ""
  "*
  {
     gcc_assert(GET_MODE_FBIT(<MODE>mode) == 15);
     if (pic30_fp_round_p() == pic30_convergent) {
       return \"asr %1,#14,%0\;\"  /* as conventional */
              \"btss %0,#0\;\"
              \"clr %0\;\"
              \"sl %1,#2,%2\;\"    /* x.yz13 where z13 == 0 */
              \"btsc SR,#1\;\"    /*   always rounds to even 0 */
              \"clr %0\";
     } else {
       error(\"Invalid fixed-point rounding mode specified\");
     }
     gcc_assert(0);
     return \"nop\";

  }"
  [
    (set_attr "cc" "clobber")
  ]
)

(define_expand "satfract<mode>hi2_32"
  [(set (match_operand:HI    0 "pic30_register_operand"  "=r")
        (sat_fract:HI
          (match_operand:QUQ15 1 "pic30_register_operand"  "r")))]
  ""
  "
  { rtx r0,r1;

    if (!pic30_fp_inline_p() && (pic30_fp_round_p() == pic30_convergent))
      FAIL;
    r0 = operands[0];
    r1 = operands[1];
    if (!pic30_register_operand(r0,HImode)) {
      r0 = gen_reg_rtx(HImode);
    }
    if (!pic30_register_operand(r1,VOIDmode)) {
      r1 = force_reg(<MODE>mode, r1);
    }
    if (pic30_fp_round_p() == pic30_convergent) {
      emit(
        gen_satfract<mode>hi2_convergent_32(r0,r1)
      );
    } else {
      emit(
        gen_satfract<mode>hi2_helper_32(r0,r1)
      );
    }
    if (r0 != operands[0]) {
      emit_move_insn(operands[0],r0);
    }
    DONE;
  }
  "
)

(define_insn "satfract<mode>si2_helper_32"
  [(set (match_operand:SI    0 "pic30_register_operand"  "=&r")
        (sat_fract:SI
          (match_operand:QUQ15 1 "pic30_register_operand"  "r")))]
  ""
  "*
  {
     gcc_assert(GET_MODE_FBIT(<MODE>mode) == 15);
     if ((pic30_fp_round_p() == pic30_truncation) ||
         (pic30_fp_round_p() == pic30_fastest)) {
       /* chop the low order bits, signed results are either 0 or -1 */
       return \"asr %1,#15,%d0\;asr %1,#15,%0\";
     } else if (pic30_fp_round_p() == pic30_conventional) {
       /* [-1,-.5] round to -1, (-.5,.5) round to 0, otherwise 1? */
       return \"asr %1,#14,%0\;\"
              \"btss %0,#0\;\"
              \"clr %0\;\"
              \"asr %0,#15,%d0\";
     } else {
       error(\"Invalid fixed-point rounding mode specified\");
     }
     gcc_assert(0);
     return \"nop\";
  }"
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "satfract<mode>si2_convergent_32"
  [(set (match_operand:SI    0 "pic30_register_operand"  "=&r")
        (sat_fract:SI
          (match_operand:QUQ15 1 "pic30_register_operand"  "r")))
   (clobber (match_scratch:QUQ15 2                         "=r"))
  ]
  ""
  "*
  {
     gcc_assert(GET_MODE_FBIT(<MODE>mode) == 15);
     if (pic30_fp_round_p() == pic30_convergent) {
       return \"asr %1,#14,%0\;\" /* as conventional */
              \"btss %0,#0\;\"
              \"clr  %0\;\"
              \"asr %0,#15,%d0\;\"
              \"sl %1,#2,%2\;\"    /* x.yz13 where z13 == 0 */
              \"btsc SR,#1\;\"    /*   always rounds to even 0 */
              \"mul.uu %0,#0,%0\";
     } else {
       error(\"Invalid fixed-point rounding mode specified\");
     }
     gcc_assert(0);
     return \"nop\";

  }"
  [
    (set_attr "cc" "clobber")
  ]
)

(define_expand "satfract<mode>si2_32"
  [(set (match_operand:SI    0 "pic30_register_operand"  "=r")
        (sat_fract:SI
          (match_operand:QUQ15 1 "pic30_register_operand"  "r")))]
  ""
  "
  { rtx r0,r1;

    if (!pic30_fp_inline_p() && (pic30_fp_round_p() == pic30_convergent))
      FAIL;
    r0 = operands[0];
    r1 = operands[1];
    if (!pic30_register_operand(r0,SImode)) {
      r0 = gen_reg_rtx(SImode);
    }
    if (!pic30_register_operand(r1,VOIDmode)) {
      r1 = force_reg(<MODE>mode, r1);
    }
    if (pic30_fp_round_p() == pic30_convergent) {
      emit(
        gen_satfract<mode>si2_convergent_32(r0,r1)
      );
    } else {
      emit(
        gen_satfract<mode>si2_helper_32(r0,r1)
      );
    }
    if (r0 != operands[0]) {
      emit_move_insn(operands[0],r0);
    }
    DONE;
  }
  "
)

(define_insn "satfract<mode>di2_helper_32"
  [(set (match_operand:DI    0 "pic30_register_operand"  "=&r")
        (sat_fract:DI
          (match_operand:QUQ15 1 "pic30_register_operand"  "r")))]
  ""
  "*
  {
     gcc_assert(GET_MODE_FBIT(<MODE>mode) == 15);
     if ((pic30_fp_round_p() == pic30_truncation) ||
         (pic30_fp_round_p() == pic30_fastest)) {
       /* chop the low order bits, signed results are either 0 or -1 */
       return \"asr %1,#15,%q0\;asr %1,#15,%t0\;mul.su %t0,#1,%0\";
     } else if (pic30_fp_round_p() == pic30_conventional) {
       /* [-1,-.5] round to -1, (-.5,.5) round to 0, otherwise 1? */
       return \"asr %1,#14,%0\;\"
              \"btss %0,#0\;\"
              \"clr %0\;\"
              \"asr %0,#15,%d0\;\"
              \"mul.su %d0,#1,%t0\";
     } else {
       error(\"Invalid fixed-point rounding mode specified\");
     }
     gcc_assert(0);
     return \"nop\";
  }"
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "satfract<mode>di2_convergent_32"
  [(set (match_operand:DI    0 "pic30_register_operand"  "=&r")
        (sat_fract:DI
          (match_operand:QUQ15 1 "pic30_register_operand"  "r")))
   (clobber (match_scratch:QUQ15 2                         "=r"))
  ]
  ""
  "*
  {
     gcc_assert(GET_MODE_FBIT(<MODE>mode) == 15);
     if (pic30_fp_round_p() == pic30_convergent) {
       return \"asr %1,#14,%0\;\"  /* as conventional */
              \"btss %0,#0\;\"
              \"clr  %0\;\"
              \"asr %0,#15,%d0\;\"
              \"sl %1,#2,%2\;\"    /* x.yz13 where z13 == 0 */
              \"btsc SR,#1\;\"    /*   always rounds to even 0 */
              \"mul.uu %0,#0,%0\;\"
              \"mul.su %d0,#1,%t0\";
     } else {
       error(\"Invalid fixed-point rounding mode specified\");
     }
     gcc_assert(0);
     return \"nop\";

  }"
  [
    (set_attr "cc" "clobber")
  ]
)

(define_expand "satfract<mode>di2_32"
  [(set (match_operand:DI    0 "pic30_register_operand"  "=r")
        (sat_fract:DI
          (match_operand:QUQ15 1 "pic30_register_operand"  "r")))]
  ""
  "
  { rtx r0,r1;

    if (!pic30_fp_inline_p() && 
        ((pic30_fp_round_p() == pic30_convergent) ||
         (pic30_fp_round_p() == pic30_conventional)))
      FAIL;
    r0 = operands[0];
    r1 = operands[1];
    if (!pic30_register_operand(r0,DImode)) {
      r0 = gen_reg_rtx(DImode);
    }
    if (!pic30_register_operand(r1,VOIDmode)) {
      r1 = force_reg(<MODE>mode, r1);
    }
    if (pic30_fp_round_p() == pic30_convergent) {
      emit(
        gen_satfract<mode>di2_convergent_32(r0,r1)
      );
    } else {
      emit(
        gen_satfract<mode>di2_helper_32(r0,r1)
      );
    }
    if (r0 != operands[0]) {
      emit_move_insn(operands[0],r0);
    }
    DONE;
  }
  "
)

; from Q31

(define_insn "satfract<mode>qi2_helper_32"
  [(set (match_operand:QI    0 "pic30_register_operand"  "=&r")
        (sat_fract:QI
          (match_operand:QUQ31 1 "pic30_register_operand"  "r")))]
  ""
  "*
  {
     gcc_assert(GET_MODE_FBIT(<MODE>mode) == 31);
     if ((pic30_fp_round_p() == pic30_truncation) ||
         (pic30_fp_round_p() == pic30_fastest)) {
       /* chop the low order bits, signed results are either 0 or -1 */
       return \"asr %d1,#15,%0\"; 
     } else if (pic30_fp_round_p() == pic30_conventional) {
       /* [-1,-.5] round to -1, (-.5,.5) round to 0, otherwise 1? */
       return \"asr %d1,#14,%0\;btss %0,#0\;clr.b %0\";
     } else {
       error(\"Invalid fixed-point rounding mode specified\");
     }
     gcc_assert(0);
     return \"nop\";

  }"
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "satfract<mode>qi2_convergent_32"
  [(set (match_operand:QI    0 "pic30_register_operand"  "=&r")
        (sat_fract:QI
          (match_operand:QUQ31 1 "pic30_register_operand"  "r")))
   (clobber (match_scratch:QUQ31 2                         "=&r"))
  ]
  ""
  "*
  {
     gcc_assert(GET_MODE_FBIT(<MODE>mode) == 31);
     if (pic30_fp_round_p() == pic30_convergent) {
       return \"asr %d1,#14,%0\;\"  /* as conventional */
              \"btss %0,#0\;\"
              \"clr.b %0\;\"
              \"sl %d1,#2,%2\;\"    /* x.yz13 where z13 == 0 */
              \"btsc SR,#1\;\"     /*   always rounds to even 0 */
              \"cp0 %1\;\"  
              \"btsc SR,#1\;\"
              \"clr %0\";
     } else {
       error(\"Invalid fixed-point rounding mode specified\");
     }
     gcc_assert(0);
     return \"nop\";

  }"
  [
    (set_attr "cc" "clobber")
  ]
)

(define_expand "satfract<mode>qi2_32"
  [(set (match_operand:QI    0 "pic30_register_operand"  "=r")
        (sat_fract:QI
          (match_operand:QUQ31 1 "pic30_register_operand"  "r")))]
  ""
  "
  { rtx r0,r1;

    if (!pic30_fp_inline_p() && (pic30_fp_round_p() == pic30_convergent))
      FAIL;
    r0 = operands[0];
    r1 = operands[1];
    if (!pic30_register_operand(r0,QImode)) {
      r0 = gen_reg_rtx(QImode);
    }
    if (!pic30_register_operand(r1,VOIDmode)) {
      r1 = force_reg(<MODE>mode, r1);
    }
    if (pic30_fp_round_p() == pic30_convergent) {
      emit(
        gen_satfract<mode>qi2_convergent_32(r0,r1)
      );
    } else {
      emit(
        gen_satfract<mode>qi2_helper_32(r0,r1)
      );
    }
    if (r0 != operands[0]) {
      emit_move_insn(operands[0],r0);
    }
    DONE;
  }
  "
)

(define_insn "satfract<mode>hi2_helper_32"
  [(set (match_operand:HI    0 "pic30_register_operand"  "=&r")
        (sat_fract:HI
          (match_operand:QUQ31 1 "pic30_register_operand"  "r")))]
  ""
  "*
  {
     gcc_assert(GET_MODE_FBIT(<MODE>mode) == 31);
     if ((pic30_fp_round_p() == pic30_truncation) ||
         (pic30_fp_round_p() == pic30_fastest)) {
       /* chop the low order bits, signed results are either 0 or -1 */
       return \"asr %d1,#15,%0\"; 
     } else if (pic30_fp_round_p() == pic30_conventional) {
       /* [-1,-.5] round to -1, (-.5,.5) round to 0, otherwise 1? */
       return \"asr %d1,#14,%0\;btss %0,#0\;clr %0\";
     } else {
       error(\"Invalid fixed-point rounding mode specified\");
     }
     gcc_assert(0);
     return \"nop\";

  }"
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "satfract<mode>hi2_convergent_32"
  [(set (match_operand:HI    0 "pic30_register_operand"  "=&r")
        (sat_fract:HI
          (match_operand:QUQ31 1 "pic30_register_operand"  "r")))
   (clobber (match_scratch:QUQ31 2                         "=&r"))
  ]
  ""
  "*
  {
     gcc_assert(GET_MODE_FBIT(<MODE>mode) == 31);
     if (pic30_fp_round_p() == pic30_convergent) {
       return \"asr %d1,#14,%0\;\"  /* as conventional */
              \"btss %0,#0\;\"
              \"clr %0\;\"
              \"sl %d1,#2,%2\;\"    /* x.yz13 where z13 == 0 */
              \"btsc SR,#1\;\"     /*   always rounds to even 0 */
              \"cp0 %1\;\"  
              \"btsc SR,#1\;\"
              \"clr %0\";
     } else {
       error(\"Invalid fixed-point rounding mode specified\");
     }
     gcc_assert(0);
     return \"nop\";

  }"
  [
    (set_attr "cc" "clobber")
  ]
)

(define_expand "satfract<mode>hi2_32"
  [(set (match_operand:HI    0 "pic30_register_operand"  "=r")
        (sat_fract:HI
          (match_operand:QUQ31 1 "pic30_register_operand"  "r")))]
  ""
  "
  { rtx r0,r1;

    if (!pic30_fp_inline_p() && (pic30_fp_round_p() == pic30_convergent))
      FAIL;
    r0 = operands[0];
    r1 = operands[1];
    if (!pic30_register_operand(r0,HImode)) {
      r0 = gen_reg_rtx(HImode);
    }
    if (!pic30_register_operand(r1,VOIDmode)) {
      r1 = force_reg(<MODE>mode, r1);
    }
    if (pic30_fp_round_p() == pic30_convergent) {
      emit(
        gen_satfract<mode>hi2_convergent_32(r0,r1)
      );
    } else {
      emit(
        gen_satfract<mode>hi2_helper_32(r0,r1)
      );
    }
    if (r0 != operands[0]) {
      emit_move_insn(operands[0],r0);
    }
    DONE;
  }
  "
)

(define_insn "satfract<mode>si2_helper_32"
  [(set (match_operand:SI    0 "pic30_register_operand"  "=&r")
        (sat_fract:SI
          (match_operand:QUQ31 1 "pic30_register_operand"  "r")))]
  ""
  "*
  {
     gcc_assert(GET_MODE_FBIT(<MODE>mode) == 31);
     if ((pic30_fp_round_p() == pic30_truncation) ||
         (pic30_fp_round_p() == pic30_fastest)) {
       /* chop the low order bits, signed results are either 0 or -1 */
       return \"asr %d1,#15,%d0\;asr %d1,#15,%0\";
     } else if (pic30_fp_round_p() == pic30_conventional) {
       /* [-1,-.5] round to -1, (-.5,.5) round to 0, otherwise 1? */
       return \"asr %d1,#14,%0\;\"
              \"btss %0,#0\;\"
              \"clr %0\;\"
              \"asr %0,#15,%d0\";
     } else {
       error(\"Invalid fixed-point rounding mode specified\");
     }
     gcc_assert(0);
     return \"nop\";
  }"
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "satfract<mode>si2_convergent_32"
  [(set (match_operand:SI    0 "pic30_register_operand"  "=&r")
        (sat_fract:SI
          (match_operand:QUQ31 1 "pic30_register_operand"  "r")))
   (clobber (match_scratch:QUQ31 2                         "=&r"))
  ]
  ""
  "*
  {
     gcc_assert(GET_MODE_FBIT(<MODE>mode) == 31);
     if (pic30_fp_round_p() == pic30_convergent) {
       return \"asr %d1,#14,%0\;\"  /* as conventional */
              \"btss %0,#0\;\"
              \"clr  %0\;\"
              \"asr %0,#15,%d0\;\"
              \"sl %d1,#2,%2\;\"    /* x.yz13 where z13 == 0 */
              \"btsc SR,#1\;\"     /*   always rounds to even 0 */
              \"cp0 %1\;\"
              \"btsc SR,#1\;\"
              \"mul.uu %0,#0,%0\";
     } else {
       error(\"Invalid fixed-point rounding mode specified\");
     }
     gcc_assert(0);
     return \"nop\";

  }"
  [
    (set_attr "cc" "clobber")
  ]
)

(define_expand "satfract<mode>si2_32"
  [(set (match_operand:SI    0 "pic30_register_operand"  "=r")
        (sat_fract:SI
          (match_operand:QUQ31 1 "pic30_register_operand"  "r")))]
  ""
  "
  { rtx r0,r1;

    if (!pic30_fp_inline_p() && (pic30_fp_round_p() == pic30_convergent))
      FAIL;
    r0 = operands[0];
    r1 = operands[1];
    if (!pic30_register_operand(r0,SImode)) {
      r0 = gen_reg_rtx(SImode);
    }
    if (!pic30_register_operand(r1,VOIDmode)) {
      r1 = force_reg(<MODE>mode, r1);
    }
    if (pic30_fp_round_p() == pic30_convergent) {
      emit(
        gen_satfract<mode>si2_convergent_32(r0,r1)
      );
    } else {
      emit(
        gen_satfract<mode>si2_helper_32(r0,r1)
      );
    }
    if (r0 != operands[0]) {
      emit_move_insn(operands[0],r0);
    }
    DONE;
  }
  "
)

(define_insn "satfract<mode>di2_helper_32"
  [(set (match_operand:DI    0 "pic30_register_operand"  "=&r")
        (sat_fract:DI
          (match_operand:QUQ31 1 "pic30_register_operand"  "r")))]
  ""
  "*
  {
     gcc_assert(GET_MODE_FBIT(<MODE>mode) == 31);
     if ((pic30_fp_round_p() == pic30_truncation) ||
         (pic30_fp_round_p() == pic30_fastest)) {
       /* chop the low order bits, signed results are either 0 or -1 */
       return \"asr %d1,#15,%q0\;asr %d1,#15,%t0\;mul.su %t0,#1,%0\";
     } else if (pic30_fp_round_p() == pic30_conventional) {
       /* [-1,-.5] round to -1, (-.5,.5) round to 0, otherwise 1? */
       return \"asr %d1,#14,%0\;\"
              \"btss %0,#0\;\"
              \"clr %0\;\"
              \"asr %0,#15,%d0\;\"
              \"mul.su %d0,#1,%t0\";
     } else {
       error(\"Invalid fixed-point rounding mode specified\");
     }
     gcc_assert(0);
     return \"nop\";
  }"
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "satfract<mode>di2_convergent_32"
  [(set (match_operand:DI    0 "pic30_register_operand"  "=&r")
        (sat_fract:DI
          (match_operand:QUQ31 1 "pic30_register_operand"  "r")))
   (clobber (match_scratch:QUQ31 2                         "=&r"))
  ]
  ""
  "*
  {
     gcc_assert(GET_MODE_FBIT(<MODE>mode) == 31);
     if (pic30_fp_round_p() == pic30_convergent) {
       return \"asr %d1,#14,%0\;\"  /* as conventional */
              \"btss %0,#0\;\"
              \"clr  %0\;\"
              \"asr %0,#15,%d0\;\"
              \"sl %d1,#2,%2\;\"    /* x.yz13 where z13 == 0 */
              \"btsc SR,#1\;\"     /*   always rounds to even 0 */
              \"cp0 %1\;\"
              \"btsc SR,#1\;\"
              \"mul.uu %0,#0,%0\;\"
              \"mul.su %d0,#1,%t0\";
     } else {
       error(\"Invalid fixed-point rounding mode specified\");
     }
     gcc_assert(0);
     return \"nop\";

  }"
  [
    (set_attr "cc" "clobber")
  ]
)

(define_expand "satfract<mode>di2_32"
  [(set (match_operand:DI    0 "pic30_register_operand"  "=r")
        (sat_fract:DI
          (match_operand:QUQ31 1 "pic30_register_operand"  "r")))]
  ""
  "
  { rtx r0,r1;

    if (!pic30_fp_inline_p() && 
         ((pic30_fp_round_p() == pic30_convergent) ||
          (pic30_fp_round_p() == pic30_convergent)))
      FAIL;
    r0 = operands[0];
    r1 = operands[1];
    if (!pic30_register_operand(r0,DImode)) {
      r0 = gen_reg_rtx(DImode);
    }
    if (!pic30_register_operand(r1,VOIDmode)) {
      r1 = force_reg(<MODE>mode, r1);
    }
    if (pic30_fp_round_p() == pic30_convergent) {
      emit(
        gen_satfract<mode>di2_convergent_32(r0,r1)
      );
    } else {
      emit(
        gen_satfract<mode>di2_helper_32(r0,r1)
      );
    }
    if (r0 != operands[0]) {
      emit_move_insn(operands[0],r0);
    }
    DONE;
  }
  "
)

; to Q15

(define_insn "satfractqi<mode>2_32"
  [(set (match_operand:QUQ15  0 "pic30_register_operand"  "=r")
        (sat_fract:QUQ15
          (match_operand:QI 1 "pic30_register_operand"  "r")))]
  "pic30_fp_inline_p()"
  "*
   {  
      return \"cp0.b %1\;\"        /* compare input with 0 */
             \"mov  #0x8000,%0\;\" /* assume -1 */
             \"btsc SR,#0\;\"     /* positive => 0 or .99 */
             \"mov #0x7FFF,%0\;\"  /*   load .99... */
             \"btsc SR,#1\;\"     /* if zero */
             \"clr %0\";           /*   load zero */
   }
  "
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "satfracthi<mode>2_32"
  [(set (match_operand:QUQ15  0 "pic30_register_operand"  "=r")
        (sat_fract:QUQ15
          (match_operand:HI 1 "pic30_register_operand"  "r")))]
  "pic30_fp_inline_p()"
  "*
   {
      return \"cp0  %1\;\"         /* compare input with 0 */
             \"mov  #0x8000,%0\;\" /* assume -1 */
             \"btsc SR,#0\;\"     /* positive => 0 or .99 */
             \"mov #0x7FFF,%0\;\"  /*   load .99... */
             \"btsc SR,#1\;\"     /* if zero */
             \"clr %0\";           /*   load zero */
   }
  "
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "satfractsi<mode>2_32"
  [(set (match_operand:QUQ15  0 "pic30_register_operand"  "=r")
        (sat_fract:QUQ15
          (match_operand:SI 1 "pic30_register_operand"  "r")))]
  "pic30_fp_inline_p()"
  "*
   {  
      return \"subr %1,#0,[w15]\;\"   /* compare input with 0 */
             \"subbr %d1,#0,[w15]\;\" /* compare input with 0 */
             \"mov  #0x8000,%0\;\"    /* assume -1 */
             \"btsc SR,#0\;\"        /* positive => 0 or .99 */
             \"mov #0x7FFF,%0\;\"     /*   load .99... */
             \"btsc SR,#1\;\"        /* if zero */
             \"clr %0\";              /*   load zero */
   }
  "
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "satfractdi<mode>2_32"
  [(set (match_operand:QUQ15  0 "pic30_register_operand"  "=r")
        (sat_fract:QUQ15
          (match_operand:DI 1 "pic30_register_operand"  "r")))]
  "pic30_fp_inline_p()"
  "*
   {  
      return \"subr %1,#0,[w15]\;\"   /* compare input with 0 */
             \"subbr %d1,#0,[w15]\;\" /* compare input with 0 */
             \"subbr %t1,#0,[w15]\;\" /* compare input with 0 */
             \"subbr %q1,#0,[w15]\;\" /* compare input with 0 */
             \"mov  #0x8000,%0\;\"    /* assume -1 */
             \"btsc SR,#0\;\"        /* positive => 0 or .99 */
             \"mov #0x7FFF,%0\;\"     /*   load .99... */
             \"btsc SR,#1\;\"        /* if zero */
             \"clr %0\";              /*   load zero */
   }
  "
  [
    (set_attr "cc" "clobber")
  ]
)

; to Q31

(define_insn "satfractqi<mode>2_32"
  [(set (match_operand:QUQ31  0 "pic30_register_operand"  "=r")
        (sat_fract:QUQ31
          (match_operand:QI 1 "pic30_register_operand"  "r")))]
  "pic30_fp_inline_p()"
  "*
   {  
      return 
             \"cp0.b %1\;\"         /* compare input with 0 */
             \"mov #0x8000,%d0\;\"  /* assume -1 */
             \"btsc SR,#0\;\"      /* positive => 0 or .99 */
             \"mov #0x7FFF,%d0\;\"  /*   load .99... */
             \"btsc SR,#1\;\"      /* if zero */
             \"clr %d0\;\"          /*   load zero */
             \"sl  %d0,%0\;\"       /* form low word by propagating non sign */
             \"asr %0,#15,%0\";     /*   bits into low word */
   }
  "
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "satfracthi<mode>2_32"
  [(set (match_operand:QUQ31  0 "pic30_register_operand"  "=r")
        (sat_fract:QUQ31
          (match_operand:HI 1 "pic30_register_operand"  "r")))]
  "pic30_fp_inline_p()"
  "*
   {
      return 
             \"cp0.b %1\;\"         /* compare input with 0 */
             \"mov #0x8000,%d0\;\"  /* assume -1 */
             \"btsc SR,#0\;\"      /* positive => 0 or .99 */
             \"mov #0x7FFF,%d0\;\"  /*   load .99... */
             \"btsc SR,#1\;\"      /* if zero */
             \"clr %d0\;\"          /*   load zero */
             \"sl  %d0,%0\;\"       /* form low word by propagating non sign */
             \"asr %0,#15,%0\";     /*   bits into low word */
   }
  "
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "satfractsi<mode>2_32"
  [(set (match_operand:QUQ31  0 "pic30_register_operand"  "=r")
        (sat_fract:QUQ31
          (match_operand:SI 1 "pic30_register_operand"  "r")))]
  "pic30_fp_inline_p()"
  "*
   {  
      return 
             \"sub %1,#0,[w15]\;\"   /* compare input with 0 */
             \"subb %d1,#0,[w15]\;\" /* compare input with 0 */
             \"mov #0x8000,%d0\;\"   /* assume -1 */
             \"btsc SR,#0\;\"       /* positive => 0 or .99 */
             \"mov #0x7FFF,%d0\;\"   /*   load .99... */
             \"btsc SR,#1\;\"       /* if zero */
             \"clr %d0\;\"           /*   load zero */
             \"sl  %d0,%0\;\"        /* form low word by propagating non sign */
             \"asr %0,#15,%0\";      /*   bits into low word */
   }
  "
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "satfractdi<mode>2_32"
  [(set (match_operand:QUQ31  0 "pic30_register_operand"  "=r")
        (sat_fract:QUQ31
          (match_operand:DI 1 "pic30_register_operand"  "r")))]
  "pic30_fp_inline_p()"
  "*
   {  
      return 
             \"sub %1,#0,[w15]\;\"   /* compare input with 0 */
             \"subb %d1,#0,[w15]\;\" /* compare input with 0 */
             \"subb %t1,#0,[w15]\;\" /* compare input with 0 */
             \"subb %q1,#0,[w15]\;\" /* compare input with 0 */
             \"mov #0x8000,%d0\;\"   /* assume -1 */
             \"btsc SR,#0\;\"       /* positive => 0 or .99 */
             \"mov #0x7FFF,%d0\;\"   /*   load .99... */
             \"btsc SR,#1\;\"       /* if zero */
             \"clr %d0\;\"           /*   load zero */
             \"sl  %d0,%0\;\"        /* form low word by propagating non sign */
             \"asr %0,#15,%0\";      /*   bits into low word */
   }
  "
  [
    (set_attr "cc" "clobber")
  ]
)

; to ACC

; cannot saturate
(define_insn "satfractqi<mode>2_32"
  [(set (match_operand:AUACC 0 "pic30_register_operand"  "=&r")
        (sat_fract:AUACC
          (match_operand:QI  1 "pic30_register_operand"    "r")))]
  ""
  "*
   {
      return
             \"mul.uu %0,#0,%0\;\"  /* clear low bits */
             \"mov.b %1,%t0\;\"     /* copy integer portion into upper byte */
             \"asr.b %t0,%t0\;\"    /* shift it into position */
             \"rrc %d0,%d0\";
   }
  "
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "satfractunsqi<mode>2_32"
  [(set (match_operand:AUACC 0 "pic30_register_operand"  "=&r")
        (unsigned_sat_fract:AUACC
          (match_operand:QI  1 "pic30_register_operand"    "r")))]
  ""
  "*
  {
      return
             \"mul.uu %0,#0,%0\;\"  /* clear low bits */
             \"mov.b %1,%t0\;\"     /* copy integer portion into upper byte */
             \"asr.b %t0,%t0\;\"    /* shift it into position */
             \"rrc %d0,%d0\";
   }
  "
  [
    (set_attr "cc" "clobber")
  ]
)

; the rest are function only

; unsigned

(define_insn "satfractuns<mode>qi2_helper_32"
  [(set (match_operand:QI    0 "pic30_register_operand"  "=&r")
        (unsigned_sat_fract:QI
          (match_operand:QUQ15 1 "pic30_register_operand"  "r")))]
  ""
  "*
  {
     gcc_assert(GET_MODE_FBIT(<MODE>mode) == 15);
     if ((pic30_fp_round_p() == pic30_truncation) ||
         (pic30_fp_round_p() == pic30_fastest)) {
       return \"asr %1,#15,%0\"; 
     } else if (pic30_fp_round_p() == pic30_conventional) {
       /* [-1,-.5] round to -1, (-.5,.5) round to 0, otherwise 1? */
       return \"asr %1,#14,%0\;btss %0,#0\;clr.b %0\";
     } else {
       error(\"Invalid fixed-point rounding mode specified\");
     }
     gcc_assert(0);
     return \"nop\";

  }"
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "satfractuns<mode>qi2_convergent_32"
  [(set (match_operand:QI    0 "pic30_register_operand"  "=&r")
        (unsigned_sat_fract:QI
          (match_operand:QUQ15 1 "pic30_register_operand"  "r")))
   (clobber (match_scratch:QUQ15 2                         "=r"))
  ]
  ""
  "*
  {
     gcc_assert(GET_MODE_FBIT(<MODE>mode) == 15);
     if (pic30_fp_round_p() == pic30_convergent) {
       return \"asr %1,#14,%0\;\"  /* as conventional */
              \"btss %0,#0\;\"
              \"clr.b %0\;\"
              \"sl %1,#2,%2\;\"    /* x.yz13 where z13 == 0 */
              \"btsc SR,#1\;\"    /*   always rounds to even 0 */
              \"clr.b %0\";
     } else {
       error(\"Invalid fixed-point rounding mode specified\");
     }
     gcc_assert(0);
     return \"nop\";

  }"
  [
    (set_attr "cc" "clobber")
  ]
)

(define_expand "satfractuns<mode>qi2_32"
  [(set (match_operand:QI    0 "pic30_register_operand"  "=r")
        (unsigned_sat_fract:QI
          (match_operand:QUQ15 1 "pic30_register_operand"  "r")))]
  ""
  "
  { rtx r0,r1;

    if (!pic30_fp_inline_p() && (pic30_fp_round_p() == pic30_convergent))
      FAIL;
    r0 = operands[0];
    r1 = operands[1];
    if (!pic30_register_operand(r0,QImode)) {
      r0 = gen_reg_rtx(QImode);
    }
    if (!pic30_register_operand(r1,VOIDmode)) {
      r1 = force_reg(<MODE>mode, r1);
    }
    if (pic30_fp_round_p() == pic30_convergent) {
      emit(
        gen_satfractuns<mode>qi2_convergent_32(r0,r1)
      );
    } else {
      emit(
        gen_satfractuns<mode>qi2_helper_32(r0,r1)
      );
    }
    if (r0 != operands[0]) {
      emit_move_insn(operands[0],r0);
    }
    DONE;
  }
  "
)

(define_insn "satfractuns<mode>hi2_helper_32"
  [(set (match_operand:HI    0 "pic30_register_operand"  "=&r")
        (unsigned_sat_fract:HI
          (match_operand:QUQ15 1 "pic30_register_operand"  "r")))]
  ""
  "*
  {
     gcc_assert(GET_MODE_FBIT(<MODE>mode) == 15);
     if ((pic30_fp_round_p() == pic30_truncation) ||
         (pic30_fp_round_p() == pic30_fastest)) {
       /* chop the low order bits, signed results are either 0 or -1 */
       return \"asr %1,#15,%0\"; 
     } else if (pic30_fp_round_p() == pic30_conventional) {
       /* [-1,-.5] round to -1, (-.5,.5) round to 0, otherwise 1? */
       return \"asr %1,#14,%0\;btss %0,#0\;clr %0\";
     } else {
       error(\"Invalid fixed-point rounding mode specified\");
     }
     gcc_assert(0);
     return \"nop\";

  }"
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "satfractuns<mode>hi2_convergent_32"
  [(set (match_operand:HI    0 "pic30_register_operand"  "=&r")
        (unsigned_sat_fract:HI
          (match_operand:QUQ15 1 "pic30_register_operand"  "r")))
   (clobber (match_scratch:QUQ15 2                         "=r"))
  ]
  ""
  "*
  {
     gcc_assert(GET_MODE_FBIT(<MODE>mode) == 15);
     if (pic30_fp_round_p() == pic30_convergent) {
       return \"asr %1,#14,%0\;\"  /* as conventional */
              \"btss %0,#0\;\"
              \"clr %0\;\"
              \"sl %1,#2,%2\;\"    /* x.yz13 where z13 == 0 */
              \"btsc SR,#1\;\"    /*   always rounds to even 0 */
              \"clr %0\";
     } else {
       error(\"Invalid fixed-point rounding mode specified\");
     }
     gcc_assert(0);
     return \"nop\";

  }"
  [
    (set_attr "cc" "clobber")
  ]
)

(define_expand "satfractuns<mode>hi2_32"
  [(set (match_operand:HI    0 "pic30_register_operand"  "=r")
        (unsigned_sat_fract:HI
          (match_operand:QUQ15 1 "pic30_register_operand"  "r")))]
  ""
  "
  { rtx r0,r1;

    if (!pic30_fp_inline_p() && (pic30_fp_round_p() == pic30_convergent))
      FAIL;
    r0 = operands[0];
    r1 = operands[1];
    if (!pic30_register_operand(r0,HImode)) {
      r0 = gen_reg_rtx(HImode);
    }
    if (!pic30_register_operand(r1,VOIDmode)) {
      r1 = force_reg(<MODE>mode, r1);
    }
    if (pic30_fp_round_p() == pic30_convergent) {
      emit(
        gen_satfractuns<mode>hi2_convergent_32(r0,r1)
      );
    } else {
      emit(
        gen_satfractuns<mode>hi2_helper_32(r0,r1)
      );
    }
    if (r0 != operands[0]) {
      emit_move_insn(operands[0],r0);
    }
    DONE;
  }
  "
)

(define_insn "satfractuns<mode>si2_helper_32"
  [(set (match_operand:SI    0 "pic30_register_operand"  "=&r")
        (unsigned_sat_fract:SI
          (match_operand:QUQ15 1 "pic30_register_operand"  "r")))]
  ""
  "*
  {
     gcc_assert(GET_MODE_FBIT(<MODE>mode) == 15);
     if ((pic30_fp_round_p() == pic30_truncation) ||
         (pic30_fp_round_p() == pic30_fastest)) {
       /* chop the low order bits, signed results are either 0 or -1 */
       return \"asr %1,#15,%d0\;asr %1,#15,%0\";
     } else if (pic30_fp_round_p() == pic30_conventional) {
       /* [-1,-.5] round to -1, (-.5,.5) round to 0, otherwise 1? */
       return \"asr %1,#14,%0\;\"
              \"btss %0,#0\;\"
              \"clr %0\;\"
              \"asr %0,#15,%d0\";
     } else {
       error(\"Invalid fixed-point rounding mode specified\");
     }
     gcc_assert(0);
     return \"nop\";
  }"
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "satfractuns<mode>si2_convergent_32"
  [(set (match_operand:SI    0 "pic30_register_operand"  "=&r")
        (unsigned_sat_fract:SI
          (match_operand:QUQ15 1 "pic30_register_operand"  "r")))
   (clobber (match_scratch:QUQ15 2                         "=r"))
  ]
  ""
  "*
  {
     gcc_assert(GET_MODE_FBIT(<MODE>mode) == 15);
     if (pic30_fp_round_p() == pic30_convergent) {
       return \"asr %1,#14,%0\;\" /* as conventional */
              \"btss %0,#0\;\"
              \"clr  %0\;\"
              \"asr %0,#15,%d0\;\"
              \"sl %1,#2,%2\;\"    /* x.yz13 where z13 == 0 */
              \"btsc SR,#1\;\"    /*   always rounds to even 0 */
              \"mul.uu %0,#0,%0\";
     } else {
       error(\"Invalid fixed-point rounding mode specified\");
     }
     gcc_assert(0);
     return \"nop\";

  }"
  [
    (set_attr "cc" "clobber")
  ]
)

(define_expand "satfractuns<mode>si2_32"
  [(set (match_operand:SI    0 "pic30_register_operand"  "=r")
        (unsigned_sat_fract:SI
          (match_operand:QUQ15 1 "pic30_register_operand"  "r")))]
  ""
  "
  { rtx r0,r1;

    if (!pic30_fp_inline_p() && (pic30_fp_round_p() == pic30_convergent))
      FAIL;
    r0 = operands[0];
    r1 = operands[1];
    if (!pic30_register_operand(r0,SImode)) {
      r0 = gen_reg_rtx(SImode);
    }
    if (!pic30_register_operand(r1,VOIDmode)) {
      r1 = force_reg(<MODE>mode, r1);
    }
    if (pic30_fp_round_p() == pic30_convergent) {
      emit(
        gen_satfractuns<mode>si2_convergent_32(r0,r1)
      );
    } else {
      emit(
        gen_satfractuns<mode>si2_helper_32(r0,r1)
      );
    }
    if (r0 != operands[0]) {
      emit_move_insn(operands[0],r0);
    }
    DONE;
  }
  "
)

(define_insn "satfractuns<mode>di2_helper_32"
  [(set (match_operand:DI    0 "pic30_register_operand"  "=&r")
        (unsigned_sat_fract:DI
          (match_operand:QUQ15 1 "pic30_register_operand"  "r")))]
  ""
  "*
  {
     gcc_assert(GET_MODE_FBIT(<MODE>mode) == 15);
     if ((pic30_fp_round_p() == pic30_truncation) ||
         (pic30_fp_round_p() == pic30_fastest)) {
       /* chop the low order bits, signed results are either 0 or -1 */
       return \"asr %1,#15,%q0\;asr %1,#15,%t0\;mul.su %t0,#1,%0\";
     } else if (pic30_fp_round_p() == pic30_conventional) {
       /* [-1,-.5] round to -1, (-.5,.5) round to 0, otherwise 1? */
       return \"asr %1,#14,%0\;\"
              \"btss %0,#0\;\"
              \"clr %0\;\"
              \"asr %0,#15,%d0\;\"
              \"mul.su %d0,#1,%t0\";
     } else {
       error(\"Invalid fixed-point rounding mode specified\");
     }
     gcc_assert(0);
     return \"nop\";
  }"
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "satfractuns<mode>di2_convergent_32"
  [(set (match_operand:DI    0 "pic30_register_operand"  "=&r")
        (unsigned_sat_fract:DI
          (match_operand:QUQ15 1 "pic30_register_operand"  "r")))
   (clobber (match_scratch:QUQ15 2                         "=r"))
  ]
  ""
  "*
  {
     gcc_assert(GET_MODE_FBIT(<MODE>mode) == 15);
     if (pic30_fp_round_p() == pic30_convergent) {
       return \"asr %1,#14,%0\;\"  /* as conventional */
              \"btss %0,#0\;\"
              \"clr  %0\;\"
              \"asr %0,#15,%d0\;\"
              \"sl %1,#2,%2\;\"    /* x.yz13 where z13 == 0 */
              \"btsc SR,#1\;\"    /*   always rounds to even 0 */
              \"mul.uu %0,#0,%0\;\"
              \"mul.su %d0,#1,%t0\";
     } else {
       error(\"Invalid fixed-point rounding mode specified\");
     }
     gcc_assert(0);
     return \"nop\";

  }"
  [
    (set_attr "cc" "clobber")
  ]
)

(define_expand "satfractuns<mode>di2_32"
  [(set (match_operand:DI    0 "pic30_register_operand"  "=r")
        (unsigned_sat_fract:DI
          (match_operand:QUQ15 1 "pic30_register_operand"  "r")))]
  ""
  "
  { rtx r0,r1;

    if (!pic30_fp_inline_p() && 
        ((pic30_fp_round_p() == pic30_convergent) ||
         (pic30_fp_round_p() == pic30_conventional)))
      FAIL;
    r0 = operands[0];
    r1 = operands[1];
    if (!pic30_register_operand(r0,DImode)) {
      r0 = gen_reg_rtx(DImode);
    }
    if (!pic30_register_operand(r1,VOIDmode)) {
      r1 = force_reg(<MODE>mode, r1);
    }
    if (pic30_fp_round_p() == pic30_convergent) {
      emit(
        gen_satfractuns<mode>di2_convergent_32(r0,r1)
      );
    } else {
      emit(
        gen_satfractuns<mode>di2_helper_32(r0,r1)
      );
    }
    if (r0 != operands[0]) {
      emit_move_insn(operands[0],r0);
    }
    DONE;
  }
  "
)

; from Q31

(define_insn "satfractuns<mode>qi2_helper_32"
  [(set (match_operand:QI    0 "pic30_register_operand"  "=&r")
        (unsigned_sat_fract:QI
          (match_operand:QUQ31 1 "pic30_register_operand"  "r")))]
  ""
  "*
  {
     gcc_assert(GET_MODE_FBIT(<MODE>mode) == 31);
     if ((pic30_fp_round_p() == pic30_truncation) ||
         (pic30_fp_round_p() == pic30_fastest)) {
       /* chop the low order bits, signed results are either 0 or -1 */
       return \"asr %d1,#15,%0\"; 
     } else if (pic30_fp_round_p() == pic30_conventional) {
       /* [-1,-.5] round to -1, (-.5,.5) round to 0, otherwise 1? */
       return \"asr %d1,#14,%0\;btss %0,#0\;clr.b %0\";
     } else {
       error(\"Invalid fixed-point rounding mode specified\");
     }
     gcc_assert(0);
     return \"nop\";

  }"
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "satfractuns<mode>qi2_convergent_32"
  [(set (match_operand:QI    0 "pic30_register_operand"  "=&r")
        (unsigned_sat_fract:QI
          (match_operand:QUQ31 1 "pic30_register_operand"  "r")))
   (clobber (match_scratch:QUQ31 2                         "=&r"))
  ]
  ""
  "*
  {
     gcc_assert(GET_MODE_FBIT(<MODE>mode) == 31);
     if (pic30_fp_round_p() == pic30_convergent) {
       return \"asr %d1,#14,%0\;\"  /* as conventional */
              \"btss %0,#0\;\"
              \"clr.b %0\;\"
              \"sl %d1,#2,%2\;\"    /* x.yz13 where z13 == 0 */
              \"btsc SR,#1\;\"     /*   always rounds to even 0 */
              \"cp0 %1\;\"  
              \"btsc SR,#1\;\"
              \"clr %0\";
     } else {
       error(\"Invalid fixed-point rounding mode specified\");
     }
     gcc_assert(0);
     return \"nop\";

  }"
  [
    (set_attr "cc" "clobber")
  ]
)

(define_expand "satfractuns<mode>qi2_32"
  [(set (match_operand:QI    0 "pic30_register_operand"  "=r")
        (unsigned_sat_fract:QI
          (match_operand:QUQ31 1 "pic30_register_operand"  "r")))]
  ""
  "
  { rtx r0,r1;

    if (!pic30_fp_inline_p() && (pic30_fp_round_p() == pic30_convergent))
      FAIL;
    r0 = operands[0];
    r1 = operands[1];
    if (!pic30_register_operand(r0,QImode)) {
      r0 = gen_reg_rtx(QImode);
    }
    if (!pic30_register_operand(r1,VOIDmode)) {
      r1 = force_reg(<MODE>mode, r1);
    }
    if (pic30_fp_round_p() == pic30_convergent) {
      emit(
        gen_satfract<mode>qi2_convergent_32(r0,r1)
      );
    } else {
      emit(
        gen_satfractuns<mode>qi2_helper_32(r0,r1)
      );
    }
    if (r0 != operands[0]) {
      emit_move_insn(operands[0],r0);
    }
    DONE;
  }
  "
)

(define_insn "satfractuns<mode>hi2_helper_32"
  [(set (match_operand:HI    0 "pic30_register_operand"  "=&r")
        (unsigned_sat_fract:HI
          (match_operand:QUQ31 1 "pic30_register_operand"  "r")))]
  ""
  "*
  {
     gcc_assert(GET_MODE_FBIT(<MODE>mode) == 31);
     if ((pic30_fp_round_p() == pic30_truncation) ||
         (pic30_fp_round_p() == pic30_fastest)) {
       /* chop the low order bits, signed results are either 0 or -1 */
       return \"asr %d1,#15,%0\"; 
     } else if (pic30_fp_round_p() == pic30_conventional) {
       /* [-1,-.5] round to -1, (-.5,.5) round to 0, otherwise 1? */
       return \"asr %d1,#14,%0\;btss %0,#0\;clr %0\";
     } else {
       error(\"Invalid fixed-point rounding mode specified\");
     }
     gcc_assert(0);
     return \"nop\";

  }"
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "satfractuns<mode>hi2_convergent_32"
  [(set (match_operand:HI    0 "pic30_register_operand"  "=&r")
        (unsigned_sat_fract:HI
          (match_operand:QUQ31 1 "pic30_register_operand"  "r")))
   (clobber (match_scratch:QUQ31 2                         "=&r"))
  ]
  ""
  "*
  {
     gcc_assert(GET_MODE_FBIT(<MODE>mode) == 31);
     if (pic30_fp_round_p() == pic30_convergent) {
       return \"asr %d1,#14,%0\;\"  /* as conventional */
              \"btss %0,#0\;\"
              \"clr %0\;\"
              \"sl %d1,#2,%2\;\"    /* x.yz13 where z13 == 0 */
              \"btsc SR,#1\;\"     /*   always rounds to even 0 */
              \"cp0 %1\;\"  
              \"btsc SR,#1\;\"
              \"clr %0\";
     } else {
       error(\"Invalid fixed-point rounding mode specified\");
     }
     gcc_assert(0);
     return \"nop\";

  }"
  [
    (set_attr "cc" "clobber")
  ]
)

(define_expand "satfractuns<mode>hi2_32"
  [(set (match_operand:HI    0 "pic30_register_operand"  "=r")
        (unsigned_sat_fract:HI
          (match_operand:QUQ31 1 "pic30_register_operand"  "r")))]
  ""
  "
  { rtx r0,r1;

    if (!pic30_fp_inline_p() && (pic30_fp_round_p() == pic30_convergent))
      FAIL;
    r0 = operands[0];
    r1 = operands[1];
    if (!pic30_register_operand(r0,HImode)) {
      r0 = gen_reg_rtx(HImode);
    }
    if (!pic30_register_operand(r1,VOIDmode)) {
      r1 = force_reg(<MODE>mode, r1);
    }
    if (pic30_fp_round_p() == pic30_convergent) {
      emit(
        gen_satfractuns<mode>hi2_convergent_32(r0,r1)
      );
    } else {
      emit(
        gen_satfractuns<mode>hi2_helper_32(r0,r1)
      );
    }
    if (r0 != operands[0]) {
      emit_move_insn(operands[0],r0);
    }
    DONE;
  }
  "
)

(define_insn "satfractuns<mode>si2_helper_32"
  [(set (match_operand:SI    0 "pic30_register_operand"  "=&r")
        (unsigned_sat_fract:SI
          (match_operand:QUQ31 1 "pic30_register_operand"  "r")))]
  ""
  "*
  {
     gcc_assert(GET_MODE_FBIT(<MODE>mode) == 31);
     if ((pic30_fp_round_p() == pic30_truncation) ||
         (pic30_fp_round_p() == pic30_fastest)) {
       /* chop the low order bits, signed results are either 0 or -1 */
       return \"asr %d1,#15,%d0\;asr %d1,#15,%0\";
     } else if (pic30_fp_round_p() == pic30_conventional) {
       /* [-1,-.5] round to -1, (-.5,.5) round to 0, otherwise 1? */
       return \"asr %d1,#14,%0\;\"
              \"btss %0,#0\;\"
              \"clr %0\;\"
              \"asr %0,#15,%d0\";
     } else {
       error(\"Invalid fixed-point rounding mode specified\");
     }
     gcc_assert(0);
     return \"nop\";
  }"
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "satfractuns<mode>si2_convergent_32"
  [(set (match_operand:SI    0 "pic30_register_operand"  "=&r")
        (unsigned_sat_fract:SI
          (match_operand:QUQ31 1 "pic30_register_operand"  "r")))
   (clobber (match_scratch:QUQ31 2                         "=&r"))
  ]
  ""
  "*
  {
     gcc_assert(GET_MODE_FBIT(<MODE>mode) == 31);
     if (pic30_fp_round_p() == pic30_convergent) {
       return \"asr %d1,#14,%0\;\"  /* as conventional */
              \"btss %0,#0\;\"
              \"clr  %0\;\"
              \"asr %0,#15,%d0\;\"
              \"sl %d1,#2,%2\;\"    /* x.yz13 where z13 == 0 */
              \"btsc SR,#1\;\"     /*   always rounds to even 0 */
              \"cp0 %1\;\"
              \"btsc SR,#1\;\"
              \"mul.uu %0,#0,%0\";
     } else {
       error(\"Invalid fixed-point rounding mode specified\");
     }
     gcc_assert(0);
     return \"nop\";

  }"
  [
    (set_attr "cc" "clobber")
  ]
)

(define_expand "satfractuns<mode>si2_32"
  [(set (match_operand:SI    0 "pic30_register_operand"  "=r")
        (unsigned_sat_fract:SI
          (match_operand:QUQ31 1 "pic30_register_operand"  "r")))]
  ""
  "
  { rtx r0,r1;

    if (!pic30_fp_inline_p() && (pic30_fp_round_p() == pic30_convergent))
      FAIL;
    r0 = operands[0];
    r1 = operands[1];
    if (!pic30_register_operand(r0,SImode)) {
      r0 = gen_reg_rtx(SImode);
    }
    if (!pic30_register_operand(r1,VOIDmode)) {
      r1 = force_reg(<MODE>mode, r1);
    }
    if (pic30_fp_round_p() == pic30_convergent) {
      emit(
        gen_satfractuns<mode>si2_convergent_32(r0,r1)
      );
    } else {
      emit(
        gen_satfractuns<mode>si2_helper_32(r0,r1)
      );
    }
    if (r0 != operands[0]) {
      emit_move_insn(operands[0],r0);
    }
    DONE;
  }
  "
)

(define_insn "satfractuns<mode>di2_helper_32"
  [(set (match_operand:DI    0 "pic30_register_operand"  "=&r")
        (unsigned_sat_fract:DI
          (match_operand:QUQ31 1 "pic30_register_operand"  "r")))]
  ""
  "*
  {
     gcc_assert(GET_MODE_FBIT(<MODE>mode) == 31);
     if ((pic30_fp_round_p() == pic30_truncation) ||
         (pic30_fp_round_p() == pic30_fastest)) {
       /* chop the low order bits, signed results are either 0 or -1 */
       return \"asr %d1,#15,%q0\;asr %d1,#15,%t0\;mul.su %t0,#1,%0\";
     } else if (pic30_fp_round_p() == pic30_conventional) {
       /* [-1,-.5] round to -1, (-.5,.5) round to 0, otherwise 1? */
       return \"asr %d1,#14,%0\;\"
              \"btss %0,#0\;\"
              \"clr %0\;\"
              \"asr %0,#15,%d0\;\"
              \"mul.su %d0,#1,%t0\";
     } else {
       error(\"Invalid fixed-point rounding mode specified\");
     }
     gcc_assert(0);
     return \"nop\";
  }"
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "satfractuns<mode>di2_convergent_32"
  [(set (match_operand:DI    0 "pic30_register_operand"  "=&r")
        (unsigned_sat_fract:DI
          (match_operand:QUQ31 1 "pic30_register_operand"  "r")))
   (clobber (match_scratch:QUQ31 2                         "=&r"))
  ]
  ""
  "*
  {
     gcc_assert(GET_MODE_FBIT(<MODE>mode) == 31);
     if (pic30_fp_round_p() == pic30_convergent) {
       return \"asr %d1,#14,%0\;\"  /* as conventional */
              \"btss %0,#0\;\"
              \"clr  %0\;\"
              \"asr %0,#15,%d0\;\"
              \"sl %d1,#2,%2\;\"    /* x.yz13 where z13 == 0 */
              \"btsc SR,#1\;\"     /*   always rounds to even 0 */
              \"cp0 %1\;\"
              \"btsc SR,#1\;\"
              \"mul.uu %0,#0,%0\;\"
              \"mul.su %d0,#1,%t0\";
     } else {
       error(\"Invalid fixed-point rounding mode specified\");
     }
     gcc_assert(0);
     return \"nop\";

  }"
  [
    (set_attr "cc" "clobber")
  ]
)

(define_expand "satfractuns<mode>di2_32"
  [(set (match_operand:DI    0 "pic30_register_operand"  "=r")
        (unsigned_sat_fract:DI
          (match_operand:QUQ31 1 "pic30_register_operand"  "r")))]
  ""
  "
  { rtx r0,r1;

    if (!pic30_fp_inline_p() && 
         ((pic30_fp_round_p() == pic30_convergent) ||
          (pic30_fp_round_p() == pic30_convergent)))
      FAIL;
    r0 = operands[0];
    r1 = operands[1];
    if (!pic30_register_operand(r0,DImode)) {
      r0 = gen_reg_rtx(DImode);
    }
    if (!pic30_register_operand(r1,VOIDmode)) {
      r1 = force_reg(<MODE>mode, r1);
    }
    if (pic30_fp_round_p() == pic30_convergent) {
      emit(
        gen_satfractuns<mode>di2_convergent_32(r0,r1)
      );
    } else {
      emit(
        gen_satfractuns<mode>di2_helper_32(r0,r1)
      );
    }
    if (r0 != operands[0]) {
      emit_move_insn(operands[0],r0);
    }
    DONE;
  }
  "
)

; to Q15

(define_insn "satfractunsqi<mode>2_32"
  [(set (match_operand:QUQ15  0 "pic30_register_operand"  "=r")
        (unsigned_sat_fract:QUQ15
          (match_operand:QI 1 "pic30_register_operand"  "r")))]
  "pic30_fp_inline_p()"
  "*
   {  
      return \"cp0.b %1\;\"        /* compare input with 0 */
             \"mov  #0x8000,%0\;\" /* assume -1 */
             \"btsc SR,#0\;\"     /* positive => 0 or .99 */
             \"mov #0x7FFF,%0\;\"  /*   load .99... */
             \"btsc SR,#1\;\"     /* if zero */
             \"clr %0\";           /*   load zero */
   }
  "
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "satfractunshi<mode>2_32"
  [(set (match_operand:QUQ15  0 "pic30_register_operand"  "=r")
        (unsigned_sat_fract:QUQ15
          (match_operand:HI 1 "pic30_register_operand"  "r")))]
  "pic30_fp_inline_p()"
  "*
   {
      return \"cp0  %1\;\"         /* compare input with 0 */
             \"mov  #0x8000,%0\;\" /* assume -1 */
             \"btsc SR,#0\;\"     /* positive => 0 or .99 */
             \"mov #0x7FFF,%0\;\"  /*   load .99... */
             \"btsc SR,#1\;\"     /* if zero */
             \"clr %0\";           /*   load zero */
   }
  "
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "satfractunssi<mode>2_32"
  [(set (match_operand:QUQ15  0 "pic30_register_operand"  "=r")
        (unsigned_sat_fract:QUQ15
          (match_operand:SI 1 "pic30_register_operand"  "r")))]
  "pic30_fp_inline_p()"
  "*
   {  
      return \"subr %1,#0,[w15]\;\"   /* compare input with 0 */
             \"subbr %d1,#0,[w15]\;\" /* compare input with 0 */
             \"mov  #0x8000,%0\;\"    /* assume -1 */
             \"btsc SR,#0\;\"        /* positive => 0 or .99 */
             \"mov #0x7FFF,%0\;\"     /*   load .99... */
             \"btsc SR,#1\;\"        /* if zero */
             \"clr %0\";              /*   load zero */
   }
  "
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "satfractunsdi<mode>2_32"
  [(set (match_operand:QUQ15  0 "pic30_register_operand"  "=r")
        (unsigned_sat_fract:QUQ15
          (match_operand:DI 1 "pic30_register_operand"  "r")))]
  "pic30_fp_inline_p()"
  "*
   {  
      return \"subr %1,#0,[w15]\;\"   /* compare input with 0 */
             \"subbr %d1,#0,[w15]\;\" /* compare input with 0 */
             \"subbr %t1,#0,[w15]\;\" /* compare input with 0 */
             \"subbr %q1,#0,[w15]\;\" /* compare input with 0 */
             \"mov  #0x8000,%0\;\"    /* assume -1 */
             \"btsc SR,#0\;\"        /* positive => 0 or .99 */
             \"mov #0x7FFF,%0\;\"     /*   load .99... */
             \"btsc SR,#1\;\"        /* if zero */
             \"clr %0\";              /*   load zero */
   }
  "
  [
    (set_attr "cc" "clobber")
  ]
)

; to Q31

(define_insn "satfractunsqi<mode>2_32"
  [(set (match_operand:QUQ31  0 "pic30_register_operand"  "=r")
        (unsigned_sat_fract:QUQ31
          (match_operand:QI 1 "pic30_register_operand"  "r")))]
  "pic30_fp_inline_p()"
  "*
   {  
      return 
             \"cp0.b %1\;\"         /* compare input with 0 */
             \"mov #0x8000,%d0\;\"  /* assume -1 */
             \"btsc SR,#0\;\"      /* positive => 0 or .99 */
             \"mov #0x7FFF,%d0\;\"  /*   load .99... */
             \"btsc SR,#1\;\"      /* if zero */
             \"clr %d0\;\"          /*   load zero */
             \"sl  %d0,%0\;\"       /* form low word by propagating non sign */
             \"asr %0,#15,%0\";     /*   bits into low word */
   }
  "
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "satfractunshi<mode>2_32"
  [(set (match_operand:QUQ31  0 "pic30_register_operand"  "=r")
        (unsigned_sat_fract:QUQ31
          (match_operand:HI 1 "pic30_register_operand"  "r")))]
  "pic30_fp_inline_p()"
  "*
   {
      return 
             \"cp0.b %1\;\"         /* compare input with 0 */
             \"mov #0x8000,%d0\;\"  /* assume -1 */
             \"btsc SR,#0\;\"      /* positive => 0 or .99 */
             \"mov #0x7FFF,%d0\;\"  /*   load .99... */
             \"btsc SR,#1\;\"      /* if zero */
             \"clr %d0\;\"          /*   load zero */
             \"sl  %d0,%0\;\"       /* form low word by propagating non sign */
             \"asr %0,#15,%0\";     /*   bits into low word */
   }
  "
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "satfractunssi<mode>2_32"
  [(set (match_operand:QUQ31  0 "pic30_register_operand"  "=r")
        (unsigned_sat_fract:QUQ31
          (match_operand:SI 1 "pic30_register_operand"  "r")))]
  "pic30_fp_inline_p()"
  "*
   {  
      return 
             \"sub %1,#0,[w15]\;\"   /* compare input with 0 */
             \"subb %d1,#0,[w15]\;\" /* compare input with 0 */
             \"mov #0x8000,%d0\;\"   /* assume -1 */
             \"btsc SR,#0\;\"       /* positive => 0 or .99 */
             \"mov #0x7FFF,%d0\;\"   /*   load .99... */
             \"btsc SR,#1\;\"       /* if zero */
             \"clr %d0\;\"           /*   load zero */
             \"sl  %d0,%0\;\"        /* form low word by propagating non sign */
             \"asr %0,#15,%0\";      /*   bits into low word */
   }
  "
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "satfractunsdi<mode>2_32"
  [(set (match_operand:QUQ31  0 "pic30_register_operand"  "=r")
        (unsigned_sat_fract:QUQ31
          (match_operand:DI 1 "pic30_register_operand"  "r")))]
  "pic30_fp_inline_p()"
  "*
   {  
      return 
             \"sub %1,#0,[w15]\;\"   /* compare input with 0 */
             \"subb %d1,#0,[w15]\;\" /* compare input with 0 */
             \"subb %t1,#0,[w15]\;\" /* compare input with 0 */
             \"subb %q1,#0,[w15]\;\" /* compare input with 0 */
             \"mov #0x8000,%d0\;\"   /* assume -1 */
             \"btsc SR,#0\;\"       /* positive => 0 or .99 */
             \"mov #0x7FFF,%d0\;\"   /*   load .99... */
             \"btsc SR,#1\;\"       /* if zero */
             \"clr %d0\;\"           /*   load zero */
             \"sl  %d0,%0\;\"        /* form low word by propagating non sign */
             \"asr %0,#15,%0\";      /*   bits into low word */
   }
  "
  [
    (set_attr "cc" "clobber")
  ]
)


;; conversions back and forth to floating point or double will be done by
;;  out of line functions only

;; conversions to and from fractional types
;;   some of these can actually cause saturation

(define_insn "satfractsq<mode>2_helper_32"
  [(set (match_operand:QUQ15 0 "pic30_register_operand" "=&r")
        (sat_fract:QUQ15
          (match_operand:SQ  1 "pic30_register_operand" "r")))]
  ""
  "*
  {
    if ((pic30_fp_round_p() == pic30_truncation) ||
        (pic30_fp_round_p() == pic30_fastest)) {
      return \"mov %d1,%0\";
    } else if (pic30_fp_round_p() == pic30_conventional) {
      return \"sl %1,[w15]\;\"       /* shift overflow bit to carry */
             \"addc %d1,#0,%0\;\"    /* add */
             \"bra nov,.L1_%=\;\"    /* no overflow? finished */
             \"mov #0x7FFF,%0\;\"    /* return ~1 */
             \"btsc %d1,#15\;\"
             \"com %0,%0\n\"         /* or -1 */
             \".L1_%=:\";
    } else {
       error(\"Invalid fixed-point rounding mode specified\");
    }
     gcc_assert(0);
     return \"nop\";
  }
  "
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "satfractsq<mode>2_convergent_32"
  [(set (match_operand:QUQ15  0 "pic30_register_operand" "=&r")
        (sat_fract:QUQ15
          (match_operand:SQ   1 "pic30_register_operand" "r")))
   (clobber (match_scratch:HI 2                          "=&r"))
  ]
  ""
  "*
  {
    if (pic30_fp_round_p() == pic30_convergent) {
      return \"setm %2\;\"
             \"sl %1,[w15]\;\"          /* get top bit into carry */
             \"btsc SR,#1\;\"          /* if the result is 0 */
             \"mov #0xFFFE,%2\;\"       /*   mask off low bit */
             \"addc %d1,#0,%0\;\"       /* add in 'overflow' */
             \"bra nov,.L1_%=\;\"       /* nov ? continue */
             \"mov #0x7FFF,%0\;\"       /* return ~1 */
             \"btsc %d1,#15\;\"
             \"com %0,%0\;\"            /* or -1 */
             \"setm %2\;\"              /* prevent mask */
             \".L1_%=:\;\"
             \"and %0,%2,%0\";          /* maybe mask off low bit */
    } else {
       error(\"Invalid fixed-point rounding mode specified\");
    }
     gcc_assert(0);
     return \"nop\";
  }
  "
  [
    (set_attr "cc" "clobber")
  ]
)

(define_expand "satfractsq<mode>2_32"
  [(set (match_operand:QUQ15 0 "pic30_register_operand"  "=r")
        (sat_fract:QUQ15
          (match_operand:SQ  1 "pic30_register_operand"  "r")))]
  ""
  "
  { rtx r0,r1;

    if (!pic30_fp_inline_p() && 
         ((pic30_fp_round_p() == pic30_convergent) ||
          (pic30_fp_round_p() == pic30_conventional)))
      FAIL;
    r0 = operands[0];
    r1 = operands[1];
    if (!pic30_register_operand(r0,SImode)) {
      r0 = gen_reg_rtx(<MODE>mode);
    }
    if (!pic30_register_operand(r1,VOIDmode)) {
      r1 = force_reg(<MODE>mode, r1);
    }
    if (pic30_fp_round_p() == pic30_convergent) {
      emit(
        gen_satfractsq<mode>2_convergent_32(r0,r1)
      );
    } else {
      emit(
        gen_satfractsq<mode>2_helper_32(r0,r1)
      );
    }
    if (r0 != operands[0]) {
      emit_move_insn(operands[0],r0);
    }
    DONE;
  }
  "
)

(define_insn "satfractusq<mode>2_helper_32"
  [(set (match_operand:QUQ15 0 "pic30_register_operand" "=&r")
        (sat_fract:QUQ15
          (match_operand:USQ 1 "pic30_register_operand" "r")))]
  ""
  "*
  {
    if ((pic30_fp_round_p() == pic30_truncation) ||
        (pic30_fp_round_p() == pic30_fastest)) {
      return \"mov %d1,%0\";
    } else if (pic30_fp_round_p() == pic30_conventional) {
      return \"sl %1,[w15]\;\"       /* shift overflow bit to carry */
             \"addc %d1,#0,%0\;\"    /* add */
             \"bra nov,.L1_%=\;\"    /* no overflow? finished */
             \"mov #0x7FFF,%0\;\"    /* return ~1 */
             \"btsc %d1,#15\;\"      
             \"com %0,%0\n\"         /* or -1 */
             \".L1_%=:\";
    } else {
       error(\"Invalid fixed-point rounding mode specified\");
    }
     gcc_assert(0);
     return \"nop\";
  }
  "
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "satfractusq<mode>2_convergent_32"
  [(set (match_operand:QUQ15  0 "pic30_register_operand" "=&r")
        (sat_fract:QUQ15
          (match_operand:USQ  1 "pic30_register_operand" "r")))
   (clobber (match_scratch:HI 2                          "=&r"))
  ]
  ""
  "*
  {
    if (pic30_fp_round_p() == pic30_convergent) {
      return \"setm %2\;\"
             \"sl %1,[w15]\;\"          /* get top bit into carry */
             \"btsc SR,#1\;\"          /* if the result is 0 */
             \"mov #0xFFFE,%2\;\"       /*   mask off low bit */
             \"addc %d1,#0,%0\;\"       /* add in 'overflow' */
             \"bra nov,.L1_%=\;\"       /* nov ? continue */
             \"mov #0x7FFF,%0\;\"       /* return ~1 */
             \"btsc %d1,#15\;\" 
             \"com %0,%0\;\"            /* or -1 */
             \"setm %2\n\"              /* prevent mask */
             \".L1_%=:\;\"
             \"and %0,%2,%0\";          /* maybe mask off low bit */
    } else { 
       error(\"Invalid fixed-point rounding mode specified\");
    }  
     gcc_assert(0);
     return \"nop\";
  } 
  "
  [
    (set_attr "cc" "clobber")
  ]
) 

(define_expand "satfractusq<mode>2_32"
  [(set (match_operand:QUQ15 0 "pic30_register_operand"  "=r")
        (sat_fract:QUQ15
          (match_operand:USQ 1 "pic30_register_operand"  "r")))]
  ""      
  "
  { rtx r0,r1;

    if (!pic30_fp_inline_p() && 
         ((pic30_fp_round_p() == pic30_convergent) ||
          (pic30_fp_round_p() == pic30_conventional)))
      FAIL;
    r0 = operands[0];
    r1 = operands[1];
    if (!pic30_register_operand(r0,SImode)) {
      r0 = gen_reg_rtx(<MODE>mode);
    } 
    if (!pic30_register_operand(r1,VOIDmode)) {
      r1 = force_reg(<MODE>mode, r1);
    } 
    if (pic30_fp_round_p() == pic30_convergent) {
      emit(
        gen_satfractusq<mode>2_convergent_32(r0,r1)
      );
    } else {
      emit(
        gen_satfractusq<mode>2_helper_32(r0,r1)
      );
    }
    if (r0 != operands[0]) {
      emit_move_insn(operands[0],r0);
    }
    DONE;
  }
  "
)

(define_insn "satfractdq<mode>2_helper_32"
  [(set (match_operand:QUQ15 0 "pic30_register_operand" "=&r")
        (sat_fract:QUQ15
          (match_operand:DQ  1 "pic30_register_operand" "r")))]
  ""
  "*
  {
    if ((pic30_fp_round_p() == pic30_truncation) ||
        (pic30_fp_round_p() == pic30_fastest)) {
      return \"mov %d1,%0\";
    } else if (pic30_fp_round_p() == pic30_conventional) {
      return \"sl %1,[w15]\;\"       /* shift overflow bit to carry */
             \"addc %d1,#0,%0\;\"    /* add */
             \"bra nov,.L1_%=\;\"    /* no overflow? finished */
             \"mov #0x7FFF,%0\;\"    /* return ~1 */
             \"btsc %d1,#15\;\"      
             \"com %0,%0\n\"         /* or -1 */
             \".L1_%=:\";
    } else {
       error(\"Invalid fixed-point rounding mode specified\");
    }
     gcc_assert(0);
     return \"nop\";
  }
  "
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "satfractdq<mode>2_convergent_32"
  [(set (match_operand:QUQ15  0 "pic30_register_operand" "=&r")
        (sat_fract:QUQ15
          (match_operand:DQ   1 "pic30_register_operand" "r")))
   (clobber (match_scratch:HI 2                          "=&r"))
  ]
  ""
  "*
  {
    if (pic30_fp_round_p() == pic30_convergent) {
      return \"setm %2\;\"
             \"sl %1,[w15]\;\"          /* get top bit into carry */
             \"btsc SR,#1\;\"          /* if the result is 0 */
             \"mov #0xFFFE,%2\;\"       /*   mask off low bit */
             \"addc %d1,#0,%0\;\"       /* add in 'overflow' */
             \"bra nov,.L1_%=\;\"       /* nov ? continue */
             \"mov #0x7FFF,%0\;\"       /* return ~1 */
             \"btsc %d1,#15\;\" 
             \"com %0,%0\;\"            /* or -1 */
             \"setm %2\n\"              /* prevent mask */
             \".L1_%=:\;\"
             \"and %0,%2,%0\";          /* maybe mask off low bit */
    } else { 
       error(\"Invalid fixed-point rounding mode specified\");
    }  
     gcc_assert(0);
     return \"nop\";
  } 
  "
  [
    (set_attr "cc" "clobber")
  ]
) 

(define_expand "satfractdq<mode>2_32"
  [(set (match_operand:QUQ15 0 "pic30_register_operand"  "=r")
        (sat_fract:QUQ15
          (match_operand:DQ  1 "pic30_register_operand"  "r")))]
  ""      
  "
  { rtx r0,r1;

    if (!pic30_fp_inline_p() && 
         ((pic30_fp_round_p() == pic30_convergent) ||
          (pic30_fp_round_p() == pic30_conventional)))
      FAIL;
    r0 = operands[0];
    r1 = operands[1];
    if (!pic30_register_operand(r0,SImode)) {
      r0 = gen_reg_rtx(<MODE>mode);
    } 
    if (!pic30_register_operand(r1,VOIDmode)) {
      r1 = force_reg(<MODE>mode, r1);
    } 
    if (pic30_fp_round_p() == pic30_convergent) {
      emit(
        gen_satfractdq<mode>2_convergent_32(r0,r1)
      );
    } else {
      emit(
        gen_satfractdq<mode>2_helper_32(r0,r1)
      );
    }
    if (r0 != operands[0]) {
      emit_move_insn(operands[0],r0);
    }
    DONE;
  }
  "
)
(define_insn "satfractudq<mode>2_helper_32"
  [(set (match_operand:QUQ15 0 "pic30_register_operand" "=&r")
        (sat_fract:QUQ15
          (match_operand:UDQ 1 "pic30_register_operand" "r")))]
  ""
  "*
  {
    if ((pic30_fp_round_p() == pic30_truncation) ||
        (pic30_fp_round_p() == pic30_fastest)) {
      return \"mov %d1,%0\";
    } else if (pic30_fp_round_p() == pic30_conventional) {
      return \"sl %1,[w15]\;\"       /* shift overflow bit to carry */
             \"addc %d1,#0,%0\;\"    /* add */
             \"bra nov,.L1_%=\;\"    /* no overflow? finished */
             \"mov #0x7FFF,%0\;\"    /* return ~1 */
             \"btsc %d1,#15\;\"      
             \"com %0,%0\n\"         /* or -1 */
             \".L1_%=:\";
    } else {
       error(\"Invalid fixed-point rounding mode specified\");
    }
     gcc_assert(0);
     return \"nop\";
  }
  "
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "satfractudq<mode>2_convergent_32"
  [(set (match_operand:QUQ15  0 "pic30_register_operand" "=&r")
        (sat_fract:QUQ15
          (match_operand:UDQ   1 "pic30_register_operand" "r")))
   (clobber (match_scratch:HI 2                          "=&r"))
  ]
  ""
  "*
  {
    if (pic30_fp_round_p() == pic30_convergent) {
      return \"setm %2\;\"
             \"sl %1,[w15]\;\"          /* get top bit into carry */
             \"btsc SR,#1\;\"          /* if the result is 0 */
             \"mov #0xFFFE,%2\;\"       /*   mask off low bit */
             \"addc %d1,#0,%0\;\"       /* add in 'overflow' */
             \"bra nov,.L1_%=\;\"       /* nov ? continue */
             \"mov #0x7FFF,%0\;\"       /* return ~1 */
             \"btsc %d1,#15\;\" 
             \"com %0,%0\;\"            /* or -1 */
             \"setm %2\n\"              /* prevent mask */
             \".L1_%=:\;\"
             \"and %0,%2,%0\";          /* maybe mask off low bit */
    } else { 
       error(\"Invalid fixed-point rounding mode specified\");
    }  
     gcc_assert(0);
     return \"nop\";
  } 
  "
  [
    (set_attr "cc" "clobber")
  ]
) 

(define_expand "satfractudq<mode>2_32"
  [(set (match_operand:QUQ15 0 "pic30_register_operand"  "=r")
        (sat_fract:QUQ15
          (match_operand:UDQ 1 "pic30_register_operand"  "r")))]
  ""      
  "
  { rtx r0,r1;

    if (!pic30_fp_inline_p() && 
         ((pic30_fp_round_p() == pic30_convergent) ||
          (pic30_fp_round_p() == pic30_conventional)))
      FAIL;
    r0 = operands[0];
    r1 = operands[1];
    if (!pic30_register_operand(r0,SImode)) {
      r0 = gen_reg_rtx(<MODE>mode);
    } 
    if (!pic30_register_operand(r1,VOIDmode)) {
      r1 = force_reg(<MODE>mode, r1);
    } 
    if (pic30_fp_round_p() == pic30_convergent) {
      emit(
        gen_satfractudq<mode>2_convergent_32(r0,r1)
      );
    } else {
      emit(
        gen_satfractudq<mode>2_helper_32(r0,r1)
      );
    }
    if (r0 != operands[0]) {
      emit_move_insn(operands[0],r0);
    }
    DONE;
  }
  "
)

(define_insn "satfracttq<mode>2_helper_32"
  [(set (match_operand:QUQ15 0 "pic30_register_operand" "=&r")
        (sat_fract:QUQ15
          (match_operand:TQ  1 "pic30_register_operand" "r")))]
  ""
  "*
  {
    if ((pic30_fp_round_p() == pic30_truncation) ||
        (pic30_fp_round_p() == pic30_fastest)) {
      return \"mov %d1,%0\";
    } else if (pic30_fp_round_p() == pic30_conventional) {
      return \"sl %1,[w15]\;\"       /* shift overflow bit to carry */
             \"addc %d1,#0,%0\;\"    /* add */
             \"bra nov,.L1_%=\;\"    /* no overflow? finished */
             \"mov #0x7FFF,%0\;\"    /* return ~1 */
             \"btsc %d1,#15\;\"      
             \"com %0,%0\n\"         /* or -1 */
             \".L1_%=:\";
    } else {
       error(\"Invalid fixed-point rounding mode specified\");
    }
     gcc_assert(0);
     return \"nop\";
  }
  "
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "satfracttq<mode>2_convergent_32"
  [(set (match_operand:QUQ15  0 "pic30_register_operand" "=&r")
        (sat_fract:QUQ15
          (match_operand:TQ   1 "pic30_register_operand" "r")))
   (clobber (match_scratch:HI 2                          "=&r"))
  ]
  ""
  "*
  {
    if (pic30_fp_round_p() == pic30_convergent) {
      return \"setm %2\;\"
             \"sl %1,[w15]\;\"          /* get top bit into carry */
             \"btsc SR,#1\;\"          /* if the result is 0 */
             \"mov #0xFFFE,%2\;\"       /*   mask off low bit */
             \"addc %d1,#0,%0\;\"       /* add in 'overflow' */
             \"bra nov,.L1_%=\;\"       /* nov ? continue */
             \"mov #0x7FFF,%0\;\"       /* return ~1 */
             \"btsc %d1,#15\;\" 
             \"com %0,%0\;\"            /* or -1 */
             \"setm %2\n\"              /* prevent mask */
             \".L1_%=:\;\"
             \"and %0,%2,%0\";          /* maybe mask off low bit */
    } else { 
       error(\"Invalid fixed-point rounding mode specified\");
    }  
     gcc_assert(0);
     return \"nop\";
  } 
  "
  [
    (set_attr "cc" "clobber")
  ]
) 

(define_expand "satfracttq<mode>2_32"
  [(set (match_operand:QUQ15 0 "pic30_register_operand"  "=r")
        (sat_fract:QUQ15
          (match_operand:TQ  1 "pic30_register_operand"  "r")))]
  ""      
  "
  { rtx r0,r1;

    if (!pic30_fp_inline_p() && 
         ((pic30_fp_round_p() == pic30_convergent) ||
          (pic30_fp_round_p() == pic30_conventional)))
      FAIL;
    r0 = operands[0];
    r1 = operands[1];
    if (!pic30_register_operand(r0,SImode)) {
      r0 = gen_reg_rtx(<MODE>mode);
    } 
    if (!pic30_register_operand(r1,VOIDmode)) {
      r1 = force_reg(<MODE>mode, r1);
    } 
    if (pic30_fp_round_p() == pic30_convergent) {
      emit(
        gen_satfracttq<mode>2_convergent_32(r0,r1)
      );
    } else {
      emit(
        gen_satfracttq<mode>2_helper_32(r0,r1)
      );
    }
    if (r0 != operands[0]) {
      emit_move_insn(operands[0],r0);
    }
    DONE;
  }
  "
)

(define_insn "satfractutq<mode>2_helper_32"
  [(set (match_operand:QUQ15 0 "pic30_register_operand" "=&r")
        (sat_fract:QUQ15
          (match_operand:UTQ 1 "pic30_register_operand" "r")))]
  ""
  "*
  {
    if ((pic30_fp_round_p() == pic30_truncation) ||
        (pic30_fp_round_p() == pic30_fastest)) {
      return \"mov %d1,%0\";
    } else if (pic30_fp_round_p() == pic30_conventional) {
      return \"sl %1,[w15]\;\"       /* shift overflow bit to carry */
             \"addc %d1,#0,%0\;\"    /* add */
             \"bra nov,.L1_%=\;\"    /* no overflow? finished */
             \"mov #0x7FFF,%0\;\"    /* return ~1 */
             \"btsc %d1,#15\;\"      
             \"com %0,%0\n\"         /* or -1 */
             \".L1_%=:\";
    } else {
       error(\"Invalid fixed-point rounding mode specified\");
    }
     gcc_assert(0);
     return \"nop\";
  }
  "
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "satfractutq<mode>2_convergent_32"
  [(set (match_operand:QUQ15  0 "pic30_register_operand" "=&r")
        (sat_fract:QUQ15
          (match_operand:UTQ  1 "pic30_register_operand" "r")))
   (clobber (match_scratch:HI 2                          "=&r"))
  ]
  ""
  "*
  {
    if (pic30_fp_round_p() == pic30_convergent) {
      return \"setm %2\;\"
             \"sl %1,[w15]\;\"          /* get top bit into carry */
             \"btsc SR,#1\;\"          /* if the result is 0 */
             \"mov #0xFFFE,%2\;\"       /*   mask off low bit */
             \"addc %d1,#0,%0\;\"       /* add in 'overflow' */
             \"bra nov,.L1_%=\;\"       /* nov ? continue */
             \"mov #0x7FFF,%0\;\"       /* return ~1 */
             \"btsc %d1,#15\;\" 
             \"com %0,%0\;\"            /* or -1 */
             \"setm %2\n\"              /* prevent mask */
             \".L1_%=:\;\"
             \"and %0,%2,%0\";          /* maybe mask off low bit */
    } else { 
       error(\"Invalid fixed-point rounding mode specified\");
    }  
     gcc_assert(0);
     return \"nop\";
  } 
  "
  [
    (set_attr "cc" "clobber")
  ]
) 

(define_expand "satfractutq<mode>2_32"
  [(set (match_operand:QUQ15 0 "pic30_register_operand"  "=r")
        (sat_fract:QUQ15
          (match_operand:UTQ 1 "pic30_register_operand"  "r")))]
  ""      
  "
  { rtx r0,r1;

    if (!pic30_fp_inline_p() && 
         ((pic30_fp_round_p() == pic30_convergent) ||
          (pic30_fp_round_p() == pic30_conventional)))
      FAIL;
    r0 = operands[0];
    r1 = operands[1];
    if (!pic30_register_operand(r0,SImode)) {
      r0 = gen_reg_rtx(<MODE>mode);
    } 
    if (!pic30_register_operand(r1,VOIDmode)) {
      r1 = force_reg(<MODE>mode, r1);
    } 
    if (pic30_fp_round_p() == pic30_convergent) {
      emit(
        gen_satfractutq<mode>2_convergent_32(r0,r1)
      );
    } else {
      emit(
        gen_satfractutq<mode>2_helper_32(r0,r1)
      );
    }
    if (r0 != operands[0]) {
      emit_move_insn(operands[0],r0);
    }
    DONE;
  }
  "
)

(define_insn "satfractqq<mode>2_32"
  [(set (match_operand:QUQ31 0 "pic30_register_operand" "=r")
        (sat_fract:QUQ31
          (match_operand:QQ  1 "pic30_register_operand" "r")))]
  ""
  "mov %1,%d0\;clr %0"
  [
    (set_attr "cc" "change0")
  ]
)

(define_insn "satfractuqq<mode>2_32"
  [(set (match_operand:QUQ31 0 "pic30_register_operand" "=r")
        (sat_fract:QUQ31
          (match_operand:UQQ 1 "pic30_register_operand" "r")))]
  ""
  "mov %1,%d0\;clr %0"
  [
    (set_attr "cc" "change0")
  ]
)

(define_insn "satfracthq<mode>2_32"
  [(set (match_operand:QUQ31 0 "pic30_register_operand" "=r")
        (sat_fract:QUQ31
          (match_operand:HQ  1 "pic30_register_operand" "r")))]
  ""
  "mov %1,%d0\;clr %0"
  [
    (set_attr "cc" "change0")
  ]
)

(define_insn "satfractuhq<mode>2_32"
  [(set (match_operand:QUQ31 0 "pic30_register_operand" "=r")
        (sat_fract:QUQ31
          (match_operand:UHQ 1 "pic30_register_operand" "r")))]
  ""
  "mov %1,%d0\;clr %0"
  [
    (set_attr "cc" "change0")
  ]
)

;; conversions between different fractional modes of the same size
;;   these are nops but must exist

(define_insn "satfract<mode>sq2_32"
  [(set (match_operand:SQ      0 "pic30_register_operand" "=r,r")
        (sat_fract:SQ
          (match_operand:QUQ31 1 "pic30_register_operand" "r,0")))]
  ""
  "@
   mov.d %1,%0
   ; nop"
  [
    (set_attr "cc" "change0,unchanged")
  ]
)

(define_insn "satfract<mode>dq2_32"
  [(set (match_operand:DQ      0 "pic30_register_operand" "=r,r")
        (sat_fract:DQ
          (match_operand:QUQ31 1 "pic30_register_operand" "r,0")))]
  ""
  "@
   mov.d %1,%0
   ; nop"
  [
    (set_attr "cc" "change0,unchanged")
  ]
)

(define_insn "satfract<mode>tq2_32"
  [(set (match_operand:TQ      0 "pic30_register_operand" "=r,r")
        (sat_fract:TQ
          (match_operand:QUQ31 1 "pic30_register_operand" "r,0")))]
  ""
  "@
   mov.d %1,%0
   ; nop"
  [
    (set_attr "cc" "change0,unchanged")
  ]
)

(define_insn "satfract<mode>qq2_32"
  [(set (match_operand:QQ      0 "pic30_register_operand" "=r,r")
        (sat_fract:QQ
          (match_operand:QUQ15 1 "pic30_register_operand" "r,0")))]
  ""
  "@
   mov %1,%0
   ; nop"
  [
    (set_attr "cc" "change0,unchanged")
  ]
)

(define_insn "satfract<mode>hq2_32"
  [(set (match_operand:HQ      0 "pic30_register_operand" "=r,r")
        (sat_fract:HQ
          (match_operand:QUQ15 1 "pic30_register_operand" "r,0")))]
  ""
  "@
   mov %1,%0
   ; nop"
  [
    (set_attr "cc" "change0,unchanged")
  ]
)

; missing conversions between signed/unsigned 'self'

(define_insn "satfractqquqq2_32"
  [(set (match_operand:UQQ  0 "register_operand" "=r")
       (unsigned_sat_fract:UQQ
         (match_operand:QQ 1 "register_operand" "0")))]
  ""
  "; nop"
)

(define_insn "satfracthquhq2_32"
  [(set (match_operand:UHQ  0 "register_operand" "=r")
       (unsigned_sat_fract:UHQ
         (match_operand:HQ 1 "register_operand" "0")))]
  ""
  "; nop"
)

(define_insn "satfractsqusq2_32"
  [(set (match_operand:USQ  0 "register_operand" "=r")
       (unsigned_sat_fract:USQ
         (match_operand:SQ 1 "register_operand" "0")))]
  ""
  "; nop"
)

(define_insn "satfractdqudq2_32"
  [(set (match_operand:UDQ  0 "register_operand" "=r")
       (unsigned_sat_fract:UDQ
         (match_operand:DQ 1 "register_operand" "0")))]
  ""
  "; nop"
)

(define_insn "satfracttqutq2_32"
  [(set (match_operand:UTQ  0 "register_operand" "=r")
       (unsigned_sat_fract:UTQ
         (match_operand:TQ 1 "register_operand" "0")))]
  ""
  "; nop"
)

(define_insn "satfracthauha2_32"
  [(set (match_operand:UHA  0 "register_operand" "=r")
       (unsigned_sat_fract:UHA
         (match_operand:HA 1 "register_operand" "0")))]
  ""
  "; nop"
)

(define_insn "satfractuhaha2_32"
  [(set (match_operand:HA    0 "register_operand" "=r")
       (sat_fract:HA
         (match_operand:UHA 1 "register_operand" "0")))]
  ""
  "; nop"
)

(define_insn "satfractsausa2_32"
  [(set (match_operand:USA  0 "register_operand" "=r")
       (unsigned_sat_fract:USA
         (match_operand:SA 1 "register_operand" "0")))]
  ""
  "; nop"
)

(define_insn "satfractusasa2_32"
  [(set (match_operand:SA    0 "register_operand" "=r")
       (sat_fract:SA
         (match_operand:USA 1 "register_operand" "0")))]
  ""
  "; nop"
)

(define_insn "satfractdauda2_32"
  [(set (match_operand:UDA  0 "register_operand" "=r")
       (unsigned_sat_fract:UDA
         (match_operand:DA 1 "register_operand" "0")))]
  ""
  "; nop"
)

(define_insn "satfractudada2_32"
  [(set (match_operand:DA    0 "register_operand" "=r")
       (sat_fract:DA
         (match_operand:UDA 1 "register_operand" "0")))]
  ""
  "; nop"
)

(define_insn "satfracttauta2_32"
  [(set (match_operand:UTA  0 "register_operand" "=r")
       (unsigned_sat_fract:UTA
         (match_operand:TA 1 "register_operand" "0")))]
  ""
  "; nop"
)

(define_insn "satfractutata2_32"
  [(set (match_operand:TA    0 "register_operand" "=r")
       (sat_fract:TA
         (match_operand:UTA 1 "register_operand" "0")))]
  ""
  "; nop"
)

;; negate

(define_insn "neg<mode>2_32"
  [(set (match_operand:QUQ15   0 "pic30_mode2_operand" "=rR<>")
        (neg:QUQ15
          (match_operand:QUQ15 1 "pic30_mode2_operand" " rR<>")))]
  ""
  "neg %1,%0"
  [
    (set_attr "cc" "math")
  ]
)

(define_insn "ssneg<mode>2_32"
  [(set (match_operand:QUQ15   0 "pic30_register_operand" "=&r")
        (ss_neg:QUQ15
          (match_operand:QUQ15 1 "pic30_register_operand" " r")))]
  "pic30_fp_inline_p()"
  "*
   return \"neg %1,%0\;\"
          \"bra nov,.L1_%=\;\"
          \"mov #0x7FFF,%0\;\"
          \"btss %1,#15\;\"
          \"com %0,%0\n\"
          \".L1_%=:\;\";
  "
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "usneg<mode>2_32"
  [(set (match_operand:Q15   0 "pic30_register_operand" "=&r")
        (us_neg:Q15
          (match_operand:Q15 1 "pic30_register_operand" " r")))]
  ""
  "neg %1,%0;\;btsc SR,#2\;mov #0x7FFF,%0\;btsc _SR,#3\;clr %0"
  [
    (set_attr "cc" "clobber")
  ]
)

; cannot negate with unigned saturation a positive value
(define_insn "usneg<mode>2_32"
  [(set (match_operand:UQ15   0 "pic30_register_operand" "=&r")
        (us_neg:UQ15
          (match_operand:UQ15 1 "pic30_register_operand" " r")))]
  ""
  "clr %0"
  [
    (set_attr "cc" "clobber")
  ]
)


(define_insn "neg<mode>2_32"
  [(set (match_operand:QUQ31   0 "pic30_mode2_operand" "=r<>,R")
        (neg:QUQ31
          (match_operand:QUQ31 1 "pic30_register_operand" " r,r")))]
  ""
  "@
   subr %1,#0,%0\;subbr %d1,#0,%d0
   subr %1,#0,%I0\;subbr %d1,#0,%D0"
  [
    (set_attr "cc" "math")
  ]
)

(define_insn "ssneg<mode>2_32"
  [(set (match_operand:QUQ31   0 "pic30_register_operand" "=&r")
        (ss_neg:QUQ31
          (match_operand:QUQ31 1 "pic30_register_operand" " r")))]
  "pic30_fp_inline_p()"
  "*
   return \"subr %1,#0,%0\;\"
          \"subbr %d1,#0,%d0\;\"
          \"bra nov,.L1_%=\;\"
          \"setm %0\;\"
          \"mov #0x7FFF,%d0\;\"
          \"btst.c %d1,#15\;\"
          \"bra c,.L1_%=\;\"
          \"com %d0,%d0\;\"
          \"com %0,%0\n\"
          \".L1_%=:\;\";
  "
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "usneg<mode>2_32"
  [(set (match_operand:Q31   0 "pic30_register_operand" "=r")
        (us_neg:Q31
          (match_operand:Q31 1 "pic30_register_operand" " r")))]
  "pic30_fp_inline_p()"
  "*
   return \"subr %1,#0,%0\;\"
          \"subbr %d1,#0,%d0\;\"
          \"bra nov,.L1_%=\;\"
          \"setm %0\;\"
          \"mov #0x7FFF,%d0\n\"
          \".L1_%=:\;\"
          \"btsc SR,#3\;\"
          \"mul.uu %0,#0,%0\";
  "
  [
    (set_attr "cc" "clobber")
  ]
)

; cannot negate with unigned saturation a positive value
(define_insn "usneg<mode>2_32"
  [(set (match_operand:UQ31   0 "pic30_register_operand" "=&r")
        (us_neg:UQ31
          (match_operand:UQ31 1 "pic30_register_operand" " r")))]
  ""
  "mul.uu %0,#0,%0"
  [
    (set_attr "cc" "clobber")
  ]
)

;; shift

(define_insn "ashl<mode>3_32"
  [(set (match_operand:QUQ15   0 "pic30_register_operand"       "=r")
        (ashift:QUQ15 
          (match_operand:QUQ15 1 "pic30_register_operand"       "r")
          (match_operand:HI    2 "pic30_register_operand"       "r")))
  ]
  ""
  "sl %1,%2,%0"
  [ 
   (set_attr "cc" "math")
  ]
)

(define_insn "ssashl<mode>3_32"
  [(set (match_operand:Q15   0 "pic30_register_operand"     "=&r")
        (ss_ashift:Q15
          (match_operand:Q15 1 "pic30_register_operand"       "r")
          (match_operand:HI  2 "pic30_register_operand"       "r")))
   (clobber (match_scratch:HI 3                              "=&r"))
  ]
  ""
  "*
   return \"mov #0x8000,%3\;\"        /* create mask for bits */
          \"asr %3,%2,%3\;\"          /*   that drop off */
          \"sl  %1,%2,%0\;\"          /* shift */
          \"and %3,%1,%3\;\"          /* mask input and mask */
          \"bra z,.L1_%=\;\"          /* non-zero means bits drop off */
          \"mov #0x7FFF,%0\;\"        /* Saturate to ~.99 */
          \"btsc %1,#15\;\"
          \"com %0,%0\n\"             /* or -1 */
          \".L1_%=:\";
  "
  [
   (set_attr "cc" "clobber")
  ]
)

(define_insn "ssashl<mode>3_32"
  [(set (match_operand:UQ15   0 "pic30_register_operand"     "=&r")
        (ss_ashift:UQ15
          (match_operand:UQ15 1 "pic30_register_operand"       "r")
          (match_operand:HI   2 "pic30_register_operand"       "r")))
   (clobber (match_scratch:HI 3                              "=&r"))
  ]
  ""
  "*
   return \"mov #0x8000,%3\;\"        /* create mask for bits */
          \"asr %3,%2,%3\;\"          /*   that drop off */
          \"sl  %1,%2,%0\;\"          /* shift */
          \"and %3,%1,%3\;\"          /* mask input and mask */
          \"btss SR,#1\;\"           /* non-zero means bits drop off */
          \"mov #0x7FFF,%0\;\";
  "
  [
   (set_attr "cc" "clobber")
  ]
)

(define_insn "usashl<mode>3_32"
  [(set (match_operand:QUQ15   0 "pic30_register_operand"     "=&r")
        (us_ashift:QUQ15
          (match_operand:QUQ15 1 "pic30_register_operand"       "r")
          (match_operand:HI    2 "pic30_register_operand"       "r")))
   (clobber (match_scratch:HI 3                               "=&r"))
  ]
  ""
  "*
   return \"mov #0x8000,%3\;\"        /* create mask for bits */
          \"asr %3,%2,%3\;\"          /*   that drop off */
          \"sl  %1,%2,%0\;\"          /* shift */
          \"and %3,%1,%3\;\"          /* mask input and mask */
          \"btss SR,#1\;\"           /* non-zero means bits drop off */
          \"mov #0x7FFF,%0\";
  "
  [
   (set_attr "cc" "clobber")
  ]
)

; 32-bit shift left

(define_insn "ashl<mode>3_imm1_32"
  [(set (match_operand:QUQ31   0 "pic30_register_operand" "=r")
        (ashift:QUQ31 
          (match_operand:QUQ31 1 "pic30_register_operand"  "r")
          (match_operand:HI    2 "pic30_I_operand"         "I")))]
  ""
  "add %1,%1,%0\;addc %d1,%d1,%d0"
  [
   (set_attr "cc" "clobber")
   (set_attr "type" "def")
  ]
)

(define_insn "ssashl<mode>3_imm1_32"
  [(set (match_operand:Q31   0 "pic30_register_operand" "=&r")
        (ss_ashift:Q31 
          (match_operand:Q31 1 "pic30_register_operand"   "r")
          (match_operand:HI  2 "pic30_I_operand"          "I")))]
  "pic30_fp_inline_p()"
  "*
   return \"add %1,%1,%0\;\"          /* shift */
          \"addc %d1,%d1,%d0\;\"
          \"bra nov,.L1_%=\;\"        /* but if the sign changes ... */
          \"mov #0x7FFF,%d0\;\"       /* Saturate to ~.99 */
          \"btsc %d1,#15\;\"
          \"com %d0,%d0\;\"           /* or -1 */
          \"sl  %d0,%0\;\"            /* form low word by propagating nonsign */
          \"asr %0,#15,%0\n\"         /*   bits into low word */
          \".L1_%=:\;\";
  "
  [
   (set_attr "cc" "clobber")
   (set_attr "type" "def")
  ]
)

(define_insn "ssashl<mode>3_imm1_32"
  [(set (match_operand:UQ31   0 "pic30_register_operand" "=&r")
        (ss_ashift:UQ31
          (match_operand:UQ31 1 "pic30_register_operand"   "r")
          (match_operand:HI   2 "pic30_I_operand"          "I")))]
  "pic30_fp_inline_p()"
  "*
   return \"add %1,%1,%0\;\"          /* shift */
          \"addc %d1,%d1,%d0\;\"
          \"bra nov,.L1_%=\;\"        /* but if the sign changes ... */
          \"mov #0x7FFF,%d0\;\"       /* Saturate to ~.99 */
          \"setm %0\";                /*   bits into low word */
  "
  [
   (set_attr "cc" "clobber")
   (set_attr "type" "def")
  ]
)

(define_insn "usashl<mode>3_imm1_32"
  [(set (match_operand:QUQ31   0 "pic30_register_operand" "=&r")
        (us_ashift:QUQ31
          (match_operand:QUQ31 1 "pic30_register_operand"   "r")
          (match_operand:HI    2 "pic30_I_operand"          "I")))]
  "pic30_fp_inline_p()"
  "*
   return \"add %1,%1,%0\;\"          /* shift */
          \"addc %d1,%d1,%d0\;\"
          \"bra nov,.L1_%=\;\"        /* but if the sign changes ... */
          \"mov #0x7FFF,%d0\;\"       /* Saturate to ~.99 */
          \"setm %0\;\"               /*   bits into low word */
          \".L1_%=:\";
          
  "
  [
   (set_attr "cc" "clobber")
   (set_attr "type" "def")
  ]
)

(define_insn "ashl<mode>3_imm8_32"
  [(set (match_operand:QUQ31   0 "pic30_register_operand" "=&r")
        (ashift:QUQ31 
          (match_operand:QUQ31 1 "pic30_register_operand"   "r")
          (match_operand:HI    2 "pic30_imm8_operand"       "i")))
   (clobber (match_dup 1))
  ]
  ""
  "*
  {
    int idDst = REGNO(operands[0]);
    int idSrc = REGNO(operands[1]);
    if (idDst == idSrc) {
      return \"sl %d1,#%2,%d0\;\"
             \"swap %1\;\"
             \"mov.b %0,%d0\;\"
             \"clr.b %0\";
    } else {
      return \"sl %d1,#%2,%0\;\"
             \"lsr %1,#%k2,%d0\;\"
             \"ior %0,%d0,%d0\;\"
             \"sl %1,#%2,%0\";
    }
  }"
  [
   (set_attr "cc" "clobber")
   (set_attr "type" "def")
  ]
)

(define_insn "ssashl<mode>3_imm8_32"
  [(set (match_operand:Q31   0 "pic30_register_operand" "=&r")
        (ss_ashift:Q31
          (match_operand:Q31 1 "pic30_register_operand"   "r")
          (match_operand:HI  2 "pic30_imm8_operand"       "i")))
   (clobber (match_dup 1))
  ]
  ""
  "*
  {
    int idDst = REGNO(operands[0]);
    int idSrc = REGNO(operands[1]);
    if (idDst == idSrc) {
      return 
             \"asr %d1,#(16-9),%0\;\"    /* test the bits that drop off */
             \"asr %d1,#15,%d0\;\"       /* or move into sign */
             \"xor %d0,%0,%0\;\"         /* match the original sign */
             \"bra z,.L1_%=\;\"          /* normal shift if zero */
             \"mov #0x7FFF,%d0\;\"       /* assume ~.99 if ... */
             \"btsc %d1,#15\;\"
             \"com %d0,%d0\;\"
             \"sl  %d0,%0\;\"
             \"asr %0,#15,%0\;\"
             \"bra .L2_%=\n\"
	     \".L1_%=:\;\"               /* safe to shift ... */
	     \"sl %d1,#%2,%d0\;\"
             \"swap %1\;\"
             \"mov.b %0,%d0\;\"
             \"clr.b %0\n\"
             \".L2_%=:\;\";
    } else {
      return 
             \"asr %d1,#(16-9),%0\;\"    /* test the bits that drop off */
             \"asr %d1,#15,%d0\;\"       /* or move into sign */
             \"xor %d0,%0,%0\;\"         /* match the original sign */
             \"bra z,.L1_%=\;\"          /* normal shift if zero */
             \"mov #0x7FFF,%d0\;\"       /* assume ~.99 if ... */
             \"btsc %d1,#15\;\"
             \"com %d0,%d0\;\"
             \"sl  %d0,%0\;\"
             \"asr %0,#15,%0\;\"
             \"bra .L2_%=\n\"
             \".L1_%=:\;\"               /* safe to shift ... */
             \"sl %d1,#%2,%0\;\"
             \"lsr %1,#%k2,%d0\;\"
             \"ior %0,%d0,%d0\;\"
             \"sl %1,#%2,%0\n\"
             \".L2_%=:\;\";
    }
  }"
  [
   (set_attr "cc" "clobber")
   (set_attr "type" "def")
  ]
)

(define_insn "ssashl<mode>3_imm8_32"
  [(set (match_operand:UQ31   0 "pic30_register_operand" "=&r")
        (ss_ashift:UQ31
          (match_operand:UQ31 1 "pic30_register_operand"   "r")
          (match_operand:HI   2 "pic30_imm8_operand"       "i")))
   (clobber (match_dup 1))
  ]
  ""
  "*
  {
    int idDst = REGNO(operands[0]);
    int idSrc = REGNO(operands[1]);
    if (idDst == idSrc) {
      return
             \"mov #0xFF10, %0\;\"       /* test the bits that drop off */
             \"and %d1,%0,%0\;\"         /* or move into sign for zero */
             \"bra z,.L1_%=\;\"          /* normal shift if zero */
             \"mov #0x7FFF,%d0\;\"       /* return ~.99  */
             \"setm %0\;\"
             \"bra .L2_%=\n\"
             \".L1_%=:\;\"               /* safe to shift ... */
             \"sl %d1,#%2,%d0\;\"        
             \"swap %1\;\"
             \"mov.b %0,%d0\;\"
             \"clr.b %0\n\"
             \".L2_%=:\;\";
    } else { 
      return 
             \"mov #0xFF10, %0\;\"       /* test the bits that drop off */
             \"and %d1,%0,%0\;\"         /* or move into sign for zero */
             \"bra z,.L1_%=\;\"          /* normal shift if zero */
             \"mov #0x7FFF,%d0\;\"       /* return ~.99 ... */
             \"setm %d0\;\"
             \"bra .L2_%=\n\"
             \".L1_%=:\;\"               /* safe to shift ... */
             \"sl %d1,#%2,%0\;\"
             \"lsr %1,#%k2,%d0\;\"
             \"ior %0,%d0,%d0\;\"
             \"sl %1,#%2,%0\n\"
             \".L2_%=:\;\";
    }
  }"
  [
   (set_attr "cc" "clobber")
   (set_attr "type" "def")
  ]
)

(define_insn "usashl<mode>3_imm8_32"
  [(set (match_operand:QUQ31   0 "pic30_register_operand" "=&r")
        (us_ashift:QUQ31
          (match_operand:QUQ31 1 "pic30_register_operand"   "r")
          (match_operand:HI    2 "pic30_imm8_operand"       "i")))
   (clobber (match_dup 1))
  ]       
  ""
  "*
  {
    int idDst = REGNO(operands[0]);
    int idSrc = REGNO(operands[1]);
    if (idDst == idSrc) {
      return
             \"mov #0xFF10, %0\;\"       /* test the bits that drop off */
             \"and %d1,%0,%0\;\"         /* or move into sign for zero */
             \"bra z,.L1_%=\;\"          /* normal shift if zero */
             \"mov #0x7FFF,%d0\;\"       /* return ~.99  */
             \"setm %0\;\"
             \"bra .L2_%=\n\"
             \".L1_%=:\;\"               /* safe to shift ... */
             \"sl %d1,#%2,%d0\;\"
             \"swap %1\;\"
             \"mov.b %0,%d0\;\"
             \"clr.b %0\n\"
             \".L2_%=:\;\";
    } else {
      return  
             \"mov #0xFF10, %0\;\"       /* test the bits that drop off */
             \"and %d1,%0,%0\;\"         /* or move into sign for zero */
             \"bra z,.L1_%=\;\"          /* normal shift if zero */
             \"mov #0x7FFF,%d0\;\"       /* return ~.99 ... */
             \"setm %d0\;\"
             \"bra .L2_%=\n\"
             \".L1_%=:\;\"               /* safe to shift ... */
             \"sl %d1,#%2,%0\;\"
             \"lsr %1,#%k2,%d0\;\"
             \"ior %0,%d0,%d0\;\"
             \"sl %1,#%2,%0\n\"
             \".L2_%=:\;\";
    }
  }"
  [
   (set_attr "cc" "clobber")
   (set_attr "type" "def")
  ]
)

(define_insn "ashl<mode>3_imm16plus_32"
  [(set (match_operand:QUQ31   0 "pic30_register_operand"  "=r")
        (ashift:QUQ31 
          (match_operand:QUQ31 1 "pic30_register_operand"   "r")
          (match_operand:HI    2 "pic30_imm16plus_operand"  "i")))]
  ""
  "sl %1,#%K2,%d0\;mov #0,%0"
  [
   (set_attr "cc" "clobber")
   (set_attr "type" "def")
  ]
)

(define_insn "ssashl<mode>3_imm16plus_32"
  [(set (match_operand:Q31   0 "pic30_register_operand"  "=&r")
        (ss_ashift:Q31
          (match_operand:Q31 1 "pic30_register_operand"    "r")
          (match_operand:HI  2 "pic30_imm16plus_operand"   "i")))]
  ""
  "*
   return 
          \"asr %d1,#15,%d0\;\"       /* for signed we care about 1s */
          \"asr %1,#(16-%K2),%0\;\"   /* for unsigned we care about 0s */
          \"xor %0,%d0,[w15]\;\"      /* shifting bits out? */
          \"btsc SR,#1\;\"           /*  yes? don't compare the high word */
          \"cp %d0,%d1\;\"
          \"bra z,.L1_%=\;\"          /* if not losing bits, shift as normal */
          \"mov #0x7FFF,%d0\;\"       /* assume ~.9999 */
          \"btsc %d1,#15\;\"          /* if negatvie */
          \"com %d0,%d0\;\"           /* make -1 */
          \"sl %d0,%0\;\"             /* propagate non sign bits */
          \"asr %0,#15,%0\;\"
          \"bra .L2_%=\n\"
          \".L1_%=:\;\"               /* shift as normal */
          \"sl %1,#%K2,%d0\;\" 
          \"mov #0,%0\n\"
          \".L2_%=:\;\";
  "
  [
   (set_attr "cc" "clobber")
   (set_attr "type" "def")
  ]
)

(define_insn "ssashl<mode>3_imm16plus_32"
  [(set (match_operand:UQ31   0 "pic30_register_operand"  "=&r")
        (ss_ashift:UQ31
          (match_operand:UQ31 1 "pic30_register_operand"    "r")
          (match_operand:HI   2 "pic30_imm16plus_operand"   "i")))]
  ""
  "*
   return
          \"asr %1,#(16-%K2),%0\;\"   /* for unsigned we care about 0s */
          \"btsc SR,#1\;\"           /*  yes? don't compare the high word */
          \"cp0 %d1\;\"
          \"bra z,.L1_%=\;\"          /* if not losing bits, shift as normal */
          \"mov #0x7FFF,%d0\;\"       /* make ~.9999 */
          \"setm %d0\;\"
          \"bra .L2_%=\n\"
          \".L1_%=:\;\"               /* shift as normal */
          \"sl %1,#%K2,%d0\;\" 
          \"mov #0,%0\n\"
          \".L2_%=:\;\";
  " 
  [
   (set_attr "cc" "clobber")
   (set_attr "type" "def")
  ]
)

(define_insn "usashl<mode>3_imm16plus_32"
  [(set (match_operand:QUQ31   0 "pic30_register_operand"  "=&r")
        (us_ashift:QUQ31
          (match_operand:QUQ31 1 "pic30_register_operand"    "r")
          (match_operand:HI    2 "pic30_imm16plus_operand"   "i")))]
  ""
  "*
   return
          \"asr %1,#(16-%K2),%0\;\"   /* for unsigned we care about 0s */
          \"btsc SR,#1\;\"           /*  yes? don't compare the high word */
          \"cp0 %d1\;\"
          \"bra z,.L1_%=\;\"          /* if not losing bits, shift as normal */
          \"mov #0x7FFF,%d0\;\"       /* make ~.9999 */
          \"setm %d0\;\"
          \"bra .L2_%=\n\"
          \".L1_%=:\;\"               /* shift as normal */
          \"sl %1,#%K2,%d0\;\"
          \"mov #0,%0\n\"
          \".L2_%=:\;\";
  "
  [
   (set_attr "cc" "clobber")
   (set_attr "type" "def")
  ]
)

(define_insn "ashl<mode>3_imm2to15_32"
  [(set (match_operand:QUQ31   0 "pic30_register_operand" "=r,&r")
        (ashift:QUQ31 
          (match_operand:QUQ31 1 "pic30_register_operand"  "r, r")
          (match_operand:HI    2 "pic30_imm2to15_operand"  "i, i")))
   (clobber (match_scratch:HI 3                          "=&r, X"))]
  ""
  "*
  { int idDst, idSrc;

    switch (which_alternative) {
      case 0:
        /*
        ** Take care that the source and dest don't overlap
        */
        idDst = REGNO(operands[0]);
        idSrc = REGNO(operands[1]);
        if (idDst >= idSrc) {
          return \"sl %d1,#%2,%3\;\"
                 \"lsr %1,#%k2,%d0\;\"
                 \"ior %3,%d0,%d0\;\"
                 \"sl %1,#%2,%0\";
        } else {
          return \"sl %1,#%2,%0\;\"
                 \"sl %d1,#%2,%3\;\"
                 \"lsr %1,#%k2,%d0\;\"
                 \"ior %3,%d0,%d0\";
        }
      case 2:
        /*
        ** The dest and source don't overlap
        ** so use dest lsw as a temporary
        */
        return \"sl %d1,#%2,%0\;\"
               \"lsr %1,#%k2,%d0\;\"
               \"ior %0,%d0,%d0\;\"
               \"sl %1,#%2,%0\";
      default: gcc_assert(0);
    }
    return \"nop\";
  }"
  [
   (set_attr "cc" "clobber")
   (set_attr "type" "def")
  ]
)

(define_insn "ssashl<mode>3_imm2to15_32"
  [(set (match_operand:Q31    0 "pic30_register_operand" "=r,&r")
        (ss_ashift:Q31 
          (match_operand:Q31  1 "pic30_register_operand"  "r, r")
          (match_operand:HI   2 "pic30_imm2to15_operand"  "i, i")))
   (clobber (match_scratch:HI 3                         "=&r, X"))]
  ""
  "*
  { int idDst, idSrc;

    switch (which_alternative) {
      case 0:
        /* 
        ** Take care that the source and dest don't overlap
        */
        idDst = REGNO(operands[0]);
        idSrc = REGNO(operands[1]);
        if (idDst >= idSrc) {
          return 
                 \"asr %d1,#15,%d0\;\"       /* for signed we care about 1s */
                 \"asr %d1,#(15-%2),%0\;\"   /* for unsigned we care about 0s */
                 \"xor %0,%d0,%d0\;\"        /* shifting bits out? */
                 \"bra z,.L1_%=\;\"          /* if not, shift as normal */
                 \"mov #0x7FFF,%d0\;\"       /* assume ~.9999 */
                 \"btsc %d1,#15\;\"          /* if negatvie */
                 \"com %d0,%d0\;\"           /* make -1 */
                 \"sl %d0,%0\;\"             /* propagate non sign bits */
                 \"asr %0,#15,%0\;\"
                 \"bra .L2_%=\n\"
                 \".L1_%=:\;\"               /* shift as normal */
                 \"sl %d1,#%2,%3\;\"
                 \"lsr %1,#%k2,%d0\;\"
                 \"ior %3,%d0,%d0\;\"
                 \"sl %1,#%2,%0\n\"
                 \".L2_%=:\;\";
        } else {
          return
                 \"asr %d1,#15,%d0\;\"       /* for signed we care about 1s */
                 \"asr %d1,#(15-%2),%0\;\"   /* for unsigned we care about 0s */
                 \"xor %0,%d0,%d0\;\"        /* shifting bits out? */
                 \"bra z,.L1_%=\;\"          /* if not, shift as normal */
                 \"mov #0x7FFF,%d0\;\"       /* assume ~.9999 */
                 \"btsc %d1,#15\;\"          /* if negatvie */
                 \"com %d0,%d0\;\"           /* make -1 */
                 \"sl %d0,%0\;\"             /* propagate non sign bits */
                 \"asr %0,#15,%0\;\"
                 \"bra .L2_%=\n\"
                 \".L1_%=:\;\"               /* shift as normal */
                 \"sl %1,#%2,%0\;\"
                 \"sl %d1,#%2,%3\;\"
                 \"lsr %1,#%k2,%d0\;\"
                 \"ior %3,%d0,%d0\n\"
                 \".L2_%=:\;\";
        }
      case 2:
        /*
        ** The dest and source don't overlap
        ** so use dest lsw as a temporary
        */
        return
               \"asr %d1,#15,%d0\;\"         /* for signed we care about 1s */
               \"asr %d1,#(15-%2),%0\;\"     /* for unsigned we care about 0s */
               \"xor %0,%d0,%d0\;\"          /* shifting bits out? */
               \"bra z,.L1_%=\;\"            /* if not, shift as normal */
               \"mov #0x7FFF,%d0\;\"         /* assume ~.9999 */
               \"btsc %d1,#15\;\"            /* if negatvie */
               \"com %d0,%d0\;\"             /* make -1 */
               \"sl %d0,%0\;\"               /* propagate non sign bits */
               \"asr %0,#15,%0\;\"
               \"bra .L2_%=\n\"
               \".L1_%=:\;\"                 /* shift as normal */
               \"sl %d1,#%2,%0\;\"
               \"lsr %1,#%k2,%d0\;\"
               \"ior %0,%d0,%d0\;\"
               \"sl %1,#%2,%0\n\"
               \".L2_%=:\;\";

      default: gcc_assert(0);
    }
    return \"nop\";
  }"
  [
   (set_attr "cc" "clobber")
   (set_attr "type" "def")
  ]
)

(define_insn "ssashl<mode>3_imm2to15_32"
  [(set (match_operand:UQ31   0 "pic30_register_operand" "=r,&r")
        (ss_ashift:UQ31
          (match_operand:UQ31 1 "pic30_register_operand"  "r, r")
          (match_operand:HI   2 "pic30_imm2to15_operand"  "i, i")))
   (clobber (match_scratch:HI 3                         "=&r, X"))]
  ""
  "*
  { int idDst, idSrc;

    switch (which_alternative) {
      case 0:
        /*
        ** Take care that the source and dest don't overlap
        */
        idDst = REGNO(operands[0]);
        idSrc = REGNO(operands[1]);
        if (idDst >= idSrc) {
          return
                 \"asr %d1,#(15-%2),%0\;\"   /* for unsigned we care about 0s */
                 \"bra z,.L1_%=\;\"          /* if not, shift as normal */
                 \"mov #0x7FFF,%d0\;\"       /* return ~.9999 */
                 \"setm %0\;\"
                 \"bra .L2_%=\n\"
                 \".L1_%=:\;\"               /* shift as normal */
                 \"sl %d1,#%2,%3\;\"
                 \"lsr %1,#%k2,%d0\;\"
                 \"ior %3,%d0,%d0\;\"
                 \"sl %1,#%2,%0\n\"
                 \".L2_%=:\;\";
        } else {
          return
                 \"asr %d1,#(15-%2),%0\;\"   /* for unsigned we care about 0s */
                 \"bra z,.L1_%=\;\"          /* if not, shift as normal */
                 \"mov #0x7FFF,%d0\;\"       /* return ~.9999 */
                 \"setm %0\;\"
                 \"bra .L2_%=\n\"
                 \".L1_%=:\;\"               /* shift as normal */
                 \"sl %1,#%2,%0\;\"
                 \"sl %d1,#%2,%3\;\"
                 \"lsr %1,#%k2,%d0\;\"
                 \"ior %3,%d0,%d0\n\"
                 \".L2_%=:\;\";
        }
      case 2:
        /*
        ** The dest and source don't overlap
        ** so use dest lsw as a temporary
        */
        return 
               \"asr %d1,#(15-%2),%0\;\"     /* for unsigned we care about 0s */
               \"bra z,.L1_%=\;\"            /* if not, shift as normal */
               \"mov #0x7FFF,%d0\;\"         /* return ~.9999 */
               \"setm %0\;\"
               \"bra .L2_%=\n\"
               \".L1_%=:\;\"                 /* shift as normal */
               \"sl %d1,#%2,%0\;\"
               \"lsr %1,#%k2,%d0\;\"
               \"ior %0,%d0,%d0\;\"
               \"sl %1,#%2,%0\n\"
               \".L2_%=:\;\";
      default: gcc_assert(0);
    }
    return \"nop\";
  }"
  [
   (set_attr "cc" "clobber")
   (set_attr "type" "def")
  ]
)

(define_insn "usashl<mode>3_imm2to15_32"
  [(set (match_operand:QUQ31   0 "pic30_register_operand" "=r,&r")
        (us_ashift:QUQ31
          (match_operand:QUQ31 1 "pic30_register_operand"  "r, r")
          (match_operand:HI    2 "pic30_imm2to15_operand"  "i, i")))
   (clobber (match_scratch:HI  3                         "=&r, X"))]
  ""
  "*
  { int idDst, idSrc;

    switch (which_alternative) {
      case 0:
        /*
        ** Take care that the source and dest don't overlap
        */
        idDst = REGNO(operands[0]);
        idSrc = REGNO(operands[1]);
        if (idDst >= idSrc) {
          return
                 \"asr %d1,#(15-%2),%0\;\"   /* for unsigned we care about 0s */
                 \"bra z,.L1_%=\;\"          /* if not, shift as normal */
                 \"mov #0x7FFF,%d0\;\"       /* return ~.9999 */
                 \"setm %0\;\"
                 \"bra .L2_%=\n\"
                 \".L1_%=:\;\"               /* shift as normal */
                 \"sl %d1,#%2,%3\;\"
                 \"lsr %1,#%k2,%d0\;\"
                 \"ior %3,%d0,%d0\;\"
                 \"sl %1,#%2,%0\n\"
                 \".L2_%=:\;\";
        } else {
          return
                 \"asr %d1,#(15-%2),%0\;\"   /* for unsigned we care about 0s */
                 \"bra z,.L1_%=\;\"          /* if not, shift as normal */
                 \"mov #0x7FFF,%d0\;\"       /* return ~.9999 */
                 \"setm %0\;\"
                 \"bra .L2_%=\n\"
                 \".L1_%=:\;\"               /* shift as normal */
                 \"sl %1,#%2,%0\;\"
                 \"sl %d1,#%2,%3\;\"
                 \"lsr %1,#%k2,%d0\;\"
                 \"ior %3,%d0,%d0\n\"
                 \".L2_%=:\;\";
        }
      case 2:
        /*
        ** The dest and source don't overlap
        ** so use dest lsw as a temporary
        */
        return
               \"asr %d1,#(15-%2),%0\;\"     /* for unsigned we care about 0s */
               \"bra z,.L1_%=\;\"            /* if not, shift as normal */
               \"mov #0x7FFF,%d0\;\"         /* return ~.9999 */
               \"setm %0\;\"
               \"bra .L2_%=\n\"
               \".L1_%=:\;\"                 /* shift as normal */
               \"sl %d1,#%2,%0\;\"
               \"lsr %1,#%k2,%d0\;\"
               \"ior %0,%d0,%d0\;\"
               \"sl %1,#%2,%0\n\"
               \".L2_%=:\;\";
      gcc_assert(0);
    }
    return \"nop\";
  }"
  [
   (set_attr "cc" "clobber")
   (set_attr "type" "def")
  ]
)

(define_insn "ashl<mode>3_reg_32"
  [(set (match_operand:QUQ31   0 "pic30_register_operand" "=r")
        (ashift:QUQ31 
          (match_operand:QUQ31 1 "pic30_register_operand" " 0")
          (match_operand:HI    2 "pic30_register_operand" " r")))
   (clobber (match_scratch:HI  3                          "=2"))
  ]
  ""
  "*
  {
    return
           \".set ___BP___,0\n\"
           \".LB%=:\;\"
           \"dec %2,%2\;\"
           \"bra n,.LE%=\;\"
           \"add %1,%1,%0\;\"
           \"addc %d1,%d1,%d0\;\"
           \"bra .LB%=\n\"
           \".LE%=:\";
  }"
  [
   (set_attr "cc" "clobber")
   (set_attr "type" "def")
  ]
)

(define_insn "ssashl<mode>3_reg_32"
  [(set (match_operand:Q31    0 "pic30_register_operand" "=&r")
        (ss_ashift:Q31
          (match_operand:Q31  1 "pic30_register_operand" " r")
          (match_operand:HI   2 "pic30_register_operand" " r")))
   (clobber (match_scratch:HI 3                          "=2"))
  ]
  ""
  "*
  {
    return
           \".set ___BP___,0\n\"
           \"mov.d %1,%0\;\"
           \".LB%=:\;\"
           \"dec %2,%2\;\"
           \"bra n,.LE%=\;\"
           \"add %0,%0,%0\;\"
           \"addc %d0,%d0,%d0\;\"
           \"bra nov,.LB%=\;\"        /* if we drop a bit into the sign */
           \"mov #0x7FFF,%d0\;\"      /* assume ~.9999 */
           \"btsc %d1,#15\;\"
           \"com %d0,%d0\;\"
           \"sl %d0,%0\;\"
           \"asr %0,#15,%0\n\"
           \".LE%=:\";
  }"
  [
   (set_attr "cc" "clobber")
   (set_attr "type" "def")
  ]
)

(define_insn "ssashl<mode>3_reg_32"
  [(set (match_operand:UQ31   0 "pic30_register_operand" "=r")
        (ss_ashift:UQ31
          (match_operand:UQ31 1 "pic30_register_operand" " 0")
          (match_operand:HI   2 "pic30_register_operand" " r")))
   (clobber (match_scratch:HI 3                          "=2"))
  ]
  ""
  "*
  {
    return
           \".set ___BP___,0\n\"
           \".LB%=:\;\"
           \"dec %2,%2\;\"
           \"bra n,.LE%=\;\"
           \"add %1,%1,%0\;\"
           \"addc %d1,%d1,%d0\;\"
           \"bra nov,.LB%=\;\"        /* if we drop a bit into the sign */
           \"mov #0x7FFF,%d0\;\"      /* return ~.9999 */
           \"setm %0\;\"
           \".LE%=:\";
  }"
  [
   (set_attr "cc" "clobber")
   (set_attr "type" "def")
  ]
)

(define_insn "usashl<mode>3_reg_32"
  [(set (match_operand:QUQ31   0 "pic30_register_operand" "=r")
        (us_ashift:QUQ31
          (match_operand:QUQ31 1 "pic30_register_operand" " 0")
          (match_operand:HI    2 "pic30_register_operand" " r")))
   (clobber (match_scratch:HI  3                          "=2"))
  ]
  ""
  "*
  {
    return
           \".set ___BP___,0\n\"
           \".LB%=:\;\"
           \"dec %2,%2\;\"
           \"bra n,.LE%=\;\"
           \"add %1,%1,%0\;\"
           \"addc %d1,%d1,%d0\;\"
           \"bra nov,.LB%=\;\"        /* if we drop a bit into the sign */
           \"mov #0x7FFF,%d0\;\"      /* return ~.9999 */
           \"setm %0\;\"
           \".LE%=:\";
  }"
  [
   (set_attr "cc" "clobber")
   (set_attr "type" "def")
  ]
)

(define_expand "ashl<mode>3_32"
  [(set (match_operand:QUQ31   0 "pic30_register_operand" "")
        (ashift:QUQ31 
          (match_operand:QUQ31 1 "pic30_register_operand" "")
          (match_operand:HI    2 "pic30_reg_or_imm_operand" "")))]
  ""
  "
  {
    if (GET_CODE(operands[2]) == CONST_INT) {
      switch (INTVAL(operands[2])) {
        case 0:
          emit_insn(
            gen_mov<mode>_32(operands[0], operands[1])
          );
          break;
        case 1:
          emit_insn(
            gen_ashl<mode>3_imm1_32(operands[0], operands[1], operands[2])
          );
          break;
        case 8:
          emit_insn(
            gen_ashl<mode>3_imm8_32(operands[0], operands[1], operands[2])
          );
          break;
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
          emit_insn(
            gen_ashl<mode>3_imm2to15_32(operands[0], operands[1], operands[2])
          );
          break;
        case 16:
          emit_insn(
            gen_ashl<mode>3_imm16plus_32(operands[0], operands[1], operands[2])
          );
          break;
        case 17:
        case 18:
        case 19:
        case 20:
        case 21:
        case 22:
        case 23:
        case 24:
        case 25:
        case 26:
        case 27:
        case 28:
        case 29:
        case 30:
        case 31:
          emit_insn(
            gen_ashl<mode>3_imm16plus_32(operands[0], operands[1], operands[2])
          );
          break;
        default:
          emit_insn(
            gen_mov<mode>_32(operands[0], const0_rtx)
          ); 
          break;
      }
    } else {
      emit_insn(
        gen_ashl<mode>3_reg_32(operands[0],operands[1],operands[2])
      );
    }
    DONE;
}")

(define_expand "ssashl<mode>3_32"
  [(set (match_operand:QUQ31   0 "pic30_register_operand" "")
        (ss_ashift:QUQ31 
          (match_operand:QUQ31 1 "pic30_register_operand" "")
          (match_operand:HI    2 "pic30_reg_or_imm_operand" "")))]
  ""
  "
  {
    if (!pic30_fp_inline_p()) FAIL;
    if (GET_CODE(operands[2]) == CONST_INT) {
      switch (INTVAL(operands[2])) {
        case 0:
          emit_insn(
            gen_mov<mode>_32(operands[0], operands[1])
          );
          break;
        case 1:
          emit_insn(
            gen_ssashl<mode>3_imm1_32(operands[0], operands[1], operands[2])
          );
          break;
        case 8:
          emit_insn(
            gen_ssashl<mode>3_imm8_32(operands[0], operands[1], operands[2])
          );
          break;
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
          emit_insn(
            gen_ssashl<mode>3_imm2to15_32(operands[0], operands[1], operands[2])
          );
          break;
        case 16:
          emit_insn(
            gen_ssashl<mode>3_imm16plus_32(operands[0], operands[1], operands[2])
          );
          break;
        case 17:
        case 18:
        case 19:
        case 20:
        case 21:
        case 22:
        case 23:
        case 24:
        case 25:
        case 26:
        case 27:
        case 28:
        case 29:
        case 30:
        case 31:
          emit_insn(
            gen_ssashl<mode>3_imm16plus_32(operands[0], operands[1], operands[2])
          );
          break;
        default:
          emit_insn(
            gen_mov<mode>_32(operands[0], const0_rtx)
          ); 
          break;
      }
    } else {
      emit_insn(
        gen_ssashl<mode>3_reg_32(operands[0],operands[1],operands[2])
      );
    }
    DONE;
}")

(define_expand "usashl<mode>3_32"
  [(set (match_operand:QUQ31   0 "pic30_register_operand" "")
        (us_ashift:QUQ31 
          (match_operand:QUQ31 1 "pic30_register_operand" "")
          (match_operand:HI    2 "pic30_reg_or_imm_operand" "")))]
  ""
  "
  {
    if (!pic30_fp_inline_p()) FAIL;
    if (GET_CODE(operands[2]) == CONST_INT) {
      switch (INTVAL(operands[2])) {
        case 0:
          emit_insn(
            gen_mov<mode>_32(operands[0], operands[1])
          );
          break;
        case 1:
          emit_insn(
            gen_usashl<mode>3_imm1_32(operands[0], operands[1], operands[2])
          );
          break;
        case 8:
          emit_insn(
            gen_usashl<mode>3_imm8_32(operands[0], operands[1], operands[2])
          );
          break;
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
          emit_insn(
            gen_usashl<mode>3_imm2to15_32(operands[0], operands[1], operands[2])
          );
          break;
        case 16:
          emit_insn(
            gen_usashl<mode>3_imm16plus_32(operands[0], operands[1], operands[2])
          );
          break;
        case 17:
        case 18:
        case 19:
        case 20:
        case 21:
        case 22:
        case 23:
        case 24:
        case 25:
        case 26:
        case 27:
        case 28:
        case 29:
        case 30:
        case 31:
          emit_insn(
            gen_usashl<mode>3_imm16plus_32(operands[0], operands[1], operands[2])
          );
          break;
        default:
          emit_insn(
            gen_mov<mode>_32(operands[0], const0_rtx)
          ); 
          break;
      }
    } else {
      emit_insn(
        gen_usashl<mode>3_reg_32(operands[0],operands[1],operands[2])
      );
    }
    DONE;
}")

(define_insn "ashr<mode>3_helper_32"
  [(set (match_operand:QUQ15   0 "pic30_register_operand"  "=&r,&r,&r")
        (ashiftrt:QUQ15 
          (match_operand:QUQ15 1 "pic30_register_operand"   "r,r,r")
          (match_operand:HI    2 "pic30_reg_or_imm_operand" "Kh,i,r")))]
  ""
  "*
  {
    if ((pic30_fp_round_p() == pic30_truncation) ||
        (pic30_fp_round_p() == pic30_fastest)) {
      switch (which_alternative) {
        case 0: 
          return \"asr %1,#%2,%0\";
        case 1:
          if (INTVAL(operands[2]) < 0)
            return \"sl %1,#%J2%%16,%0\";
          else
            return \"asr %1,#%2%%16,%0\";
        case 2: 
            return \"asr %1,%2,%0\";
        default: gcc_assert(0);
      }
    } else if (pic30_fp_round_p() == pic30_conventional) 
    switch (which_alternative) {
      case 0:  
        if (INTVAL(operands[2]) == 1) {
          return 
                 \"asr %1,%0\;\"
                 \"addc %0,#0,%0\";
        } else {
          return \"asr %1,#%L2,%0\;\"      /* shift one less than we need */
                 \"asr %0,%0\;\"           /*   so that we can shift once */
                 \"addc %0,#0,%0\";        /*   to capture and add carry */
        }
      case 1:
        if (INTVAL(operands[2]) < 0)
          return \"cannot generate instruction\"; 
        else
          return \"asr %1,#(%2%%16)-1,%0\;\" /* shift one less than we need */
                 \"asr %0,%0\;\"             /*   so that we can shift once */
                 \"addc %0,#0,%0\";          /*   to capture and add carry */
      case 2: 
	  return \"cp0 %2\;\"
                 \"mov %1,%0\;\"
                 \"bra z,.LE%=\;\"
                 \"dec %2,%0\;\"             /* shift one less than we need */
                 \"asr %1,%0,%0\;\"
                 \"asr %0,%0\;\"             /*   so that we can shift once */
                 \"addc %0,#0,%0\n\"         /*   to capture and add carry */
                 \".LE%=:\";
      default: gcc_assert(0);
    }
    else 
      error(\"Invalid fixed-point rounding mode specified\");
    return \"cannot generate instruction\";
  }"
  [
   (set_attr "cc" "math")
   (set_attr "type" "def")
  ]
)

(define_insn "ashr<mode>3_convergent_32"
  [(set (match_operand:QUQ15   0 "pic30_register_operand"  "=r,r,r")
        (ashiftrt:QUQ15
          (match_operand:QUQ15 1 "pic30_register_operand"   "r,r,r")
          (match_operand:HI    2 "pic30_reg_or_imm_operand" "Kh,i,r")))
   (clobber (match_scratch:HI  3                          "=&r,&r,&r"))
  ]
  ""
  "*
  {
    if (pic30_fp_round_p() == pic30_convergent)
    switch (which_alternative) {
      case 0:  
        if (INTVAL(operands[2]) == 1) {
          return 
                 \"asr %1,%0\;\"
                 \"btsc %0,#0\;\"          /* do not add carry if low bit == 0*/
                 \"addc %0,#0,%0\";
        } else {
          return 
                 \"sl %1,%K2,%3\;\"        /* shift up bits that drop off */
                 \"asr %1,#%2,%0\;\"       /* shift down the bits we need */
                 \"sl %3,%3\;\"            /* shift top overfaow into carry */
                 \"bra nc,.L1_%=\;\"       /* if no carry, finish */
                 \"addc %0,#0,%0\;\"       /* add overflow from carry */
                 \"cp0 %3\;\"              /* test rest of the overflow */
                 \"mov #0xFFFE,%3\;\"      /* create mask off low bit */
                 \"btsc SR,#1\;\"         /* if the rest of overflow is 0 */
                 \"and %0,%3,%0\n\"        /*   mask off low bit */
                 \".L1_%=:\";
        }
      case 1:
        if (INTVAL(operands[2]) < 0)
          return \"cannot generate instruction\"; 
        else
          return \"sl %1,#16-(%2%%16),%3\;\" /* shift up bits that drop off */
                 \"asr %1,#(%2%%16),%0\;\"   /* shift down bits we need */
                 \"sl %3,%3\;\"              /* shift top overflow into carry */
                 \"bra nc,.L1_%=\;\"         /* if no carry, finish */
                 \"addc %0,#0,%0\;\"         /* add carry */
                 \"cp0 %3\;\"                /* test rest of the overflow */
                 \"mov #0xFFFE,%3\;\"        /* create mask clear low bit */
                 \"btsc SR,#1\;\"           /* if the rest of overflow is 0 */
                 \"and %0,%3,%0\n\"          /*   mask off low bit */
                 \".L1_%=:\";
      case 2: 
          return \"subr %2,#16,%3\;\"
                 \"sl %1,%3,%3\;\"           /* shift up bits that drop off */
                 \"asr %1,%2,%0\;\"          /* shift down bits we need */
                 \"sl %3,%3\;\"              /* shift top overflow into carry */
                 \"bra nc,.L1_%=\;\"         /* if no carry, finish */
                 \"addc %0,#0,%0\;\"         /* add carry */
                 \"cp0 %3\;\"                /* test rest of the overflow */
                 \"mov #0xFFFE,%3\;\"        /* create mask clear low bit */
                 \"btsc SR,#1\;\"           /* if the rest of overflow is 0 */
                 \"and %0,%3,%0\n\"          /*   mask off low bit */
                 \".L1_%=:\";
      default:  gcc_assert(0);
    } else
      error(\"Invalid fixed-point round mode specified\n\");
    return \"cannot generate instruction\";

  }"
  [
   (set_attr "cc" "math")
   (set_attr "type" "def")
  ]
)

(define_expand "ashr<mode>3_32"
  [(set (match_operand:QUQ15   0 "pic30_register_operand"   "")
        (ashiftrt:QUQ15
          (match_operand:QUQ15 1 "pic30_register_operand"   "")
          (match_operand:HI    2 "pic30_reg_or_imm_operand" "")))]
  ""
  "
  { rtx r0,r1,r2;

    r0 = operands[0];
    r1 = operands[1];
    r2 = operands[2];
    if (!pic30_register_operand(r0,<MODE>mode)) {
      r0 = gen_reg_rtx(<MODE>mode);
    }
    if (!pic30_register_operand(r1,<MODE>mode)) {
      r1 = force_reg(<MODE>mode, r1);
    }
    if (!pic30_reg_or_imm_operand(r2,<MODE>mode)) {
      r2 = force_reg(HImode, r2);
    }
    if (pic30_fp_round_p() == pic30_convergent) {
      emit(
        gen_ashr<mode>3_convergent_32(r0,r1,r2)
      );
    } else {
      emit(
        gen_ashr<mode>3_helper_32(r0,r1,r2)
      );
    }
    if (r0 != operands[0]) {
      emit_move_insn(operands[0],r0);
    }
    DONE;
  }"
)

(define_expand "ashr<mode>3_32"
  [(set (match_operand:QUQ31   0 "pic30_register_operand"   "")
        (ashiftrt:QUQ31 
          (match_operand:QUQ31 1 "pic30_register_operand"   "")
          (match_operand:HI    2 "pic30_reg_or_imm_operand" "")))]
  ""
  "
  {
    if ((pic30_fp_round_p() == pic30_conventional) ||
        (pic30_fp_round_p() == pic30_convergent)) 
      FAIL;
    if (GET_CODE(operands[2]) == CONST_INT) {
      switch (INTVAL(operands[2])) {
        case 0:
          emit_insn(
            gen_mov<mode>_32(operands[0], operands[1])
          );
          break;
        case 1:
          emit_insn(
            gen_ashr<mode>3_imm1_32(operands[0], operands[1], operands[2])
          );
          break;
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
          emit_insn(
            gen_ashr<mode>3_imm2to15_32(operands[0], operands[1], operands[2])
          );
          break;
        case 16:
          emit_insn(
            gen_ashr<mode>3_imm16plus_32(operands[0], operands[1], operands[2])
          );
          break;
        case 17:
        case 18:
        case 19:
        case 20:
        case 21:
        case 22:
        case 23:
        case 24:
        case 25:
        case 26:
        case 27:
        case 28:
        case 29:
        case 30:
        case 31:
          if (pic30_fp_round_p() == pic30_convergent) {
            emit_insn(
              gen_ashr<mode>3_imm16plus_convergent_32(operands[0], operands[1], 
                                                      operands[2])
            );
          } else {
            emit_insn(
              gen_ashr<mode>3_imm16plus_32(operands[0],operands[1], operands[2])
            );
          }
          break;
        default:
          emit_insn(
            gen_mov<mode>_32(operands[0], const0_rtx)
          );
          break;
      }
    } else {
      {
        emit_insn(
          gen_ashr<mode>3_reg_32(operands[0],operands[1],operands[2])
        );
      }
    }
    DONE;
  }"
)

(define_insn "ashr<mode>3_imm1_32"
  [(set (match_operand:QUQ31   0 "pic30_register_operand" "=r")
        (ashiftrt:QUQ31 
          (match_operand:QUQ31 1 "pic30_register_operand"  "r")
          (match_operand:HI    2 "pic30_I_operand"         "I")))]
  ""
  "*
   if ((pic30_fp_round_p() == pic30_truncation) ||
       (pic30_fp_round_p() == pic30_fastest)) {
     return \"asr %d1,%d0\;rrc %1,%0\";
   } else if (pic30_fp_round_p() == pic30_conventional) {
     return 
            \"asr %d1,%d0\;\"       /* shift right ... */
            \"rrc %1,%0\;\"
            \"addc %0,#0,%0\;\"     /* add in drop overflow bit */
            \"addc %d0,#0,%d0\;\";
   } else if (pic30_fp_round_p() == pic30_convergent) {
     return 
            \"asr %d1,%d0\;\"       /* shift right ... */
            \"rrc %1,%0\;\"
            \"btsc %0,#0\;\"        /* if bit 0 is 0 */
            \"bclr SR,#0\;\"       /*   clear overflow bit */
            \"addc %0,#0,%0\;\"     /* add in drop overflow bit */
            \"addc %d0,#0,%d0\;\";
   }
   error(\"Invalid fixed-point round mode specified\n\");
   return \"cannot generate instruction\";
  "
  [
   (set_attr "cc" "clobber")
   (set_attr "type" "def")
  ]
)

(define_insn "ashr<mode>3_imm2to15_32"
  [(set (match_operand:QUQ31   0 "pic30_register_operand" "=r")
        (ashiftrt:QUQ31 
          (match_operand:QUQ31 1 "pic30_register_operand"  "r")
          (match_operand:HI    2 "pic30_imm2to15_operand"  "i")))
   (clobber (match_scratch:HI  3                         "=&r"))]
  ""
  "*
  {
    if ((pic30_fp_round_p() == pic30_truncation) ||
        (pic30_fp_round_p() == pic30_fastest)) {
      /*
      ** Take care that the source and dest don't overlap
      */
      if (REGNO(operands[0]) <= REGNO(operands[1])) {
        return \"sl %d1,#%k2,%3\;\"
               \"lsr %1,#%2,%0\;\"
               \"ior %3,%0,%0\;\"
               \"asr %d1,#%2,%d0\";
      } else {
        return \"asr %d1,#%2,%d0\;\"
               \"sl %d1,#%k2,%3\;\"
               \"lsr %1,#%2,%0\;\"
               \"ior %3,%0,%0\";
      }
    } else if (pic30_fp_round_p() == pic30_conventional) {
      if (REGNO(operands[0]) <= REGNO(operands[1])) {
        /* reorder a bit to preserve C for overflow */
        return \"sl %d1,#%k2,%3\;\"    /* shift up low bits of high word to t */
               \"asr %d1,#%2,%d0\;\"   /* shift down high bits of high word
                                          making the high part of result */
               \"lsr %1,#%L2,%0\;\"    /* shift down low bits of low word,
                                          1 less (clearing upper bits) */
               \"lsr %0,%0\;\"         /* final shift, capturing overflow in C*/
               \"ior %3,%0,%0\;\"      /* or in high bits of low word from t */
               \"addc %0,#0,%0\;\"     /* add in overflow */
               \"addc %d0,#0,%d0\;\";
      } else {
        return \"asr %d1,#%2,%d0\;\"   /* shift down high bits of high word
                                          making the high part of result */
               \"sl %d1,#%k2,%3\;\"    /* shift up low bits of high word to t */
               \"lsr %1,#%L2,%0\;\"    /* shift down low bits of low word,
                                          1 less (clearing upper bits) */
               \"lsr %0,%0\;\"         /* final shift, capturing overflow in C*/
               \"ior %3,%0,%0\;\"      /* or in high bits of low word from t */
               \"addc %0,#0,%0\;\"     /* add in overflow */
               \"addc %d0,#0,%d0\;\";
      }
    } else if (pic30_fp_round_p() == pic30_convergent) {
      if (REGNO(operands[0]) <= REGNO(operands[1])) {
        /* reorder a bit to preserve C for overflow */
        return \"sl %d1,#%k2,%3\;\"    /* shift up low bits of high word to t */
               \"asr %d1,#%2,%d0\;\"   /* shift down high bits of high word
                                          making the high part of result */
               \"sl  %1,#%K2,%0\;\"    /* shift up low bits (that drop) */
               \"sl  %0,%0\;\"         /* shift topmost into carry */
               \"btsc SR,#1\;\"       /* if zero clear carry */
               \"bclr SR,#0\;\"
               \"lsr %1,#%2,%0\;\"     /* shift down low bits of low word */
               \"ior %3,%0,%0\;\"      /* or in high bits of low word from t */
               \"addc %0,#0,%0\;\"     /* maybe add in overflow */
               \"addc %d0,#0,%d0\;\";
      } else {
        return \"asr %d1,#%2,%d0\;\"   /* shift down high bits of high word
                                          making the high part of result */
               \"sl %d1,#%k2,%3\;\"    /* shift up low bits of high word to t */
               \"sl  %0,%0\;\"         /* shift topmost into carry */
               \"btsc SR,#1\;\"       /* if zero clear carry */
               \"bclr SR,#0\;\"
               \"lsr %1,#%2,%0\;\"     /* shift down low bits of low word */
               \"ior %3,%0,%0\;\"      /* or in high bits of low word from t */
               \"addc %0,#0,%0\;\"     /* add in overflow */
               \"addc %d0,#0,%d0\;\";
      }
    } else
      error(\"Invalid fixed-point round mode specified\n\");
    return \"cannot generate instruction\";
  }"
  [
   (set_attr "cc" "clobber")
   (set_attr "type" "def")
  ]
)

(define_insn "ashr<mode>3_imm16plus_32"
  [(set (match_operand:QUQ31   0 "pic30_register_operand" "=&r")
        (ashiftrt:QUQ31 
          (match_operand:QUQ31 1 "pic30_register_operand"  "r")
          (match_operand:HI    2 "pic30_imm16plus_operand" "i")))]
  ""
  "*
   {
     if ((pic30_fp_round_p() == pic30_truncation) ||
         (pic30_fp_round_p() == pic30_fastest)) {
       return \"asr %d1,#%K2,%0\;asr %0,#15,%d0\";
     } else if (pic30_fp_round_p() == pic30_conventional) {
       if (INTVAL(operands[2]) == 16) {
         return 
                \"mov %d1,%0\;\"         /* copy high word to low result */
                \"asr %0,#15,%d0\;\"     /* shift down sign to high result */
                \"btst.c %1,#15\;\"      /* copy overflow bit to carry */
                \"addc %0,#0,%0\;\"      /* add in overflow bit */
                \"addc %d0,#0,%d0\";
       } else {
         return 
                \"asr %d1,#%K2,%0\;\"    /* copy high word to low result */
                \"asr %0,#15,%d0\;\"     /* shift down sign to high result */
                \"btst.c %d1,#(%K2)-1\;\"/* copy overflow bit to carry */
                \"addc %0,#0,%0\;\"      /* add in overflow bit */
                \"addc %d0,#0,%d0\";
       }
     } else
       error(\"Invalid fixed-point round mode specified\n\");
     return \"cannot generate instruction\";
   }"
  [
   (set_attr "cc" "clobber")
   (set_attr "type" "def")
  ]
)

(define_insn "ashr<mode>3_imm16plus_convergent_32"
  [(set (match_operand:QUQ31   0 "pic30_register_operand" "=&r")
        (ashiftrt:QUQ31 
          (match_operand:QUQ31 1 "pic30_register_operand"  "r")
          (match_operand:HI    2 "pic30_imm16plus_operand" "i")))
   (clobber (match_scratch:HI  3                          "=r"))
  ]
  ""
  "*
   {
     if (pic30_fp_round_p() == pic30_convergent) {
       if (INTVAL(operands[2]) == 16) {
         return 
                \"mov %d1,%0\;\"         /* copy high word to low result */
                \"asr %0,#15,%d0\;\"     /* shift down sign to high result */
                \"sl %1,%3\;\"           /* shift up dropped off bits */
                \"btsc SR,#1\;\"        /* if zero clear carry */
                \"bclr SR,#0\;\"
                \"addc %0,#0,%0\;\"      /* add in overflow bit */
                \"addc %d0,#0,%d0\";
       } else {
         return 
                \"asr %d1,#%K2,%0\;\"    /* copy high word to low result */
                \"asr %0,#15,%d0\;\"     /* shift down sign to high result */
                \"sl %d1,#%K2,%3\;\"     /* shift up dropped off bits */
                \"btsc SR,#1\;\"        /* if zero clear carry */
                \"bclr SR,#0\;\"
                \"addc %0,#0,%0\;\"      /* add in overflow bit */
                \"addc %d0,#0,%d0\";
       }
     } else
       error(\"Invalid fixed-point round mode specified\n\");
     return \"cannot generate instruction\";
   }"
  [
   (set_attr "cc" "clobber")
   (set_attr "type" "def")
  ]
)

(define_insn "ashr<mode>3_reg_32"
  [(set (match_operand:QUQ31   0 "pic30_register_operand" "=r")
        (ashiftrt:QUQ31 
          (match_operand:QUQ31 1 "pic30_register_operand"  "0")
          (match_operand:HI    2 "pic30_register_operand"  "r")))
   (clobber (match_scratch:HI  3                          "=2"))
   (clobber (match_scratch:HI  4                          "=&r"))
  ]
  ""
  "*
  {
    if ((pic30_fp_round_p() == pic30_truncation) ||
        (pic30_fp_round_p() == pic30_fastest)) {
      return \".set ___BP___,0\n\"
             \".LB%=:\;\"
             \"dec %2,%2\;\"
             \"bra n,.LE%=\;\"
             \"asr %d1,%d0\;\"
             \"rrc %1,%0\;\"
             \"bra .LB%=\n\"
             \".LE%=:\";
    } else if (pic30_fp_round_p() == pic30_conventional) {
      return \".set ___BP___,0\n\"
             \"cp0 %2\;\"
             \"bra z,.LE%=\;\"
             \".LB%=:\;\"
             \"dec %2,%2\;\"
             \"bra z,.LF%=\;\"
             \"asr %d1,%d0\;\"
             \"rrc %1,%0\;\"
             \"bra .LB%=\n\"
             \".LF%=:\;\"
             \"asr %d1,%d0\;\"
             \"rrc %1,%0\;\"
             \"addc %0,#0,%0\;\"    
             \"addc %d0,#0,%d0\n\"
             \".LE%=:\;\";
    } else if (pic30_fp_round_p() == pic30_convergent) {
      /* I would have liked to have this as a separate pattern; but
         it conflicts with ashr<mode>3_reg even though this would have had
         one more clobber - now they have the same */
      return 
             \"clr %4\;\"              /* clear overflow holder */
             \".set ___BP___,0\n\"
             \".LB%=:\;\"
             \"dec %2,%2\;\"
             \"bra n,.LE%=\;\"
             \"asr %d1,%d0\;\"
             \"rrc %1,%0\;\"
             \"rrc %4,%4\;\"           /* store overflow bits */
             \"bra .LB%=\n\"
             \".LE%=:\;\"
             \"sl %4,%4\"              /* check overflow */
             \"btsc SR,#1\;\"         /* if zero clear carry */
             \"bclr SR,#0\;\"
             \"addc %0,#0,%0\;\"
             \"addc %d0,#0,%d0\";
      
    } else
      error(\"Invalid fixed-point round mode specified\n\");
    return \"cannot generate instruction\";
  }"
  [
   (set_attr "cc" "clobber")
   (set_attr "type" "def")
  ]
)

; logical shift right

(define_insn "lshr<mode>3_helper_32"
  [(set (match_operand:QUQ15   0 "pic30_register_operand"  "=r,r,&r")
        (lshiftrt:QUQ15 
          (match_operand:QUQ15 1 "pic30_register_operand"   "r,r, r")
          (match_operand:HI    2 "pic30_reg_or_imm_operand" "Kh,i, r")))
  ]
  ""
  "*
  {
    if ((pic30_fp_round_p() == pic30_truncation) ||
        (pic30_fp_round_p() == pic30_fastest)) {
      switch (which_alternative) {
        case 0: 
          return \"lsr %1,#%2,%0\";
        case 1:
          if (INTVAL(operands[2]) < 0)
            return \"sl %1,#%J2%%16,%0\";
          else
            return \"lsr %1,#%2%%16,%0\";
        case 2: 
            return \"lsr %1,%2,%0\";
        default: gcc_assert(0);
      }
    } else if (pic30_fp_round_p() == pic30_conventional) 
    switch (which_alternative) {
      case 0:  
        if (INTVAL(operands[2]) == 1) {
          return 
                 \"lsr %1,%0\;\"
                 \"addc %0,#0,%0\";
        } else {
          return \"lsr %1,#%L2,%0\;\"      /* shift one less than we need */
                 \"lsr %1,%0\;\"           /*   so that we can shift once */
                 \"addc %0,#0,%0\";        /*   to capture and add carry */
        }
      case 1:
        if (INTVAL(operands[2]) < 0)
          return \"cannot generate instruction\"; 
        else
          return \"lsr %1,#(%2%%16)-1,%0\;\" /* shift one less than we need */
                 \"lsr %1,%0\;\"             /*   so that we can shift once */
                 \"addc %0,#0,%0\";          /*   to capture and add carry */
      case 2: 
	  return \"cp0 %2\;\"
                 \"mov %1,%0\;\"
                 \"bra z,.LE%=\;\"
                 \"dec %2,%0\;\"             /* shift one less than we need */
                 \"lsr %1,%0,%0\;\"
                 \"lsr %0,%0\;\"             /*   so that we can shift once */
                 \"addc %0,#0,%0\n\"         /*   to capture and add carry */
                 \".LE%=:\";
      default: gcc_assert(0);
    }
    else 
      error(\"Invalid fixed-point rounding mode specified\");
    return \"cannot generate instruction\";
  }"
  [
   (set_attr "cc" "math")
   (set_attr "type" "def")
  ]
)

(define_insn "lshr<mode>3_convergent_32"
  [(set (match_operand:QUQ15   0 "pic30_register_operand"  "=r,r,r")
        (lshiftrt:QUQ15
          (match_operand:QUQ15 1 "pic30_register_operand"   "r,r,r")
          (match_operand:HI    2 "pic30_reg_or_imm_operand" "Kh,i,r")))
   (clobber (match_scratch:HI  3                          "=&r,&r,&r"))
  ]
  ""
  "*
  {
    if (pic30_fp_round_p() == pic30_convergent)
    switch (which_alternative) {
      case 0:  
        if (INTVAL(operands[2]) == 1) {
          return 
                 \"lsr %1,%0\;\"
                 \"btsc %0,#0\;\"          /* do not add carry if low bit == 0*/
                 \"addc %0,#0,%0\";
        } else {
          return 
                 \"sl %1,%K2,%3\;\"        /* shift up bits that drop off */
                 \"lsr %1,#%2,%0\;\"       /* shift down the bits we need */
                 \"sl %3,%3\;\"            /* shift top overflow into carry */
                 \"addc %0,#0,%0\;\"       /* add overflow from carry */
                 \"cp0 %3\;\"              /* test rest of the overflow */
                 \"mov #0xFFFE,%3\;\"      /* create mask off low bit */
                 \"btsc SR,#1\;\"         /* if the rest of overflow is 0 */
                 \"and %0,%3,%0\n\"        /*   mask off low bit */
                 \".L1_%=:\";

        }
      case 1:
        if (INTVAL(operands[2]) < 0)
          return \"cannot generate instruction\"; 
        else
          return \"sl %1,#16-(%2%%16),%3\;\" /* shift up bits that drop off */
                 \"lsr %1,#(%2%%16),%0\;\"   /* shift down bits we need */
                 \"sl %3,%3\;\"              /* shift top overflow into carry */
                 \"bra nc,.L1_%=\;\"         /* if no carry, finish */
                 \"addc %0,#0,%0\;\"         /* add carry */
                 \"cp0 %3\;\"                /* test rest of the overflow */
                 \"mov #0xFFFE,%3\;\"        /* create mask clear low bit */
                 \"btsc SR,#1\;\"           /* if the rest of overflow is 0 */
                 \"and %0,%3,%0\n\"          /*   mask off low bit */
                 \".L1_%=:\";

      case 2: 
          return \"subr %2,#16,%3\;\"
                 \"sl %1,%3,%3\;\"           /* shift up bits that drop off */
                 \"lsr %1,%2,%0\;\"          /* shift down bits we need */
                 \"sl %3,%3\;\"              /* shift top overflow into carry */
                 \"bra nc,.L1_%=\;\"         /* if no carry, finish */
                 \"addc %0,#0,%0\;\"         /* add carry */
                 \"cp0 %3\;\"                /* test rest of the overflow */
                 \"mov #0xFFFE,%3\;\"        /* create mask clear low bit */
                 \"btsc SR,#1\;\"           /* if the rest of overflow is 0 */
                 \"and %0,%3,%0\n\"          /*   mask off low bit */
                 \".L1_%=:\";
       default: gcc_assert(0);
    } else
      error(\"Invalid fixed-point round mode specified\n\");
    return \"cannot generate instruction\";

  }"
  [
   (set_attr "cc" "math")
   (set_attr "type" "def")
  ]
)

(define_expand "lshr<mode>3_32"
  [(set (match_operand:QUQ15   0 "pic30_register_operand"   "")
        (lshiftrt:QUQ15
          (match_operand:QUQ15 1 "pic30_register_operand"   "")
          (match_operand:HI    2 "pic30_reg_or_imm_operand" "")))]
  ""
  "
  { rtx r0,r1,r2;

    r0 = operands[0];
    r1 = operands[1];
    r2 = operands[2];
    if (!pic30_register_operand(r0,<MODE>mode)) {
      r0 = gen_reg_rtx(<MODE>mode);
    }
    if (!pic30_register_operand(r1,<MODE>mode)) {
      r1 = force_reg(<MODE>mode, r1);
    }
    if (!pic30_reg_or_imm_operand(r2,<MODE>mode)) {
      r2 = force_reg(HImode, r2);
    }
    if (pic30_fp_round_p() == pic30_convergent) {
      emit(
        gen_lshr<mode>3_convergent_32(r0,r1,r2)
      );
    } else {
      emit(
        gen_lshr<mode>3_helper_32(r0,r1,r2)
      );
    }
    if (r0 != operands[0]) {
      emit_move_insn(operands[0],r0);
    }
    DONE;
  }"
)

(define_expand "lshr<mode>3_32"
  [(set (match_operand:QUQ31   0 "pic30_register_operand"   "")
        (lshiftrt:QUQ31 
          (match_operand:QUQ31 1 "pic30_register_operand"   "")
          (match_operand:HI    2 "pic30_reg_or_imm_operand" "")))]
  ""
  "
  {
    if ((pic30_fp_round_p() == pic30_conventional) ||
        (pic30_fp_round_p() == pic30_convergent)) 
      FAIL;
    if (GET_CODE(operands[2]) == CONST_INT) {
      switch (INTVAL(operands[2])) {
        case 0:
          emit_insn(
            gen_mov<mode>_32(operands[0], operands[1])
          );
          break;
        case 1:
          emit_insn(
            gen_lshr<mode>3_imm1_32(operands[0], operands[1], operands[2])
          );
          break;
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
          emit_insn(
            gen_lshr<mode>3_imm2to15_32(operands[0], operands[1], operands[2])
          );
          break;
        case 16:
          emit_insn(
            gen_lshr<mode>3_imm16plus_32(operands[0], operands[1], operands[2])
          );
          break;
        case 17:
        case 18:
        case 19:
        case 20:
        case 21:
        case 22:
        case 23:
        case 24:
        case 25:
        case 26:
        case 27:
        case 28:
        case 29:
        case 30:
        case 31:
          if (pic30_fp_round_p() == pic30_convergent) {
            emit_insn(
              gen_lshr<mode>3_imm16plus_convergent_32(operands[0], operands[1], 
                                                      operands[2])
            );
          } else {
            emit_insn(
              gen_lshr<mode>3_imm16plus_32(operands[0],operands[1], operands[2])
            );
          }
          break;
        default:
          emit_insn(
            gen_mov<mode>_32(operands[0], const0_rtx)
          );
          break;
      }
    } else {
      {
        emit_insn(
          gen_lshr<mode>3_reg_32(operands[0],operands[1],operands[2])
        );
      }
    }
    DONE;
  }"
)

(define_insn "lshr<mode>3_imm1_32"
  [(set (match_operand:QUQ31   0 "pic30_register_operand" "=r")
        (lshiftrt:QUQ31 
          (match_operand:QUQ31 1 "pic30_register_operand"  "r")
          (match_operand:HI    2 "pic30_I_operand"         "I")))]
  ""
  "*
   if ((pic30_fp_round_p() == pic30_truncation) ||
       (pic30_fp_round_p() == pic30_fastest)) {
     return \"asr %d1,%d0\;rrc %1,%0\";
   } else if (pic30_fp_round_p() == pic30_conventional) {
     return 
            \"lsr %d1,%d0\;\"       /* shift right ... */
            \"rrc %1,%0\;\"
            \"addc %0,#0,%0\;\"     /* add in drop overflow bit */
            \"addc %d0,#0,%d0\;\";
   } else if (pic30_fp_round_p() == pic30_convergent) {
     return 
            \"lsr %d1,%d0\;\"       /* shift right ... */
            \"rrc %1,%0\;\"
            \"btsc %0,#0\;\"        /* if bit 0 is 0 */
            \"bclr SR,#0\;\"       /*   clear overflow bit */
            \"addc %0,#0,%0\;\"     /* add in drop overflow bit */
            \"addc %d0,#0,%d0\;\";
   }
   error(\"Invalid fixed-point rounding mode specified\");
   return \"nop\";
  "
  [
   (set_attr "cc" "clobber")
   (set_attr "type" "def")
  ]
)

(define_insn "lshr<mode>3_imm2to15_32"
  [(set (match_operand:QUQ31   0 "pic30_register_operand" "=r")
        (lshiftrt:QUQ31 
          (match_operand:QUQ31 1 "pic30_register_operand"  "r")
          (match_operand:HI    2 "pic30_imm2to15_operand"  "i")))
   (clobber (match_scratch:HI  3                         "=&r"))]
  ""
  "*
  {
    if ((pic30_fp_round_p() == pic30_truncation) ||
        (pic30_fp_round_p() == pic30_fastest)) {
      /*
      ** Take care that the source and dest don't overlap
      */
      if (REGNO(operands[0]) <= REGNO(operands[1])) {
        return \"sl %d1,#%k2,%3\;\"
               \"lsr %1,#%2,%0\;\"
               \"ior %3,%0,%0\;\"
               \"lsr %d1,#%2,%d0\";
      } else {
        return \"lsr %d1,#%2,%d0\;\"
               \"sl %d1,#%k2,%3\;\"
               \"lsr %1,#%2,%0\;\"
               \"ior %3,%0,%0\";
      }
    } else if (pic30_fp_round_p() == pic30_conventional) {
      if (REGNO(operands[0]) <= REGNO(operands[1])) {
        /* reorder a bit to preserve C for overflow */
        return \"sl %d1,#%k2,%3\;\"    /* shift up low bits of high word to t */
               \"lsr %d1,#%2,%d0\;\"   /* shift down high bits of high word
                                          making the high part of result */
               \"lsr %1,#%L2,%0\;\"    /* shift down low bits of low word,
                                          1 less (clearing upper bits) */
               \"lsr %0,%0\;\"         /* final shift, capturing overflow in C*/
               \"ior %3,%0,%0\;\"      /* or in high bits of low word from t */
               \"addc %0,#0,%0\;\"     /* add in overflow */
               \"addc %d0,#0,%d0\;\";
      } else {
        return \"lsr %d1,#%2,%d0\;\"   /* shift down high bits of high word
                                          making the high part of result */
               \"sl %d1,#%k2,%3\;\"    /* shift up low bits of high word to t */
               \"lsr %1,#%L2,%0\;\"    /* shift down low bits of low word,
                                          1 less (clearing upper bits) */
               \"lsr %0,%0\;\"         /* final shift, capturing overflow in C*/
               \"ior %3,%0,%0\;\"      /* or in high bits of low word from t */
               \"addc %0,#0,%0\;\"     /* add in overflow */
               \"addc %d0,#0,%d0\;\";
      }
    } else if (pic30_fp_round_p() == pic30_convergent) {
      if (REGNO(operands[0]) <= REGNO(operands[1])) {
        /* reorder a bit to preserve C for overflow */
        return \"sl %d1,#%k2,%3\;\"    /* shift up low bits of high word to t */
               \"lsr %d1,#%2,%d0\;\"   /* shift down high bits of high word
                                          making the high part of result */
               \"sl  %1,#%K2,%0\;\"    /* shift up low bits (that drop) */
               \"sl  %0,%0\;\"         /* shift topmost into carry */
               \"btsc SR,#1\;\"       /* if zero clear carry */
               \"bclr SR,#0\;\"
               \"lsr %1,#%2,%0\;\"     /* shift down low bits of low word */
               \"ior %3,%0,%0\;\"      /* or in high bits of low word from t */
               \"addc %0,#0,%0\;\"     /* maybe add in overflow */
               \"addc %d0,#0,%d0\;\";
      } else {
        return \"lsr %d1,#%2,%d0\;\"   /* shift down high bits of high word
                                          making the high part of result */
               \"sl %d1,#%k2,%3\;\"    /* shift up low bits of high word to t */
               \"sl  %0,%0\;\"         /* shift topmost into carry */
               \"btsc SR,#1\;\"       /* if zero clear carry */
               \"bclr SR,#0\;\"
               \"lsr %1,#%2,%0\;\"     /* shift down low bits of low word */
               \"ior %3,%0,%0\;\"      /* or in high bits of low word from t */
               \"addc %0,#0,%0\;\"     /* add in overflow */
               \"addc %d0,#0,%d0\;\";
      }
    } else
      error(\"Invalid fixed-point round mode specified\n\");
    return \"cannot generate instruction\";
  }"
  [
   (set_attr "cc" "clobber")
   (set_attr "type" "def")
  ]
)

(define_insn "lshr<mode>3_imm16plus_32"
  [(set (match_operand:QUQ31   0 "pic30_register_operand" "=&r")
        (lshiftrt:QUQ31 
          (match_operand:QUQ31 1 "pic30_register_operand"  "r")
          (match_operand:HI    2 "pic30_imm16plus_operand" "i")))]
  ""
  "*
   {
     if ((pic30_fp_round_p() == pic30_truncation) ||
         (pic30_fp_round_p() == pic30_fastest)) {
       return \"asr %d1,#%K2,%0\;asr %0,#15,%d0\";
     } else if (pic30_fp_round_p() == pic30_conventional) {
       if (INTVAL(operands[2]) == 16) {
         return 
                \"mov %d1,%0\;\"         /* copy high word to low result */
                \"lsr %0,#15,%d0\;\"     /* shift down sign to high result */
                \"btst.c %1,#15\;\"      /* copy overflow bit to carry */
                \"addc %0,#0,%0\;\"      /* add in overflow bit */
                \"addc %d0,#0,%d0\";
       } else {
         return 
                \"asr %d1,#%K2,%0\;\"    /* copy high word to low result */
                \"lsr %0,#15,%d0\;\"     /* shift down sign to high result */
                \"btst.c %d1,#(%K2)-1\;\"/* copy overflow bit to carry */
                \"addc %0,#0,%0\;\"      /* add in overflow bit */
                \"addc %d0,#0,%d0\";
       }
     } else
       error(\"Invalid fixed-point round mode specified\n\");
     return \"cannot generate instruction\";
   }"
  [
   (set_attr "cc" "clobber")
   (set_attr "type" "def")
  ]
)

(define_insn "lshr<mode>3_imm16plus_convergent_32"
  [(set (match_operand:QUQ31   0 "pic30_register_operand" "=&r")
        (lshiftrt:QUQ31 
          (match_operand:QUQ31 1 "pic30_register_operand"  "r")
          (match_operand:HI    2 "pic30_imm16plus_operand" "i")))
   (clobber (match_scratch:HI  3                          "=r"))
  ]
  ""
  "*
   {
     if (pic30_fp_round_p() == pic30_convergent) {
       if (INTVAL(operands[2]) == 16) {
         return 
                \"mov %d1,%0\;\"         /* copy high word to low result */
                \"lsr %0,#15,%d0\;\"     /* shift down sign to high result */
                \"sl %1,%3\;\"           /* shift up dropped off bits */
                \"btsc SR,#1\;\"        /* if zero clear carry */
                \"bclr SR,#0\;\"
                \"addc %0,#0,%0\;\"      /* add in overflow bit */
                \"addc %d0,#0,%d0\";
       } else {
         return 
                \"asr %d1,#%K2,%0\;\"    /* copy high word to low result */
                \"lsr %0,#15,%d0\;\"     /* shift down sign to high result */
                \"sl %d1,#%K2,%3\;\"     /* shift up dropped off bits */
                \"btsc SR,#1\;\"        /* if zero clear carry */
                \"bclr SR,#0\;\"
                \"addc %0,#0,%0\;\"      /* add in overflow bit */
                \"addc %d0,#0,%d0\";
       }
     } else
       error(\"Invalid fixed-point round mode specified\n\");
     return \"cannot generate instruction\";
   }"
  [
   (set_attr "cc" "clobber")
   (set_attr "type" "def")
  ]
)

(define_insn "lshr<mode>3_reg_32"
  [(set (match_operand:QUQ31   0 "pic30_register_operand" "=r")
        (lshiftrt:QUQ31 
          (match_operand:QUQ31 1 "pic30_register_operand"  "0")
          (match_operand:HI    2 "pic30_register_operand"  "r")))
   (clobber (match_scratch:HI  3                          "=2"))
   (clobber (match_scratch:HI  4                          "=&r"))
  ]
  ""
  "*
  {
    if ((pic30_fp_round_p() == pic30_truncation) ||
        (pic30_fp_round_p() == pic30_fastest)) {
      return \".set ___BP___,0\n\"
             \".LB%=:\;\"
             \"dec %2,%2\;\"
             \"bra n,.LE%=\;\"
             \"lsr %d1,%d0\;\"
             \"rrc %1,%0\;\"
             \"bra .LB%=\n\"
             \".LE%=:\";
    } else if (pic30_fp_round_p() == pic30_conventional) {
      return \".set ___BP___,0\n\"
             \"cp0 %2\;\"
             \"bra z,.LE%=\;\"
             \".LB%=:\;\"
             \"dec %2,%2\;\"
             \"bra z,.LF%=\;\"
             \"lsr %d1,%d0\;\"
             \"rrc %1,%0\;\"
             \"bra .LB%=\n\"
             \".LF%=:\;\"
             \"asr %d1,%d0\;\"
             \"rrc %1,%0\;\"
             \"addc %0,#0,%0\;\"    
             \"addc %d0,#0,%d0\n\"
             \".LE%=:\;\";
    } else if (pic30_fp_round_p() == pic30_convergent) {
      /* I would have liked to have this as a separate pattern; but
         it conflicts with lshr<mode>3_reg even though this would have had
         one more clobber - now they have the same */
      return 
             \"clr %4\;\"              /* clear overflow holder */
             \".set ___BP___,0\n\"
             \".LB%=:\;\"
             \"dec %2,%2\;\"
             \"bra n,.LE%=\;\"
             \"lsr %d1,%d0\;\"
             \"rrc %1,%0\;\"
             \"rrc %4,%4\;\"           /* store overflow bits */
             \"bra .LB%=\n\"
             \".LE%=:\;\"
             \"sl %4,%4\"              /* check overflow */
             \"btsc SR,#1\;\"         /* if zero clear carry */
             \"bclr SR,#0\;\"
             \"addc %0,#0,%0\;\"
             \"addc %d0,#0,%d0\";
      
    } else
      error(\"Invalid fixed-point round mode specified\n\");
    return \"cannot generate instruction\";
  }"
  [
   (set_attr "cc" "clobber")
   (set_attr "type" "def")
  ]
)

; compare

(define_insn "cmp<mode>_imm_32"
  [(set (reg:CC CC_REG)
        (compare
          (match_operand:QUQ15 0 "pic30_register_operand" "r")  
          (match_operand:QUQ15 1 "pic30_M_operand"        "M"))) 
   (clobber (match_dup 0))]
  "(pic30_isa32_target())"
  "add #%J1,%0"
  [(set_attr "cc" "set")])
   
(define_insn "cmp<mode>_sfr0_32"
  [(set (reg:CC CC_REG)
        (compare 
          (match_operand:QUQ15 0 "pic30_reg_or_near_operand" "U,r")
          (match_operand:QUQ15 1 "pic30_register_operand"    "a,r")))]
  ""
  "@
   cp %0
   sub %0,%1,[w15]"
  [(set_attr "cc" "set")])
  
(define_insn "cmp<mode>_32"
  [(set (reg:CC CC_REG)
        (compare 
          (match_operand:QUQ15 0 "pic30_mode2_operand"   "r,r,  R<>,r,r")
          (match_operand:QUQ15 1 "pic30_mode1PN_operand" "r,R<>,r,  P,N")))]
  ""
  "@
   sub %0,%1,[w15]
   sub %0,%1,[w15]
   subr %1,%0,[w15]
   sub %0,#%1,[w15]
   add %0,#%J1,[w15]"
  [ 
   (set_attr "cc" "set")
   (set_attr "type" "etc,use,use,etc,etc")
  ]
) 

(define_insn "cmp<mode>_helper_32"
  [(set (reg:CC CC_REG)
        (compare 
          (match_operand:QUQ31 0 "pic30_mode2mres_operand" "r,r,R,r,>")
          (match_operand:QUQ31 1 "pic30_mode2mres_operand" "r,R,r,>,r")))]
  ""
  "*
{
  static const char *patterns[] = {
     \"sub %0,%1,[w15]\;subb %d0,%d1,[w15]\",
     \"sub %0,%I1,[w15]\;subb %d0,%D1,[w15]\",
     \"subr %1,%I0,[w15]\;subbr %d1,%D0,[w15]\",
     \"sub %0,%1,[w15]\;subb %d0,%1,[w15]\",
     \"subr %1,%0,[w15]\;subbr %d1,%0,[w15]\",
  0 };

  static const char *psrd_psrd_patterns[] = {
     \"sub %0,%1,[w15]\;subb %d0,%d1,[w15]\",
     \"sub %0,%I1,[w15]\;nop\;subb %d0,%D1,[w15]\",
     \"subr %1,%I0,[w15]\;nop\;subbr %d1,%D0,[w15]\",
     \"sub %0,%1,[w15]\;nop\;subb %d0,%1,[w15]\",
     \"subr %1,%0,[w15]\;nop\;subbr %d1,%0,[w15]\",
  0 };

  if (pic30_psrd_psrd_errata(operands[0],operands[1])) {
    pic30_rtx_nops++;
    return psrd_psrd_patterns[which_alternative];
  } else {
  return patterns[which_alternative];
  }
}"
  [(set_attr "cc" "set")])

(define_insn "cmp<mode>_zero_32"
  [(set (reg:CC CC_REG)
        (compare 
          (match_operand:QUQ31 0 "pic30_register_operand" "r")
          (match_operand:QUQ31 1 "pic30_O_operand"        "fO")))]
  ""
  "sub %0,#0,[w15]\;subb %d0,#0,[w15]"
  [(set_attr "cc" "set")])

(define_insn "cmp<mode>_immNP_32"
  [(set (reg:CC CC_REG)
        (compare 
          (match_operand:QUQ31 0 "pic30_register_operand"  "r,r,r")
          (match_operand:QUQ31 1 "immediate_operand"       "P,N,i")))]
  "(((-31 <= INTVAL(operands[1])) && (INTVAL(operands[1]) <= 31)) ||
    ((0xFFE1 <= INTVAL(operands[1])) && (INTVAL(operands[1]) <= 0xFFFF))) &&
   (INTVAL(operands[1]) != 0) &&
   (pic30_isa32_target())"
  "@
   sub %0,#%1,[w15]\;subb %d0,#0,[w15]
   add %0,#%J1,[w15]\;addc %d0,#0,[w15]
   add %0,#%j1,[w15]\;subb %d0,#0,[w15]"
  [(set_attr "cc" "set")])

(define_insn "cmp<mode>_imm_32"
  [(set (reg:CC CC_REG)
        (compare 
          (match_operand:QUQ31 0 "pic30_register_operand"  "r")
          (match_operand:QUQ31 1 "immediate_operand"       "i")))
   (clobber (match_scratch:HI  2                         "=&r"))]
  "((1<INTVAL(operands[1])) && (INTVAL(operands[1])<65536)) &&
   (pic30_isa32_target())"
  "mov #%1,%2\;sub %0,%2,[w15]\;subb %d0,#0,[w15]"
  [(set_attr "cc" "set")])

(define_expand "cmp<mode>_32"
  [(set (reg:CC CC_REG)
        (compare 
          (match_operand:QUQ31 0 "pic30_mode2mres_operand" "")
          (match_operand:QUQ31 1 "pic30_mode2mres_operand" "")))]
  ""
  "
{
  if (pic30_mode2mres_operand(operands[1], <MODE>mode)) {
    emit(
      gen_cmp<mode>_helper_32(operands[0],operands[1])
    );
  } else if (immediate_operand(operands[1],VOIDmode)) {
    rtx reg_0 = force_reg(<MODE>mode,operands[0]);
    if (INTVAL(operands[1]) == 0) {
      emit(
        gen_cmp<mode>_zero_32(reg_0, operands[1])
      );
    } else {
      rtx reg = force_reg(<MODE>mode,operands[1]);
      emit( 
        gen_cmp<mode>_helper_32(reg_0, reg)
      );
    }
  }
  DONE;
}")

; Fixed Point Accum

;; *** load/store

(define_insn "mov<mode>_rimm_32"
   [(set (match_operand:AUACC  0 "pic30_accum_or_reg_operand" "=w,?????w,???r")
         (match_operand:AUACC  1 "immediate_operand"           "fO,    i,   i"))
    (clobber (match_scratch:HI 2                              "=X,     &r,  X"))
   ]
   ""
   "*
    {
      int zero = 0;

      zero = (CONST_FIXED_VALUE_LOW(operands[1]) == 0) &&
             (CONST_FIXED_VALUE_HIGH(operands[1]) == 0);
      if (REGNO(operands[0]) == A_REGNO) {
        if (zero) {
          return \"clr A\";
        } else {
          return 
               \"mov #%z1,%2\;\"
               \"mov %2,ACCAL\;\"
               \"mov #%y1,%2\;\"
               \"mov %2,ACCAH\;\"
               \"mov #%x1,%2\;\"
               \"mov %2,ACCAU\";
        }
      } else if (REGNO(operands[0]) == B_REGNO) {
        if (zero) {
          return \"clr B\";
        } else {
          return 
               \"mov #%z1,%2\;\"
               \"mov %2,ACCBL\;\"
               \"mov #%y1,%2\;\"
               \"mov %2,ACCBH\;\"
               \"mov #%x1,%2\;\"
               \"mov %2,ACCBU\";
        }
      } else {
        if (zero) {
          return 
               \"mul.uu %0,#0,%0\;\"
               \"mov.b #%x1,%t0\";
        } else {
          return 
               \"mov #%z1,%0\;\"
               \"mov #%y1,%d0\;\"
               \"mov #%x1,%t0\";
        }
      }
    }"
)

(define_insn "reload<mode>_imm_32"
   [(set (match_operand:AUACC  0 "pic30_accumulator_operand" "= w")
         (match_operand:AUACC  1 "immediate_operand"         "  i"))
    (clobber (match_operand:HI 2 "pic30_register_operand"    "=&r"))
   ]
   ""
   "*
    {
      int zero = 0;

      zero = (CONST_FIXED_VALUE_LOW(operands[1]) == 0) &&
             (CONST_FIXED_VALUE_HIGH(operands[1]) == 0);
      if (REGNO(operands[0]) == A_REGNO) {
        if (zero) {
          return \"clr A\";
        } else {
          return
               \"mov #%z1,%2\;\"
               \"mov %2,ACCAL\;\"
               \"mov #%y1,%2\;\"
               \"mov %2,ACCAH\;\"
               \"mov #%x1,%2\;\"
               \"mov %2,ACCAU\";
        }
      } else if (REGNO(operands[0]) == B_REGNO) {
        if (zero) {
          return \"clr B\";
        } else {
          return
               \"mov #%z1,%2\;\"
               \"mov %2,ACCBL\;\"
               \"mov #%y1,%2\;\"
               \"mov %2,ACCBH\;\"
               \"mov #%x1,%2\;\"
               \"mov %2,ACCBU\";
        }
      } else {
        gcc_assert(0);
      }
    }"
)

(define_insn "reloadsi_imm_32"
   [(set (match_operand:SI     0 "pic30_accumulator_operand" "= w")
         (match_operand:SI     1 "immediate_operand"         "  i"))
    (clobber (match_operand:HI 2 "pic30_register_operand"    "=&r"))
   ]
   ""
   "*
    {
      int zero = 0;

      zero = (CONST_FIXED_VALUE_LOW(operands[1]) == 0) &&
             (CONST_FIXED_VALUE_HIGH(operands[1]) == 0);
      if (REGNO(operands[0]) == A_REGNO) {
        if (zero) {
          return \"clr A\";
        } else {
          return
               \"mov #%z1,%2\;\"
               \"mov %2,ACCAL\;\"
               \"mov #%y1,%2\;\"
               \"mov %2,ACCAH\;\";
        }
      } else if (REGNO(operands[0]) == B_REGNO) {
        if (zero) {
          return \"clr B\";
        } else {
          return
               \"mov #%z1,%2\;\"
               \"mov %2,ACCBL\;\"
               \"mov #%y1,%2\;\"
               \"mov %2,ACCBH\;\";
        }
      } else {
        gcc_assert(0);
      }
    }"
)

; NB: like movsi_gen this does not cover all possible versions of move_operand
;     ugh

(define_insn "mov<mode>_gen_32"
   [(set
      (match_operand:AUACC 0 
         "pic30_move_operand" "=wr,????wr,????wr,????R, >, >, wr,  <, r, Q")
      (match_operand:AUACC 1 
         "pic30_move_operand"  "wr,     R,     >,   wr,wr, >,  <, wr, Q, r"))
   ]
   ""
   "*
    switch (which_alternative) {
      default: gcc_assert(0);

      case 0:  /* wr,wr */
               if (pic30_accumulator_operand(operands[0],
                                             <MODE>mode) &&
                   pic30_accumulator_operand(operands[1],
                                             <MODE>mode)) {
                 /* saturation mode doesn't matter... we won't overflow */
                 if (REGNO(operands[0]) == A_REGNO) {
                   return \"clr A\;\"
                          \"add A\";
                 } else if (REGNO(operands[0]) == B_REGNO) {
                   return \"clr B\;\"
                          \"add B\";
                 } else gcc_assert(0);
               } else if (pic30_accumulator_operand(operands[0],
                                                    <MODE>mode)) {
                 if (REGNO(operands[0]) == A_REGNO) {
                   return \"mov %1,ACCAL\;\"
                          \"mov %d1,ACCAH\;\"
                          \"mov %t1,ACCAU\";
                 } else if (REGNO(operands[0]) == B_REGNO) {
                   return \"mov %1,ACCBL\;\"
                          \"mov %d1,ACCBH\;\"
                          \"mov %t1,ACCBU\";
                 } else gcc_assert(0);
               } else if (pic30_accumulator_operand(operands[1],
                                                    <MODE>mode)) {
                 if (REGNO(operands[1]) == A_REGNO) {
                   return \"mov ACCAL,%0\;\"
                          \"mov ACCAH,%d0\;\"
                          \"mov ACCAU,%t0\";
                 } else if (REGNO(operands[1]) == B_REGNO) {
                   return \"mov ACCBL,%0\;\"
                          \"mov ACCBH,%d0\;\"
                          \"mov ACCBU,%t0\";
                 } else gcc_assert(0);
               } else {
                 /* Try and prevent clobbering, apparently using an &
                    early clobber confuses reload */
                 /* op0 and op1 regnos will only be even ... */
                 if (REGNO(operands[0]) == REGNO(operands[1])) {
                   return \"; nop\";
                 } else if (REGNO(operands[0]) == REGNO(operands[1])+2) {
                   return \"mov.b %t1,%t0\;\"
                          \"mov.d %1,%0\";
                 } /* else the other overlap doesn't matter */ 
                 return \"mov.d %1,%0\;\"
                        \"mov.b %t1,%t0\";
               }
               break;
      case 1:  /* wr,R */
               if (pic30_accumulator_operand(operands[0],
                                             <MODE>mode)) {
                 if (REGNO(operands[0]) == A_REGNO) {
                   return \"mov %I1,[w15++]\;\"
                          \"pop ACCAL\;\"
                          \"mov %I1,[W15++]\;\"
                          \"pop ACCAH\;\"
                          \"mov %D1,[W15++]\;\"
                          \"pop ACCAU\;\"
                          \"dec2 %r1,%r1\";
                 } else if (REGNO(operands[0]) == B_REGNO) {
                   return \"mov %I1,[w15++]\;\"
                          \"pop ACCBL\;\"
                          \"mov %I1,[W15++]\;\"
                          \"pop ACCBH\;\"
                          \"mov %D1,[W15++]\;\"
                          \"pop ACCBU\;\"
                          \"dec2 %r1,%r1\";
                 } else gcc_assert(0);
               } else {
                 /* Try and prevent clobbering, apparently using an
                    & earlyclobber confuses reload */
                 unsigned int index;
                 index = REGNO(XEXP(operands[1],0));
                 if (pic30_psrd_psrd_errata(operands[1],NULL)) {
                   pic30_rtx_nops+=2;
                 if (index == REGNO(operands[0])) {
                   /* write to the index register last */
                   return \"mov %Q1,%d0\;\"
                            \"nop\;\"
                          \"mov.b %R1,%t0\;\"
                            \"nop\;\"
                          \"mov %1,%0\";
                 } else if (index == REGNO(operands[0])+1) {
                   /* write to the index+1 register last */
                   return \"mov.b %R1,%t0\;\"
                            \"nop\;\"
                          \"mov %1,%0\;\"
                            \"nop\;\"
                          \"mov %Q1,%d0\";
                 } else {
                   /* maybe write to the index+2 register last */
                   return \"mov %1,%0\;\"
                            \"nop\;\"
                          \"mov %Q1,%d0\;\"
                            \"nop\;\"
                          \"mov.b %R1,%t0\";
                 }
                 } else {
                   if (index == REGNO(operands[0])) {
                     /* write to the index register last */
                     return \"mov %Q1,%d0\;\"
                            \"mov.b %R1,%t0\;\"
                            \"mov %1,%0\";
                   } else if (index == REGNO(operands[0])+1) {
                     /* write to the index+1 register last */
                     return \"mov.b %R1,%t0\;\"
                            \"mov %1,%0\;\"
                            \"mov %Q1,%d0\";
                   } else {
                     /* maybe write to the index+2 register last */
                     return \"mov %1,%0\;\"
                            \"mov %Q1,%d0\;\"
                            \"mov.b %R1,%t0\";
               }
                 }
               }
               break;
      case 2:  /* wr,> */
               if (pic30_accumulator_operand(operands[0],
                                             <MODE>mode)) {
                 if (REGNO(operands[0]) == A_REGNO) {
                   return \"mov %I1,[w15++]\;\"
                          \"pop ACCAL\;\"
                          \"mov %I1,[W15++]\;\"
                          \"pop ACCAH\;\"
                          \"mov %I1,[W15++]\;\"
                          \"pop ACCAU\";
                 } else if (REGNO(operands[0]) == B_REGNO) {
                   return \"mov %I1,[w15++]\;\"
                          \"pop ACCBL\;\"
                          \"mov %I1,[W15++]\;\"
                          \"pop ACCBU\;\"
                          \"mov %I1,[W15++]\;\"
                          \"pop ACCBH\;\";
                 } else gcc_assert(0);
               } else {
                 /* Try and prevent clobbering, apparently using an
                    & earlyclobber confuses reload */
                 unsigned int index;
                 index = REGNO(XEXP(operands[1],0));
                 if (pic30_psrd_psrd_errata(operands[1],NULL)) {
                   pic30_rtx_nops+=2;
                 if (index == REGNO(operands[0])) {
                   /* write to the index register last */
                   return \"mov %Q1,%d0\;\"
                            \"nop\;\"
                          \"mov.b %R1,%t0\;\"
                            \"nop\;\"
                          \"mov %1,%0\;\"
                          \"add %1,#6,%1\";
                 } else if (index == REGNO(operands[0])+1) {
                   /* write to the index+1 register last */
                   return \"mov.b %R1,%t0\;\"
                            \"nop\;\"
                          \"mov %1,%0\;\"
                            \"nop\;\"
                          \"mov %Q1,%d0\;\"
                          \"add %1,#6,%1\";
                 } else {
                   /* maybe write to the index+2 register last */
                   return \"mov %I1,%0\;\"
                            \"nop\;\"
                          \"mov %I1,%d0\;\"
                            \"nop\;\"
                          \"mov.b %1,%t0\";
                 }
                 } else {
                   if (index == REGNO(operands[0])) {
                     /* write to the index register last */
                     return \"mov %Q1,%d0\;\"
                            \"mov.b %R1,%t0\;\"
                            \"mov %1,%0\;\"
                            \"add %1,#6,%1\";
                   } else if (index == REGNO(operands[0])+1) {
                     /* write to the index+1 register last */
                     return \"mov.b %R1,%t0\;\"
                            \"mov %1,%0\;\"
                            \"mov %Q1,%d0\;\"
                            \"add %1,#6,%1\";
                   } else {
                     /* maybe write to the index+2 register last */
                     return \"mov %I1,%0\;\"
                            \"mov %I1,%d0\;\"
                            \"mov.b %1,%t0\";
               }
                 }
               }
               break;
      case 3:  /* R,wr */
               if (pic30_accumulator_operand(operands[1],
                                             <MODE>mode)) {
                 return \"push %m1U\;\"
                        \"push %m1H\;\"
                        \"push %m1L\;\"
                        \"mov [--w15],%I0\;\"
                        \"mov [--W15],%I0\;\"
                        \"mov [--W15],%D0\;\"
                        \"dec2 %r0,%r0\";
               } else {
                 return \"mov %1,%I0\;\"
                        \"mov %d1,%I0\;\"
                        \"mov.b %t1,%0\;\"
                        \"sub %r0,#4,%r0\";
               }
               break;
      case 4:  /* >,wr */
               if (pic30_accumulator_operand(operands[1],
                                             <MODE>mode)) {
                 rtx reg;
                 rtx post_inc = XEXP(operands[0],0);

                 reg = XEXP(post_inc,0);
                 if (REGNO(reg) == WR15_REGNO) {
                   /* push onto the stack */
                   return \"push %m1L\;\"
                          \"push %m1H\;\"
                          \"push %m1U\";
                 } else {
                   /* push the value backward, so we can pop it the right way
                      round */
                   return \"push %m1U\;\"
                          \"push %m1H\;\"
                          \"push %m1L\;\"
                          \"mov [--w15],%0\;\"
                          \"mov [--W15],%0\;\"
                          \"mov [--W15],%0\";
                 }
               } else {
                 return \"mov %1,%0\;\"
                        \"mov %d1,%0\;\"
                        \"mov %t1,%0\";
               }
               break;
      case 5:  /* >,> */
               if (pic30_psrd_psrd_errata(operands[1],NULL)) {
               return \"mov %1,%0\;\"
                        \"nop\;\"
                      \"mov %1,%0\;\" 
                        \"nop\;\"
                      \"mov %1,%0\;\";
               } else {
                 return \"mov %1,%0\;\"
                        \"mov %1,%0\;\" 
                        \"mov %1,%0\;\";
               }
               break;
      case 6:  /* wr,< */
               if (pic30_accumulator_operand(operands[0],
                                             <MODE>mode)) {
                 rtx reg;
                 rtx pre_dec = XEXP(operands[1],0);

                 reg = XEXP(pre_dec,0);
                 if (REGNO(reg) == WR15_REGNO) {
                   return \"pop %m0U\;\"
                          \"pop %m0H\;\"
                          \"pop %m0L\";
                 } else {
                   return \"mov %1,[w15++]\;\"
                          \"pop %m0U\;\"
                          \"mov %1,[W15++]\;\"
                          \"pop %m0H\;\"
                          \"mov %1,[W15++]\;\"
                          \"pop %m0L\";
                 }
               } else {
                 if (pic30_psrd_psrd_errata(operands[1],NULL)) {
                   pic30_rtx_nops+=2;
                 return \"mov %1,%t0\;\"
                          \"nop\;\"
                        \"mov %1,%d0\;\"
                          \"nop\;\"
                        \"mov %1,%0\";
                 } else {
                   return \"mov %1,%t0\;\"
                          \"mov %1,%d0\;\"
                          \"mov %1,%0\";
               }
               }
               break;
      case 7:  /* <,wr */
               if (pic30_accumulator_operand(operands[1],
                                             <MODE>mode)) {
                 return \"push %m1L\;\"
                        \"push %m1H\;\"
                        \"push %m1U\;\"
                        \"mov [--w15],%0\;\"
                        \"mov [--W15],%0\;\"
                        \"mov [--W15],%0\";
               } else {
                 return \"mov %t1,%0\;\"
                        \"mov %d1,%0\;\"
                        \"mov %1,%0\";
               }
               break;
      case 8:  /* r,Q */
               if (pic30_psrd_psrd_errata(operands[1],NULL)) {
                 pic30_rtx_nops+=2;
               return \"mov %1,%0\;\"
                        \"nop\;\"
                      \"mov %Q1,%d0\;\"
                        \"nop\;\"
                      \"mov %R1,%t0\;\";
               } else {
                 return \"mov %1,%0\;\"
                        \"mov %Q1,%d0\;\"
                        \"mov %R1,%t0\;\";
               }
      case 9:  /* Q,r */
               return \"mov %1,%0\;\"
                      \"mov %d1,%Q0\;\"
                      \"mov %t1,%R0;\";
    }"
)

(define_expand "mov<mode>_32"
  [(set (match_operand:AUACC 0 "pic30_move2_operand" "")
        (match_operand:AUACC 1 "pic30_move2_operand" ""))]
  ""
  "
{ int result;

  result = pic30_emit_move_sequence(operands, <MODE>mode);

  if (result > 0) DONE;
  if (immediate_operand(operands[1],VOIDmode)) {
    rtx reg = operands[0];

    if (!pic30_accum_or_reg_operand(operands[0],<MODE>mode)) {
      if (pic30_reload_in_progress()) FAIL;
      reg = gen_reg_rtx(<MODE>mode);
    }

    if (pic30_reload_in_progress() && 
        pic30_accumulator_operand(operands[0],<MODE>mode)) {
      FAIL; 
    }
    emit(
      gen_mov<mode>_rimm_32(reg,operands[1])
    );
    if (reg != operands[0])
      emit_move_insn(operands[0], reg);
    DONE;
  }
  emit(
    gen_mov<mode>_gen_32(operands[0], operands[1])
  );
  DONE;
}")


(define_insn "add<mode>3_32"
  [(set (match_operand:AUACC    0 "pic30_accum_or_reg_operand" "=&r,w")
        (plus:AUACC
           (match_operand:AUACC 1 "pic30_accum_or_reg_operand"  "%r,0")
           (match_operand:AUACC 2 "pic30_accum_or_reg_operand"  " r,w")))
   (use (reg:HI CORCON))
  ]
  ""
  "@
   add %1,%2,%0\;addc %d1,%d2,%d0\;addc.b %t1,%t2,%t0
   add %0"
)


(define_insn "ssadd<mode>3_32"
  [(set (match_operand:AUACC    0 "pic30_accum_or_reg_operand" "=&r,w")
        (ss_plus:AUACC
           (match_operand:AUACC 1 "pic30_accum_or_reg_operand"  "%r,0")
           (match_operand:AUACC 2 "pic30_accum_or_reg_operand"  " r,0")))
   (use (reg:HI CORCON))
  ]
  ""
  "@
   add %1,%2,%0\;addc %d1,%d2,%d0\;addc.b %t1,%t2,%t0\;bra nov,.L1_%=\;setm %0\;setm %d0\;mov.b #0x7F,%t0\;btst.c %t2,#7\;bra nc,.L1_%=\;com.b %t0,%t0\;com %d0,%d0\;com %0,%0\;.L1_%=:
   add %0"
)

(define_insn "usadd<mode>3_32"
  [(set (match_operand:AUACC    0 "pic30_accum_or_reg_operand" "=&r,w")
        (us_plus:AUACC
           (match_operand:AUACC 1 "pic30_accum_or_reg_operand"  "%r,0")
           (match_operand:AUACC 2 "pic30_accum_or_reg_operand"   "r,w")))
   (use (reg:HI CORCON))
  ]
  ""
  "@
   add %1,%2,%0\;addc %d1,%d2,%d0\;addc.b %t1,%t2,%t0\;bra nov,.L1_%=\;setm %0\;setm %d0\;mov.b #0x7F,%t0\;.L1_%=:
   add %0"
)

(define_insn "sub<mode>3_32"
  [(set (match_operand:AUACC    0 "pic30_accum_or_mode1_operand" "=&r,>,r,>,r,R,R,w")
        (minus:AUACC
           (match_operand:AUACC 1 "pic30_accum_or_reg_operand"    " r,r,r,r,r,r,r,0")
           (match_operand:AUACC 2 "pic30_accum_or_mode1_operand"  " r,r,>,>,R,r,R,w")))
   (use (reg:HI CORCON))
  ]
  ""
  "*
{
   const char *patterns[] = {
     \"sub %1,%2,%0\;subb %d1,%d2,%d0\;subb.b %t1,%t2,%t0\",
     \"sub %1,%2,%I0\;subb %d1,%d2,%I0\;subb %t1,%t2,%I0\",
     \"sub %1,%I2,%0\;subb %d1,%I2,%d0\;subb.b %t1,%I2,%t0\",
     \"sub %1,%I2,%I0\;subb %d1,%I2,%I0\;subb %t1,%I2,%I0\",
     \"sub %1,%I2,%0\;subb %d1,%I2,%d0\;subb %t1,%D2,%t0\;dec2 %r2,%r2\",
     \"sub %1,%2,%I0\;subb %d1,%d2,%I0\;subb %t1,%t2,%D0\;dec2 %r0,%r0\",
     \"sub %1,%I2,%I0\;subb %d1,%I2,%I0\;subb %t1,%D2,%D0\;dec2 %r0,%r0\;dec2 %r2,%r2\",
     \"sub %0\"
   };

   const char *psrd_psrd_patterns[] = {
     \"sub %1,%2,%0\;subb %d1,%d2,%d0\;subb.b %t1,%t2,%t0\",
     \"sub %1,%2,%I0\;subb %d1,%d2,%I0\;subb %t1,%t2,%I0\",
     \"sub %1,%I2,%0\;nop\;subb %d1,%I2,%d0\;nop\;subb.b %t1,%I2,%t0\",
     \"sub %1,%I2,%I0\;nop\;subb %d1,%I2,%I0\;nop\;subb %t1,%I2,%I0\",
     \"sub %1,%I2,%0\;nop\;subb %d1,%I2,%d0\;nop\;subb %t1,%D2,%t0\;dec2 %r2,%r2\",
     \"sub %1,%2,%I0\;subb %d1,%d2,%I0\;subb %t1,%t2,%D0\;dec2 %r0,%r0\",
     \"sub %1,%I2,%I0\;nop\;subb %d1,%I2,%I0\;nop\;subb %t1,%D2,%D0\;dec2 %r0,%r0\;dec2 %r2,%r2\",
     \"sub %0\"
   };
   
   if (pic30_psrd_psrd_errata(operands[2],NULL)) {
     pic30_rtx_nops+=2;
     return psrd_psrd_patterns[which_alternative];
   } else {
     return patterns[which_alternative];
   }
}"
)

(define_insn "sssub<mode>3_32"
  [(set (match_operand:AUACC    0 "pic30_accum_or_reg_operand" "=&r,w")
        (ss_minus:AUACC
           (match_operand:AUACC 1 "pic30_accum_or_reg_operand"  " r,0")
           (match_operand:AUACC 2 "pic30_accum_or_reg_operand"  " r,w")))
   (use (reg:HI CORCON))
  ]
  ""
  "@
   sub %1,%2,%0\;subb %d1,%d2,%d0\;subb.b %t1,%t2,%t0\;bra nov,.L1_%=\;setm %0\;setm %d0\;mov.b #0x7F,%t0\;btst.c %t2,#7\;bra c,.L1_%=\;com.b %t0,%t0\;com %d0,%d0\;com %0,%0\;.L1_%=:
   sub %0"
)

(define_insn "ussub<mode>3_32"
  [(set (match_operand:AUACC    0 "pic30_accum_or_reg_operand" "=&r,w")
        (us_minus:AUACC
           (match_operand:AUACC 1 "pic30_accum_or_reg_operand"  " r,0")
           (match_operand:AUACC 2 "pic30_accum_or_reg_operand"  " r,w")))
   (use (reg:HI CORCON))
  ]
  ""
  "@
   sub %1,%2,%0\;subb %d1,%d2,%d0\;subb.b %t1,%t2,%t0\;bra nn,.L1_%=\;mul.uu %0,#0,%0\;clr.b %t0\;.L1_%=:
   sub %0"
)

;; multiply of accumulators, ie 8.31*8.31 will be handled by a function

; widening multiply/ multiply accumulate

(define_insn "muluqq<mode>3_32"
  [(set (match_operand:AUACC   0 "pic30_accumulator2_operand" "=w")
        (mult:AUACC
          (fract_convert:AUACC
            (match_operand:UQQ 1 "pic30_mac_input_operand"     "za"))
          (fract_convert:AUACC
            (match_operand:UQQ 2 "pic30_mac_input_operand"     "za"))))
   (use (reg:HI CORCON))
  ]
  ""
  "mpy %1,%2,%0"
)

(define_insn "mulqq<mode>3_32"
  [(set (match_operand:AUACC   0 "pic30_accumulator2_operand" "=w")
        (mult:AUACC
          (fract_convert:AUACC
            (match_operand:QQ  1 "pic30_mac_input_operand"     "za"))
          (fract_convert:AUACC
            (match_operand:QQ  2 "pic30_mac_input_operand"     "za"))))
   (use (reg:HI CORCON))
  ]
  ""
  "mpy %1*%2,%0"
)

(define_insn "umadduqq<mode>4_32"
  [(set (match_operand:AUACC       0 "pic30_accumulator2_operand" "=w")
        (plus:AUACC 
           (match_operand:AUACC    3 "pic30_accumulator2_operand" " 0")
           (mult:AUACC 
              (fract_convert:AUACC
                (match_operand:UQQ 1 "pic30_mac_input_operand"     "za"))
              (fract_convert:AUACC
                (match_operand:UQQ 2 "pic30_mac_input_operand"     "za")))))

   (use (reg:HI CORCON))
  ]
  ""
  "mac %1*%2,%0"
)

(define_insn "maddqq<mode>4_32"
  [(set (match_operand:AUACC      0 "pic30_accumulator2_operand" "=w")
        (plus:AUACC 
           (match_operand:AUACC   3 "pic30_accumulator2_operand" " 0")
           (mult:AUACC 
              (fract_convert:AUACC
                (match_operand:QQ 1 "pic30_mac_input_operand"     "za"))
              (fract_convert:AUACC
                (match_operand:QQ 2 "pic30_mac_input_operand"     "za")))))
   (use (reg:HI CORCON))
  ]
  ""
  "mac %1*%2,%0"
)

(define_insn "umaddhisi4_32"
  [(set (match_operand:SI          0 "pic30_accumulator2_operand" "=w")
        (plus:SI
           (match_operand:SI       3 "pic30_accumulator2_operand" " 0")
           (mult:SI
              (zero_extend:SI
                (match_operand:HI  1 "pic30_mac_input_operand"     "za"))
              (zero_extend:SI
                (match_operand:HI  2 "pic30_mac_input_operand"     "za")))))

   (use (reg:HI CORCON))
  ]
  "pic30_integer_mac_support"
  "mac %1*%2,%0"
)

(define_insn "maddhisi4_32"
  [(set (match_operand:SI         0 "pic30_accumulator2_operand" "=w")
        (plus:SI
           (match_operand:SI      3 "pic30_accumulator2_operand" " 0")
           (mult:SI
              (sign_extend:SI
                (match_operand:HI 1 "pic30_mac_input_operand"     "za"))
              (sign_extend:SI
                (match_operand:HI 2 "pic30_mac_input_operand"     "za")))))
   (use (reg:HI CORCON))
  ]
  "pic30_integer_mac_support"
  "mac %1*%2,%0"
)

(define_insn "umsubuqq<mode>4_32"
  [(set (match_operand:AUACC       0 "pic30_accumulator2_operand" "=w")
        (minus:AUACC 
           (match_operand:AUACC    3 "pic30_accumulator2_operand" " 0")
           (mult:AUACC 
              (fract_convert:AUACC
                (match_operand:UQQ 1 "pic30_mac_input_operand"     "za"))
              (fract_convert:AUACC
                (match_operand:UQQ 2 "pic30_mac_input_operand"     "za")))))

   (use (reg:HI CORCON))
  ]
  ""
  "msc %1*%2,%0"
)

(define_insn "msubqq<mode>4_32"
  [(set (match_operand:AUACC      0 "pic30_accumulator2_operand" "=w")
        (minus:AUACC 
           (match_operand:AUACC   3 "pic30_accumulator2_operand" " 0")
           (mult:AUACC 
              (fract_convert:AUACC
                (match_operand:QQ 1 "pic30_mac_input_operand"     "za"))
              (fract_convert:AUACC
                (match_operand:QQ 2 "pic30_mac_input_operand"     "za")))))
   (use (reg:HI CORCON))
  ]
  ""
  "msc %1*%2,%0"
)

;; negate

(define_insn "neg<mode>2_32"
  [(set (match_operand:ACC   0 "pic30_accum_or_reg_operand" "=w,&r")
        (neg:ACC
          (match_operand:ACC 1 "pic30_accum_or_reg_operand" " 0,r")))
   (use (reg:HI CORCON))]
  ""
  "@
   neg %0
   subr %1,#0,%0\;subbr %d1,#0,%d0\;subbr.b %t1,#0,%t0"
  [
    (set_attr "cc" "math")
  ]
)

(define_insn "ssneg<mode>2_32"
  [(set (match_operand:ACC   0 "pic30_accum_or_reg_operand" "=w,&r")
        (ss_neg:ACC
          (match_operand:ACC 1 "pic30_accum_or_reg_operand" " 0,r")))
   (use (reg:HI CORCON))
  ]
  ""
  "@
   neg %0
   subr %1,#0,%0\;subbr %d1,#0,%d0\;subbr.b %t1,#0,%t0\;bra nov,.L1_%=\;setm %0\;setm %d0\;mov.b #0x7F,%t0\;btst.c %t1,#7\;bra c,.L1_%=\;com %0,%0\;com %d0,%d0\;com.b %t0,%t0\n.L1_%=:"
  [
    (set_attr "cc" "math")
  ]
)
(define_insn "usneg<mode>2_32"
  [(set (match_operand:ACC   0 "pic30_accum_or_reg_operand" "=w,&r")
        (us_neg:ACC
          (match_operand:ACC 1 "pic30_accum_or_reg_operand" " 0,r")))
   (use (reg:HI CORCON))
  ]
  ""
  "@
   neg %0
   subr %1,#0,%0\;subbr %d1,#0,%d0\;subbr.b %t1,#0,%t0\;bra nov,.L1_%=\;setm %0\;setm %d0\;mov.b #0x7F,%t0\n.L1_%=:"
  [
    (set_attr "cc" "math")
  ]
)

;; shift

; sftac can only shift +-16 for some unfathomable reason
; this might be better handled in a library fn

(define_insn "ashl<mode>3_imm15_32"
  [(set (match_operand:AUACC   0 "pic30_accum_or_reg_operand"  "=w,&r")
        (ashift:AUACC
          (match_operand:AUACC 1 "pic30_accum_or_reg_operand"    "0,r")
          (match_operand:HI    2 "immediate_operand"             "i,i")))
   (clobber (match_scratch:HI  3                               "=X,&r"))
   (use (reg:HI CORCON))
  ]
  "((INTVAL(operands[2]) > 0) && (INTVAL(operands[2]) < 16))"
  "@
   sftac %0,#%J2
   sl %t1,#%2,%t0\;lsr %d1,#%k2,%3\;ior.b %3,%t0,%t0\;sl %d1,#%2,%d0\;lsr %1,#%k2,%3\;ior %3,%d0,%d0\;sl %1,#%2,%0"
  [
   (set_attr "cc" "clobber")
  ]
)

(define_insn "ashl<mode>3_imm16to31_32"
  [(set (match_operand:AUACC   0 "pic30_accum_or_reg_operand"  "=w,&r")
        (ashift:AUACC
          (match_operand:AUACC 1 "pic30_accum_or_reg_operand"    "0,r")
          (match_operand:HI    2 "immediate_operand"             "i,i")))
   (clobber (match_scratch:HI  3                               "=X,&r"))
   (use (reg:HI CORCON))
  ]
  "((INTVAL(operands[2]) > 15) && (INTVAL(operands[2]) < 31))"
  "@
   sftac %0,#-16\;sftac %0,#-(%2-16)
   sl %d1,#(%2-16),%t0\;lsr %0,#(32-%2),%3\;ior.b %3,%t0,%t0\;sl %0,#(%2-16),%d0\;clr %0"
  [
   (set_attr "cc" "clobber")
  ]
)

(define_insn "ashl<mode>3_imm32to39_32"
  [(set (match_operand:AUACC   0 "pic30_accum_or_reg_operand"  "=w,&r")
        (ashift:AUACC
          (match_operand:AUACC 1 "pic30_accum_or_reg_operand"    "0,r")
          (match_operand:HI    2 "immediate_operand"             "i,i")))
   (clobber (match_scratch:HI  3                               "=X,&r"))
   (use (reg:HI CORCON))
  ]
  "((INTVAL(operands[2]) > 31) && (INTVAL(operands[2]) < 40))"
  "@
   sftac %0,#-16\;sftac %0,#-16\;sftac %0,-#(%2-32)
   sl %1,#(%2-32),%t0\;mul.uu %0,#0,%0"
  [
   (set_attr "cc" "clobber")
  ]
)

(define_insn "ashl<mode>3_gen_32"
  [(set (match_operand:AUACC   0 "pic30_accum_or_reg_operand"   "=w,&r")
        (ashift:AUACC
          (match_operand:AUACC 1 "pic30_accum_or_reg_operand"    "0,0")
          (match_operand:HI    2 "pic30_register_operand"        "r,r")))
   (clobber (match_scratch:HI  3                                "=2,2"))
   (use (reg:HI CORCON))
  ]
  ""
  "@
   .LB%=:\;dec %2,%2\;bra n,.LE%=\;repeat %2\;sftac %1,#-1\n.LE%=:
   .LB%=:\;dec %2,%2\;bra n,.LE%=\;add %1,%1,%0\;addc %d1,%d1,%d0\;addc.b %t1,%t1,%t0\;bra .LB%=\n.LE%=:"
  [
   (set_attr "cc" "clobber")
  ]
)

(define_expand "ashl<mode>3_32"
  [(set (match_operand:AUACC   0 "pic30_accum_or_reg_operand"   "=w,r")
        (ashift:AUACC
          (match_operand:AUACC 1 "pic30_accum_or_reg_operand"    "w,r")
          (match_operand:HI    2 "pic30_reg_or_imm_operand"      "ri,ri")))
  ]
  ""
  "
  {
    if (immediate_operand(operands[2],VOIDmode)) {
      HOST_WIDE_INT shiftval = INTVAL(operands[2]); 
      if (shiftval < 16) {
        emit(
          gen_ashl<mode>3_imm15_32(operands[0], operands[1], operands[2])
        );
      } else if (shiftval < 32) {
        emit(
          gen_ashl<mode>3_imm16to31_32(operands[0], operands[1], operands[2])
        );
      } else if (shiftval < 39) {
        emit(
          gen_ashl<mode>3_imm32to39_32(operands[0], operands[1], operands[2])
        );
      } else {
        emit(
          gen_mov<mode>_32(operands[0], GEN_INT(0))
        );
      }
      DONE;
    } else {
      emit(
        gen_ashl<mode>3_gen_32(operands[0], operands[1], operands[2])
      );
      DONE;
    }
  }
  "
)

; ss arithmetic shift left

(define_insn "ssashl<mode>3_imm15_32"
  [(set (match_operand:AUACC   0 "pic30_accum_or_reg_operand"  "=w,&r")
        (ss_ashift:AUACC
          (match_operand:AUACC 1 "pic30_accum_or_reg_operand"    "0,r")
          (match_operand:HI    2 "immediate_operand"             "i,i")))
   (clobber (match_scratch:HI  3                               "=X,&r"))
   (use (reg:HI CORCON))
  ]
  "((INTVAL(operands[2]) > 0) && (INTVAL(operands[2]) < 16))"
  "@
   sftac %0,#%J2
   sl %t1,#%2,%t0\;lsr %d1,#%k2,%3\;ior.b %3,%t0,%t0\;sl %d1,#%2,%d0\;lsr %1,#%k2,%3\;ior %3,%d0,%d0\;sl %1,#%2,%0\;xor %t0,%t1,%3\;btss %3,#7\;bra .LE%=\;mov.b #0x7f,%t0\;setm %d0\;setm %0\;btsc %t1,#7\;bset %t0,#7\;.LE%=:"
  [
   (set_attr "cc" "clobber")
  ]
)

(define_insn "ssashl<mode>3_imm16to31_32"
  [(set (match_operand:AUACC   0 "pic30_accum_or_reg_operand"  "=w,&r")
        (ss_ashift:AUACC
          (match_operand:AUACC 1 "pic30_accum_or_reg_operand"    "0,r")
          (match_operand:HI    2 "immediate_operand"             "i,i")))
   (clobber (match_scratch:HI  3                               "=X,&r"))
   (use (reg:HI CORCON))
  ]
  "((INTVAL(operands[2]) > 15) && (INTVAL(operands[2]) < 31))"
  "@
   sftac %0,#-16\;sftac %0,#-(%2-16)
   sl %d1,#(%2-16),%t0\;lsr %0,#(32-%2),%3\;ior.b %3,%t0,%t0\;sl %0,#(%2-16),%d0\;clr %0\;xor %t0,%t1,%3\;btss %3,#7\;bra .LE%=\;mov.b #0x7f,%t0\;setm %d0\;setm %0\;btsc %t1,#7\;bset %t0,#7\;.LE%=:"
  [
   (set_attr "cc" "clobber")
  ]
)

(define_insn "ssashl<mode>3_imm32to39_32"
  [(set (match_operand:AUACC   0 "pic30_accum_or_reg_operand"  "=w,&r")
        (ss_ashift:AUACC
          (match_operand:AUACC 1 "pic30_accum_or_reg_operand"    "0,r")
          (match_operand:HI    2 "immediate_operand"             "i,i")))
   (clobber (match_scratch:HI  3                               "=X,&r"))
   (use (reg:HI CORCON))
  ]
  "((INTVAL(operands[2]) > 31) && (INTVAL(operands[2]) < 40))"
  "@
   sftac %0,#-16\;sftac %0,#-16\;sftac %0,-#(%2-32)
   sl %1,#(%2-32),%t0\;mul.uu %0,#0,%0\;xor %t0,%t1,%3\;btss %3,#7\;bra .LE%=\;mov.b #0x7f,%t0\;setm %d0\;setm %0\;btsc %t1,#7\;bset %t0,#7\;.LE%=:"
  [
   (set_attr "cc" "clobber")
  ]
)

(define_insn "ssashl<mode>3_imm40_32"
  [(set (match_operand:AUACC   0 "pic30_accum_or_reg_operand"  "=w,&r")
        (ss_ashift:AUACC
          (match_operand:AUACC 1 "pic30_accum_or_reg_operand"    "0,r")
          (match_operand:HI    2 "immediate_operand"             "i,i")))
   (clobber (match_scratch:HI  3                               "=X,&r"))
   (use (reg:HI CORCON))
  ]
  "(INTVAL(operands[2]) == 40)"
  "@
   sftac %0,#-16\;sftac %0,#-16\;sftac %0,-#(%2-32)
   mov.b #0x7f,%t0\;setm %d0\;setm %0\;btsc %t1,#7\;bset %t0,#7"
  [
   (set_attr "cc" "clobber")
  ]
)


(define_insn "ssashl<mode>3_gen_32"
  [(set (match_operand:AUACC   0 "pic30_accum_or_reg_operand"   "=w,&r")
        (ss_ashift:AUACC
          (match_operand:AUACC 1 "pic30_accum_or_reg_operand"    "0,r")
          (match_operand:HI    2 "pic30_register_operand"        "r,r")))
   (clobber (match_scratch:HI  3                                "=2,2"))
   (use (reg:HI CORCON))
  ]
  ""
  "@
   .LB%=:\;dec %2,%2\;bra n,.LE%=\;repeat %2\;sftac %1,#-1\n.LE%=:
   mov.d %1,%0\;mov.b %t1,%t0\;.LB%=:\;dec %2,%2\;bra n,.LE%=\;add %0,%0,%0\;addc %d0,%d0,%d0\;addc.b %t0,%t0,%t0\;bra nov,.LB%=\;mov.b #0x7F,%t0\;btsc %t1,#7\;com.b %t0,%t0\;sl %t0,#9,%d0\;asr %d0,#15,%d0\;asr %d0,#15,%0\n.LE%=:"
  [
   (set_attr "cc" "clobber")
  ]
)

(define_expand "ssashl<mode>3_32"
  [(set (match_operand:AUACC   0 "pic30_accum_or_reg_operand"   "=w,r")
        (ss_ashift:AUACC
          (match_operand:AUACC 1 "pic30_accum_or_reg_operand"    "w,r")
          (match_operand:HI    2 "pic30_reg_or_imm_operand"      "ri,ri")))
  ]
  ""
  "
  {
    if (immediate_operand(operands[2],VOIDmode)) {
      HOST_WIDE_INT shiftval = INTVAL(operands[2]); 
      if (shiftval < 16) {
        emit(
          gen_ssashl<mode>3_imm15_32(operands[0], operands[1], operands[2])
        );
      } else if (shiftval < 32) {
        emit(
          gen_ssashl<mode>3_imm16to31_32(operands[0], operands[1], operands[2])
        );
      } else if (shiftval < 39) {
        emit(
          gen_ssashl<mode>3_imm32to39_32(operands[0], operands[1], operands[2])
        );
      } else {
        emit(
          gen_ssashl<mode>3_imm40_32(operands[0], operands[1], GEN_INT(40))
        );
      }
      DONE;
    } else {
      emit(
        gen_ssashl<mode>3_gen_32(operands[0], operands[1], operands[2])
      );
      DONE;
    }
  }
  "
)

; us arithmetic shift left

(define_insn "usashl<mode>3_imm_32"
  [(set (match_operand:AUACC   0 "pic30_accum_or_reg_operand"  "=w,&r")
        (us_ashift:AUACC
          (match_operand:AUACC 1 "pic30_accum_or_reg_operand"    "0,r")
          (match_operand:HI    2 "immediate_operand"             "i,i")))
   (clobber (match_scratch:HI  3                               "=X,&r"))
   (use (reg:HI CORCON))
  ]
  "((INTVAL(operands[2]) > 0) && (INTVAL(operands[2]) <= 16))"
  "@
   sftac %0,#%J2
   sl %t1,#%2,%t0\;lsr %d1,#%k2,%3\;ior.b %3,%t0,%t0\;sl %d1,#%2,%d0\;lsr %1,#%k2,%3\;ior %3,%d0,%d0\;sl %1,#%2,%0\;xor %t0,%t1,%3\;btss %3,#7\;bra .LE%=\;mov.b #0x7f,%t0\;setm %d0\;setm %0\;.LE%=:"

  [
   (set_attr "cc" "clobber")
  ]
)

(define_insn "usashl<mode>3_imm15_32"
  [(set (match_operand:AUACC   0 "pic30_accum_or_reg_operand"  "=w,&r")
        (us_ashift:AUACC
          (match_operand:AUACC 1 "pic30_accum_or_reg_operand"    "0,r")
          (match_operand:HI    2 "immediate_operand"             "i,i")))
   (clobber (match_scratch:HI  3                               "=X,&r"))
   (use (reg:HI CORCON))
  ]
  "((INTVAL(operands[2]) > 0) && (INTVAL(operands[2]) < 16))"
  "@
   sftac %0,#%J2
   sl %t1,#%2,%t0\;lsr %d1,#%k2,%3\;ior.b %3,%t0,%t0\;sl %d1,#%2,%d0\;lsr %1,#%k2,%3\;ior %3,%d0,%d0\;sl %1,#%2,%0\;xor %t0,%t1,%3\;btss %3,#7\;bra .LE%=\;mov.b #0x7f,%t0\;setm %d0\;setm %0\;.LE%=:"
  [
   (set_attr "cc" "clobber")
  ]
)

(define_insn "usashl<mode>3_imm16to31_32"
  [(set (match_operand:AUACC   0 "pic30_accum_or_reg_operand"  "=w,&r")
        (us_ashift:AUACC
          (match_operand:AUACC 1 "pic30_accum_or_reg_operand"    "0,r")
          (match_operand:HI    2 "immediate_operand"             "i,i")))
   (clobber (match_scratch:HI  3                               "=X,&r"))
   (use (reg:HI CORCON))
  ]
  "((INTVAL(operands[2]) > 15) && (INTVAL(operands[2]) < 31))"
  "@
   sftac %0,#-16\;sftac %0,#-(%2-16)
   sl %d1,#(%2-16),%t0\;lsr %0,#(32-%2),%3\;ior.b %3,%t0,%t0\;sl %0,#(%2-16),%d0\;clr %0\;xor %t0,%t1,%3\;btss %3,#7\;bra .LE%=\;mov.b #0x7f,%t0\;setm %d0\;setm %0\;.LE%=:"
  [
   (set_attr "cc" "clobber")
  ]
)

(define_insn "usashl<mode>3_imm32to39_32"
  [(set (match_operand:AUACC   0 "pic30_accum_or_reg_operand"  "=w,&r")
        (us_ashift:AUACC
          (match_operand:AUACC 1 "pic30_accum_or_reg_operand"    "0,r")
          (match_operand:HI    2 "immediate_operand"             "i,i")))
   (clobber (match_scratch:HI  3                               "=X,&r"))
   (use (reg:HI CORCON))
  ]
  "((INTVAL(operands[2]) > 31) && (INTVAL(operands[2]) < 40))"
  "@
   sftac %0,#-16\;sftac %0,#-16\;sftac %0,-#(%2-32)
   sl %1,#(%2-32),%t0\;mul.uu %0,#0,%0\;xor %t0,%t1,%3\;btss %3,#7\;bra .LE%=\;mov.b #0x7f,%t0\;setm %d0\;setm %0\;.LE%=:"
  [
   (set_attr "cc" "clobber")
  ]
)

(define_insn "usashl<mode>3_imm40_32"
  [(set (match_operand:AUACC   0 "pic30_accum_or_reg_operand"  "=w,&r")
        (us_ashift:AUACC
          (match_operand:AUACC 1 "pic30_accum_or_reg_operand"    "0,r")
          (match_operand:HI    2 "immediate_operand"             "i,i")))
   (clobber (match_scratch:HI  3                               "=X,&r"))
   (use (reg:HI CORCON))
  ]
  "(INTVAL(operands[2]) == 40)"
  "@
   sftac %0,#-16\;sftac %0,#-16\;sftac %0,-#(%2-32)
   mov.b #0x7f,%t0\;setm %d0\;setm %0"
  [
   (set_attr "cc" "clobber")
  ]
)

(define_insn "usashl<mode>3_gen_32"
  [(set (match_operand:AUACC   0 "pic30_accum_or_reg_operand"   "=w,&r")
        (us_ashift:AUACC
          (match_operand:AUACC 1 "pic30_accum_or_reg_operand"    "0,r")
          (match_operand:HI    2 "pic30_register_operand"        "r,r")))
   (clobber (match_scratch:HI  3                                "=2,2"))
   (use (reg:HI CORCON))
  ]
  ""
  "@
   dec %2,%2\;bra n,.LE%=\;repeat %2\;sftac %1,#-1\n.LE%=:
   mov.d %1,%0\;mov.b %t1,%t0\;.LB%=:\;dec %2,%2\;bra n,.LE%=\;add %0,%0,%0\;addc %d0,%d0,%d0\;addc.b %t0,%t0,%t0\;bra nov,.LB%=\;mov.b #0x7F,%t0\;setm %d0\;setm %0\n.LE%=:"
  [
   (set_attr "cc" "clobber")
  ]
)

(define_expand "usashl<mode>3_32"
  [(set (match_operand:AUACC   0 "pic30_accum_or_reg_operand"   "=w,r")
        (us_ashift:AUACC
          (match_operand:AUACC 1 "pic30_accum_or_reg_operand"    "w,r")
          (match_operand:HI    2 "pic30_reg_or_imm_operand"      "ri,ri")))
  ]
  ""
  "
  {
    if (immediate_operand(operands[2],VOIDmode)) {
      HOST_WIDE_INT shiftval = INTVAL(operands[2]); 
      if (shiftval < 16) {
        emit(
          gen_usashl<mode>3_imm15_32(operands[0], operands[1], operands[2])
        );
      } else if (shiftval < 32) {
        emit(
          gen_usashl<mode>3_imm16to31_32(operands[0], operands[1], operands[2])
        );
      } else if (shiftval < 39) {
        emit(
          gen_usashl<mode>3_imm32to39_32(operands[0], operands[1], operands[2])
        );
      } else {
        emit(
          gen_usashl<mode>3_imm40_32(operands[0], operands[1], GEN_INT(40))
        );
      }
      DONE;
    } else {
      emit(
        gen_usashl<mode>3_gen_32(operands[0], operands[1], operands[2])
      );
      DONE;
    }
  }
  "
)

; logical shift right 

(define_insn "lshr<mode>3_imm_32"
  [(set (match_operand:AUACC   0 "pic30_accum_or_reg_operand"  "=w,&r")
        (lshiftrt:AUACC
          (match_operand:AUACC 1 "pic30_accum_or_reg_operand"    "0,r")
          (match_operand:HI    2 "immediate_operand"             "i,i")))
   (clobber (match_scratch:HI  3                               "=X,&r"))
   (use (reg:HI CORCON))
  ]
  "((INTVAL(operands[2]) > 0) && (INTVAL(operands[2]) <= 16))"
  "@
   sftac %0,#%2
   lsr %1,#%2,%0\;sl %d1,#%k2,%3\;ior %3,%0,%0\;lsr %d1,#%2,%d0\;sl %t1,#%k2,%3\;ior %3,%d0,%d0\;mov %t1,%t0\;and #0xFF,%t0\;lsr %t0,#%2,%t0"
  [
   (set_attr "cc" "clobber")
  ]
)

(define_insn "lshr<mode>3_imm15_32"
  [(set (match_operand:AUACC   0 "pic30_accum_or_reg_operand"  "=w,&r")
        (lshiftrt:AUACC
          (match_operand:AUACC 1 "pic30_accum_or_reg_operand"    "0,r")
          (match_operand:HI    2 "immediate_operand"             "i,i")))
   (clobber (match_scratch:HI  3                               "=X,&r"))
   (use (reg:HI CORCON))
  ]
  "((INTVAL(operands[2]) > 0) && (INTVAL(operands[2]) <= 15))"
  "@
   sftac %0,#%2
   lsr %1,#%2,%0\;sl %d1,#%k2,%3\;ior %3,%0,%0\;lsr %d1,#%2,%d0\;sl %t1,#%k2,%3\;ior %3,%d0,%d0\;mov %t1,%t0\;and #0xFF,%t0\;lsr %t0,#%2,%t0"
  [
   (set_attr "cc" "clobber")
  ]
)

(define_insn "lshr<mode>3_imm16to31_32"
  [(set (match_operand:AUACC   0 "pic30_accum_or_reg_operand"  "=w,&r")
        (lshiftrt:AUACC
          (match_operand:AUACC 1 "pic30_accum_or_reg_operand"    "0,r")
          (match_operand:HI    2 "immediate_operand"             "i,i")))
   (clobber (match_scratch:HI  3                               "=X,&r"))
   (use (reg:HI CORCON))
  ]
  "((INTVAL(operands[2]) > 15) && (INTVAL(operands[2]) <= 31))"
  "@
   sftac %0,#16\;sftac %0,#(%2-16)
   lsr %d1,#(%2-16),%0\;sl %t1,#(32-%2),%3\;ior %3,%0,%0\;lsr %t1,#(%2-16),%d0\;\;and #0x7F,%d0\;clr.b %t0"
  [
   (set_attr "cc" "clobber")
  ]
)

(define_insn "lshr<mode>3_imm32to39_32"
  [(set (match_operand:AUACC   0 "pic30_accum_or_reg_operand"  "=w,&r")
        (lshiftrt:AUACC
          (match_operand:AUACC 1 "pic30_accum_or_reg_operand"    "0,r")
          (match_operand:HI    2 "immediate_operand"             "i,i")))
   (clobber (match_scratch:HI  3                               "=X,&r"))
   (use (reg:HI CORCON))
  ]
  "((INTVAL(operands[2]) > 31) && (INTVAL(operands[2]) <= 39))"
  "@
   sftac %0,#16\;sftac %0,#16\;sftac %0,#(%2-32)
   lsr %t1,#(%2-32),%0\;and #0x7f,%0\;clr %d0\;clr.b %t0"
  [
   (set_attr "cc" "clobber")
  ]
)

(define_insn "lshr<mode>3_gen_32"
  [(set (match_operand:AUACC   0 "pic30_accum_or_reg_operand"   "=w,&r")
        (lshiftrt:AUACC
          (match_operand:AUACC 1 "pic30_accum_or_reg_operand"    "0,0")
          (match_operand:HI    2 "pic30_register_operand"        "r,r")))
   (clobber (match_scratch:HI  3                                "=2,2"))
   (use (reg:HI CORCON))
  ]
  ""
  "@
   .LB%=:\;dec %2,%2\;bra n,.LE%=\;sftac %1,#1\;bra .LB%=\n.LE%=:
   .LB%=:\;dec %2,%2\;bra n,.LE%=\;lsr.b %t1,%t0\;rrc %d1,%d0\;rrc %1,%0\;bra .LB%=\n.LE%=:"
  [
   (set_attr "cc" "clobber")
  ]
)

(define_expand "lshr<mode>3_32"
  [(set (match_operand:AUACC   0 "pic30_accum_or_reg_operand"   "=w,r")
        (lshiftrt:AUACC
          (match_operand:AUACC 1 "pic30_accum_or_reg_operand"    "w,r")
          (match_operand:HI    2 "pic30_reg_or_imm_operand"      "ri,ri")))
  ]
  ""
  "
  {
    if (pic30_fp_round_p() == pic30_conventional) FAIL;
    if (pic30_fp_round_p() == pic30_convergent) FAIL;
    /* truncation or fastest */
    if (immediate_operand(operands[2],VOIDmode)) {
      HOST_WIDE_INT shiftval = INTVAL(operands[2]); 
      if (shiftval < 16) {
        emit(
          gen_lshr<mode>3_imm15_32(operands[0], operands[1], operands[2])
        );
      } else if (shiftval < 32) {
        emit(
          gen_lshr<mode>3_imm16to31_32(operands[0], operands[1], operands[2])
        );
      } else if (shiftval < 39) {
        emit(
          gen_lshr<mode>3_imm32to39_32(operands[0], operands[1], operands[2])
        );
      } else {
        emit(
          gen_mov<mode>_32(operands[0], GEN_INT(0))
        );
      }
      DONE;
    } else {
      emit(
        gen_lshr<mode>3_gen_32(operands[0], operands[1], operands[2])
      );
      DONE;
    }
  }
  "
)

; arithemetic shift right

(define_insn "ashr<mode>3_gen_32"
  [(set (match_operand:AUACC   0 "pic30_accum_or_reg_operand"   "=w,&r")
        (ashiftrt:AUACC
          (match_operand:AUACC 1 "pic30_accum_or_reg_operand"    "0,0")
          (match_operand:HI    2 "pic30_register_operand"        "r,r")))
   (clobber (match_scratch:HI  3                                "=2,2"))
   (use (reg:HI CORCON))
  ]
  ""
  "@
   dec %2,%2\;repeat %2\;sftac %1,#1
   .LB%=:\;dec %2,%2\;bra n,.LE%=\;asr.b %t1,%t0\;rrc %d1,%d0\;rrc %1,%0\;bra .LB%=\n.LE%=:"
  [
   (set_attr "cc" "clobber")
  ]
)

(define_expand "ashr<mode>3_32"
  [(set (match_operand:AUACC   0 "pic30_accum_or_reg_operand"   "=w,r")
        (ashiftrt:AUACC
          (match_operand:AUACC 1 "pic30_accum_or_reg_operand"    "w,r")
          (match_operand:HI    2 "pic30_register_operand"        "r,r")))
  ]
  ""
  "
  {
    if (pic30_fp_round_p() == pic30_conventional) {
      FAIL;
    } else if (pic30_fp_round_p() == pic30_convergent) {
      FAIL;
    } else {
      emit(
        gen_ashr<mode>3_gen_32(operands[0], operands[1], operands[2])
      );
      DONE;
    } 
    FAIL;
  }
  "
)

;; compare

(define_insn "cmp<mode>_helper_32"
  [(set (reg:CC CC_REG)
        (compare 
          (match_operand:AUACC 0 "pic30_accum_or_reg_operand" "w,r")
          (match_operand:AUACC 1 "pic30_accum_or_reg_operand" "w,r")))
   (clobber (match_scratch:HI  2                             "=&a,X"))
  ]
  "" 
  "@
   mov %m1L,w0\;cp %m0L\;mov %m1H,w0\;cpb %m0H\;mov %m1U,w0\;cpb.b %m0U
   cp %0,%1\;cpb %d0,%d1\;cpb.b %t0,%t1"
  [
    (set_attr "cc" "set,set")
  ]
)

(define_expand "cmp<mode>_32"
  [(set (reg:CC CC_REG)
        (compare 
          (match_operand:AUACC 0 "" "w,r")
          (match_operand:AUACC 1 "" "w,r")))
   (clobber (match_scratch:HI  2  "=&a,X"))
  ]
  "" 
  "
  {
    rtx op0, op1;
    op0 = operands[0];
    op1 = operands[1];

    if (!pic30_accum_or_reg_operand(op0, <MODE>mode)) {
      op0 = force_reg(<MODE>mode,op0);
    }
    if (!pic30_accum_or_reg_operand(op1, <MODE>mode)) {
      op1 = force_reg(<MODE>mode,op1);
    }
    emit(
      gen_cmp<mode>_helper_32(op0,op1)
    );
    DONE;
  }"
)

;; Accum / fract conversions
;  to Q15

(define_insn "fract<mode>qq2_trunc_32"
  [(set (match_operand:QQ      0 "pic30_register_operand"     "=r,r")
        (fract_convert:QQ
          (match_operand:AUACC 1 "pic30_accum_or_reg_operand" " w,r")))]
  ""
  "@
   sac %1,%0
   mov %d1,%0"
)
  
(define_insn "fract<mode>uqq2_trunc_32"
  [(set (match_operand:UQQ      0 "pic30_register_operand"     "=r,r")
        (fract_convert:UQQ
          (match_operand:AUACC 1 "pic30_accum_or_reg_operand" " w,r")))]
  ""
  "@
   sac %1,%0
   mov %d1,%0"
)
  
(define_insn "fract<mode>hq2_trunc_32"
  [(set (match_operand:HQ      0 "pic30_register_operand"     "=r,r")
        (fract_convert:HQ
          (match_operand:AUACC 1 "pic30_accum_or_reg_operand" " w,r")))]
  ""
  "@
   sac %1,%0
   mov %d1,%0"
)
  
(define_insn "fract<mode>uhq2_trunc_32"
  [(set (match_operand:UHQ      0 "pic30_register_operand"     "=r,r")
        (fract_convert:UHQ
          (match_operand:AUACC 1 "pic30_accum_or_reg_operand" " w,r")))]
  ""
  "@
   sac %1,%0
   mov %d1,%0"
)

(define_insn "fract<mode>qq2_conv_32"
  [(set (match_operand:QQ      0 "pic30_register_operand"    "=r, &r")
        (fract_convert:QQ
          (match_operand:AUACC 1 "pic30_accum_or_reg_operand" "w, r")))
   (clobber (match_scratch:HI  2                             "=X,&r"))
   (use (reg:HI CORCON))
  ]
  ""
  "*
   if (pic30_fp_round_p() == pic30_conventional) {
     switch (which_alternative) {
       case 0: return \"sac.r %1,%0\";
       case 1: return \"mov %d1,%0\;btsc %1,#15\;inc %0,%0\";
       default: gcc_assert(0);
     }
   } else if (pic30_fp_round_p() == pic30_convergent) {
     switch (which_alternative) {
       case 0: return \"sac.r %1,%0\";
       case 1: return \"mov %d1,%0\;sl %1,%2\;bra nz,.LINC%=\;btsc %0,#0\n.LINC%=:addc %0,#0,%0\";
       default: gcc_assert(0);
     }
   }
   error(\"Cannot generate instruction\");
   return \"cannot generate instruction\";
  "
)

(define_insn "fract<mode>uqq2_conv_32"
  [(set (match_operand:UQQ     0 "pic30_register_operand"     "=r,&r")
        (fract_convert:UQQ
          (match_operand:AUACC 1 "pic30_accum_or_reg_operand" " w, r")))
   (clobber (match_scratch:HI  2                              "=X,&r"))
   (use (reg:HI CORCON))
  ]
  ""
  "*
   if (pic30_fp_round_p() == pic30_conventional) {
     switch (which_alternative) {
       case 0: return \"sac.r %1,%0\";
       case 1: return \"mov %d1,%0\;btsc %1,#15\;inc %0,%0\";
       default: gcc_assert(0);
     }
   } else if (pic30_fp_round_p() == pic30_convergent) {
     switch (which_alternative) {
       case 0: return \"sac.r %1,%0\";
       case 1: return \"mov %d1,%0\;sl %1,%2\;bra nz,.LINC%=\;btsc %0,#0\n.LINC%=:addc %0,#0,%0\";
       default: gcc_assert(0);
     }
   }
   error(\"Cannot generate instruction\");
   return \"cannot generate instruction\";
  "
)

(define_insn "fract<mode>hq2_conv_32"
  [(set (match_operand:HQ      0 "pic30_register_operand"     "=r,&r")
        (fract_convert:HQ
          (match_operand:AUACC 1 "pic30_accum_or_reg_operand" " w, r")))
   (clobber (match_scratch:HI  2                              "=X,&r"))
   (use (reg:HI CORCON))
  ]
  ""
  "*
   if (pic30_fp_round_p() == pic30_conventional) {
     switch (which_alternative) {
       case 0: return \"sac.r %1,%0\";
       case 1: return \"mov %d1,%0\;btsc %1,#15\;inc %0,%0\";
       default: gcc_assert(0);
     }
   } else if (pic30_fp_round_p() == pic30_convergent) {
     switch (which_alternative) {
       case 0: return \"sac.r %1,%0\";
       case 1: return \"mov %d1,%0\;sl %1,%2\;bra nz,.LINC%=\;btsc %0,#0\n.LINC%=:addc %0,#0,%0\";
       default: gcc_assert(0);
     }
   }
   error(\"Cannot generate instruction\");
   return \"cannot generate instruction\";
  "
)

(define_insn "fract<mode>uhq2_conv_32"
  [(set (match_operand:UHQ     0 "pic30_register_operand"    "=r,&r")
        (fract_convert:UHQ
          (match_operand:AUACC 1 "pic30_accum_or_reg_operand" "w, r")))
   (clobber (match_scratch:HI  2                             "=X,&r"))
   (use (reg:HI CORCON))
  ]
  ""
  "*
   if (pic30_fp_round_p() == pic30_conventional) {
     switch (which_alternative) {
       case 0: return \"sac.r %1,%0\";
       case 1: return \"mov %d1,%0;btsc %1,#15\;inc %0,%0\";
       default: gcc_assert(0);
     }
   } else if (pic30_fp_round_p() == pic30_convergent) {
     switch (which_alternative) {
       case 0: return \"sac.r %1,%0\";
       case 1: return \"mov %d1,%0\;sl %1,%2\;bra nz,.LINC%=\;btsc %0,#0\n.LINC%=:addc %0,#0,%0\";
       default: gcc_assert(0);
     }
   }
   error(\"Cannot generate instruction\");
   return \"cannot generate instruction\";
  "
)

(define_expand "fract<mode>qq2_32"
  [(set (match_operand:QQ    0 "pic30_register_operand"    "=r,r")
      (fract_convert:QQ
        (match_operand:AUACC 1 "pic30_accum_or_reg_operand" "w,r")))
  ]
  ""
  "
  {
    switch (pic30_fp_round_p()) {
      default: gcc_assert(0);

      case pic30_truncation:
      case pic30_fastest:
        emit(
          gen_fract<mode>qq2_trunc_32(operands[0],operands[1])
        );
        DONE;
        break;
      case pic30_convergent:
      case pic30_conventional:
        emit(
          gen_fract<mode>qq2_conv_32(operands[0],operands[1])
        );
        DONE;
        break;
    }
    FAIL;
  }
  "
)   

(define_expand "fract<mode>uqq2_32"
  [(set (match_operand:UQQ   0 "pic30_register_operand"    "=r,r")
      (fract_convert:UQQ
        (match_operand:AUACC 1 "pic30_accum_or_reg_operand" "w,r")))
  ]
  ""
  "
  {
    switch (pic30_fp_round_p()) {
      default: gcc_assert(0);

      case pic30_truncation:
      case pic30_fastest:
        emit(
          gen_fract<mode>uqq2_trunc_32(operands[0],operands[1])
        );
        DONE;
        break;
      case pic30_convergent:
      case pic30_conventional:
        emit(
          gen_fract<mode>uqq2_conv_32(operands[0],operands[1])
        );
        DONE;
        break;
    }
    FAIL;
  }
  "
)   

(define_expand "fract<mode>hq2_32"
  [(set (match_operand:HQ    0 "pic30_register_operand"    "=r,r")
      (fract_convert:QQ
        (match_operand:AUACC 1 "pic30_accum_or_reg_operand" "w,r")))
  ]
  ""
  "
  {
    switch (pic30_fp_round_p()) {
      default: gcc_assert(0);

      case pic30_truncation:
      case pic30_fastest:
        emit(
          gen_fract<mode>hq2_trunc_32(operands[0],operands[1])
        );
        DONE;
        break;
      case pic30_convergent:
      case pic30_conventional:
        emit(
          gen_fract<mode>hq2_conv_32(operands[0],operands[1])
        );
        DONE;
        break;
    }
    FAIL;
  }
  "
)   

(define_expand "fract<mode>uhq2_32"
  [(set (match_operand:UHQ   0 "pic30_register_operand"    "=r,r")
      (fract_convert:UHQ
        (match_operand:AUACC 1 "pic30_accum_or_reg_operand" "w,r")))
  ]
  ""
  "
  {
    switch (pic30_fp_round_p()) {
      default: gcc_assert(0);

      case pic30_truncation:
      case pic30_fastest:
        emit(
          gen_fract<mode>uhq2_trunc_32(operands[0],operands[1])
        );
        DONE;
        break;
      case pic30_convergent:
      case pic30_conventional:
        emit(
          gen_fract<mode>uhq2_conv_32(operands[0],operands[1])
        );
        DONE;
        break;
    }
    FAIL;
  }
  "
)   

; to Q31

(define_insn "fract<mode>sq2_32"
  [(set (match_operand:SQ      0 "pic30_register_operand"     "=r,r")
        (fract_convert:SQ
          (match_operand:AUACC 1 "pic30_accum_or_reg_operand" " w,r")))]
  ""
  "@
   mov %m1L,%0\;mov %m1H,%d0
   mov.d %1,%0"
)
 
(define_insn "fract<mode>usq2_32"
  [(set (match_operand:USQ      0 "pic30_register_operand"     "=r,r")
        (fract_convert:USQ
          (match_operand:AUACC 1 "pic30_accum_or_reg_operand" " w,r")))]
  ""
  "@
   mov %m1L,%0\;mov %m1H,%d0
   mov.d %1,%0"
)

(define_insn "fract<mode>dq2_32"
  [(set (match_operand:DQ      0 "pic30_register_operand"     "=r,r")
        (fract_convert:DQ
          (match_operand:AUACC 1 "pic30_accum_or_reg_operand" " w,r")))]
  ""
  "@
   mov %m1L,%0\;mov %m1H,%d0
   mov.d %1,%0"
)
 
(define_insn "fract<mode>udq2_32"
  [(set (match_operand:UDQ      0 "pic30_register_operand"     "=r,r")
        (fract_convert:UDQ
          (match_operand:AUACC 1 "pic30_accum_or_reg_operand" " w,r")))]
  ""
  "@
   mov %m1L,%0\;mov %m1H,%d0
   mov.d %1,%0"
)

(define_insn "fract<mode>tq2_32"
  [(set (match_operand:TQ      0 "pic30_register_operand"     "=r,r")
        (fract_convert:TQ
          (match_operand:AUACC 1 "pic30_accum_or_reg_operand" " w,r")))]
  ""
  "@
   mov %m1L,%0\;mov %m1H,%d0
   mov.d %1,%0"
)
 
(define_insn "fract<mode>utq2_32"
  [(set (match_operand:UTQ      0 "pic30_register_operand"     "=r,r")
        (fract_convert:UTQ
          (match_operand:AUACC 1 "pic30_accum_or_reg_operand" " w,r")))]
  ""
  "@
   mov %m1L,%0\;mov %m1H,%d0
   mov.d %1,%0"
)

;  saturating to Q15

(define_insn "satfract<mode>qq2_trunc_32"
  [(set (match_operand:QQ      0 "pic30_register_operand"     "=r,&r")
        (sat_fract:QQ
          (match_operand:AUACC 1 "pic30_accum_or_reg_operand" " w,r")))
   (clobber (match_scratch:HI  2                             "=X,&r"))
  ]
  ""
  "@
   sac %1,%0
   mov %d1,%0\;se %t1,%2\;rlc %d1,[w15]\;rlc %2,%2\;bra z,.LOK%=\;inc %2,%2\;bra z,.LOK%=\;mov #0x7FFF,%0\;btsc %t1,#7\;com %0,%0\;.LOK%=:\n"
)
  
(define_insn "satfract<mode>uqq2_trunc_32"
  [(set (match_operand:UQQ      0 "pic30_register_operand"     "=r,&r")
        (sat_fract:UQQ
          (match_operand:AUACC 1 "pic30_accum_or_reg_operand" " w,r")))
   (clobber (match_scratch:HI  2                             "=X,&r"))
  ]
  ""
  "@
   sac %1,%0
   mov %d1,%0\;se %t1,%2\;rlc %d1,[w15]\;rlc %2,%2\;bra z,.LOK%=\;inc %2,%2\;bra z,.LOK%=\;mov #0x7FFF,%0\;btsc %t1,#7\;clr %0\;.LOK%=:\n"
)
  
(define_insn "satfract<mode>hq2_trunc_32"
  [(set (match_operand:HQ      0 "pic30_register_operand"     "=r,&r")
        (sat_fract:HQ
          (match_operand:AUACC 1 "pic30_accum_or_reg_operand" " w,r")))
   (clobber (match_scratch:HI  2                             "=X,&r"))
  ]
  ""
  "@
   sac %1,%0
   mov %d1,%0\;se %t1,%2\;rlc %d1,[w15]\;rlc %2,%2\;bra z,.LOK%=\;inc %2,%2\;bra z,.LOK%=\;mov #0x7FFF,%0\;btsc %t1,#7\;com %0,%0\;.LOK%=:\n"
)
  
(define_insn "satfract<mode>uhq2_trunc_32"
  [(set (match_operand:UHQ      0 "pic30_register_operand"     "=r,&r")
        (sat_fract:UHQ
          (match_operand:AUACC 1 "pic30_accum_or_reg_operand" " w,r")))
   (clobber (match_scratch:HI  2                             "=X,&r"))
  ]
  ""
  "@
   sac %1,%0
   mov %d1,%0\;se %t1,%2\;rlc %d1,[w15]\;rlc %2,%2\;bra z,.LOK%=\;inc %2,%2\;bra z,.LOK%=\;mov #0x7FFF,%0\;btsc %t1,#7\;clr %0\;.LOK%=:\n"
)

(define_insn "satfract<mode>qq2_conv_32"
  [(set (match_operand:QQ      0 "pic30_register_operand"     "=r,&r")
        (sat_fract:QQ
          (match_operand:AUACC 1 "pic30_accum_or_reg_operand" " w,r")))
   (clobber (match_scratch:HI  2                             "=X,&r"))
   (use (reg:HI CORCON))
  ]
  ""
  "*
   if (pic30_fp_round_p() == pic30_conventional) {
     switch (which_alternative) {
       default: gcc_assert(0);
       case 0: return \"sac.r %1,%0\";
                      /* Like truncating, but ... */
       case 1: return \"mov %d1,%0\;\"            /* copy result */
                      \"sl %1,[w15]\;\"           /* check for round */
                      \"addc %0,#0,%0\;\"
                      \"bra ov,.LOV%=\;\"         /* if 0x7FFF -> 0x8000, OV */
                      \"se %t1,%2\;\"             /* otherwise check for */
                      \"rlc %d1,[w15]\;\"         /*   out of bounds input */
                      \"rlc %2,%2\;\"
                      \"bra z,.LOK%=\;\"
                      \"inc %2,%2\;\"
                      \"bra z,.LOK%=\n\"
                      \".LOV%=:\"
                      \"mov #0x7FFF,%0\;\"
                      \"btsc %t1,#7\;\"
                      \"com %0,%0\n\"
                      \".LOK%=:\n\";
     }
   } else if (pic30_fp_round_p() == pic30_convergent) {
     switch (which_alternative) {
       default: gcc_assert(0);
       case 0: return \"sac.r %1,%0\";
       case 1: return \"mov %d1,%0\;\"            /* copy result */
                      \"sl %1,[w15]\;\"           /* check for round */
                      \"bra nz,.LINC%=\;\"
                      \"btsc %0,#0\n\"
                      \".LINC%=:\"
                      \"addc %0,#0,%0\;\"
                      \"bra ov,.LOV%=\;\"         /* if 0x7FFF -> 0x8000, OV */
                      \"se %t1,%2\;\"             /* otherwise check for */
                      \"rlc %d1,[w15]\;\"         /*   out of bounds input */
                      \"rlc %2,%2\;\"
                      \"bra z,.LOK%=\;\"
                      \"inc %2,%2\;\"
                      \"bra z,.LOK%=\n\"
                      \".LOV%=:\"
                      \"mov #0x7FFF,%0\;\"
                      \"btsc %t1,#7\;\"
                      \"com %0,%0\n\"
                      \".LOK%=:\n\";
     }
   }
   error(\"Invalid fixed-point round mode specified\n\");
   return \"cannot generate instruction\";
   "
)
 
(define_insn "satfract<mode>uqq2_conv_32"
  [(set (match_operand:UQQ      0 "pic30_register_operand"     "=r,&r")
        (sat_fract:UQQ
          (match_operand:AUACC 1 "pic30_accum_or_reg_operand" " w,r")))
   (clobber (match_scratch:HI  2                             "=X,&r"))
   (use (reg:HI CORCON))
  ]
  ""
  "*
   if (pic30_fp_round_p() == pic30_conventional) {
     switch (which_alternative) {
       default: gcc_assert(0);
       case 0: return \"sac.r %1,%0\";
                      /* Like truncating, but ... */
       case 1: return \"mov %d1,%0\;\"            /* copy result */
                      \"sl %1,[w15]\;\"           /* check for round */
                      \"addc %0,#0,%0\;\"
                      \"bra ov,.LOV%=\;\"         /* if 0x7FFF -> 0x8000, OV */
                      \"se %t1,%2\;\"             /* otherwise check for */
                      \"rlc %d1,[w15]\;\"         /*   out of bounds input */
                      \"rlc %2,%2\;\"
                      \"bra z,.LOK%=\;\"
                      \"inc %2,%2\;\"
                      \"bra z,.LOK%=\n\"
                      \".LOV%=:\"
                      \"mov #0x7FFF,%0\;\"
                      \"btsc %t1,#7\;\"
                      \"clr %0\n\"
                      \".LOK%=:\n\";
     }
   } else if (pic30_fp_round_p() == pic30_convergent) {
     switch (which_alternative) {
       default: gcc_assert(0);
       case 0: return \"sac.r %1,%0\";
       case 1: return \"mov %d1,%0\;\"            /* copy result */
                      \"sl %1,[w15]\;\"           /* check for round */
                      \"bra nz,.LINC%=\;\"
                      \"btsc %0,#0\n\"
                      \".LINC%=:\"
                      \"addc %0,#0,%0\;\"
                      \"bra ov,.LOV%=\;\"         /* if 0x7FFF -> 0x8000, OV */
                      \"se %t1,%2\;\"             /* otherwise check for */
                      \"rlc %d1,[w15]\;\"         /*   out of bounds input */
                      \"rlc %2,%2\;\"
                      \"bra z,.LOK%=\;\"
                      \"inc %2,%2\;\"
                      \"bra z,.LOK%=\n\"
                      \".LOV%=:\"
                      \"mov #0x7FFF,%0\;\"
                      \"btsc %t1,#7\;\"
                      \"clr %0\n\"
                      \".LOK%=:\n\";
     }
   }
   error(\"Invalid fixed-point round mode specified\n\");
   return \"cannot generate instruction\";
   "
)
 
(define_insn "satfract<mode>hq2_conv_32"
  [(set (match_operand:HQ      0 "pic30_register_operand"     "=r,&r")
        (sat_fract:HQ
          (match_operand:AUACC 1 "pic30_accum_or_reg_operand" " w,r")))
   (clobber (match_scratch:HI  2                             "=X,&r"))
   (use (reg:HI CORCON))
  ]
  ""
  "*
   if (pic30_fp_round_p() == pic30_conventional) {
     switch (which_alternative) {
       default: gcc_assert(0);
       case 0: return \"sac.r %1,%0\";
                      /* Like truncating, but ... */
       case 1: return \"mov %d1,%0\;\"            /* copy result */
                      \"sl %1,[w15]\;\"           /* check for round */
                      \"addc %0,#0,%0\;\"
                      \"bra ov,.LOV%=\;\"         /* if 0x7FFF -> 0x8000, OV */
                      \"se %t1,%2\;\"             /* otherwise check for */
                      \"rlc %d1,[w15]\;\"         /*   out of bounds input */
                      \"rlc %2,%2\;\"
                      \"bra z,.LOK%=\;\"
                      \"inc %2,%2\;\"
                      \"bra z,.LOK%=\n\"
                      \".LOV%=:\"
                      \"mov #0x7FFF,%0\;\"
                      \"btsc %t1,#7\;\"
                      \"com %0,%0\n\"
                      \".LOK%=:\n\";
     }
   } else if (pic30_fp_round_p() == pic30_convergent) {
     switch (which_alternative) {
       default: gcc_assert(0);
       case 0: return \"sac.r %1,%0\";
       case 1: return \"mov %d1,%0\;\"            /* copy result */
                      \"sl %1,[w15]\;\"           /* check for round */
                      \"bra nz,.LINC%=\;\"
                      \"btsc %0,#0\n\"
                      \".LINC%=:\"
                      \"addc %0,#0,%0\;\"
                      \"bra ov,.LOV%=\;\"         /* if 0x7FFF -> 0x8000, OV */
                      \"se %t1,%2\;\"             /* otherwise check for */
                      \"rlc %d1,[w15]\;\"         /*   out of bounds input */
                      \"rlc %2,%2\;\"
                      \"bra z,.LOK%=\;\"
                      \"inc %2,%2\;\"
                      \"bra z,.LOK%=\n\"
                      \".LOV%=:\"
                      \"mov #0x7FFF,%0\;\"
                      \"btsc %t1,#7\;\"
                      \"com %0,%0\n\"
                      \".LOK%=:\n\";
     }
   }
   error(\"Invalid fixed-point round mode specified\n\");
   return \"cannot generate instruction\";
  "
)
 
(define_insn "satfract<mode>uhq2_conv_32"
  [(set (match_operand:UHQ      0 "pic30_register_operand"     "=r,&r")
        (sat_fract:UHQ
          (match_operand:AUACC 1 "pic30_accum_or_reg_operand" " w,r")))
   (clobber (match_scratch:HI  2                             "=X,&r"))
   (use (reg:HI CORCON))
  ]
  ""
  "*
   if (pic30_fp_round_p() == pic30_conventional) {
     switch (which_alternative) {
       default: gcc_assert(0);
       case 0: return \"sac.r %1,%0\";
                      /* Like truncating, but ... */
       case 1: return \"mov %d1,%0\;\"            /* copy result */
                      \"sl %1,[w15]\;\"           /* check for round */
                      \"addc %0,#0,%0\;\"
                      \"bra ov,.LOV%=\;\"         /* if 0x7FFF -> 0x8000, OV */
                      \"se %t1,%2\;\"             /* otherwise check for */
                      \"rlc %d1,[w15]\;\"         /*   out of bounds input */
                      \"rlc %2,%2\;\"
                      \"bra z,.LOK%=\;\"
                      \"inc %2,%2\;\"
                      \"bra z,.LOK%=\n\"
                      \".LOV%=:\"
                      \"mov #0x7FFF,%0\;\"
                      \"btsc %t1,#7\;\"
                      \"clr %0\n\"
                      \".LOK%=:\n\";
     }
   } else if (pic30_fp_round_p() == pic30_convergent) {
     switch (which_alternative) {
       default: gcc_assert(0);
       case 0: return \"sac.r %1,%0\";
       case 1: return \"mov %d1,%0\;\"            /* copy result */
                      \"sl %1,[w15]\;\"           /* check for round */
                      \"bra nz,.LINC%=\;\"
                      \"btsc %0,#0\n\"
                      \".LINC%=:\"
                      \"addc %0,#0,%0\;\"
                      \"bra ov,.LOV%=\;\"         /* if 0x7FFF -> 0x8000, OV */
                      \"se %t1,%2\;\"             /* otherwise check for */
                      \"rlc %d1,[w15]\;\"         /*   out of bounds input */
                      \"rlc %2,%2\;\"
                      \"bra z,.LOK%=\;\"
                      \"inc %2,%2\;\"
                      \"bra z,.LOK%=\n\"
                      \".LOV%=:\"
                      \"mov #0x7FFF,%0\;\"
                      \"btsc %t1,#7\;\"
                      \"clr %0\n\"
                      \".LOK%=:\n\";
     }
   }
   error(\"Invalid fixed-point round mode specified\n\");
   return \"cannot generate instruction\";
  "
)


(define_expand "satfract<mode>qq2_32"
  [(set (match_operand:QQ    0 "pic30_register_operand"    "=r,r")
      (sat_fract:QQ
        (match_operand:AUACC 1 "pic30_accum_or_reg_operand" "w,r")))
  ]
  ""
  "
  {
    switch (pic30_fp_round_p()) {
      default: gcc_assert(0);

      case pic30_truncation:
      case pic30_fastest:
        emit(
          gen_satfract<mode>qq2_trunc_32(operands[0],operands[1])
        );
        DONE;
        break;
      case pic30_convergent:
      case pic30_conventional:
        emit(
          gen_satfract<mode>qq2_conv_32(operands[0],operands[1])
        );
        DONE;
        break;
    }
    FAIL;
  }
  "
)   

(define_expand "satfract<mode>uqq2_32"
  [(set (match_operand:UQQ   0 "pic30_register_operand"    "=r,r")
      (sat_fract:UQQ
        (match_operand:AUACC 1 "pic30_accum_or_reg_operand" "w,r")))
  ]
  ""
  "
  {
    switch (pic30_fp_round_p()) {
      default: gcc_assert(0);

      case pic30_truncation:
      case pic30_fastest:
        emit(
          gen_satfract<mode>uqq2_trunc_32(operands[0],operands[1])
        );
        DONE;
        break;
      case pic30_convergent:
      case pic30_conventional:
        emit(
          gen_satfract<mode>uqq2_conv_32(operands[0],operands[1])
        );
        DONE;
        break;
    }
    FAIL;
  }
  "
)   

(define_expand "satfract<mode>hq2_32"
  [(set (match_operand:HQ    0 "pic30_register_operand"    "=r,r")
      (sat_fract:QQ
        (match_operand:AUACC 1 "pic30_accum_or_reg_operand" "w,r")))
  ]
  ""
  "
  {
    switch (pic30_fp_round_p()) {
      default: gcc_assert(0);

      case pic30_truncation:
      case pic30_fastest:
        emit(
          gen_satfract<mode>hq2_trunc_32(operands[0],operands[1])
        );
        DONE;
        break;
      case pic30_convergent:
      case pic30_conventional:
        emit(
          gen_satfract<mode>hq2_conv_32(operands[0],operands[1])
        );
        DONE;
        break;
    }
    FAIL;
  }
  "
)   

(define_expand "satfract<mode>uhq2_32"
  [(set (match_operand:UHQ   0 "pic30_register_operand"    "=r,r")
      (sat_fract:UHQ
        (match_operand:AUACC 1 "pic30_accum_or_reg_operand" "w,r")))
  ]
  ""
  "
  {
    switch (pic30_fp_round_p()) {
      default: gcc_assert(0);

      case pic30_truncation:
      case pic30_fastest:
        emit(
          gen_satfract<mode>uhq2_trunc_32(operands[0],operands[1])
        );
        DONE;
        break;
      case pic30_convergent:
      case pic30_conventional:
        emit(
          gen_satfract<mode>uhq2_conv_32(operands[0],operands[1])
        );
        DONE;
        break;
    }
    FAIL;
  }
  "
)   


; to Q31 - done by routine, these patterns are too long (and complex)

;  from Q15

; might be overkill
;

(define_insn "fractqq<mode>2_32"
  [(set (match_operand:AUACC 0 "pic30_accum_or_reg_operand" "=w,r")
        (fract_convert:AUACC
          (match_operand:QQ  1 "pic30_register_operand"     " r,r")))]
  ""
  "@
   lac %1,%0
   mov %1,%d0\;clr %0\;asr %d0,#15,%t0"
)
  
(define_insn "fractuqq<mode>2_32"
  [(set (match_operand:AUACC 0 "pic30_accum_or_reg_operand" "=w,r")
        (fract_convert:AUACC
          (match_operand:UQQ 1 "pic30_register_operand"     " r,r")))]
  ""
  "@
   lac %1,%0
   mov %1,%d0\;clr %0\;asr %d0,#15,%t0"
)
  
(define_insn "fracthq<mode>2_32"
  [(set (match_operand:AUACC 0 "pic30_accum_or_reg_operand" "=w,r")
        (fract_convert:AUACC
          (match_operand:HQ  1 "pic30_register_operand"     " r,r")))]
  ""
  "@
   lac %1,%0
   mov %1,%d0\;clr %0\;asr %d0,#15,%t0"
)
  
(define_insn "fractuhq<mode>2_32"
  [(set (match_operand:AUACC 0 "pic30_accum_or_reg_operand" "=w,r")
        (fract_convert:AUACC
          (match_operand:UHQ 1 "pic30_register_operand"     " r,r")))]
  ""
  "@
   lac %1,%0
   mov %1,%d0\;clr %0\;asr %d0,#15,%t0"
)

; from Q31

(define_insn "fractsq<mode>2_32"
  [(set (match_operand:AUACC 0 "pic30_accum_or_reg_operand" "=w,r")
        (fract_convert:AUACC
          (match_operand:SQ  1 "pic30_register_operand"     " r,r")))]
  ""
  "@
   lac %d1,%0\;mov %1,%m0L
   mov.d %1,%0\;asr %d0,#15,%t0"
)
 
(define_insn "fractusq<mode>2_32"
  [(set (match_operand:AUACC 0 "pic30_accum_or_reg_operand" "=w,r")
        (fract_convert:AUACC
          (match_operand:USQ 1 "pic30_register_operand"     " r,r")))]
  ""
  "@
   lac %d1,%0\;mov %1,%m0L
   mov.d %1,%0\;asr %d0,#15,%t0"
)

(define_insn "fractdq<mode>2_32"
  [(set (match_operand:AUACC 0 "pic30_accum_or_reg_operand" "=w,r")
        (fract_convert:AUACC
          (match_operand:DQ  1 "pic30_register_operand"     " r,r")))]
  ""
  "@
   lac %d1,%0\;mov %1,%m0L
   mov.d %1,%0\;asr %d0,#15,%t0"
)
 
(define_insn "fractudq<mode>2_32"
  [(set (match_operand:AUACC 0 "pic30_accum_or_reg_operand" "=w,r")
        (fract_convert:AUACC
          (match_operand:UDQ 1 "pic30_register_operand"     " r,r")))]
  ""
  "@
   lac %d1,%0\;mov %1,%m0L
   mov.d %1,%0\;asr %d0,#15,%t0"
)

(define_insn "fracttq<mode>2_32"
  [(set (match_operand:AUACC 0 "pic30_accum_or_reg_operand" "=w,r")
        (fract_convert:AUACC
          (match_operand:TQ  1 "pic30_register_operand"     " r,r")))]
  ""
  "@
   lac %d1,%0\;mov %1,%m0L
   mov.d %1,%0\;asr %d0,#15,%t0"
)
 
(define_insn "fractutq<mode>2_32"
  [(set (match_operand:AUACC 0 "pic30_accum_or_reg_operand" "=w,r")
        (fract_convert:AUACC
          (match_operand:UTQ 1 "pic30_register_operand"     " r,r")))]
  ""
  "@
   lac %d1,%0\;mov %1,%m0L
   mov.d %1,%0\;asr %d0,#15,%t0"
)

;  saturating from Q15
;   these never saturate because AUACC is always bigger than a Qn

(define_insn "satfractqq<mode>2_32"
  [(set (match_operand:AUACC 0 "pic30_accum_or_reg_operand" "=w,r")
        (sat_fract:AUACC
          (match_operand:QQ  1 "pic30_register_operand"     " r,r")))]
  ""
  "@
   lac %1,%0
   mov %1,%d0\;clr %0\;asr %d0,#15,%t0"

)
  
(define_insn "satfractuqq<mode>2_32"
  [(set (match_operand:AUACC 0 "pic30_accum_or_reg_operand" "=w,r")
        (sat_fract:AUACC
          (match_operand:UQQ 1 "pic30_register_operand"     " r,r")))]
  ""
  "@
   lac %1,%0
   mov %1,%d0\;clr %0\;asr %d0,#15,%t0"
)
  
(define_insn "satfracthq<mode>2_32"
  [(set (match_operand:AUACC 0 "pic30_accum_or_reg_operand" "=w,r")
        (sat_fract:AUACC
          (match_operand:HQ  1 "pic30_register_operand"     " r,r")))]
  ""
  "@
   lac %1,%0
   mov %1,%d0\;clr %0\;asr %d0,#15,%t0"
)
  
(define_insn "satfractuhq<mode>2_32"
  [(set (match_operand:AUACC 0 "pic30_accum_or_reg_operand" "=w,r")
        (sat_fract:AUACC
          (match_operand:UHQ 1 "pic30_register_operand"     " r,r")))]
  ""
  "@
   lac %1,%0
   mov %1,%d0\;clr %0\;asr %d0,#15,%t0"
)

; from Q31

(define_insn "satfractsq<mode>2_32"
  [(set (match_operand:AUACC 0 "pic30_accum_or_reg_operand" "=w,r")
        (sat_fract:AUACC
          (match_operand:SQ  1 "pic30_register_operand"     " r,r")))]
  ""
  "@
   lac %d1,%0\;mov %1,%m0L
   mov.d %1,%0\;asr %d0,#15,%t0"
)
 
(define_insn "satfractusq<mode>2_32"
  [(set (match_operand:AUACC 0 "pic30_accum_or_reg_operand" "=w,r")
        (sat_fract:AUACC
          (match_operand:USQ 1 "pic30_register_operand"     " r,r")))]
  ""
  "@
   lac %d1,%0\;mov %1,%m0L
   mov.d %1,%0\;asr %d0,#15,%t0"
)

(define_insn "satfractdq<mode>2_32"
  [(set (match_operand:AUACC 0 "pic30_accum_or_reg_operand" "=w,r")
        (sat_fract:AUACC
          (match_operand:DQ  1 "pic30_register_operand"     " r,r")))]
  ""
  "@
   lac %d1,%0\;mov %1,%m0L
   mov.d %1,%0\;asr %d0,#15,%t0"
)
 
(define_insn "satfractudq<mode>2_32"
  [(set (match_operand:AUACC 0 "pic30_accum_or_reg_operand" "=w,r")
        (sat_fract:AUACC
          (match_operand:UDQ 1 "pic30_register_operand"     " r,r")))]
  ""
  "@
   lac %d1,%0\;mov %1,%m0L
   mov.d %1,%0\;asr %d0,#15,%t0"
)

(define_insn "satfracttq<mode>2_32"
  [(set (match_operand:AUACC 0 "pic30_accum_or_reg_operand" "=w,r")
        (sat_fract:AUACC
          (match_operand:TQ  1 "pic30_register_operand"     " r,r")))]
  ""
  "@
   lac %d1,%0\;mov %1,%m0L
   mov.d %1,%0\;asr %d0,#15,%t0"
)
 
(define_insn "satfractutq<mode>2_32"
  [(set (match_operand:AUACC 0 "pic30_accum_or_reg_operand" "=w,r")
        (sat_fract:AUACC
          (match_operand:UTQ 1 "pic30_register_operand"     " r,r")))]
  ""
  "@
   lac %d1,%0\;mov %1,%m0L
   mov.d %1,%0\;asr %d0,#15,%t0"
)

; Accum <-> float conversions appear as functions only


;;  Secondary reload functions

(define_insn "Treload_outqi_32"
  [(set
     (match_operand:QI 0 "pic30_data_operand32"   "=T")
     (match_operand:QI 1 "pic30_register_operand" " r"))
   (clobber 
     (match_operand:SI 2 "pic30_register_operand" "=&r"))
  ]
  ""
  "mov.sl #%0,%2\;mov.b %1,[%2]"
)

(define_insn "Treload_outhi_32"
  [(set
     (match_operand:HI 0 "pic30_data_operand32"   "=T")
     (match_operand:HI 1 "pic30_register_operand" " r"))
   (clobber 
     (match_operand:SI 2 "pic30_register_operand" "=&r"))
  ]
  ""
  "mov.sl #%0,%2\;mov.w %1,[%2]"
)

(define_insn "Treload_out<mode>_32"
  [(set
     (match_operand:SINGLE_REGISTER_MODES_32 0 "pic30_data_operand32"   "=T")
     (match_operand:SINGLE_REGISTER_MODES_32 1 "pic30_register_operand" " r"))
   (clobber 
     (match_operand:SI 2 "pic30_register_operand" "=&r"))
  ]
  ""
  "mov.sl #%0,%2\;mov.l %1,[%2]"
)

(define_insn "Treload_out<mode>_32"
  [(set
     (match_operand:DOUBLE_REGISTER_MODES_32 0 "pic30_data_operand32"   "=T")
     (match_operand:DOUBLE_REGISTER_MODES_32 1 "pic30_register_operand" " r"))
   (clobber 
     (match_operand:SI 2 "pic30_register_operand" "=&r"))
  ]
  ""
  "mov.sl #%0,%2\;mov.l %1,[%2++]\;mov.l %d1,[%2]"
)


(define_insn "Ureload_inQI_32"
  [(set
     (match_operand:QI 0 "pic30_register_operand" "=r")
     (match_operand:QI 1 "pic30_near_operand"     " U"))
   (clobber 
     (match_operand:HI 2 "pic30_register_operand" "=&r"))
  ]
  ""
  "*
   if (REGNO(operands[0]) == WR0_REGNO)
     return \"mov.b %1,WREG\";
   return \"mov #%1,%2\;mov.b [%2],%0\";
  "
)

(define_insn "Ureload_outQI_32"
  [(set
     (match_operand:QI 0 "pic30_near_operand"     "=U")
     (match_operand:QI 1 "pic30_register_operand" " r"))
   (clobber 
     (match_operand:HI 2 "pic30_register_operand" "=&r"))
  ]
  ""
  "*
   if (REGNO(operands[1]) == WR0_REGNO) 
     return \"mov.b WREG,%0\";
   else return \"mov #%0,%2\;mov.b %1,[%2]\";
  "
)

(define_insn "Qreload_in<mode>_32"
  [(set
     (match_operand:QUQ31 0 "pic30_accum_or_reg_operand"  "=w,  r")
     (match_operand:QUQ31 1 "pic30_RQ_operand"           "  RQ, RQ"))
   (clobber (match_operand:HI 2 "pic30_register_operand" "=&r,  X"))
  ]
  ""
  "*
   if (which_alternative == 0) 
     return \"mov %1,%2\;mov %2,%m0L\;mov %Q1,%2\;mov %2,%m0H\;clr %m0U\";
   else {
     rtx Wn = 0;
   
     if (GET_CODE(operands[1]) == MEM) {
       if (GET_CODE(XEXP(operands[1],0)) == REG) {
         Wn = XEXP(operands[1],0);
       } else if (GET_CODE(XEXP(operands[1],0)) == PLUS) {
         Wn = XEXP(XEXP(operands[1],0),0);
       }
     }
     if (Wn) {
       unsigned int regno;

       regno = REGNO(Wn);
       if (regno == REGNO(operands[0]))
         return \"mov %Q1,%d0\;mov %1,%0\";
       else 
         return \"mov %1,%0\;mov %Q1,%d0\";
     }
   }
   gcc_assert(0);
   return \"nop\";
  "
)

(define_insn "Qreload_out<mode>_32"
  [(set
     (match_operand:QUQ31 0 "pic30_RQ_operand"            "=RQ, RQ")
     (match_operand:QUQ31 1 "pic30_accum_or_reg_operand"  "  w,  r"))
   (clobber (match_operand:HI 2 "pic30_register_operand"  "=&r,  X"))
  ]
  ""
  "@
   mov %m1L,%2\;mov %2,%0\;mov %m1H,%2\;mov %2,%Q0
   mov %1,%0\;mov %d1,%Q0"
)

(define_insn "Qreload_in<mode>_32"
  [(set
     (match_operand:AUACC 0 "pic30_accum_or_reg_operand"  "=w,  r")
     (match_operand:AUACC 1 "pic30_RQ_operand"           "  RQ, RQ"))
   (clobber (match_operand:HI 2 "pic30_register_operand"  "=&r, X"))
  ]
  ""
  "*
   if (which_alternative == 0)
     return \"mov %1,%2\;mov %2,%m0L\;mov %Q1,%2\;mov %2,%m0H\;mov %R1,%2\;mov %2,%m0U\";
   else {
     rtx Wn = 0;
   
     if (GET_CODE(operands[1]) == MEM) {
       if (GET_CODE(XEXP(operands[1],0)) == REG) {
         Wn = XEXP(operands[1],0);
       } else if (GET_CODE(XEXP(operands[1],0)) == PLUS) {
         Wn = XEXP(XEXP(operands[1],0),0);
       } 
     }
     if (Wn) {
       unsigned int regno;

       regno = REGNO(Wn);
       if (pic30_psrd_psrd_errata(operands[1],NULL)) {
         pic30_rtx_nops+=2;
       if (regno == REGNO(operands[0]))
           return \"mov %Q1,%d0\;nop\;mov.b %R1,%t0\;nop\;mov %1,%0\";
         else if (regno == REGNO(operands[0])+1)
           return \"mov %1,%0\;nop\;mov.b %R1,%t0\;nop\;mov %Q1,%d0\";
         else
           return \"mov %1,%0\;nop\;mov %Q1,%d0\;nop\;mov.b %R1,%t0\";
       } else {
         if (regno == REGNO(operands[0]))
         return \"mov %Q1,%d0\;mov.b %R1,%t0\;mov %1,%0\";
       else if (regno == REGNO(operands[0])+1)
         return \"mov %1,%0\;mov.b %R1,%t0\;mov %Q1,%d0\";
       else
         return \"mov %1,%0\;mov %Q1,%d0\;mov.b %R1,%t0\";
     }
   }
   }
   gcc_assert(0);
   return \"nop\";
  "
)

(define_insn "Qreload_out<mode>_32"
  [(set
     (match_operand:AUACC 0 "pic30_RQ_operand"           "=RQ, RQ")
     (match_operand:AUACC 1 "pic30_accum_or_reg_operand" "  w,  r"))
   (clobber (match_operand:HI 2 "pic30_register_operand" "=&r,  X"))
  ]
  ""
  "@
   mov %m1L,%2\;mov %2,%0\;mov %m1H,%2\;mov %2,%Q0\;mov %m1U,%2\;mov %2,%R0
   mov %1,%0\;mov %d1,%Q0\;mov.b %t1,%R0"
)

(define_insn "TUreload_in<mode>_32"
  [(set
     (match_operand:AUACC 0 "pic30_accum_or_reg_operand"  "=w, &r")
     (match_operand:AUACC 1 "pic30_data_operand"          " TU,TU"))
   (clobber (match_operand:HI 2 "pic30_register_operand"  "=&r, X"))
  ]
  ""
  "@
   mov %1,%2\;mov %2,%m0L\;mov %1+2,%2\;mov %2,%m0H\;mov %1+4,%2\;mov %2,%m0U
   mov %1,%0\;mov %1+2,%d0\;mov %1+4,%t0"
)

(define_insn "TUreload_insi_32"
  [(set
     (match_operand:SI 0 "pic30_accum_or_reg_operand"     "=w, &r")
     (match_operand:SI 1 "pic30_data_operand"             " TU,TU"))
   (clobber (match_operand:HI 2 "pic30_register_operand"  "=&r, X"))
  ]
  ""
  "@
   mov %1,%2\;mov %2,%m0L\;mov %1+2,%2\;mov %2,%m0H
   mov %1,%0\;mov %1+2,%d0"
)
      

(define_insn "TUreload_out<mode>_32"
  [(set
     (match_operand:AUACC 0 "pic30_data_operand"         "=TU,TU")
     (match_operand:AUACC 1 "pic30_accum_or_reg_operand" " w, r"))
   (clobber (match_operand:HI 2 "pic30_register_operand" "=&r,X"))
  ]
  ""
  "@
   mov %m1L,%2\;mov %2,%0\;mov %m1H,%2\;mov %2,%0+2\;mov %m1U,%2\;mov %2,%0+4
   mov %1,%0\;mov %d1,%0+2\;mov %t1,%0+4"
)

(define_insn "TUreload_outhi_32"
  [(set
     (match_operand:HI   0 "pic30_data_operand"         "=TU,TU")
     (match_operand:HI   1 "pic30_accum_or_reg_operand" " w, r"))
   (clobber (match_operand:HI 2 "pic30_register_operand" "=&r,X"))
  ]
  ""
  "@
   mov %m1L,%2\;mov %2,%0
   mov %1,%0"
)

(define_insn "TUreload_outsi_32"
  [(set
     (match_operand:SI   0 "pic30_data_operand"         "=TU,TU")
     (match_operand:SI   1 "pic30_accum_or_reg_operand" " w, r"))
   (clobber (match_operand:HI 2 "pic30_register_operand" "=&r,X"))
  ]
  ""
  "@
   mov %m1L,%2\;mov %2,%0\;mov %m1H,%2\;mov %2,%0+2
   mov %1,%0\;mov %d1,%0+2"
)

; reloads because the displacement is too large

(define_insn "Qreloaddisp_out<mode>_32"
  [(set
      (match_operand:QRELOADS  0 "pic30_any_QR_operand"    "= R, X")
      (match_operand:QRELOADS  1 "pic30_register_operand"  "  r, r"))
   (clobber (match_operand:HI  2 "pic30_register_operand"  "=&r,&r"))
  ]
  ""
  "*
  { 
    if (which_alternative == 1) {
      rtx inner;
      rtx lhs,rhs;
      int offset;
      static char result[256];

      gcc_assert(MEM_P(operands[0]));
      inner = XEXP(operands[0],0);
      gcc_assert((GET_CODE(inner) == PLUS) || (GET_CODE(inner) == MINUS));
      lhs = XEXP(inner,0);
      rhs = XEXP(inner,1);
      gcc_assert(REG_P(lhs));
      gcc_assert(GET_CODE(rhs) == CONST_INT);
      offset = INTVAL(rhs);
      if (GET_CODE(inner) == MINUS) offset = -1*offset;
      
      if (GET_MODE_SIZE(<MODE>mode) == 1) {
        sprintf(result, \"mov #%d,%%2\;\"
                        \"mov.b %%1,[%s+%%2]\", 
                offset, reg_names[REGNO(lhs)]);
      } else if (GET_MODE_SIZE(<MODE>mode) == 2) {
        sprintf(result, \"mov #%d,%%2\;\"
                        \"mov %%1,[%s+%%2]\",
                offset, reg_names[REGNO(lhs)]);
      } else if (GET_MODE_SIZE(<MODE>mode) == 4) {
        sprintf(result, \"mov #%d,%%2\;\"
                        \"add.l %s,%%2,%%2\;\"
                        \"mov %%1,[%%2++]\;mov %%d1,[%%2]\",
                offset, reg_names[REGNO(lhs)]);
      } else if (GET_MODE_SIZE(<MODE>mode) == 6) {
        sprintf(result, \"mov #%d,%%2\;\"
                        \"add.l %s,%%2,%%2\;\"
                        \"mov %%1,[%%2++]\;mov %%d1,[%%2++]\;mov %%t1,[%%2]\",
               offset, reg_names[REGNO(lhs)]);
      } else if (GET_MODE_SIZE(<MODE>mode) == 8) {
        sprintf(result, \"mov #%d,%%2\;\"
                        \"add.l %s,%%2,%%2\;\"
                        \"mov %%1,[%%2++]\;mov %%d1,[%%2++]\;\"
                        \"mov %%t1,[%%2++]\;mov %%q1,[%%2]\", 
              offset, reg_names[REGNO(lhs)]);
      } else gcc_assert(0);
      return result;
    } else {
      if (GET_MODE_SIZE(<MODE>mode) == 1) {
        return \"mov.b %1,%0\";
      } else if (GET_MODE_SIZE(<MODE>mode) == 2) {
        return \"mov %1,%0\";
      } else if (GET_MODE_SIZE(<MODE>mode) == 4) {
        return \"mov %1,%I0\;mov %d1,%D0\";
      } else if (GET_MODE_SIZE(<MODE>mode) == 6) {
        return \"mov %r0,%2\;\"
               \"mov %1,[%2++]\;mov %d1,[%2++]\;mov %t1,[%2]\";
      } else if (GET_MODE_SIZE(<MODE>mode) == 8) {
        return \"mov %r0,%2\;\"
               \"mov %1,[%2++]\;mov %d1,[%2++]\;mov %t1,[%2++]\;mov %q1,[%2]\";
      } else gcc_assert(0);
      return \"nop\";
    }
  }
  "
)

(define_insn "Qreloaddisp_in<mode>_32"
  [(set
     (match_operand:QRELOADS  0 "pic30_register_operand" "= r, r")
     (match_operand:QRELOADS  1 "pic30_any_QR_operand"   "  R, X"))
   (clobber (match_operand:HI 2 "pic30_register_operand" "=&r,&r"))
  ]
  ""
  "*
  {
    if (which_alternative == 1) {
      rtx inner;
      rtx lhs,rhs;
      int offset;
      static char result[256];

      gcc_assert(MEM_P(operands[1]));
      inner = XEXP(operands[1],0);
      gcc_assert((GET_CODE(inner) == PLUS) || (GET_CODE(inner) == MINUS));
      lhs = XEXP(inner,0);
      rhs = XEXP(inner,1);
      gcc_assert(REG_P(lhs));
      gcc_assert(GET_CODE(rhs) == CONST_INT);
      offset = INTVAL(rhs);
      if (GET_CODE(inner) == MINUS) offset = -1*offset;
      
      if (GET_MODE_SIZE(<MODE>mode) == 1) {
        sprintf(result, \"mov #%d,%%2\;\"
                        \"mov.b [%s+%%2],%%0\",
               offset, reg_names[REGNO(lhs)]);
      } else if (GET_MODE_SIZE(<MODE>mode) == 2) {
        sprintf(result, \"mov #%d,%%2\;\"
                        \"mov [%s+%%2],%%0\",
               offset, reg_names[REGNO(lhs)]);
      } else if (GET_MODE_SIZE(<MODE>mode) == 4) {
        if (pic30_psrd_psrd_errata(operands[1],NULL)) {
          pic30_rtx_nops+=1;
        sprintf(result, \"mov #%d,%%2\;\"
                        \"add.l %s,%%2,%%2\;\"
                        \"mov [%%2++],%%0\;\"
                        \"nop\;\"
                        \"mov [%%2],%%d0\",
               offset, reg_names[REGNO(lhs)]);
        } else {
          sprintf(result, \"mov #%d,%%2\;\"
                          \"add.l %s,%%2,%%2\;\"
                          \"mov [%%2++],%%0\;\"
                          \"mov [%%2],%%d0\",
                 offset, reg_names[REGNO(lhs)]);
        }
      } else if (GET_MODE_SIZE(<MODE>mode) == 6) {
        if (pic30_psrd_psrd_errata(operands[1],NULL)) {
          pic30_rtx_nops+=2;
        sprintf(result, \"mov #%d,%%2\;\"
                        \"add.l %s,%%2,%%2\;\"
                        \"mov [%%2++],%%0\;\"
                        \"nop\;\"
                        \"mov [%%2++],%%d0\;\"
                        \"nop\;\"
                        \"mov [%%2],%%t0\",
               offset, reg_names[REGNO(lhs)]);
        } else {
          sprintf(result, \"mov #%d,%%2\;\"
                          \"add.l %s,%%2,%%2\;\"
                          \"mov [%%2++],%%0\;\"
                          \"mov [%%2++],%%d0\;\"
                          \"mov [%%2],%%t0\",
                 offset, reg_names[REGNO(lhs)]);
        }
      } else if (GET_MODE_SIZE(<MODE>mode) == 8) {
        if (pic30_psrd_psrd_errata(operands[1],NULL)) {
          pic30_rtx_nops+=3;
        sprintf(result, \"mov #%d,%%2\;\"
                        \"add.l %s,%%2,%%2\;\"
                        \"mov [%%2++],%%0\;\"
                        \"nop\;\"
                        \"mov [%%2++],%%d0\;\"
                        \"nop\;\"
                        \"mov [%%2++],%%t0\;\"
                        \"nop\;\"
                        \"mov [%%2],%%q0\",
               offset, reg_names[REGNO(lhs)]);
        } else {
          sprintf(result, \"mov #%d,%%2\;\"
                          \"add.l %s,%%2,%%2\;\"
                          \"mov [%%2++],%%0\;\"
                          \"mov [%%2++],%%d0\;\"
                          \"mov [%%2++],%%t0\;\"
                          \"mov [%%2],%%q0\",
                 offset, reg_names[REGNO(lhs)]);
        }
      } else gcc_assert(0);
      return result;
    } else {
      if (GET_MODE_SIZE(<MODE>mode) == 1) {
        return \"mov.b %1,%0\";
      } else if (GET_MODE_SIZE(<MODE>mode) == 2) {
        return \"mov %1,%0\";
      } else if (GET_MODE_SIZE(<MODE>mode) == 4) {
        if (pic30_psrd_psrd_errata(operands[1],NULL)) {
          pic30_rtx_nops+=1;
          return \"mov %I1,%0\;\"
                 \"nop\;\"
                 \"mov %D1,%d0\";
        } else {
          return \"mov %I1,%0\;\"
                 \"mov %D1,%d0\";
        }
      } else if (GET_MODE_SIZE(<MODE>mode) == 6) {
        if (pic30_psrd_psrd_errata(operands[1],NULL)) {
          pic30_rtx_nops+=2;
        return \"mov %r1,%2\;\"
               \"nop\;\"
               \"mov [%2++],%0\;\"
               \"nop\;\"
               \"mov [%2++],%d0\;\"
               \"nop\;\"
               \"mov [%2],%t0\";
        } else {
          return \"mov %r1,%2\;\"
                 \"mov [%2++],%0\;\"
                 \"mov [%2++],%d0\;\"
                 \"mov [%2],%t0\";
        }
      } else if (GET_MODE_SIZE(<MODE>mode) == 8) {
        if (pic30_psrd_psrd_errata(operands[1],NULL)) {
          pic30_rtx_nops+=3;
        return \"mov %r1,%2\;\"
                 \"mov [%2++],%0\;\"
                 \"nop\;\"
                 \"mov [%2++],%d0\;\"
                 \"nop\;\"
                 \"mov [%2++],%t0\;\"
                 \"nop\;\"
                 \"mov [%2],%q0\";
        } else {
          return \"mov %r1,%2\;\"
                 \"mov [%2++],%0\;\"
                 \"mov [%2++],%d0\;\"
                 \"mov [%2++],%t0\;\"
                 \"mov [%2],%q0\";
        }
      } else gcc_assert(0);
    }
  }
  "
)

;;;; float SF

(define_insn "movfpsf_32"
  [(set
     (match_operand:SF 0 "pic30_float_mem_operand"        "=R,Q,TU,TU")
     (match_operand:SF 1 "pic30_float_reg_or_mem_operand" " l,l,l, TU"))
  ]
  "(pic30_isa32_target())"
  "@
   mov.l %1,%0
   mov.l %1,%0
   push.l %1\;pop.l %0
   push.l %1\;pop.l %0"
)

(define_insn "movsffp_32"
  [(set (match_operand:SF 0 "pic30_float_register_operand" "=l,l,l")
        (match_operand:SF 1 "pic30_float_mem_operand"       "R,Q,TU"))
  ]
  "(pic30_isa32_target())"
  "@
   mov.l %1,%0
   mov.l %1,%0
   push.l %1\;pop.l %0"
;  push.l w0\;mov.l %1,w0\;mov.l w0,%0\;pop.l w0"
)

(define_insn "movsfsf_32"
  [(set (match_operand:SF 0 "pic30_float_register_operand" "=rl,l,l,r")
        (match_operand:SF 1 "pic30_any_register_operand"   " 0, l,r,l"))]
  "(pic30_isa32_target())"
  "@
   \; nop
   mov.s %1,%0
   mov.l %1,%0
   mov.l %1,%0"
)

(define_insn "addsf3_32"
  [(set (match_operand:SF    0 "pic30_float_register_operand" "=l")
        (plus:SF
           (match_operand:SF 1 "pic30_float_register_operand" " l")
           (match_operand:SF 2 "pic30_float_register_operand" " l")))
   (clobber (reg:CC FSR))
  ]
  "pic30_isa32_target()"
  "add.s %1,%2,%0"
)

(define_insn "subsf3_32"
  [(set (match_operand:SF    0 "pic30_float_register_operand" "=l")
        (minus:SF
           (match_operand:SF 1 "pic30_float_register_operand" " l")
           (match_operand:SF 2 "pic30_float_register_operand" " l")))
   (clobber (reg:CC FSR))
  ]
  "pic30_isa32_target()"
  "sub.s %1,%2,%0"
)

(define_insn "divsf3_32"
  [(set (match_operand:SF    0 "pic30_float_register_operand" "=l")
        (div:SF
           (match_operand:SF 1 "pic30_float_register_operand" " l")
           (match_operand:SF 2 "pic30_float_register_operand" " l")))
   (clobber (reg:CC FSR))
  ]
  "pic30_isa32_target()"
  "div.s %1,%2,%0"
)

(define_insn "mulsf3_32"
  [(set (match_operand:SF    0 "pic30_float_register_operand" "=l")
        (mult:SF
           (match_operand:SF 1 "pic30_float_register_operand" " l")
           (match_operand:SF 2 "pic30_float_register_operand" " l")))
   (clobber (reg:CC FSR))
  ]
  "pic30_isa32_target()"
  "mul.s %1,%2,%0"
)

(define_insn "sminsf3_32"
  [(set (match_operand:SF    0 "pic30_float_register_operand" "=l")
        (smin:SF
           (match_operand:SF 1 "pic30_float_register_operand" " l")
           (match_operand:SF 2 "pic30_float_register_operand" " l")))
   (clobber (reg:CC FSR))
  ]
  "pic30_isa32_target()"
  "min.s %1,%2,%0"
)

(define_insn "smaxsf3_32"
  [(set (match_operand:SF    0 "pic30_float_register_operand" "=l")
        (smax:SF
           (match_operand:SF 1 "pic30_float_register_operand" " l")
           (match_operand:SF 2 "pic30_float_register_operand" " l")))
   (clobber (reg:CC FSR))
  ]
  "pic30_isa32_target()"
  "max.s %1,%2,%0"
)

(define_insn "negsf2_32"
  [(set (match_operand:SF    0 "pic30_float_register_operand" "=l")
        (neg:SF
           (match_operand:SF 1 "pic30_float_register_operand" "l")))
   (clobber (reg:CC FSR))
  ]
  "pic30_isa32_target()"
  "neg.s %1,%0"
)

(define_insn "abssf2_32"
  [(set (match_operand:SF    0 "pic30_float_register_operand" "=l")
        (abs:SF
           (match_operand:SF 1 "pic30_float_register_operand" "l")))
   (clobber (reg:CC FSR))
  ]
  "pic30_isa32_target()"
  "abs.s %1,%0"
)

(define_insn "sqrtsf2_32"
  [(set (match_operand:SF    0 "pic30_float_register_operand" "=l")
        (sqrt:SF
           (match_operand:SF 1 "pic30_float_register_operand" "l")))
   (clobber (reg:CC FSR))
  ]
  "pic30_isa32_target()"
  "sqrt.s %1,%0"
)

(define_insn "cossf2"
  [(set (match_operand:SF    0 "pic30_float_register_operand" "=l")
        (unspec:SF  [
           (match_operand:SF 1 "pic30_float_register_operand" "l")
         ] UNSPEC_COS))
   (clobber (reg:CC FSR))
  ]
  "pic30_isa32_target()"
  "cos.s %1,%0"
)

(define_insn "sinsf2"
  [(set (match_operand:SF    0 "pic30_float_register_operand" "=l")
        (unspec:SF  [
           (match_operand:SF 1 "pic30_float_register_operand" "l")
         ] UNSPEC_SIN))
   (clobber (reg:CC FSR))
  ]
  "pic30_isa32_target()"
  "sin.s %1,%0"
)

(define_insn "adddf3_32"
  [(set (match_operand:DF    0 "pic30_float_register_operand" "=l")
        (plus:DF
           (match_operand:DF 1 "pic30_float_register_operand" " l")
           (match_operand:DF 2 "pic30_float_register_operand" " l")))
   (clobber (reg:CC FSR))
  ]
  "pic30_isa32_target()"
  "add.d %1,%2,%0"
)

(define_insn "subdf3_32"
  [(set (match_operand:DF    0 "pic30_float_register_operand" "=l")
        (minus:DF
           (match_operand:DF 1 "pic30_float_register_operand" " l")
           (match_operand:DF 2 "pic30_float_register_operand" " l")))
   (clobber (reg:CC FSR))
  ]
  "pic30_isa32_target()"
  "sub.d %1,%2,%0"
)

(define_insn "divdf3_32"
  [(set (match_operand:DF    0 "pic30_float_register_operand" "=l")
        (div:DF
           (match_operand:DF 1 "pic30_float_register_operand" " l")
           (match_operand:DF 2 "pic30_float_register_operand" " l")))
   (clobber (reg:CC FSR))
  ]
  "pic30_isa32_target()"
  "div.d %1,%2,%0"
)

(define_insn "muldf3_32"
  [(set (match_operand:DF    0 "pic30_float_register_operand" "=l")
        (mult:DF
           (match_operand:DF 1 "pic30_float_register_operand" " l")
           (match_operand:DF 2 "pic30_float_register_operand" " l")))
   (clobber (reg:CC FSR))
  ]
  "pic30_isa32_target()"
  "mul.d %1,%2,%0"
)

(define_insn "smindf3_32"
  [(set (match_operand:DF    0 "pic30_float_register_operand" "=l")
        (smin:DF
           (match_operand:DF 1 "pic30_float_register_operand" " l")
           (match_operand:DF 2 "pic30_float_register_operand" " l")))
   (clobber (reg:CC FSR))
  ]
  "pic30_isa32_target()"
  "min.d %1,%2,%0"
)

(define_insn "smaxdf3_32"
  [(set (match_operand:DF    0 "pic30_float_register_operand" "=l")
        (smax:DF
           (match_operand:DF 1 "pic30_float_register_operand" " l")
           (match_operand:DF 2 "pic30_float_register_operand" " l")))
   (clobber (reg:CC FSR))
  ]
  "pic30_isa32_target()"
  "max.d %1,%2,%0"
)

(define_insn "negdf2_32"
  [(set (match_operand:DF    0 "pic30_float_register_operand" "=l")
        (neg:DF
           (match_operand:DF 1 "pic30_float_register_operand"  "l")))
   (clobber (reg:CC FSR))
  ]
  "pic30_isa32_target()"
  "neg.d %1,%0"
)

(define_insn "absdf2_32"
  [(set (match_operand:DF    0 "pic30_float_register_operand" "=l")
        (abs:DF
           (match_operand:DF 1 "pic30_float_register_operand"  "l")))
   (clobber (reg:CC FSR))
  ]
  "pic30_isa32_target()"
  "abs.d %1,%0"
)

(define_insn "sqrtdf2_32"
  [(set (match_operand:DF    0 "pic30_float_register_operand" "=l")
        (sqrt:DF
           (match_operand:DF 1 "pic30_float_register_operand"  "l")))
   (clobber (reg:CC FSR))
  ]
  "pic30_isa32_target()"
  "sqrt.d %1,%0"
)

(define_insn "cmpsf_32"
  [(set (reg:CC FSR)
        (compare
          (match_operand:SF 0 "pic30_float_register_operand" "l")
          (match_operand:SF 1 "pic30_float_register_operand" "l")))]
  "(pic30_isa32_target())"
  "cpq.s %0,%1"
  [
    (set_attr "cc" "set")
  ]
)

(define_insn "cmpdf_32"
  [(set (reg:CC FSR)
        (compare
          (match_operand:DF 0 "pic30_float_register_operand" "l")
          (match_operand:DF 1 "pic30_float_register_operand" "l")))]
  "(pic30_isa32_target())"
  "cpq.d %0,%1"
  [
    (set_attr "cc" "set")
  ]
)

;  SF->SI
;  DF->SI
;  SF->DI
;  DF->DI

(define_insn "fix_truncsfsi2"
  [(set (match_operand:SI   0 "pic30_register_operand" "=r")
        (fix:SI
          (match_operand:SF 1 "pic30_float_register_operand" "l")))
   (clobber 
     (match_scratch:SF      2                                "=l"))
  ]
  "(pic30_isa32_target())"
  "f2li.s %1,%2\;mov.l %2,%0"
)

(define_insn "fix_truncdfsi2"
  [(set (match_operand:SI   0 "pic30_register_operand" "=r")
        (fix:SI
          (match_operand:DF 1 "pic30_float_register_operand" "l")))
   (clobber 
     (match_scratch:SF      2                                "=l"))
  ]
  "(pic30_isa32_target())"
  "f2li.d %1,%2\;mov.l %2,%0"
)

(define_insn "fix_truncsfdi2"
  [(set (match_operand:DI   0 "pic30_register_operand"       "=r")
        (fix:DI
          (match_operand:SF 1 "pic30_float_register_operand" " l")))
   (clobber 
     (match_scratch:DF      2                                "=l"))
  ]
  "(pic30_isa32_target())"
  "f2di.s %1,%2\;mov.l %2,%0\;mov.l %d2,%d0"
)

(define_insn "fix_truncdfdi2"
  [(set (match_operand:DI   0 "pic30_register_operand" "=r")
        (fix:DI
          (match_operand:DF 1 "pic30_float_register_operand" "l")))
   (clobber 
     (match_scratch:DF      2                                "=l"))
  ]
  "(pic30_isa32_target())"
  "f2di.d %1,%2\;mov.l %2,%0\;mov.l %d2,%d0"
)

; SI->SF
; DI->SF
; DI->DF
; SI->DF

(define_insn "floatsisf2"
  [(set (match_operand:SF   0 "pic30_float_register_operand" "=l")
        (float:SF
          (match_operand:SI 1 "pic30_register_operand" "r")))]
  "(pic30_isa32_target())"
  "mov.l %1,%0\;li2f.s %0,%0"
)

(define_insn "floatdisf2"
  [(set (match_operand:SF   0 "pic30_float_register_operand" "=l")
        (float:SF
          (match_operand:DI 1 "pic30_register_operand" " r")))
   (clobber  
          (match_scratch:DF 2                          "=l"))
  ]
  "(pic30_isa32_target())"
  "mov.l %1,%2\;mov.l %d1,%d2\;di2f.s %2,%0"
)

(define_insn "floatdidf2"
  [(set (match_operand:DF   0 "pic30_float_register_operand" "=l")
        (float:DF
          (match_operand:DI 1 "pic30_register_operand" "r")))]
  "(pic30_isa32_target())"
  "mov.l %1,%0\;mov.l %d1,%d0\;di2f.d %0,%0"
)

(define_insn "floatsidf2"
  [(set (match_operand:DF   0 "pic30_float_register_operand" "=l")
        (float:DF
          (match_operand:SI 1 "pic30_register_operand" "r")))]
  "(pic30_isa32_target())"
  "mov.l %1,%0\;li2f.d %0,%0"
)

;;
;; looping
(define_expand "decrement_and_branch_until_zero"
   [(parallel [(set (pc)
                 (if_then_else
                   (ne (match_operand 0 "register_operand" "")
                       (const_int 1))
                   (label_ref (match_operand 1 "" ""))
                   (pc)))
               (set (match_dup 0)
                 (plus (match_dup 0)
                       (const_int -1)))
               (clobber (match_scratch:SI 2 ""))])]
  "(pic30_isa32_target())"
  ""
)

(define_expand "doloop_end_32"
  [(use (match_operand 0 "" ""))        ; loop pseudo
   (use (match_operand 1 "" ""))]       ; label
  "(pic30_isa32_target())"
{
  if (GET_MODE(operands[0]) == SImode) {  
    emit_jump_insn(
      gen_dtbsi(operands[0], operands[1])
    );
    DONE;
  }
  if (GET_MODE(operands[0]) == HImode) {
    emit_jump_insn(
      gen_dtbhi(operands[0], operands[1])
    );
    DONE;
  }
  if (GET_MODE(operands[0]) == QImode) {
    emit_jump_insn(
      gen_dtbqi(operands[0], operands[1])
    );
    DONE;
  } 
  FAIL;
})

(define_insn "dtbqi"
  [(set (pc)
        (if_then_else
          (ne (match_operand:QI 0 "pic30_reg_or_mem_operand" "+r,R,m")
              (const_int 1))
          (label_ref (match_operand 1 "" ""))
          (pc)))
   (set (match_dup 0)
        (plus:QI (match_dup 0)
                 (const_int -1)))
   (clobber (match_scratch:SI 2 "=X,X,r"))
  ]
  "(pic30_isa32_target())"
  "*
   switch (which_alternative) {
     case 0:
       return \"dtb %0,%1\";
     case 1:
       return \"sub.%v0 %0,#1,%0\;bra nz,%1\";
     case 2:
       if (!optimize_size)
         return \"mov.%v0 %0,%2\;sub.%v0 %2,#1,%2\;mov.%v0 %2,%0\;bra nz,%1\";
       else return \"%l0.2 %2\;sub.%v0 [%2],#1,[%2]\;bra nz,%1\";
   }
  "
)

(define_insn "dtbhi"
  [(set (pc)
        (if_then_else
          (ne (match_operand:HI 0 "pic30_reg_or_mem_operand" "+r,R,m")
              (const_int 1))
          (label_ref (match_operand 1 "" ""))
          (pc)))
   (set (match_dup 0)
        (plus:HI (match_dup 0)
                 (const_int -1)))
   (clobber (match_scratch:SI 2 "=X,X,r"))
  ]
  "(pic30_isa32_target())"
  "*
   switch (which_alternative) {
     case 0:
       return \"dtb %0,%1\";
     case 1:
       return \"sub.%v0 %0,#1,%0\;bra nz,%1\";
     case 2:
       if (!optimize_size)
         return \"mov.%v0 %0,%2\;sub.%v0 %2,#1,%2\;mov.%v0 %2,%0\;bra nz,%1\";
       else return \"%l0.2 %2\;sub.%v0 [%2],#1,[%2]\;bra nz,%1\";
   }
  "
)


(define_insn "dtbsi"
  [(set (pc)
        (if_then_else
          (ne (match_operand:SI 0 "pic30_reg_or_mem_operand" "+r,R,m")
              (const_int 1))
          (label_ref (match_operand 1 "" ""))
          (pc)))
   (set (match_dup 0)
        (plus:SI (match_dup 0)
                 (const_int -1)))
   (clobber (match_scratch:SI 2 "=X,X,r"))
  ]
  "(pic30_isa32_target())"
  "*
   switch (which_alternative) {
     case 0:
       return \"dtb %0,%1\";
     case 1:
       return \"sub.%v0 %0,#1,%0\;bra nz,%1\";
     case 2:
       if (!optimize_size)
         return \"mov.%v0 %0,%2\;sub.%v0 %2,#1,%2\;mov.%v0 %2,%0\;bra nz,%1\";
       else return \"%l0.2 %2\;sub.%v0 [%2],#1,[%2]\;bra nz,%1\";
   }
  "
)

(define_insn "swapqi_32"
    [(set (match_operand:QI     0 "pic30_register_operand" "=r,r")
          (ior:QI
            (ashift:QI
              (match_operand:QI 1 "pic30_register_operand" " 0,r")
              (const_int 4))
            (lshiftrt:QI
              (match_dup 1)
              (const_int 4))))]

   "TARGET_ISA32"
   "@
    swap.b %0
    mov.b %1,%0\;swap.b %0"
)

(define_insn "swaphi_32"
    [(set (match_operand:HI     0 "pic30_register_operand" "=r,r")
          (ior:HI
            (ashift:HI
              (match_operand:HI 1 "pic30_register_operand" " 0,r")
              (const_int 8))
            (lshiftrt:HI
              (match_dup 1)
              (const_int 8))))]

   "TARGET_ISA32"
   "@
    swap.w %0
    mov.w %1,%0\;swap.w %0"
)

(define_insn "flim_packhi_32"
  [(set (match_operand:DI       0 "pic30_register_operand" "=r")
        (ior:DI
          (zero_extend:DI
            (match_operand:HI   1 "pic30_register_operand"  "r"))
          (ashift:DI
            (zero_extend:DI
              (match_operand:HI 2 "pic30_register_operand"  "r"))
            (const_int 32))))
  ]
  "TARGET_ISA32"
  "*
   if (REGNO(operands[1]) == REGNO(operands[0]))
      return \"; nop\";
   if (REGNO(operands[2]) == REGNO(operands[0]))
      return \"mov.w %2,%d0\;mov.w %1,%0\";
   else return \"mov.w %1,%0\;mov.w %2,%d0\";
  "
)

(define_insn "flimhi_32"
  [(set (match_operand:HI   0 "pic30_register_operand" "+r")
        (if_then_else
          (gt (match_operand:HI 1 "pic30_register_operand" "0")
              (subreg:HI (match_operand:DI 2 "pic30_register_operand" "r") 0))
          (subreg:HI (match_dup 2) 0)
          (if_then_else
            (lt (match_dup 1)
                (subreg:HI (match_dup 2) 4))
             (subreg:HI (match_dup 2) 4)
             (match_dup 1))))
   (clobber (reg:CC_NZ CC_REG))
  ]
  "(TARGET_ISA32)"
  "flim.w %2,%0"
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "flim_packsi_32"
  [(set (match_operand:DI       0 "pic30_register_operand" "=r")
        (ior:DI
          (zero_extend:DI
            (match_operand:SI   1 "pic30_register_operand"  "r"))
          (ashift:DI
            (zero_extend:DI
              (match_operand:SI 2 "pic30_register_operand"  "r"))
            (const_int 32))))
  ]
  "TARGET_ISA32"
  "*
   if (REGNO(operands[1]) == REGNO(operands[0]))
      return \"; nop\";
   if (REGNO(operands[2]) == REGNO(operands[0]))
      return \"mov.l %2,%d0\;mov.l %1,%0\";
   else return \"mov.l %1,%0\;mov.l %2,%d0\";
  "
)

(define_insn "flimsi_32"
  [(set (match_operand:SI   0 "pic30_register_operand" "+r")
        (if_then_else
          (gt (match_operand:SI 1 "pic30_register_operand" "0")
              (subreg:SI (match_operand:DI 2 "pic30_register_operand" "r") 0))
          (subreg:SI (match_dup 2) 0)
          (if_then_else
            (lt (match_dup 1)
                (subreg:SI (match_dup 2) 4))
             (subreg:SI (match_dup 2) 4)
             (match_dup 1))))
   (clobber (reg:CC_NZ CC_REG))
  ]
  "(TARGET_ISA32)"
  "flim.l %2,%0"
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "flim_excesshi_32"
  [(parallel
    [(set (match_operand:HI 0 "pic30_register_operand" "+r")
         (if_then_else
            (gt (match_operand:HI 1 "pic30_register_operand" " 0")
                (subreg:HI (match_operand:DI 2 "pic30_register_operand" "r") 0))
            (subreg:HI (match_dup 2) 0)
            (if_then_else
              (lt (match_dup 1)
                  (subreg:HI (match_dup 2) 4))
               (subreg:HI (match_dup 2) 4)
               (match_dup 1))))
    (set (match_operand:HI   3 "pic30_register_operand" "=r")
         (if_then_else
            (gt (match_dup 1)
                (subreg:HI (match_dup 2) 0))
            (const_int 1)
          (if_then_else
              (lt (match_dup 1)
                  (subreg:HI (match_dup 2) 4))
               (const_int -1)
               (const_int 0))))
     (clobber (reg:CC_NZ CC_REG))
    ])
  ]
  "(TARGET_CLASSIC) && (pic30_isav4_target())"
  "flim.w %2,%1,%3"
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "flim_excesssi_32"
  [(parallel
    [(set (match_operand:SI 0 "pic30_register_operand" "+r")
         (if_then_else
            (gt (match_operand:SI 1 "pic30_register_operand" " 0")
                (subreg:SI (match_operand:DI 2 "pic30_register_operand" "r") 0))
            (subreg:SI (match_dup 2) 0)
            (if_then_else
              (lt (match_dup 1)
                  (subreg:SI (match_dup 2) 4))
               (subreg:SI (match_dup 2) 4)
               (match_dup 1))))
    (set (match_operand:SI   3 "pic30_register_operand" "=r")
         (if_then_else
            (gt (match_dup 1)
                (subreg:SI (match_dup 2) 0))
            (const_int 1)
          (if_then_else
              (lt (match_dup 1)
                  (subreg:SI (match_dup 2) 4))
               (const_int -1)
               (const_int 0))))
     (clobber (reg:CC_NZ CC_REG))
    ])
  ]
  "(TARGET_CLASSIC) && (pic30_isav4_target())"
  "flim.l %2,%1,%3"
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "flimv_excesshi_32"
  [(parallel
    [(set (match_operand:HI 0 "pic30_register_operand" "+r")
         (if_then_else
            (gt (match_operand:HI 1 "pic30_register_operand" " 0")
                (subreg:HI (match_operand:DI 2 "pic30_register_operand" "r") 0))
            (subreg:HI (match_dup 2) 0)
            (if_then_else
              (lt (match_dup 1)
                  (subreg:HI (match_dup 2) 4))
               (subreg:HI (match_dup 2) 4)
               (match_dup 1))))
    (set (match_operand:HI   3 "pic30_register_operand" "=r")
         (if_then_else
            (gt (match_dup 1)
                (subreg:HI (match_dup 2) 0))
            (minus:HI
              (match_dup 1)
              (subreg:HI (match_dup 2) 0))
            (if_then_else
              (lt (match_dup 1)
                  (subreg:HI (match_dup 2) 4))
               (minus:HI
                 (match_dup 1)
                 (subreg:HI (match_dup 2) 4))
               (const_int 0))))
     (clobber (reg:CC_NZ CC_REG))
    ])
  ]
  "(TARGET_CLASSIC) && (pic30_isav4_target())"
  "flim.wv %2,%1,%3"
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "flimv_excesssi_32"
  [(parallel
    [(set (match_operand:SI 0 "pic30_register_operand" "+r")
         (if_then_else
            (gt (match_operand:SI 1 "pic30_register_operand" " 0")
                (subreg:SI (match_operand:DI 2 "pic30_register_operand" "r") 0))
            (subreg:SI (match_dup 2) 0)
            (if_then_else
              (lt (match_dup 1)
                  (subreg:SI (match_dup 2) 4))
               (subreg:SI (match_dup 2) 4)
               (match_dup 1))))
    (set (match_operand:SI   3 "pic30_register_operand" "=r")
         (if_then_else
            (gt (match_dup 1)
                (subreg:SI (match_dup 2) 0))
            (minus:SI
              (match_dup 1)
              (subreg:SI (match_dup 2) 0))
            (if_then_else
              (lt (match_dup 1)
                  (subreg:SI (match_dup 2) 4))
               (minus:SI
                 (match_dup 1)
                 (subreg:SI (match_dup 2) 4))
               (const_int 0))))
     (clobber (reg:CC_NZ CC_REG))
    ])
  ]
  "(TARGET_CLASSIC) && (pic30_isav4_target())"
  "flim.lv %2,%1,%3"
  [
    (set_attr "cc" "clobber")
  ]
)

;;
;; DSP min/max
;; 

(define_insn "min_32"
  [
  (set (match_operand:SI   0 "pic30_accumulator_operand" "=w")
       (unspec:SI [
          (match_operand:SI 1 "pic30_accumulator_operand" " 0")
          (match_operand:SI 2 "pic30_accumulator_operand" " w")
        ] UNSPEC_MIN))
   (clobber (reg:CC_NZ CC_REG))
  ]
  "(TARGET_ISA32)"
  "min %0"
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "min_excess_32"
  [(parallel [
    (set (match_operand:SI   0 "pic30_accumulator_operand" "=w")
         (unspec:SI [
            (match_operand:SI 1 "pic30_accumulator_operand" " 0")
            (match_operand:SI 2 "pic30_accumulator_operand" " w")
          ] UNSPEC_MIN))
    (set (match_operand:SI   3 "pic30_reg_or_R_operand" "=rR")
         (unspec:SI [
            (match_dup 1)
            (match_dup 2)
         ] UNSPEC_MINV))
    (clobber (reg:CC_NZ CC_REG))
    ])
  ]
  "(TARGET_ISA32)"
  "min %0, %3"
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "minv_excess_32"
  [(parallel [
    (set (match_operand:SI   0 "pic30_accumulator_operand" "=w")
         (unspec:SI [
            (match_operand:SI 1 "pic30_accumulator_operand" " 0")
            (match_operand:SI 2 "pic30_accumulator_operand" " w")
          ] UNSPEC_MIN))
    (set (match_operand:SI   3 "pic30_reg_or_R_operand" "=rR")
         (unspec:SI [
            (match_dup 1)
            (match_dup 2)
         ] UNSPEC_MINV2))
    (clobber (reg:CC_NZ CC_REG))
    ])
  ]
  "(TARGET_ISA32)"
  "min.v %0, %3"
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "max_32"
  [(set (match_operand:SI   0 "pic30_accumulator_operand" "=w")
        (unspec:SI [
          (match_operand:SI 1 "pic30_accumulator_operand" " 0")
          (match_operand:SI 2 "pic30_accumulator_operand" " w")
        ] UNSPEC_MAX))
   (clobber (reg:CC_NZ CC_REG))
  ]
  "(TARGET_ISA32)"
  "max %0"
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "max_excess_32"
  [(parallel [
    (set (match_operand:SI   0 "pic30_accumulator_operand" "=w")
         (unspec:SI [
            (match_operand:SI 1 "pic30_accumulator_operand" " 0")
            (match_operand:SI 2 "pic30_accumulator_operand" " w")
          ] UNSPEC_MAX))
    (set (match_operand:SI   3 "pic30_reg_or_R_operand" "=rR")
         (unspec:SI [
            (match_dup 1)
            (match_dup 2)
         ] UNSPEC_MAXV))
    (clobber (reg:CC_NZ CC_REG))
    ])
  ]
  "(TARGET_ISA32)"
  "max %0, %3"
  [
    (set_attr "cc" "clobber")
  ]
)

(define_insn "maxv_excess_32"
  [(parallel [
    (set (match_operand:SI   0 "pic30_accumulator_operand" "=w")
         (unspec:SI [
            (match_operand:SI 1 "pic30_accumulator_operand" " 0")
            (match_operand:SI 2 "pic30_accumulator_operand" " w")
          ] UNSPEC_MAX))
    (set (match_operand:SI   3 "pic30_reg_or_R_operand" "=rR")
         (unspec:SI [
            (match_dup 1)
            (match_dup 2)
         ] UNSPEC_MAXV2))
    (clobber (reg:CC_NZ CC_REG))
    ])
  ]
  "(TARGET_ISA32)"
  "max.v %0, %3"
  [
    (set_attr "cc" "clobber")
  ]
)

