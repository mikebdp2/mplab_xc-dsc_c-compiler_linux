#ifndef __PIC30_OPC_H__
#define __PIC30_OPC_H__

/* pic30-opc.h -- This file contains defines and externs for the
 * opcodes/operands for Microchip dsPIC30
 * Copyright (C) 2000 Free Software Foundation, Inc.
 * Contributed by Microchip Corporation.
 * Written by Tracy A. Kuhrt
 * Modified by John Elliott (john.elliott@microchip.com)
 *
 * This file is part of GAS, the GNU Assembler.
 *
 * GAS is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * GAS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with GAS; see the file COPYING.  If not, write to the Free Software
 * Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

/******************************************************************************
 *
 * The following are mode flags for use with operands
 *
 ******************************************************************************/

#define NO_MODE  (0)
#define P_OR_Q   (1 << 0)
#define G_OR_H   (1 << 2)

#define MODE_MASK (0xF)

/******************************************************************************
 *
 * The following are mode values for insertion into and extraction from
 * instructions.
 *
 ******************************************************************************/

#define P_OR_Q_REGISTER_DIRECT            (0x0)
#define P_OR_Q_REGISTER_INDIRECT          (0x1)
#define P_OR_Q_REGISTER_POST_DECREMENT    (0x2)
#define P_OR_Q_REGISTER_POST_INCREMENT    (0x3)
#define P_OR_Q_REGISTER_PRE_DECREMENT     (0x4)
#define P_OR_Q_REGISTER_PRE_INCREMENT     (0x5)

#define G_OR_H_REGISTER_DIRECT            (0x0)
#define G_OR_H_REGISTER_INDIRECT          (0x1)
#define G_OR_H_REGISTER_POST_DECREMENT    (0x2)
#define G_OR_H_REGISTER_POST_INCREMENT    (0x3)
#define G_OR_H_REGISTER_PRE_DECREMENT     (0x4)
#define G_OR_H_REGISTER_PRE_INCREMENT     (0x5)
#define G_OR_H_REGISTER_WITH_DISPLACEMENT (0x6)
#define USR_REGISTER_MODE                 (0x6)
#define G_OR_H_REGISTER_WITH_DISPLACEMENT_32 (0x7)

/******************************************************************************
 *
 * The following are flags for the different type of operands that exist
 *
 ******************************************************************************/

#define OPND_UNKNOWN                             (1 << 4)
#define OPND_REGISTER_DIRECT                     (1 << 5)
#define OPND_REGISTER_INDIRECT                   (1 << 6)
#define OPND_REGISTER_POST_DECREMENT             (1 << 7)
#define OPND_REGISTER_POST_INCREMENT             (1 << 8)
#define OPND_REGISTER_PRE_DECREMENT              (1 << 9)
#define OPND_REGISTER_PRE_INCREMENT              (1 << 10)
#define OPND_REGISTER_WITH_DISPLACEMENT          (1 << 11)
#define OPND_REGISTER_PLUS_LITERAL               (1 << 12)
#define OPND_REGISTER_MINUS_LITERAL              (1 << 13)
#define OPND_REGISTER_POST_INCREMENT_BY_N        (1 << 14)
#define OPND_REGISTER_POST_DECREMENT_BY_N        (1 << 15)
#define OPND_EXPRESSION                          (1 << 16)
#define OPND_SYMBOL                              (1 << 17)
#define OPND_CONSTANT                            (1 << 18)
#define OPND_W_REG                               (1 << 19)
#define OPND_BRANCH_ON_DSP_CONDITION             (1 << 20)
#define OPND_BRANCH_ON_CONDITION                 (1 << 21)
#define OPND_DSP_ACCUMULATOR_SELECT              (1 << 22)
#define OPND_DSP_MULTIPLICAND                    (1 << 23)
#define OPND_DSP_SQUARE                          (1 << 24)
#define OPND_PAGE_SYMBOL                         (1 << 25)
#define OPND_W_REGISTER_DIRECT                   (1 << 26)
#define OPND_REGISTER_PRE_INCREMENT_BY_N         (1 << 27)
#define OPND_REGISTER_PRE_DECREMENT_BY_N         (1 << 28)
#define OPND_ANY                                ((1 << 29) - 1)

#define OPND_VALUE (OPND_EXPRESSION | OPND_SYMBOL | OPND_CONSTANT)

/******************************************************************************
 *
 * The following are combination flags for modes P or Q.
 *
 ******************************************************************************/

#define OPND_P_OR_Q_REGISTER_DIRECT            (P_OR_Q | \
                                                OPND_REGISTER_DIRECT)
#define OPND_P_OR_Q_REGISTER_INDIRECT          (P_OR_Q | \
                                                OPND_REGISTER_INDIRECT)
#define OPND_P_OR_Q_REGISTER_POST_DECREMENT    (P_OR_Q | \
                                                OPND_REGISTER_POST_DECREMENT)
#define OPND_P_OR_Q_REGISTER_POST_INCREMENT    (P_OR_Q | \
                                                OPND_REGISTER_POST_INCREMENT)
#define OPND_P_OR_Q_REGISTER_PRE_DECREMENT     (P_OR_Q | \
                                                OPND_REGISTER_PRE_DECREMENT)
#define OPND_P_OR_Q_REGISTER_PRE_INCREMENT     (P_OR_Q | \
                                                OPND_REGISTER_PRE_INCREMENT)

#define OPND_P_OR_Q_IND ( \
                     OPND_P_OR_Q_REGISTER_INDIRECT       | \
                     OPND_P_OR_Q_REGISTER_POST_DECREMENT | \
                     OPND_P_OR_Q_REGISTER_POST_INCREMENT | \
                     OPND_P_OR_Q_REGISTER_PRE_DECREMENT  | \
                     OPND_P_OR_Q_REGISTER_PRE_INCREMENT)

#define OPND_P_OR_Q (OPND_P_OR_Q_REGISTER_DIRECT         | \
                     OPND_P_OR_Q_IND)

#define OPND_P_OR_Q_INC_IND ( \
                     OPND_P_OR_Q_REGISTER_POST_INCREMENT | \
                     OPND_P_OR_Q_REGISTER_PRE_INCREMENT)

#define OPND_P_OR_Q_BIT_REV_SRC ( \
                     OPND_P_OR_Q_REGISTER_DIRECT | \
                     OPND_P_OR_Q_REGISTER_INDIRECT | \
                     OPND_P_OR_Q_INC_IND)

#define OPND_P_OR_Q_BIT_REV_DST ( \
                     OPND_P_OR_Q_INC_IND)

/******************************************************************************
 *
 * The following are combination flags for modes G or H.
 *
 ******************************************************************************/

#define OPND_G_OR_H_REGISTER_DIRECT            (G_OR_H | \
                                                OPND_REGISTER_DIRECT)
#define OPND_G_OR_H_REGISTER_INDIRECT          (G_OR_H | \
                                                OPND_REGISTER_INDIRECT)
#define OPND_G_OR_H_REGISTER_POST_DECREMENT    (G_OR_H | \
                                                OPND_REGISTER_POST_DECREMENT)
#define OPND_G_OR_H_REGISTER_POST_INCREMENT    (G_OR_H | \
                                                OPND_REGISTER_POST_INCREMENT)
#define OPND_G_OR_H_REGISTER_PRE_DECREMENT     (G_OR_H | \
                                                OPND_REGISTER_PRE_DECREMENT)
#define OPND_G_OR_H_REGISTER_PRE_INCREMENT     (G_OR_H | \
                                                OPND_REGISTER_PRE_INCREMENT)
#define OPND_G_OR_H_REGISTER_WITH_DISPLACEMENT (G_OR_H | \
                                                OPND_REGISTER_WITH_DISPLACEMENT)

#define OPND_G_OR_H (OPND_G_OR_H_REGISTER_DIRECT             | \
                     OPND_G_OR_H_REGISTER_INDIRECT           | \
                     OPND_G_OR_H_REGISTER_POST_DECREMENT     | \
                     OPND_G_OR_H_REGISTER_POST_INCREMENT     | \
                     OPND_G_OR_H_REGISTER_PRE_DECREMENT      | \
                     OPND_G_OR_H_REGISTER_PRE_INCREMENT      | \
                     OPND_G_OR_H_REGISTER_WITH_DISPLACEMENT)

#define OPND_G_OR_H_NOT_DIRECT ( \
                     OPND_G_OR_H_REGISTER_INDIRECT           | \
                     OPND_G_OR_H_REGISTER_POST_DECREMENT     | \
                     OPND_G_OR_H_REGISTER_POST_INCREMENT     | \
                     OPND_G_OR_H_REGISTER_PRE_DECREMENT      | \
                     OPND_G_OR_H_REGISTER_PRE_INCREMENT      | \
                     OPND_G_OR_H_REGISTER_WITH_DISPLACEMENT)

/******************************************************************************
 *
 * The following combination flag is for the literal offset MOV instructions.
 *
 ******************************************************************************/
#define OPND_LO_REGISTER (OPND_REGISTER_PLUS_LITERAL   | \
                          OPND_REGISTER_MINUS_LITERAL)

/******************************************************************************
 *
 * The following are combination flags for DSP instructions.
 *
 ******************************************************************************/
#define OPND_DSP_PRESHIFT            (OPND_VALUE)
#define OPND_DSP_PREFETCH_OPERATION  (OPND_REGISTER_INDIRECT            | \
                                      OPND_REGISTER_POST_INCREMENT_BY_N | \
                                      OPND_REGISTER_POST_DECREMENT_BY_N | \
                                      OPND_REGISTER_WITH_DISPLACEMENT)
#define OPND_DSP_FETCH_OPERATION_32  (OPND_REGISTER_DIRECT              | \
                                      OPND_REGISTER_INDIRECT            | \
                                      OPND_REGISTER_POST_INCREMENT_BY_N | \
                                      OPND_REGISTER_POST_DECREMENT_BY_N | \
                                      OPND_REGISTER_PRE_INCREMENT_BY_N  | \
                                      OPND_REGISTER_PRE_DECREMENT_BY_N  | \
                                      OPND_REGISTER_WITH_DISPLACEMENT)
#define OPND_DSP_WRITEBACK           (OPND_REGISTER_DIRECT              | \
                                      OPND_REGISTER_POST_INCREMENT_BY_N)
#define OPND_DSP_WRITEBACK_32        (OPND_REGISTER_DIRECT              | \
                                      OPND_REGISTER_POST_INCREMENT)

/******************************************************************************
 *
 * Opcode flags
 *
 ******************************************************************************/
/*
 *  A word about flags...
 *     (flags & (F_ECORE | F_FCORE | F_ISAV4 | F_ISA32V0)) == 0 implies valid on
 *        all isas
 *     F_ISAV4 only valid if HAS_ISAV4
 *     F_ISA32V0 only valid if HAS_ISA32V0
 *     F_FCORE only valid if not ecore isav5 or isav4 device
 *     clear? (as mud?)
 */
#define F_NONE                       (0)
#define F_WORD                       (1 << 0)
#define F_HAS_REQUIRED_WREG          (1 << 1)
#define F_HAS_IMPLIED_WREG           (1 << 2)
#define F_HAS_WREG                   (F_HAS_REQUIRED_WREG | F_HAS_IMPLIED_WREG)
#define F_HAS_BRANCH_FLAG            (1 << 3)
#define F_IS_DSP_INSN                (1 << 4)
#define F_CANNOT_FOLLOW_REPEAT       (1 << 5)
#define F_IS_2_WORD_INSN             (1 << 6)
#define F_ECORE                      (1 << 7)
#define F_FCORE                      (1 << 8)
#define F_DUALPARTITION              (1 << 9)
#define F_ISAV4                      (1 << 10) 
#define F_CONTEXTS                   (1 << 11)
#define F_ISA32V0                    (1 << 12)  /* ISA V5 ONLY */
#define F_ISA32V016                  (1 << 13)  /* ISA V5, 16bit insn */
#define F_PCREL_INSN                 (1 << 14) 
#define F_GENERATED                  (1 << 15)  /* A generated mnemonic,
                                                 * perhaps for compatibility or
                                                 * other reason... do not decode
                                                 * to this instruction */
#define F_DEPRECATED                 (1 << 16)  /* deprecated */
#define F_HAS_F_BRANCH_FLAG          (1 << 17)  /* A floating point branch */
#define F_IS_DSP2_INSN               (1 << 18)  /* DSP insn, with different
                                                     displacement offset */
#define F_COMPATIBILITY              (1 << 29)  /* supported in ISA32V0 */
#define F_NOTIMPLEMENTED             (1 << 30)
#define F_NOTISAB                    (1UL << 31)  /* tmp - not supported on isab*/

/******************************************************************************
 *
 * The following defines give clear names to the machine dependent operatorT's.
 *
 ******************************************************************************/

#define O_REGISTER_INDIRECT                  (operatorT)O_md1
#define O_REGISTER_POST_DECREMENT            (operatorT)O_md2
#define O_REGISTER_POST_INCREMENT            (operatorT)O_md3
#define O_REGISTER_PRE_DECREMENT             (operatorT)O_md4
#define O_REGISTER_PRE_INCREMENT             (operatorT)O_md5
#define O_REGISTER_WITH_DISPLACEMENT         (operatorT)O_md6
#define O_REGISTER_PLUS_LITERAL              (operatorT)O_md7
#define O_REGISTER_MINUS_LITERAL             (operatorT)O_md8
#define O_REGISTER_POST_INCREMENT_BY_N       (operatorT)O_md9
#define O_REGISTER_POST_DECREMENT_BY_N       (operatorT)O_md10
#define O_W_REG                              (operatorT)O_md11
#define O_BRANCH_ON_DSP_CONDITION            (operatorT)O_md12
#define O_BRANCH_ON_CONDITION                (operatorT)O_md13
#define O_DSP_ACCUMULATOR_SELECT             (operatorT)O_md14
#define O_DSP_MULTIPLICAND                   (operatorT)O_md15
#define O_DSP_SQUARE                         (operatorT)O_md16
#define O_REGISTER_PRE_INCREMENT_BY_N        (operatorT)O_md17
#define O_REGISTER_PRE_DECREMENT_BY_N        (operatorT)O_md18

/******************************************************************************
 *
 * The following defines allow us to set whether the destination of a file
 * register instruction is the working register or the file register.
 *
 ******************************************************************************/

#define PIC30_DESTINATION_WORKING_REGISTER 0

/******************************************************************************
 *
 * The following defines allow us to set the condition code portion of the
 * branch instruction.
 *
 ******************************************************************************/

#define PIC30_BRANCH_FLAG_OPERAND_POSITION 1

#define PIC30_BRANCH_ON_OVERFLOW                  (0x0)
#define PIC30_BRANCH_ON_CARRY                     (0x1)
#define PIC30_BRANCH_ON_GREATER_OR_EQUAL_UNSIGNED (0x1)
#define PIC30_BRANCH_ON_ZERO                      (0x2)
#define PIC30_BRANCH_ON_NEGATIVE                  (0x3)
#define PIC30_BRANCH_ON_LESS_OR_EQUAL             (0x4)
#define PIC30_BRANCH_ON_LESS_THAN                 (0x5)
#define PIC30_BRANCH_ON_LESS_OR_EQUAL_UNSIGNED    (0x6)
#define PIC30_BRANCH_ON_NOT_OVERFLOW              (0x8)
#define PIC30_BRANCH_ON_NOT_CARRY                 (0x9)
#define PIC30_BRANCH_ON_LESS_THAN_UNSIGNED        (0x9)
#define PIC30_BRANCH_ON_NOT_ZERO                  (0xA)
#define PIC30_BRANCH_ON_NOT_NEGATIVE              (0xB)
#define PIC30_BRANCH_ON_GREATER_THAN              (0xC)
#define PIC30_BRANCH_ON_GREATER_OR_EQUAL          (0xD)
#define PIC30_BRANCH_ON_GREATER_THAN_UNSIGNED     (0xE)

#define PIC30_BRANCH_ON_ACCUMULATOR_A_OVERFLOW    (0x0)
#define PIC30_BRANCH_ON_ACCUMULATOR_B_OVERFLOW    (0x1)
#define PIC30_BRANCH_ON_ACCUMULATOR_A_SATURATION  (0x2)
#define PIC30_BRANCH_ON_ACCUMULATOR_B_SATURATION  (0x3)

#define PIC30_F_BRANCH_ON_EQ                      (0x0)
#define PIC30_F_BRANCH_ON_GE                      (0x1)
#define PIC30_F_BRANCH_ON_NE                      (0x2)
#define PIC30_F_BRANCH_ON_GT                      (0x3)
#define PIC30_F_BRANCH_ON_LE                      (0x4)
#define PIC30_F_BRANCH_ON_LT                      (0x5)
#define PIC30_F_BRANCH_ON_UN                      (0x6)
#define PIC30_F_BRANCH_ON_UNE                     (0x7)
#define PIC30_F_BRANCH_ON_UEQ                     (0x8)
#define PIC30_F_BRANCH_ON_ULE                     (0x9)
#define PIC30_F_BRANCH_ON_ULT                     (0xA)
#define PIC30_F_BRANCH_ON_UGE                     (0xB)
#define PIC30_F_BRANCH_ON_UGT                     (0xC)
#define PIC30_F_BRANCH_ON_OR                      (0xD)

#define PIC30_MAX_CONDITION_CODE_LENGTH 3

/******************************************************************************
 *
 * The following defines allow us to set the accumulator select bit of the
 * DSP instructions.
 *
 ******************************************************************************/

#define PIC30_DSP_ACCUMULATOR_A 0
#define PIC30_DSP_ACCUMULATOR_B 1

#define PIC30_DSP_ACCUMULATOR_A_STRING "A"
#define PIC30_DSP_ACCUMULATOR_B_STRING "B"

#define PIC30_MAX_DSP_ACCUMULATOR_LENGTH 1

/******************************************************************************
 *
 * The following defines allow us to set the writeback bits of the
 * DSP instructions.
 *
 ******************************************************************************/

#define PIC30_DSP_WRITEBACK_DIRECT              0
#define PIC30_DSP_WRITEBACK_POST_INCREMENT_BY_N 1

#define PIC30_DSP_WRITEBACK_REGISTER_NUMBER (13)
#define PIC30_MAX_DSP_WRITEBACK_LENGTH      (8)

/******************************************************************************
 *
 * The following defines allow us to set the prefetch bits of the DSP
 * instructions.
 *
 ******************************************************************************/
#define PIC30_MAX_REGISTER_LENGTH 3

#define PIC30_MIN_DSP_PREFETCH_DST_REGISTER_VALUE 4
#define PIC30_MAX_DSP_PREFETCH_DST_REGISTER_VALUE 7

#define PIC30_MAX_DSP_X_PREFETCH_OPERATION_LENGTH 9
#define PIC30_MAX_DSP_Y_PREFETCH_OPERATION_LENGTH 9

#define PIC30_X_PREFETCH_MAC1_REGISTER_VALUE 8
#define PIC30_X_PREFETCH_MAC2_REGISTER_VALUE 9

#define PIC30_X_PREFETCH_MAC1_REGISTER "w8"
#define PIC30_X_PREFETCH_MAC2_REGISTER "w9"

#define PIC30_Y_PREFETCH_MAC1_REGISTER_VALUE 10
#define PIC30_Y_PREFETCH_MAC2_REGISTER_VALUE 11

#define PIC30_Y_PREFETCH_MAC1_REGISTER "w10"
#define PIC30_Y_PREFETCH_MAC2_REGISTER "w11"

#define PIC30_DSP_PREFETCH_DISPLACEMENT_REGISTER_VALUE 12
#define PIC30_DSP_PREFETCH_DISPLACEMENT_REGISTER "w12"

#define PIC30_DSP_X_PREFETCH_OPERATION_MAC1_INDIRECT 0x0
#define PIC30_DSP_X_PREFETCH_OPERATION_MAC1_PLUS2    0x1
#define PIC30_DSP_X_PREFETCH_OPERATION_MAC1_PLUS4    0x2
#define PIC30_DSP_X_PREFETCH_OPERATION_MAC1_PLUS6    0x3
#define PIC30_DSP_X_PREFETCH_OPERATION_NO_PREFETCH   0x4
#define PIC30_DSP_X_PREFETCH_OPERATION_MAC1_MINUS2   0x7
#define PIC30_DSP_X_PREFETCH_OPERATION_MAC1_MINUS4   0x6
#define PIC30_DSP_X_PREFETCH_OPERATION_MAC1_MINUS6   0x5
#define PIC30_DSP_X_PREFETCH_OPERATION_MAC2_INDIRECT 0x8
#define PIC30_DSP_X_PREFETCH_OPERATION_MAC2_PLUS2    0x9
#define PIC30_DSP_X_PREFETCH_OPERATION_MAC2_PLUS4    0xA
#define PIC30_DSP_X_PREFETCH_OPERATION_MAC2_PLUS6    0xB
#define PIC30_DSP_X_PREFETCH_OPERATION_MAC2_PLUS_REG 0xC
#define PIC30_DSP_X_PREFETCH_OPERATION_MAC2_MINUS6   0xD
#define PIC30_DSP_X_PREFETCH_OPERATION_MAC2_MINUS4   0xE
#define PIC30_DSP_X_PREFETCH_OPERATION_MAC2_MINUS2   0xF

#define PIC30_DSP_Y_PREFETCH_OPERATION_MAC1_INDIRECT 0x0
#define PIC30_DSP_Y_PREFETCH_OPERATION_MAC1_PLUS2    0x1
#define PIC30_DSP_Y_PREFETCH_OPERATION_MAC1_PLUS4    0x2
#define PIC30_DSP_Y_PREFETCH_OPERATION_MAC1_PLUS6    0x3
#define PIC30_DSP_Y_PREFETCH_OPERATION_NO_PREFETCH   0x4
#define PIC30_DSP_Y_PREFETCH_OPERATION_MAC1_MINUS2   0x7
#define PIC30_DSP_Y_PREFETCH_OPERATION_MAC1_MINUS4   0x6
#define PIC30_DSP_Y_PREFETCH_OPERATION_MAC1_MINUS6   0x5
#define PIC30_DSP_Y_PREFETCH_OPERATION_MAC2_INDIRECT 0x8
#define PIC30_DSP_Y_PREFETCH_OPERATION_MAC2_PLUS2    0x9
#define PIC30_DSP_Y_PREFETCH_OPERATION_MAC2_PLUS4    0xA
#define PIC30_DSP_Y_PREFETCH_OPERATION_MAC2_PLUS6    0xB
#define PIC30_DSP_Y_PREFETCH_OPERATION_MAC2_PLUS_REG 0xC
#define PIC30_DSP_Y_PREFETCH_OPERATION_MAC2_MINUS6   0xD
#define PIC30_DSP_Y_PREFETCH_OPERATION_MAC2_MINUS4   0xE
#define PIC30_DSP_Y_PREFETCH_OPERATION_MAC2_MINUS2   0xF

/* ISA32 versions */
/* encoding of address mode */
#define PIC30_DSP_FETCH_OPERATION_DIRECT      0x0
#define PIC30_DSP_FETCH_OPERATION_INDIRECT    0x1
#define PIC30_DSP_FETCH_OPERATION_INDIRECT_PD 0x2
#define PIC30_DSP_FETCH_OPERATION_INDIRECT_PI 0x3
#define PIC30_DSP_FETCH_OPERATION_PD_INDIRECT 0x4
#define PIC30_DSP_FETCH_OPERATION_PI_INDIRECT 0x5
#define PIC30_DSP_FETCH_OPERATION_NONE        0x6
#define PIC30_DSP_FETCH_OPERATION_IND_OFFSET  0x7

/* insn offset of modifier/register value */
#define PIC30_DSP_X_MODIFIER                  11
#define PIC30_DSP_X_REGISTER                  16
#define PIC30_DSP_Y_MODIFIER                  6
#define PIC30_DSP_Y_REGISTER                  20

/* insn writeback */
#define PIC30_DSP_WRITEBACK_W0                0x0
#define PIC30_DSP_WRITEBACK_W1                0x1
#define PIC30_DSP_WRITEBACK_W2                0x2
#define PIC30_DSP_WRITEBACK_W3                0x3
#define PIC30_DSP_WRITEBACK_W13               0x4
#define PIC30_DSP_WRITEBACK_W13_PI            0x5
#define PIC30_DSP_WRITEBACK_W15_PI            0x6
#define PIC30_DSP_WRITE_BACK_NONE             0x7

extern const char *pic30_dsp_writeback_decode[];

/******************************************************************************
 *
 * The following defines allow us to set the multiplicand bits of the DSP
 * instructions.
 *
 ******************************************************************************/
#define PIC30_MAX_DSP_MULTIPLICAND_LENGTH 7

#define PIC30_DSP_W4_W5_MULTIPLICAND 0x0
#define PIC30_DSP_W4_W6_MULTIPLICAND 0x1
#define PIC30_DSP_W4_W7_MULTIPLICAND 0x2
#define PIC30_DSP_W5_W6_MULTIPLICAND 0x4
#define PIC30_DSP_W5_W7_MULTIPLICAND 0x5
#define PIC30_DSP_W6_W7_MULTIPLICAND 0x6

#define PIC30_DSP_W4_W5_MULTIPLICAND_STRING "w4 * w5"
#define PIC30_DSP_W4_W6_MULTIPLICAND_STRING "w4 * w6"
#define PIC30_DSP_W4_W7_MULTIPLICAND_STRING "w4 * w7"
#define PIC30_DSP_W5_W6_MULTIPLICAND_STRING "w5 * w6"
#define PIC30_DSP_W5_W7_MULTIPLICAND_STRING "w5 * w7"
#define PIC30_DSP_W6_W7_MULTIPLICAND_STRING "w6 * w7"

/******************************************************************************/

#define PIC30_SIZE_OF_INSN_MASK 0x80000000UL
#define PIC30_MAX_SIZE_OF_PROGRAM_WORD 4
#define PIC30_SIZE_OF_PROGRAM_WORD (pic30_is_isav32_machine(global_PROCESSOR) ? 2 : 4)
#define PIC30_SIZE_OF_DATA_WORD 2

#define PIC30_CALL_SECOND_PROGRAM_WORD 0x000000

#if 0
#define PIC30_ADD_OPERAND(insn, op_val, bits, shift) \
   ((insn) | ((op_val) & ((1 << (bits)) - 1)) << (shift))
#else
extern unsigned int PIC30_ADD_OPERAND(unsigned long*, unsigned int value,
                                      unsigned int bits, unsigned int shift);
#endif

#if 0
#define PIC30_EXTRACT_OPERAND(insn, bits, shift) \
  (((insn) >> (shift)) & ((1 << (bits)) - 1))
#else
extern unsigned int PIC30_EXTRACT_OPERAND(unsigned long *,
                                          unsigned int, unsigned int);
#endif

extern unsigned int pic30_extract_operand(unsigned long *, 
                                          struct pic30_operand *opnd);
extern unsigned int pic30_add_operand(unsigned long *insn , unsigned int value,
                                      struct pic30_operand *opnd);

#define PIC30_BASE_OPERAND (0)

#define BASE_10_STRING_LENGTH 10

#define PIC30_ADDRESSING_MODE_BITS 3
#define PIC30_SRC_MODE_SHIFT 4
#define PIC30_DST_MODE_SHIFT 11

#define PIC30_ADDRESSING_MODE_BITS_32 3
#define PIC30_SRC_MODE_SHIFT_32 13
#define PIC30_DST_MODE_SHIFT_32 10
#define PIC30_DST_MODE_SHIFT_64 42

/* moved from pic30-opc.c pic30_insert_g_or_h_displacement_value */
#define DISPLACEMENT_BITS             4
#define EXTENDED_DISPLACEMENT_BITS    1
#define DSP_DISPLACEMENT_SHIFT       11
#define DISPLACEMENT_SHIFT           15
#define ISA32_DISPLACEMENT_SHIFT      6
#define ISA32_DSP_DISPLACEMENT_SHIFT 20

#define PIC30_W_REGISTER_STRING "WREG"

extern const struct pic30_operand pic30_operands[];
extern const int pic30_num_operands;
extern const struct pic30_opcode pic30_opcodes[];
extern const int pic30_num_opcodes;
extern const unsigned long pic30_operator_mask[];
extern const struct relocation_info pic30_relocation_info[];
extern const char * pic30_registers[];
extern int pic30_num_registers;
extern int pic30_first_float_register;
extern int pic30_usr_register;
extern int pic30_fsr_register;
extern int pic30_fcr_register;
extern int pic30_fear_register;
extern int pic30_fsrh_register;
#if 0
extern const struct pic30_processor pic30_processors[];
#endif
extern const int pic30_num_processors;

#if 0
/* CAW - not used */
struct pic30_predefined_symbol
{
   const char * name;
   long value;
};

extern const struct pic30_predefined_symbol pic30_sfr_symbols[];
extern const int pic30_num_sfr_symbols;
#endif

/******************************************************************************/

extern const struct relocation_info pic30_get_pc_relative_relocation_info(void);

/******************************************************************************/

#define CALL_INSN     0x020000UL
#define RCALL_INSN    0x070000UL
#define GOTO_INSN     0x040000UL
#define BRA_DSP_INSN  0x0C0000UL
#define BRA_CC_INSN   0x300000UL
#define BRA_INSN      0x370000UL
#define DO_INSN       0x080000UL
#define DOW_INSN      0x088000UL
#define REPEAT_INSN   0x090000UL
#define REPEATW_INSN  0x098000UL
#define CPWBEQ_W_INSN 0xE78000UL
#define CPWBEQ_B_INSN 0xE78400UL
#define CPWBGT_W_INSN 0xE60000UL
#define CPWBGT_B_INSN 0xE60400UL
#define CPWBLT_W_INSN 0xE68000UL
#define CPWBLT_B_INSN 0xE68400UL
#define CPWBNE_W_INSN 0xE70000UL
#define CPWBNE_B_INSN 0xE70400UL
#define BFINS_INSN    0x0A0000UL
#define BFINSF_INSN   0x0A2000UL
#define BFINSL_INSN   0x0A4000UL
#define BFEXT_INSN    0x0A8000UL
#define BFEXTF_INSN   0x0AA000UL
#define DIVSW2_INSN   0xD80020UL
#define DIVUW2_INSN   0xD88020UL
#define DIVXW2_MASK   0xFF8060UL

#define ISAV32_REPEATLS_INSN 0x720FULL
#define ISAV32_REPEAT_INSN   0xD4000006ULL
#define ISAV32_REPEATW_INSN  0xD8000002ULL

/******************************************************************************/

/* all new ops are:
**    OP_<op>16_<selection> for 16-bit opcodes
**    OP_<op>32_<selection> for 32-bit opcodes
**
** None of the original opcodes carry over, they are duplicated
**
*/

/*
**      baseop           bytes per IW
**      |                |  number_of_words
**      |                |  |  opcode                           mask
**      |                |  |  |                                |
**      |                |  |  |                                |
**      v                v  v  v                                v
*                                                                             */
#define DUMMY \
        OP_DUMMY,        3, 0, { 0xFFFFFFUL },                  0xFFFFFFUL

#define ADDWFF_B \
        OP_ADDWFF_B,     3, 1, { 0xB46000UL },                  0xFFE000UL
#define ADDWFW_B \
        OP_ADDWFW_B,     3, 1, { 0xB44000UL },                  0xFFE000UL
#define ADDLW_B \
        OP_ADDLW_B,      3, 1, { 0xB04000UL },                  0xFFC000UL
#define ADDLS_B \
        OP_ADDLS_B,      3, 1, { 0x404060UL },                  0xF84060UL
#define ADD_B \
        OP_ADD_B,        3, 1, { 0x404000UL },                  0xF84000UL

#define ADDWFF_W \
        OP_ADDWFF_W,     3, 1, { 0xB42000UL },                  0xFFE000UL
#define ADDWFW_W \
        OP_ADDWFW_W,     3, 1, { 0xB40000UL },                  0xFFE000UL
#define ADDLW_W \
        OP_ADDLW_W,      3, 1, { 0xB00000UL },                  0xFFC000UL
#define ADDLS_W \
        OP_ADDLS_W,      3, 1, { 0x400060UL },                  0xF84060UL
#define ADD_W \
        OP_ADD_W,        3, 1, { 0x400000UL },                  0xF84000UL
#define ADDAB \
        OP_ADDAB,        3, 1, { 0xCB0000UL },                  0xFF7000UL
#define ADDAC_PS \
        OP_ADDAC_PS,     3, 1, { 0xC90000UL },                  0xFF0000UL
#define ADDAC \
        OP_ADDAC,        3, 1, { 0xC90000UL },                  0xFF0780UL

#define ADDCWFF_B \
        OP_ADDCWFF_B,    3, 1, { 0xB4E000UL },                  0xFFE000UL
#define ADDCWFW_B \
        OP_ADDCWFW_B,    3, 1, { 0xB4C000UL },                  0xFFE000UL
#define ADDCLW_B \
        OP_ADDCLW_B,     3, 1, { 0xB0C000UL },                  0xFFC000UL
#define ADDCLS_B \
        OP_ADDCLS_B,     3, 1, { 0x484060UL },                  0xF84060UL
#define ADDC_B \
        OP_ADDC_B,       3, 1, { 0x484000UL },                  0xF84000UL

#define ADDCWFF_W \
        OP_ADDCWFF_W,    3, 1, { 0xB4A000UL },                  0xFFE000UL
#define ADDCWFW_W \
        OP_ADDCWFW_W,    3, 1, { 0xB48000UL },                  0xFFE000UL
#define ADDCLW_W \
        OP_ADDCLW_W,     3, 1, { 0xB08000UL },                  0xFFC000UL
#define ADDCLS_W \
        OP_ADDCLS_W,     3, 1, { 0x480060UL },                  0xF84060UL
#define ADDC_W \
        OP_ADDC_W,       3, 1, { 0x480000UL },                  0xF84000UL

#define ANDWFF_B \
        OP_ANDWFF_B,     3, 1, { 0xB66000UL },                  0xFFE000UL
#define ANDWFW_B \
        OP_ANDWFW_B,     3, 1, { 0xB64000UL },                  0xFFE000UL
#define ANDLW_B \
        OP_ANDLW_B,      3, 1, { 0xB24000UL },                  0xFFC000UL
#define ANDLS_B \
        OP_ANDLS_B,      3, 1, { 0x604060UL },                  0xF84060UL
#define AND_B \
        OP_AND_B,        3, 1, { 0x604000UL },                  0xF84000UL

#define ANDWFF_W \
        OP_ANDWFF_W,     3, 1, { 0xB62000UL },                  0xFFE000UL
#define ANDWFW_W \
        OP_ANDWFW_W,     3, 1, { 0xB60000UL },                  0xFFE000UL
#define ANDLW_W \
        OP_ANDLW_W,      3, 1, { 0xB20000UL },                  0xFFC000UL
#define ANDLS_W \
        OP_ANDLS_W,      3, 1, { 0x600060UL },                  0xF84060UL
#define AND_W \
        OP_AND_W,        3, 1, { 0x600000UL },                  0xF84000UL

#define ASRFF_B \
        OP_ASRFF_B,      3, 1, { 0xD5E000UL },                  0xFFE000UL
#define ASRFW_B \
        OP_ASRFW_B,      3, 1, { 0xD5C000UL },                  0xFFE000UL
#define ASR_B \
        OP_ASR_B,        3, 1, { 0xD1C000UL },                  0xFFC000UL

#define ASRFF_W \
        OP_ASRFF_W,      3, 1, { 0xD5A000UL },                  0xFFE000UL
#define ASRFW_W \
        OP_ASRFW_W,      3, 1, { 0xD58000UL },                  0xFFE000UL
#define ASR_W \
        OP_ASR_W,        3, 1, { 0xD18000UL },                  0xFFC000UL
#define ASRW_W \
        OP_ASRW_W,       3, 1, { 0xDE8000UL },                  0xFF8070UL
#define ASRK_W \
        OP_ASRK_W,       3, 1, { 0xDE8040UL },                  0xFF8070UL

#define BCLR_W \
        OP_BCLR_W,       3, 1, { 0xA10000UL },                  0xFF0F80UL
#define BCLRF_W \
        OP_BCLRF_B,      3, 1, { 0xA90000UL },                  0x000000UL
#define BCLRF_B \
        OP_BCLRF_B,      3, 1, { 0xA90000UL },                  0xFF0000UL
#define BCLR_B \
        OP_BCLR_B,       3, 1, { 0xA10400UL },                  0xFF0F80UL

#define BFCLR \
        OP_BFCLR,        3, 1, { 0x0B8000UL },                  0xFF8000UL

#define BFEXT \
        OP_BFEXT,        3, 2, { BFEXT_INSN, 0x000000UL },    0x00FFE000UL
#define BFEXTF \
        OP_BFEXTF,       3, 2, { BFEXTF_INSN, 0x000000UL },   0x00FFE000UL

#define BFINS \
        OP_BFINS,        3, 2, { BFINS_INSN, 0x000000UL },    0x00FFE000UL
#define BFINSF \
        OP_BFINSF,       3, 2, { BFINSF_INSN, 0x000000UL },   0x00FFE000UL
#define BFINSL \
        OP_BFINSL,       3, 2, { BFINSL_INSN, 0x000000UL },   0x00FFE000UL

#define BFSET \
        OP_BFSET,        3, 1, { 0x0B0000UL },                  0xFF8000UL

#define BOOTSWP \
        OP_BOOTSWP,      3, 1, { 0xFE2000UL },                  0xFFFFFFUL

#define BRA_DSP \
        OP_BRA_DSP,      3, 1, { BRA_DSP_INSN },                  0xFC0000UL
#define BRA \
        OP_BRA,          3, 1, { BRA_INSN },                    0xFF0000UL
#define BRA_CC \
        OP_BRA_CC,       3, 1, { BRA_CC_INSN },                 0xF00000UL
#define BRAW \
        OP_BRAW,         3, 1, { 0x016000UL },                  0xFFFFF0UL
#define BRAWE \
        OP_BRAWE,        3, 1, { 0x010600UL },                  0xFF0600UL

#define BREAK \
        OP_BREAK,        3, 1, { 0xDA4000UL },                  0xFFC000UL

#define BSET_W \
        OP_BSET_W,       3, 1, { 0xA00000UL },                  0xFF0F80UL
#define BSETF_W \
        OP_BSETF_B,      3, 1, { 0xA80000UL },                  0x000000UL
#define BSETF_B \
        OP_BSETF_B,      3, 1, { 0xA80000UL },                  0xFF0000UL
#define BSET_B \
        OP_BSET_B,       3, 1, { 0xA00400UL },                  0xFF8F80UL

#define BSW_Z \
        OP_BSW_Z,        3, 1, { 0xAD8000UL },                  0xFF8780UL
#define BSW_C \
        OP_BSW_C,        3, 1, { 0xAD0000UL },                  0xFF8780UL

#define BTG_W \
        OP_BTG_W,        3, 1, { 0xA20000UL },                  0xFF0400UL
#define BTGF_W \
        OP_BTGF_B,       3, 1, { 0xAA0000UL },                  0x000000UL
#define BTGF_B \
        OP_BTGF_B,       3, 1, { 0xAA0000UL },                  0xFF0000UL
#define BTG_B \
        OP_BTG_B,        3, 1, { 0xA20400UL },                  0xFF8400UL

#define BTSC_W \
        OP_BTSC_W,       3, 1, { 0xA70000UL },                  0xFF0F80UL
#define BTSCF_W \
        OP_BTSCF_B,      3, 1, { 0xAF0000UL },                  0x000000UL
#define BTSCF_B \
        OP_BTSCF_B,      3, 1, { 0xAF0000UL },                  0xFF0000UL
#define BTSC_B \
        OP_BTSC_B,       3, 1, { 0xA70000UL },                  0xFF8F80UL

#define BTSS_W \
        OP_BTSS_W,       3, 1, { 0xA60000UL },                  0xFF0F80UL
#define BTSSF_W \
        OP_BTSSF_B,      3, 1, { 0xAE0000UL },                  0x000000UL
#define BTSSF_B \
        OP_BTSSF_B,      3, 1, { 0xAE0000UL },                  0xFF0000UL
#define BTSS_B \
        OP_BTSS_B,       3, 1, { 0xA60000UL },                  0xFF8F80UL

#define BTST_Z \
        OP_BTST_Z,       3, 1, { 0xA30800UL },                  0xFF0F80UL
#define BTSTW_Z \
        OP_BTSTW_Z,      3, 1, { 0xA58000UL },                  0xFF8780UL
#define BTST_C \
        OP_BTST_C,       3, 1, { 0xA30000UL },                  0xFF0F80UL
#define BTSTW_C \
        OP_BTSTW_C,      3, 1, { 0xA50000UL },                  0xFF8780UL
#define BTSTF_W \
        OP_BTSTF_B,      3, 1, { 0xAB0000UL },                  0x000000UL
#define BTSTF_B \
        OP_BTSTF_B,      3, 1, { 0xAB0000UL },                  0xFF0000UL
#define BTST_B \
        OP_BTST_B,       3, 1, { 0xA30800UL },                  0xFF8F80UL

#define BTSTS_Z \
        OP_BTSTS_Z,      3, 1, { 0xA40800UL },                  0xFF0F80UL
#define BTSTS_C \
        OP_BTSTS_C,      3, 1, { 0xA40000UL },                  0xFF0F80UL
#define BTSTSF_W \
        OP_BTSTSF_B,     3, 1, { 0xAC0000UL },                  0x000000UL
#define BTSTSF_B \
        OP_BTSTSF_B,     3, 1, { 0xAC0000UL },                  0xFF0000UL
#define BTSTS_B \
        OP_BTSTS_B,      3, 1, { 0xA40800UL },                  0xFF8F80UL


#define CALLW \
        OP_CALLW,        3, 1, { 0x010000UL },                  0xFFC600UL
#define CALL \
        OP_CALL,         3, 2, { CALL_INSN,  0x000000UL },    0x00FF0000UL
#define CALL_L \
        OP_CALLW,        3, 1, { 0x018000 },                  0x00FF8600UL


#define CLRW_B \
        OP_CLRW_B,       3, 1, { 0xEF4000UL },                  0xFFFFFFUL
#define CLR_B \
        OP_CLR_B,        3, 1, { 0xEB4000UL },                  0xFFC07FUL
#define CLRF_B \
        OP_CLRF_B,       3, 1, { 0xEF6000UL },                  0xFFE000UL

#define CLRW_W \
        OP_CLRW_W,       3, 1, { 0xEF0000UL },                  0xFFFFFFUL
#define CLR_W \
        OP_CLR_W,        3, 1, { 0xEB0000UL },                  0xFFC07FUL
#define CLRF_W \
        OP_CLRF_W,       3, 1, { 0xEF2000UL },                  0xFFE000UL

#define CLRAC_A \
        OP_CLRAC_A,      3, 1, { 0xC30112UL },                  0xFF7FFFUL
#define CLRAC_WB \
        OP_CLRAC_WB,     3, 1, { 0xC30110UL },                  0xFF7FFCUL
#define CLRAC_X \
        OP_CLRAC_X,      3, 1, { 0xC30012UL },                  0xFF4C3FUL
#define CLRAC_Y \
        OP_CLRAC_Y,      3, 1, { 0xC30102UL },                  0xFF43C3UL
#define CLRAC_XWB \
        OP_CLRAC_XWB,    3, 1, { 0xC30010UL },                  0xFF4C3CUL
#define CLRAC_YWB \
        OP_CLRAC_YWB,    3, 1, { 0xC30100UL },                  0xFF43C0UL
#define CLRAC_XY \
        OP_CLRAC_XY,     3, 1, { 0xC30002UL },                  0xFF4003UL
#define CLRAC \
        OP_CLRAC,        3, 1, { 0xC30000UL },                  0xFF4000UL

#define CLRWDT \
        OP_CLRWDT,       3, 1, { 0xFE6000UL },                  0xFFFFFFUL

#define COMFF_B \
        OP_COMFF_B,      3, 1, { 0xEEE000UL },                  0xFFE000UL
#define COMFW_B \
        OP_COMFW_B,      3, 1, { 0xEEC000UL },                  0xFFE000UL
#define COM_B \
        OP_COM_B,        3, 1, { 0xEAC000UL },                  0xFFC000UL

#define COMFF_W \
        OP_COMFF_W,      3, 1, { 0xEEA000UL },                  0xFFE000UL
#define COMFW_W \
        OP_COMFW_W,      3, 1, { 0xEE8000UL },                  0xFFE000UL
#define COM_W \
        OP_COM_W,        3, 1, { 0xEA8000UL },                  0xFFC000UL

#define CPLS_B \
        OP_CPLS_B,       3, 1, { 0xE10460UL },                  0xFF87E0UL
#define CPLS8_B \
        OP_CPLS8_B,      3, 1, { 0xE10460UL },                  0xFF8460UL
#define CPF_B \
        OP_CPF_B,        3, 1, { 0xE34000UL },                  0xFFE000UL
#define CP_B \
        OP_CP_B,         3, 1, { 0xE10400UL },                  0xFF8780UL

#define CPLS_W \
        OP_CPLS_W,       3, 1, { 0xE10060UL },                  0xFF87E0UL
#define CPLS8_W \
        OP_CPLS8_W,      3, 1, { 0xE10060UL },                  0xFF8460UL
#define CPF_W \
        OP_CPF_W,        3, 1, { 0xE30000UL },                  0xFFE000UL
#define CP_W \
        OP_CP_W,         3, 1, { 0xE10000UL },                  0xFF8780UL

#define CPBLS_B \
        OP_CPBLS_B,      3, 1, { 0xE18460UL },                  0xFF87E0UL
#define CPBLS8_B \
        OP_CPBLS8_B,     3, 1, { 0xE18460UL },                  0xFF8460UL
#define CPBF_B \
        OP_CPBF_B,       3, 1, { 0xE3C000UL },                  0xFFE000UL
#define CPB_B \
        OP_CPB_B,        3, 1, { 0xE18400UL },                  0xFF8780UL

#define CPBLS_W \
        OP_CPBLS_W,      3, 1, { 0xE18060UL },                  0xFF87E0UL
#define CPBLS8_W \
        OP_CPBLS8_W,     3, 1, { 0xE18060UL },                  0xFF8460UL
#define CPBF_W \
        OP_CPBF_W,       3, 1, { 0xE38000UL },                  0xFFE000UL
#define CPB_W \
        OP_CPB_W,        3, 1, { 0xE18000UL },                  0xFF8780UL

#define CP0F_B \
        OP_CP0F_B,       3, 1, { 0xE24000UL },                  0xFFE000UL
#define CP0_B \
        OP_CP0_B,        3, 1, { 0xE00400UL },                  0xFFFF80UL

#define CP0F_W \
        OP_CP0F_W,       3, 1, { 0xE20000UL },                  0xFFE000UL
#define CP0_W \
        OP_CP0_W,        3, 1, { 0xE00000UL },                  0xFFFF80UL

#define CP1_B \
        OP_CP1_B,        3, 1, { 0xE0C000UL },                  0xFFFF80UL
#define CP1_W \
        OP_CP1_W,        3, 1, { 0xE08000UL },                  0xFFFF80UL

#define CPWSEQ_B \
        OP_CPWSEQ_B,     3, 1, { CPWBEQ_B_INSN },               0xFF87F0UL
#define CPWBEQ_B \
        OP_CPWBEQ_B,     3, 1, { CPWBEQ_B_INSN },               0xFF8400UL
#define CPWSEQ_W \
        OP_CPWSEQ_W,     3, 1, { CPWBEQ_W_INSN },               0xFF87F0UL
#define CPWBEQ_W \
        OP_CPWBEQ_W,     3, 1, { CPWBEQ_W_INSN },               0xFF8400UL
#define CPWSEQ_E_B \
        OP_CPWSEQ_E_B,   3, 1, { 0xE78410UL },                  0xFF87F0UL
#define CPWSEQ_E_W \
        OP_CPWSEQ_E_W,   3, 1, { 0xE78010UL },                  0xFF87F0UL


#define CPWSGT_B \
        OP_CPWSGT_B,     3, 1, { CPWBGT_B_INSN },               0xFF87F0UL
#define CPWBGT_B \
        OP_CPWBGT_B,     3, 1, { CPWBGT_B_INSN },               0xFF8400UL
#define CPWSGT_W \
        OP_CPWSGT_W,     3, 1, { CPWBGT_W_INSN },               0xFF87F0UL
#define CPWBGT_W \
        OP_CPWBGT_W,     3, 1, { CPWBGT_W_INSN },               0xFF8400UL
#define CPWSGT_E_B \
        OP_CPWSGT_E_B,   3, 1, { 0xE60410UL },                  0xFF87F0UL
#define CPWSGT_E_W \
        OP_CPWSGT_E_W,   3, 1, { 0xE60010UL },                  0xFF87F0UL


#define CPWSLT_B \
        OP_CPWSLT_B,     3, 1, { CPWBLT_B_INSN },               0xFF87F0UL
#define CPWBLT_B \
        OP_CPWBLT_B,     3, 1, { CPWBLT_B_INSN },               0xFF8400UL
#define CPWSLT_W \
        OP_CPWSLT_W,     3, 1, { CPWBLT_W_INSN },               0xFF87F0UL
#define CPWBLT_W \
        OP_CPWBLT_W,     3, 1, { CPWBLT_W_INSN },               0xFF8400UL
#define CPWSLT_E_B \
        OP_CPWSLT_E_B,   3, 1, { 0xE68410UL },                  0xFF87F0UL
#define CPWSLT_E_W \
        OP_CPWSLT_E_W,   3, 1, { 0xE68010UL },                  0xFF87F0UL


#define CPWSNE_B \
        OP_CPWSNE_B,     3, 1, { CPWBNE_B_INSN },               0xFF87F0UL
#define CPWBNE_B \
        OP_CPWBNE_B,     3, 1, { CPWBNE_B_INSN },               0xFF8400UL
#define CPWSNE_W \
        OP_CPWSNE_W,     3, 1, { CPWBNE_W_INSN },               0xFF87F0UL
#define CPWBNE_W \
        OP_CPWBNE_W,     3, 1, { CPWBNE_W_INSN },               0xFF8400UL
#define CPWSNE_E_B \
        OP_CPWSNE_E_B,   3, 1, { 0xE70410UL },                  0xFF87F0UL
#define CPWSNE_E_W \
        OP_CPWSNE_E_W,   3, 1, { 0xE70010UL },                  0xFF87F0UL
#define CTXTSWPL \
        OP_CTXTSWPL,     3, 1, { 0xFEE000UL },                  0xFFF000UL
#define CTXTSWPW \
        OP_CTXTSWPW,     3, 1, { 0xFEF000UL },                  0xFFF000UL

#define DAW_B \
        OP_DAW_B,        3, 1, { 0xFD4000UL },                  0xFFFFF0UL

#define DECFF_B \
        OP_DECFF_B,      3, 1, { 0xED6000UL },                  0xFFE000UL
#define DECFW_B \
        OP_DECFW_B,      3, 1, { 0xED4000UL },                  0xFFE000UL
#define DEC_B \
        OP_DEC_B,        3, 1, { 0xE94000UL },                  0xFFC000UL

#define DECFF_W \
        OP_DECFF_W,      3, 1, { 0xED2000UL },                  0xFFE000UL
#define DECFW_W \
        OP_DECFW_W,      3, 1, { 0xED0000UL },                  0xFFE000UL
#define DEC_W \
        OP_DEC_W,        3, 1, { 0xE90000UL },                  0xFFC000UL

#define DEC2FF_B \
        OP_DEC2FF_B,     3, 1, { 0xEDE000UL },                  0xFFE000UL
#define DEC2FW_B \
        OP_DEC2FW_B,     3, 1, { 0xEDC000UL },                  0xFFE000UL
#define DEC2_B \
        OP_DEC2_B,       3, 1, { 0xE9C000UL },                  0xFFC000UL
#define DEC2FF_W \
        OP_DEC2FF_W,     3, 1, { 0xEDA000UL },                  0xFFE000UL
#define DEC2FW_W \
        OP_DEC2FW_W,     3, 1, { 0xED8000UL },                  0xFFE000UL
#define DEC2_W \
        OP_DEC2_W,       3, 1, { 0xE98000UL },                  0xFFC000UL

#define DISI \
        OP_DISI,         3, 1, { 0xFC0000UL },                  0xFFC000UL

#define DIVF \
        OP_DIVF,         3, 1, { 0xD90000UL },                  0xFF0020UL

#define DIVF2 \
        OP_DIVF2,        3, 1, { 0xD90020UL },                  0xFF0020UL

#define DIVS_W \
        OP_DIVS_W,       3, 1, { 0xD80000UL },                  0xFF8060UL
#define DIVS_D \
        OP_DIVS_D,       3, 1, { 0xD80040UL },                  0xFF8060UL

#define DIVSW2 \
        OP_DIVSW2,       3, 1, { DIVSW2_INSN },                 DIVXW2_MASK
#define DIVSL2 \
        OP_DIVSL2,       3, 1, { DIVSL2_INSN },                 DIVXL2_MASK
#define DIVSD2 \
        OP_DIVSD2,       3, 1, { 0xD80060UL },                  0xFF8060UL

#define DIVU_W \
        OP_DIVU_W,       3, 1, { 0xD88000UL },                  0xFF8060UL
#define DIVU_D \
        OP_DIVU_D,       3, 1, { 0xD88040UL },                  0xFF8060UL

#define DIVUW2 \
        OP_DIVUW2,       3, 1, { DIVUW2_INSN },                 DIVXW2_MASK
#define DIVUL2 \
        OP_DIVUL2,       3, 1, { DIVUL2_INSN },                 DIVXL2_MASK
#define DIVUD2 \
        OP_DIVUD2,       3, 1, { 0xD88060UL },                  0xFF8060UL

#define DOW \
        OP_DOW,          3, 2, { DOW_INSN, 0x000000UL },      0x00FF80F0UL
#define DO \
        OP_DO,           3, 2, { DO_INSN,  0x000000UL },      0x00FFC000UL
#define DOE \
        OP_DO,           3, 2, { DO_INSN,  0x000000UL },      0x00FF8000UL

#define ED \
        OP_ED,           3, 1, { 0xF04003UL },                  0xFC4003UL

#define EDAC \
        OP_EDAC,         3, 1, { 0xF04002UL },                  0xFC4C03UL

#define EXCH \
        OP_EXCH,         3, 1, { 0xFD0000UL },                  0xFFF870UL

#define FBCL \
        OP_FBCL,         3, 1, { 0xDF0000UL },                  0xFF1000UL

#define FF1L \
        OP_FF1L,         3, 1, { 0xCF8000UL },                  0xFFF800UL

#define FF1R \
        OP_FF1R,         3, 1, { 0xCF0000UL },                  0xFFF800UL

#define FLIM \
        OP_FLIM,         3, 1, { 0xE40000UL },                  0xFF8780UL
#define FLIMW \
        OP_FLIMW,        3, 1, { 0xE50000UL },                  0xFF8000UL
#define FLIMWV \
        OP_FLIMWV,       3, 1, { 0xE58000UL },                  0xFF8000UL

#define GOTOW \
        OP_GOTOW,        3, 1, { 0x014000UL },                  0xFFC000UL
#define GOTO \
        OP_GOTO,         3, 2, { GOTO_INSN,  0x000000UL },    0x00FF0000UL
#define GOTOWE \
        OP_GOTOW,        3, 1, { 0x010400UL },                  0xFF8600UL
#define GOTOW_L \
        OP_GOTOW,        3, 1, { 0x018400UL },                  0xFF8600UL


#define INCFF_B \
        OP_INCFF_B,      3, 1, { 0xEC6000UL },                  0xFFE000UL
#define INCFW_B \
        OP_INCFW_B,      3, 1, { 0xEC4000UL },                  0xFFE000UL
#define INC_B \
        OP_INC_B,        3, 1, { 0xE84000UL },                  0xFFC000UL

#define INCFF_W \
        OP_INCFF_W,      3, 1, { 0xEC2000UL },                  0xFFE000UL
#define INCFW_W \
        OP_INCFW_W,      3, 1, { 0xEC0000UL },                  0xFFE000UL
#define INC_W \
        OP_INC_W,        3, 1, { 0xE80000UL },                  0xFFC000UL

#define INC2FF_B \
        OP_INC2FF_B,     3, 1, { 0xECE000UL },                  0xFFE000UL
#define INC2FW_B \
        OP_INC2FW_B,     3, 1, { 0xECC000UL },                  0xFFE000UL
#define INC2_B \
        OP_INC2_B,       3, 1, { 0xE8C000UL },                  0xFFC000UL
#define INC2FF_W \
        OP_INC2FF_W,     3, 1, { 0xECA000UL },                  0xFFE000UL
#define INC2FW_W \
        OP_INC2FW_W,     3, 1, { 0xEC8000UL },                  0xFFE000UL
#define INC2_W \
        OP_INC2_W,       3, 1, { 0xE88000UL },                  0xFFC000UL

#define IORWFF_B \
        OP_IORWFF_B,     3, 1, { 0xB76000UL },                  0xFFE000UL
#define IORWFW_B \
        OP_IORWFW_B,     3, 1, { 0xB74000UL },                  0xFFE000UL
#define IORLW_B \
        OP_IORLW_B,      3, 1, { 0xB34000UL },                  0xFFC000UL
#define IORLS_B \
        OP_IORLS_B,      3, 1, { 0x704060UL },                  0xF84060UL
#define IOR_B \
        OP_IOR_B,        3, 1, { 0x704000UL },                  0xF84000UL

#define IORWFF_W \
        OP_IORWFF_W,     3, 1, { 0xB72000UL },                  0xFFE000UL
#define IORWFW_W \
        OP_IORWFW_W,     3, 1, { 0xB70000UL },                  0xFFE000UL
#define IORLW_W \
        OP_IORLW_W,      3, 1, { 0xB30000UL },                  0xFFC000UL
#define IORLS_W \
        OP_IORLS_W,      3, 1, { 0x700060UL },                  0xF84060UL
#define IOR_W \
        OP_IOR_W,        3, 1, { 0x700000UL },                  0xF84000UL

#define LAC_PS \
        OP_LAC_PS,       3, 1, { 0xCA0000UL },                  0xFF0000UL
#define LAC \
        OP_LAC,          3, 1, { 0xCA0000UL },                  0xFF0780UL

#define LACD_PS \
        OP_LACD_PS,      3, 1, { 0xDB0000UL },                  0xFF0000UL
#define LACD \
        OP_LACD,         3, 1, { 0xDB0000UL },                  0xFF0780UL

#define LDSLV \
        OP_LDSLV,        3, 1, { 0x030010UL },                  0xFF8050UL

#define LNK \
        OP_LNK,          3, 1, { 0xFA0000UL },                  0xFFC000UL

#define LSRFF_B \
        OP_LSRFF_B,      3, 1, { 0xD56000UL },                  0xFFE000UL
#define LSRFW_B \
        OP_LSRFW_B,      3, 1, { 0xD54000UL },                  0xFFE000UL
#define LSR_B \
        OP_LSR_B,        3, 1, { 0xD14000UL },                  0xFFC000UL

#define LSRFF_W \
        OP_LSRFF_W,      3, 1, { 0xD52000UL },                  0xFFE000UL
#define LSRFW_W \
        OP_LSRFW_W,      3, 1, { 0xD50000UL },                  0xFFE000UL
#define LSR_W \
        OP_LSR_W,        3, 1, { 0xD10000UL },                  0xFFC000UL
#define LSRW_W \
        OP_LSRW_W,       3, 1, { 0xDE0000UL },                  0xFF8070UL
#define LSRK_W \
        OP_LSRK_W,       3, 1, { 0xDE0040UL },                  0xFF8070UL

#define MAXAB \
        OP_MAXAB,        3, 1, { 0xCE0000UL },                  0xFF6000UL
#define MAXABW \
        OP_MAXABW,       3, 1, { 0xCE1000UL },                  0xFF7800UL
#define MAXABWV \
        OP_MAXABWV,      3, 1, { 0xCE1800UL },                  0xFF7800UL

#define MINAB \
        OP_MINAB,        3, 1, { 0xCE2000UL },                  0xFF6400UL
#define MINABW \
        OP_MINABW,       3, 1, { 0xCE3000UL },                  0xFF7C00UL
#define MINABWV \
        OP_MINABWV,      3, 1, { 0xCE3800UL },                  0xFF7C00UL

#define MINABZ \
        OP_MINABZ,       3, 1, { 0xCE2400UL },                  0xFF6400UL
#define MINABWZ \
        OP_MINABWZ,      3, 1, { 0xCE3400UL },                  0xFF7C00UL
#define MINABWVZ \
        OP_MINABWVZ,     3, 1, { 0xCE3C00UL },                  0xFF7C00UL

#define MPY_A \
        OP_MPY_A,        3, 1, { 0xC00113UL },                  0xF87FFFUL
#define MPY_X \
        OP_MPY_X,        3, 1, { 0xC00013UL },                  0xF84C3FUL
#define MPY_Y \
        OP_MPY_Y,        3, 1, { 0xC00103UL },                  0xF843C3UL
#define MPY \
        OP_MPY,          3, 1, { 0xC00003UL },                  0xF84003UL
#define SQR_A \
        OP_SQR_A,        3, 1, { 0xF00111UL },                  0xFC7FFFUL
#define SQR_X \
        OP_SQR_X,        3, 1, { 0xF00011UL },                  0xFC4C3FUL
#define SQR_Y \
        OP_SQR_Y,        3, 1, { 0xF00101UL },                  0xFC73C3UL
#define SQR \
        OP_SQR,          3, 1, { 0xF00001UL },                  0xFC4003UL

#define MPYN_A \
        OP_MPYN_A,       3, 1, { 0xC04113UL },                  0xF87FFFUL
#define MPYN_X \
        OP_MPYN_X,       3, 1, { 0xC04013UL },                  0xF84C3FUL
#define MPYN_Y \
        OP_MPYN_Y,       3, 1, { 0xC04103UL },                  0xF843C3UL
#define MPYN \
        OP_MPYN,         3, 1, { 0xC04003UL },                  0xF84003UL

#define MPY_A \
        OP_MPY_A,        3, 1, { 0xC00113UL },                  0xF87FFFUL
#define MPY_X \
        OP_MPY_X,        3, 1, { 0xC00013UL },                  0xF84C3FUL
#define MPY_Y \
        OP_MPY_Y,        3, 1, { 0xC00103UL },                  0xF843C3UL
#define MPY \
        OP_MPY,          3, 1, { 0xC00003UL },                  0xF84003UL
#define SQR_A \
        OP_SQR_A,        3, 1, { 0xF00111UL },                  0xFC7FFFUL
#define SQR_X \
        OP_SQR_X,        3, 1, { 0xF00011UL },                  0xFC4C3FUL
#define SQR_Y \
        OP_SQR_Y,        3, 1, { 0xF00101UL },                  0xFC73C3UL
#define SQR \
        OP_SQR,          3, 1, { 0xF00001UL },                  0xFC4003UL

#define MPYN_A \
        OP_MPYN_A,       3, 1, { 0xC04113UL },                  0xF87FFFUL
#define MPYN_X \
        OP_MPYN_X,       3, 1, { 0xC04013UL },                  0xF84C3FUL
#define MPYN_Y \
        OP_MPYN_Y,       3, 1, { 0xC04103UL },                  0xF843C3UL
#define MPYN \
        OP_MPYN,         3, 1, { 0xC04003UL },                  0xF84003UL

#define MOVSAC_A \
        OP_MOVSAC_A,     3, 1, { 0xC70112UL },                  0xFF7FFFUL
#define MOVSAC_WB \
        OP_MOVSAC_WB,    3, 1, { 0xC70110UL },                  0xFF7FFCUL
#define MOVSAC_X \
        OP_MOVSAC_X,     3, 1, { 0xC70012UL },                  0xFF4C3FUL
#define MOVSAC_Y \
        OP_MOVSAC_Y,     3, 1, { 0xC70102UL },                  0xFF43C3UL
#define MOVSAC_XWB \
        OP_MOVSAC_XWB,   3, 1, { 0xC70010UL },                  0xFF4C3CUL
#define MOVSAC_YWB \
        OP_MOVSAC_YWB,   3, 1, { 0xC70100UL },                  0xFF43C0UL
#define MOVSAC_XY \
        OP_MOVSAC_XY,    3, 1, { 0xC70002UL },                  0xFF4003UL
#define MOVSAC \
        OP_MOVSAC,       3, 1, { 0xC70000UL },                  0xFF4000UL

#define MAC_A \
        OP_MAC_A,        3, 1, { 0xC00112UL },                  0xF87FFFUL
#define MAC_WB \
        OP_MAC_WB,       3, 1, { 0xC00110UL },                  0xF87FFCUL
#define MAC_X \
        OP_MAC_X,        3, 1, { 0xC00012UL },                  0xF84C3FUL
#define MAC_Y \
        OP_MAC_Y,        3, 1, { 0xC00102UL },                  0xF843C3UL
#define MAC_XWB \
        OP_MAC_XWB,      3, 1, { 0xC00010UL },                  0xF84C3CUL
#define MAC_YWB \
        OP_MAC_YWB,      3, 1, { 0xC00100UL },                  0xF843C0UL
#define MAC_XY \
        OP_MAC_XY,       3, 1, { 0xC00002UL },                  0xF84003UL
#define MAC \
        OP_MAC,          3, 1, { 0xC00000UL },                  0xF84000UL
#define SQRAC_A \
        OP_SQRAC_A,      3, 1, { 0xF00110UL },                  0xFC7FFFUL
#define SQRAC_X \
        OP_SQRAC_X,      3, 1, { 0xF00010UL },                  0xFC4C3FUL
#define SQRAC_Y \
        OP_SQRAC_Y,      3, 1, { 0xF00100UL },                  0xFC73C3UL
#define SQRAC \
        OP_SQRAC,        3, 1, { 0xF00000UL },                  0xFC4003UL

#define MOVWF_B \
        OP_MOVWF_B,      3, 1, { 0xB7E000UL },                  0xFFE000UL
#define MOVLW_B \
        OP_MOVLW_B,      3, 1, { 0xB3C000UL },                  0xFFF000UL
#define MOVLW_W \
        OP_MOVLW_W,      3, 1, { 0xB38000UL },                  0xFFC000UL
#define MOVFF_B \
        OP_MOVFF_B,      3, 1, { 0xBFE000UL },                  0xFFE000UL
#define MOVFW_B \
        OP_MOVFW_B,      3, 1, { 0xBFC000UL },                  0xFFE000UL
#define MOV_B \
        OP_MOV_B,        3, 1, { 0x784000UL },                  0xF84000UL
#define LDWLO_B \
        OP_LDWLO_B,      3, 1, { 0x904000UL },                  0xF84000UL
#define STWLO_B \
        OP_STWLO_B,      3, 1, { 0x984000UL },                  0xF84000UL

#define LDDW \
        OP_LDDW,         3, 1, { 0xBE0000UL },                  0xFFF800UL
#define STDW \
        OP_STDW,         3, 1, { 0xBE8000UL },                  0xFFC070UL

#define MOVWF_W \
        OP_MOVWF_W,      3, 1, { 0xB7A000UL },                  0xFFE000UL
#define MOVL_W \
        OP_MOVL_W,       3, 1, { 0x200000UL },                  0xF00000UL
#define MOVFF_W \
        OP_MOVFF_W,      3, 1, { 0xBFA000UL },                  0xFFE000UL
#define MOVFW_W \
        OP_MOVFW_W,      3, 1, { 0xBF8000UL },                  0xFFE000UL
#define MOV_W \
        OP_MOV_W,        3, 1, { 0x780000UL },                  0xF84000UL
#define MOVPAG \
        OP_MOVPAG,       3, 1, { 0xFEC000UL },                  0xFFE000UL
#define MOVPAG_W \
        OP_MOVPAG_W,     3, 1, { 0xFED000UL },                  0xFFF000UL
#define LDWLO_W \
        OP_LDWLO_W,      3, 1, { 0x900000UL },                  0xF84000UL
#define STWLO_W \
        OP_STWLO_W,      3, 1, { 0x980000UL },                  0xF84000UL
#define STW \
        OP_STW,          3, 1, { 0x880000UL },                  0xF80000UL
#define LDW \
	OP_LDW,          3, 1, { 0x800000UL },                  0xF80000UL

#define MSC_A \
        OP_MSC_A,        3, 1, { 0xC04112UL },                  0xF87FFFUL
#define MSC_WB \
        OP_MSC_WB,       3, 1, { 0xC04110UL },                  0xF87FFCUL
#define MSC_X \
        OP_MSC_X,        3, 1, { 0xC04012UL },                  0xF84C3FUL
#define MSC_Y \
        OP_MSC_Y,        3, 1, { 0xC04102UL },                  0xF843C3UL
#define MSC_XWB \
        OP_MSC_XWB,      3, 1, { 0xC04010UL },                  0xF84C3CUL
#define MSC_YWB \
        OP_MSC_YWB,      3, 1, { 0xC04100UL },                  0xF843C0UL
#define MSC_XY \
        OP_MSC_XY,       3, 1, { 0xC04002UL },                  0xF84003UL
#define MSC \
        OP_MSC,          3, 1, { 0xC04000UL },                  0xF84000UL

#define MULWF_B \
        OP_MULWF_B,      3, 1, { 0xBC4000UL },                  0xFFE000UL
#define MULWF_W \
        OP_MULWF_W,      3, 1, { 0xBC0000UL },                  0xFFE000UL

#define MUL_SS \
        OP_MUL_SS,       3, 1, { 0xB98000UL },                  0xFF8080UL
#define MUL_SS_ACC \
        OP_MUL_SS_ACC,   3, 1, { 0xB98700UL },                  0xFF8700UL
#define MULW_SS \
        OP_MULW_SS,      3, 1, { 0xB98080UL },                  0xFF8080UL
#define MULLS_SU \
        OP_MULLS_SU,     3, 1, { 0xB90060UL },                  0xFF80E0UL
#define MULLSW_SU \
        OP_MULLSW_SU,    3, 1, { 0xB900E0UL },                  0xFF80E0UL
#define MULLS_SU_ACC \
        OP_MULLS_SU_ACC, 3, 1, { 0xB90760UL },                  0xFF8760UL
#define MUL_SU \
        OP_MUL_SU,       3, 1, { 0xB90000UL },                  0xFF8080UL
#define MUL_SU_ACC \
        OP_MUL_SU_ACC,   3, 1, { 0xB90700UL },                  0xFF8700UL
#define MULW_SU \
        OP_MULW_SU,      3, 1, { 0xB90080UL },                  0xFF8080UL
#define MULLS_UU \
        OP_MULLS_UU,     3, 1, { 0xB80060UL },                  0xFF80E0UL
#define MULLSW_UU \
        OP_MULLSW_UU,    3, 1, { 0xB800E0UL },                  0xFF80E0UL
#define MULLS_UU_ACC \
        OP_MULLS_UU_ACC, 3, 1, { 0xB80760UL },                  0xFF8760UL
#define MUL_UU \
        OP_MUL_UU,       3, 1, { 0xB80000UL },                  0xFF8080UL
#define MULW_UU \
        OP_MULW_UU,      3, 1, { 0xB80080UL },                  0xFF8080UL
#define MUL_UU_ACC \
        OP_MUL_UU_ACC,   3, 1, { 0xB80700UL },                  0xFF8700UL
#define MUL_US \
        OP_MUL_US,       3, 1, { 0xB88000UL },                  0xFF8080UL
#define MUL_US_ACC \
        OP_MUL_US_ACC,   3, 1, { 0xB88700UL },                  0xFF8700UL
#define MULW_US \
        OP_MULW_US,      3, 1, { 0xB88080UL },                  0xFF8080UL

#define NEGFF_B \
        OP_NEGFF_B,      3, 1, { 0xEE6000UL },                  0xFFE000UL
#define NEGFW_B \
        OP_NEGFW_B,      3, 1, { 0xEE4000UL },                  0xFFE000UL
#define NEG_B \
        OP_NEG_B,        3, 1, { 0xEA4000UL },                  0xFFC000UL

#define NEGFF_W \
        OP_NEGFF_W,      3, 1, { 0xEE2000UL },                  0xFFE000UL
#define NEGFW_W \
        OP_NEGFW_W,      3, 1, { 0xEE0000UL },                  0xFFE000UL
#define NEG_W \
        OP_NEG_W,        3, 1, { 0xEA0000UL },                  0xFFC000UL
#define NEGAB \
        OP_NEGAB,        3, 1, { 0xCB1000UL },                  0xFF7000UL

#define NOP \
        OP_NOP,          3, 1, { 0x000000UL },                  0xFF0000UL
#define NOPR \
        OP_NOPR,         3, 1, { 0xFF0000UL },                  0xFF0000UL

#define NORMACW \
        OP_NORMACW,      3, 1, { 0xCE6000UL },                  0xFF6000UL

#define POP_W \
        OP_POP_W,        3, 1, { 0x78004FUL },                  0xFFC07FUL
#define POPF \
        OP_POPF,         3, 1, { 0xF90000UL },                  0xFF0000UL

#define POP_D \
        OP_POP_D,        3, 1, { 0xBE004FUL },                  0xFFF87FUL

#define ITCH \
        OP_ITCH,         3, 1, { 0xFE8000UL },                  0xFFFFFFUL

#define PWRSAV \
        OP_PWRSAV,       3, 1, { 0xFE4000UL },                  0xFFFFFEUL

#define PUSH_W \
        OP_PUSH_W,       3, 1, { 0x781F80UL },                  0xFFFF80UL
#define PUSHF \
        OP_PUSHF,        3, 1, { 0xF80000UL },                  0xFF0000UL

#define PUSH_D \
        OP_PUSH_D,       3, 1, { 0xBE9F80UL },                  0xFFFFF0UL

#define SCRATCH \
        OP_SCRATCH,      3, 1, { 0xFEA000UL },                  0xFFFFFFUL

#define RCALLW \
        OP_RCALLW,       3, 1, { 0x012000UL },                  0xFFFFF0UL
#define RCALL \
        OP_RCALL,        3, 1, { RCALL_INSN },                  0xFF0000UL
#define RCALLWE \
        OP_RCALLW,       3, 1, { 0x010200UL },                  0xFF0600UL


#define REPEATW \
        OP_REPEATW,      3, 1, { REPEATW_INSN },                  0xFFFFF0UL
#define REPEATL \
        OP_REPEATL,      3, 1, { REPEATW_INSN },                  0xFFFFF0UL
#define REPEAT \
        OP_REPEAT,       3, 1, { REPEAT_INSN },                 0xFFC000UL
#define REPEATE \
        OP_REPEAT,       3, 1, { REPEAT_INSN },                 0xFF8000UL

#define RESET \
        OP_RESET,        3, 1, { 0xFE0000UL },                  0xFFFFFFUL

#define RETFIE \
        OP_RETFIE,       3, 1, { 0x064000UL },                  0xFFFFFFUL

#define RETLW_B \
        OP_RETLW_B,      3, 1, { 0x054000UL },                  0xFFC000UL
#define RETLW_W \
        OP_RETLW_W,      3, 1, { 0x050000UL },                  0xFFC000UL

#define RETURN \
        OP_RETURN,       3, 1, { 0x060000UL },                  0xFFFFFFUL

#define RLCFF_B \
        OP_RLCFF_B,      3, 1, { 0xD6E000UL },                  0xFFE000UL
#define RLCFW_B \
        OP_RLCFW_B,      3, 1, { 0xD6C000UL },                  0xFFE000UL
#define RLC_B \
        OP_RLC_B,        3, 1, { 0xD2C000UL },                  0xFFC000UL

#define RLCFF_W \
        OP_RLCFF_W,      3, 1, { 0xD6A000UL },                  0xFFE000UL
#define RLCFW_W \
        OP_RLCFW_W,      3, 1, { 0xD68000UL },                  0xFFE000UL
#define RLC_W \
        OP_RLC_W,        3, 1, { 0xD28000UL },                  0xFFC000UL

#define RLNCFF_B \
        OP_RLNCFF_B,     3, 1, { 0xD66000UL },                  0xFFE000UL
#define RLNCFW_B \
        OP_RLNCFW_B,     3, 1, { 0xD64000UL },                  0xFFE000UL
#define RLNC_B \
        OP_RLNC_B,       3, 1, { 0xD24000UL },                  0xFFC000UL

#define RLNCFF_W \
        OP_RLNCFF_W,     3, 1, { 0xD62000UL },                  0xFFE000UL
#define RLNCFW_W \
        OP_RLNCFW_W,     3, 1, { 0xD60000UL },                  0xFFE000UL
#define RLNC_W \
        OP_RLNC_W,       3, 1, { 0xD20000UL },                  0xFFC000UL

#define RRCFF_B \
        OP_RRCFF_B,      3, 1, { 0xD7E000UL },                  0xFFE000UL
#define RRCFW_B \
        OP_RRCFW_B,      3, 1, { 0xD7C000UL },                  0xFFE000UL
#define RRC_B \
        OP_RRC_B,        3, 1, { 0xD3C000UL },                  0xFFC000UL

#define RRCFF_W \
        OP_RRCFF_W,      3, 1, { 0xD7A000UL },                  0xFFE000UL
#define RRCFW_W \
        OP_RRCFW_W,      3, 1, { 0xD78000UL },                  0xFFE000UL
#define RRC_W \
        OP_RRC_W,        3, 1, { 0xD38000UL },                  0xFFC000UL

#define RRNCFF_B \
        OP_RRNCFF_B,     3, 1, { 0xD76000UL },                  0xFFE000UL
#define RRNCFW_B \
        OP_RRNCFW_B,     3, 1, { 0xD74000UL },                  0xFFE000UL
#define RRNC_B \
        OP_RRNC_B,       3, 1, { 0xD34000UL },                  0xFFC000UL

#define RRNCFF_W \
        OP_RRNCFF_W,     3, 1, { 0xD72000UL },                  0xFFE000UL
#define RRNCFW_W \
        OP_RRNCFW_W,     3, 1, { 0xD70000UL },                  0xFFE000UL
#define RRNC_W \
        OP_RRNC_W,       3, 1, { 0xD30000UL },                  0xFFC000UL

#define SAC_PS \
        OP_SAC_PS,       3, 1, { 0xCC0000UL },                  0xFF0000UL
#define SAC \
        OP_SAC,          3, 1, { 0xCC0000UL },                  0xFF0780UL

#define SACD_PS \
        OP_SACD_PS,      3, 1, { 0xDC0000UL },                  0xFF4070UL
#define SACD \
        OP_SACD,         3, 1, { 0xDC0000UL },                  0xFF47F0UL

#define SAC_R_PS \
        OP_SAC_R_PS,     3, 1, { 0xCD0000UL },                  0xFF0000UL
#define SAC_R \
        OP_SAC_R,        3, 1, { 0xCD0000UL },                  0xFF0780UL

#define SE_W \
        OP_SE_W,         3, 1, { 0xFB0000UL },                  0xFFF800UL
 
#define SETW_B \
        OP_SETW_B,       3, 1, { 0xEFC000UL },                  0xFFFFFFUL
#define SET_B \
        OP_SET_B,        3, 1, { 0xEBC000UL },                  0xFFC07FUL
#define SETF_B \
        OP_SETF_B,       3, 1, { 0xEFE000UL },                  0xFFE000UL

#define SETW_W \
        OP_SETW_W,       3, 1, { 0xEF8000UL },                  0xFFFFFFUL
#define SET_W \
        OP_SET_W,        3, 1, { 0xEB8000UL },                  0xFFC07FUL
#define SETF_W \
        OP_SETF_W,       3, 1, { 0xEFA000UL },                  0xFFE000UL

#define SFTACW \
        OP_SFTACW,       3, 1, { 0xC80000UL },                  0xFF0040UL
#define SFTACK \
        OP_SFTACK,       3, 1, { 0xC80040UL },                  0xFF0040UL

#define SLFF_B \
        OP_SLFF_B,       3, 1, { 0xD46000UL },                  0xFFE000UL
#define SLFW_B \
        OP_SLFW_B,       3, 1, { 0xD44000UL },                  0xFFE000UL
#define SL_B \
        OP_SL_B,         3, 1, { 0xD04000UL },                  0xFFC000UL

#define SLFF_W \
        OP_SLFF_W,       3, 1, { 0xD42000UL },                  0xFFE000UL
#define SLFW_W \
        OP_SLFW_W,       3, 1, { 0xD40000UL },                  0xFFE000UL
#define SL_W \
        OP_SL_W,         3, 1, { 0xD00000UL },                  0xFFC000UL
#define SLW_W \
        OP_SLW_W,        3, 1, { 0xDD0000UL },                  0xFF8070UL
#define SLK_W \
        OP_SLK_W,        3, 1, { 0xDD0040UL },                  0xFF8070UL

#define SUBWFF_B \
        OP_SUBWFF_B,     3, 1, { 0xB56000UL },                  0xFFE000UL
#define SUBWFW_B \
        OP_SUBWFW_B,     3, 1, { 0xB54000UL },                  0xFFE000UL
#define SUBLW_B \
        OP_SUBLW_B,      3, 1, { 0xB14000UL },                  0xFFC000UL
#define SUBLS_B \
        OP_SUBLS_B,      3, 1, { 0x504060UL },                  0xF84060UL
#define SUB_B \
        OP_SUB_B,        3, 1, { 0x504000UL },                  0xF84000UL

#define SUBWFF_W \
        OP_SUBWFF_W,     3, 1, { 0xB52000UL },                  0xFFE000UL
#define SUBWFW_W \
        OP_SUBWFW_W,     3, 1, { 0xB50000UL },                  0xFFE000UL
#define SUBLW_W \
        OP_SUBLW_W,      3, 1, { 0xB10000UL },                  0xFFC000UL
#define SUBLS_W \
        OP_SUBLS_W,      3, 1, { 0x500060UL },                  0xF84060UL
#define SUB_W \
        OP_SUB_W,        3, 1, { 0x500000UL },                  0xF84000UL
#define SUBAB \
        OP_SUBAB,        3, 1, { 0xCB3000UL },                  0xFF7000UL

#define SUBBWFF_B \
        OP_SUBBWFF_B,    3, 1, { 0xB5E000UL },                  0xFFE000UL
#define SUBBWFW_B \
        OP_SUBBWFW_B,    3, 1, { 0xB5C000UL },                  0xFFE000UL
#define SUBBLW_B \
        OP_SUBBLW_B,     3, 1, { 0xB1C000UL },                  0xFFC000UL
#define SUBBLS_B \
        OP_SUBBLS_B,     3, 1, { 0x584060UL },                  0xF84060UL
#define SUBB_B \
        OP_SUBB_B,       3, 1, { 0x584000UL },                  0xF84000UL

#define SUBBWFF_W \
        OP_SUBBWFF_W,    3, 1, { 0xB5A000UL },                  0xFFE000UL
#define SUBBWFW_W \
        OP_SUBBWFW_W,    3, 1, { 0xB58000UL },                  0xFFE000UL
#define SUBBLW_W \
        OP_SUBBLW_W,     3, 1, { 0xB18000UL },                  0xFFC000UL
#define SUBBLS_W \
        OP_SUBBLS_W,     3, 1, { 0x580060UL },                  0xF84060UL
#define SUBB_W \
        OP_SUBB_W,       3, 1, { 0x580000UL },                  0xF84000UL

#define SUBRWFF_B \
        OP_SUBRWFF_B,    3, 1, { 0xBD6000UL },                  0xFFE000UL
#define SUBRWFW_B \
        OP_SUBRWFW_B,    3, 1, { 0xBD4000UL },                  0xFFE000UL
#define SUBRLS_B \
        OP_SUBRLS_B,     3, 1, { 0x104060UL },                  0xF84060UL
#define SUBR_B \
        OP_SUBR_B,       3, 1, { 0x104000UL },                  0xF84000UL

#define SUBRWFF_W \
        OP_SUBRWFF_W,    3, 1, { 0xBD2000UL },                  0xFFE000UL
#define SUBRWFW_W \
        OP_SUBRWFW_W,    3, 1, { 0xBD0000UL },                  0xFFE000UL
#define SUBRLS_W \
        OP_SUBRLS_W,     3, 1, { 0x100060UL },                  0xF84060UL
#define SUBR_W \
        OP_SUBR_W,       3, 1, { 0x100000UL },                  0xF84000UL

#define SUBBRWFF_B \
        OP_SUBBRWFF_B,   3, 1, { 0xBDE000UL },                  0xFFE000UL
#define SUBBRWFW_B \
        OP_SUBBRWFW_B,   3, 1, { 0xBDC000UL },                  0xFFE000UL
#define SUBBRLS_B \
        OP_SUBBRLS_B,    3, 1, { 0x184060UL },                  0xF84060UL
#define SUBBR_B \
        OP_SUBBR_B,      3, 1, { 0x184000UL },                  0xF84000UL

#define SUBBRWFF_W \
        OP_SUBBRWFF_W,   3, 1, { 0xBDA000UL },                  0xFFE000UL
#define SUBBRWFW_W \
        OP_SUBBRWFW_W,   3, 1, { 0xBD8000UL },                  0xFFE000UL
#define SUBBRLS_W \
        OP_SUBBRLS_W,    3, 1, { 0x180060UL },                  0xF84060UL
#define SUBBR_W \
        OP_SUBBR_W,      3, 1, { 0x180000UL },                  0xF84000UL

#define SWAP_B \
        OP_SWAP_B,       3, 1, { 0xFDC000UL },                  0xFFFFF0UL
#define SWAP_W \
        OP_SWAP_W,       3, 1, { 0xFD8000UL },                  0xFFFFF0UL

#define TBLRDH_B \
        OP_TBLRDH_B,     3, 1, { 0xBAC000UL },                  0xFFC000UL
#define TBLRDH_W \
        OP_TBLRDH_W,     3, 1, { 0xBA8000UL },                  0xFFC000UL

#define TBLRDL_B \
        OP_TBLRDL_B,     3, 1, { 0xBA4000UL },                  0xFFC000UL
#define TBLRDL_W \
        OP_TBLRDL_W,     3, 1, { 0xBA0000UL },                  0xFFC000UL

#define TBLWTH_B \
        OP_TBLWTH_B,     3, 1, { 0xBBC000UL },                  0xFFC000UL
#define TBLWTH_W \
        OP_TBLWTH_W,     3, 1, { 0xBB8000UL },                  0xFFC000UL

#define TBLWTL_B \
        OP_TBLWTL_B,     3, 1, { 0xBB4000UL },                  0xFFC000UL
#define TBLWTL_W \
        OP_TBLWTL_W,     3, 1, { 0xBB0000UL },                  0xFFC000UL

#define ULNK \
        OP_ULNK,         3, 1, { 0xFA8000UL },                  0xFFFFFFUL

#define VFSLV \
        OP_VFSLV,        3, 1, { 0x038010UL },                  0xFF8050UL

#define XORWFF_B \
        OP_XORWFF_B,     3, 1, { 0xB6E000UL },                  0xFFE000UL
#define XORWFW_B \
        OP_XORWFW_B,     3, 1, { 0xB6C000UL },                  0xFFE000UL
#define XORLW_B \
        OP_XORLW_B,      3, 1, { 0xB2C000UL },                  0xFFC000UL
#define XORLS_B \
        OP_XORLS_B,      3, 1, { 0x684060UL },                  0xF84060UL
#define XOR_B \
        OP_XOR_B,        3, 1, { 0x684000UL },                  0xF84000UL

#define XORWFF_W \
        OP_XORWFF_W,     3, 1, { 0xB6A000UL },                  0xFFE000UL
#define XORWFW_W \
        OP_XORWFW_W,     3, 1, { 0xB68000UL },                  0xFFE000UL
#define XORLW_W \
        OP_XORLW_W,      3, 1, { 0xB28000UL },                  0xFFC000UL
#define XORLS_W \
        OP_XORLS_W,      3, 1, { 0x680060UL },                  0xF84060UL
#define XOR_W \
        OP_XOR_W,        3, 1, { 0x680000UL },                  0xF84000UL

#define ZE_W \
        OP_ZE_W,         3, 1, { 0xFB8000UL },                  0xFFF800UL

/******************************************************************************/

/*  INSN -                                       RS  RD
 *  general 32-bit format: |S|OOOOOO|W|RsRs|RdRd| EA| EA|RbRb|XX|CC|EE|
 *                         | |3     | |   2|        |  1|    |  |  | 0|
 *                         |1|098765|4|3210|9876|543|210|9876|54|32|10|
 *
 *  general 16-bit format: |S|OOOOOO|W|RsRs|RdRd|
 *                         | |    1 | |    |    |
 *                         |5|432109|8|7654|3210|
 *
 *  Legend:
 *    S  - size (32-bit or 16-bit)
 *    O  - opcode
 *    W  - data width (.l or .w)
 *    Rs - Typically Wd +                             (*)
 *    Rd - Typically Ws + ---- confusing, right?      (*)
 *    EA - effective address for each register        (*)
 *    Rb - base register                              (*)
 *    X  - often literal parts                        (*)
 *    C  - 'control bits'
 *    E  - opcode extension                           
 *
 * 16-bit addressing modes are implicit by the opcode
 * (*) these are operands, not encoded here
 *
 */

/* F_ISA32V0 */
/* for now use the generated file directly */

#include "header.h"

#endif /* #ifndef __PIC30_ OPC_H__ */
