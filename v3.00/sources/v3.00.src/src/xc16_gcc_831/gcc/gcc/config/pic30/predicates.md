;;- Machine description for GNU compiler
;;- Microchip predicates
;;- Copyright (C) 1994-2022 Free Software Foundation, Inc.

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
;;      These is the machine predicates for the Microchip dsPIC.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;; define_predicate automatically includes the equivalent of
;;    (ior (match_test "GET_MODE(op) == mode")
;;         (macch_test "GET_MODE(op) == VOIDmode"))
;;
;; so no need to do this.  If you don't want it, use define_special_predicate
;;

;;  {"pic30_general_operand", {CONST_INT, CONST_DOUBLE, CONST, SYMBOL_REF, 
;;                             LABEL_REF, SUBREG, REG, MEM }}, 

;; 
;; pic30_general_operand does not accept any for the funky C30 pointer modes
;;
(define_predicate "pic30_invalid_immediate_operand"
  (and (match_code "const_int")
       (match_test "(immediate_operand(op,mode) == 0)")))

(define_predicate "pic30_general_operand"
  (match_operand 0 "general_operand")
{
  if (GET_CODE(op) == MEM) {
    rtx inner = XEXP(op,0);

    if ((GET_MODE(inner) == P24PROGmode) || (GET_MODE(inner) == P24PSVmode) ||
        (GET_MODE(inner) == P16PMPmode) || (GET_MODE(inner) == P32EXTmode) ||
        (GET_MODE(inner) == P32EDSmode) || (GET_MODE(inner) == P32PEDSmode) ||
        (GET_MODE(inner) == P32UMMmode)) {
      /* a de-reference of an extended pointer - not general */
      return 0;
    }
  } else if (REG_P(op) && (REGNO(op) < FIRST_PSEUDO_REGISTER)) {
    /* general regs doesn't check the regclass - which is really annoying
       because 'g' does... */
    return reg_fits_class_p(op, GENERAL_REGS, 0, GET_MODE(op));
  }
  return 1;
})

(define_predicate "pic30_pushhi_operand"
  (ior (match_code "reg")
       (and (match_test "pic30_T_constraint(op,mode)")
            (match_test "GET_MODE(XEXP(op,0)) == machine_Pmode"))
       (and (match_code "mem")
            (match_test "GET_CODE(XEXP(op,0)) == REG")
            (match_test "GET_MODE(XEXP(op,0)) == machine_Pmode"))
       (and (match_code "const_int")
            (match_test "(INTVAL(op) == 0)"))
       (match_code "post_inc")
       (match_code "pre_inc")))

(define_predicate "pic30_pushsi_operand"
  (and (match_test "pic30_isa32_target()")
       (ior (match_code "reg")
            (and (match_code "mem,subreg")
                 (match_test "pic30_T_constraint(op,mode)")
            )
            (and (match_code "mem")
                 (match_test "GET_CODE(XEXP(op,0)) == REG")
                 (match_test "GET_MODE(XEXP(op,0)) == machine_Pmode")
            )
       )
  )
)

;;  { "pic30_psvapge_operand", { REG }}, 

(define_special_predicate "pic30_psvpage_operand"
  (and (match_code "reg")
       (match_test "REGNO(op) == PSVPAG")
       (match_test "GET_MODE(op) == QImode")))

;;  { "pic30_psv_operand", { MEM }}, 

(define_predicate "pic30_prog_operand"
  (and (match_test "pic30_extended_pointer_operand(op, P24PROGmode)")
       (match_code "post_inc,pre_dec,pre_inc,post_dec,reg,subreg,plus,symbol_ref,const")))

(define_predicate "pic30_psv_operand"
  (and (match_test "pic30_extended_pointer_operand(op, P24PSVmode)")
       (match_code "post_inc,pre_dec,pre_inc,post_dec,reg,subreg,plus,symbol_ref,const")))

(define_predicate "pic30_apsv_operand"
  (and (match_test "pic30_extended_pointer_operand(op, P16APSVmode)")
       (match_code "post_inc,pre_dec,pre_inc,post_dec,reg,subreg,plus,symbol_ref,const")))

(define_predicate "pic30_pmp_operand"
  (and (match_test "pic30_extended_pointer_operand(op, P16PMPmode)")
       (match_code "post_inc,pre_dec,pre_inc,post_dec,reg,subreg,plus,symbol_ref,const")))

(define_predicate "pic30_ext_operand"
  (and (match_test "pic30_extended_pointer_operand(op, P32EXTmode)")
       (match_code "post_inc,pre_dec,pre_inc,post_dec,reg,subreg,plus,symbol_ref,const")))

(define_predicate "pic30_eds_operand"
  (and (match_test "pic30_extended_pointer_operand(op, mode)")
       (match_code "post_inc,pre_dec,pre_inc,post_dec,reg,subreg,plus,symbol_ref,const")))

(define_predicate "pic30_df_operand"
  (and (match_test "pic30_extended_pointer_operand(op, mode)")
       (match_code "post_inc,pre_dec,pre_inc,post_dec,reg,subreg,plus,symbol_ref,const")))

(define_predicate "pic30_umm_operand"
  (and (match_test "pic30_extended_pointer_operand(op, P32UMMmode)")
       (match_code "reg,subreg,plus,symbol_ref,const")))

;;  { "pic30_tbl_operand", { MEM }}, 
;;  unused

;;  { "pic30_register_operand", { REG, SUBREG }}, 

;; do not allow float<->int conversions
(define_predicate "pic30_float_register_operand"
  (and 
    (match_code "reg,subreg")
    (match_test "pic30_register_operand_helper(op,mode,parf_float_subreg_ok)")))



;; do not allow floating point registers
(define_predicate "pic30_register_operand"
  (match_code "reg,subreg")
{
  enum pic30_acceptible_regs_flags flags = parf_int_subreg_ok;
  if (!pic30_isa32_target()) 
    flags = parf_subreg_ok;
  return pic30_register_operand_helper(op,mode,flags);
})

;; allow any register, even if they are conversions
(define_predicate "pic30_any_register_operand"
  (and (match_code "reg,subreg")
       (match_test "pic30_register_operand_helper(op,mode,parf_subreg_ok)")))

(define_predicate "pic30_D_register_operand"
  (and (match_code "reg,subreg")
       (match_test "pic30_register_operand_helper(op,mode,(pic30_acceptible_regs_flags)(parf_D_regs_only | parf_subreg_ok))")))

(define_predicate "pic30_strict_register_operand"
  (and (match_code "reg")
       (match_test "pic30_register_operand_helper(op,mode,(pic30_acceptible_regs_flags)0)")))

;;  { "pic30_mode1_operand", { REG, MEM, SUBREG, CONST_INT }}, 

(define_predicate "pic30_mode1_operand"
  (and (match_test "pic30_mode1MinMax_operand(op,mode,\"K\")")
       (match_code "subreg,reg,mem,const_int")))

(define_predicate "pic30_mode1_0k_operand"
  (and (match_test "pic30_mode1MinMax_operand(op,mode,\"O\")")
       (match_code "subreg,reg,mem,const_int")))

(define_predicate "pic30_mode1_APSV_operand"
  (and (match_test "pic30_mode1MinMax_APSV_operand(op,mode,\"K\")")
       (match_code "subreg,reg,mem,const_int")))

;;  { "pic30_mode1P_operand", { REG, MEM, SUBREG, CONST_INT }}, 

(define_predicate "pic30_mode1P_operand"
  (and (match_test "pic30_mode1MinMax_operand(op,mode,\"P\")")
       (match_code "subreg,reg,mem,const_int")))

(define_predicate "pic30_mode1P_APSV_operand"
  (and (match_test "pic30_mode1MinMax_APSV_operand(op,mode,\"P\")")
       (match_code "subreg,reg,mem,const_int")))

(define_predicate "pic30_mode1P_or_1bit_APSV_operand"
  (and 
       (ior 
           (match_test "pic30_mode1MinMax_APSV_operand(op,mode,\"P\")")
           (match_test "pic30_one_bit_set(mode,op,1)")
       )
       (match_code "subreg,reg,mem,const_int")))

(define_predicate "pic30_mode1P_or_1bitclr_APSV_operand"
  (and 
       (ior 
           (match_test "pic30_mode1MinMax_APSV_operand(op,mode,\"P\")")
           (match_test "pic30_one_bit_set(mode,op,0)")
       )
       (match_code "subreg,reg,mem,const_int")))

;;  { "pic30_mode1PN_operand", { REG, MEM, SUBREG, CONST_INT }}, 

(define_predicate "pic30_mode1PN_operand"
  (and (match_test "pic30_mode1MinMax_operand(op,mode,\"NP\")")
       (match_code "subreg,reg,mem,const_int")))

(define_predicate "pic30_mode1PN_UMM_operand"
  (and (match_test "pic30_mode1MinMax_operand(op,mode,\"h\")")
       (match_code "subreg,reg,mem,const_int")))

(define_predicate "pic30_mode1PN_APSV_operand"
  (and (match_test "pic30_mode1MinMax_APSV_operand(op,mode,\"NP\")")
       (match_code "subreg,reg,mem,const_int")))

;;  { "pic30_mode1J_operand", { REG, MEM, SUBREG, CONST_INT }}, 

(define_predicate "pic30_mode1J_operand"
  (and (match_test "pic30_mode1MinMax_operand(op,mode,\"J\")")
       (match_code "subreg,reg,mem,const_int")))

(define_predicate "pic30_mode1J_APSV_operand"
  (and (match_test "pic30_mode1MinMax_APSV_operand(op,mode,\"J\")")
       (match_code "subreg,reg,mem,const_int")))

(define_predicate "pic30_mode1J_or_1bitclr_APSV_operand"
  (and
       (ior
           (match_test "pic30_mode1MinMax_APSV_operand(op,mode,\"J\")")
           (match_test "pic30_one_bit_set(mode,op,0)")
       )
       (match_code "subreg,reg,mem,const_int")))

;;  { "pic30_mode1JN_operand", { REG, MEM, SUBREG, CONST_INT }}, 

(define_predicate "pic30_mode1JN_operand"
  (and (match_test "pic30_mode1MinMax_operand(op,mode,\"JN\")")
       (match_code "subreg,reg,mem,const_int")))

(define_predicate "pic30_mode1JN_APSV_operand"
  (and (match_test "pic30_mode1MinMax_APSV_operand(op,mode,\"JN\")")
       (match_code "subreg,reg,mem,const_int")))

;;  { "pic30_indirect_mem_operand", { MEM }}, 

(define_predicate "pic30_indirect_mem_operand_modify"
  (and (match_code "mem")
       (match_code "post_inc,post_dec" "0")
       (match_test "GET_MODE(XEXP(op,0)) == machine_Pmode")))

(define_predicate "pic30_reg_or_indirect_mem_operand_modify"
  (ior (match_code "reg")
       (and (match_code "mem")
            (match_code "post_inc,post_dec" "0")
            (match_test "GET_MODE(XEXP(op,0)) == machine_Pmode"))))

(define_predicate "pic30_APSV_indirect_mem_operand_modify"
  (and (match_code "mem")
       (match_code "post_inc,post_dec,pre_inc,pre_dec" "0")
       (ior (match_test "GET_MODE(XEXP(op,0)) == machine_Pmode")
            (match_test "GET_MODE(XEXP(op,0)) == P16APSVmode"))))

(define_predicate "pic30_indirect_mem_operand"
  (and (match_code "mem")
       (match_code "post_inc,post_dec,pre_inc,pre_dec,reg,subreg" "0")
       (match_test "GET_MODE(XEXP(op,0)) == machine_Pmode"))
{
  int return_value;
  enum rtx_code code;
 
  return_value = FALSE;
  code = GET_CODE(XEXP(op,0));
  switch (code) {
    case SUBREG:
      return_value = pic30_register_operand(XEXP(op,0), machine_Pmode);
      break;
    case POST_INC:
    case POST_DEC:
    case PRE_INC:
    case PRE_DEC:
    case REG:
      return_value = TRUE;
      break;
    default:
      break;
  }
  return return_value;
})

(define_predicate "pic30_DI_indirect_mem_operand"
  (and (match_code "mem")
       (match_code "post_inc,reg" "0")
       (match_test "GET_MODE(XEXP(op,0)) == machine_Pmode")))

(define_predicate "pic30_APSV_indirect_mem_operand"
  (and (match_code "mem")
       (match_code "post_inc,post_dec,pre_inc,pre_dec,reg" "0")
       (ior (match_test "GET_MODE(XEXP(op,0)) == machine_Pmode")
            (match_test "GET_MODE(XEXP(op,0)) == P16APSVmode"))))

;;  { "pic30_mode3_operand", { SUBREG, REG, MEM }}, 

(define_predicate "pic30_mode3_operand"
  (and (match_code "subreg,reg,mem")
       (match_test "pic30_mode3_operand_helper(op,mode)")))

(define_predicate "pic30_mode3_APSV_operand"
  (and (match_code "subreg,reg,mem")
       (match_test "pic30_mode3_APSV_operand_helper(op,mode)")))

;;  { "pic30_modek_operand", { MEM }}, 

(define_predicate "pic30_modek_operand"
  (and (match_code "subreg,mem")
       (match_test "pic30_modek_operand_helper(op,mode)")))

(define_predicate "pic30_modek_APSV_operand"
  (and (match_code "subreg,mem")
       (match_test "pic30_modek_APSV_operand_helper(op,mode)")))

;;  { "pic30_data_operand", { MEM }}, 

(define_predicate "pic30_data_operand32"
  (and (match_code "subreg,mem")
       (match_test "pic30_T_constraint(op,mode)")
       (match_test "!pic30_U_constraint(op,mode)")))

(define_predicate "pic30_reg_or_data_operand32"
  (and (match_code "subreg,mem,reg")
       (ior (match_operand 0 "register_operand")
            (match_operand 0 "pic30_data_operand32"))
  )
)
     
(define_predicate "pic30_data_operand"
  (and (match_code "mem,subreg")
       (match_test "GET_MODE(op) != QImode")
       (match_test "pic30_T_constraint(op,mode)")
       (match_test "GET_MODE(XEXP(op,0)) == machine_Pmode")))

;;  { "pic30_move2_operand", { SUBREG, REG, MEM }}, 

(define_predicate "pic30_move2_operand"
  (ior (match_operand 0 "pic30_mode3_operand")
       (match_operand 0 "pic30_modek_operand")))

(define_predicate "pic30_move2_APSV_operand"
  (ior (match_operand 0 "pic30_mode3_APSV_operand")
       (match_operand 0 "pic30_modek_APSV_operand")))

;;  { "pic30_mode2_operand", { SUBREG, REG, MEM }}, 

(define_predicate "pic30_mode2_operand"
  (match_code "subreg,reg,mem")
{
  int fMode2Operand;
  enum rtx_code code;

  if (!pic30_isa32_target()) return pic30_any_mode2_operand(op,mode);
  fMode2Operand = FALSE;
  if (mode == VOIDmode) mode = GET_MODE(op);
  code = GET_CODE(op);
  switch (code) {
    case SUBREG:
      fMode2Operand = pic30_register_operand(op, mode);
      break;
    case REG:
      /*
      ** Register to register
      */
      fMode2Operand = ((GET_MODE(op) == mode) &&
                       ((REGNO(op) <= pic30_num_regs) ||
                        (REGNO(op) >= FIRST_PSEUDO_REGISTER)));
      break;
    case MEM:
      fMode2Operand = pic30_indirect_mem_operand(op,mode);
      break;
    default:
        break;
  }
  return fMode2Operand;
})

(define_predicate "pic30_float_mode2_operand"
  (match_code "subreg,reg,mem")
{
  int fMode2Operand;
  enum rtx_code code;

  fMode2Operand = FALSE;
  if (mode == VOIDmode) mode = GET_MODE(op);
  code = GET_CODE(op);
  if ((mode == SFmode) || (mode == DFmode))
  switch (code) {
    case SUBREG:
      fMode2Operand = pic30_float_register_operand(op, mode);
      break;
    case REG:
      /*
      ** Register to register
      */
      fMode2Operand = ((GET_MODE(op) == mode) &&
                       ((REGNO(op) <= pic30_num_regs) ||
                        (REGNO(op) >= FIRST_PSEUDO_REGISTER)));
      break;
    case MEM:
      fMode2Operand = pic30_indirect_mem_operand(op,mode);
      break;
    default:
        break;
  }
  return fMode2Operand;
})

(define_predicate "pic30_any_mode2_operand"
  (match_code "subreg,reg,mem")
{
  int fMode2Operand;
  enum rtx_code code; 
       
  fMode2Operand = FALSE;
  if (mode == VOIDmode) mode = GET_MODE(op);
  code = GET_CODE(op);
  switch (code) {
    case SUBREG:
      fMode2Operand = pic30_any_register_operand(op, mode);
      break;
    case REG:
      /*
      ** Register to register
      */
#if 0
      fMode2Operand = ((GET_MODE(op) == mode) &&
                       ((REGNO(op) <= pic30_num_regs) ||
                        (REGNO(op) >= FIRST_PSEUDO_REGISTER)));
#else
      fMode2Operand = (pic30_any_register_operand(op,mode) ||
                        (REGNO(op) >= FIRST_PSEUDO_REGISTER));
#endif
      break;
    case MEM:
      fMode2Operand = pic30_indirect_mem_operand(op,mode);
      break;
    default:
        break;
  }
  return fMode2Operand; 
})

(define_predicate "pic30_DI_mode2_operand"
  (match_code "subreg,reg,mem")
{
  int fMode2Operand;
  enum rtx_code code;

  fMode2Operand = FALSE;
  if (mode == VOIDmode) mode = GET_MODE(op);
  code = GET_CODE(op);
  switch (code) {
    case SUBREG:
      fMode2Operand = pic30_register_operand(op, mode);
      break;
    case REG:
      /*
      ** Register to register
      */
      fMode2Operand = ((GET_MODE(op) == mode) &&
                       ((REGNO(op) <= pic30_num_regs) ||
                        (REGNO(op) >= FIRST_PSEUDO_REGISTER)));
      break;
    case MEM:
      fMode2Operand = pic30_DI_indirect_mem_operand(op,mode);
      break;
    default:
        break;
  }
  return fMode2Operand;
})

(define_predicate "pic30_DI_mode2k_operand"
  (match_code "subreg,reg,mem")
{
  int fMode2Operand;
  enum rtx_code code;

  fMode2Operand = FALSE;
  if (mode == VOIDmode) mode = GET_MODE(op);
  code = GET_CODE(op);
  switch (code) {
    case SUBREG:
      fMode2Operand = pic30_register_operand(op, mode);
      break;
    case REG:
      /*
      ** Register to register
      */
      fMode2Operand = ((GET_MODE(op) == mode) &&
                       ((REGNO(op) <= pic30_num_regs) ||
                        (REGNO(op) >= FIRST_PSEUDO_REGISTER)));
      break;
    case MEM:
      fMode2Operand = pic30_DI_indirect_mem_operand(op,mode) |
                      pic30_modek_operand(op,mode);
      break;
    default:
        break;
  }
  return fMode2Operand;
})

(define_predicate "pic30_D_mode2_operand"
  (match_code "subreg,reg,mem")
{
  int fMode2Operand;
  enum rtx_code code;

  fMode2Operand = FALSE;
  if (mode == VOIDmode) mode = GET_MODE(op);
  code = GET_CODE(op);
  switch (code) {
    case SUBREG:
      fMode2Operand = pic30_register_operand(op, mode);
      break;
    case REG:
      /*
      ** Register to register
      */
      fMode2Operand = ((GET_MODE(op) == mode) &&
                       (((REGNO(op) <= pic30_num_regs) &&
                         ((REGNO(op) & 1) == 0)) ||
                        (REGNO(op) >= FIRST_PSEUDO_REGISTER)));
      break;
    case MEM:
      fMode2Operand = pic30_indirect_mem_operand(op,mode);
      break;
    default:
        break;
  }
  return fMode2Operand;
})

(define_predicate "pic30_mode2_APSV_operand"
  (match_code "subreg,reg,mem")
{
  int fMode2Operand;
  enum rtx_code code;

  fMode2Operand = FALSE;
  if (mode == VOIDmode) mode = GET_MODE(op);
  code = GET_CODE(op);
  switch (code) {
    case SUBREG:
      fMode2Operand = pic30_register_operand(op, mode);
      break;
    case REG:
      /*
      ** Register to register
      */
      fMode2Operand = ((GET_MODE(op) == mode) &&
                       ((REGNO(op) <= pic30_num_regs) ||
                        (REGNO(op) >= FIRST_PSEUDO_REGISTER)));
      break;
    case MEM:
      fMode2Operand = pic30_indirect_mem_operand(op,mode);
      break;
    default:
        break;
  }
  return fMode2Operand;
})

(define_predicate "pic30_D_mode2_APSV_operand"
  (match_code "subreg,reg,mem")
{
  int fMode2Operand;
  enum rtx_code code;

  fMode2Operand = FALSE;
  code = GET_CODE(op);
  if (mode == VOIDmode) mode = GET_MODE(op);
  switch (code) {
    case SUBREG:
      fMode2Operand = pic30_register_operand(op, mode);
      break;
    case REG:
      /*
      ** Register to register
      */
      fMode2Operand = ((GET_MODE(op) == mode) &&
                       (((REGNO(op) <= pic30_num_regs) &&
                         ((REGNO(op) & 1) == 0)) ||
                        (REGNO(op) >= FIRST_PSEUDO_REGISTER)));
      break;
    case MEM:
      fMode2Operand = pic30_indirect_mem_operand(op,mode);
      break;
    default:
        break;
  }
  return fMode2Operand;
})

;;  { "pic30_P_operand", { CONST_INT }}, 

(define_special_predicate "pic30_lit7_operand"
  (and (match_code "const_int")
       (match_test "INTVAL(op) >= 0")
       (match_test "INTVAL(op) <= 127")))

(define_special_predicate "pic30_P_operand"
  (and (match_code "const_int")
       (match_test "satisfies_constraint_P(op)")))

(define_special_predicate "pic30_N_operand"
  (and (match_code "const_int")
       (match_test "satisfies_constraint_N(op)")))

;;  { "pic30_mode2_or_P_operand", {SUBREG, REG, MEM, CONST_INT }}, 

(define_predicate "pic30_mode2_or_P_operand"
  (ior (match_operand 0 "pic30_mode2_operand")
       (match_operand 0 "pic30_P_operand")))

(define_predicate "pic30_mode2_or_P_APSV_operand"
  (ior (match_operand 0 "pic30_mode2_APSV_operand")
       (match_operand 0 "pic30_P_operand")))

;;  { "pic30_immediate_1bit_operand", { CONST_INT }}, 

(define_predicate "pic30_immediate_1bit_operand"
  (and (match_code "const_int")
       (match_test "(INTVAL(op) & 0xFFFF) & ((INTVAL(op) & 0xFFFF) - 1) == 0")))

;;  { "pic30_accumulator_operand", { REG }}, 

(define_predicate "pic30_accumulator_operand"
  (and (match_code "reg")
       (match_test "IS_ACCUM_REG(REGNO(op))")))

(define_predicate "pic30_accumulator2_operand"
  (and (match_code "reg")
       (ior (match_test "IS_ACCUM_REG(REGNO(op))")
            (match_test "REGNO(op) >= FIRST_PSEUDO_REGISTER"))))

(define_predicate "pic30_accum_or_reg_operand"
  (and (match_code "reg,subreg")
       (ior (match_operand 0 "pic30_accumulator2_operand")
            (match_operand 0 "pic30_register_operand"))
  )
)

; this operand allows a mode1 op with no +/-k small literal
(define_predicate "pic30_accum_or_mode1_operand"
  (and (match_code "mem,reg,subreg")
       (ior (match_operand 0 "pic30_accumulator2_operand")
            (match_operand 0 "pic30_mode1_0k_operand"))
  )
)

;;  { "pic30_awb_operand", { REG }}, 

(define_predicate "pic30_awb_operand"
  (and (match_code "reg")
       (ior (match_test "REGNO(op) >= FIRST_PSEUDO_REGISTER")
            (match_test "IS_AWB_REG(REGNO(op))"))))

;;  { "pic30_mac_input_operand", { REG }}, 

(define_predicate "pic30_mac_input_operand"
  (and (match_code "reg")
       (ior (match_test "REGNO(op) >= FIRST_PSEUDO_REGISTER")
            (match_test "REGNO_REG_CLASS(REGNO(op)) == SINK_REGS")
            (match_test "IS_PRODUCT_REG(REGNO(op))")
       )
  )
)

;;   { "pic30_xprefetch_operand", { REG }}, 

(define_predicate "pic30_xprefetch_operand"
  (and (match_code "reg")
       (ior (match_test "REGNO(op) >= FIRST_PSEUDO_REGISTER")
            (match_test "REGNO_REG_CLASS(REGNO(op)) == SINK_REGS")
            (match_test "IS_XPREFETCH_REG(REGNO(op))"))))

;;  { "pic30_yprefetch_operand", { REG }}, 

(define_predicate "pic30_yprefetch_operand"
  (and (match_code "reg")
       (ior (match_test "REGNO(op) >= FIRST_PSEUDO_REGISTER")
            (match_test "REGNO_REG_CLASS(REGNO(op)) == SINK_REGS")
            (match_test "IS_YPREFETCH_REG(REGNO(op))"))))

;;  { "pic30_mode2res_operand", { SUBREG, REG, MEM }}, 

(define_predicate "pic30_mode2res_operand"
  (and (match_operand 0 "pic30_mode2_operand")
       (if_then_else (match_code "mem")
                     (not (match_code "pre_inc,post_dec" "0"))
                     (match_test "1"))))

;;  { "pic30_mode2mres_operand", { SUBREG, REG, MEM }}, 

(define_predicate "pic30_mode2mres_operand"
  (and (match_operand 0 "pic30_mode2_operand")
       (if_then_else (match_code "mem")
                     (not (match_code "pre_inc,post_dec" "0"))
                     (match_test "1"))))

(define_predicate "pic30_mode2mres_APSV_operand"
  (and (match_operand 0 "pic30_mode2_APSV_operand")
       (if_then_else (match_code "mem")
                     (not (match_code "pre_inc,post_dec" "0"))
                     (match_test "1"))))

;;  { "pic30_wreg_operand",  { SUBREG, REG }}, 

(define_predicate "pic30_wreg_operand"
  (match_operand 0 "pic30_register_operand")
{  rtx inner = op;
 
   if (GET_CODE(inner) == SUBREG) inner = SUBREG_REG(inner);
   return (REG_P(inner) && IS_AREG_OR_PSEUDO_REGNO(inner));
})
   
;;  { "pic30_breg_operand",  { SUBREG, REG }}, 

(define_predicate "pic30_breg_operand"
  (match_operand 0 "pic30_register_operand")
{  rtx inner = op;
 
   if (GET_CODE(inner) == SUBREG) inner = SUBREG_REG(inner);
   return (REG_P(inner) && IS_BREG_OR_PSEUDO_REGNO(inner));
})
   
;;  { "pic30_creg_operand", { SUBREG, REG }}, 

(define_predicate "pic30_creg_operand"
  (match_operand 0 "pic30_register_operand")
{  rtx inner = op;
 
   if (GET_CODE(inner) == SUBREG) inner = SUBREG_REG(inner);
   return (REG_P(inner) && IS_CREG_OR_PSEUDO_REGNO(inner));
})
   
;;  { "pic30_ereg_operand", { SUBREG, REG }}, 

(define_predicate "pic30_ereg_operand"
  (match_operand 0 "pic30_register_operand")
{  rtx inner = op;
 
   if (GET_CODE(inner) == SUBREG) inner = SUBREG_REG(inner);
   return (REG_P(inner) && IS_EREG_OR_PSEUDO_REGNO(inner));
})

;;  { "pic30_near_operand", { MEM }}, 

(define_predicate "pic30_near_operand"
  (and 
       (ior (match_code "mem")
            (and (match_code "subreg")
                 (match_code "mem" "0")))
       (match_test "pic30_U_constraint(op,mode)"))
)

(define_predicate "pic30_sfr_operand"
  (and (match_test "pic30_U_constraint(op,mode)")
       (match_test "pic30_sfr_operand_helper(op)")))

;;  { "pic30_T_operand", { MEM }}, 

(define_predicate "pic30_T_operand"
  (and (match_code "mem")
       (match_test "pic30_T_constraint(op,mode)")))

(define_predicate "pic30_UT_operand"
  (ior (match_operand 0 "pic30_T_operand")
       (match_operand 0 "pic30_near_operand")))

;;  { "pic30_code_operand", { MEM }}, 

(define_predicate "pic30_code_operand"
  (and (match_code "mem")
       (match_test "pic30_program_space_operand_p(XEXP(op,0))")))

;;  { "pic30_reg_or_code_operand", { SUBREG, REG, MEM }}, 

(define_predicate "pic30_reg_or_code_operand"
  (ior (match_operand 0 "pic30_register_operand")
       (match_operand 0 "pic30_code_operand")))

;;  { "pic30_reg_or_near_operand", { SUBREG, REG, MEM }}, 

(define_predicate "pic30_reg_or_near_operand"
  (ior (match_operand 0 "pic30_register_operand")
       (match_operand 0 "pic30_near_operand")))

;;  { "pic30_mode2_or_near_operand", { SUBREG, REG, MEM }},

(define_predicate "pic30_mode2_or_near_operand"
  (ior (match_operand 0 "pic30_mode2_operand")
       (match_operand 0 "pic30_near_operand")))

;;  { "pic30_reg_imm_or_near_operand", { SUBREG, REG, MEM, CONST_INT }}, 

(define_predicate "pic30_reg_imm_or_near_operand"
  (ior (match_operand 0 "immediate_operand")
       (match_operand 0 "pic30_register_operand")
       (match_operand 0 "pic30_near_operand")))

;;  { "pic30_R_operand", { MEM }},

(define_predicate "pic30_R_operand"
  (and (match_code "mem")
       (match_test "(GET_MODE(XEXP(op,0)) == machine_Pmode)")
       (match_test "pic30_R_constraint(op)")))

(define_predicate "pic30_R_APSV_operand"
  (and (match_code "mem")
       (match_test "(GET_MODE(XEXP(op,0)) == machine_Pmode)")
       (match_test "pic30_R_constraint(op)")))

;;  { "pic30_reg_or_R_operand", { SUBREG, REG, MEM }}, 

(define_predicate "pic30_reg_or_R_operand"
  (ior (match_operand 0 "pic30_register_operand")
       (match_operand 0 "pic30_R_operand")))

(define_predicate "pic30_reg_or_R_APSV_operand"
  (ior (match_operand 0 "pic30_register_operand")
       (match_operand 0 "pic30_R_APSV_operand")))

;;  { "pic30_rR_or_near_operand", { SUBREG, REG, MEM }}, 

(define_predicate "pic30_rR_or_near_operand"
  (ior (match_operand 0 "pic30_reg_or_R_operand")
       (match_operand 0 "pic30_near_operand")))

(define_predicate "pic30_rR_or_near_APSV_operand"
  (ior (match_operand 0 "pic30_reg_or_R_APSV_operand")
       (match_operand 0 "pic30_near_operand")))

;;  { "pic30_wreg_or_near_operand", { SUBREG, REG, MEM }}, 

(define_predicate "pic30_wreg_or_near_operand"
  (ior (match_operand 0 "pic30_wreg_operand")
       (match_operand 0 "pic30_near_operand")))

;;  { "pic30_reg_or_imm_operand", { SUBREG, REG, CONST_INT }}, 

(define_predicate "pic30_reg_or_imm_operand"
  (ior (match_operand 0 "pic30_register_operand")
       (match_operand 0 "immediate_operand")))

;;  { "pic30_imm2to15_operand", { CONST_INT }}, 

(define_special_predicate "pic30_imm2to15_operand"
  (and (match_operand 0 "immediate_operand")
       (match_test "INTVAL(op) >= 2")
       (match_test "INTVAL(op) <= 15")))

;;  { "pic30_imm8_operand", { CONST_INT }}, 

(define_special_predicate "pic30_imm8_operand"
  (and (match_operand 0 "immediate_operand")
       (match_test "INTVAL(op) == 8")))

(define_special_predicate "pic30_reg_or_lit8"
  (ior (match_operand 0 "pic30_register_operand")
       (and (match_operand 0 "immediate_operand")
            (match_test "INTVAL(op) >= 0")
            (match_test "INTVAL(op) < 256"))))

(define_special_predicate "pic30_reg_or_lit16"
  (ior (match_operand 0 "pic30_register_operand")
       (and (match_operand 0 "immediate_operand")
            (match_test "INTVAL(op) >= 0")
            (match_test "INTVAL(op) < 65536"))))

(define_special_predicate "pic30_reg_or_lit"
  (ior (match_operand 0 "pic30_register_operand")
       (match_operand 0 "immediate_operand")))

;;  { "pic30_imm16plus_operand", { CONST_INT }}, 

(define_special_predicate "pic30_imm16plus_operand"
  (and (match_operand 0 "immediate_operand")
       (match_test "INTVAL(op) >= 16")))

;;  { "pic30_M_operand", { CONST_INT }}, 

(define_special_predicate "pic30_M_operand"
  (and (match_code "const_int")
       (match_test "satisfies_constraint_M(op)")))

;;  { "pic30_J_operand", { CONST_INT }}, 

(define_special_predicate "pic30_J_operand"
  (and (match_code "const_int")
       (match_test "satisfies_constraint_J(op)")))

(define_special_predicate "pic30_OJM_operand"
  (and (match_code "const_int")
       (match_test "pic30_const_ok_for_str_p(INTVAL(op),mode,\"OJM\")")))

;;  { "pic30_JN_operand", { CONST_INT }}, 

(define_special_predicate "pic30_JN_operand"
  (ior (match_operand 0 "pic30_J_operand")
       (match_operand 0 "pic30_N_operand")))

;;  { "pic30_JM_operand", { CONST_INT }}, 

(define_special_predicate "pic30_JM_operand"
  (ior (match_operand 0 "pic30_J_operand")
       (match_operand 0 "pic30_M_operand")))

;;  { "pic30_math_operand", { REG, MEM, SUBREG, CONST_INT }}, 

(define_special_predicate "pic30_math_operand"
  (ior (match_operand 0 "pic30_mode1_operand")
       (match_operand 0 "pic30_JN_operand")))

(define_special_predicate "pic30_JMmath_operand"
  (ior (match_operand 0 "pic30_mode1_operand")
       (match_operand 0 "pic30_JM_operand")))

(define_special_predicate "pic30_math_APSV_operand"
  (ior (match_operand 0 "pic30_mode1_APSV_operand")
       (match_operand 0 "pic30_JN_operand")))

;;  { "pic30_inc_imm_operand", { CONST_INT }}, 

(define_special_predicate "pic30_inc_imm_operand"
  (and (match_operand 0 "immediate_operand")
       (match_test "INTVAL(op) >= -2")
       (match_test "INTVAL(op) <= 2")))
      
;;  { "pic30_near_math_operand", { REG, MEM, SUBREG,  CONST_INT }}, 

(define_special_predicate "pic30_near_math_operand"
  (ior (match_operand 0 "pic30_math_operand")
       (match_operand 0 "pic30_wreg_or_near_operand")))

;;  { "pic30_near_mode2_operand", { REG, MEM, SUBREG }}, 

(define_predicate "pic30_near_mode2_operand"
  (ior (match_operand 0 "pic30_mode2_operand")
       (match_operand 0 "pic30_wreg_or_near_operand")))
   
(define_predicate "pic30_near_mode2_APSV_operand"
  (ior (match_operand 0 "pic30_mode2_APSV_operand")
       (match_operand 0 "pic30_wreg_or_near_operand")))
   
;;  { "pic30_near_mode1PN_operand", { REG, MEM, SUBREG,  CONST_INT }}, 

(define_predicate "pic30_near_mode1PN_operand"
  (ior (match_operand 0 "pic30_mode1PN_operand")
       (match_operand 0 "pic30_wreg_or_near_operand")))

(define_predicate "pic30_near_mode1PN_APSV_operand"
  (ior (match_operand 0 "pic30_mode1PN_APSV_operand")
       (match_operand 0 "pic30_wreg_or_near_operand")))

;;  { "pic30_move_operand", { SUBREG, REG, MEM }}, 

(define_predicate "pic30_floatmove_operand32"
  (ior (match_operand 0 "pic30_any_mode2_operand")
       (match_operand 0 "pic30_modek_operand")))

(define_predicate "pic30_move_operand32"
  (ior (match_operand 0 "pic30_mode3_operand")
       (match_operand 0 "pic30_modek_operand")
       (match_operand 0 "pic30_near_operand")))

(define_predicate "pic30_move_operand"
  (and (match_code "subreg,reg,mem")
       (ior (match_operand 0 "pic30_mode3_operand")
            (match_operand 0 "pic30_modek_operand")
            (match_operand 0 "pic30_near_operand")
            (match_operand 0 "pic30_data_operand")
       )
  )
)

(define_predicate "pic30_movedi_operand"
  (and (match_code "mem,reg,subreg")
       (ior (match_operand 0 "pic30_mode2_operand")
            (match_operand 0 "pic30_modek_operand")
            (match_operand 0 "pic30_near_operand")
            (match_operand 0 "pic30_data_operand"))
  )
)

(define_predicate "pic30_moveb_operand"
  (ior (match_operand 0 "pic30_mode3_operand")
       (match_operand 0 "pic30_near_operand")
       (match_operand 0 "pic30_data_operand")))

(define_predicate "pic30_moveb_APSV_operand"
  (ior (match_operand 0 "pic30_mode3_APSV_operand")
       (match_operand 0 "pic30_near_operand")
       (match_operand 0 "pic30_data_operand")))

(define_predicate "pic30_move_APSV_operand"
  (ior (match_operand 0 "pic30_mode3_APSV_operand")
       (match_operand 0 "pic30_modek_APSV_operand")
       (match_operand 0 "pic30_near_operand")
       (match_operand 0 "pic30_data_operand")))

(define_predicate "pic30_move_d_operand"
  (ior (match_test      "pic30_mode1MinMax_operand(op,mode,\"O\")")
       (match_operand 0 "pic30_data_operand")
       (match_operand 0 "pic30_modek_operand")))

;;  { "pic30_invalid_address_operand", { SYMBOL_REF, LABEL_REF, CONST }}, 

(define_predicate "pic30_invalid_address_operand"
  (match_test "1")
{
  if (TARGET_CLASSIC) switch (GET_CODE(op)) {
    default: return FALSE;
    case REG: return (mode != machine_Pmode);
    case MEM: return pic30_invalid_address_operand(XEXP(op,0), mode);
    case LABEL_REF:  return 0;
    case PLUS: return pic30_invalid_address_operand(XEXP(op,0),mode);
    case SYMBOL_REF:
      if ((PIC30_HAS_NAME_P(XSTR(op,0), PIC30_PROG_FLAG)) ||
          (PIC30_HAS_NAME_P(XSTR(op,0), PIC30_AUXFLASH_FLAG))) {
        const char *real_name = pic30_strip_name_encoding_helper(XSTR(op,0));
        tree sym=0;

        sym = pic30_maybe_get_identifier(real_name);
        if (sym) sym = lookup_name(sym);
        if (sym) error("%JInappropriate program address '%s'", sym, real_name);
        else error("Inappropriate program address '%s'", real_name);
        return 1;
      }
      break;
    case CONST:
      op = XEXP (op, 0);
      if (((GET_CODE (XEXP (op, 0)) == SYMBOL_REF) ||
           (GET_CODE (XEXP (op, 0)) == LABEL_REF)) &&
          (GET_CODE (XEXP (op, 1)) == CONST_INT))
      return pic30_invalid_address_operand(XEXP(op,0), mode);
  }
  return 0;
})

;;  { "pic30_symbolic_address_operand", { SYMBOL_REF, LABEL_REF, CONST }}, 

(define_special_predicate "pic30_symbolic_address_operand"
  (match_test "1")
{
  int done;

  do {
    done = 1;
    switch (GET_CODE (op)) {
      case SUBREG:
        if (GET_MODE(op) != mode) break;
        op = XEXP(op,0);
        /* allow the cast from modes that do not require special handling */
        switch (GET_CODE(op)) {
          case SYMBOL_REF:
          case LABEL_REF: 
            switch (GET_MODE(op)) {
              default:  /* the outer type must match */
                        break;
              case E_QImode:
              case E_HImode:
              case E_SImode:
              case E_DImode:
              case E_SFmode:
              case E_DFmode:
              case E_P16APSVmode:
                mode = VOIDmode;
                break;
            }
            break;
          default:
            /* other operations do not require special handling, regardless of
               mode */
	    mode = VOIDmode;
            break;
        }
        done = 0;
        break;
      case SYMBOL_REF:
      case LABEL_REF:
        if ((mode == P24PSVmode) ||
            (mode == P24PROGmode) ||
            (mode == VOIDmode)) {
          int string_constant = 0;

          if (GET_CODE(op) == SYMBOL_REF) {
            tree exp;
            exp = SYMBOL_REF_DECL(op);

            /* I assume this can only happen in there is no SYMBOL_REF
               which means that this is coming from IV cost anaylis? */
            if (exp == 0) return TRUE;

            string_constant = ((TREE_CODE(exp) == STRING_CST) &&
                               (TARGET_CONST_IN_CODE || TARGET_CONST_IN_PSV));
          }
          if (((mode == VOIDmode) || (GET_MODE(op) == mode)) &&
              (string_constant || pic30_builtin_tblpsv_arg_p(NULL,op)))
            return TRUE;
        }
        if ((mode == P16APSVmode) || (mode == VOIDmode)) {
          if (pic30_has_space_operand_p(op,PIC30_APSV_FLAG)) return TRUE;
          if (pic30_data_space_operand_p(GET_MODE(op),op,0)) return TRUE;
        }
        if ((mode == P32DFmode) || (mode == VOIDmode)) {
          if (pic30_has_space_operand_p(op,PIC30_PACKEDFLASH_FLAG)) return TRUE;
        }
        if ((mode == P32EXTmode) || (mode == VOIDmode)) {
          if (pic30_has_space_operand_p(op,PIC30_EXT_FLAG)) return TRUE;
        }
        if ((mode == P16PMPmode) || (mode == VOIDmode)) {
          if (pic30_has_space_operand_p(op,PIC30_PMP_FLAG)) return TRUE;
        }
        if ((mode == P32UMMmode) || (mode == VOIDmode)) {
          if (pic30_eds_space_operand_p(op)) return TRUE;
        }
        if ((mode == P32EDSmode) ||
            (mode == P32PEDSmode) ||
            (mode == VOIDmode)) {
          /* these pointers are compatible with a multitude of spaces */
          if (pic30_eds_space_operand_p(op)) return TRUE;
        }
        if (mode == STACK_Pmode) return TRUE;
        if (mode == FN_Pmode) return TRUE;
        if (mode == Pmode) return TRUE;
        break;
      case CONST:
        op = XEXP (op, 0);
        if ((mode == P24PSVmode) ||
            (mode == P24PROGmode) ||
            (mode == VOIDmode)) {
          if (((GET_CODE (XEXP (op, 0)) == SYMBOL_REF ||
                    GET_CODE (XEXP (op, 0)) == LABEL_REF) &&
                   ((mode == VOIDmode) || (GET_MODE(XEXP(op,0)) == mode)) &&
                   (pic30_builtin_tblpsv_arg_p(NULL,op))) &&
                  GET_CODE (XEXP (op, 1)) == CONST_INT) return TRUE;
        }
        if ((mode == P16PMPmode) || (mode == P32EXTmode) || 
            (mode == P32DFmode)) {
          op = XEXP(op,0);
          done = 0;
          break;
        } else {
          return (((GET_CODE (XEXP (op, 0)) == SYMBOL_REF ||
                    GET_CODE (XEXP (op, 0)) == LABEL_REF)) &&
                  GET_CODE (XEXP (op, 1)) == CONST_INT);
        }
      default:
        break;
    }
  } while (!done);
  return FALSE;
})

(define_special_predicate "pic30_positive_symbolic_address_operand"
  (match_code "const,subreg,symbol_ref,label_ref,plus")
{
  int done;

  do {
    rtx lhs,rhs;
    done = 1;
    switch (GET_CODE (op)) {
      case SUBREG:
        if (GET_MODE(op) != mode) break;
        op = XEXP(op,0);
        /* allow the cast from modes that do not require special handling */
        switch (GET_CODE(op)) {
          case SYMBOL_REF:
          case LABEL_REF: 
            switch (GET_MODE(op)) {
              default:  /* the outer type must match */
                        break;
              case E_QImode:
              case E_HImode:
              case E_SImode:
              case E_DImode:
              case E_SFmode:
              case E_DFmode:
              case E_P16APSVmode:
                mode = VOIDmode;
                break;
            }
            break;
          default:
            /* other operations do not require special handling, regardless of
               mode */
	    mode = VOIDmode;
            break;
        }
        done = 0;
        break;
      case SYMBOL_REF:
      case LABEL_REF:
        if ((mode == P24PSVmode) ||
            (mode == P24PROGmode) ||
            (mode == VOIDmode)) {
          int string_constant = 0;

          if (GET_CODE(op) == SYMBOL_REF) {
            tree exp;
            exp = SYMBOL_REF_DECL(op);

            /* I assume this can only happen in there is no SYMBOL_REF
               which means that this is coming from IV cost anaylis? */
            if (exp == 0) return TRUE;

            string_constant = ((TREE_CODE(exp) == STRING_CST) &&
                               (TARGET_CONST_IN_CODE || TARGET_CONST_IN_PSV));
          }
          if (((mode == VOIDmode) || (GET_MODE(op) == mode)) &&
              (string_constant || pic30_builtin_tblpsv_arg_p(NULL,op)))
            return TRUE;
        }
        if ((mode == P16APSVmode) || (mode == VOIDmode)) {
          if (pic30_has_space_operand_p(op,PIC30_APSV_FLAG)) return TRUE;
          if (pic30_data_space_operand_p(GET_MODE(op),op,0)) return TRUE;
        }
        if ((mode == P32DFmode) || (mode == VOIDmode)) {
          if (pic30_has_space_operand_p(op,PIC30_PACKEDFLASH_FLAG)) return TRUE;
        }
        if ((mode == P32EXTmode) || (mode == VOIDmode)) {
          if (pic30_has_space_operand_p(op,PIC30_EXT_FLAG)) return TRUE;
        }
        if ((mode == P16PMPmode) || (mode == VOIDmode)) {
          if (pic30_has_space_operand_p(op,PIC30_PMP_FLAG)) return TRUE;
        }
        if ((mode == P32UMMmode) || (mode == VOIDmode)) {
          if (pic30_eds_space_operand_p(op)) return TRUE;
        }
        if ((mode == P32EDSmode) ||
            (mode == P32PEDSmode) ||
            (mode == VOIDmode)) {
          /* these pointers are compatible with a multitude of spaces */
          if (pic30_eds_space_operand_p(op)) return TRUE;
        }
        if (mode == STACK_Pmode) return TRUE;
        if (mode == FN_Pmode) return TRUE;
        if (mode == Pmode) return TRUE;
        break;
      case CONST:
        op = XEXP (op, 0);
        if (GET_CODE(op) == PLUS) {
          rhs = XEXP(op,1);
          if (GET_CODE(rhs) == CONST_INT) {
            if (INTVAL(rhs) < 0) return FALSE;
            if (INTVAL(rhs) > 32768) return FALSE;
          }
        }
        if ((mode == P24PSVmode) ||
            (mode == P24PROGmode) ||
            (mode == VOIDmode)) {
          if (((GET_CODE (XEXP (op, 0)) == SYMBOL_REF ||
                    GET_CODE (XEXP (op, 0)) == LABEL_REF) &&
                   ((mode == VOIDmode) || (GET_MODE(XEXP(op,0)) == mode)) &&
                   (pic30_builtin_tblpsv_arg_p(NULL,op))) &&
                  GET_CODE (XEXP (op, 1)) == CONST_INT) return TRUE;
        }
        if ((mode == P16PMPmode) || (mode == P32EXTmode) || 
            (mode == P32DFmode)) {
          op = XEXP(op,0);
          done = 0;
          break;
        } else {
          return (((GET_CODE (XEXP (op, 0)) == SYMBOL_REF ||
                    GET_CODE (XEXP (op, 0)) == LABEL_REF)) &&
                  GET_CODE (XEXP (op, 1)) == CONST_INT);
        }
      default:
        break;
    }
  } while (!done);
  return FALSE;
})

(define_special_predicate "pic30_negative_symbolic_address_operand"
  (match_code "const,subreg,symbol_ref,label_ref,plus")
{
  int done;

  do {
    rtx lhs,rhs;
    done = 1;
    switch (GET_CODE (op)) {
      case SUBREG:
        if (GET_MODE(op) != mode) break;
        op = XEXP(op,0);
        /* allow the cast from modes that do not require special handling */
        switch (GET_CODE(op)) {
          case SYMBOL_REF:
          case LABEL_REF: 
            switch (GET_MODE(op)) {
              default:  /* the outer type must match */
                        break;
              case E_QImode:
              case E_HImode:
              case E_SImode:
              case E_DImode:
              case E_SFmode:
              case E_DFmode:
              case E_P16APSVmode:
                mode = VOIDmode;
                break;
            }
            break;
          default:
            /* other operations do not require special handling, regardless of
               mode */
	    mode = VOIDmode;
            break;
        }
        done = 0;
        break;
      case SYMBOL_REF:
      case LABEL_REF:
        if ((mode == P24PSVmode) ||
            (mode == P24PROGmode) ||
            (mode == VOIDmode)) {
          int string_constant = 0;

          if (GET_CODE(op) == SYMBOL_REF) {
            tree exp;
            exp = SYMBOL_REF_DECL(op);

            /* I assume this can only happen in there is no SYMBOL_REF
               which means that this is coming from IV cost anaylis? */
            if (exp == 0) return TRUE;

            string_constant = ((TREE_CODE(exp) == STRING_CST) &&
                               (TARGET_CONST_IN_CODE || TARGET_CONST_IN_PSV));
          }
          if (((mode == VOIDmode) || (GET_MODE(op) == mode)) &&
              (string_constant || pic30_builtin_tblpsv_arg_p(NULL,op)))
            return TRUE;
        }
        if ((mode == P16APSVmode) || (mode == VOIDmode)) {
          if (pic30_has_space_operand_p(op,PIC30_APSV_FLAG)) return TRUE;
          if (pic30_data_space_operand_p(GET_MODE(op),op,0)) return TRUE;
        }
        if ((mode == P32DFmode) || (mode == VOIDmode)) {
          if (pic30_has_space_operand_p(op,PIC30_PACKEDFLASH_FLAG)) return TRUE;
        }
        if ((mode == P32EXTmode) || (mode == VOIDmode)) {
          if (pic30_has_space_operand_p(op,PIC30_EXT_FLAG)) return TRUE;
        }
        if ((mode == P16PMPmode) || (mode == VOIDmode)) {
          if (pic30_has_space_operand_p(op,PIC30_PMP_FLAG)) return TRUE;
        }
        if ((mode == P32UMMmode) || (mode == VOIDmode)) {
          if (pic30_eds_space_operand_p(op)) return TRUE;
        }
        if ((mode == P32EDSmode) ||
            (mode == P32PEDSmode) ||
            (mode == VOIDmode)) {
          /* these pointers are compatible with a multitude of spaces */
          if (pic30_eds_space_operand_p(op)) return TRUE;
        }
        if (mode == STACK_Pmode) return TRUE;
        if (mode == FN_Pmode) return TRUE;
        if (mode == Pmode) return TRUE;
        break;
      case CONST:
        op = XEXP (op, 0);
        if (GET_CODE(op) == PLUS) {
          rhs = XEXP(op,1);
          if (GET_CODE(rhs) == CONST_INT) {
            if (INTVAL(rhs) > 0) return FALSE;
            if (INTVAL(rhs) < -1023) return FALSE;
          }
        }
        if ((mode == P24PSVmode) ||
            (mode == P24PROGmode) ||
            (mode == VOIDmode)) {
          if (((GET_CODE (XEXP (op, 0)) == SYMBOL_REF ||
                    GET_CODE (XEXP (op, 0)) == LABEL_REF) &&
                   ((mode == VOIDmode) || (GET_MODE(XEXP(op,0)) == mode)) &&
                   (pic30_builtin_tblpsv_arg_p(NULL,op))) &&
                  GET_CODE (XEXP (op, 1)) == CONST_INT) return TRUE;
        }
        if ((mode == P16PMPmode) || (mode == P32EXTmode) || 
            (mode == P32DFmode)) {
          op = XEXP(op,0);
          done = 0;
          break;
        } else {
          return (((GET_CODE (XEXP (op, 0)) == SYMBOL_REF ||
                    GET_CODE (XEXP (op, 0)) == LABEL_REF)) &&
                  GET_CODE (XEXP (op, 1)) == CONST_INT);
        }
      default:
        break;
    }
  } while (!done);
  return FALSE;
})

;;  { "pic30_call_address_operand", { SYMBOL_REF, LABEL_REF, CONST, REG }}, 

(define_predicate "pic30_call_address_operand"
  (ior (match_code "reg")
       (match_operand 0 "pic30_symbolic_address_operand")))

(define_predicate "pic30_reg_or_symbolic_address"
  (ior (match_operand 0 "pic30_register_operand")
       (match_operand 0 "pic30_positive_symbolic_address_operand")))

(define_predicate "pic30_reg_or_symbolic_address_or_immediate"
  (ior (match_operand 0 "pic30_register_operand")
       (ior (match_operand 0 "immediate_operand")
            (match_operand 0 "pic30_positive_symbolic_address_operand"))))

;;  { "pic30_reg_or_zero_operand", { SUBREG, REG, CONST_INT }}, 

(define_special_predicate "pic30_reg_or_zero_operand"
  (ior (match_test "op == CONST0_RTX(mode)")
       (match_operand 0 "pic30_register_operand")))
 
;;  { "pic30_rR_or_zero_operand", { SUBREG, REG, MEM, CONST_INT }}, 

(define_special_predicate "pic30_rR_or_zero_operand"
  (ior (match_test "op == CONST0_RTX(mode)")
       (match_operand 0 "pic30_reg_or_R_operand")))

;;  { "pic30_O_operand", { CONST_INT }}, 

(define_special_predicate "pic30_O_operand"
  (match_test "op == CONST0_RTX(mode)"))

;;  { "pic30_I_operand", { CONST_INT }}, 

(define_special_predicate "pic30_I_operand"
  (match_test "op == CONST1_RTX(mode)"))

;;  { "pic30_PN_operand", { CONST_INT }}, 

(define_special_predicate "pic30_PN_operand"
  (ior (match_operand 0 "pic30_P_operand")
       (match_operand 0 "pic30_N_operand")))

;;  { "pic30_JMNP_operand", { CONST_INT }},

(define_special_predicate "pic30_JMNP_operand"
  (ior (match_operand 0 "pic30_JM_operand")
       (match_operand 0 "pic30_PN_operand")))

;;  { "pic30_reg_or_P_operand", { SUBREG, REG, CONST_INT }}, 

(define_special_predicate "pic30_reg_or_PN_operand"
  (ior (match_operand 0 "pic30_register_operand")
       (match_operand 0 "pic30_PN_operand")))

(define_special_predicate "pic30_reg_or_P_operand"
  (ior (match_operand 0 "pic30_register_operand")
       (match_operand 0 "pic30_P_operand")))

;;  { "pic30_rR_or_JN_operand", { MEM, SUBREG, REG, CONST_INT }}, 

(define_special_predicate "pic30_rR_or_JN_operand"
  (ior (match_operand 0 "pic30_reg_or_R_operand")
       (match_operand 0 "pic30_JN_operand")))

(define_special_predicate "pic30_rR_or_JN_APSV_operand"
  (ior (match_operand 0 "pic30_reg_or_R_APSV_operand")
       (match_operand 0 "pic30_JN_operand")))

;;  { "pic30_rR_or_JMNP_operand", { MEM, SUBREG, REG, CONST_INT }},

(define_special_predicate "pic30_rR_or_JMNP_operand"
  (ior (match_operand 0 "pic30_reg_or_R_operand")
       (match_operand 0 "pic30_JMNP_operand")))

(define_special_predicate "pic30_rR_or_JMNP_APSV_operand"
  (ior (match_operand 0 "pic30_reg_or_R_APSV_operand")
       (match_operand 0 "pic30_JMNP_operand")))

;;  { "pic30_Q_operand", { MEM }}, 

(define_predicate "pic30_Q_operand"
  (match_test "pic30_Q_constraint(op)"))

(define_predicate "pic30_any_QR_operand"
  (and (match_code "mem")
       (match_test "1"))
  {
    int disp;
    if (pic30_Q_constraint_displacement(op,&disp)) return 1;
    if (pic30_R_constraint(op)) return 1;
    return disp != INT_MAX;
  }
)

(define_predicate "pic30_RQ_operand"
  (and (match_code "mem")
       (ior (match_test "pic30_R_constraint(op)")
            (match_test "pic30_Q_constraint(op)"))
  )
)

/*
 * for movmem and the like, do not accept other address spaces
 *   this will fall through to the libcall method
 */
(define_predicate "pic30_memory_operand"
  (and (match_code "mem")
       (ior (match_test "MEM_ADDR_SPACE(op) == ADDR_SPACE_GENERIC")
            (match_test "MEM_ADDR_SPACE(op) == pic30_space_stack"))
       (match_operand 0 "memory_operand")))

(define_predicate "pic30_umm_memory_operand"
  (and (match_code "mem")
       (match_test "pic30_extended_pointer_operand(XEXP(op,0), P32UMMmode)")
       (match_operand 0 "memory_operand")))

(define_special_predicate "pic30_mem_umm_operand"
  (and (match_code "mem")
       (match_test "pic30_extended_pointer_operand(XEXP(op,0), P32UMMmode)")))
       
(define_predicate "pic30_mem_wtumm_operand"
  (and (match_code "mem")
       (match_test "GET_CODE(XEXP(op,0)) != POST_INC")
       (match_test "GET_CODE(XEXP(op,0)) != POST_DEC")
       (match_test "GET_CODE(XEXP(op,0)) != PRE_INC")
       (match_test "GET_CODE(XEXP(op,0)) != PRE_DEC")
       (match_test "pic30_extended_pointer_operand(XEXP(op,0), P32UMMmode)")
       (match_operand 0 "memory_operand")))

(define_predicate "pic30_RTU_operand"
  (and (match_code "mem")
       (ior (match_operand 0 "pic30_R_operand")
            (match_operand 0 "pic30_near_operand")
            (match_operand 0 "pic30_data_operand"))))

(define_predicate "pic30_mode2k_operand"
  (ior (match_operand 0 "pic30_mode2_operand")
       (match_operand 0 "pic30_modek_operand")))
  
(define_predicate "new_pic30_inc_imm_operand"
  (and (match_operand 0 "immediate_operand")
       (match_test "(-2<=INTVAL(op)) && (INTVAL(op)!=0) && (INTVAL(op)<=2)")))

(define_predicate "pic30_string_operand"
  (match_code "const_string"))

(define_predicate "pic30_reg_or_mem_operand"
  (ior (match_operand 0 "pic30_register_operand")
       (match_operand 0 "pic30_move_operand")))

(define_predicate "pic30_valid_operator"
  (ior (match_test "GET_RTX_CLASS(GET_CODE(op)) == '2'")
       (match_test "GET_RTX_CLASS(GET_CODE(op)) == 'c'"))
)
; do not allow things other than const int
(define_special_predicate "pic30_immediate_operand"
  (match_test "1")
{
  rtx x;

  x = op;
  while(GET_CODE(x) == CONST) x = XEXP(x,0);
  if (GET_CODE(x) == CONST_INT) return immediate_operand(op,mode);
  return FALSE;
})

(define_special_predicate "pic30_skip2_operator"
  (match_test "pic30_skip2_operator_helper(op,mode)")
)

(define_special_predicate "pic30_skip1_operator"
  (match_test "pic30_skip1_operator_helper(op,mode)")
)
; isa32 specific

;; accept (mem (reg
;;        (mem (symbol_ref
;;        (mem (plus (reg, reg)
;;        (mem (plus (reg, const_int)
;;        (mem (minus (reg, reg)
;;        (mem (minus (reg, const_int)

(define_predicate "pic30_float_mem_operand"
  (and (match_code "mem,subreg")
       (match_test "pic30_isa32_target()"))
  {  rtx inner;
     int result = 0;

     if ((GET_MODE(op) == DFmode) || (GET_MODE(op) == SFmode)) {

       while (GET_CODE(op) == SUBREG) op = XEXP(op,0);
       // at this point op better be mem
       if (GET_CODE(op) != MEM) return 0;
       inner = XEXP(op,0); 
       while (GET_CODE(inner) == CONST) inner = XEXP(inner,0);
       if ((GET_CODE(inner) == PLUS) ||
           (GET_CODE(inner) == MINUS)) {
         rtx inner0 = XEXP(inner,0);
         rtx inner1 = XEXP(inner,1);
         result = (((GET_CODE(inner0) == REG) || 
                   (GET_CODE(inner0) == SYMBOL_REF)) &&
                   (GET_CODE(inner1) == CONST_INT));
       }
       else if (GET_CODE(inner) == SYMBOL_REF) result = 1;
       else if (GET_CODE(inner) == REG) result = 1;
     }
     return result;
})

(define_predicate "pic30_float_reg_or_mem_operand"
  (and (match_test "pic30_isa32_target()")
       (ior (match_operand 0 "pic30_float_register_operand")
            (match_operand 0 "pic30_float_mem_operand")))
)

(define_predicate "pic30_comparison_operator"
  (and (match_test "pic30_isa32_target()")
       (match_operand 0 "comparison_operator"))
  {
    return ((GET_MODE(XEXP(op,0)) != SFmode) && 
            (GET_MODE(XEXP(op,0)) != DFmode));
  }
)

(define_predicate "pic30_dsp_word_offset_operand" 
  (and (match_test "pic30_isa32_target()")
       (match_operand 0 "immediate_operand"))
  {  return (INTVAL(op) == -8 ||
             INTVAL(op) == -6 ||
             INTVAL(op) == -4 ||
             INTVAL(op) == -2 ||
             INTVAL(op) ==  2 ||
             INTVAL(op) ==  4 ||
             INTVAL(op) ==  6 ||
             INTVAL(op) ==  8);
  }
)

(define_predicate "pic30_dsp_preinc_input_operand"
  (and (match_test "pic30_isa32_target()")
       (ior (match_code "reg,subreg")
            (and (match_code "mem")
                 (match_code "reg,plus,minus" "0"))
            (and (match_code "mem")
                 (match_code "plus,minus" "0")
                 (match_test "pic30_dsp_word_offset_operand(XEXP(op,1),
                                                            VOIDmode)"))))
)

(define_predicate "pic30_isa32_float_const_operand"
  (and (match_test "pic30_isa32_target()")
       (match_code "const_double")
       (match_test "pic30_isa32_float_const(op) >= 0"))
)
