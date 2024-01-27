;;- Machine description for GNU compiler
;;- Microchip dsPIC30 version.
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
;;      This is the peephole description for the Microchip dsPIC.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;	Peephole
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;; generate btst f, #bit


; first from load, and, compare
(define_peephole
  [(set (match_operand:HI 0 "pic30_register_operand" "")
        (match_operand:HI 1 "pic30_near_operand" ""))
   (set (match_operand:HI 2 "pic30_register_operand" "")
        (and:HI 
           (match_dup 0)
           (match_operand 3 "immediate_operand" "")))
   (set (cc0)
        (compare
          (match_dup 2)
          (const_int 0)))
  ]
  "(TARGET_CLASSIC)&&
   ((pic30_one_bit_set(GET_MODE(operands[0]),operands[3],1)) &&
    (find_regno_note(prev_active_insn(insn), REG_DEAD, REGNO(operands[0])) ||
     (REGNO(operands[2]) == REGNO(operands[0])) ||
     pic30_dead_or_set_p(insn,operands[0])) &&
    (find_regno_note(insn, REG_DEAD, REGNO(operands[2])) ||
     pic30_dead_or_set_p(next_active_insn(insn),operands[2])))"
  "btst %1,#%b3"
  [(set_attr "cc" "math")]
)
 

(define_peephole
  [(set (match_operand:HI 0 "pic30_register_operand" "")
        (rotate:HI
           (match_operand:HI 1 "pic30_register_operand" "")
           (const_int 1)))
   (set (match_dup 0)
        (rotatert:HI
           (match_dup 0)
           (const_int 1)))
  ]
  ""
  ""
  [(set_attr "cc" "unchanged")]
)

(define_peephole
  [(set (match_operand:HI 0 "pic30_register_operand" "")
        (rotate:HI
           (match_operand:HI 1 "pic30_register_operand" "")
           (const_int 1)))
   (set (match_operand:HI 2 "pic30_register_operand" "")
        (rotatert:HI
           (match_dup 0)
           (const_int 1)))
  ]
  "(pic30_dead_or_set_p(NEXT_INSN(insn),operands[0]) && 
    (REGNO(operands[1]) == REGNO(operands[2])))"
  ""
  [(set_attr "cc" "unchanged")]
)

(define_peephole
  [(set (match_operand:HI 0 "pic30_register_operand" "")
        (rotate:HI
           (match_operand:HI 1 "pic30_register_operand" "")
           (const_int 1)))
   (set (match_operand:HI 2 "pic30_register_operand" "")
        (rotatert:HI
           (match_dup 0)
           (const_int 1)))
  ]
  "pic30_dead_or_set_p(NEXT_INSN(insn),operands[0])"
  "mov %1,%2"
  [(set_attr "cc" "unchanged")]
)

;; sequential assignments of 0 to a register
(define_peephole
  [
   (set (match_operand:HI 0 "pic30_register_operand" "")
        (const_int 0))
   (set (match_operand:HI 1 "pic30_register_operand" "")
        (const_int 0))
  ]
  "(TARGET_CLASSIC)&&
    ((REGNO(operands[0]) + 1 == REGNO(operands[1])) && 
    ((REGNO(operands[0]) & 1) == 0))"
  "mul.uu %0, #0, %0"
  [(set_attr "cc" "unchanged")]
)

(define_peephole
  [
   (set (match_operand:HI 0 "pic30_register_operand" "")
        (const_int 0))
   (set (match_operand:HI 1 "pic30_register_operand" "")
        (const_int 0))
  ]
  "(TARGET_CLASSIC)&&
    ((REGNO(operands[1]) + 1 == REGNO(operands[0])) && 
    ((REGNO(operands[1]) & 1) == 0))"
  "mul.uu %1, #0, %1"
  [(set_attr "cc" "unchanged")]
)


;; remove redundant lnk
(define_peephole2
  [(parallel[
      (set (reg:HI SPREG)
            (match_operand 0 "immediate_operand" ""))
      (clobber (reg:HI FPREG))
      (use (reg:HI FPREG))
      (use (reg:HI SPREG))]
  )]
  "pic30_lnk_removed(insn,1+INTVAL(operands[0]))"
  [(set (reg:HI SPREG)
        (plus:HI (reg:HI SPREG)
                 (match_dup 0)))]
  "{ operands[0] = GEN_INT(INTVAL(operands[0])+2); }"
)

(define_peephole2
  [(parallel[
     (set (reg:HI SPREG)
          (reg:HI FPREG))
     (clobber (reg:HI FPREG))]
  )]
  "pic30_lnk_removed(insn,0)"
  [(unspec_volatile [(const_int 0)] UNSPECV_NOP)]
  ""
)

;;
;; redundant copy
;;
(define_peephole2
  [
   (set (match_operand 0 "pic30_register_operand" "")
        (match_operand 1 "pic30_register_operand" ""))
   (set (match_operand 2 "pic30_register_operand" "")
        (match_operand 3 "pic30_register_operand" ""))
  ]
  "((REGNO(operands[3]) == REGNO(operands[0])) &&
    (GET_MODE(operands[3]) == GET_MODE(operands[0])) &&
    peep2_reg_dead_p(2, operands[0]))"
  [
   (set (match_dup 2)
        (match_dup 1))
  ]
  ""
)

(define_peephole2
  [
   (set (match_operand 0 "pic30_strict_register_operand" "")
        (match_operand 1 "immediate_operand" ""))
   (set (match_operand 2 "pic30_strict_register_operand" "")
        (match_operand 3 "pic30_strict_register_operand" ""))
  ]
  "((REGNO(operands[3]) == REGNO(operands[0])) &&
    (GET_MODE(operands[3]) == GET_MODE(operands[0])) &&
    peep2_reg_dead_p(2, operands[0]))"
  [
   (set (match_dup 2)
        (match_dup 1))
  ]
  ""
)
   
;;
;; clean up Ureload_outQI
;;
(define_peephole2
  [
   (set (match_operand:QI 0 "pic30_strict_register_operand" "")
        (match_operand:QI 1 "pic30_strict_register_operand" ""))
   (parallel [
     (set
        (match_operand:QI 2 "pic30_near_operand"     "")
        (match_operand:QI 3 "pic30_strict_register_operand" ""))
     (clobber
        (match_operand:HI 4 "pic30_strict_register_operand" ""))
   ])
  ]
  "((REGNO(operands[3]) == REGNO(operands[0])) &&
    (REGNO(operands[1]) != REGNO(operands[4])) &&
    peep2_reg_dead_p(2, operands[0]))"
  [(parallel [
     (set
        (match_dup 2)
        (match_dup 1))
     (clobber
        (match_dup 4))
   ])
  ]
  ""
)

  
;;
;; 16-bit shift right SI followed by truncate to HI.
;; Simplify to most-significant subreg.
;; Unsigned shift.
;;
(define_peephole2
  [(set (match_operand:SI 0            "pic30_strict_register_operand"    "")
        (lshiftrt:SI (match_operand:SI 1 "pic30_strict_register_operand"  "")
                     (match_operand 2 "immediate_operand" "")))
   (set (match_operand:HI 3 "pic30_move_operand" "")
        (match_operand:HI 4 "pic30_strict_register_operand" ""))
  ]
  "(TARGET_CLASSIC)&&
    (INTVAL(operands[2]) == 16) &&
   (REGNO(operands[0]) == REGNO(operands[4])) &&
   (((REGNO(operands[0]) <= REGNO(operands[3])) && 
     (REGNO(operands[3]) < REGNO(operands[0]) + 
       hard_regno_nregs(REGNO(operands[0]), GET_MODE (operands[0]))
     )) || peep2_reg_dead_p(2, operands[0]))"
  [(set (match_dup 3)
        (subreg:HI (match_dup 1) 2))]
  ""
  )

(define_peephole2
  [(set (match_operand:SI 0            "pic30_strict_register_operand"    "")
        (lshiftrt:SI (match_operand:SI 1 "pic30_strict_register_operand"  "")
                     (match_operand 2 "immediate_operand" "")))
   (set (match_operand:HI 3 "pic30_strict_register_operand" "")
        (match_operator:HI 6 "pic30_valid_operator"
           [(match_operand:HI 4 "pic30_strict_register_operand" "")
            (match_operand:HI 5 "" "")]))
  ]
  "(TARGET_CLASSIC)&&
    (INTVAL(operands[2]) == 16) &&
   (REGNO(operands[0]) == REGNO(operands[4])) &&
   (((REGNO(operands[0]) <= REGNO(operands[3])) &&
     (REGNO(operands[3]) < REGNO(operands[0]) + 
       hard_regno_nregs(REGNO(operands[0]), GET_MODE (operands[0]))
     )) || peep2_reg_dead_p(2, operands[0]))"
  [ 
   (set (match_dup 4) (subreg:HI (match_dup 1) 2))
   (set (match_dup 3)
        (match_op_dup 6 [(match_dup 4) (match_dup 5)]))
  ]
  ""
  )

;; Ditto for signed shift.
(define_peephole2
  [(set (match_operand:SI 0            "pic30_strict_register_operand"    "")
        (ashiftrt:SI (match_operand:SI 1 "pic30_strict_register_operand"  "")
                     (match_operand 2 "immediate_operand" "")))
   (set (match_operand:HI 3 "pic30_move_operand" "")
        (match_operand:HI 4 "pic30_strict_register_operand" ""))
  ]
  "(TARGET_CLASSIC)&&
    (INTVAL(operands[2]) == 16) &&
   (REGNO(operands[0]) == REGNO(operands[4])) &&
   (((REGNO(operands[0]) <= REGNO(operands[3])) &&
     (REGNO(operands[3]) < REGNO(operands[0]) + 
       hard_regno_nregs(REGNO(operands[0]), GET_MODE (operands[0]))
     )) || peep2_reg_dead_p(2, operands[0]))"
  [(set (match_dup 3)
        (subreg:HI (match_dup 1) 2))]
  ""
  )

(define_peephole2
  [(set (match_operand:SI 0            "pic30_strict_register_operand"    "")
        (ashiftrt:SI (match_operand:SI 1 "pic30_strict_register_operand"  "")
                     (match_operand 2 "immediate_operand" "")))
   (set (match_operand:HI 3 "pic30_strict_register_operand" "")
        (match_operator:HI 6 "pic30_valid_operator"
           [(match_operand:HI 4 "pic30_strict_register_operand" "")
            (match_operand:HI 5 "" "")]))
  ]
  "(TARGET_CLASSIC)&&
    (INTVAL(operands[2]) == 16) &&
   (REGNO(operands[0]) == REGNO(operands[4])) &&
   (((REGNO(operands[0]) <= REGNO(operands[3])) &&
     (REGNO(operands[3]) < REGNO(operands[0]) +
       hard_regno_nregs(REGNO(operands[0]), GET_MODE (operands[0]))
     )) || peep2_reg_dead_p(2, operands[0]))"
  [
   (set (match_dup 4) (subreg:HI (match_dup 1) 2))
   (set (match_dup 3)
        (match_op_dup 6 [(match_dup 4) (match_dup 5)]))
  ]
  ""
  )


;; Move [Base+Index] to [Base+Index] where Base or Index is common
(define_peephole
  [(set (match_operand:HI 0 "pic30_strict_register_operand" "=r")
	(match_operand:HI 1 "pic30_mode3_operand" "RS<>r"))
   (set (match_operand:HI 2 "pic30_mode3_operand" "=RS<>r")
	(match_dup 0))
  ]
 "pic30_IndexEqual(operands[1], operands[2]) &&
	dead_or_set_p(insn, operands[0])"
 "mov.w %1,%2"
  [(set_attr "cc" "unchanged")])

(define_peephole
  [(set (match_operand:QI 0 "pic30_strict_register_operand" "=r")
	(match_operand:QI 1 "pic30_mode3_operand" "RS<>r"))
   (set (match_operand:QI 2 "pic30_mode3_operand" "=RS<>r")
	(match_dup 0))
  ]
 "pic30_IndexEqual(operands[1], operands[2]) &&
	dead_or_set_p(insn, operands[0])"
 "mov.b %1,%2"
  [(set_attr "cc" "unchanged")])

(define_peephole
  [(set (match_operand:SI 0 "pic30_strict_register_operand" "=r")
	(match_operand:SI 1 "pic30_mode3_operand" "RS<>rl"))
   (set (match_operand:SI 2 "pic30_mode3_operand" "=RS<>rl")
	(match_dup 0))
  ]
 "(TARGET_ISA32) &&
  pic30_IndexEqual(operands[1], operands[2]) &&
	dead_or_set_p(insn, operands[0])"
 "mov.l %1,%2"
  [(set_attr "cc" "unchanged")])


;; add a,b,c;  mov [c], d => mov [a+b], d
(define_peephole2
  [(set (match_operand:HI 0 "pic30_strict_register_operand" "")
        (plus:HI (match_operand:HI 1 "pic30_strict_register_operand" "")
                 (match_operand:HI 2 "pic30_strict_register_operand" "")))
   (set (match_operand:HI 3 "pic30_strict_register_operand" "")
        (mem:HI (match_dup 0)))]
  "(TARGET_CLASSIC)&&
   ((!pic30_ecore_target()) && (!pic30_isav4_target()) && (peep2_reg_dead_p(2, operands[0]) || (REGNO(operands[0]) == REGNO(operands[3]))))"
  [(set (match_dup 3)
        (mem:HI (plus:HI (match_dup 1) (match_dup 2))))]
)

(define_peephole
 [(set (match_operand:HI 0 "pic30_strict_register_operand" "=r")
       (match_dup 0))]
 ""
 "; mov %0, %0"
)

(define_peephole
 [(set (match_operand:HI 0 "pic30_strict_register_operand" "=r")
       (match_operand:HI 1 "pic30_move_operand" "RS<>rTQ"))
  (set (match_operand:HI 2 "pic30_strict_register_operand" "=r")
       (match_dup 0))]
 "dead_or_set_p(insn, operands[0])"
 "mov.w %1, %2"
)

(define_peephole
 [(set (match_operand:SI 0 "pic30_strict_register_operand" "=r")
       (match_operand:SI 1 "pic30_move_operand" "RS<>rTQ"))
  (set (match_operand:SI 2 "pic30_strict_register_operand" "=r")
       (match_dup 0))]
 "(TARGET_ISA32)&&
  dead_or_set_p(insn, operands[0])"
 "mov.l %1, %2"
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; GCC often loads a function parameter into an arbitrary register,
;; then moves that register to one appropriate for the function call.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;; mov #addr,r0; mov r0,r1 becomes mov #addr,r1

(define_peephole
  [(set (match_operand:HI 0 "pic30_strict_register_operand"        "=r")
        (match_operand:HI 1 "pic30_symbolic_address_operand" "qs"))
   (set (match_operand:HI 2 "pic30_strict_register_operand"        "=r")
        (match_dup 0))]
 "(TARGET_CLASSIC)&&
  dead_or_set_p(insn, operands[0])"
  "*
{ rtx sym;

  sym = pic30_program_space_operand_p(operands[1]);
  if (sym) {
    tree fndecl = SYMBOL_REF_DECL(sym);
    tree fndecl_attrib;
    const char *access=0;
    int slot = 0;

    if ((fndecl_attrib =pic30_lookup_attribute(IDENTIFIER_POINTER(pic30_identBoot[0]),
                                         DECL_ATTRIBUTES(fndecl)))) {
      access=\"boot\";
    } else if ((fndecl_attrib = pic30_lookup_attribute(
                                IDENTIFIER_POINTER(pic30_identSecure[0]),
                                DECL_ATTRIBUTES(fndecl)))) {
      access=\"secure\";
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
      sprintf(buffer,\"mov #%s(%d),%%2\", access, slot);
      return buffer;
    } else {
      return \"mov #handle(%1),%2\";
    }
  } else {
    return \"mov #%1,%2\";
  }
}"
  [(set_attr "cc" "clobber")])

;; mov.d [wn+k],r0; mov.d r0,r1 becomes mov.d [wn+k],r1

(define_peephole
  [(set (match_operand:GM32BIT 0 "pic30_strict_register_operand" "=r")
        (match_operand:GM32BIT 1 "pic30_Q_operand"   "Q"))
   (set (match_operand:GM32BIT 2 "pic30_strict_register_operand" "=r")
        (match_dup 0))
  ]
 "(TARGET_CLASSIC)&&
  dead_or_set_p(insn, operands[0])"
 "*
{
	int idSrc, idDst;

	idDst = REGNO(operands[2]);
	idSrc = REGNO(XEXP(XEXP(operands[1],0),0));
  if (pic30_psrd_psrd_errata(NULL,operands[1])) {
    pic30_rtx_nops++;
    if (idDst == idSrc) {
      return \"mov %Q1,%d2\;nop\;mov %1,%2\";
    } else {
      return \"mov %1,%2\;nop\;mov %Q1,%d2\";
	}
  } else {
    if (idDst == idSrc) {
		return \"mov %Q1,%d2\;mov %1,%2\";
    } else {
		return \"mov %1,%2\;mov %Q1,%d2\";
	}
  }
}"
  [(set_attr "cc" "clobber")])

;(define_peephole
;  [(set (match_operand:SI 0 "pic30_strict_register_operand" "=r")
;        (match_operand:SI 1 "pic30_Q_operand"   "Q"))
;   (set (match_operand:SI 2 "pic30_strict_register_operand" "=r")
;        (match_dup 0))
;  ]
; "dead_or_set_p(insn, operands[0])"
; "*
;{
;  int idSrc, idDst;
;
;  idDst = REGNO(operands[2]);
;  idSrc = REGNO(XEXP(XEXP(operands[1],0),0));
;  if (pic30_psrd_psrd_errata(NULL,operands[1])) {
;    if (idDst == idSrc) {
;    pic30_rtx_nops++;
;      return \"mov %Q1,%d2\;nop\;mov %1,%2\";
;    } else {
;     return \"mov %1,%2\;nop\;mov %Q1,%d2\";
;    }
;  } else {
;    if (idDst == idSrc) {
;      return \"mov %Q1,%d2\;mov %1,%2\";
;    } else {
;     return \"mov %1,%2\;mov %Q1,%d2\";
;    }
;  }
;}"
;  [(set_attr "cc" "clobber")])

;; mov.d a,r0; mov.d r0,r1 becomes mov.d a,r1

(define_peephole
  [(set (match_operand:GM32BIT 0 "pic30_strict_register_operand" "=r")
        (match_operand:GM32BIT 1 "pic30_T_operand"   "T"))
   (set (match_operand:GM32BIT 2 "pic30_strict_register_operand" "=r")
        (match_dup 0))
  ]
 "(TARGET_CLASSIC)&&
  dead_or_set_p(insn, operands[0])"
 "*
{
  if (pic30_psrd_psrd_errata(NULL,operands[1])) {
    pic30_rtx_nops++;
    return \"mov %1,%2\;nop\;mov %Q1,%d2\";
  } else {
	return \"mov %1,%2\;mov %Q1,%d2\";
  }
}"
  [(set_attr "cc" "clobber")])

;(define_peephole
;  [(set (match_operand:SI 0 "pic30_strict_register_operand" "=r")
;        (match_operand:SI 1 "pic30_T_operand"   "T"))
;   (set (match_operand:SI 2 "pic30_strict_register_operand" "=r")
;        (match_dup 0))
;  ]
; "dead_or_set_p(insn, operands[0])"
; "*
;{
;	return \"mov %1,%2\;mov %Q1,%d2\";
;}"
;  [(set_attr "cc" "clobber")])

;; mov.q #k,r0; mov.q r0,r1 becomes mov.q #k,r1

(define_peephole
  [(set (match_operand:DF 0 "pic30_strict_register_operand" "=r,r")
        (match_operand:DF 1 "immediate_operand" "G,i"))
   (set (match_operand:DF 2 "pic30_strict_register_operand" "=r,r")
        (match_dup 0))
  ]
 "(TARGET_CLASSIC)&&
  dead_or_set_p(insn, operands[0])"
 "*
  {
    REAL_VALUE_TYPE r;
    long l[4] = { 0 };

    switch (which_alternative) {
      case 0:
        return \"mul.uu %2,#0,%2\;\"
               \"mul.uu %t2,#0,%t2\";
      case 1:
        r = *CONST_DOUBLE_REAL_VALUE(operands[1]);
        REAL_VALUE_TO_TARGET_DOUBLE(r, l);
        if (l[0] == 0) {
          return \"mul.uu %0,#0,%2\;\"
                 \"mov #%x1,%t2\;\"
                 \"mov #%w1,%q2\";
        } else {
          return \"mov #%z1,%2\;\"
                 \"mov #%y1,%d2\;\"
                 \"mov #%x1,%t2\;\"
                 \"mov #%w1,%q2\";
        }
      default: gcc_assert(0);
    }
   }"
  [
    (set_attr "cc" "clobber")
  ]
)

;; mov.q [wn+k],r0; mov.q r0,r1 becomes mov.q [wn+k],r1

(define_peephole
  [(set (match_operand:DF 0 "pic30_strict_register_operand" "=r")
        (match_operand:DF 1 "pic30_Q_operand"   "Q"))
   (set (match_operand:DF 2 "pic30_strict_register_operand" "=r")
        (match_dup 0))
  ]
 "(TARGET_CLASSIC)&&
  dead_or_set_p(insn, operands[0])"
 "*
{
  int idSrc, idDst;
  char temp[53];
  char save[53];
  static char szInsn[53];

  szInsn[0] = 0;
  temp[0] = 0;
  save[0] = 0;

  idDst = REGNO(operands[2]);
  idSrc = REGNO(XEXP(XEXP(operands[1],0),0));
  strcpy(temp, \"mov %1,%2\;\");
  if (idDst != idSrc)
    strcat(szInsn, temp);
  else
    strcat(save, temp);
  idDst++;
  strcpy(temp, \"mov %Q1,%d2\;\");
  if (idDst != idSrc)
    strcat(szInsn, temp);
  else
    strcat(save, temp);
  idDst++;
  strcpy(temp, \"mov %R1,%t2\;\");
  if (idDst != idSrc)
    strcat(szInsn, temp);
  else
    strcat(save, temp);
  idDst++;
  strcpy(temp, \"mov %S1,%q2\;\");
  if (idDst != idSrc)
    strcat(szInsn, temp);
  else
    strcat(save, temp);
  idDst++;
  if (save[0]) {
    save[strlen(save)-2] = 0;
    strcat(szInsn, save);
  }
  return szInsn;
}"
  [(set_attr "cc" "clobber")])

;;
;; Substitute RETLW #k,Wn for MOV #k,Wn; RETURN
;;
(define_peephole
  [(set (match_operand:QI 0 "pic30_strict_register_operand" "=r")
        (match_operand:QI 1 "pic30_J_operand"   "J"))
   (return)
  ]
 "pic30_null_epilogue_p(1)"
 "*
{
        pic30_set_function_return(TRUE);
        if (REGNO(operands[0]) == WR0_REGNO)
        {
                return \"retlw #%1,%0\";
        }
        else
        {
                return \"mov.b #%1,%0\;return\";
        }
}"
  [(set_attr "cc" "clobber")])


(define_peephole
  [(set (match_operand:HI 0 "pic30_strict_register_operand" "=r")
	(match_operand:HI 1 "pic30_J_operand"   "J"))
   (return)
  ]
 "pic30_null_epilogue_p(1)"
 "*
{
	pic30_set_function_return(TRUE);
	if (REGNO(operands[0]) == WR0_REGNO)
	{
		return \"retlw #%1,%0\";
	}
	else
	{
		return \"mov #%1,%0\;return\";
	}
}"
  [(set_attr "cc" "clobber")])

(define_peephole
  [(set (match_operand:SI 0 "pic30_strict_register_operand" "=r")
	(match_operand:SI 1 "pic30_J_operand"   "J"))
   (return)
  ]
 "(TARGET_ISA32)&&
  pic30_null_epilogue_p(1)"
 "*
{
	pic30_set_function_return(TRUE);
	if (REGNO(operands[0]) == WR0_REGNO)
	{
		return \"retlw.l #%1,%0\";
	}
	else
	{
		return \"mov.sl #%1,%0\;return\";
	}
}"
  [(set_attr "cc" "clobber")])

(define_peephole
  [(set (match_operand:SI 0 "pic30_strict_register_operand" "=r")
	(match_operand:SI 1 "pic30_J_operand"   "J"))
   (return)
  ]
 "(TARGET_CLASSIC)&&
   pic30_null_epilogue_p(1)"
 "*
{
	pic30_set_function_return(TRUE);
	if (REGNO(operands[0]) == WR0_REGNO)
	{
		return \"mov #0,%d0\;retlw #%1,%0\";
	}
	else
	{
		return \"mov #0,%d0\;mov #%1,%0\;return\";
	}
}"
  [(set_attr "cc" "clobber")])

;; Substitute bra/goto f for rcall/call f; ret

(define_peephole
  [(call (match_operand:QI 0 "memory_operand" "R,mp")
         (match_operand:HI 1 "pic30_general_operand" ""))
   (return)
  ]
  "pic30_null_epilogue_p(1) /* a */"
  "*
{
	pic30_set_function_return(TRUE);
	switch (which_alternative)
	{
	static char szInsn[48];
	case 0:
		sprintf(szInsn, \"goto %s\",
				reg_names[REGNO(XEXP(operands[0],0))]);
		return szInsn;
	case 1:
		if (pic30_near_function_p(operands[0]))
			return \"bra %0\";
		else
			return \"goto %0\";
	default:
		gcc_assert(0);
	}
}")

(define_peephole
  [(set (match_operand 0 "pic30_strict_register_operand"         "rl,rl")
        (call (match_operand:QI 1 "memory_operand"  "R,mp")
              (match_operand:HI 2 "pic30_general_operand" "")))
   (return)
  ]
  "pic30_null_epilogue_p(1)"
  "*
{
	pic30_set_function_return(TRUE);
	switch (which_alternative)
	{
	static char szInsn[48];
	case 0:
		sprintf(szInsn, \"goto %s\",
				reg_names[REGNO(XEXP(operands[1],0))]);
		return szInsn;
	case 1:
		if (pic30_near_function_p(operands[1]))
			return \"bra %1\";
		else
			return \"goto %1\";
	default:
		gcc_assert(0);
	}
}")

;; Combine mov.w pairs to mov.d

(define_peephole
  [(set (match_operand:HI 0 "pic30_strict_register_operand" "=r")
        (match_operand:HI 1 "pic30_strict_register_operand"  "r"))
   (set (match_operand:HI 2 "pic30_strict_register_operand" "=r")
        (match_operand:HI 3 "pic30_strict_register_operand"  "r"))
  ]
  "(TARGET_CLASSIC)&&
   pic30_registerpairs_p(operands[0],operands[2],operands[1],operands[3])"
  "*
{
    if (REGNO(operands[0]) < REGNO(operands[2])) {
		return \"mov.d %1,%0\";
    } else {
		return \"mov.d %3,%2\";
	}
}"
  [(set_attr "cc" "clobber")])
  
;; Combine mov.w [Wn],Wm; mov.w [Wn+2],Wm+1 to mov.d [Wn],Wm

(define_peephole
  [(set (match_operand:HI 0 "pic30_strict_register_operand" "=r")
        (match_operand:HI 1 "pic30_R_operand"   "R"))
   (set (match_operand:HI 2 "pic30_strict_register_operand" "=r")
        (match_operand:HI 3 "pic30_Q_operand"   "Q"))
  ]
  "(TARGET_CLASSIC)&&
   (IS_EVEN_REG(REGNO(operands[0]))) &&
   (pic30_Q_base(operands[3]) == REGNO(operands[1])) &&
   (pic30_Q_displacement(operands[3]) == 2) &&
    (pic30_psrd_psrd_errata_movd(NULL,operands[1]) == 0)"
  "mov.d %1,%0"
  [(set_attr "cc" "clobber")])

;; Zero-extend followed by shift

(define_peephole
  [(set (match_operand:SI 0 "pic30_strict_register_operand"                  "=r")
        (zero_extend:SI (match_operand:QI 1 "pic30_strict_register_operand"   "r")) )
   (set (match_dup 0)
        (ashift:SI (match_dup 0)
                   (match_operand:HI 2 "pic30_imm16plus_operand" "i")))
  ]
  "(TARGET_CLASSIC)"
  "*
{
    int n = INTVAL(operands[2]);
    if (n == 16) {
      return \"ze %1,%d0\;mov #0,%0\";
    } else if (n >= 24) {
      return \"sl %1,#%K2,%d0\;mov #0,%0\";
    } else {
      return \"ze %1,%0\;sl %0,#%K2,%d0\;mov #0,%0\";
    }
}"
  [(set_attr "cc" "clobber")])


;
;  CAW - I am not convinced this is safe
;
(define_peephole2
  [(set (match_operand:HI 0 "pic30_strict_register_operand" "")
        (match_operand:HI 1 "pic30_near_operand"  "") )
   (set (match_operand:SI 2 "pic30_wreg_operand" "")
        (mult:SI (zero_extend:SI (match_dup 0))
                 (zero_extend:SI (match_operand 3 "immediate_operand"   ""))))]
  "0&&
   ((INTVAL(operands[3]) == 2) && (peep2_reg_dead_p(2, operands[0]) || (REGNO(operands[0]) == REGNO(operands[2]))))"
  [(set (subreg:HI (match_dup 2) 0)
        (ashift:HI (match_dup 1)
                   (const_int 1)))]
  ""
  )

;
;  CAW - I am not convinced this is safe
;
(define_peephole2
  [(set (match_operand:HI 0 "pic30_strict_register_operand" "")
        (match_operand:HI 1 "pic30_near_operand"  "") )
   (set (match_operand:SI 2 "pic30_strict_register_operand" "")
        (mult:SI (zero_extend:SI (match_dup 0))
                 (match_operand:SI 3 "immediate_operand"   "")))
   (set (match_operand:HI 4 "pic30_wreg_operand" "" )
        (match_operand:HI 5 "pic30_strict_register_operand" ""))
]
  "0&&
   ((INTVAL(operands[3]) == 2) && (REGNO(operands[2]) == REGNO(operands[5])) &&     (peep2_reg_dead_p(3, operands[0]) || (REGNO(operands[0]) == REGNO(operands[4]))))"
  [(set (subreg:HI (match_dup 2) 0)
        (ashift:HI (match_dup 1)
                   (const_int 1)))]
  ""
  )

(define_peephole2
  [(match_scratch:HI 5 "a") 
   (set (match_operand:HI 0 "pic30_strict_register_operand" "")
        (match_operand:HI 1 "pic30_near_operand"  ""))
   (set (match_operand:SI 2 "pic30_strict_register_operand" "")
        (mult:SI (zero_extend:SI (match_dup 0))
                 (match_operand:SI 3 "immediate_operand"   "")))
   (set (match_operand:HI 6 "pic30_strict_register_operand" "")
        (plus:HI (match_operand:HI 7 "pic30_strict_register_operand" "")
                 (match_operand:HI 4 "pic30_math_operand" "")))]
  "(TARGET_CLASSIC)&&
   ((INTVAL(operands[3]) == 2) && (REGNO(operands[7]) == REGNO(operands[2])) &&
    (peep2_reg_dead_p(3, operands[0]) ||
      (REGNO(operands[0]) == REGNO(operands[6])) ||
      (REGNO(operands[0]) == REGNO(operands[2]))) &&
    (peep2_reg_dead_p(3, operands[2]) ||
      (REGNO(operands[2]) == REGNO(operands[6]))))"
  [(set (match_dup 5)
        (ashift:HI (match_dup 1)
                   (const_int 1)))
   (set (match_dup 6) 
        (plus:HI (match_dup 4) (match_dup 5)))]
  ""
  )

(define_peephole2
  [(match_scratch:HI 5 "a") 
   (set (match_operand:HI 0 "pic30_strict_register_operand" "")
        (match_operand:HI 1 "pic30_near_operand"  ""))
   (set (match_operand:SI 2 "pic30_strict_register_operand" "")
        (mult:SI (zero_extend:SI (match_dup 0))
                 (match_operand:SI 3 "immediate_operand"   "")))
   (set (match_operand:HI 6 "pic30_strict_register_operand" "")
        (plus:HI (match_operand:HI 4 "pic30_math_operand" "")
                 (match_operand:HI 7 "pic30_strict_register_operand" "")))]
  "(TARGET_CLASSIC)&&
   ((INTVAL(operands[3]) == 2) && (REGNO(operands[7]) == REGNO(operands[2])) &&
    (peep2_reg_dead_p(3, operands[0]) ||
      (REGNO(operands[0]) == REGNO(operands[6])) ||
      (REGNO(operands[0]) == REGNO(operands[2]))) &&
    (peep2_reg_dead_p(3, operands[2]) ||
      (REGNO(operands[2]) == REGNO(operands[6]))))"
  [(set (match_dup 5)
        (ashift:HI (match_dup 1)
                   (const_int 1)))
   (set (match_dup 6) 
        (plus:HI (match_dup 4) (match_dup 5)))]
  ""
  )

;
;  CAW - I am not convinced this is safe
;
(define_peephole
  [(set (match_operand:SI 0 "pic30_strict_register_operand"                "=r")
        (zero_extend:SI (match_operand:QI 1 "pic30_strict_register_operand" "r")) )
   (set (match_dup 0)
        (ashift:SI (match_dup 0)
                   (match_operand:HI 2 "pic30_imm8_operand"    "i")))
  ]
  "0"
  "*
   {
     return \"sl %1,#%2,%0\;\"
            \"mov #0,%d0\";
   }"
  [
    (set_attr "cc" "clobber")
  ]
)

;; bit set optimizations

(define_peephole
  [(set (match_operand:SI 0 "pic30_strict_register_operand" "=r")
	(match_operand:SI 1 "pic30_R_operand"   "R"))
   (set (match_dup 0)
        (ior:SI  (match_dup 0)
                 (match_operand:SI 2 "const_int_operand"   "i")))
   (set (match_dup 0)
        (ior:SI  (match_dup 0)
                 (match_operand:SI 3 "const_int_operand"   "i")))
   (set (match_dup 1)
	(match_dup 0))
  ]
 "dead_or_set_p(insn, operands[0]) &&
  (pic30_one_bit_set(GET_MODE(operands[0]),operands[2],1)) &&
  (pic30_which_bit(INTVAL(operands[2]))<16) &&
  (pic30_one_bit_set(GET_MODE(operands[0]),operands[3],1)) &&
  (pic30_which_bit(INTVAL(operands[2]))<16)"
 "bset %1,#%b2\;bset %1,#%b3"
  [(set_attr "cc" "clobber")])

;; Improve mov.b Wd,sfr

(define_peephole
  [(set (match_operand:QI 0 "pic30_near_operand" "=U,U")
	(match_operand:QI 1 "pic30_strict_register_operand"    "d,a"))
   (set (match_operand:QI 2 "pic30_near_operand" "=U,U")
	(match_dup 1))
   (set (match_operand:QI 3 "pic30_near_operand" "=U,U")
	(match_dup 1))
   (set (match_operand:QI 4 "pic30_near_operand" "=U,U")
	(match_dup 1))
   (set (match_operand:QI 5 "pic30_near_operand" "=U,U")
	(match_dup 1))
  ]
 "(TARGET_CLASSIC)&&
  !(pic30_errata_mask & exch_errata)"
 "@
  exch w0,%1\;mov.b WREG,%0\;mov.b WREG,%2\;mov.b WREG,%3\;mov.b WREG,%4\;mov.b WREG,%5\;exch w0,%1
  mov.b WREG,%0\;mov.b WREG,%2\;mov.b WREG,%3\;mov.b WREG,%4\;mov.b WREG,%5"
  [(set_attr "cc" "unchanged")])

(define_peephole
  [(set (match_operand:QI 0 "pic30_near_operand" "=U,U")
	(match_operand:QI 1 "pic30_strict_register_operand"    "d,a"))
   (set (match_operand:QI 2 "pic30_near_operand" "=U,U")
	(match_dup 1))
   (set (match_operand:QI 3 "pic30_near_operand" "=U,U")
	(match_dup 1))
   (set (match_operand:QI 4 "pic30_near_operand" "=U,U")
	(match_dup 1))
  ]
 "(TARGET_CLASSIC)&&
  !(pic30_errata_mask & exch_errata)"
 "@
  exch w0,%1\;mov.b WREG,%0\;mov.b WREG,%2\;mov.b WREG,%3\;mov.b WREG,%4\;exch w0,%1
  mov.b WREG,%0\;mov.b WREG,%2\;mov.b WREG,%3\;mov.b WREG,%4"
  [(set_attr "cc" "unchanged")])

(define_peephole
  [(set (match_operand:QI 0 "pic30_near_operand" "=U,U")
	(match_operand:QI 1 "pic30_strict_register_operand"    "d,a"))
   (set (match_operand:QI 2 "pic30_near_operand" "=U,U")
	(match_dup 1))
   (set (match_operand:QI 3 "pic30_near_operand" "=U,U")
	(match_dup 1))
  ]
 "(TARGET_CLASSIC)&&
  !(pic30_errata_mask & exch_errata)"
 "@
  exch w0,%1\;mov.b WREG,%0\;mov.b WREG,%2\;mov.b WREG,%3\;exch w0,%1
  mov.b WREG,%0\;mov.b WREG,%2\;mov.b WREG,%3"
  [(set_attr "cc" "unchanged")])

(define_peephole
  [(set (match_operand:QI 0 "pic30_near_operand" "=U,U")
	(match_operand:QI 1 "pic30_strict_register_operand"    "d,a"))
   (set (match_operand:QI 2 "pic30_near_operand" "=U,U")
	(match_dup 1))
  ]
 "(TARGET_CLASSIC)&&
  !(pic30_errata_mask & exch_errata)"
 "@
  exch w0,%1\;mov.b WREG,%0\;mov.b WREG,%2\;exch w0,%1
  mov.b WREG,%0\;mov.b WREG,%2"
  [(set_attr "cc" "unchanged")])

;
;  mov val, reg
;  mov reg, reg2
;
;  -> mov val, reg2
;
;  don't know why we need them, seems the register allocator should do better
;

(define_peephole2
  [(set (match_operand:QI 0 "pic30_strict_register_operand" "")
        (match_operand:QI 1 "immediate_operand" ""))
   (set (match_operand:QI 2 "pic30_strict_register_operand" "")
        (match_dup 0))]
  "peep2_reg_dead_p(2,operands[0])"
  [(set (match_dup 2)
        (match_dup 1))]
  ""
)

(define_peephole
  [(set (match_operand:QI 0 "pic30_strict_register_operand" "=r")
        (match_operand:QI 1 "immediate_operand" "i"))
   (set (match_operand:QI 2 "pic30_strict_register_operand" "=r")
        (match_dup 0))]
  "dead_or_set_p(insn,operands[0])"
  "mov.b #%1,%2"
  [(set_attr "cc" "unchanged")])

(define_peephole
  [(set (match_operand:QI 0 "pic30_strict_register_operand" "=r")
        (match_operand:QI 1 "pic30_near_operand" "U"))
   (set (match_operand:QI 2 "pic30_strict_register_operand" "=r")
        (match_dup 0))]
  "(TARGET_CLASSIC)&&
    dead_or_set_p(insn,operands[0])"
  "mov #%1,%2\;mov.b [%2],%2"
  [(set_attr "cc" "unchanged")])

; CAW - might not be needed for 33a
(define_peephole
  [(set (match_operand:QI 0 "pic30_strict_register_operand" "=r")
        (match_operand:QI 1 "pic30_near_operand" "U"))
   (set (match_operand:QI 2 "pic30_strict_register_operand" "=r")
        (match_dup 0))]
  "(TARGET_ISA32)&&
    dead_or_set_p(insn,operands[0])"
  "mov.l #%1,%2\;mov.b [%2],%2"
  [(set_attr "cc" "unchanged")])

(define_peephole
  [(set (match_operand:QI 0 "pic30_strict_register_operand" "=r")
        (match_operand:QI 1 "pic30_general_operand" "g"))
   (set (match_operand:QI 2 "pic30_strict_register_operand" "=r")
        (match_dup 0))]
  "dead_or_set_p(insn,operands[0])"
  "mov.b %1,%2"
  [(set_attr "cc" "unchanged")])
 
(define_peephole
  [(set (match_operand:HI 0 "pic30_strict_register_operand" "=r")
        (match_operand:HI 1 "immediate_operand" "i"))
   (set (match_operand:HI 2 "pic30_strict_register_operand" "=r")
        (match_dup 0))]
  "dead_or_set_p(insn,operands[0])"
  "mov.w #%1,%2"
  [(set_attr "cc" "unchanged")])

(define_peephole
  [(set (match_operand:SI 0 "pic30_strict_register_operand" "=r")
        (match_operand:SI 1 "immediate_operand" "i"))
   (set (match_operand:SI 2 "pic30_strict_register_operand" "=r")
        (match_dup 0))]
  "(TARGET_ISA32)&&
   dead_or_set_p(insn,operands[0])"
  "mov.l #%1,%2"
  [(set_attr "cc" "unchanged")])

(define_peephole
  [(set (match_operand:HI 0 "pic30_strict_register_operand" "=r")
        (match_operand:HI 1 "pic30_general_operand" "g"))
   (set (match_operand:HI 2 "pic30_strict_register_operand" "=r")
        (match_dup 0))]
  "dead_or_set_p(insn,operands[0])"
  "mov.w %1,%2"
  [(set_attr "cc" "unchanged")])

(define_peephole
  [(set (match_operand:HI 0 "register_operand" "=r")
        (match_operand:HI 1 "general_operand" "g"))
   (set (match_dup 1)
        (match_dup 0))]
  "dead_or_set_p(insn,operands[0])"
  "; move deleted"
  [(set_attr "cc" "unchanged")])

(define_peephole
  [(set (match_operand:QI 0 "pic30_strict_register_operand" "=r")
        (match_operand:QI 1 "pic30_general_operand" "g"))
   (set (match_dup 1)
        (match_dup 0))]
  "pic30_dead_or_set_p(NEXT_INSN(insn),operands[0])"
  "; move deleted"
  [(set_attr "cc" "unchanged")])

(define_peephole
  [(set (match_operand:SI 0 "pic30_strict_register_operand" "=r")
        (match_operand:SI 1 "pic30_general_operand" "g"))
   (set (match_dup 1)
        (match_dup 0))]
  "pic30_dead_or_set_p(NEXT_INSN(insn),operands[0])"
  "; move deleted"
  [(set_attr "cc" "unchanged")])

(define_peephole
  [(set (match_operand:DI 0 "pic30_strict_register_operand" "=r")
        (match_operand:DI 1 "pic30_general_operand" "g"))
   (set (match_dup 1)
        (match_dup 0))]
  "pic30_dead_or_set_p(NEXT_INSN(insn),operands[0])"
  "; move deleted"
  [(set_attr "cc" "unchanged")])

(define_peephole
  [(set (match_operand:SF 0 "pic30_strict_register_operand" "=r")
        (match_operand:SF 1 "pic30_general_operand" "g"))
   (set (match_dup 1)
        (match_dup 0))]
  "pic30_dead_or_set_p(NEXT_INSN(insn),operands[0])"
  "; move deleted"
  [(set_attr "cc" "unchanged")])

(define_peephole
  [(set (match_operand:DF 0 "pic30_strict_register_operand" "=r")
        (match_operand:DF 1 "pic30_general_operand" "g"))
   (set (match_dup 1)
        (match_dup 0))]
  "pic30_dead_or_set_p(NEXT_INSN(insn),operands[0])"
  "; move deleted"
  [(set_attr "cc" "unchanged")])

;
; some general const int optimizations
;

(define_peephole2
 [(set (match_operand:HI 0 "pic30_strict_register_operand" "")
       (match_operand:HI 1 "immediate_operand" ""))
  (set (match_operand:HI 2 "pic30_near_operand" "")
       (match_dup 0))]
 "(TARGET_CLASSIC)&&
   ((INTVAL(operands[1]) == 255) && 
   (!pic30_volatile_operand(operands[2],HImode)) &&
   (peep2_regno_dead_p(2, REGNO(operands[0]))))"
 [(set (match_dup 2) (const_int 0))
  (set (subreg:QI (match_dup 2) 0) (const_int -1))]
 ""
)

(define_peephole2
 [(set (match_operand:HI 0 "pic30_strict_register_operand" "")
       (match_operand:HI 1 "immediate_operand" ""))
  (set (match_operand:HI 2 "pic30_near_operand" "")
       (match_dup 0))]
 "(TARGET_CLASSIC)&&
   ((INTVAL(operands[1]) == -256) && 
   (!pic30_volatile_operand(operands[2],HImode)) &&
   (peep2_regno_dead_p(2, REGNO(operands[0]))))"
 [(set (match_dup 2) (const_int -1))
  (set (subreg:QI (match_dup 2) 0) (const_int 0))]
 ""
)

;
; add become subtract
;
(define_peephole2
 [(set (match_operand:HI 0 "pic30_strict_register_operand" "")
       (match_operand:HI 1 "immediate_operand" ""))
  (set (match_operand:HI 2 "pic30_strict_register_operand" "")
       (plus:HI (match_dup 2)
                (match_dup 0)))]
 "((INTVAL(operands[1]) <= 0) && (INTVAL(operands[1]) >= -1023) && peep2_regno_dead_p(2,REGNO(operands[0])))"
 [ (set (match_dup 2)
        (minus:HI (match_dup 2) (match_dup 1)))]
 "{ operands[1] = gen_rtx_CONST_INT(HImode,-INTVAL(operands[1])); }"
)

(define_peephole2
 [(set (match_operand:HI 0 "pic30_strict_register_operand" "")
       (match_operand:HI 1 "immediate_operand" ""))
  (set (match_operand:HI 2 "pic30_strict_register_operand" "")
       (plus:HI (match_operand:HI 3 "pic30_strict_register_operand" "")
                (match_dup 0)))]
 "((INTVAL(operands[1]) <= 0) && (INTVAL(operands[1]) >= -31) && peep2_regno_dead_p(2,REGNO(operands[0])))"
 [ (set (match_dup 2)
        (minus:HI (match_dup 3) (match_dup 1)))]
 "{ operands[1] = gen_rtx_CONST_INT(HImode,-INTVAL(operands[1])); }"
)

;
; combining two HI's into an SI
;

(define_peephole2
 [(set (match_operand:SI 0 "pic30_strict_register_operand" "")
       (zero_extend:SI (match_operand:HI 1 "pic30_strict_register_operand" "")))
  (set (match_operand:SI 2 "pic30_strict_register_operand" "")
       (ashift:SI (match_dup 0) (const_int 16)))
  (set (match_operand:SI 3 "pic30_strict_register_operand" "")
       (zero_extend:SI (match_operand:HI 4 "pic30_strict_register_operand" "")))
  (set (match_operand:SI 5 "pic30_strict_register_operand" "")
       (ior:SI (match_dup 2)
               (match_dup 3)))]
 "((REGNO(operands[5]) != REGNO(operands[1])) && pic30_dead_or_set_p(peep2_next_insn(4), operands[2]) && pic30_dead_or_set_p(peep2_next_insn(4),operands[3]))"
 [(set (match_dup 5) (match_dup 4))
  (set (match_dup 0) (match_dup 1))]
 "{ int reg = REGNO(operands[5]);
    operands[5] = gen_rtx_REG(HImode,reg);
    operands[0] = gen_rtx_REG(HImode,reg+1); }"
)

; [(set (subreg:HI (match_dup 5) 0) (match_dup 4))
;  (set (subreg:HI (match_dup 5) 2) (match_dup 1))]

(define_peephole2
 [(set (match_operand:SI 0 "pic30_strict_register_operand" "")
       (zero_extend:SI (match_operand:HI 1 "pic30_strict_register_operand" "")))
  (set (match_operand:SI 2 "pic30_strict_register_operand" "")
       (ashift:SI (match_dup 0) (const_int 16)))
  (set (match_operand:SI 3 "pic30_strict_register_operand" "")
       (zero_extend:SI (match_operand:HI 4 "pic30_strict_register_operand" "")))
  (set (match_operand:SI 5 "pic30_strict_register_operand" "")
       (ior:SI (match_dup 2)
               (match_dup 3)))]
 "(((REGNO(operands[5])+1) != REGNO(operands[4])) && pic30_dead_or_set_p(peep2_next_insn(4), operands[2]) && pic30_dead_or_set_p(peep2_next_insn(4),operands[3]))"
 [(set (match_dup 5) (match_dup 1))
  (set (match_dup 0) (match_dup 4))]
 "{ int reg = REGNO(operands[5]);
    operands[5] = gen_rtx_REG(HImode,reg+1);
    operands[0] = gen_rtx_REG(HImode,reg); }"
)

(define_peephole2
 [(set (match_operand:SI 0 "pic30_strict_register_operand" "")
       (zero_extend:SI (match_operand:HI 1 "pic30_strict_register_operand" "")))
  (set (match_operand:SI 2 "pic30_strict_register_operand" "")
       (ashift:SI (match_dup 0) (const_int 16)))
  (set (match_operand:SI 3 "pic30_strict_register_operand" "")
       (zero_extend:SI (match_operand:HI 4 "pic30_strict_register_operand" "")))
  (set (match_operand:SI 5 "pic30_strict_register_operand" "")
       (ior:SI (match_dup 2)
               (match_dup 3)))]
 "(REGNO(operands[0])+1 != REGNO(operands[4]))"
 [
  (set (match_dup 2) (match_dup 1))
  (set (match_dup 3) (match_dup 4))
  (set (match_dup 5) (match_dup 0))
 ] 
 "{ int reg = REGNO(operands[0]);
    operands[2] = gen_rtx_REG(HImode, reg+1);
    operands[3] = gen_rtx_REG(HImode, reg);  }"
)

(define_peephole2
 [(set (match_operand:SI 0 "pic30_strict_register_operand" "")
       (match_operand:SI 1 "pic30_move_operand" ""))
  (set (match_operand:SI 2 "pic30_strict_register_operand" "")
       (match_dup 0))]
 "peep2_reg_dead_p(2, operands[0])"
 [(set (match_dup 2) (match_dup 1))]
)

(define_peephole2
 [(set (match_operand:SI 0 "pic30_strict_register_operand" "")
       (match_operand:SI 1 "pic30_strict_register_operand" ""))
  (set (match_operand:SI 2 "pic30_move_operand" "")
       (match_dup 0))]
 "peep2_reg_dead_p(2, operands[0])"
 [(set (match_dup 2) (match_dup 1))]
)

(define_peephole2
 [(set (match_operand:SI 0 "pic30_move_operand" "")
       (match_operand:SI 1 "pic30_strict_register_operand" ""))
  (set (match_operand:SI 2 "pic30_strict_register_operand" "")
       (match_dup 0))]
 "peep2_reg_dead_p(2, operands[1])"
 [(set (match_dup 0) (match_dup 1))
  (set (match_dup 2) (match_dup 1))]
)


; pic30_isa32
;
; (set (reg:HI x (const_int )))
; (set (reg:HI (and:HI (reg:HI x)
;                      (reg:HI))))
; ->andlw 

(define_peephole
 [(set (match_operand:HI 0 "pic30_strict_register_operand" "")
       (match_operand:HI 1 "immediate_operand" ""))
  (set (match_operand:HI 2 "pic30_strict_register_operand" "")
       (and:HI
          (match_dup 2)
          (match_dup 0)))
 ]
 "(pic30_isa32_target() && 
   dead_or_set_p(insn,operands[0]))"
 "and.w #(%1 & 0xFFFF),%2"
 [(set_attr "cc" "math")]
)

; pic30_isa32
;
; (set (reg:HI (minus:HI (x) (y))))
; (set (reg:SI (x) (sign_extend:SI (reg:HI x))))
; ->sub:si () 

(define_peephole
 [(set (match_operand:HI 0 "pic30_strict_register_operand" "")
       (minus:HI 
          (match_operand:HI 1 "pic30_strict_register_operand" "")
          (match_operand:HI 2 "pic30_strict_register_operand" "")))
  (set (match_operand:SI 3 "pic30_strict_register_operand" "")
       (sign_extend:SI
          (match_dup 0)))
 ]
 "(pic30_isa32_target() && 
   (REGNO(operands[3]) == REGNO(operands[0])))"
 "sub.l %1,%2,%0"
 [(set_attr "cc" "math")]
)

; pic30_isa32
;
; (set (reg x) (y))
; (set (cc0)
;      (compare (reg:HI x))
;               (const_int 0))
; -> sub (reg x) #0 (y)

(define_peephole
 [(set (match_operand:QI 0 "pic30_strict_register_operand" "")
       (match_operand:QI 1 "pic30_mode2_operand" ""))
  (set (cc0)
       (compare
          (match_dup 0)
          (const_int 0)))
 ]
 "pic30_isa32_target()"
 "sub.b %1,#0,%0"
 [(set_attr "cc" "math")]
)

(define_peephole
 [(set (match_operand:HI 0 "pic30_strict_register_operand" "")
       (match_operand:HI 1 "pic30_mode2_operand" ""))
  (set (cc0)
       (compare
          (match_dup 0)
          (const_int 0)))
 ]
 "pic30_isa32_target()"
 "sub.w %1,#0,%0"
 [(set_attr "cc" "math")]
)

(define_peephole
 [(set (match_operand:SI 0 "pic30_strict_register_operand" "")
       (match_operand:SI 1 "pic30_mode2_operand" ""))
  (set (cc0)
       (compare
          (match_dup 0)
          (const_int 0)))
 ]
 "pic30_isa32_target()"
 "sub.l %1,#0,%0"
 [(set_attr "cc" "math")]
)

; pic30_isa32
;
; (set (reg x) (const_int 0))
; (set (reg x+1) (const_int 0))
;
; -> (set (reg x) (x * 0)

(define_peephole
 [(set (match_operand:SI 0 "pic30_strict_register_operand" "")
       (const_int 0))
  (set (match_operand:SI 1 "pic30_strict_register_operand" "")
       (const_int 0))
 ]
 "pic30_isa32_target() && (REGNO(operands[1]) == (REGNO(operands[0])+1))"
 "mulss.d %0,#0,%0"
)

(define_peephole
 [(set (match_operand:SI 0 "pic30_strict_register_operand" "")
       (const_int 0))
  (set (match_operand:SI 1 "pic30_strict_register_operand" "")
       (const_int 0))
 ]
 "pic30_isa32_target() && (REGNO(operands[0]) == (REGNO(operands[1])+1))"
 "mulss.d %0,#0,%1"
)

; pic30_isa32
;
; (set (reg x (const_int 0)))
; (set (mem(reg y ) (reg x)))
; ->sub wn, wn, [y]

(define_peephole
 [(set (match_operand:QI 0 "pic30_strict_register_operand" "")
       (const_int 0))
  (set (match_operand:QI 1 "pic30_mode2_operand" "")
       (match_dup 0))
 ]
 "(pic30_isa32_target() &&
   dead_or_set_p(insn,operands[0]))"
 "sub.b w0,w0,%1"
 [(set_attr "cc" "math")]
)

(define_peephole
 [(set (match_operand:HI 0 "pic30_strict_register_operand" "")
       (const_int 0))
  (set (match_operand:HI 1 "pic30_mode2_operand" "")
       (match_dup 0))
 ]
 "(pic30_isa32_target() &&
   dead_or_set_p(insn,operands[0]))"
 "sub.w w0,w0,%1"
 [(set_attr "cc" "math")]
)

(define_peephole
 [(set (match_operand:SI 0 "pic30_strict_register_operand" "")
       (const_int 0))
  (set (match_operand:SI 1 "pic30_mode2_operand" "")
       (match_dup 0))
 ]
 "(pic30_isa32_target() &&
   dead_or_set_p(insn,operands[0]))"
 "sub.l w0,w0,%1"
 [(set_attr "cc" "math")]
)

; pic30_isa32

;
; (set (reg x)  (reg y))
; (set (reg y ) (minus (reg x) (short lit)))
; ->sub wn, wn, [y]

(define_peephole
 [(set (match_operand:QI 0 "pic30_strict_register_operand" "")
       (match_operand:QI 1 "pic30_mode2_operand" ""))
  (set (match_operand:QI 2 "pic30_mode2_operand" "")
       (minus:QI 
         (match_dup 0)
         (match_operand:QI 3 "pic30_lit7_operand" "")))
 ]
 "(pic30_isa32_target() &&
   dead_or_set_p(insn,operands[0]))"
 "sub.b %1,#%3,%2"
 [(set_attr "cc" "math")]
)

(define_peephole
 [(set (match_operand:HI 0 "pic30_strict_register_operand" "")
       (match_operand:HI 1 "pic30_mode2_operand" ""))
  (set (match_operand:HI 2 "pic30_mode2_operand" "")
       (minus:HI 
         (match_dup 0)
         (match_operand:HI 3 "pic30_lit7_operand" "")))
 ]
 "(pic30_isa32_target() &&
   dead_or_set_p(insn,operands[0]))"
 "sub.w %1,#%3,%2"
 [(set_attr "cc" "math")]
)

(define_peephole
 [(set (match_operand:SI 0 "pic30_strict_register_operand" "")
       (match_operand:SI 1 "pic30_mode2_operand" ""))
  (set (match_operand:SI 2 "pic30_mode2_operand" "")
       (minus:SI 
         (match_dup 0)
         (match_operand:SI 3 "pic30_lit7_operand" "")))
 ]
 "(pic30_isa32_target() &&
   dead_or_set_p(insn,operands[0]))"
 "sub.l %1,#%3,%2"
 [(set_attr "cc" "math")]
)

; add n

(define_peephole
 [(set (match_operand:QI 0 "pic30_strict_register_operand" "")
       (match_operand:QI 1 "pic30_mode2_operand" ""))
  (set (match_operand:QI 2 "pic30_mode2_operand" "")
       (plus:QI
         (match_dup 0)
         (match_operand:QI 3 "pic30_P_operand" "")))
 ]
 "(pic30_isa32_target() &&
   dead_or_set_p(insn,operands[0]))"
 "add.b %1,#%3,%2"
 [(set_attr "cc" "math")]
)

(define_peephole
 [(set (match_operand:HI 0 "pic30_strict_register_operand" "")
       (match_operand:HI 1 "pic30_mode2_operand" ""))
  (set (match_operand:HI 2 "pic30_mode2_operand" "")
       (plus:HI
         (match_dup 0)
         (match_operand:HI 3 "pic30_P_operand" "")))
 ]
 "(pic30_isa32_target() &&
   dead_or_set_p(insn,operands[0]))"
 "add.w %1,#%3,%2"
 [(set_attr "cc" "math")]
)

(define_peephole
 [(set (match_operand:SI 0 "pic30_strict_register_operand" "")
       (match_operand:SI 1 "pic30_mode2_operand" ""))
  (set (match_operand:SI 2 "pic30_mode2_operand" "")
       (plus:SI
         (match_dup 0)
         (match_operand:SI 3 "pic30_P_operand" "")))
 ]
 "(pic30_isa32_target() &&
   dead_or_set_p(insn,operands[0]))"
 "add.l %1,#%3,%2"
 [(set_attr "cc" "math")]
)

; add -n

(define_peephole
 [(set (match_operand:QI 0 "pic30_strict_register_operand" "")
       (match_operand:QI 1 "pic30_mode2_operand" ""))
  (set (match_operand:QI 2 "pic30_mode2_operand" "")
       (plus:QI
         (match_dup 0)
         (match_operand:QI 3 "pic30_N_operand" "")))
 ]
 "(pic30_isa32_target() &&
   dead_or_set_p(insn,operands[0]))"
 "sub.b %1,#%J3,%2"
 [(set_attr "cc" "math")]
)

(define_peephole
 [(set (match_operand:HI 0 "pic30_strict_register_operand" "")
       (match_operand:HI 1 "pic30_mode2_operand" ""))
  (set (match_operand:HI 2 "pic30_mode2_operand" "")
       (plus:HI
         (match_dup 0)
         (match_operand:HI 3 "pic30_N_operand" "")))
 ]
 "(pic30_isa32_target() &&
   dead_or_set_p(insn,operands[0]))"
 "sub.w %1,#%J3,%2"
 [(set_attr "cc" "math")]
)

(define_peephole
 [(set (match_operand:SI 0 "pic30_strict_register_operand" "")
       (match_operand:SI 1 "pic30_mode2_operand" ""))
  (set (match_operand:SI 2 "pic30_mode2_operand" "")
       (plus:SI
         (match_dup 0)
         (match_operand:SI 3 "pic30_N_operand" "")))
 ]
 "(pic30_isa32_target() &&
   dead_or_set_p(insn,operands[0]))"
 "sub.l %1,#%J3,%2"
 [(set_attr "cc" "math")]
)

; all
; (set (reg:r)  (ashiftrt: (reg:y) (const_int n)))
; (set (reg:r1) (and (reg:r) (const_int m)))
; (set (reg:r2) (ashift: (reg:r1) (const_int k)))
;
; where (m << n ) < 127 
;
; ->
;
; (set (reg:r1) (and (reg:y) (const_int m << n)))
; (set (reg:r2) (shift (reg:r1) (const_int k-n)))

(define_peephole
 [(set (match_operand:QI    0 "pic30_strict_register_operand" "")
       (ashiftrt:QI 
          (match_operand:QI 1 "pic30_strict_register_operand" "")
          (match_operand    2 "immediate_operand" "")))
  (set (match_operand:QI    3 "pic30_strict_register_operand" "")
       (and:QI
          (match_dup 0)
          (match_operand    4 "immediate_operand" "")))
  (set (match_operand:QI    5 "pic30_strict_register_operand" "")
       (ashift:QI 
          (match_dup 3)
          (match_operand    6 "immediate_operand" "")))
  ]
  "(((INTVAL(operands[4]) & 0x80) == 0) &&
    (INTVAL(operands[2]) > INTVAL(operands[6])) &&
    dead_or_set_p(insn,operands[0]))"
  "and.b %1,#(%4<<%2),%3\;asr.b %3,#(%2-%6),%5"
 [(set_attr "cc" "math")]
)

(define_peephole
 [(set (match_operand:HI    0 "pic30_strict_register_operand" "")
       (ashiftrt:HI 
          (match_operand:HI 1 "pic30_strict_register_operand" "")
          (match_operand    2 "immediate_operand" "")))
  (set (match_operand:HI    3 "pic30_strict_register_operand" "")
       (and:HI
          (match_dup 0)
          (match_operand    4 "immediate_operand" "")))
  (set (match_operand:HI    5 "pic30_strict_register_operand" "")
       (ashift:HI 
          (match_dup 3)
          (match_operand    6 "immediate_operand" "")))
  ]
  "(((INTVAL(operands[4]) & 0x8000) == 0) &&
    (INTVAL(operands[2]) > INTVAL(operands[6])) &&
    dead_or_set_p(insn,operands[0]))"
  "and.w %1,#(%4<<%2),%3\;asr.w %3,#(%2-%6),%5"
 [(set_attr "cc" "math")]
)

(define_peephole
 [(set (match_operand       0 "pic30_strict_register_operand" "")
       (ashiftrt    
          (match_operand    1 "pic30_strict_register_operand" "")
          (match_operand    2 "immediate_operand" "")))
  (set (match_operand:SI    3 "pic30_strict_register_operand" "")
       (and:SI
          (match_operand:SI 4 "pic30_strict_register_operand" "")
          (match_operand    5 "immediate_operand" "")))
  (set (match_operand:SI    6 "pic30_strict_register_operand" "")
       (ashift:SI 
          (match_dup 3)
          (match_operand    7 "immediate_operand" "")))
  ]
  "((pic30_isa32_target()) &&
    ((INTVAL(operands[5]) << (INTVAL(operands[2]))) < 0x80) &&
    // ((INTVAL(operands[5]) & 0x80000000) == 0) &&
    // (INTVAL(operands[2]) > INTVAL(operands[7])) &&
    dead_or_set_p(insn,operands[0]) &&
    (REGNO(operands[4]) == REGNO(operands[0])))"
  "and.l %1,#(%5<<%2),%3\;asr.l %3,#(%2-%7),%6"
 [(set_attr "cc" "math")]
)

;
;  if ((x >> y) & 1)
;

(define_peephole
 [(set (match_operand       0 "pic30_strict_register_operand" "")
       (lshiftrt    
          (match_operand    1 "pic30_mode2_operand" "")
          (match_operand    2 "pic30_strict_register_operand" "")))
  (set (cc0)
       (compare
          (zero_extract
            (match_operand  3 "pic30_strict_register_operand" "")
            (const_int 1)
            (const_int 0))
          (const_int 0)))
  ]
  "((pic30_isa32_target()) &&
    (REGNO(operands[3]) == REGNO(operands[0])) &&
    dead_or_set_p(insn,operands[0]))"
  "btst.lz %1,%2"
 [(set_attr "cc" "math")]
)

;  x = *y >> 8
;  z = *y & 0xFF00  
;  *y = x | z
;  + variants thereof

(define_peephole
  [(set (match_operand:HI 0 "pic30_strict_register_operand")
        (match_operand:HI 1 "pic30_mode2_operand"))
   (set (match_operand:HI 2 "pic30_strict_register_operand")
        (lshiftrt:HI
           (match_dup 0)
           (const_int 8)))
   (set (match_dup 0)
        (and:HI
           (match_dup 0)
           (const_int 65280)))
   (set (match_dup 1)
        (ior:HI
           (match_dup 2)
           (match_dup 0)))
  ]
  "((pic30_isa32_target()) &&
    (dead_or_set_p(insn,operands[0])) &&
    (dead_or_set_p(insn,operands[2])))"
  "mov.b [%T1+1],%0\;mov.b %0,%1"
)

(define_peephole
  [
   (set (match_operand:HI    0 "pic30_strict_register_operand")
        (lshiftrt:HI
           (match_operand:HI 1 "pic30_strict_register_operand")
           (const_int 8)))
   (set (match_operand:HI    2 "pic30_strict_register_operand")
        (and:HI
           (match_dup 1)
           (const_int 65280)))
   (set (match_dup 1)
        (ior:HI
           (match_dup 2)
           (match_dup 0)))
  ]
  "((pic30_isa32_target()) &&
    (dead_or_set_p(insn,operands[0])) &&
    (dead_or_set_p(insn,operands[2])))"
  "lsr.w %1,#8,%0\;mov.b %0,%1"
)

(define_peephole
  [
   (set (match_operand:RIM 0 "pic30_mode2_operand" "+r,R,Q")
        (plus (match_dup 0)
              (const_int -1)))
   (clobber
        (match_operand:SI 1 "pic30_strict_register_operand" "=X,X,r"))
   (set (pc)
        (if_then_else
          (ge (match_dup 0) (const_int 0))
          (label_ref (match_operand 2 "" ""))
          (pc)))
   (clobber (match_dup 1))
  ]
  "((pic30_isa32_target()) &&
    (dead_or_set_p(insn,operands[0])) &&
    (dead_or_set_p(insn,operands[2])))"
  "@
   dtb %0,%2
   sub.%v0 %0,#1,%0\;bra nz,%2
   %l0.1 %1\;sub.%v0 [%1],#1,[%1]\;bra nz,%2"
)

;; make a indirect bfext
(define_peephole2
  [
    (set (match_operand:HI 0 "pic30_strict_register_operand" "")
         (match_operand:HI 1 "pic30_mode2_operand"    ""))
    (set (match_operand:SI 2 "pic30_strict_register_operand" "")
         (zero_extract:SI
            (subreg:SI (match_dup 0) 0)
            (match_operand 3 "immediate_operand"      "")
            (match_operand 4 "immediate_operand"      "")))
  ]
  "(pic30_isa32_target())"
  [
    (set (match_dup 2)
         (zero_extract:SI
            (subreg:SI (match_dup 1) 0)
            (match_dup 3)
            (match_dup 4)))
  ]              
  ""
)

(define_peephole
  [
   (set (match_operand:SI   0 "pic30_strict_register_operand" "=r")
        (zero_extend:SI 
          (match_operand:HI 1 "pic30_mode2_operand"    " rR<>")))
   (set (match_operand:SI   2 "pic30_strict_register_operand" "=r")
        (ashift:SI 
          (match_dup 0)
          (match_operand:HI 3 "immediate_operand"      " i")))
  ]
  "((pic30_isa32_target()) &&
    (dead_or_set_p(insn,operands[0])) &&
    (INTVAL(operands[3]) <= 7))"
  "muluu.w %1,#(1<<%3),%0"
)

; [(set (subreg:HI (match_dup 5) 2) (match_dup 1))
;  (set (subreg:HI (match_dup 5) 0) (match_dup 4))]

;; (define_peephole
;;   [(set (match_operand:SI 0 "pic30_strict_register_operand"                "=r")
;;         (sign_extend:SI (match_operand:HI 1 "pic30_strict_register_operand" "r")) )
;;    (set (match_operand:SF 2 "pic30_strict_register_operand" "=r")
;;         (call (match_operand:QI 3 "memory_operand"  "m")
;;               (match_operand:HI 4 "pic30_general_operand" "g")))
;;   ]
;;   "isfloatsisf(operands[3])"
;;   "*
;; {
;; 	if (TARGET_SMALL_CODE)
;; 		return \"rcall __floathisf\";
;; 	else
;; 		return \"call __floathisf\";
;; }")
