;;- Machine description for GNU compiler
;;- Microchip dsPIC30 version.
;; Copyright (C) 1994, 1995, 1996, 1997 Free Software Foundation, Inc.

;; Contributed by John Elliott (john.elliott@microchip.com)

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
;;	This is the machine description for the Microchip dsPIC.
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
(define_attr "cc" "clobber,unchanged,set,change0,math,move"
  (const_string "clobber"))
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(include "predicates.md")
(include "constraints.md")

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define_mode_iterator GM16BIT [HI P16APSV P16PMP])
(define_mode_iterator SM16BIT [HI QQ HQ UQQ UHQ P16APSV P16PMP])
(define_mode_iterator GM32BIT [SI SF P24PSV P24PROG P32EDS P32PEDS P32EXT P32DF P32UMM])
(define_mode_iterator SM32BIT [SI SF P24PSV P24PROG P32EDS P32PEDS P32EXT P32DF SQ DQ TQ USQ UDQ UTQ P32UMM])
(define_mode_iterator SM48BIT [HA SA DA TA UHA USA UDA UTA])
(define_mode_iterator GM64BIT [DI DF])
(define_mode_iterator SM64BIT [DI DF])

(define_mode_iterator M32BIT_CLASSIC [P16PMP P24PSV P24PROG P32EDS P32PEDS P32DF P32UMM])
(define_mode_iterator M32BIT_SHIFT_CLASSIC [P24PSV P24PROG P32EDS P32PEDS P32EXT P32DF P32UMM])
(define_mode_iterator MXXBIT_CLASSIC_SUB [P16APSV P32EDS P32UMM])
(define_mode_iterator M32BIT_ISA32 [SI P32EXT ])
;; (define_mode_iterator M64BIT [DI DF])
(define_mode_iterator MODES32 [SI SF P32PEDS P32EDS P24PSV P24PROG P32EXT P32UMM])
(define_mode_iterator CMPMODES [QI HI SI DI QQ HQ UQQ UHQ SQ USQ DQ UDQ TQ UTQ HA UHA SA USA DA UDA TA UTA])
(define_mode_iterator CMPMODES16 [P32EDS P32UMM])
(define_mode_iterator CMPMODES32 [SF DF])
(define_mode_iterator EXT16BITRD [HI P16APSV P16PMP QQ HQ UQQ UHQ ])
(define_mode_iterator EXT16BITWT [HI P16APSV P16PMP QQ HQ UQQ UHQ ])
(define_mode_iterator A16BITADD [HI P16APSV P16PMP])
(define_mode_iterator TBLMODE [P24PSV P24PROG])
(define_mode_iterator Q15  [QQ HQ])
(define_mode_iterator Q31  [SQ DQ TQ])
(define_mode_iterator UQ15 [UQQ UHQ])
(define_mode_iterator UQ31 [USQ UDQ UTQ])
(define_mode_iterator QUQ15 [QQ UQQ HQ UHQ])
(define_mode_iterator QUQ31 [SQ USQ DQ UDQ TQ UTQ])
(define_mode_iterator QQQQQ [QQ UQQ HQ UHQ SQ USQ DQ UDQ TQ UTQ]) ; all Qs
(define_mode_iterator ACC   [HA SA DA TA])
(define_mode_iterator UACC  [UHA USA UDA UTA])
(define_mode_iterator AUACC [HA UHA SA USA DA UDA TA UTA])
(define_mode_iterator QRELOADS [QI HI SI DI SF DF QQ HQ UQQ UHQ SQ USQ DQ UDQ TQ UTQ HA UHA SA USA DA UDA TA UTA])
(define_mode_iterator QRELOADS_SCRATCH [DF DI])
(define_mode_iterator NORMAL_INT_MODES [QI HI SI DI])
(define_mode_iterator RESTRICTED_INT_MODES [QI HI SI])
(define_mode_iterator RIM16 [QI HI])
(define_mode_iterator RIM [QI HI SI])
(define_mode_iterator NORMAL_MODES [QI HI SI DI SF DF])
(define_mode_iterator FLOAT_MODES [SF DF])
(define_mode_iterator ALLRDMODES [QI HI QQ HQ UQQ UHQ P16APSV P16PMP SI SF P24PSV P24PROG P32EDS P32PEDS P32EXT P32DF SQ DQ TQ USQ UDQ UTQ P32UMM HA SA DA TA UHA USA UDA UTA DI DF])

;; Uses of UNSPEC in this file:

(define_constants
 [(UNSPEC_POPHI		1) ; pop HI
  (UNSPEC_PUSHSHADOW	2) ; push.s -- push shadowed registers
  (UNSPEC_POPSHADOW	3) ; pop.s -- pop shadowed registers
 ]
)

;; UNSPEC_VOLATILE:

(define_constants
 [
  (UNSPECV_BLOCKAGE	        0) ; block insn movement
  (UNSPECV_PA		        1) ; enable/disable PA
  (UNSPECV_IV		        2) ; interrupt vector
  (UNSPECV_PP		        3) ; pre-prologue
  (UNSPECV_NOP		        4) ; '__builtin_nop' instruction
  
  
  
  
  (UNSPECV_DIVSD                9) ; '__builtin_divsd' instruction
  (UNSPECV_DIVUD               10) ; '__builtin_divud' instruction
  (UNSPECV_MULSS               11) ; '__builtin_mulss' instruction
  (UNSPECV_MULSU               12) ; '__builtin_mulsu' instruction
  (UNSPECV_MULUS               13) ; '__builtin_mulus' instruction
  (UNSPECV_MULUU               14) ; '__builtin_muluu' instruction
  (UNSPECV_READSFR             15) ; '__builtin_readsfr' instruction
  (UNSPECV_DISI                16) ; disi instruction
  (UNSPECV_WRITESFR            17) ; '__builtin_writesfr' instruction
  (UNSPECV_WRITEDISICNT        18) ; 
  (UNSPECV_SAC                 19) ; __builtin_sac
  (UNSPECV_SACR                20) ; __builtin_sacr
  (UNSPECV_FBCL                21) ; __builtin_fbcl
  (UNSPECV_LAC                 22) ; __builtin_lac
  (UNSPECV_SFTAC               23) ; __builtin_sftac
  (UNSPECV_RRNC                24) ; rrnc
  (UNSPECV_TBLADDRESS          25) ; __builtin_tbladdress
  (UNSPECV_SETPSV              26) ; set psv
  (UNSPECV_WRITEOSCCONH        27) ; __builtin_write_OSCCONH
  (UNSPECV_WRITENVM            28) ; __builtin_write_NVM
  (UNSPECV_MODSD               29) ; __builtin_modsd
  (UNSPECV_MODUD               30) ; __builtin_modud
  (UNSPECV_DIVMODSD            31) ; __builtin_divmodsd
  (UNSPECV_DIVMODS             31) ; __builtin_divmodsd
  (UNSPECV_DIVMODUD            32) ; __builtin_divmodud
  (UNSPECV_DIVMODU             32) ; __builtin_divmodud
  (UNSPECV_DSPMULT             33) ; __builtin_mpy, etc
  (UNSEPCV_GENLABEL            34) ; __builtin_unique_id
  (UNSPECV_WRITEOSCCONL        35) ; __builtin_write_OSCCONL
  (UNSPECV_TBLRDL              36) ; __builtin_tblrdl
  (UNSPECV_TBLRDH              37) ; __builtin_tblrd7
  (UNSPECV_TBLWTL              38) ; __builtin_tblwtl
  (UNSPECV_TBLWTH              39) ; __builtin_tblwth
  (UNSPEC_SAVE_CONSTPSV        40)
  (UNSPECV_UNPACKEDS           41)
  (UNSPECV_P24PROGPTR          42)
  (UNSPECV_UNPACKMPSV          43)  
  (UNSPECV_UNPACKMPROG         44)  
  (UNSPECV_DIVF                45)
  (UNSPECV_WRITERTCWEN         46)
  (UNSPECV_PSVRDPSV            47) ; psv pointer access
  (UNSPECV_PSVRDPROG           48) ; psv pointer access
  (UNSPECV_PMPRD               49) ; PMP read
  (UNSPECV_PMPWT               50) ; PMP write
  (UNSPECV_EXTRD               51) ; EXT read
  (UNSPECV_EXTWT               52) ; EXT write
  (UNSPECV_WHILEPMMODE         53) ; while (!PMMODEbits.busy)
  (UNSPECV_PMDIN1RD            54) ; read PMPDIN1
  (UNSPECV_PMDIN1WT            55) ; write PMPDIN1
  (UNSPECV_SETPMMODE           56) 
  (UNSPECV_CLRPMMODE           57)
  (UNSPECV_TBLRDLB             58) ; __builtin_tblrdlb
  (UNSPECV_TBLRDHB             59) ; __builtin_tblrdhb
  (UNSPECV_TBLWTLB             60) ; __builtin_tblwtlb
  (UNSPECV_TBLWTHB             61) ; __builtin_tblwthb
  (UNSPECV_USEPSV              62) ; any operation that uses the PSV
  (UNSPECV_EDSRD               63) ;  EDS read
  (UNSPECV_EDSWT               64) ;  EDS write
  (UNSPECV_PEDSRD              65) ;  PEDS read  (paged)
  (UNSPECV_PEDSWT              66) ;  PEDS write (paged)
  (UNSPECV_SETNVPSV            67) ; set psv nonvolatile
  (UNSPECV_SETDSW              68) ; set dsw 
  (UNSPECV_SETNVDSW            69) ; set dsw nonvolatile
  (UNSPECV_OFFSET              70)  
  (UNSPECV_PAGE                71)  
  (UNSPECV_NOEDSRD             72)
  (UNSPECV_NOEDSWT             72)
  (UNSPECV_WRITEPWMCON         73) ; __builtin_write_PWMCON
  (UNSPECV_WRITECRTOTP         74) ; __builtin_write_CRYOTP
  (UNSPEC_SECTION_BEGIN        75)
  (UNSPEC_SECTION_END          76)
  (UNSPEC_SECTION_SIZE         77)
  (UNSPECV_SET_ISR_STATE       78)
  (UNSPEC_GET_ISR_STATE        79)
  (UNSPECV_ENABLE_ISR          80)
  (UNSPECV_DISABLE_ISR         81)
  (UNSPECV_ENABLE_ISR_GIE      82)
  (UNSPECV_DISABLE_ISR_GIE     83)
  (UNSPEC_EXTRACT_GIE          84)
  (UNSPEC_INSERT_GIE           85)
  (UNSPEC_EDSSTACKADDR         86)
  (UNSPECV_ENTER               87)
  (UNSPECV_EXIT                88)
  (UNSPEC_SATURATE             89)
  (UNSPECV_SETSAT              90)
  (UNSPEC_MPY                  91)
  (UNSPEC_FIXSIGN              92)
  (UNSPECV_SOFTWARE_BREAK      93)
  (UNSPECV_WRITEDATAFLASH      94) ; __builtin_write_DATAFLASH
  (UNSPEC_ADDR_LOW             95)
  (UNSPEC_ADDR_HI              96)
  (UNSPEC_ADDR                 97)
  (UNSPECV_PWRSAV              98)
  (UNSPECV_CLRWDT              99)
  (UNSPEC_ACCL                 100)
  (UNSPEC_ACCH                 101)
  (UNSPEC_ACCU                 102)
  (UNSPECV_ASSERT_PSV          103)
  (UNSPECV_SACD                104) ; __builtin_sacd
  (UNSPECV_LACD                105) ; __builtin_lacd
  (UNSPEC_EDSCONSTADDR         106)
  (UNSPECV_WRITEWRLOCK         107)
  (UNSPECV_PSVCONVERT          108)
  (UNSPEC_FF1L                 109)
  (UNSPEC_ASHIFTSI_LOW         110)
  (UNSPECV_WRITERPCON          111) ; __builtin_write_RPCON
  (UNSPECV_COVER               112) ; code coverage insn
  (UNSPECV_SETACC              113) ; generic set of accumulator to prevent
                                    ;  optimisation
  (UNSPEC_MIN                  114)
  (UNSPEC_MINV                 115)
  (UNSPEC_MINV2                116)
  (UNSPEC_MAX                  117)
  (UNSPEC_MAXV                 118)
  (UNSPEC_MAXV2                119)
  (UNSPEC_BTSC                 120)
  (UNSPEC_BTSS                 121)
  (UNSPECV_EDSDIVMODSD         122) ; __builtin_divmodsd
  (UNSPECV_EDSDIVMODUD         123) ; __builtin_divmodud
  (UNSPECV_EDSMOD              124) ; __builtin_divmodud
  (UNSPECV_PEDSBTG             125) ;  PEDS bit toggle
  (UNSPEC_FLIMV                126) ; distinguish FLIM from FLIMV
  (UNSPEC_UNIFIED_PUSH         127) ; not a builtin... used in prologue
  (UNSPEC_UNIFIED_POP          128) ; not a builtin... used in epilogue
  (UNSPEC_UNIFIED_WT           129)
  (UNSPEC_UNIFIED_RD           130)
  (UNSPEC_UMMPAGE              131)
  (UNSPEC_UMMOFFSET            132)
  (UNSPEC_WRITE_LOW            133)
  (UNSPECV_DO                  134)
  (UNSPECV_MOVCC               135)
  (UNSPEC_DIVMOD               136) ; generic divmod for isaxx
  (UNSPEC_UDIVMOD              137) ; generic divmod for isaxx
  (UNSPEC_DOLOOP_DEC           138) 
  (UNSPEC_DOLOOP_BRANCH        136) 
  (UNSPEC_COS                  137)
  (UNSPEC_SIN                  138)
  (UNSPEC_DIVMODF              139)
  (UNSPEC_FF1R                 140) ; for versions that don't conform
  (UNSPEC_EXTHI                141) ; highpart of p32ext for 32bit targets
  (UNSPECV_SET_FSR             142)
  (UNSPECV_SET_FCR             143)
  (UNSPECV_SET_FEAR            144)
  (UNSPECV_GET_FSR             145)
  (UNSPECV_GET_FCR             146)
  (UNSPECV_GET_FEAR            147)
  (UNSPECV_IOR_FCR             148)
  (UNSPECV_AND_FCR             149)
  (UNSPEC_ASHIFTSIHI           150) ; __builtin_ashifrt_32_16()
  (UNSPEC_LSHIFTSIHI           151) ; __builtin_lshifrt_32_16()
  (UNSPECV_BITCOPY             152) ; __builtin_bitcopy()
  (UNSPECV_TEMP                199)
 ]
)

;; Hard registers (SFRs) && other constants
;; duplicates... make sure they match the defininitions in pic30.h

(define_constants
  [
   (FPREG	    14) ; Frame-pointer hard register number
   (SPREG	    15) ; Stack-pointer hard register number
   (RCOUNT	    48) ; Repeat-count hard register number
   (A_REGNO         49)
   (B_REGNO         50)
   (PSVPAG          51) ; PSVPAG or DSRPAG
   (PMADDR          52) 
   (PMMODE          53)
   (PMDIN1          54)
   (PMDIN2          55)
   (DSWPAG          56)
   (CORCON          68) ; Any part of CORCON
   (FSR             69)
   (CC_REG          70)
  ]
)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Instruction scheduling
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;; Memory access attribute.
;; We only care about loads and stores.
;; use -- indirect memory load
;; def -- store to register
;; Everything else is 'etc'.
;;
;; For dealing with RAW stalls, we use the following truth table,
;; where D denotes a store to a wreg (definition), and U denotes an
;; indirect load of a source operand (use).
;;
;;           type   description
;; +---+---+
;; | D | U | 
;; +---+---+
;; | F | F | etc    Insn does not load indirect, does not store to wreg
;; | F | T | use    Insn loads indirect
;; | T | F | def    Insn stores to wreg
;; | T | T | defuse Insn loads indirect, and stores to wreg
;; +---+---+
;;
;; RAW stalls occur when D is followed by U.


(define_attr "tune" "classic,pipe_v1"
  (const (symbol_ref "((enum attr_tune) xc16_tune)")))

(define_automaton "dsPIC30F")

(define_cpu_unit "sched_raw" "dsPIC30F")


(define_insn_reservation "insn_def" 0
  (and (eq_attr "tune" "classic")
       (eq_attr "type" "def"))
  "sched_raw")

(define_insn_reservation "insn_use" 2
  (and (eq_attr "tune" "classic")
       (eq_attr "type" "use"))
  "sched_raw")

(define_insn_reservation "insn_defuse" 2
  (and (eq_attr "tune" "classic")
       (eq_attr "type" "defuse"))
  "sched_raw")

(define_insn_reservation "insn_etc" 0
  (and (eq_attr "tune" "classic")
       (eq_attr "type" "etc"))
  "nothing")

;; Insn OP type
;;  movlit  (a literal mov insn)
;;  mov     (a mov insn)
;;  alu     (an alu/non-mov insn)

(define_attr "op_type"
  "movlit,mov,alu,etc"
  (const_string "etc"))

;; Insn type.

(define_attr "type" 
  "def,use,defuse,etc"
  (const_string "etc"))

;set type like this
;  (set (attr "type")
;       (if_then_else (symbol_ref "TARGET_PIPE")
;                     (const_string pipe_line_style)
;                     (const_string RAW_style)))


;
; exch
;

(define_expand "exch"
 [(parallel [(set (match_operand:HI 0 "pic30_register_operand" "+r")
                  (match_operand:HI 1 "pic30_register_operand" "+r"))
             (set (match_dup 1)
                  (match_dup 0))]
 )]
 ""
 "
  {
    PIC30_TARGET_EXPAND(exch,(operands[0],operands[1]));
  }"
)


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; add clobber CC to instructions that clobber CC and set_flags
;
; used in transitioning from cc0 -> MODE_CC
; used in pic30-classic.md and pic30-isa32.md
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;; ;; CCmode
;;; 
;;; ;; the noclobber version ought not be generated and the default ""
;;; ;;   will mark the clobber of CC_REG
;;; (define_subst_attr "clobber_16_qi_cc" "subst_clobber_ccqi" "xxx" "yyy")
;;; 
;;; (define_subst "subst_clobber_ccqi"
;;;   [(set (match_operand:QI 0 "" "")
;;;         (match_operand:QI 1 "" ""))]
;;;   ""
;;;   [(set (match_dup 0)
;;;         (match_dup 1))
;;;    (clobber (reg:CC CC_REG))]
;;; )
;;; 
;;; (define_subst "subst_clobber_cchi"
;;;   [(set (match_operand:HI 0 "" "")
;;;         (match_operand:HI 1 "" ""))]
;;;   ""
;;;   [(set (match_dup 0)
;;;         (match_dup 1))
;;;    (clobber (reg:CC CC_REG))]
;;; )
;;; 
;;; ;; the noclobber version ought not be generated and the default ""
;;; ;;   will mark the clobber of CC_REG
;;; (define_subst_attr "clobber_16_hi_cc" "subst_clobber_cchi" "" "16")
;;; 
;;; (define_subst "subst_clobber_ccsi"
;;;   [(set (match_operand:SI 0 "" "")
;;;         (match_operand:SI 1 "" ""))]
;;;   ""
;;;   [(set (match_dup 0)
;;;         (match_dup 1))
;;;    (clobber (reg:CC CC_REG))]
;;; )
;;; 
;;; ;; the noclobber version ought not be generated and the default ""
;;; ;;   will mark the clobber of CC_REG
;;; (define_subst_attr "clobber_16_si_cc" "subst_clobber_ccsi" "" "16")
;;; 
;;; (define_subst "subst_clobber_ccdi"
;;;   [(set (match_operand:DI 0 "" "")
;;;         (match_operand:DI 1 "" ""))]
;;;   ""
;;;   [(set (match_dup 0)
;;;         (match_dup 1))
;;;    (clobber (reg:CC CC_REG))]
;;; )
;;; 
;;; ;; the noclobber version ought not be generated and the default ""
;;; ;;   will mark the clobber of CC_REG
;;; (define_subst_attr "clobber_16_di_cc" "subst_clobber_ccdi" "_noclobber" "16")
;;; 
;;; ;; CC_NZmode
;;; 
;;; (define_subst "subst_clobber_nzqi"
;;;   [(set (match_operand:QI 0 "" "")
;;;         (match_operand:QI 1 "" ""))]
;;;   ""
;;;   [(set (match_dup 0)
;;;         (match_dup 1))
;;;    (clobber (reg:CC_NZ CC_REG))]
;;; )
;;; 
;;; ;; the noclobber version ought not be generated and the default ""
;;; ;;   will mark the clobber of CC_REG
;;; (define_subst_attr "clobber_16_qi_nz" "subst_clobber_nzqi" "_noclobber" "16")
;;; 
;;; (define_subst "subst_clobber_nzhi"
;;;   [(set (match_operand:HI 0 "" "")
;;;         (match_operand:HI 1 "" ""))]
;;;   ""
;;;   [(set (match_dup 0)
;;;         (match_dup 1))
;;;    (clobber (reg:CC_NZ CC_REG))]
;;; )
;;; 
;;; ;; the noclobber version ought not be generated and the default ""
;;; ;;   will mark the clobber of CC_REG
;;; (define_subst_attr "clobber_16_hi_nz" "subst_clobber_nzhi" "_noclobber" "16")
;;; 
;;; (define_subst "subst_clobber_nzsi"
;;;   [(set (match_operand:SI 0 "" "")
;;;         (match_operand:SI 1 "" ""))]
;;;   ""
;;;   [(set (match_dup 0)
;;;         (match_dup 1))
;;;    (clobber (reg:CC_NZ CC_REG))]
;;; )
;;; 
;;; ;; the noclobber version ought not be generated and the default ""
;;; ;;   will mark the clobber of CC_REG
;;; (define_subst_attr "clobber_16_si_nz" "subst_clobber_nzsi" "_noclobber" "16")
;;; 
;;; (define_subst "subst_clobber_nzdi"
;;;   [(set (match_operand:DI 0 "" "")
;;;         (match_operand:DI 1 "" ""))]
;;;   ""
;;;   [(set (match_dup 0)
;;;         (match_dup 1))
;;;    (clobber (reg:CC_NZ CC_REG))]
;;; )
;;; 
;;; ;; the noclobber version ought not be generated and the default ""
;;; ;;   will mark the clobber of CC_REG
;;; (define_subst_attr "clobber_16_di_nz" "subst_clobber_nzdi" "_noclobber" "16")
;;; 
;;; ;; CC_NZCmode
;;; 
;;; (define_subst "subst_clobber_nzcqi"
;;;   [(set (match_operand:QI 0 "" "")
;;;         (match_operand:QI 1 "" ""))]
;;;   ""
;;;   [(set (match_dup 0)
;;;         (match_dup 1))
;;;    (clobber (reg:CC_NZC CC_REG))]
;;; )
;;; 
;;; ;; the noclobber version ought not be generated and the default ""
;;; ;;   will mark the clobber of CC_REG
;;; (define_subst_attr "clobber_16_qi_nzc" "subst_clobber_nzcqi" "_noclobber" "16")
;;; 
;;; (define_subst "subst_clobber_nzchi"
;;;   [(set (match_operand:HI 0 "" "")
;;;         (match_operand:HI 1 "" ""))]
;;;   ""
;;;   [(set (match_dup 0)
;;;         (match_dup 1))
;;;    (clobber (reg:CC_NZC CC_REG))]
;;; )
;;; 
;;; ;; the noclobber version ought not be generated and the default ""
;;; ;;   will mark the clobber of CC_REG
;;; (define_subst_attr "clobber_16_hi_nzc" "subst_clobber_nzchi" "_noclobber" "16")
;;; 
;;; (define_subst "subst_clobber_nzcsi"
;;;   [(set (match_operand:SI 0 "" "")
;;;         (match_operand:SI 1 "" ""))]
;;;   ""
;;;   [(set (match_dup 0)
;;;         (match_dup 1))
;;;    (clobber (reg:CC_NZC CC_REG))]
;;; )
;;; 
;;; ;; the noclobber version ought not be generated and the default ""
;;; ;;   will mark the clobber of CC_REG
;;; (define_subst_attr "clobber_16_si_nzc" "subst_clobber_nzcsi" "_noclobber" "16")
;;; 
;;; (define_subst "subst_clobber_nzcdi"
;;;   [(set (match_operand:DI 0 "" "")
;;;         (match_operand:DI 1 "" ""))]
;;;   ""
;;;   [(set (match_dup 0)
;;;         (match_dup 1))
;;;    (clobber (reg:CC_NZC CC_REG))]
;;; )
;;; 
;;; ;; the noclobber version ought not be generated and the default ""
;;; ;;   will mark the clobber of CC_REG
;;; (define_subst_attr "clobber_16_di_nzc" "subst_clobber_nzcdi" "_noclobber" "16")
;;; 
;;; 
;;; (define_subst "subst_clobber_zqi"
;;;   [(set (match_operand:QI 0 "" "")
;;;         (match_operand:QI 1 "" ""))]
;;;   ""
;;;   [(set (match_dup 0)
;;;         (match_dup 1))
;;;    (clobber (reg:CC_NZC CC_REG))]
;;; )
;;; 
;;; ;; the noclobber version ought not be generated and the default ""
;;; ;;   will mark the clobber of CC_REG
;;; (define_subst_attr "clobber_16_qi_z" "subst_clobber_zqi" "_noclobber" "16")
;;; 
;;; ;; CC_Zmode
;;; 
;;; (define_subst "subst_clobber_zhi"
;;;   [(set (match_operand:HI 0 "" "")
;;;         (match_operand:HI 1 "" ""))]
;;;   ""
;;;   [(set (match_dup 0)
;;;         (match_dup 1))
;;;    (clobber (reg:CC_Z CC_REG))]
;;; )
;;; 
;;; ;; the noclobber version ought not be generated and the default ""
;;; ;;   will mark the clobber of CC_REG
;;; (define_subst_attr "clobber_16_hi_z" "subst_clobber_zhi" "_noclobber" "16")
;;; 
;;; (define_subst "subst_clobber_zsi"
;;;   [(set (match_operand:SI 0 "" "")
;;;         (match_operand:SI 1 "" ""))]
;;;   ""
;;;   [(set (match_dup 0)
;;;         (match_dup 1))
;;;    (clobber (reg:CC_Z CC_REG))]
;;; )
;;; 
;;; ;; the noclobber version ought not be generated and the default ""
;;; ;;   will mark the clobber of CC_REG
;;; (define_subst_attr "clobber_16_si_z" "subst_clobber_zsi" "_noclobber" "16")
;;; 
;;; (define_subst "subst_clobber_zdi"
;;;   [(set (match_operand:DI 0 "" "")
;;;         (match_operand:DI 1 "" ""))]
;;;   ""
;;;   [(set (match_dup 0)
;;;         (match_dup 1))
;;;    (clobber (reg:CC_Z CC_REG))]
;;; )
;;; 
;;; ;; the noclobber version ought not be generated and the default ""
;;; ;;   will mark the clobber of CC_REG
;;; (define_subst_attr "clobber_16_di_z" "subst_clobber_zdi" "_noclobber" "16")
;;; 
;;; ;; CC_Cmode
;;; 
;;; (define_subst "subst_clobber_cqi"
;;;   [(set (match_operand:QI 0 "" "")
;;;         (match_operand:QI 1 "" ""))]
;;;   ""
;;;   [(set (match_dup 0)
;;;         (match_dup 1))
;;;    (clobber (reg:CC_C CC_REG))]
;;; )
;;; 
;;; ;; the noclobber version ought not be generated and the default ""
;;; ;;   will mark the clobber of CC_REG
;;; (define_subst_attr "clobber_16_qi_c" "subst_clobber_cqi" "_noclobber" "16")
;;; 
;;; (define_subst "subst_clobber_chi"
;;;   [(set (match_operand:HI 0 "" "")
;;;         (match_operand:HI 1 "" ""))]
;;;   ""
;;;   [(set (match_dup 0)
;;;         (match_dup 1))
;;;    (clobber (reg:CC_C CC_REG))]
;;; )
;;; 
;;; ;; the noclobber version ought not be generated and the default ""
;;; ;;   will mark the clobber of CC_REG
;;; (define_subst_attr "clobber_16_hi_c" "subst_clobber_chi" "_noclobber" "16")
;;; 
;;; (define_subst "subst_clobber_csi"
;;;   [(set (match_operand:SI 0 "" "")
;;;         (match_operand:SI 1 "" ""))]
;;;   ""
;;;   [(set (match_dup 0)
;;;         (match_dup 1))
;;;    (clobber (reg:CC_C CC_REG))]
;;; )
;;; 
;;; ;; the noclobber version ought not be generated and the default ""
;;; ;;   will mark the clobber of CC_REG
;;; (define_subst_attr "clobber_16_si_c" "subst_clobber_csi" "_noclobber" "16")
;;; 
;;; (define_subst "subst_clobber_cdi"
;;;   [(set (match_operand:DI 0 "" "")
;;;         (match_operand:DI 1 "" ""))]
;;;   ""
;;;   [(set (match_dup 0)
;;;         (match_dup 1))
;;;    (clobber (reg:CC_C CC_REG))]
;;; )
;;; 
;;; ;; the noclobber version ought not be generated and the default ""
;;; ;;   will mark the clobber of CC_REG
;;; (define_subst_attr "clobber_16_di_c" "subst_clobber_cdi" "" "16")

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;; dsp instructions
; DSP instructions
; 
; See pic30_classic.md for these patterns
;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; __builtin_unique_id

(define_insn "unique_id"
  [(set (match_operand:HI 0 "pic30_register_operand" "=r")
        (unspec_volatile:HI [ (match_operand 1 "immediate_operand" "i")
                              (match_operand:HI 2 "immediate_operand" "i")
                            ] UNSEPCV_GENLABEL))]
  ""
  "*
   {  static char buffer[80];
      char *label;

      label = (char *)(INTVAL(operands[1]));
      sprintf(buffer,\".global %s\n%s:\n\tmov.w #%ld,%%0\",
              label,label,INTVAL(operands[2]));
      return buffer;
   }
  "
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; __builtin_tblrdl, __builtin_tblrdh, __builtin_tblwtl, __builtin_tblwth

;; see pic30_classic.md

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; stackpush
;; These patterns are used for passing arguments on the stack.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define_expand "push<mode>1"
  [(set (mem:NORMAL_MODES (post_inc (reg SPREG)))
        (match_operand:NORMAL_MODES 0 "pic30_register_operand" "r"))]
  ""
  "
    PIC30_TARGET_EXPAND(push<mode>1,(operands[0]));
  "
)

(define_expand "push<mode>"
  [(set (match_operand:NORMAL_MODES 0 "push_operand"   "=>")
        (match_operand:NORMAL_MODES 1 "pic30_register_operand" "r"))]
  ""
  "
    PIC30_TARGET_EXPAND(push<mode>,(operands[0],operands[1]));
  "
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; stackpop
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define_expand "pop<mode>"
  [(set (match_operand:NORMAL_MODES 0 "pic30_register_operand" "=r")
        (match_operand:NORMAL_MODES 1 "pop_operand"       "<"))]
  ""
  "
    PIC30_TARGET_EXPAND(pop<mode>,(operands[0],operands[1]));
  "
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; stackcheck
;; If STACK_CHECK_BUILTIN is zero, and you define a named pattern called
;; check_stack in your 'md' file, GCC will call that pattern with one
;; argument which is the address to compare the stack value against.
;; You must arrange for this pattern to report an error if the stack
;; pointer is out of range.
;; (define_insn "check_stack"
;;   [(match_operand 0 "pic30_register_operand" "r")
;;    (clobber (match_scratch:HI 1 "=r"))]
;;   ""
;;   "mov .BOS,%1\;cpsge %0,%1\;reset")

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Test operations.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define_expand "tst<mode>"
  [(set (reg:CC CC_REG)
        (compare
           (match_operand:NORMAL_INT_MODES 0 "pic30_near_mode2_operand" "")
           (const_int 0))
   )
  ]
  ""
  "
    PIC30_TARGET_EXPAND(tst<mode>,(operands[0]));
  "
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

(define_expand "movstrhi"
  [(parallel [(set (match_operand:BLK 0 "pic30_memory_operand" "")
		   (match_operand:BLK 1 "pic30_memory_operand" ""))
	      (use (match_operand:HI 2 "const_int_operand" ""))
	      (use (match_operand:HI 3 "const_int_operand" ""))
	      (clobber (match_dup 4))
	      (clobber (match_dup 5))
   	      (clobber (reg:HI RCOUNT))
             ]
  )]
  "(TARGET_CLASSIC)"
  "
   PIC30_CLASSIC_EXPAND(movstrhi,
                        (operands[0],operands[1],operands[2],operands[3]));
  "
)

(define_expand "movstrsi"
  [(parallel [(set (match_operand:BLK 0 "pic30_memory_operand" "")
		   (match_operand:BLK 1 "pic30_memory_operand" ""))
	      (use (match_operand:SI 2 "const_int_operand" ""))
	      (use (match_operand:SI 3 "const_int_operand" ""))
	      (clobber (match_dup 4))
	      (clobber (match_dup 5))
   	      (clobber (reg:HI RCOUNT))
             ]
  )]
  "(TARGET_ISA32)"
  "
   PIC30_ISA32_EXPAND(movstrsi,
                      (operands[0],operands[1],operands[2],operands[3]));
  "
)

;
;  This pattern should restore the src/ dest ptrs before finishing
;
;  Perhaps these patterns should be simpler (CAW)... 
;
(define_expand "movmemhi"
  [(set (match_operand:BLK 0 "pic30_memory_operand"  "=R,m,R,R,m,R,m")
        (match_operand:BLK 1 "pic30_memory_operand"   "R,R,m,R,R,m,m"))
   (use (match_operand:HI 2 "immediate_operand" "J,J,J,i,i,i,i"))
   (use (match_operand:HI 3 "const_int_operand" ""))
  ]
  "(TARGET_CLASSIC) && ((pic30_errata_mask & ecc_errata) == 0)"
  "
   PIC30_CLASSIC_EXPAND(movmemhi,
                        (operands[0],operands[1],operands[2],operands[3]));
  "
)

(define_expand "movmemsi"
  [(set (match_operand:BLK 0 "pic30_memory_operand"  "=R,m,R,R,m,R,m")
        (match_operand:BLK 1 "pic30_memory_operand"   "R,R,m,R,R,m,m"))
   (use (match_operand:SI 2 "immediate_operand" "J,J,J,i,i,i,i"))
   (use (match_operand:SI 3 "const_int_operand" ""))
  ]
  "(TARGET_ISA32)"
  "
   PIC30_ISA32_EXPAND(movmemsi,(operands[0],operands[1],operands[2],operands[3]));
  "
)


;; Block clear.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Argument 0 is the destination
;; Argument 1 is the length
;; Argument 2 is the alignment
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define_expand "setmemhi"
  [
   (set (match_operand:BLK 0 "pic30_memory_operand"  "=R,m,R,m,R,m,R,m,R,m,R,m")
        (match_operand     2 "pic30_reg_or_imm_operand" 
                                                     "O,O,O,O,i,i,i,i,r,r,r,r"))
   (use (match_operand:HI 1 "immediate_operand" "J,J,i,i,J,J,i,i,J,J,i,i"))
   (use (match_operand:HI 3 "const_int_operand" ""))
  ]
  "(TARGET_CLASSIC)"
  "
   PIC30_CLASSIC_EXPAND(setmemhi,
                        (operands[0],operands[1],operands[2],operands[3]));
  "
)

(define_expand "setmemsi"
  [
   (set (match_operand:BLK 0 "pic30_memory_operand"  "=R,m,R,m,R,m,R,m,R,m,R,m")
        (match_operand     2 "pic30_reg_or_imm_operand" 
                                                     "O,O,O,O,i,i,i,i,r,r,r,r"))
   (use (match_operand:HI 1 "immediate_operand" "J,J,i,i,J,J,i,i,J,J,i,i"))
   (use (match_operand:HI 3 "const_int_operand" ""))
  ]
  "(TARGET_ISA32)"
  "
   PIC30_ISA32_EXPAND(setmemsi,
                      (operands[0],operands[1],operands[2],operands[3]));
  "
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; branch & compare instructions.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define_expand "cbranch<mode>4"
 [(set (reg:CC CC_REG)
       (compare (match_operand:CMPMODES 1 "pic30_mode1PN_operand" "")
                (match_operand:CMPMODES 2 "pic30_mode1PN_operand" "")))
  (set (pc)
       (if_then_else (match_operator 0 "comparison_operator"
                      [(reg:CC CC_REG) (const_int 0)])
                     (label_ref (match_operand 3 "" ""))
                     (pc)))]
  ""
  "
    PIC30_TARGET_EXPAND(cbranch<mode>4,
                          (operands[0],operands[1],operands[2],operands[3]));
  "
) 

(define_expand "cbranch<mode>4"
 [(set (reg:CC CC_REG)
       (compare (match_operand:CMPMODES16 1 "pic30_mode1PN_operand" "")
                (match_operand:CMPMODES16 2 "pic30_mode1PN_operand" "")))
  (set (pc)
       (if_then_else (match_operator 0 "comparison_operator"
                      [(reg:CC CC_REG) (const_int 0)])
                     (label_ref (match_operand 3 "" ""))
                     (pc)))]
  "TARGET_CLASSIC"
  "
    PIC30_CLASSIC_EXPAND(cbranch<mode>4,
                           (operands[0],operands[1],operands[2],operands[3]));
  " 
) 

(define_expand "cbranch<mode>4"
 [(set (reg:CC CC_REG)
       (compare (match_operand:CMPMODES32 1 "pic30_mode1PN_operand" "")
                (match_operand:CMPMODES32 2 "pic30_mode1PN_operand" "")))
  (set (pc)
       (if_then_else (match_operator 0 "comparison_operator"
                      [(reg:CC CC_REG) (const_int 0)])
                     (label_ref (match_operand 3 "" ""))
                     (pc)))]
  "TARGET_ISA32"
  "
    PIC30_ISA32_EXPAND(cbranch<mode>4,
                         (operands[0],operands[1],operands[2],operands[3]));
  " 
) 

(define_expand "cmp<mode>"
 [(set (reg:CC CC_REG)
       (compare 
         (match_operand:NORMAL_INT_MODES 0 "pic30_general_operand" "")
         (match_operand:NORMAL_INT_MODES 1 "pic30_general_operand" ""))
  )]
  ""
  "
    PIC30_TARGET_EXPAND(cmp<mode>,(operands[0],operands[1]));
  "
)

(define_expand "cmp<mode>"
 [(set (reg:CC CC_REG)
       (compare 
         (match_operand:CMPMODES32 0 "pic30_general_operand" "")
         (match_operand:CMPMODES32 1 "pic30_general_operand" ""))
  )]
  "TARGET_ISA32"
  "
    PIC30_ISA32_EXPAND(cmp<mode>,(operands[0],operands[1]));
  "
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; zero extension instructions
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define_expand "zero_extendqihi2"
  [(set (match_operand:HI 0 "pic30_register_operand"       "=r,r")
        (zero_extend:HI 
           (match_operand:QI 1 "pic30_mode2_APSV_operand"  " r,R<>")))]
  ""
  "
  {
    PIC30_TARGET_EXPAND(zero_extendqihi2, (operands[0],operands[1]));
  }")

(define_expand "zero_extendqisi2"
  [(set (match_operand:SI 0 "pic30_register_operand"       "")
        (zero_extend:HI
           (match_operand:QI 1 "pic30_mode2_APSV_operand"  "")))]
  ""
  "
  {
    PIC30_TARGET_EXPAND(zero_extendqisi2,(operands[0],operands[1]));
  }")


(define_expand "zero_extendqidi2"
  [(set (match_operand:DI   0 "pic30_register_operand" "=r,r")
        (zero_extend:DI 
          (match_operand:QI 1 "pic30_register_operand" " r,<>R")))
  ]
  ""
  "
    PIC30_TARGET_EXPAND(zero_extendqidi2,(operands[0],operands[1]));
  "
)

(define_expand "zero_extendhisi2"
  [(set (match_operand:SI   0 "pic30_register_operand" "=r")
        (zero_extend:SI 
          (match_operand:HI 1 "pic30_register_operand" " r")))
  ]
  ""
  "
    PIC30_TARGET_EXPAND(zero_extendhisi2,(operands[0],operands[1]));
  "
)

(define_expand "zero_extendhidi2"
  [(set (match_operand:DI 0 "pic30_register_operand"                "=r,r")
        (zero_extend:DI 
           (match_operand:HI 1 "pic30_register_operand"              "0,r"))
   )
  ]
  ""
  "
    PIC30_TARGET_EXPAND(zero_extendhidi2,(operands[0],operands[1]));
  "
)

(define_expand "zero_extendsidi2"
  [(set (match_operand:DI 0 "pic30_register_operand"  "=r,r,&r,r,r")
        (zero_extend:DI 
            (match_operand:SI 1 "pic30_mode2_operand"  "0,r, R,<,>")) 
   )
  ]
  ""
  "
    PIC30_TARGET_EXPAND(zero_extendsidi2,(operands[0],operands[1]));
  "
)

(define_expand "zero_extendhip32eds2"
  [(set (match_operand:P32EDS 0 "pic30_register_operand" "=r")
        (zero_extend:P32EDS
           (match_operand:HI  1 "pic30_register_operand" " r")))
  ]
  "(TARGET_CLASSIC)"
  "
     PIC30_CLASSIC_EXPAND(zero_extendhip32eds2,(operands[0],operands[1]));
  "
)

(define_expand "zero_extendsip32eds2"
  [(set (match_operand:P32EDS 0 "pic30_register_operand" "=r")
        (zero_extend:P32EDS
           (match_operand:SI  1 "pic30_register_operand" " r")))
  ]
  "(TARGET_CLASSIC)"
  "
     PIC30_CLASSIC_EXPAND(zero_extendsip32eds2,(operands[0],operands[1]));
  "
)

(define_expand "zero_extendhip32ext2"
  [(set (match_operand:P32EXT 0 "pic30_register_operand" "=r")
        (zero_extend:P32EXT
           (match_operand:HI  1 "pic30_register_operand" " r")))
  ]
  "(TARGET_CLASSIC)"
  "
     PIC30_CLASSIC_EXPAND(zero_extendhip32ext2,(operands[0],operands[1]));
  "
)

(define_expand "zero_extendsip32ext2"
  [(set (match_operand:P32EXT 0 "pic30_register_operand" "=r")
        (zero_extend:P32EXT
           (match_operand:SI  1 "pic30_register_operand" " r")))
  ]
  "(TARGET_CLASSIC)"
  "
     PIC30_CLASSIC_EXPAND(zero_extendsip32ext2,(operands[0],operands[1]));
  "
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; sign extension instructions
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define_expand "extendqihi2"
 [(set (match_operand:HI 0 "pic30_register_operand"                   "=r,r")
        (sign_extend:HI (match_operand:QI 1 "pic30_mode2_APSV_operand" "r,R<>"))
   )
  ]
  ""
  "
    PIC30_TARGET_EXPAND(extendqihi2,(operands[0],operands[1]));
  "
)

(define_expand "extendqisi2"
  [(set (match_operand:SI   0 "pic30_register_operand" "=r,r")
        (sign_extend:SI 
          (match_operand:QI 1 "pic30_mode2_operand"     "r,R<>")))
  ]
  ""
  "
    PIC30_TARGET_EXPAND(extendqisi2,(operands[0],operands[1]));
  "
)

(define_expand "extendqidi2"
  [(set (match_operand:DI   0 "pic30_register_operand" "=r,r")
        (sign_extend:DI 
          (match_operand:QI 1 "pic30_mode2_operand"    " r,R<>")))
  ]
  ""
  "
    PIC30_TARGET_EXPAND(extendqidi2,(operands[0],operands[1]));
  "
)

(define_expand "extendhisi2"
  [(set (match_operand:SI 0 "pic30_register_operand"   "=r")
        (sign_extend:SI 
           (match_operand:HI 1 "pic30_register_operand" "r")) 
   )
  ]
  ""
  "
    PIC30_TARGET_EXPAND(extendhisi2,(operands[0],operands[1]));
  "
)

(define_expand "extendhidi2"
  [(set (match_operand:DI 0 "pic30_register_operand"                "=r,r")
        (sign_extend:DI (match_operand:HI 1 "pic30_register_operand" "0,r")) )]
  ""
  "
    PIC30_TARGET_EXPAND(extendhidi2,(operands[0],operands[1]));
  "
)

(define_expand "extendsidi2"
  [(set (match_operand:DI 0 "pic30_register_operand"    "=r,r")
        (sign_extend:DI 
            (match_operand:SI 1 "pic30_register_operand" "0,r")) 
   )
  ]
  ""
  "
    PIC30_TARGET_EXPAND(extendsidi2,(operands[0],operands[1]));
  "
)

(define_expand "extendhip32eds2"
  [(set (match_operand:P32EDS 0 "pic30_register_operand" "=r")
        (sign_extend:P32EDS
           (match_operand:HI  1 "pic30_register_operand" " r")))
  ]
  "(TARGET_CLASSIC)"
  "
     PIC30_CLASSIC_EXPAND(extendhip32eds2,(operands[0],operands[1]));
  "
)

(define_expand "extendsip32eds2"
  [(set (match_operand:P32EDS 0 "pic30_register_operand" "=r")
        (sign_extend:P32EDS
           (match_operand:SI  1 "pic30_register_operand" " r")))
  ]
  "(TARGET_CLASSIC)"
  "
     PIC30_CLASSIC_EXPAND(extendsip32eds2,(operands[0],operands[1]));
  "
)

(define_expand "extendhip32ext2"
  [(set (match_operand:P32EXT 0 "pic30_register_operand" "=r")
        (sign_extend:P32EXT
           (match_operand:HI  1 "pic30_register_operand" " r")))
  ]
  "(TARGET_CLASSIC)"
  "
     PIC30_CLASSIC_EXPAND(extendhip32ext2,(operands[0],operands[1]));
  "
)

(define_expand "extendsip32ext2"
  [(set (match_operand:P32EXT 0 "pic30_register_operand" "=r")
        (sign_extend:P32EXT
           (match_operand:SI  1 "pic30_register_operand" " r")))
  ]
  "(TARGET_ISA32)"
  "
     PIC30_ISA32_EXPAND(extendsip32ext2,(operands[0],operands[1]));
  "
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Move instructions
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;; Invalid move

(define_insn "*movqi_invalid_1"
  [(set (match_operand:QI 0 "pic30_register_operand"  "=r")
        (match_operand:QI 1 "pic30_code_operand" "g"))]
  "(TARGET_CLASSIC)"
  "*
   {
     error(\"invalid address space for operand\");
     return \"cannot generate instruction\";
   }
  "
)

(define_insn "*movqi_invalid_2"
  [(set (match_operand:QI 0 "pic30_code_operand" "=g")
        (match_operand:QI 1 "pic30_register_operand"    "r"))]
  ""
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

(define_insn "*movqi_const0"
  [(set (match_operand:QI 0 "pic30_mode2_operand" "=r,R<>")
	(const_int 0))]
  ""
  "@
   clr.b %0
   clr.b %0"
  [
    (set_attr "cc" "change0,change0")
    (set_attr "type" "def,etc")
    (set_attr "op_type" "mov")
  ]
)

(define_insn "*movqi_const1"
  [(set (match_operand:QI 0 "pic30_mode2_operand" "=r,R<>")
	(const_int -1))]
  ""
  "@
   setm.b %0
   setm.b %0"
  [
    (set_attr "cc" "change0,change0")
    (set_attr "type" "def,etc")
    (set_attr "op_type" "mov")
  ]
)

(define_insn "*movqi_const0sfr"
  [(set (match_operand:QI 0 "pic30_near_operand" "=U")
	(const_int 0))]
  ""
  "clr.b %0"
  [
    (set_attr "cc" "change0")
    (set_attr "op_type" "mov")
  ]
)

(define_insn "*movqi_const1sfr"
  [(set (match_operand:QI 0 "pic30_near_operand" "=U")
	(const_int -1))]
  ""
  "setm.b %0"
  [
    (set_attr "cc" "change0")
    (set_attr "op_type" "mov")
  ]
)

; general case

(define_expand "movqi"
  [(set (match_operand:QI 0 "pic30_general_operand" "")
        (match_operand:QI 1 "pic30_general_operand" ""))]
  ""
  "
    PIC30_TARGET_EXPAND(movqi,(operands[0],operands[1]));
  "
)

;; Invalid move


(define_insn "*movhi_invalid_2"
  [(set (match_operand:HI 0 "pic30_code_operand" "=g")
        (match_operand:HI 1 "pic30_register_operand"  "r"))]
  ""
  "*
   {
        error(\"invalid address space for operand\");
        return \"cannot generate instruction\";

   }
")

(define_insn "*movhi_invalid_3"
  [(set (match_operand:HI 0 "pic30_move_operand" "")
        (mem: HI (match_operand:HI 1 "pic30_invalid_address_operand"  "")))]
  ""
  "*
   {
        error(\"invalid address space for operand\");
        return \"cannot generate instruction\";
   }
")

(define_insn "*movhi_invalid_4"
  [(set (mem (match_operand:P24PROG 0 "pic30_prog_operand" ""))
        (match_operand 1 "general_operand" ""))]
  ""
  "*
   {
     error(\"invalid address space for destination\");
     return \"cannot generate instruction\";
   }
")

(define_insn "*movhi_invalid_5"
  [(set (mem (match_operand:P24PSV 0 "pic30_psv_operand" ""))
        (match_operand 1 "general_operand" ""))]
  ""
  "*
   {
     error(\"invalid address space for destination\");
     return \"cannot generate instruction\";

   }
")

(define_insn "*movhi_invalid_6"
  [(set (mem (match_operand:P32DF 0 "pic30_df_operand" ""))
        (match_operand 1 "general_operand" ""))]
  ""
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

;(define_insn "*movhi_const0sfr"
;  [(set (match_operand:HI 0 "pic30_near_operand" "=U")
;	(const_int 0))]
;  ""
;  "clr.w %0"
;  [
;    (set_attr "cc" "change0")
;    (set_attr "type" "etc")
;    (set_attr "op_type" "mov")
;  ]
;)
;
;(define_insn "*movhi_const1sfr"
;  [(set (match_operand:HI 0 "pic30_near_operand" "=U")
;	(const_int -1))]
;  ""
;  "setm.w %0"
;  [
;    (set_attr "cc" "change0")
;    (set_attr "type" "etc")
;    (set_attr "op_type" "mov")
;  ]
;)
;
;(define_insn "movhi_const0"
;  [(set (match_operand:HI 0 "pic30_mode2_operand" "=r,R<>")
;	(const_int 0))]
;  ""
;  "@
;   clr.w %0
;   clr.w %0"
;  [
;   (set_attr "cc" "change0,change0")
;   (set_attr "type" "def,etc")
;    (set_attr "op_type" "mov")
;  ]
;)
;
;(define_insn "*movhi_const1"
;  [(set (match_operand:HI 0 "pic30_mode2_operand" "=r,R<>")
;	(const_int -1))]
;  ""
;  "@
;   setm.w %0
;   setm.w %0"
;  [
;   (set_attr "cc" "change0")
;   (set_attr "type" "def,etc")
;    (set_attr "op_type" "mov")
;  ]
;)

;;;;;;;;;;;;;;;;;;;;
;; general immediate
;;;;;;;;;;;;;;;;;;;;


;;;;;;;;;;;;;;;
;; general case
;;;;;;;;;;;;;;;

(define_expand "movhi"
  [(set (match_operand:HI 0 "pic30_move_operand" "")
        (match_operand:HI 1 "pic30_move_operand" ""))]
  ""
  "
  { 
     PIC30_TARGET_EXPAND(movhi,(operands[0],operands[1]));
  }"
)

;; rot

;;;;;;;;;;;;;;;;;;;;;;; EXT

;;

(define_expand "P32EXTrd_qi"
   [(set (match_operand:QI 0 "pic30_move_operand"       "=r,R,rR,r,R")
         (mem:QI
           (match_operand:P32EXT 1 "pic30_ext_operand" "r,r,RQ,ST,ST")))]
   ""
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

(define_expand "P32EXTwt_qi"
   [(set (mem:QI (match_operand:P32EXT 0 "pic30_ext_operand" "r,r,RQ,ST,ST"))
         (match_operand:QI 1 "pic30_move_operand"            "r,R,rR,r,R"))]
   ""
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

(define_expand "P32EXTrd_<mode>"
   [(set (match_operand:EXT16BITRD 0 "pic30_move_operand"       "=r,R,rR,r,R")
         (mem:EXT16BITRD
           (match_operand:P32EXT 1 "pic30_ext_operand" "r,r,RQ,ST,ST")))]
   ""
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
          PIC30_TARGET_GEN(gen_addp32ext3_imm)(input, input, GEN_INT(1))
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

(define_expand "P32EXTwt_<mode>"
   [(set (mem:EXT16BITWT
           (match_operand:P32EXT 0 "pic30_ext_operand"          "r,r,RQ,ST,ST"))
         (match_operand:EXT16BITWT 1 "pic30_move_operand"       "r,R,rR,r,R"))]
   ""
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
          PIC30_TARGET_GEN(gen_addp32ext3_imm)(input, input, GEN_INT(1))
        );
        emit_insn(
          gen_addqi3(input1, input1, GEN_INT(1))
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

(define_expand "P32EXTrd_p24psv"
   [(set (match_operand:P24PSV 0 "pic30_move_operand"       "=r,R,rR,r,R")
         (mem:P24PSV
           (match_operand:P32EXT 1 "pic30_ext_operand" "r,r,RQ,ST,ST")))]
   ""
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
              \"\tdeclare __write_external or __write_external32\");
      }
    }"
)

(define_expand "P32EXTwt_p24psv"
   [(set (mem:P24PSV 
           (match_operand:P32EXT 0 "pic30_ext_operand" "r,r,RQ,ST,ST"))
         (match_operand:P24PSV 1 "pic30_move_operand"  "r,R,rR,r,R"))]
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

(define_expand "P32EXTrd_p24prog"
   [(set (match_operand:P24PROG 0 "pic30_move_operand"       "=r,R,rR,r,R")
         (mem:P24PROG
           (match_operand:P32EXT 1 "pic30_ext_operand" "r,r,RQ,ST,ST")))]
   ""
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
              \"\tdeclare __write_external or __write_external32\");
      }
    }"
)

(define_expand "P32EXTwt_p24prog"
   [(set (mem:P24PROG 
           (match_operand:P32EXT 0 "pic30_ext_operand" "r,r,RQ,ST,ST"))
         (match_operand:P24PROG 1 "pic30_move_operand" "r,R,rR,r,R"))]
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

(define_expand "P32EXTrd_si"
   [(set (match_operand:SI 0 "pic30_move_operand"       "=r,R,rR,r,R")
         (mem:SI
           (match_operand:P32EXT 1 "pic30_ext_operand" "r,r,RQ,ST,ST")))]
   ""
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

(define_expand "P32EXTwt_si"
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

(define_expand "P32EXTrd_p32eds"
   [(set (match_operand:P32EDS 0 "pic30_move_operand"       "=r,R,rR,r,R")
         (mem:P32EDS
           (match_operand:P32EDS 1 "pic30_ext_operand" "r,r,RQ,ST,ST")))]
   ""
   "{
      if (pic30_read_externals(pst_32)) {
        rtx input = gen_reg_rtx(P32EDSmode);

        emit_move_insn(input, operands[1]);
        emit_library_call_value(XEXP(DECL_RTL(pic30_read_externals(pst_32)),0),
                          operands[0], LCT_NORMAL, GET_MODE(operands[0]), 
                          input, P32EDSmode);
        DONE;
      } else if (pic30_read_externals(pst_any)) {
        rtx input = gen_reg_rtx(P32EDSmode);
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
                          input, P32EDSmode,
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

(define_expand "P32EXTrd_p32peds"
   [(set (match_operand:P32PEDS 0 "pic30_move_operand"       "=r,R,rR,r,R")
         (mem:P32PEDS
           (match_operand:P32PEDS 1 "pic30_ext_operand" "r,r,RQ,ST,ST")))]
   ""
   "{
      if (pic30_read_externals(pst_32)) {
        rtx input = gen_reg_rtx(P32PEDSmode);

        emit_move_insn(input, operands[1]);
        emit_library_call_value(XEXP(DECL_RTL(pic30_read_externals(pst_32)),0),
                          operands[0], LCT_NORMAL, GET_MODE(operands[0]), 
                          input, P32EDSmode);
        DONE;
      } else if (pic30_read_externals(pst_any)) {
        rtx input = gen_reg_rtx(P32PEDSmode);
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
                          input, P32PEDSmode,
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

(define_expand "P32EXTrd_p32ext"
   [(set (match_operand:P32EXT 0 "pic30_move_operand"       "=r,R,rR,r,R")
         (mem:P32EXT
           (match_operand:P32EXT 1 "pic30_ext_operand" "r,r,RQ,ST,ST")))]
   ""
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

(define_expand "P32EXTwt_p32eds"
   [(set (mem:P32EDS 
           (match_operand:P32EDS 0 "pic30_ext_operand" "r,r,RQ,ST,ST"))
         (match_operand:P32EDS 1 "pic30_move_operand"  "r,R,rR,r,R"))]
   ""
   "{
      if (pic30_write_externals(pst_32)) {
        rtx input = gen_reg_rtx(P32EDSmode);
        rtx input1 = gen_reg_rtx(GET_MODE(operands[1]));

        emit_move_insn(input, operands[0]);
        emit_move_insn(input1, operands[1]);
        emit_library_call(XEXP(DECL_RTL(pic30_write_externals(pst_32)),0),
                          LCT_NORMAL, VOIDmode,
                          input, P32EDSmode,
                          input1, GET_MODE(operands[1]));
        DONE;
      } else if (pic30_write_externals(pst_any)) {
        rtx input = gen_reg_rtx(P32EDSmode);
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
                          input, P32EDSmode,
                          param2, Pmode,
                          param3, HImode);
        DONE;
      } else {
        error(\"Cannot access external memory space;\n\"
              \"\tdeclare __write_external or __write_external32\");
      }
    }"
)

(define_expand "P32EXTwt_p32peds"
   [(set (mem:P32PEDS 
           (match_operand:P32PEDS 0 "pic30_ext_operand" "r,r,RQ,ST,ST"))
         (match_operand:P32PEDS 1 "pic30_move_operand"  "r,R,rR,r,R"))]
   ""
   "{
      if (pic30_write_externals(pst_32)) {
        rtx input = gen_reg_rtx(P32PEDSmode);
        rtx input1 = gen_reg_rtx(GET_MODE(operands[1]));

        emit_move_insn(input, operands[0]);
        emit_move_insn(input1, operands[1]);
        emit_library_call(XEXP(DECL_RTL(pic30_write_externals(pst_32)),0),
                          LCT_NORMAL, VOIDmode,
                          input, P32PEDSmode,
                          input1, GET_MODE(operands[1]));
        DONE;
      } else if (pic30_write_externals(pst_any)) {
        rtx input = gen_reg_rtx(P32PEDSmode);
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
                          input, P32PEDSmode,
                          param2, Pmode,
                          param3, HImode);
        DONE;
      } else {
        error(\"Cannot access external memory space;\n\"
              \"\tdeclare __write_external or __write_external32\");
      }
    }"
)

(define_expand "P32EXTwt_p32ext"
   [(set (mem:P32EXT 
           (match_operand:P32EXT 0 "pic30_ext_operand" "r,r,RQ,ST,ST"))
         (match_operand:P32EXT 1 "pic30_move_operand"  "r,R,rR,r,R"))]
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

(define_expand "P32EXTrd_sf"
   [(set (match_operand:SF 0 "pic30_move_operand"       "=r,R,rR,r,R")
         (mem:SF
           (match_operand:P32EXT 1 "pic30_ext_operand" "r,r,RQ,ST,ST")))]
   ""
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

(define_expand "P32EXTwt_sf"
   [(set (mem:SF (match_operand:P32EXT 0 "pic30_ext_operand" "r,r,RQ,ST,ST"))
         (match_operand:SF 1 "pic30_move_operand"            "r,R,rR,r,R"))]
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

(define_expand "P32EXTrd_di"
   [(set (match_operand:DI 0 "pic30_move_operand"       "=r,R,rR,r,R")
         (mem:DI
           (match_operand:P32EXT 1 "pic30_ext_operand" "r,r,RQ,ST,ST")))]
   ""
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

(define_expand "P32EXTwt_di"
   [(set (mem:DI (match_operand:P32EXT 0 "pic30_ext_operand" "r,r,RQ,ST,ST"))
         (match_operand:DI 1 "pic30_move_operand"            "r,R,rR,r,R"))]
   ""
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

(define_expand "P32EXTrd_df"
   [(set (match_operand:DF 0 "pic30_move_operand"       "=r,R,rR,r,R")
         (mem:DF
           (match_operand:P32EXT 1 "pic30_ext_operand" "r,r,RQ,ST,ST")))]
   ""
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

(define_expand "P32EXTwt_df"
   [(set (mem:DF (match_operand:P32EXT 0 "pic30_ext_operand" "r,r,RQ,ST,ST"))
         (match_operand:DF 1 "pic30_move_operand"            "r,R,rR,r,R"))]
   ""
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

(define_insn "*movsi_invalid_1"
  [(set (match_operand:SI 0 "pic30_register_operand"  "=r")
        (match_operand:SI 1 "pic30_code_operand" "g"))]
  "(TARGET_CLASSIC)"
  "*
{
        error(\"invalid address space for operand\");
        return \"cannot generate instructions\";
}

")
(define_insn "*movsi_invalid_2"
  [(set (match_operand:SI 0 "pic30_code_operand" "=g")
        (match_operand:SI 1 "pic30_register_operand"    "r"))]
  ""
  "*
{
        error(\"invalid address space for operand\");
        return \"cannot generate instructions\";
}
")

(define_expand "movsi_address"
  [(set (match_operand:SI 0 "pic30_register_operand"        "=r")
        (match_operand:SI 1 "pic30_symbolic_address_operand" "qs"))]
  "(TARGET_ISA32)"
  "
    emit(gen_movsi_address_32(operands[0],operands[1]));
  "
)

(define_expand "mov<mode>_gen"
  [(set 
    (match_operand:M32BIT_ISA32 0 "pic30_move_operand" "")
    (match_operand:M32BIT_ISA32 1 "pic30_move_operand" "")
   )]
  ""
  "
    PIC30_TARGET_EXPAND(mov<mode>_gen,(operands[0],operands[1]));
  "
)

(define_expand "mov<mode>"
  [(set (match_operand:M32BIT_CLASSIC 0 "pic30_move_operand" "")
        (match_operand:M32BIT_CLASSIC 1 "pic30_move_operand" "")
   )]
  "(TARGET_CLASSIC)"
  "
    PIC30_CLASSIC_EXPAND(mov<mode>, (operands[0],operands[1]));
  "
)


(define_expand "movsi"
  [(set (match_operand:SI 0 "pic30_move_operand" "")
        (match_operand:SI 1 "pic30_move_operand" ""))]
  ""
  "
    PIC30_TARGET_EXPAND(movsi,(operands[0],operands[1]));
  "
)

(define_expand "movp32ext"
  [(set (match_operand:P32EXT 0 "pic30_move_operand" "")
        (match_operand:P32EXT 1 "pic30_move_operand" ""))]
  ""
  "
    PIC30_TARGET_EXPAND(movp32ext,(operands[0],operands[1]));
  "
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; 64-bit integer moves
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;; Invalid move

(define_insn "*movdi_invalid_1"
  [(set (match_operand:DI 0 "pic30_register_operand"  "=r")
        (match_operand:DI 1 "pic30_code_operand" "g"))]
  "(TARGET_CLASSIC)"
  "*
{
        error(\"invalid address space for operand\");
        return \"cannot generate instructions\";
   }"
)

(define_insn "*movdi_invalid_2"
  [(set (match_operand:DI 0 "pic30_code_operand" "=g")
        (match_operand:DI 1 "pic30_register_operand"    "r"))]
  ""
  "*
{
        error(\"invalid address space for operand\");
        return \"cannot generate instructions\";
   }"
)

;
; simplify the options and make reload generate the complex ones we used to
;   support.  This should result in better code generation
;

(define_expand "movdi_gen"
  [(set (match_operand:DI 0 "pic30_movedi_operand"
					"=r,r,&r,R,>,&r,<,TU,r,r,r,Q")
        (match_operand:DI 1 "pic30_movedi_operand" 
					 "r,R, >,r,r, <,r,r, T,U,Q,r"))]
  ""
  "
    PIC30_TARGET_EXPAND(movdi_gen,(operands[0],operands[1]));
  "
)


(define_expand "mov<mode>"
  [(set (match_operand:GM64BIT 0 "pic30_general_operand" "")
        (match_operand:GM64BIT 1 "pic30_general_operand" ""))]
  ""
  "
  {
    PIC30_TARGET_EXPAND(mov<mode>,(operands[0],operands[1]));
  }
  "
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Single Float (32 bit) moves
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;; Invalid move

(define_insn "*movsf_invalid_1"
  [(set (match_operand:SF 0 "pic30_register_operand"  "=r")
        (match_operand:SF 1 "pic30_code_operand" "g"))]
  "(TARGET_CLASSIC)"
  "*
{
        error(\"invalid address space for operand\");
        return \"cannot generate instructions\";
}

")
(define_insn "*movsf_invalid_2"
  [(set (match_operand:SF 0 "pic30_code_operand" "=g")
        (match_operand:SF 1 "pic30_register_operand"    "r"))]
  ""
  "*
{
        error(\"invalid address space for operand\");
        return \"cannot generate instructions\";
}
")


(define_expand "movsf"
  [(set (match_operand:SF 0 "pic30_general_operand" "")
        (match_operand:SF 1 "pic30_general_operand" ""))]
  ""
  "
   {
     PIC30_TARGET_EXPAND(movsf,(operands[0],operands[1]));
   }"
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Double float (64 bit) moves
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;; Invalid move

(define_insn "*movdf_invalid_1"
  [(set (match_operand:DF 0 "pic30_register_operand"  "=r")
        (match_operand:DF 1 "pic30_code_operand" "g"))]
  "(TARGET_CLASSIC)"
  "*
   {
        error(\"invalid address space for operand\");
        return \"cannot generate instructions\";
   }"
)

(define_insn "*movdf_invalid_2"
  [(set (match_operand:DF 0 "pic30_code_operand" "=g")
        (match_operand:DF 1 "pic30_register_operand"    "r"))]
  ""
  "*
   {
        error(\"invalid address space for operand\");
        return \"cannot generate instructions\";
   }"
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; add instructions 
;;
;; During the instruction canonicalization phase,
;; (minus x (const_int n)) is converted to (plus x (const_int -n)).
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define_expand "addqi3"
  [(set (match_operand:QI 0 "pic30_register_operand" "")
        (plus:QI
          (match_operand:QI 1 "pic30_register_operand" "")
          (match_operand:QI 2 "pic30_mode1JN_APSV_operand" "")))]
  ""
  "{
     PIC30_TARGET_EXPAND(addqi3,(operands[0],operands[1],operands[2]));
   }
  "
)

(define_expand "addhi3"
  [(set (match_operand:HI 0 "pic30_mode2_operand" "")
        (plus:HI
          (match_operand:HI 1 "pic30_JMmath_operand" "")
          (match_operand:HI 2 "pic30_JMmath_operand" "")))]
  ""
  "{
     PIC30_TARGET_EXPAND(addhi3,(operands[0],operands[1],operands[2]));
   }
  "
)

(define_expand "add<mode>3"
  [(set (match_operand:M32BIT_CLASSIC 0 "pic30_register_operand" "")
        (plus:M32BIT_CLASSIC
          (match_operand:M32BIT_CLASSIC 1 "pic30_register_operand" "")
          (match_operand:M32BIT_CLASSIC 2 "pic30_reg_or_PN_operand" "")))]
  "(TARGET_CLASSIC)"
  "{
     PIC30_CLASSIC_EXPAND(add<mode>3,(operands[0],operands[1],operands[2]));
   }
  "
)

(define_expand "addp32ext3"
  [(set (match_operand:P32EXT 0 "pic30_register_operand" "")
        (plus:P32EXT
          (match_operand:P32EXT 1 "pic30_register_operand" "")
          (match_operand:P32EXT 2 "pic30_reg_or_PN_operand" "")))]
  ""
  "{
     PIC30_TARGET_EXPAND(addp32ext3,(operands[0],operands[1],operands[2]));
   }
  "
)

(define_expand "addsi3"
  [(set (match_operand:SI 0 "pic30_mode2_operand" "")
        (plus:SI
          (match_operand:SI 1 "pic30_mode2_operand" "")
          (match_operand:SI 2 "pic30_reg_or_PN_operand" "")))]
  ""
  "{ rtx op0, op1, op2;
     enum machine_mode mode = GET_MODE(operands[0]);

     op0 = operands[0];
     op1 = operands[1];
     op2 = operands[2];
     if (!reload_completed) {
       if (!pic30_mode2_operand(op0,mode)) {
         op0 = force_reg(mode, op0);
       }
       if (!pic30_mode2_operand(op1,mode)) {
         if (operands[1] == operands[0]) {
           op1 = op0;
         } else op1 = force_reg(mode, op1);
       }
       if (!pic30_mode2_operand(op2,mode)) {
         if (operands[2] == operands[0]) {
           op1 = op0;
         } else if (operands[2] == operands[1]) {
           op2 = op1;
         } else op2 = force_reg(mode, op2);
       }
     }
     PIC30_TARGET_EXPAND(addsi3,(op0,op1,op2));
   }
  "
)

(define_expand "adddi3"
  [(set (match_operand:DI 0 "pic30_register_operand" "")
        (plus:DI
          (match_operand:DI 1 "pic30_register_operand" "")
          (match_operand:DI 2 "pic30_register_operand" "")))]
  ""
  "{
     PIC30_TARGET_EXPAND(adddi3,(operands[0],operands[1],operands[2]));
   }
  "
)

;;;;;;;;
;; float
;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; subtract instructions
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define_expand "sub<mode>3"
  [(set (match_operand:MXXBIT_CLASSIC_SUB   0 "pic30_register_operand" "")
        (minus:MXXBIT_CLASSIC_SUB
          (match_operand:MXXBIT_CLASSIC_SUB 1 "pic30_register_operand" "")
          (match_operand:MXXBIT_CLASSIC_SUB 2 "pic30_reg_or_PN_operand" "")))]
  "(TARGET_CLASSIC)"
  "{
     PIC30_CLASSIC_EXPAND(sub<mode>3,(operands[0],operands[1],operands[2]));
   }
  "
)

(define_expand "sub<mode>3"
  [(set (match_operand:NORMAL_INT_MODES 0 "pic30_register_operand" "")
        (minus:NORMAL_INT_MODES
          (match_operand:NORMAL_INT_MODES 1 "pic30_register_operand" "")
          (match_operand:NORMAL_INT_MODES 2 "pic30_JMmath_operand" "")))]
  ""
  "
    PIC30_TARGET_EXPAND(sub<mode>3,(operands[0],operands[1],operands[2]));
  "
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; multiply instructions
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define_expand "mulhi3"
  [(set (match_operand:HI 0 "pic30_register_operand" "")
        (mult:HI 
          (match_operand:HI 1 "pic30_register_operand" "")
          (match_operand:HI 2 "pic30_register_operand" "")))
  ]
  ""
  "
    PIC30_TARGET_EXPAND(mulhi3,(operands[0],operands[1],operands[2]));
  "
)

(define_expand "mulp16apsv3"
  [(set (match_operand:P16APSV 0 "pic30_register_operand" "")
        (mult:P16APSV
          (match_operand:P16APSV 1 "pic30_register_operand" "")
          (match_operand:P16APSV 2 "pic30_register_operand" "")))
  ]
  "(TARGET_CLASSIC)"
  "
    PIC30_CLASSIC_EXPAND(mulp16apsv3,(operands[0],operands[1],operands[2]));
  "
)

(define_expand "mulhisi3"
  [(set (match_operand:SI 0 "pic30_register_operand" "")
        (mult:SI 
          (sign_extend:SI 
             (match_operand:HI 1 "pic30_register_operand" ""))
          (sign_extend:SI 
             (match_operand:HI 2 "pic30_register_operand" ""))))
  ]
  ""
  "
    PIC30_TARGET_EXPAND(mulhisi3,(operands[0],operands[1],operands[2]));
  "
)


(define_expand "umulhip32eds3"
  [(set (match_operand:P32EDS 0 "pic30_register_operand" "=r,r")
        (mult:P32EDS
          (zero_extend:P32EDS
            (match_operand:HI 1 "pic30_register_operand" "%r,r"))
          (zero_extend:P32EDS
            (match_operand:HI 2 "pic30_mode2_APSV_operand"    "r,R<>"))))]
  "(TARGET_CLASSIC)"
  "
{
   PIC30_CLASSIC_EXPAND(umulhip32eds3,(operands[0],operands[1],operands[2]));
}")

(define_expand "umulhip32peds3"
  [(set (match_operand:P32PEDS 0 "pic30_register_operand" "=r,r")
        (mult:P32EDS
          (zero_extend:P32PEDS
            (match_operand:HI 1 "pic30_register_operand" "%r,r"))
          (zero_extend:P32PEDS
            (match_operand:HI 2 "pic30_mode2_APSV_operand"    "r,R<>"))))]
  "(TARGET_CLASSIC)"
  "
{
   PIC30_CLASSIC_EXPAND(umulhip32peds3,(operands[0],operands[1],operands[2]));
}")

(define_expand "umulhisi3"
  [(set (match_operand:SI 0 "pic30_register_operand" "")
        (mult:SI 
          (sign_extend:SI 
             (match_operand:HI 1 "pic30_register_operand" ""))
          (sign_extend:SI 
             (match_operand:HI 2 "pic30_register_operand" ""))))
  ]
  ""
  "
    PIC30_TARGET_EXPAND(umulhisi3,(operands[0],operands[1],operands[2]));
  "
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; divide instructions
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define_expand "divmodhi4"
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
  ""
  "
    PIC30_TARGET_EXPAND(divmodhi4,(operands[0],operands[1],operands[2],operands[3]));
  "
)

(define_expand "udivmodhi4"
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
  ""
  "
    PIC30_TARGET_EXPAND(udivmodhi4,(operands[0],operands[1],operands[2],operands[3]));
  "
)

(define_expand "divmodsi4"
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
    emit(
      gen_divmodsi4_32(operands[0],operands[1],operands[2],operands[3])
    );;
    DONE;
  "
)

(define_expand "udivmodsi4"
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
    emit(
      gen_udivmodsi4_32(operands[0],operands[1],operands[2],operands[3])
    );
    DONE;
  "
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

(define_expand "abs<mode>2"
  [(set (match_operand:RIM   0 "pic30_register_operand" "")
        (abs:RIM
          (match_operand:RIM 1 "pic30_register_operand" "")))
  ]
  ""
  "
    PIC30_TARGET_EXPAND(abs<mode>2,(operands[0],operands[1]));
  "
)

(define_expand "abssf2"
  [(set (match_operand:SF   0 "pic30_register_operand" "")
        (abs:SF
          (match_operand:SF 1 "pic30_register_operand" "")))
  ]
  ""
  "
    PIC30_TARGET_EXPAND(abssf2,(operands[0],operands[1]));
  "
)

(define_expand "absdf2"
  [(set (match_operand:DF   0 "pic30_register_operand" "")
        (abs:DF
          (match_operand:DF 1 "pic30_register_operand" "")))
  ]
  ""
  "
    PIC30_TARGET_EXPAND(absdf2,(operands[0],operands[1]));
  "
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Negation
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define_expand "neg<mode>2"
  [(set (match_operand:NORMAL_MODES   0 "pic30_register_operand" "")
        (neg:NORMAL_MODES
          (match_operand:NORMAL_MODES 1 "pic30_register_operand" "")))
  ]
  ""
  " 
    PIC30_TARGET_EXPAND(neg<mode>2,(operands[0],operands[1]));
  "
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; bit-logical instructions
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;
;; Quarter integer 
;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;
;; Set Bit ;;
;;;;;;;;;;;;;

(define_expand "bitset<mode>"
  [(set (match_operand:RIM   0 "pic30_mode2_operand" "=r,R<>")
        (ior:RIM 
          (match_operand:RIM 1 "pic30_mode2_operand"  "0,0")
          (match_operand     2 "const_int_operand"    "i,i")))]
  "(pic30_one_bit_set_p(INTVAL(operands[2])&0x00ff))"
  "
    PIC30_TARGET_EXPAND(bitset<mode>,(operands[0],operands[1],operands[2]));
  "
)

(define_expand "bitset<mode>_sfr"
  [(set (match_operand:RIM 0 "pic30_near_operand" "")
        (ior:RIM  
          (match_dup 0)
          (match_operand   1 "const_int_operand"   "")))]
  "pic30_one_bit_set_p(INTVAL(operands[1]) & 0x00ff)"
  "
    PIC30_TARGET_EXPAND(bitset<mode>_sfr,(operands[0],operands[1]));
  "
)

;;;;;;;;;;;;;;;;;;;
;; Code Coverage ;;
;;;;;;;;;;;;;;;;;;;
; good for all devices
(define_insn "set_cover_point"
 [(unspec_volatile [(match_operand 0 "immediate_operand" "i")] UNSPECV_COVER)]
  ""
  "*
   return pic30_cover_insn(INTVAL(operands[0]));
  "
)

;;;;;;;;;;;;;;;
;; Reset Bit ;;
;;;;;;;;;;;;;;;

(define_expand "bitclr<mode>"
  [(set (match_operand:RIM   0 "pic30_mode2_operand" "=r,R,<>")
        (and:RIM 
          (match_operand:RIM 1 "pic30_mode2_operand"  "0,0,0")
          (match_operand     2 "const_int_operand"    "i,i,i")))]
  "(pic30_one_bit_set_p((~INTVAL(operands[2])&0xff)))"
  "
    PIC30_TARGET_EXPAND(bitclr<mode>,(operands[0],operands[1],operands[2]));
  "
)

(define_expand "bitclr<mode>_sfr"
  [(set (match_operand:RIM 0 "pic30_near_operand" "")
        (and:RIM  
          (match_dup 0)
          (match_operand   1 "const_int_operand"  "")))]
  "(pic30_one_bit_set_p((~INTVAL (operands[1])) & 0x00ff))"
  "
    PIC30_TARGET_EXPAND(bitclr<mode>_sfr,(operands[0],operands[1]));
  "
)

;;;;;;;;;;;;;;;;
;; Toggle Bit ;;
;;;;;;;;;;;;;;;;

(define_expand "bittog<mode>"
  [(set (match_operand:RIM   0 "pic30_mode2_operand" "=r,R,<>")
        (xor:RIM 
          (match_operand:RIM 1 "pic30_mode2_operand"  "0,0,0")
          (match_operand     2 "const_int_operand"    "i,i,i")))]
  "(pic30_one_bit_set_p(INTVAL(operands[2])&0xffff))"
  "
    PIC30_TARGET_EXPAND(bittog<mode>,(operands[0],operands[1],operands[2]));
  "
)

(define_expand "bittog<mode>_sfr"
  [(set (match_operand:RIM 0 "pic30_near_operand" "")
        (xor:RIM  
          (match_dup 0)
          (match_operand   1 "const_int_operand"  "")))]
  "pic30_one_bit_set_p(INTVAL(operands[1]) & 0x00ff)"
  "
    PIC30_TARGET_EXPAND(bittog<mode>_sfr,(operands[0],operands[1]));
  "
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; AND
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define_expand "and<mode>3"
  [(set (match_operand:RESTRICTED_INT_MODES 0 "pic30_register_operand" "")
        (and:RESTRICTED_INT_MODES 
          (match_operand:RESTRICTED_INT_MODES 1 "pic30_reg_or_imm_operand" "")
          (match_operand:RESTRICTED_INT_MODES 2 "pic30_reg_or_imm_operand" "")))]
  ""
  "
    PIC30_TARGET_EXPAND(and<mode>3,(operands[0],operands[1],operands[2]));
  "
)

(define_expand "anddi3"
  [(set (match_operand:DI   0 "pic30_mode2_operand" "")
        (and:DI
          (match_operand:DI 1 "pic30_reg_or_lit"    "")
          (match_operand:DI 2 "pic30_mode2_operand" "")))]
  ""
  "
    PIC30_TARGET_EXPAND(anddi3,(operands[0],operands[1],operands[2]));
  "
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; OR
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define_expand "ior<mode>3"
  [(set (match_operand:RESTRICTED_INT_MODES 0 "pic30_register_operand" "")
        (ior:RESTRICTED_INT_MODES 
          (match_operand:RESTRICTED_INT_MODES 1 "pic30_reg_or_imm_operand" "")
          (match_operand:RESTRICTED_INT_MODES 2 "pic30_reg_or_imm_operand" "")))]
  ""
  "
    PIC30_TARGET_EXPAND(ior<mode>3,(operands[0],operands[1],operands[2]));
  "
)

(define_expand "iordi3"
  [(set (match_operand:DI   0 "pic30_mode2_operand" "")
        (ior:DI
          (match_operand:DI 1 "pic30_register_operand" "")
          (match_operand:DI 2 "pic30_mode2_operand" "")))]
  ""
  "
    PIC30_TARGET_EXPAND(iordi3,(operands[0],operands[1],operands[2]));
  "
)


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; XOR
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define_expand "xor<mode>3"
  [(set (match_operand:RESTRICTED_INT_MODES 0 "pic30_register_operand" "")
        (xor:RESTRICTED_INT_MODES 
          (match_operand:RESTRICTED_INT_MODES 1 "pic30_register_operand" "")
          (match_operand:RESTRICTED_INT_MODES 2 "pic30_reg_or_imm_operand" "")))]
  ""
  "
    PIC30_TARGET_EXPAND(xor<mode>3,(operands[0],operands[1],operands[2]));
  "
)

(define_expand "xordi3"
  [(set (match_operand:DI   0 "pic30_mode2_operand" "")
        (xor:DI
          (match_operand:DI 1 "pic30_register_operand" "")
          (match_operand:DI 2 "pic30_mode2_operand" "")))]
  ""
  "
    PIC30_TARGET_EXPAND(xordi3,(operands[0],operands[1],operands[2]));
  "
)


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; NOT
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define_expand "one_cmpl<mode>2"
  [(set (match_operand:RESTRICTED_INT_MODES 0 "pic30_register_operand"   "")
        (not:RESTRICTED_INT_MODES 
          (match_operand:RESTRICTED_INT_MODES 1 "pic30_register_operand" "")))
  ]
  ""
  "
    PIC30_TARGET_EXPAND(one_cmpl<mode>2,(operands[0],operands[1]));
  "
)

(define_expand "one_cmpdi2"
  [(set (match_operand:DI   0 "pic30_register_operand"   "")
        (not:DI
          (match_operand:DI 1 "pic30_register_operand" "")))
  ]
  ""
  "
    PIC30_TARGET_EXPAND(one_cmpldi2,(operands[0],operands[1]));
  "
)


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Shift instructions
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Arithmetic Shift Left instructions
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define_expand "ashlqi3"
  [(set (match_operand:QI 0 "pic30_register_operand"    "")
        (ashift:QI
          (match_operand:QI 1 "pic30_register_operand" "")
          (match_operand:QI 2 "pic30_reg_or_imm_operand" "")))]
  ""
  "
    PIC30_TARGET_EXPAND(ashlqi3,(operands[0],operands[1],operands[2]));
  "
)

(define_expand "ashlhi3"
  [(set (match_operand:HI 0 "pic30_register_operand"    "")
        (ashift:HI
          (match_operand:HI 1 "pic30_register_operand" "")
          (match_operand:HI 2 "pic30_reg_or_imm_operand" "")))]
  ""
  "
    PIC30_TARGET_EXPAND(ashlhi3,(operands[0],operands[1],operands[2]));
  "
)

(define_expand "ashlsi3"
  [(set (match_operand:SI 0 "pic30_register_operand"    "")
        (ashift:SI
          (match_operand:SI 1 "pic30_register_operand" "")
          (match_operand:HI 2 "pic30_reg_or_imm_operand" "")))]
  ""
  "
    PIC30_TARGET_EXPAND(ashlsi3,(operands[0],operands[1],operands[2]));
  "
)

(define_expand "ashldi3"
  [(set (match_operand:DI 0 "pic30_register_operand"    "")
        (ashift:DI
          (match_operand:DI 1 "pic30_register_operand" "")
          (match_operand:HI 2 "pic30_reg_or_imm_operand" "")))]
  ""
  "
    PIC30_TARGET_EXPAND(ashldi3,(operands[0],operands[1],operands[2]));
  "
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Logical Shift Right instructions
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define_expand "lshrhi3"
  [(set (match_operand:HI   0 "pic30_register_operand"   "")
        (lshiftrt:HI
          (match_operand:HI 1 "pic30_register_operand"   "")
          (match_operand:HI 2 "pic30_reg_or_imm_operand" "")))
  ]
  ""
  "
    PIC30_TARGET_EXPAND(lshrhi3,(operands[0],operands[1],operands[2]));
  "
)

(define_expand "lshr<mode>3"
  [(set (match_operand:M32BIT_SHIFT_CLASSIC   0 "pic30_register_operand"    "")
        (lshiftrt:M32BIT_SHIFT_CLASSIC
          (match_operand:M32BIT_SHIFT_CLASSIC 1 "pic30_register_operand" "")
          (match_operand:HI                   2 "pic30_reg_or_imm_operand" "")))]
  "(TARGET_CLASSIC)"
  "
    PIC30_CLASSIC_EXPAND(lshr<mode>3,(operands[0],operands[1],operands[2]));
  "
)

(define_expand "lshrsi3"
  [(set (match_operand:SI 0 "pic30_register_operand"    "")
        (lshiftrt:SI
          (match_operand:SI 1 "pic30_register_operand" "")
          (match_operand:HI 2 "pic30_reg_or_imm_operand" "")))]
  ""
  "
    PIC30_TARGET_EXPAND(lshrsi3,(operands[0],operands[1],operands[2]));
  "
)

(define_expand "lshrdi3"
  [(set (match_operand:DI 0 "pic30_register_operand"    "")
        (lshiftrt:DI
          (match_operand:DI 1 "pic30_register_operand" "")
          (match_operand:HI 2 "pic30_reg_or_imm_operand" "")))]
  ""
  "
    PIC30_TARGET_EXPAND(lshrdi3,(operands[0],operands[1],operands[2]));
  "
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Arithmetic shift right.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define_expand "ashr<mode>3"
  [(set (match_operand:NORMAL_INT_MODES   0 "pic30_register_operand"    "")
        (ashiftrt:NORMAL_INT_MODES
          (match_operand:NORMAL_INT_MODES 1 "pic30_register_operand" "")
          (match_operand:HI               2 "pic30_reg_or_imm_operand" "")))
  ]
  ""
  "
    PIC30_TARGET_EXPAND(ashr<mode>3,(operands[0],operands[1],operands[2]));
  " 
) 

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; rotate instructions
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define_expand "rotlqi3"
  [(set (match_operand:QI 0 "pic30_register_operand"    "")
        (rotate:QI
          (match_operand:QI 1 "pic30_register_operand" "")
          (match_operand:QI 2 "immediate_operand" "")))]
  ""
  "
    PIC30_TARGET_EXPAND(rotlqi3,(operands[0],operands[1],operands[2]));
  " 
) 

(define_expand "rotlhi3"
  [(set (match_operand:HI 0 "pic30_register_operand"    "")
        (rotate:HI
          (match_operand:HI 1 "pic30_register_operand" "")
          (match_operand:HI 2 "immediate_operand" "")))]
  ""
  "
    PIC30_TARGET_EXPAND(rotlhi3,(operands[0],operands[1],operands[2]));
  " 
) 

(define_expand "rotrqi3"
  [(set (match_operand:QI 0 "pic30_register_operand"    "")
        (rotatert:QI
          (match_operand:QI 1 "pic30_register_operand" "")
          (match_operand:QI 2 "immediate_operand" "")))]
  ""
  "
    PIC30_TARGET_EXPAND(rotrqi3,(operands[0],operands[1],operands[2]));
  " 
) 

(define_expand "rotrhi3"
  [(set (match_operand:HI 0 "pic30_register_operand"    "")
        (rotatert:HI
          (match_operand:HI 1 "pic30_register_operand" "")
          (match_operand:HI 2 "immediate_operand" "")))]
  ""
  "
    PIC30_TARGET_EXPAND(rotrhi3,(operands[0],operands[1],operands[2]));
  " 
) 

(define_expand "rotrsi3"
  [(set (match_operand:SI 0 "pic30_register_operand"    "")
        (rotatert:SI
          (match_operand:SI 1 "pic30_register_operand" "")
          (match_operand:SI 2 "immediate_operand" "")))]
  "(pic30_isa32_target())"
  "
    rtx x = gen_rotrsi3_32(operands[0],operands[1],operands[2]);
    if (x) emit(x); else FAIL;
  " 
) 

;; (define_insn "extv" ...

;; (define_insn "extzv" ...

;; operand 0 may have byte_mode or word_mode
;; operand 1 the value to extract (register or memory) (byte_mode or word_mode)
;; operand 2 specifies the width in bits
;; operand 3 specifies the starting bit

(define_expand "extzv"
  [(set (match_operand 0 "pic30_register_operand" "")
        (zero_extract
          (match_operand    1 "pic30_register_operand"   "")
          (match_operand    2 "immediate_operand" "")
          (match_operand    3 "immediate_operand" "")))]
  ""
  "
    if (INTVAL(operands[2]) == 1) {
      FAIL;
    } else {
      PIC30_TARGET_EXPAND(extzv,(operands[0],operands[1],operands[2],operands[3]));
    }
  "
)

;; (define_insn "insv" ...

;; operand 0 may have byte_mode or word_mode
;; operand 1 specifies the width in bits
;; operand 2 specifies the starting bit
;; operand 3 specifies the operand to store 

(define_expand "insv"
  [(set (zero_extract
          (match_operand    0 "pic30_mode2_or_near_operand" "+RrU")
          (match_operand    1 "immediate_operand"           "i")
          (match_operand    2 "immediate_operand"           "i"))
        (match_operand       3 "pic30_reg_or_imm_operand"   "i"))]
  ""
  "
#if 0
    if (pic30_isa32_target() && immediate_operand(operands[3],VOIDmode)) {
      FAIL;
    } else 
#endif
    if (pic30_register_operand(operands[3],VOIDmode)) {
      FAIL;
    } else {
      PIC30_TARGET_EXPAND(insv,(operands[0],operands[1],operands[2],operands[3]));
    }
  "
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
;
(define_expand "beq"
  [(set (pc)
        (if_then_else (eq (reg:CC CC_REG)
                          (const_int 0))
                      (label_ref (match_operand 0 "" ""))
                      (pc)))]
  ""
  "PIC30_TARGET_EXPAND(beq,(operands[0]));"
)

(define_expand "bne"
  [(set (pc)
        (if_then_else (ne (reg:CC CC_REG)
                          (const_int 0))
                      (label_ref (match_operand 0 "" ""))
                      (pc)))]
  ""
  "PIC30_TARGET_EXPAND(bne,(operands[0]));"
)

(define_expand "bgt"
  [(set (pc)
        (if_then_else (gt (reg:CC CC_REG)
                          (const_int 0))
                      (label_ref (match_operand 0 "" ""))
                      (pc)))]
  ""
  "PIC30_TARGET_EXPAND(bgt,(operands[0]));"
)

(define_expand "blt"
  [(set (pc)
        (if_then_else (lt (reg:CC CC_REG)
                          (const_int 0))
                      (label_ref (match_operand 0 "" ""))
                      (pc)))]
  ""
  "PIC30_TARGET_EXPAND(blt,(operands[0]));"
)

(define_expand "bge"
  [(set (pc)
        (if_then_else (ge (reg:CC CC_REG)
                          (const_int 0))
                      (label_ref (match_operand 0 "" ""))
                      (pc)))]
  ""
  "PIC30_TARGET_EXPAND(bge,(operands[0]));"
)

(define_expand "ble"
  [(set (pc)
        (if_then_else (le (reg:CC CC_REG)
                          (const_int 0))
                      (label_ref (match_operand 0 "" ""))
                      (pc)))]
  ""
  "PIC30_TARGET_EXPAND(ble,(operands[0]));"
)


; unsigned branches:

(define_expand "bgtu"
  [(set (pc)
        (if_then_else (gtu (reg:CC CC_REG)
                          (const_int 0))
                      (label_ref (match_operand 0 "" ""))
                      (pc)))]
  ""
  "PIC30_TARGET_EXPAND(bgtu,(operands[0]));"
)

(define_expand "bltu"
  [(set (pc)
        (if_then_else (ltu (reg:CC CC_REG)
                          (const_int 0))
                      (label_ref (match_operand 0 "" ""))
                      (pc)))]
  ""
  "PIC30_TARGET_EXPAND(bltu,(operands[0]));"
)

(define_expand "bgeu"
  [(set (pc)
        (if_then_else (geu (reg:CC CC_REG)
                          (const_int 0))
                      (label_ref (match_operand 0 "" ""))
                      (pc)))]
  ""
  "PIC30_TARGET_EXPAND(bgeu,(operands[0]));"
)

(define_expand "bleu"
  [(set (pc)
        (if_then_else (leu (reg:CC CC_REG)
                          (const_int 0))
                      (label_ref (match_operand 0 "" ""))
                      (pc)))]
  ""
  "PIC30_TARGET_EXPAND(bleu,(operands[0]));"
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; An instruction to jump to an address which is operand zero.
;; This pattern name is mandatory on all machines.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define_expand "indirect_jump"
 [(set (pc)
       (match_operand 0 "pic30_register_operand" "r"))
 ]
 ""
 "
   PIC30_TARGET_EXPAND(indirect_jump,(operands[0]));
 "
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
(define_expand "tablejump"
  [(set (pc) (match_operand:HI 0 "pic30_register_operand" "r"))
   (use (label_ref (match_operand 1 "" "")))]
  ""
  "
    PIC30_TARGET_EXPAND(tablejump,(operands[0],operands[1]));
  "
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Subroutines of "casesi".
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;; Operand 0 is index
;; operand 1 is the minimum bound
;; operand 2 is the maximum bound - minimum bound + 1
;; operand 3 is CODE_LABEL for the table;
;; operand 4 is the CODE_LABEL to go to if index out of range.

(define_expand "casesi"
  [(match_operand:SI 0 "pic30_general_operand" "")
   (match_operand:SI 1 "immediate_operand" "")
   (match_operand:SI 2 "immediate_operand" "")
   (match_operand 3 "" "")
   (match_operand 4 "" "")]
  ""
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

(define_expand "casesi0"
  [(set (pc) (plus:SI
     (mem:SI (plus:SI (pc)
               (match_operand:SI 0 "pic30_mode2_operand" "r,R")))
     (label_ref (match_operand 1 "" ""))))
  ]
  ""
  "
    PIC30_TARGET_EXPAND(casesi0,(operands[0],operands[1]));
  "
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Unconditional jump
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define_insn "jump"
  [(set (pc)
        (label_ref (match_operand 0 "" "")))]
  ""
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
(define_expand "call_value"
  [(set (match_operand 0 "pic30_register_operand"        "=r,r ,r")
        (call (match_operand    1 "memory_operand"  "R,QS,m")
              (match_operand:HI 2 "pic30_general_operand" "")))]
  ;; Operand 2 not really used for dsPIC30.
  ""
  "
    PIC30_TARGET_EXPAND(call_value,(operands[0],operands[1],operands[2]));
  "
)

;; Call subroutine with no return value.
(define_expand "call_void"
  [ (call (match_operand:QI 0 "memory_operand"  "R,QS,m")
          (match_operand:HI 1 "pic30_general_operand" ""))]
  ;; Operand 1 not really used for dsPIC30.
  ""
  "
    PIC30_TARGET_EXPAND(call_void,(operands[0],operands[1]));
  "
)

;; Call subroutine with no return value.
;; This handles intrinsics, such as bcopy.

(define_expand "call"
 [(call (match_operand:QI 0 "" "")
        (match_operand:HI 1 "" ""))]
 ""
 "
    PIC30_TARGET_EXPAND(call,(operands[0],operands[1]));
 "
)

;;
;; return
;;
(define_insn "return"
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

(define_insn "return_from_epilogue"
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
;; link - defined in target md file
;;

;;
;; unlink - defined in target md file
;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; prologue/epilogue
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define_expand "prologue"
  [(const_int 1)]
  ""                           
  "
  {
     // PIC30_TARGET_FN(pic30_expand_prologue,());
     pic30_expand_prologue();
     DONE;
  }"
)

(define_expand "epilogue"
  [(const_int 1)]
  ""
  "
  {
     // PIC30_TARGET_FN(pic30_expand_epilogue,());
     pic30_expand_epilogue();
     DONE;
  }"
)

(define_insn "profile_enter"
  [(unspec_volatile [(const_int 0)] UNSPECV_ENTER)]
  ""
  "call __function_level_profiling_long"
)

(define_insn "profile_exit"
  [(unspec_volatile [(const_int 0)] UNSPECV_EXIT)]
  ""
  "goto __function_level_profiling_long_zero"
)

(define_insn "profile_exit_call"
  [(unspec_volatile [(const_int 1)] UNSPECV_EXIT)]
  ""
  "call __function_level_profiling_long_zero"
)

;; UNSPEC_VOLATILE is considered to use and clobber all hard registers and
;; all of memory.  This blocks insns from being moved across this point.

(define_insn "blockage"
  [(unspec_volatile [(const_int 0)] UNSPECV_BLOCKAGE)]
  ""
  "")

(define_insn "pa"
  [(unspec_volatile [(match_operand 0 "immediate_operand" "i")] UNSPECV_PA)]
  ""
  ".set ___PA___,%0")

(define_insn "iv"
  [(unspec_volatile [(match_operand 0 "immediate_operand" "i")] UNSPECV_IV)]
  ""
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
(define_insn "pp"
  [(unspec_volatile [(const_int 0)] UNSPECV_PP)]
  ""
  {
    return pic30_interrupt_preprologue();
  }
)

(define_insn "nop"
  [(const_int 0)]
  ""
  "nop"
  [
    (set_attr "cc" "unchanged")
    (set_attr "op_type" "etc")
  ]
)

;
;; some builtins (in target.md file)

;
;; misc
;

(define_expand "section_begin"
  [(set (match_operand:SI 0 "pic30_register_operand" "=r")
        (unspec:SI [ 
          (match_operand 1 "immediate_operand" "i")
        ] UNSPEC_SECTION_BEGIN))]
  ""
  "
    PIC30_TARGET_EXPAND(section_begin,(operands[0],operands[1]));
  "
)

(define_expand "section_end"
  [(set (match_operand:SI 0 "pic30_register_operand" "=r")
        (unspec:SI [ 
          (match_operand 1 "immediate_operand" "i")
        ] UNSPEC_SECTION_END))]
  ""
  "
    PIC30_TARGET_EXPAND(section_end,(operands[0],operands[1]));
  "
)

(define_expand "section_size"
  [(set (match_operand:SI 0 "pic30_register_operand" "=r")
        (unspec:SI [ 
          (match_operand 1 "immediate_operand" "i")
        ] UNSPEC_SECTION_SIZE))]
  ""
  "
    PIC30_TARGET_EXPAND(section_size,(operands[0],operands[1]));
  "
)

(define_expand "get_isr_state"
 [(set (match_operand:HI 0 "pic30_register_operand" "=r")
       (unspec:HI [(const_int 0)] UNSPEC_GET_ISR_STATE))]
 ""
 "
   PIC30_TARGET_EXPAND(get_isr_state,(operands[0]));
 "
)

(define_expand "set_isr_state"
 [(unspec_volatile [ 
     (match_operand:HI 0 "pic30_register_operand" "=r")] UNSPECV_SET_ISR_STATE)]
 ""
 "
   PIC30_TARGET_EXPAND(set_isr_state,(operands[0]));
 "
)

(define_expand "disable_isr_gie"
  [(unspec_volatile [(const_int 0)] UNSPECV_DISABLE_ISR_GIE)]
  ""
  "
    PIC30_TARGET_EXPAND(disable_isr_gie,());
  "
)

(define_expand "enable_isr_gie"
  [(unspec_volatile [(const_int 0)] UNSPECV_ENABLE_ISR_GIE)]
  ""
  "
    PIC30_TARGET_EXPAND(enable_isr_gie,());
  "
)  

(define_expand "disable_isr_ipl"
  [(unspec_volatile [(const_int 0)] UNSPECV_DISABLE_ISR)]
  ""
  "
    PIC30_TARGET_EXPAND(disable_isr_ipl,());
  "
)

(define_expand "enable_isr_ipl"
  [(unspec_volatile [(const_int 0)] UNSPECV_ENABLE_ISR)]
  ""
  "
    PIC30_TARGET_EXPAND(enable_isr_ipl,());
  "
)

(define_expand "disable_isr"
  [(const_int 0)]
  ""
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

(define_expand "enable_isr"
  [(const_int 0)]
  ""
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

(define_insn "software_break"
  [(unspec_volatile [(const_int 0)] UNSPECV_SOFTWARE_BREAK)]
  ""
  "break"
)
 
;; Fixed Point

;; Use mode iterators alot here, because they are "new" and these are new
;;   patterns (and because most of our Fract/Accum types are the same size

;; *** helper patterns

; for use in the prologue to set the right initial value
(define_expand "setCORCON"
  [(set (reg:HI CORCON)
        (match_operand:HI 0 "pic30_register_operand" "r"))]
  ""
  "
    PIC30_DSP_TARGET_EXPAND(setCORCON,(operands[0]));
  "
)

(define_expand "abs<mode>2"
  [(set (match_operand:Q15   0 "pic30_register_operand" "=r")
        (abs:Q15 
          (match_operand:Q15 1 "pic30_register_operand"  "0")))]
  ""
  "
    PIC30_TARGET_EXPAND(abs<mode>2,(operands[0],operands[1]));
  "
)

(define_expand "bsetCORCON"
  [(set (reg:HI CORCON)
        (ior:HI (reg:HI CORCON)
                (match_operand 0 "const_int_operand"      "i,i,i")))]
  "(pic30_one_bit_set_p(INTVAL(operands[0])&0xffff))"
  "
    PIC30_DSP_TARGET_EXPAND(bsetCORCON, (operands[0]));
  "
)

(define_insn "bclrCORCON"
  [(set (reg:HI CORCON)
        (and:HI (reg:HI CORCON)
                (match_operand 0 "const_int_operand"      "i,i,i")))]
  "(pic30_one_bit_set_p(((~INTVAL(operands[0]))&0xffff)))"
  "bclr.b _CORCON,#%B0"
)

;; *** load/store

(define_expand "mov<mode>_rimm"
   [(set (match_operand:QUQ15 0 "pic30_register_operand" "=r")
         (match_operand:QUQ15 1 "immediate_operand" "i"))]
   ""
   "
     PIC30_TARGET_EXPAND(mov<mode>_rimm,(operands[0],operands[1]));
   "
)

(define_expand "mov<mode>"
  [(set (match_operand:QUQ15 0 "pic30_move_operand" "")
        (match_operand:QUQ15 1 "pic30_move_operand" ""))]
  ""
  "
    PIC30_TARGET_EXPAND(mov<mode>,(operands[0],operands[1]));
  "
)

(define_expand "mov<mode>"
  [(set (match_operand:QUQ31 0 "pic30_move_d_operand" "")
        (match_operand:QUQ31 1 "pic30_move_d_operand" ""))]
  ""
  "
    PIC30_TARGET_EXPAND(mov<mode>,(operands[0],operands[1]));
  "
)

;; *** add/sub
;  16-bit
(define_expand "add<mode>3"
  [(set (match_operand:QUQ15 0 "pic30_register_operand" "=r")
        (plus:QUQ15
           (match_operand:QUQ15 1 "pic30_reg_or_imm_operand" "r")
           (match_operand:QUQ15 2 "pic30_reg_or_imm_operand" "r")))]
  ""
  "
    PIC30_TARGET_EXPAND(add<mode>3,(operands[0],operands[1],operands[2]));
  "
)

(define_expand "sub<mode>3"
  [(set (match_operand:QUQ15 0 "pic30_register_operand" "=r")
        (minus:QUQ15
           (match_operand:QUQ15 1 "pic30_register_operand" "r")
           (match_operand:QUQ15 2 "pic30_register_operand" "r")))]
  ""
  "
    PIC30_TARGET_EXPAND(sub<mode>3,(operands[0],operands[1],operands[2]));
  "
)

;  signed saturating
(define_expand "ssadd<mode>3"
  [(set (match_operand:QUQ15 0 "pic30_register_operand" "=&r")
        (ss_plus:QUQ15
           (match_operand:QUQ15 1 "pic30_register_operand" "r")
           (match_operand:QUQ15 2 "pic30_register_operand" "r")))]
  "pic30_fp_inline_p()"
  "
    PIC30_TARGET_EXPAND(ssadd<mode>3,(operands[0],operands[1],operands[2]));
  "
)

(define_expand "sssub<mode>3"
  [(set (match_operand:QUQ15 0 "pic30_register_operand" "=&r")
        (ss_minus:QUQ15
           (match_operand:QUQ15 1 "pic30_register_operand" "r")
           (match_operand:QUQ15 2 "pic30_register_operand" "r")))]
  "pic30_fp_inline_p()"
  "
    PIC30_TARGET_EXPAND(sssub<mode>3,(operands[0],operands[1],operands[2]));
  "
)

;  unsigned saturating - our unsigned mode really is the same width as signed
(define_expand "usadd<mode>3"
  [(set (match_operand:QUQ15 0 "pic30_register_operand" "=&r")
        (us_plus:QUQ15
           (match_operand:QUQ15 1 "pic30_register_operand" "r")
           (match_operand:QUQ15 2 "pic30_register_operand" "r")))]
  ""
  "
    PIC30_TARGET_EXPAND(usadd<mode>3,(operands[0],operands[1],operands[2]));
  "
)

(define_expand "ussub<mode>3"
  [(set (match_operand:QUQ15 0 "pic30_register_operand" "=&r")
        (us_minus:QUQ15
           (match_operand:QUQ15 1 "pic30_register_operand" "r")
           (match_operand:QUQ15 2 "pic30_register_operand" "r")))]
  ""
  "
    PIC30_TARGET_EXPAND(ussub<mode>3,(operands[0],operands[1],operands[2]));
  "
)

;  32-bit
(define_expand "add<mode>3"
  [(set (match_operand:QUQ31 0 "pic30_register_operand" "=r")
        (plus:QUQ31
           (match_operand:QUQ31 1 "pic30_register_operand" "r")
           (match_operand:QUQ31 2 "pic30_register_operand" "r")))]
  ""
  "
    PIC30_TARGET_EXPAND(add<mode>3,(operands[0],operands[1],operands[2]));
  "
)

(define_expand "sub<mode>3"
  [(set (match_operand:QUQ31 0 "pic30_register_operand" "=r")
        (minus:QUQ31
           (match_operand:QUQ31 1 "pic30_register_operand" "r")
           (match_operand:QUQ31 2 "pic30_register_operand" "r")))]
  ""
  "
    PIC30_TARGET_EXPAND(sub<mode>3,(operands[0],operands[1],operands[2]));
  "
)

;  signed saturating
(define_expand "ssadd<mode>3"
  [(set (match_operand:QUQ31 0 "pic30_register_operand" "=&r")
        (ss_plus:QUQ31
           (match_operand:QUQ31 1 "pic30_register_operand" "r")
           (match_operand:QUQ31 2 "pic30_register_operand" "r")))]
  "pic30_fp_inline_p()"
  "
    PIC30_TARGET_EXPAND(ssadd<mode>3,(operands[0],operands[1],operands[2]));
  "
)

(define_expand "sssub<mode>3"
  [(set (match_operand:QUQ31 0 "pic30_register_operand" "=&r")
        (ss_minus:QUQ31
           (match_operand:QUQ31 1 "pic30_register_operand" "r")
           (match_operand:QUQ31 2 "pic30_register_operand" "r")))]
  "pic30_fp_inline_p()"
  "
    PIC30_TARGET_EXPAND(sssub<mode>3,(operands[0],operands[1],operands[2]));
  "
)

;  unsigned saturating - our unsigned mode really is the same width as signed
(define_expand "usadd<mode>3"
  [(set (match_operand:QUQ31 0 "pic30_register_operand" "=&r")
        (us_plus:QUQ31
           (match_operand:QUQ31 1 "pic30_register_operand" "r")
           (match_operand:QUQ31 2 "pic30_register_operand" "r")))]
  "pic30_fp_inline_p()"
  "
    PIC30_TARGET_EXPAND(usadd<mode>3,(operands[0],operands[1],operands[2]));
  "
)

(define_expand "ussub<mode>3"
  [(set (match_operand:QUQ31 0 "pic30_register_operand" "=&r")
        (us_minus:QUQ31
           (match_operand:QUQ31 1 "pic30_register_operand" "r")
           (match_operand:QUQ31 2 "pic30_register_operand" "r")))]
  "pic30_fp_inline_p()"
  "
    PIC30_TARGET_EXPAND(ussub<mode>3,(operands[0],operands[1],operands[2]));
  "
)

;; *** multiply
;  16-bit


(define_expand "mul<mode>3"
  [(set (match_operand:QUQ15 0 "pic30_move_operand" "=rRQST<>")
        (mult:QUQ15 
          (match_operand:QUQ15 1 "pic30_register_operand" "=r")
          (match_operand:QUQ15 2 "pic30_register_operand" "=r")))]
  ""
  "
    PIC30_TARGET_EXPAND(mul<mode>3,(operands[0],operands[1],operands[2]));
  "
)

(define_expand "ssmul<mode>3"
  [(set (match_operand:QUQ15 0 "pic30_move_operand" "=rRQST<>")
        (ss_mult:QUQ15 
          (match_operand:QUQ15 1 "pic30_register_operand" "=r")
          (match_operand:QUQ15 2 "pic30_register_operand" "=r")))]
  ""
  "
    PIC30_TARGET_EXPAND(ssmul<mode>3,(operands[0],operands[1],operands[2]));
  "
)

(define_expand "usmul<mode>3"
  [(set (match_operand:QUQ15 0 "pic30_move_operand" "=rRQST<>")
        (us_mult:QUQ15
          (match_operand:QUQ15 1 "pic30_register_operand" "=r")
          (match_operand:QUQ15 2 "pic30_register_operand" "=r")))]
  ""
  "
    PIC30_TARGET_EXPAND(usmul<mode>3,(operands[0],operands[1],operands[2]));
  "
)

;; *** multiply
;  32-bit
; handled by helper routines in libfx

;; *** divide
;  16-bit

(define_expand "udiv<mode>3"
  [(set (match_operand:UQ15 0 "pic30_register_operand"    "=a,?b,??e")
        (udiv:UQ15
          (match_operand:UQ15 1 "pic30_register_operand"   "r, r,  r")
          (match_operand:UQ15 2 "pic30_ereg_operand"       "e, e,  e")))]
  "pic30_dsp_target()"
  "
    PIC30_DSP_TARGET_EXPAND(udiv<mode>3,(operands[0],operands[1],operands[2]));
  "
)

(define_expand "div<mode>3"
  [(set (match_operand:Q15 0 "pic30_register_operand"    "=a,?b,??e")
        (div:Q15
          (match_operand:Q15 1 "pic30_register_operand"   "r, r,  r")
          (match_operand:Q15 2 "pic30_ereg_operand"       "e, e,  e")))]
  "pic30_dsp_target()"
  "
    PIC30_DSP_TARGET_EXPAND(div<mode>3,(operands[0],operands[1],operands[2]));
  "
)

(define_expand "sssaturate<mode>"
  [(set (match_operand:QUQ15    0 "pic30_register_operand" "=&r")
        (unspec:QUQ15 [
           (match_operand:QUQ15 1 "pic30_register_operand" "0")
           (match_operand:HI    2 "pic30_register_operand" "r")
         ] UNSPEC_SATURATE)
   ) 
  ]
  ""
  "
    PIC30_TARGET_EXPAND(sssaturate<mode>,(operands[0],operands[1],operands[2]));
  "
)

(define_expand "usdiv<mode>3"
  [(set (match_operand:UQ15 0 "pic30_register_operand"    "=a,?b,??e")
        (us_div:UQ15
          (match_operand:UQ15 1 "pic30_register_operand"   "r, r,  r")
          (match_operand:UQ15 2 "pic30_ereg_operand"       "e, e,  e")))]
  "pic30_dsp_target()"
  "
    PIC30_DSP_TARGET_EXPAND(usdiv<mode>3,(operands[0],operands[1],operands[2]));
  "
)

(define_expand "ssdiv<mode>3"
  [(set (match_operand:Q15    0 "pic30_register_operand"  "=a,?b,??e")
        (ss_div:Q15
          (match_operand:Q15  1 "pic30_register_operand"   "r, r,  r")
          (match_operand:Q15  2 "pic30_ereg_operand"       "e, e,  e")))
   (clobber (match_scratch:HI 3                           "=r, r,  r"))
  ]
  "pic30_dsp_target()"
  "
    PIC30_DSP_TARGET_EXPAND(ssdiv<mode>3,(operands[0],operands[1],operands[2]));
  "
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
;  from Q31

(define_expand "fract<mode>qi2"
  [(set (match_operand:QI    0 "pic30_register_operand" "=r")
        (fract_convert:QI
          (match_operand:QQQQQ 1 "pic30_register_operand"  "r")))]
  ""
  "
    PIC30_TARGET_EXPAND(fract<mode>qi2,(operands[0],operands[1]));
  "
)

(define_expand "fract<mode>hi2"
  [(set (match_operand:HI    0 "pic30_register_operand"  "=r")
        (fract_convert:HI
          (match_operand:QQQQQ 1 "pic30_register_operand"  "r")))]
  ""
  "
    PIC30_TARGET_EXPAND(fract<mode>hi2,(operands[0],operands[1]));
  "
)

(define_expand "fract<mode>si2"
  [(set (match_operand:SI    0 "pic30_register_operand"  "=r")
        (fract_convert:SI
          (match_operand:QQQQQ 1 "pic30_register_operand"  "r")))]
  ""
  "
    PIC30_TARGET_EXPAND(fract<mode>si2,(operands[0],operands[1]));
  "
)

(define_expand "fract<mode>di2"
  [(set (match_operand:DI    0 "pic30_register_operand"  "=r")
        (fract_convert:DI
          (match_operand:QQQQQ 1 "pic30_register_operand"  "r")))]
  ""
  "
    PIC30_TARGET_EXPAND(fract<mode>di2,(operands[0],operands[1]));
  "
)

; from ACC

(define_expand "fract<mode>qi2"
  [(set (match_operand:QI    0 "pic30_register_operand"    "=r")
        (fract_convert:QI
          (match_operand:AUACC 1 "pic30_accum_or_reg_operand" "rw")))]
  ""
  "
    PIC30_TARGET_EXPAND(fract<mode>qi2,(operands[0],operands[1]));
  "
)

(define_expand "fract<mode>hi2"
  [(set (match_operand:HI    0 "pic30_register_operand"    "=r")
        (fract_convert:HI
          (match_operand:AUACC 1 "pic30_accum_or_reg_operand" "rw")))]
  ""
  "
    PIC30_TARGET_EXPAND(fract<mode>hi2,(operands[0],operands[1]));
  "
)

; to Q15
; to Q31

(define_expand "fractqi<mode>2"
  [(set (match_operand:QQQQQ  0 "pic30_register_operand"  "=&r")
        (fract_convert:QQQQQ
          (match_operand:QI 1 "pic30_register_operand"  "r")))]
  "pic30_fp_inline_p()"
  "
    PIC30_TARGET_EXPAND(fractqi<mode>2,(operands[0],operands[1]));
  "
)

(define_expand "fracthi<mode>2"
  [(set (match_operand:QQQQQ  0 "pic30_register_operand"  "=&r")
        (fract_convert:QQQQQ
          (match_operand:HI 1 "pic30_register_operand"  "r")))]
  "pic30_fp_inline_p()"
  "
    PIC30_TARGET_EXPAND(fracthi<mode>2,(operands[0],operands[1]));
  "
)

(define_expand "fractsi<mode>2"
  [(set (match_operand:QQQQQ  0 "pic30_register_operand"  "=&r")
        (fract_convert:QQQQQ
          (match_operand:SI 1 "pic30_register_operand"  "r")))]
  "pic30_fp_inline_p()"
  "
    PIC30_TARGET_EXPAND(fractsi<mode>2,(operands[0],operands[1]));
  "
)

(define_expand "fractdi<mode>2"
  [(set (match_operand:QQQQQ  0 "pic30_register_operand"  "=&r")
        (fract_convert:QQQQQ
          (match_operand:DI 1 "pic30_register_operand"  "r")))]
  "pic30_fp_inline_p()"
  "
    PIC30_TARGET_EXPAND(fractdi<mode>2,(operands[0],operands[1]));
  "
)

; to ACC
; to UACC

(define_expand "fractqi<mode>2"
  [(set (match_operand:AUACC  0 "pic30_register_operand"  "=&r")
        (fract_convert:AUACC
          (match_operand:QI 1 "pic30_register_operand"  "r")))]
  "pic30_fp_inline_p()"
  "
    PIC30_TARGET_EXPAND(fractqi<mode>2,(operands[0],operands[1]));
  "
)

(define_expand "fracthi<mode>2"
  [(set (match_operand:AUACC  0 "pic30_register_operand"  "=&r")
        (fract_convert:AUACC
          (match_operand:HI 1 "pic30_register_operand"  "r")))]
  "pic30_fp_inline_p()"
  "
    PIC30_TARGET_EXPAND(fracthi<mode>2,(operands[0],operands[1]));
  "
)

(define_expand "fractsi<mode>2"
  [(set (match_operand:AUACC  0 "pic30_register_operand"  "=&r")
        (fract_convert:AUACC
          (match_operand:SI 1 "pic30_register_operand"  "r")))]
  "pic30_fp_inline_p()"
  "
    PIC30_TARGET_EXPAND(fractsi<mode>2,(operands[0],operands[1]));
  "
)

(define_expand "fractdi<mode>2"
  [(set (match_operand:AUACC  0 "pic30_register_operand"  "=&r")
        (fract_convert:AUACC
          (match_operand:DI 1 "pic30_register_operand"  "r")))]
  "pic30_fp_inline_p()"
  "
    PIC30_TARGET_EXPAND(fractdi<mode>2,(operands[0],operands[1]));
  "
)

; unsigned integer (IMODE) modes for qi
;  from Q15
;  from Q31

(define_expand "fractuns<mode>qi2"
  [(set (match_operand:QI    0 "pic30_register_operand" "=r")
        (unsigned_fract_convert:QI
          (match_operand:QQQQQ 1 "pic30_register_operand"  "r")))]
  ""
  "
    PIC30_TARGET_EXPAND(fractuns<mode>qi2,(operands[0],operands[1]));
  "
)

; from ACC
; from UACC

(define_expand "fractuns<mode>qi2"
  [(set (match_operand:QI    0 "pic30_register_operand" "=r")
        (unsigned_fract_convert:QI
          (match_operand:AUACC 1 "pic30_register_operand"  "r")))]
  ""
  "
    PIC30_TARGET_EXPAND(fractuns<mode>qi2,(operands[0],operands[1]));
  "
)

; unsigned integer (IMODE) modes for HI
;  from Q15
;  from Q31

(define_expand "fractuns<mode>hi2"
  [(set (match_operand:HI    0 "pic30_register_operand" "=r")
        (unsigned_fract_convert:HI
          (match_operand:QQQQQ 1 "pic30_register_operand"  "r")))]
  ""
  "
    PIC30_TARGET_EXPAND(fractuns<mode>hi2,(operands[0],operands[1]));
  "
)

; from ACC
; from UACC

(define_expand "fractuns<mode>hi2"
  [(set (match_operand:HI    0 "pic30_register_operand" "=r")
        (unsigned_fract_convert:HI
          (match_operand:AUACC 1 "pic30_register_operand"  "r")))]
  ""
  "
    PIC30_TARGET_EXPAND(fractuns<mode>hi2,(operands[0],operands[1]));
  "
)

; To SI and DI done by library calls for AUACC
; unsigned integer (IMODE) modes for SI
;  from Q15
;  from Q31

(define_expand "fractuns<mode>si2"
  [(set (match_operand:SI    0 "pic30_register_operand" "=r")
        (unsigned_fract_convert:SI
          (match_operand:QQQQQ 1 "pic30_register_operand"  "r")))]
  ""
  "
    PIC30_TARGET_EXPAND(fractuns<mode>si2,(operands[0],operands[1]));
  "
)

; unsigned integer (IMODE) modes for DI
;  from Q15
;  from Q31

(define_expand "fractuns<mode>di2"
  [(set (match_operand:DI    0 "pic30_register_operand" "=r")
        (unsigned_fract_convert:DI
          (match_operand:QQQQQ 1 "pic30_register_operand"  "r")))]
  ""
  "
    PIC30_TARGET_EXPAND(fractuns<mode>di2,(operands[0],operands[1]));
  "
)

; From QI to mode
; to Q15
; to Q31

(define_expand "fractunsqi<mode>2"
  [(set (match_operand:QQQQQ    0 "pic30_register_operand" "=r")
        (unsigned_fract_convert:QQQQQ
          (match_operand:QI 1 "pic30_register_operand"  "r")))]
  ""
  "
    PIC30_TARGET_EXPAND(fractunsqi<mode>2,(operands[0],operands[1]));
  "
) 

; to ACC
; to UACC

(define_expand "fractunsqi<mode>2"
  [(set (match_operand:AUACC    0 "pic30_register_operand" "=r")
        (unsigned_fract_convert:AUACC
          (match_operand:QI 1 "pic30_register_operand"  "r")))]
  ""
  "
    PIC30_TARGET_EXPAND(fractunsqi<mode>2,(operands[0],operands[1]));
  "
) 

; From HI to mode
; to Q15
; to Q31

(define_expand "fractunshi<mode>2"
  [(set (match_operand:QQQQQ    0 "pic30_register_operand" "=r")
        (unsigned_fract_convert:QQQQQ
          (match_operand:HI 1 "pic30_register_operand"  "r")))]
  ""
  "
    PIC30_TARGET_EXPAND(fractunshi<mode>2,(operands[0],operands[1]));
  "
) 

; to ACC
; to UACC

(define_expand "fractunshi<mode>2"
  [(set (match_operand:AUACC    0 "pic30_register_operand" "=r")
        (unsigned_fract_convert:AUACC
          (match_operand:HI 1 "pic30_register_operand"  "r")))]
  ""
  "
    PIC30_TARGET_EXPAND(fractunshi<mode>2,(operands[0],operands[1]));
  "
) 

; From SI to mode
; to Q15
; to Q31

(define_expand "fractunssi<mode>2"
  [(set (match_operand:QQQQQ    0 "pic30_register_operand" "=r")
        (unsigned_fract_convert:QQQQQ
          (match_operand:SI 1 "pic30_register_operand"  "r")))]
  ""
  "
    PIC30_TARGET_EXPAND(fractunssi<mode>2,(operands[0],operands[1]));
  "
) 

; to ACC
; to UACC

(define_expand "fractunssi<mode>2"
  [(set (match_operand:AUACC    0 "pic30_register_operand" "=r")
        (unsigned_fract_convert:AUACC
          (match_operand:SI 1 "pic30_register_operand"  "r")))]
  ""
  "
    PIC30_TARGET_EXPAND(fractunssi<mode>2,(operands[0],operands[1]));
  "
) 

; From DI to mode
; to Q15
; to Q31

(define_expand "fractunsdi<mode>2"
  [(set (match_operand:QQQQQ    0 "pic30_register_operand" "=r")
        (unsigned_fract_convert:QQQQQ
          (match_operand:DI 1 "pic30_register_operand"  "r")))]
  ""
  "
    PIC30_TARGET_EXPAND(fractunsdi<mode>2,(operands[0],operands[1]));
  "
) 

; to ACC
; to UACC

(define_expand "fractunsdi<mode>2"
  [(set (match_operand:AUACC    0 "pic30_register_operand" "=r")
        (unsigned_fract_convert:AUACC
          (match_operand:DI 1 "pic30_register_operand"  "r")))]
  ""
  "
    PIC30_TARGET_EXPAND(fractunsdi<mode>2,(operands[0],operands[1]));
  "
) 
;; conversions back and forth to floating point or double will be done by
;;  out of line functions only

;; TODO: TO DO: may need to revisit for 33A (CAW)

; conversions to and from fractional types
;; awkward - we can only move through one iterator, these are all 
;;   copies of each other with a different source mode

(define_expand "fractsq<mode>2"
  [(set (match_operand:QUQ15 0 "pic30_register_operand"  "=r")
        (fract_convert:QUQ15
          (match_operand:SQ  1 "pic30_register_operand"  "r")))]
  ""
  "
    PIC30_TARGET_EXPAND(fractsq<mode>2,(operands[0],operands[1]));
  "
)

(define_expand "fractusq<mode>2"
  [(set (match_operand:QUQ15 0 "pic30_register_operand"  "=r")
        (fract_convert:QUQ15
          (match_operand:USQ  1 "pic30_register_operand"  "r")))]
  ""
  "
    PIC30_TARGET_EXPAND(fractusq<mode>2,(operands[0],operands[1]));
  "
)

(define_expand "fracttq<mode>2"
  [(set (match_operand:QUQ15 0 "pic30_register_operand"  "=r")
        (fract_convert:QUQ15
          (match_operand:TQ  1 "pic30_register_operand"  "r")))]
  ""
  "
    PIC30_TARGET_EXPAND(fracttq<mode>2,(operands[0],operands[1]));
  "
)

(define_expand "fractutq<mode>2"
  [(set (match_operand:QUQ15 0 "pic30_register_operand"  "=r")
        (fract_convert:QUQ15
          (match_operand:UTQ  1 "pic30_register_operand"  "r")))]
  ""
  "
    PIC30_TARGET_EXPAND(fractutq<mode>2,(operands[0],operands[1]));
  "
)

(define_expand "fractdq<mode>2"
  [(set (match_operand:QUQ15 0 "pic30_register_operand"  "=r")
        (fract_convert:QUQ15
          (match_operand:DQ  1 "pic30_register_operand"  "r")))]
  ""
  "
    PIC30_TARGET_EXPAND(fractdq<mode>2,(operands[0],operands[1]));
  "
)

(define_expand "fractudq<mode>2"
  [(set (match_operand:QUQ15 0 "pic30_register_operand"  "=r")
        (fract_convert:QUQ15
          (match_operand:UDQ  1 "pic30_register_operand"  "r")))]
  ""
  "
    PIC30_TARGET_EXPAND(fractudq<mode>2,(operands[0],operands[1]));
  "
)

(define_expand "fractqq<mode>2"
  [(set (match_operand:QUQ31 0 "pic30_register_operand" "=r")
        (fract_convert:QUQ31
          (match_operand:QQ  1 "pic30_register_operand"  "r")))]
  ""
  "
    PIC30_TARGET_EXPAND(fractqq<mode>2,(operands[0],operands[1]));
  "
)

(define_expand "fractuqq<mode>2"
  [(set (match_operand:QUQ31 0 "pic30_register_operand" "=r")
        (fract_convert:QUQ31
          (match_operand:UQQ  1 "pic30_register_operand"  "r")))]
  ""
  "
    PIC30_TARGET_EXPAND(fractuqq<mode>2,(operands[0],operands[1]));
  "
)

(define_expand "fracthq<mode>2"
  [(set (match_operand:QUQ31 0 "pic30_register_operand" "=r")
        (fract_convert:QUQ31
          (match_operand:HQ  1 "pic30_register_operand"  "r")))]
  ""
  "
    PIC30_TARGET_EXPAND(fracthq<mode>2,(operands[0],operands[1]));
  "
)

(define_expand "fractuhq<mode>2"
  [(set (match_operand:QUQ31 0 "pic30_register_operand" "=r")
        (fract_convert:QUQ31
          (match_operand:UHQ  1 "pic30_register_operand"  "r")))]
  ""
  "
    PIC30_TARGET_EXPAND(fractuhq<mode>2,(operands[0],operands[1]));
  "
)

; conversions between signed/unsigned 'self'

(define_insn "fractqquqq2"
  [(set (match_operand:UQQ  0 "register_operand" "=r")
       (unsigned_fract_convert:UQQ
         (match_operand:QQ 1 "register_operand" "0")))]
  ""
  "; nop"
)

(define_insn "fractuqqqq2"
  [(set (match_operand:QQ    0 "register_operand" "=r")
       (fract_convert:QQ
         (match_operand:UQQ 1 "register_operand" "0")))]
  ""
  "; nop"
)

(define_insn "fracthquhq2"
  [(set (match_operand:UHQ  0 "register_operand" "=r")
       (unsigned_fract_convert:UHQ
         (match_operand:HQ 1 "register_operand" "0")))]
  ""
  "; nop"
)

(define_insn "fractuhqhq2"
  [(set (match_operand:HQ    0 "register_operand" "=r")
       (fract_convert:HQ
         (match_operand:UHQ 1 "register_operand" "0")))]
  ""
  "; nop"
)

(define_insn "fractsqusq2"
  [(set (match_operand:USQ  0 "register_operand" "=r")
       (unsigned_fract_convert:USQ
         (match_operand:SQ 1 "register_operand" "0")))]
  ""
  "; nop"
)

(define_insn "fractusqsq2"
  [(set (match_operand:SQ    0 "register_operand" "=r")
       (fract_convert:SQ
         (match_operand:USQ 1 "register_operand" "0")))]
  ""
  "; nop"
)

(define_insn "fractdqudq2"
  [(set (match_operand:UDQ  0 "register_operand" "=r")
       (unsigned_fract_convert:UDQ
         (match_operand:DQ 1 "register_operand" "0")))]
  ""
  "; nop"
)

(define_insn "fractudqdq2"
  [(set (match_operand:DQ    0 "register_operand" "=r")
       (fract_convert:DQ
         (match_operand:UDQ 1 "register_operand" "0")))]
  ""
  "; nop"
)

(define_insn "fracttqutq2"
  [(set (match_operand:UTQ  0 "register_operand" "=r")
       (unsigned_fract_convert:UTQ
         (match_operand:TQ 1 "register_operand" "0")))]
  ""
  "; nop"
)

(define_insn "fractutqtq2"
  [(set (match_operand:TQ    0 "register_operand" "=r")
       (fract_convert:TQ
         (match_operand:UTQ 1 "register_operand" "0")))]
  ""
  "; nop"
)

(define_insn "fracthauha2"
  [(set (match_operand:UHA  0 "register_operand" "=r")
       (unsigned_fract_convert:UHA
         (match_operand:HA 1 "register_operand" "0")))]
  ""
  "; nop"
)

(define_insn "fractuhaha2"
  [(set (match_operand:HA    0 "register_operand" "=r")
       (fract_convert:HA
         (match_operand:UHA 1 "register_operand" "0")))]
  ""
  "; nop"
)

(define_insn "fractsausa2"
  [(set (match_operand:USA  0 "register_operand" "=r")
       (unsigned_fract_convert:USA
         (match_operand:SA 1 "register_operand" "0")))]
  ""
  "; nop"
)

(define_insn "fractusasa2"
  [(set (match_operand:SA    0 "register_operand" "=r")
       (fract_convert:SA
         (match_operand:USA 1 "register_operand" "0")))]
  ""
  "; nop"
)

(define_insn "fractdauda2"
  [(set (match_operand:UDA  0 "register_operand" "=r")
       (unsigned_fract_convert:UDA
         (match_operand:DA 1 "register_operand" "0")))]
  ""
  "; nop"
)

(define_insn "fractudada2"
  [(set (match_operand:DA    0 "register_operand" "=r")
       (fract_convert:DA
         (match_operand:UDA 1 "register_operand" "0")))]
  ""
  "; nop"
)

(define_insn "fracttauta2"
  [(set (match_operand:UTA  0 "register_operand" "=r")
       (unsigned_fract_convert:UTA
         (match_operand:TA 1 "register_operand" "0")))]
  ""
  "; nop"
)

(define_insn "fractutata2"
  [(set (match_operand:TA    0 "register_operand" "=r")
       (fract_convert:TA
         (match_operand:UTA 1 "register_operand" "0")))]
  ""
  "; nop"
)

; saturating conversions
;   not all conversions saturate (but we seem to need them all)
;   many of these are copies of the "non-saturating" versions

(define_expand "satfract<mode>qi2"
  [(set (match_operand:QI    0 "pic30_register_operand"  "=r")
        (sat_fract:QI
          (match_operand:QQQQQ 1 "pic30_register_operand"  "r")))]
  ""
  "
    PIC30_TARGET_EXPAND(satfract<mode>qi2,(operands[0],operands[1]));
  "
)

(define_expand "satfract<mode>hi2"
  [(set (match_operand:HI    0 "pic30_register_operand"  "=r")
        (sat_fract:HI
          (match_operand:QQQQQ 1 "pic30_register_operand"  "r")))]
  ""
  "
    PIC30_TARGET_EXPAND(satfract<mode>hi2,(operands[0],operands[1]));
  "
)

(define_expand "satfract<mode>si2"
  [(set (match_operand:SI    0 "pic30_register_operand"  "=r")
        (sat_fract:SI
          (match_operand:QQQQQ 1 "pic30_register_operand"  "r")))]
  ""
  "
    PIC30_TARGET_EXPAND(satfract<mode>si2,(operands[0],operands[1]));
  "
)

(define_expand "satfract<mode>di2"
  [(set (match_operand:DI    0 "pic30_register_operand"  "=r")
        (sat_fract:DI
          (match_operand:QQQQQ 1 "pic30_register_operand"  "r")))]
  ""
  "
    PIC30_TARGET_EXPAND(satfract<mode>di2,(operands[0],operands[1]));
  "
)

; to Q15
; to Q31

(define_expand "satfractqi<mode>2"
  [(set (match_operand:QQQQQ  0 "pic30_register_operand"  "=r")
        (sat_fract:QQQQQ
          (match_operand:QI 1 "pic30_register_operand"  "r")))]
  "pic30_fp_inline_p()"
  "
    PIC30_TARGET_EXPAND(satfractqi<mode>2,(operands[0],operands[1]));
  "
)

(define_expand "satfracthi<mode>2"
  [(set (match_operand:QQQQQ  0 "pic30_register_operand"  "=r")
        (sat_fract:QQQQQ
          (match_operand:HI 1 "pic30_register_operand"  "r")))]
  "pic30_fp_inline_p()"
  "
    PIC30_TARGET_EXPAND(satfracthi<mode>2,(operands[0],operands[1]));
  "
)

(define_expand "satfractsi<mode>2"
  [(set (match_operand:QQQQQ  0 "pic30_register_operand"  "=r")
        (sat_fract:QQQQQ
          (match_operand:SI 1 "pic30_register_operand"  "r")))]
  "pic30_fp_inline_p()"
  "
    PIC30_TARGET_EXPAND(satfractsi<mode>2,(operands[0],operands[1]));
  "
)

(define_expand "satfractdi<mode>2"
  [(set (match_operand:QQQQQ  0 "pic30_register_operand"  "=r")
        (sat_fract:QQQQQ
          (match_operand:DI 1 "pic30_register_operand"  "r")))]
  "pic30_fp_inline_p()"
  "
    PIC30_TARGET_EXPAND(satfractdi<mode>2,(operands[0],operands[1]));
  "
)

; to ACC


(define_expand "satfractqi<mode>2"
  [(set (match_operand:AUACC 0 "pic30_register_operand"  "=&r")
        (sat_fract:AUACC
          (match_operand:QI  1 "pic30_register_operand"    "r")))]
  ""
  "
   PIC30_TARGET_EXPAND(satfractqi<mode>2,(operands[0],operands[1]));
  "
)

(define_expand "satfractunsqi<mode>2"
  [(set (match_operand:AUACC 0 "pic30_register_operand"  "=&r")
        (unsigned_sat_fract:AUACC
          (match_operand:QI  1 "pic30_register_operand"    "r")))]
  ""
  "
   PIC30_TARGET_EXPAND(satfractunsqi<mode>2,(operands[0],operands[1]));
  "
)

; unsigned

(define_expand "satfractuns<mode>qi2"
  [(set (match_operand:QI 0 "pic30_register_operand"  "=&r")
        (unsigned_sat_fract:QI
          (match_operand:QQQQQ  1 "pic30_register_operand"    "r")))]
  ""
  "
   PIC30_TARGET_EXPAND(satfractunsqi<mode>2,(operands[0],operands[1]));
  "
)

(define_expand "satfractuns<mode>hi2"
  [(set (match_operand:HI 0 "pic30_register_operand"  "=&r")
        (unsigned_sat_fract:HI
          (match_operand:QQQQQ  1 "pic30_register_operand"    "r")))]
  ""
  "
   PIC30_TARGET_EXPAND(satfractunshi<mode>2,(operands[0],operands[1]));
  "
)

(define_expand "satfractuns<mode>si2"
  [(set (match_operand:SI 0 "pic30_register_operand"  "=&r")
        (unsigned_sat_fract:SI
          (match_operand:QQQQQ  1 "pic30_register_operand"    "r")))]
  ""
  "
   PIC30_TARGET_EXPAND(satfractunssi<mode>2,(operands[0],operands[1]));
  "
)

(define_expand "satfractuns<mode>di2"
  [(set (match_operand:DI 0 "pic30_register_operand"  "=&r")
        (unsigned_sat_fract:DI
          (match_operand:QQQQQ  1 "pic30_register_operand"    "r")))]
  ""
  "
   PIC30_TARGET_EXPAND(satfractunsdi<mode>2,(operands[0],operands[1]));
  "
)

; to Q15

(define_expand "satfractunsqi<mode>2"
  [(set (match_operand:QQQQQ  0 "pic30_register_operand"  "=r")
        (unsigned_sat_fract:QQQQQ
          (match_operand:QI 1 "pic30_register_operand"  "r")))]
  "pic30_fp_inline_p()"
  "
   PIC30_TARGET_EXPAND(satfractunsqi<mode>2,(operands[0],operands[1]));
  "
)

(define_expand "satfractunshi<mode>2"
  [(set (match_operand:QQQQQ  0 "pic30_register_operand"  "=r")
        (unsigned_sat_fract:QQQQQ
          (match_operand:HI 1 "pic30_register_operand"  "r")))]
  "pic30_fp_inline_p()"
  "
   PIC30_TARGET_EXPAND(satfractunshi<mode>2,(operands[0],operands[1]));
  "
)

(define_expand "satfractunssi<mode>2"
  [(set (match_operand:QQQQQ  0 "pic30_register_operand"  "=r")
        (unsigned_sat_fract:QQQQQ
          (match_operand:SI 1 "pic30_register_operand"  "r")))]
  "pic30_fp_inline_p()"
  "
   PIC30_TARGET_EXPAND(satfractunssi<mode>2,(operands[0],operands[1]));
  "
)

(define_expand "satfractunsdi<mode>2"
  [(set (match_operand:QQQQQ  0 "pic30_register_operand"  "=r")
        (unsigned_sat_fract:QQQQQ
          (match_operand:DI 1 "pic30_register_operand"  "r")))]
  "pic30_fp_inline_p()"
  "
   PIC30_TARGET_EXPAND(satfractunsdi<mode>2,(operands[0],operands[1]));
  "
)


;; conversions back and forth to floating point or double will be done by
;;  out of line functions only

;; conversions to and from fractional types
;;   some of these can actually cause saturation

(define_expand "satfractsq<mode>2"
  [(set (match_operand:QUQ15 0 "pic30_register_operand"  "=r")
        (sat_fract:QUQ15
          (match_operand:SQ  1 "pic30_register_operand"  "r")))]
  ""
  "
    PIC30_TARGET_EXPAND(satfractsq<mode>2,(operands[0],operands[1]));
  "
)

(define_expand "satfractusq<mode>2"
  [(set (match_operand:QUQ15 0 "pic30_register_operand"  "=r")
        (sat_fract:QUQ15
          (match_operand:USQ 1 "pic30_register_operand"  "r")))]
  ""      
  "
    PIC30_TARGET_EXPAND(satfractusq<mode>2,(operands[0],operands[1]));
  "
)

(define_expand "satfractdq<mode>2"
  [(set (match_operand:QUQ15 0 "pic30_register_operand"  "=r")
        (sat_fract:QUQ15
          (match_operand:DQ  1 "pic30_register_operand"  "r")))]
  ""
  "
    PIC30_TARGET_EXPAND(satfractdq<mode>2,(operands[0],operands[1]));
  "
)

(define_expand "satfractudq<mode>2"
  [(set (match_operand:QUQ15 0 "pic30_register_operand"  "=r")
        (sat_fract:QUQ15
          (match_operand:UDQ 1 "pic30_register_operand"  "r")))]
  ""      
  "
    PIC30_TARGET_EXPAND(satfractudq<mode>2,(operands[0],operands[1]));
  "
)

(define_expand "satfracttq<mode>2"
  [(set (match_operand:QUQ15 0 "pic30_register_operand"  "=r")
        (sat_fract:QUQ15
          (match_operand:TQ  1 "pic30_register_operand"  "r")))]
  ""
  "
    PIC30_TARGET_EXPAND(satfracttq<mode>2,(operands[0],operands[1]));
  "
)

(define_expand "satfractutq<mode>2"
  [(set (match_operand:QUQ15 0 "pic30_register_operand"  "=r")
        (sat_fract:QUQ15
          (match_operand:UTQ 1 "pic30_register_operand"  "r")))]
  ""      
  "
    PIC30_TARGET_EXPAND(satfractutq<mode>2,(operands[0],operands[1]));
  "
)

(define_expand "satfractqq<mode>2"
  [(set (match_operand:QUQ31 0 "pic30_register_operand" "=r")
        (sat_fract:QUQ31
          (match_operand:QQ  1 "pic30_register_operand" "r")))]
  ""
  "
    PIC30_TARGET_EXPAND(satfractqq<mode>2,(operands[0],operands[1]));
  "
)

(define_expand "satfractuqq<mode>2"
  [(set (match_operand:QUQ31 0 "pic30_register_operand" "=r")
        (sat_fract:QUQ31
          (match_operand:UQQ  1 "pic30_register_operand" "r")))]
  ""
  "
    PIC30_TARGET_EXPAND(satfractuqq<mode>2,(operands[0],operands[1]));
  "
)

(define_expand "satfracthq<mode>2"
  [(set (match_operand:QUQ31 0 "pic30_register_operand" "=r")
        (sat_fract:QUQ31
          (match_operand:HQ  1 "pic30_register_operand" "r")))]
  ""
  "
    PIC30_TARGET_EXPAND(satfracthq<mode>2,(operands[0],operands[1]));
  "
)

(define_expand "satfractuhq<mode>2"
  [(set (match_operand:QUQ31 0 "pic30_register_operand" "=r")
        (sat_fract:QUQ31
          (match_operand:UHQ  1 "pic30_register_operand" "r")))]
  ""
  "
    PIC30_TARGET_EXPAND(satfractuhq<mode>2,(operands[0],operands[1]));
  "
)

;; conversions between different fractional modes of the same size
;;   these are nops but must exist

(define_expand "satfract<mode>sq2"
  [(set (match_operand:SQ      0 "pic30_register_operand" "=r,r")
        (sat_fract:SQ
          (match_operand:QUQ31 1 "pic30_register_operand" "r,0")))]
  ""
  "
    PIC30_TARGET_EXPAND(satfract<mode>sq2,(operands[0],operands[1]));
  "
)

(define_expand "satfract<mode>dq2"
  [(set (match_operand:DQ      0 "pic30_register_operand" "=r,r")
        (sat_fract:DQ
          (match_operand:QUQ31 1 "pic30_register_operand" "r,0")))]
  ""
  "
    PIC30_TARGET_EXPAND(satfract<mode>dq2,(operands[0],operands[1]));
  "
)

(define_expand "satfract<mode>tq2"
  [(set (match_operand:TQ      0 "pic30_register_operand" "=r,r")
        (sat_fract:TQ
          (match_operand:QUQ31 1 "pic30_register_operand" "r,0")))]
  ""
  "
    PIC30_TARGET_EXPAND(satfract<mode>tq2,(operands[0],operands[1]));
  "
)

(define_expand "satfract<mode>qq2"
  [(set (match_operand:QQ      0 "pic30_register_operand" "=r,r")
        (sat_fract:QQ
          (match_operand:QUQ15 1 "pic30_register_operand" "r,0")))]
  ""
  "
    PIC30_TARGET_EXPAND(satfract<mode>qq2,(operands[0],operands[1]));
  "
)

(define_expand "satfract<mode>hq2"
  [(set (match_operand:HQ      0 "pic30_register_operand" "=r,r")
        (sat_fract:HQ
          (match_operand:QUQ15 1 "pic30_register_operand" "r,0")))]
  ""
  "
    PIC30_TARGET_EXPAND(satfract<mode>hq2,(operands[0],operands[1]));
  "
)

; missing conversions between signed/unsigned 'self'

(define_insn "satfractqquqq2"
  [(set (match_operand:UQQ  0 "register_operand" "=r")
       (unsigned_sat_fract:UQQ
         (match_operand:QQ 1 "register_operand" "0")))]
  ""
  "; nop"
)

(define_insn "satfracthquhq2"
  [(set (match_operand:UHQ  0 "register_operand" "=r")
       (unsigned_sat_fract:UHQ
         (match_operand:HQ 1 "register_operand" "0")))]
  ""
  "; nop"
)

(define_insn "satfractsqusq2"
  [(set (match_operand:USQ  0 "register_operand" "=r")
       (unsigned_sat_fract:USQ
         (match_operand:SQ 1 "register_operand" "0")))]
  ""
  "; nop"
)

(define_insn "satfractdqudq2"
  [(set (match_operand:UDQ  0 "register_operand" "=r")
       (unsigned_sat_fract:UDQ
         (match_operand:DQ 1 "register_operand" "0")))]
  ""
  "; nop"
)

(define_insn "satfracttqutq2"
  [(set (match_operand:UTQ  0 "register_operand" "=r")
       (unsigned_sat_fract:UTQ
         (match_operand:TQ 1 "register_operand" "0")))]
  ""
  "; nop"
)

(define_insn "satfracthauha2"
  [(set (match_operand:UHA  0 "register_operand" "=r")
       (unsigned_sat_fract:UHA
         (match_operand:HA 1 "register_operand" "0")))]
  ""
  "; nop"
)

(define_insn "satfractuhaha2"
  [(set (match_operand:HA    0 "register_operand" "=r")
       (sat_fract:HA
         (match_operand:UHA 1 "register_operand" "0")))]
  ""
  "; nop"
)

(define_insn "satfractsausa2"
  [(set (match_operand:USA  0 "register_operand" "=r")
       (unsigned_sat_fract:USA
         (match_operand:SA 1 "register_operand" "0")))]
  ""
  "; nop"
)

(define_insn "satfractusasa2"
  [(set (match_operand:SA    0 "register_operand" "=r")
       (sat_fract:SA
         (match_operand:USA 1 "register_operand" "0")))]
  ""
  "; nop"
)

(define_insn "satfractdauda2"
  [(set (match_operand:UDA  0 "register_operand" "=r")
       (unsigned_sat_fract:UDA
         (match_operand:DA 1 "register_operand" "0")))]
  ""
  "; nop"
)

(define_insn "satfractudada2"
  [(set (match_operand:DA    0 "register_operand" "=r")
       (sat_fract:DA
         (match_operand:UDA 1 "register_operand" "0")))]
  ""
  "; nop"
)

(define_insn "satfracttauta2"
  [(set (match_operand:UTA  0 "register_operand" "=r")
       (unsigned_sat_fract:UTA
         (match_operand:TA 1 "register_operand" "0")))]
  ""
  "; nop"
)

(define_insn "satfractutata2"
  [(set (match_operand:TA    0 "register_operand" "=r")
       (sat_fract:TA
         (match_operand:UTA 1 "register_operand" "0")))]
  ""
  "; nop"
)

;; negate

(define_expand "neg<mode>2"
  [(set (match_operand:QQQQQ   0 "pic30_mode2_operand" "=rR<>")
        (neg:QQQQQ
          (match_operand:QQQQQ 1 "pic30_mode2_operand" " rR<>")))]
  ""
  "
    PIC30_TARGET_EXPAND(neg<mode>2,(operands[0],operands[1]));
  "
)

(define_expand "ssneg<mode>2"
  [(set (match_operand:QQQQQ   0 "pic30_register_operand" "=&r")
        (ss_neg:QQQQQ
          (match_operand:QQQQQ 1 "pic30_register_operand" " r")))]
  "pic30_fp_inline_p()"
  "
    PIC30_TARGET_EXPAND(ssneg<mode>2,(operands[0],operands[1]));
  "
)

(define_expand "usneg<mode>2"
  [(set (match_operand:QQQQQ   0 "pic30_register_operand" "=&r")
        (us_neg:QQQQQ
          (match_operand:QQQQQ 1 "pic30_register_operand" " r")))]
  ""
  " 
    PIC30_TARGET_EXPAND(usneg<mode>2,(operands[0],operands[1]));
  "
)

;; shift

(define_expand "ashl<mode>3"
  [(set (match_operand:QUQ15   0 "pic30_register_operand"       "=r")
        (ashift:QUQ15 
          (match_operand:QUQ15 1 "pic30_register_operand"       "r")
          (match_operand:HI    2 "pic30_register_operand"       "r")))
  ]
  ""
  " 
    PIC30_TARGET_EXPAND(ashl<mode>3,(operands[0],operands[1],operands[2]));
  "
)

(define_expand "ssashl<mode>3"
  [(set (match_operand:Q15   0 "pic30_register_operand"     "=&r")
        (ss_ashift:Q15
          (match_operand:Q15 1 "pic30_register_operand"       "r")
          (match_operand:HI  2 "pic30_register_operand"       "r")))
  ]
  ""
  " 
    PIC30_TARGET_EXPAND(ssashl<mode>3,(operands[0],operands[1],operands[2]));
  "
)

(define_expand "ssashl<mode>3"
  [(set (match_operand:UQ15   0 "pic30_register_operand"     "=&r")
        (ss_ashift:UQ15
          (match_operand:UQ15 1 "pic30_register_operand"       "r")
          (match_operand:HI   2 "pic30_register_operand"       "r")))
  ]
  ""
  " 
    PIC30_TARGET_EXPAND(ssashl<mode>3,(operands[0],operands[1],operands[2]));
  "
)

(define_expand "usashl<mode>3"
  [(set (match_operand:QUQ15   0 "pic30_register_operand"     "=&r")
        (us_ashift:QUQ15
          (match_operand:QUQ15 1 "pic30_register_operand"       "r")
          (match_operand:HI    2 "pic30_register_operand"       "r")))
  ]
  ""
  " 
    PIC30_TARGET_EXPAND(usashl<mode>3,(operands[0],operands[1],operands[2]));
  "
)

; 32-bit shift left

(define_expand "ashl<mode>3"
  [(set (match_operand:QUQ31   0 "pic30_register_operand" "")
        (ashift:QUQ31 
          (match_operand:QUQ31 1 "pic30_register_operand" "")
          (match_operand:HI    2 "pic30_reg_or_imm_operand" "")))]
  ""
  " 
    PIC30_TARGET_EXPAND(ashl<mode>3,(operands[0],operands[1],operands[2]));
  "
)

(define_expand "ssashl<mode>3"
  [(set (match_operand:QUQ31   0 "pic30_register_operand" "")
        (ss_ashift:QUQ31 
          (match_operand:QUQ31 1 "pic30_register_operand" "")
          (match_operand:HI    2 "pic30_reg_or_imm_operand" "")))]
  ""
  " 
    PIC30_TARGET_EXPAND(ssashl<mode>3,(operands[0],operands[1],operands[2]));
  "
)

(define_expand "usashl<mode>3"
  [(set (match_operand:QUQ31   0 "pic30_register_operand" "")
        (us_ashift:QUQ31 
          (match_operand:QUQ31 1 "pic30_register_operand" "")
          (match_operand:HI    2 "pic30_reg_or_imm_operand" "")))]
  ""
  " 
    PIC30_TARGET_EXPAND(usashl<mode>3,(operands[0],operands[1],operands[2]));
  "
)

(define_expand "ashr<mode>3"
  [(set (match_operand:QQQQQ   0 "pic30_register_operand"   "")
        (ashiftrt:QQQQQ
          (match_operand:QQQQQ 1 "pic30_register_operand"   "")
          (match_operand:HI    2 "pic30_reg_or_imm_operand" "")))]
  ""
  " 
    PIC30_TARGET_EXPAND(ashr<mode>3,(operands[0],operands[1],operands[2]));
  "
)

; logical shift right

(define_expand "lshr<mode>3"
  [(set (match_operand:QQQQQ   0 "pic30_register_operand"   "")
        (lshiftrt:QQQQQ
          (match_operand:QQQQQ 1 "pic30_register_operand"   "")
          (match_operand:HI    2 "pic30_reg_or_imm_operand" "")))]
  ""
  " 
    PIC30_TARGET_EXPAND(lshr<mode>3,(operands[0],operands[1],operands[2]));
  "
)

; compare

(define_expand "cmp<mode>"
  [(set (reg:CC CC_REG)
        (compare 
          (match_operand:QQQQQ 0 "pic30_general_operand" "")
          (match_operand:QQQQQ 1 "pic30_general_operand" "")))
  ]
  ""
  " 
    PIC30_TARGET_EXPAND(cmp<mode>,(operands[0],operands[1]));
  "
) 

; Fixed Point Accum

;; *** load/store

(define_expand "mov<mode>"
  [(set (match_operand:AUACC 0 "pic30_move2_operand" "")
        (match_operand:AUACC 1 "pic30_move2_operand" ""))]
  ""
  "
    PIC30_TARGET_EXPAND(mov<mode>,(operands[0],operands[1]));
  "
)

;; *** add/sub

(define_expand "add<mode>3"
  [(set (match_operand:AUACC    0 "pic30_accum_or_reg_operand" "=&r,w")
        (plus:AUACC
           (match_operand:AUACC 1 "pic30_accum_or_reg_operand"  "%r,0")
           (match_operand:AUACC 2 "pic30_accum_or_reg_operand"  " r,w")))
  ]
  ""
  "
    PIC30_TARGET_EXPAND(add<mode>3,(operands[0],operands[1],operands[2]));
  "
)

(define_expand "ssadd<mode>3"
  [(set (match_operand:AUACC    0 "pic30_accum_or_reg_operand" "=&r,w")
        (ss_plus:AUACC
           (match_operand:AUACC 1 "pic30_accum_or_reg_operand"  "%r,0")
           (match_operand:AUACC 2 "pic30_accum_or_reg_operand"  " r,0")))
  ]
  ""
  "
    PIC30_TARGET_EXPAND(ssadd<mode>3,(operands[0],operands[1],operands[2]));
  "
)

(define_expand "usadd<mode>3"
  [(set (match_operand:AUACC    0 "pic30_accum_or_reg_operand" "=&r,w")
        (us_plus:AUACC
           (match_operand:AUACC 1 "pic30_accum_or_reg_operand"  "%r,0")
           (match_operand:AUACC 2 "pic30_accum_or_reg_operand"   "r,w")))
  ]
  ""
  "
    PIC30_TARGET_EXPAND(usadd<mode>3,(operands[0],operands[1],operands[2]));
  "
)

(define_expand "sub<mode>3"
  [(set (match_operand:AUACC    0 "pic30_accum_or_mode1_operand" "")
        (minus:AUACC
           (match_operand:AUACC 1 "pic30_accum_or_reg_operand"   "")
           (match_operand:AUACC 2 "pic30_accum_or_mode1_operand" "")))
  ]
  ""
  "
    PIC30_TARGET_EXPAND(sub<mode>3,(operands[0],operands[1],operands[2]));
  "
)

(define_expand "sssub<mode>3"
  [(set (match_operand:AUACC    0 "pic30_accum_or_reg_operand" "=&r,w")
        (ss_minus:AUACC
           (match_operand:AUACC 1 "pic30_accum_or_reg_operand"  " r,0")
           (match_operand:AUACC 2 "pic30_accum_or_reg_operand"  " r,w")))
  ]
  ""
  "
    PIC30_TARGET_EXPAND(sssub<mode>3,(operands[0],operands[1],operands[2]));
  "
)

(define_expand "ussub<mode>3"
  [(set (match_operand:AUACC    0 "pic30_accum_or_reg_operand" "=&r,w")
        (us_minus:AUACC
           (match_operand:AUACC 1 "pic30_accum_or_reg_operand"  " r,0")
           (match_operand:AUACC 2 "pic30_accum_or_reg_operand"  " r,w")))
  ]
  ""
  "
    PIC30_TARGET_EXPAND(ussub<mode>3,(operands[0],operands[1],operands[2]));
  "
)

;; multiply of accumulators, ie 8.31*8.31 will be handled by a function

; widening multiply/ multiply accumulate

(define_expand "muluqq<mode>3"
  [(set (match_operand:AUACC   0 "pic30_accumulator2_operand" "=w")
        (mult:AUACC
          (fract_convert:AUACC
            (match_operand:UQQ 1 "pic30_mac_input_operand"     "za"))
          (fract_convert:AUACC
            (match_operand:UQQ 2 "pic30_mac_input_operand"     "za"))))
  ]
  "pic30_dsp_target()"
  "
    PIC30_DSP_TARGET_EXPAND(muluqq<mode>3,
                            (operands[0],operands[1],operands[2]));
  "
)

(define_expand "mulqq<mode>3"
  [(set (match_operand:AUACC   0 "pic30_accumulator2_operand" "=w")
        (mult:AUACC
          (fract_convert:AUACC
            (match_operand:QQ  1 "pic30_mac_input_operand"     "za"))
          (fract_convert:AUACC
            (match_operand:QQ  2 "pic30_mac_input_operand"     "za"))))
  ]
  "pic30_dsp_target()"
  "
    PIC30_DSP_TARGET_EXPAND(mulqq<mode>3,(operands[0],operands[1],operands[2]));
  "
)

(define_expand "umadduqq<mode>4"
  [(set (match_operand:AUACC       0 "pic30_accumulator2_operand" "=w")
        (plus:AUACC 
           (match_operand:AUACC    3 "pic30_accumulator2_operand" " 0")
           (mult:AUACC 
              (fract_convert:AUACC
                (match_operand:UQQ 1 "pic30_mac_input_operand"     "za"))
              (fract_convert:AUACC
                (match_operand:UQQ 2 "pic30_mac_input_operand"     "za")))))

  ]
  "pic30_dsp_target()"
  "
    PIC30_DSP_TARGET_EXPAND(umadduqq<mode>4,
                  (operands[0],operands[1],operands[2],operands[3]));
  "
)

(define_expand "maddqq<mode>4"
  [(set (match_operand:AUACC      0 "pic30_accumulator2_operand" "=w")
        (plus:AUACC 
           (match_operand:AUACC   3 "pic30_accumulator2_operand" " 0")
           (mult:AUACC 
              (fract_convert:AUACC
                (match_operand:QQ 1 "pic30_mac_input_operand"     "za"))
              (fract_convert:AUACC
                (match_operand:QQ 2 "pic30_mac_input_operand"     "za")))))
  ]
  "pic30_dsp_target()"
  "
    PIC30_DSP_TARGET_EXPAND(maddqq<mode>4,
                  (operands[0],operands[1],operands[2],operands[3]));
  "
)

(define_expand "umaddhisi4"
  [(set (match_operand:SI          0 "pic30_accumulator2_operand" "=w")
        (plus:SI
           (match_operand:SI       3 "pic30_accumulator2_operand" " 0")
           (mult:SI
              (zero_extend:SI
                (match_operand:HI  1 "pic30_mac_input_operand"     "za"))
              (zero_extend:SI
                (match_operand:HI  2 "pic30_mac_input_operand"     "za")))))

  ]
  "pic30_integer_mac_support"
  "
    PIC30_DSP_TARGET_EXPAND(umaddhisi4,
                  (operands[0],operands[1],operands[2],operands[3]));
  "
)

(define_expand "maddhisi4"
  [(set (match_operand:SI         0 "pic30_accumulator2_operand" "=w")
        (plus:SI
           (match_operand:SI      3 "pic30_accumulator2_operand" " 0")
           (mult:SI
              (sign_extend:SI
                (match_operand:HI 1 "pic30_mac_input_operand"     "za"))
              (sign_extend:SI
                (match_operand:HI 2 "pic30_mac_input_operand"     "za")))))
  ]
  "pic30_integer_mac_support"
  "
    PIC30_DSP_TARGET_EXPAND(maddhisi4,
                  (operands[0],operands[1],operands[2],operands[3]));
  "
)

(define_expand "umsubuqq<mode>4"
  [(set (match_operand:AUACC       0 "pic30_accumulator2_operand" "=w")
        (minus:AUACC 
           (match_operand:AUACC    3 "pic30_accumulator2_operand" " 0")
           (mult:AUACC 
              (fract_convert:AUACC
                (match_operand:UQQ 1 "pic30_mac_input_operand"     "za"))
              (fract_convert:AUACC
                (match_operand:UQQ 2 "pic30_mac_input_operand"     "za")))))
  ]
  "pic30_dsp_target()"
  "
    PIC30_DSP_TARGET_EXPAND(umsubuqq<mode>4,
                  (operands[0],operands[1],operands[2],operands[3]));
  "
)

(define_expand "msubqq<mode>4"
  [(set (match_operand:AUACC      0 "pic30_accumulator2_operand" "=w")
        (minus:AUACC 
           (match_operand:AUACC   3 "pic30_accumulator2_operand" " 0")
           (mult:AUACC 
              (fract_convert:AUACC
                (match_operand:QQ 1 "pic30_mac_input_operand"     "za"))
              (fract_convert:AUACC
                (match_operand:QQ 2 "pic30_mac_input_operand"     "za")))))
  ]
  "pic30_dsp_target()"
  "
    PIC30_DSP_TARGET_EXPAND(msubqq<mode>4,
                  (operands[0],operands[1],operands[2],operands[3]));
  "
)

;; negate

(define_expand "neg<mode>2"
  [(set (match_operand:ACC   0 "pic30_accum_or_reg_operand" "=w,&r")
        (neg:ACC
          (match_operand:ACC 1 "pic30_accum_or_reg_operand" " 0,r")))
  ]
  ""
  "
    PIC30_TARGET_EXPAND(neg<mode>2,(operands[0],operands[1]));
  "
)

(define_expand "ssneg<mode>2"
  [(set (match_operand:ACC   0 "pic30_accum_or_reg_operand" "=w,&r")
        (ss_neg:ACC
          (match_operand:ACC 1 "pic30_accum_or_reg_operand" " 0,r")))
  ]
  ""
  "
    PIC30_TARGET_EXPAND(ssneg<mode>2,(operands[0],operands[1]));
  "
)
(define_expand "usneg<mode>2"
  [(set (match_operand:ACC   0 "pic30_accum_or_reg_operand" "=w,&r")
        (us_neg:ACC
          (match_operand:ACC 1 "pic30_accum_or_reg_operand" " 0,r")))
  ]
  ""
  "
    PIC30_TARGET_EXPAND(neg<mode>2,(operands[0],operands[1]));
  "
)

;; shift

; sftac can only shift +-16 for some unfathomable reason
; this might be better handled in a library fn

(define_expand "ashl<mode>3"
  [(set (match_operand:AUACC   0 "pic30_accum_or_reg_operand"   "=w,r")
        (ashift:AUACC
          (match_operand:AUACC 1 "pic30_accum_or_reg_operand"    "w,r")
          (match_operand:HI    2 "pic30_reg_or_imm_operand"      "ri,ri")))
  ]
  ""
  "
    PIC30_TARGET_EXPAND(ashl<mode>3,(operands[0],operands[1],operands[2]));
  "
)

; ss arithmetic shift left

(define_expand "ssashl<mode>3"
  [(set (match_operand:AUACC   0 "pic30_accum_or_reg_operand"   "=w,r")
        (ss_ashift:AUACC
          (match_operand:AUACC 1 "pic30_accum_or_reg_operand"    "w,r")
          (match_operand:HI    2 "pic30_reg_or_imm_operand"      "ri,ri")))
  ]
  ""
  "
    PIC30_TARGET_EXPAND(ssashl<mode>3,(operands[0],operands[1],operands[2]));
  "
)

; us arithmetic shift left

(define_expand "usashl<mode>3"
  [(set (match_operand:AUACC   0 "pic30_accum_or_reg_operand"   "=w,r")
        (us_ashift:AUACC
          (match_operand:AUACC 1 "pic30_accum_or_reg_operand"    "w,r")
          (match_operand:HI    2 "pic30_reg_or_imm_operand"      "ri,ri")))
  ]
  ""
  "
    PIC30_TARGET_EXPAND(usashl<mode>3,(operands[0],operands[1],operands[2]));
  "
)

; logical shift right 

(define_expand "lshr<mode>3"
  [(set (match_operand:AUACC   0 "pic30_accum_or_reg_operand"   "=w,r")
        (lshiftrt:AUACC
          (match_operand:AUACC 1 "pic30_accum_or_reg_operand"    "w,r")
          (match_operand:HI    2 "pic30_reg_or_imm_operand"      "ri,ri")))
  ]
  ""
  "
    PIC30_TARGET_EXPAND(lshr<mode>3,(operands[0],operands[1],operands[2]));
  "
)

; arithemetic shift right

(define_expand "ashr<mode>3"
  [(set (match_operand:AUACC   0 "pic30_accum_or_reg_operand"   "=w,r")
        (ashiftrt:AUACC
          (match_operand:AUACC 1 "pic30_accum_or_reg_operand"    "w,r")
          (match_operand:HI    2 "pic30_register_operand"        "r,r")))
  ]
  ""
  "
    PIC30_TARGET_EXPAND(ashr<mode>3,(operands[0],operands[1],operands[2]));
  "
)

;; compare

(define_expand "cmp<mode>"
  [(set (reg:CC CC_REG)
        (compare 
          (match_operand:AUACC 0 "" "w,r")
          (match_operand:AUACC 1 "" "w,r")))
  ]
  "" 
  "
    PIC30_TARGET_EXPAND(cmp<mode>,(operands[0],operands[1]));
  "
)

;; Accum / fract conversions
;  to Q15

(define_expand "fract<mode>qq2"
  [(set (match_operand:QQ    0 "pic30_register_operand"    "=r,r")
      (fract_convert:QQ
        (match_operand:AUACC 1 "pic30_accum_or_reg_operand" "w,r")))
  ]
  ""
  " 
    PIC30_TARGET_EXPAND(fract<mode>qq2,(operands[0],operands[1]));
  "
)   

(define_expand "fract<mode>uqq2"
  [(set (match_operand:UQQ   0 "pic30_register_operand"    "=r,r")
      (fract_convert:UQQ
        (match_operand:AUACC 1 "pic30_accum_or_reg_operand" "w,r")))
  ]
  ""
  "
    PIC30_TARGET_EXPAND(fract<mode>uqq2,(operands[0],operands[1]));
  "
)   

(define_expand "fract<mode>hq2"
  [(set (match_operand:HQ    0 "pic30_register_operand"    "=r,r")
      (fract_convert:QQ
        (match_operand:AUACC 1 "pic30_accum_or_reg_operand" "w,r")))
  ]
  ""
  "
    PIC30_TARGET_EXPAND(fract<mode>hq2,(operands[0],operands[1]));
  "
)   

(define_expand "fract<mode>uhq2"
  [(set (match_operand:UHQ   0 "pic30_register_operand"    "=r,r")
      (fract_convert:UHQ
        (match_operand:AUACC 1 "pic30_accum_or_reg_operand" "w,r")))
  ]
  ""
  "
    PIC30_TARGET_EXPAND(fract<mode>uhq2,(operands[0],operands[1]));
  "
)   

; to Q31

(define_expand "fract<mode>sq2"
  [(set (match_operand:SQ      0 "pic30_register_operand"     "=r,r")
        (fract_convert:SQ
          (match_operand:AUACC 1 "pic30_accum_or_reg_operand" " w,r")))]
  ""
  "
    PIC30_TARGET_EXPAND(fract<mode>sq2,(operands[0],operands[1]));
  "
)
 
(define_expand "fract<mode>usq2"
  [(set (match_operand:USQ      0 "pic30_register_operand"     "=r,r")
        (fract_convert:USQ
          (match_operand:AUACC 1 "pic30_accum_or_reg_operand" " w,r")))]
  ""
  "
    PIC30_TARGET_EXPAND(fract<mode>usq2,(operands[0],operands[1]));
  "
)

(define_expand "fract<mode>dq2"
  [(set (match_operand:DQ      0 "pic30_register_operand"     "=r,r")
        (fract_convert:DQ
          (match_operand:AUACC 1 "pic30_accum_or_reg_operand" " w,r")))]
  ""
  "
    PIC30_TARGET_EXPAND(fract<mode>dq2,(operands[0],operands[1]));
  "
)
 
(define_expand "fract<mode>udq2"
  [(set (match_operand:UDQ      0 "pic30_register_operand"     "=r,r")
        (fract_convert:UDQ
          (match_operand:AUACC 1 "pic30_accum_or_reg_operand" " w,r")))]
  ""
  "
    PIC30_TARGET_EXPAND(fract<mode>udq2,(operands[0],operands[1]));
  "
)

(define_expand "fract<mode>tq2"
  [(set (match_operand:TQ      0 "pic30_register_operand"     "=r,r")
        (fract_convert:TQ
          (match_operand:AUACC 1 "pic30_accum_or_reg_operand" " w,r")))]
  ""
  "
    PIC30_TARGET_EXPAND(fract<mode>tq2,(operands[0],operands[1]));
  "
)
 
(define_expand "fract<mode>utq2"
  [(set (match_operand:UTQ      0 "pic30_register_operand"     "=r,r")
        (fract_convert:UTQ
          (match_operand:AUACC 1 "pic30_accum_or_reg_operand" " w,r")))]
  ""
  "
    PIC30_TARGET_EXPAND(fract<mode>utq2,(operands[0],operands[1]));
  "
)

;  saturating to Q15

(define_expand "satfract<mode>qq2"
  [(set (match_operand:QQ    0 "pic30_register_operand"    "=r,r")
      (sat_fract:QQ
        (match_operand:AUACC 1 "pic30_accum_or_reg_operand" "w,r")))
  ]
  ""
  "
    PIC30_TARGET_EXPAND(satfract<mode>qq2,(operands[0],operands[1]));
  "
)   

(define_expand "satfract<mode>uqq2"
  [(set (match_operand:UQQ   0 "pic30_register_operand"    "=r,r")
      (sat_fract:UQQ
        (match_operand:AUACC 1 "pic30_accum_or_reg_operand" "w,r")))
  ]
  ""
  "
    PIC30_TARGET_EXPAND(satfract<mode>uqq2,(operands[0],operands[1]));
  "
)   

(define_expand "satfract<mode>hq2"
  [(set (match_operand:HQ    0 "pic30_register_operand"    "=r,r")
      (sat_fract:QQ
        (match_operand:AUACC 1 "pic30_accum_or_reg_operand" "w,r")))
  ]
  ""
  "
    PIC30_TARGET_EXPAND(fract<mode>hq2,(operands[0],operands[1]));
  "
)   

(define_expand "satfract<mode>uhq2"
  [(set (match_operand:UHQ   0 "pic30_register_operand"    "=r,r")
      (sat_fract:UHQ
        (match_operand:AUACC 1 "pic30_accum_or_reg_operand" "w,r")))
  ]
  ""
  "
    PIC30_TARGET_EXPAND(fract<mode>uhq2,(operands[0],operands[1]));
  "
)   


; to Q31 - done by routine, these patterns are too long (and complex)

;  from Q15

(define_expand "fractqq<mode>2"
  [(set (match_operand:AUACC 0 "pic30_accum_or_reg_operand" "=w,r")
        (fract_convert:AUACC
          (match_operand:QQ  1 "pic30_register_operand"     " r,r")))]
  ""
  "
    PIC30_TARGET_EXPAND(fractqq<mode>2,(operands[0],operands[1]));
  "
)
  
(define_expand "fractuqq<mode>2"
  [(set (match_operand:AUACC 0 "pic30_accum_or_reg_operand" "=w,r")
        (fract_convert:AUACC
          (match_operand:UQQ 1 "pic30_register_operand"     " r,r")))]
  ""
  "
    PIC30_TARGET_EXPAND(fractuqq<mode>2,(operands[0],operands[1]));
  "
)
  
(define_expand "fracthq<mode>2"
  [(set (match_operand:AUACC 0 "pic30_accum_or_reg_operand" "=w,r")
        (fract_convert:AUACC
          (match_operand:HQ  1 "pic30_register_operand"     " r,r")))]
  ""
  "
    PIC30_TARGET_EXPAND(fracthq<mode>2,(operands[0],operands[1]));
  "
)
  
(define_expand "fractuhq<mode>2"
  [(set (match_operand:AUACC 0 "pic30_accum_or_reg_operand" "=w,r")
        (fract_convert:AUACC
          (match_operand:UHQ 1 "pic30_register_operand"     " r,r")))]
  ""
  "
    PIC30_TARGET_EXPAND(fractuhq<mode>2,(operands[0],operands[1]));
  "
)

; from Q31

(define_expand "fractsq<mode>2"
  [(set (match_operand:AUACC 0 "pic30_accum_or_reg_operand" "=w,r")
        (fract_convert:AUACC
          (match_operand:SQ  1 "pic30_register_operand"     " r,r")))]
  ""
  "
    PIC30_TARGET_EXPAND(fractsq<mode>2,(operands[0],operands[1]));
  "
)
 
(define_expand "fractusq<mode>2"
  [(set (match_operand:AUACC 0 "pic30_accum_or_reg_operand" "=w,r")
        (fract_convert:AUACC
          (match_operand:USQ 1 "pic30_register_operand"     " r,r")))]
  ""
  "
    PIC30_TARGET_EXPAND(fractusq<mode>2,(operands[0],operands[1]));
  "
)

(define_expand "fractdq<mode>2"
  [(set (match_operand:AUACC 0 "pic30_accum_or_reg_operand" "=w,r")
        (fract_convert:AUACC
          (match_operand:DQ  1 "pic30_register_operand"     " r,r")))]
  ""
  "
    PIC30_TARGET_EXPAND(fractdq<mode>2,(operands[0],operands[1]));
  "
)
 
(define_expand "fractudq<mode>2"
  [(set (match_operand:AUACC 0 "pic30_accum_or_reg_operand" "=w,r")
        (fract_convert:AUACC
          (match_operand:UDQ 1 "pic30_register_operand"     " r,r")))]
  ""
  "
    PIC30_TARGET_EXPAND(fractudq<mode>2,(operands[0],operands[1]));
  "
)

(define_expand "fracttq<mode>2"
  [(set (match_operand:AUACC 0 "pic30_accum_or_reg_operand" "=w,r")
        (fract_convert:AUACC
          (match_operand:TQ  1 "pic30_register_operand"     " r,r")))]
  ""
  "
    PIC30_TARGET_EXPAND(fracttq<mode>2,(operands[0],operands[1]));
  "
)
 
(define_expand "fractutq<mode>2"
  [(set (match_operand:AUACC 0 "pic30_accum_or_reg_operand" "=w,r")
        (fract_convert:AUACC
          (match_operand:UTQ 1 "pic30_register_operand"     " r,r")))]
  ""
  "
    PIC30_TARGET_EXPAND(fractqq<mode>2,(operands[0],operands[1]));
  "
)

;  saturating from Q15
;   these never saturate because AUACC is always bigger than a Qn

(define_expand "satfractqq<mode>2"
  [(set (match_operand:AUACC 0 "pic30_accum_or_reg_operand" "=w,r")
        (sat_fract:AUACC
          (match_operand:QQ  1 "pic30_register_operand"     " r,r")))]
  ""
  "
    PIC30_TARGET_EXPAND(satfractqq<mode>2,(operands[0],operands[1]));
  "
)
  
(define_expand "satfractuqq<mode>2"
  [(set (match_operand:AUACC 0 "pic30_accum_or_reg_operand" "=w,r")
        (sat_fract:AUACC
          (match_operand:UQQ 1 "pic30_register_operand"     " r,r")))]
  ""
  "
    PIC30_TARGET_EXPAND(satfractuqq<mode>2,(operands[0],operands[1]));
  "
)
  
(define_expand "satfracthq<mode>2"
  [(set (match_operand:AUACC 0 "pic30_accum_or_reg_operand" "=w,r")
        (sat_fract:AUACC
          (match_operand:HQ  1 "pic30_register_operand"     " r,r")))]
  ""
  "
    PIC30_TARGET_EXPAND(satfracthq<mode>2,(operands[0],operands[1]));
  "
)
  
(define_expand "satfractuhq<mode>2"
  [(set (match_operand:AUACC 0 "pic30_accum_or_reg_operand" "=w,r")
        (sat_fract:AUACC
          (match_operand:UHQ 1 "pic30_register_operand"     " r,r")))]
  ""
  "
    PIC30_TARGET_EXPAND(satfractuhq<mode>2,(operands[0],operands[1]));
  "
)

; from Q31

(define_expand "satfractsq<mode>2"
  [(set (match_operand:AUACC 0 "pic30_accum_or_reg_operand" "=w,r")
        (sat_fract:AUACC
          (match_operand:SQ  1 "pic30_register_operand"     " r,r")))]
  ""
  "
    PIC30_TARGET_EXPAND(satfractsq<mode>2,(operands[0],operands[1]));
  "
)
 
(define_expand "satfractusq<mode>2"
  [(set (match_operand:AUACC 0 "pic30_accum_or_reg_operand" "=w,r")
        (sat_fract:AUACC
          (match_operand:USQ 1 "pic30_register_operand"     " r,r")))]
  ""
  "
    PIC30_TARGET_EXPAND(satfractusq<mode>2,(operands[0],operands[1]));
  "
)

(define_expand "satfractdq<mode>2"
  [(set (match_operand:AUACC 0 "pic30_accum_or_reg_operand" "=w,r")
        (sat_fract:AUACC
          (match_operand:DQ  1 "pic30_register_operand"     " r,r")))]
  ""
  "
    PIC30_TARGET_EXPAND(satfractdq<mode>2,(operands[0],operands[1]));
  "
)
 
(define_expand "satfractudq<mode>2"
  [(set (match_operand:AUACC 0 "pic30_accum_or_reg_operand" "=w,r")
        (sat_fract:AUACC
          (match_operand:UDQ 1 "pic30_register_operand"     " r,r")))]
  ""
  "
    PIC30_TARGET_EXPAND(satfractudq<mode>2,(operands[0],operands[1]));
  "
)

(define_expand "satfracttq<mode>2"
  [(set (match_operand:AUACC 0 "pic30_accum_or_reg_operand" "=w,r")
        (sat_fract:AUACC
          (match_operand:TQ  1 "pic30_register_operand"     " r,r")))]
  ""
  "
    PIC30_TARGET_EXPAND(satfracttq<mode>2,(operands[0],operands[1]));
  "
)
 
(define_expand "satfractutq<mode>2"
  [(set (match_operand:AUACC 0 "pic30_accum_or_reg_operand" "=w,r")
        (sat_fract:AUACC
          (match_operand:UTQ 1 "pic30_register_operand"     " r,r")))]
  ""
  "
    PIC30_TARGET_EXPAND(satfractutq<mode>2,(operands[0],operands[1]));
  "
)

; Accum <-> float conversions appear as functions only

; Float support (isa32-target only)

(define_expand "add<mode>3"
  [(set (match_operand:FLOAT_MODES    0 "pic30_register_operand" "")
        (plus:FLOAT_MODES
           (match_operand:FLOAT_MODES 1 "pic30_register_operand" "")
           (match_operand:FLOAT_MODES 2 "pic30_register_operand" "")))]
  "pic30_isa32_target()"
  "
    emit(
      gen_add<mode>3_32(operands[0],operands[1],operands[2])
    );
    DONE;
  "
)

(define_expand "sub<mode>3"
  [(set (match_operand:FLOAT_MODES    0 "pic30_register_operand" "")
        (minus:FLOAT_MODES
           (match_operand:FLOAT_MODES 1 "pic30_register_operand" "")
           (match_operand:FLOAT_MODES 2 "pic30_register_operand" "")))]
  "pic30_isa32_target()"
  "
    emit(
      gen_sub<mode>3_32(operands[0],operands[1],operands[2])
    );
    DONE;
  "
)

(define_expand "div<mode>3"
  [(set (match_operand:FLOAT_MODES    0 "pic30_register_operand" "")
        (div:FLOAT_MODES
           (match_operand:FLOAT_MODES 1 "pic30_register_operand" "")
           (match_operand:FLOAT_MODES 2 "pic30_register_operand" "")))]
  "pic30_isa32_target()"
  " 
    emit(
      gen_div<mode>3_32(operands[0],operands[1],operands[2])
    );
    DONE;
  "
) 

(define_expand "mul<mode>3"
  [(set (match_operand:FLOAT_MODES    0 "pic30_register_operand" "")
        (mult:FLOAT_MODES
           (match_operand:FLOAT_MODES 1 "pic30_register_operand" "")
           (match_operand:FLOAT_MODES 2 "pic30_register_operand" "")))]
  "pic30_isa32_target()"
  " 
    emit(
      gen_mul<mode>3_32(operands[0],operands[1],operands[2])
    );
    DONE;
  "
) 

(define_expand "smin<mode>3"
  [(set (match_operand:FLOAT_MODES    0 "pic30_register_operand" "")
        (smin:FLOAT_MODES
           (match_operand:FLOAT_MODES 1 "pic30_register_operand" "")
           (match_operand:FLOAT_MODES 2 "pic30_register_operand" "")))]
  "pic30_isa32_target()"
  " 
    emit(
      gen_smin<mode>3_32(operands[0],operands[1],operands[2])
    );
    DONE;
  "
) 

(define_expand "smax<mode>3"
  [(set (match_operand:FLOAT_MODES    0 "pic30_register_operand" "")
        (smax:FLOAT_MODES
           (match_operand:FLOAT_MODES 1 "pic30_register_operand" "")
           (match_operand:FLOAT_MODES 2 "pic30_register_operand" "")))]
  "pic30_isa32_target()"
  " 
    emit(
      gen_smax<mode>3_32(operands[0],operands[1],operands[2])
    );
    DONE;
  "
) 


(define_expand "sqrt<mode>2"
  [(set (match_operand:FLOAT_MODES    0 "pic30_register_operand" "")
        (sqrt:FLOAT_MODES
           (match_operand:FLOAT_MODES 1 "pic30_register_operand" "")))]
  "pic30_isa32_target()"
  " 
    emit(
      gen_sqrt<mode>2_32(operands[0],operands[1])
    );
    DONE;
  "
) 

;; ;; hardware loops

;; doloop_end now only takes 2 operands
;;   further controls can be manipulated by TARGET_CAN_USE_DOLOOP_P (pg 634)
;;
(define_expand "doloop_end"
  [(use (match_operand 0 "" ""))        ; loop pseudo
   (use (match_operand 1 "" ""))]       ; label
  ""
  " 
    if (pic30_isa32_target()) {
      rtx insn = gen_doloop_end_32(operands[0], operands[1]);
      if (insn) {
        emit(insn);
        DONE;
      }
    } 
;    FAIL;
  "
)

;; mode extends

(define_expand "zero_extend<mode>p32umm2"
  [(set (match_operand:P32UMM 0 "pic30_register_operand"      "=r,r")
        (uextend:P32UMM
          (match_operand:RESTRICTED_INT_MODES 1 "pic30_reg_or_symbolic_address" " r,qs")))
  ]
  "TARGET_CLASSIC"
  "
    PIC30_CLASSIC_EXPAND(zero_extend<mode>p32umm2,
                         (operands[0],operands[1]));
  "
)

(define_expand "reload_load_address"
  [(set (match_operand:HI 0 "pic30_register_operand"  "=r")
        (match_operand:HI 1 "general_operand"         " o"))
  ]
  "(TARGET_CLASSIC)"
  "rtx lhs,rhs;
   rtx l,r;

   lhs = XEXP(operands[1],0);
   rhs = XEXP(operands[1],1);
   gcc_assert((GET_CODE(lhs) != PLUS) || (GET_CODE(rhs) != PLUS));
   if (GET_CODE(lhs) == PLUS) {
     l = XEXP(lhs,0);
     r = XEXP(lhs,1);
     if (!pic30_JMmath_operand(r,HImode)) {
       emit(
         gen_movhi_16(operands[0],r)
       );
       r = operands[0];
     }
     emit(
       gen_addhi3_16(operands[0],l,r)
     );
     lhs = operands[0];
   } else if (GET_CODE(rhs) == PLUS) {
     l = XEXP(rhs,0);
     r = XEXP(rhs,1);
     if (!pic30_JMmath_operand(r,HImode)) {
       emit(
         gen_movhi_16(operands[0],r)
       );
       r = operands[0];
     }
     emit(
       gen_addhi3_16(operands[0],l,r)
     );
     rhs = operands[0];
   } 
   emit(
     gen_addhi3_16(operands[0],lhs, rhs)
   );
   DONE;
  "
)

(define_expand "popcounthi2"
  [(set (match_operand:HI   0 "pic30_reg_or_R_operand"  "")
        (popcount:HI
          (match_operand:HI 1 "pic30_reg_or_R_operand"  "")))
  ]
  "(TARGET_CLASSIC)"
  "
   rtx op0 = operands[0];
   rtx op1 = operands[1];

   if (!pic30_reg_or_R_operand(op0,GET_MODE(op0))) {
     op0 = gen_reg_rtx(GET_MODE(op0));
   }
   if (!pic30_reg_or_R_operand(op1,GET_MODE(op1))) {
     op1 = force_reg(GET_MODE(op1),op1);
   }
   emit(
     gen_popcounthi2_16(op0,op1)
   );
   if (op0 != operands[0]) {
     emit_move_insn(operands[0],op0);
   }
   DONE;
  "
)

(define_expand "popcountsi2"
  [(set (match_operand:HI   0 "pic30_reg_or_R_operand"  "")
        (popcount:HI
          (match_operand:SI 1 "pic30_reg_or_R_operand"  "")))
  ]
  "(TARGET_CLASSIC)"
  "
   rtx op0 = operands[0];
   rtx op1 = operands[1];

   if (!pic30_reg_or_R_operand(op0,GET_MODE(op0))) {
     op0 = gen_reg_rtx(GET_MODE(op0));
   }
   if (!pic30_reg_or_R_operand(op1,GET_MODE(op1))) {
     op1 = force_reg(GET_MODE(op1),op1);
   }
   emit(
     gen_popcountsi2_16(op0,op1)
   );
   if (op0 != operands[0]) {
     emit_move_insn(operands[0],op0);
   }
   DONE;
  "
)


(include "pic30-classic.md")
(include "pic30-isa32.md")

(include "peephole.md")
