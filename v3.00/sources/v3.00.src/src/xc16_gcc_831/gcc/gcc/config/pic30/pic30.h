#ifndef _PIC30_H_
#define _PIC30_H_

/* Definitions of target machine for GNU compiler.
   Copyright (C) 1994, 95-98, 1999 Free Software Foundation, Inc.
   Contributed by Microchip.

This file is part of GNU CC.

GNU CC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 1, or (at your option)
any later version.

GNU CC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU CC; see the file COPYING.  If not, write to
the Free Software Foundation, 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  */

/************************************************************************/
/* This is the target description macros file for the Microchip dsPIC.   */
/************************************************************************/

#define IN_TARGET_CODE 1
#include "config/mchp-cci/cci-backend.h"

#if defined(OBJECT_FORMAT_ELF)
#define   PIC30_DWARF2   1      /* ELF/DWARF default output ?   */
#else
#define   PIC30_DWARF2   0      /* ELF/DWARF default output ?   */
#endif

#ifndef JOIN
#define JOIN2(X,Y) X ## Y
#define JOIN(X,Y) JOIN2(X,Y)
#endif

/*
 *  define this if you want the RTL memory address dumped out before each
 *    instruction whenever the instruction is dumped.  Useful for debugging
 *    with -dapA, but probably not how we want to release a compiler (though
 *    it'll be no uglier to the user than how the RTL is currently dumped).
 */
#define MCHP_TRACE  1

/*
 * define this to 0 or 1 depending upon the targets default value for the
 *    size of doubles  0 => large doubles
 */
#define TARGET_SHORT_DOUBLE  1

/*
 *  definitions of possible return types for pic30 operand types 
 */


/* Define the intrinsic functions for the dsPIC.  */

enum pic30_builtins
{
   PIC30_BUILTIN_READSFR,     /*  __builtin_readsfr(...) */
   PIC30_BUILTIN_WRITESFR,    /*  __builtin_writesfr(...) */
   PIC30_BUILTIN_TBLADDRESS,  /*  __builtin_tbladdress(...) */
   PIC30_BUILTIN_TBLPAGE,     /*  __builtin_tblpage(...) */
   PIC30_BUILTIN_DATAFLASHOFFSET,   /*  __builtin_dataflashoffset(...) */
   PIC30_BUILTIN_TBLOFFSET,   /*  __builtin_tbloffset(...) */
   PIC30_BUILTIN_PSVPAGE,     /*  __builtin_psvpage(...) */
   PIC30_BUILTIN_PSVOFFSET,   /*  __builtin_psvoffset(...) */
   PIC30_BUILTIN_NOP,         /*  __builtin_nop(void) */
   PIC30_BUILTIN_DIVSD,       /*  __builtin_divsd(...) */
   PIC30_BUILTIN_MODSD,       /*  __builtin_modsd(...) */
   PIC30_BUILTIN_DIVMODSD,    /*  __builtin_divmodsd(...) */
   PIC30_BUILTIN_DIVUD,       /*  __builtin_divud(...) */
   PIC30_BUILTIN_MODUD,       /*  __builtin_modud(...) */
   PIC30_BUILTIN_DIVMODUD,    /*  __builtin_divmodud(...) */
   PIC30_BUILTIN_MULSS,       /*  __builtin_mulss(...) */
   PIC30_BUILTIN_MULSU,       /*  __builtin_mulsu(...) */
   PIC30_BUILTIN_MULUS,       /*  __builtin_mulus(...) */
   PIC30_BUILTIN_MULUU,       /*  __builtin_muluu(...) */
   PIC30_BUILTIN_BTG,         /*  __builtin_btg(...) */
   PIC30_BUILTIN_DMAPAGE,     /*  __builtin_dmapage() */
   PIC30_BUILTIN_DMAOFFSET,   /*  __builtin_dmaoffset() */
   PIC30_BUILTIN_ADDAB,       /* not used anymore */
   PIC30_BUILTIN_ADD,         /* not used anymore, ADD_16 or ADD_32 */
   PIC30_BUILTIN_CLR,         /* not used anymore, CLR_16 or CLR_32 */
   PIC30_BUILTIN_CLR_PREFETCH,
   PIC30_BUILTIN_ED,          /* classic devices only */
   PIC30_BUILTIN_EDAC,
   PIC30_BUILTIN_FBCL,
   PIC30_BUILTIN_LAC,
   PIC30_BUILTIN_LACD,
   PIC30_BUILTIN_MAC,
   PIC30_BUILTIN_MOVSAC,
   PIC30_BUILTIN_MPY,
   PIC30_BUILTIN_MPYN,
   PIC30_BUILTIN_MSC,
   PIC30_BUILTIN_SAC,
   PIC30_BUILTIN_SACD,
   PIC30_BUILTIN_SACR,
   PIC30_BUILTIN_SFTAC,
   PIC30_BUILTIN_SUBAB,
   PIC30_BUILTIN_DISI,
   PIC30_BUILTIN_ITTYPE,
   PIC30_BUILTIN_UNIQUEID,
   PIC30_BUILTIN_WRITEOSCCONL,
   PIC30_BUILTIN_WRITEOSCCONH,
   PIC30_BUILTIN_WRITERTCWEN,
   PIC30_BUILTIN_WRITEWRLOCK,
   PIC30_BUILTIN_WRITENVM,
   PIC30_BUILTIN_WRITENVM_SECURE,
   PIC30_BUILTIN_TBLRDLB,
   PIC30_BUILTIN_TBLRDHB,
   PIC30_BUILTIN_TBLRDL,
   PIC30_BUILTIN_TBLRDH,
   PIC30_BUILTIN_TBLWTL,
   PIC30_BUILTIN_TBLWTH,
   PIC30_BUILTIN_TBLWTLB,
   PIC30_BUILTIN_TBLWTHB,
   PIC30_BUILTIN_DIVF,
   PIC30_BUILTIN_READ_EXT8,
   PIC30_BUILTIN_WRITE_EXT8,
   PIC30_BUILTIN_EDSPAGE,
   PIC30_BUILTIN_EDSOFFSET,
   PIC30_BUILTIN_WRITEPWMSFR,
   PIC30_BUILTIN_WRITEDISICNT,
   PIC30_BUILTIN_WRITECRYOTP,
   MCHP_BUILTIN_SECTION_BEGIN,
   MCHP_BUILTIN_SECTION_SIZE,
   MCHP_BUILTIN_SECTION_END,
   MCHP_BUILTIN_GET_ISR_STATE,
   MCHP_BUILTIN_SET_ISR_STATE,
   MCHP_BUILTIN_DISABLE_ISR,
   MCHP_BUILTIN_ENABLE_ISR,
   MCHP_BUILTIN_SOFTWARE_BREAK,
   PIC30_BUILTIN_WRITEDATAFLASH,
   PIC30_BUILTIN_WRITEDATAFLASH_SECURE,
   PIC30_BUILTIN_ADDR_LOW,
   PIC30_BUILTIN_ADDR_HIGH,
   PIC30_BUILTIN_ADDR,
   PIC30_BUILTIN_PWRSAV,
   PIC30_BUILTIN_CLRWDT,
   PIC30_BUILTIN_ACCL,
   PIC30_BUILTIN_ACCH,
   PIC30_BUILTIN_ACCU,
   PIC30_BUILTIN_FF1R,
   PIC30_BUILTIN_FF1L,
   PIC30_BUILTIN_WRITERPCON,
   PIC30_BUILTIN_SWAP,
   PIC30_BUILTIN_SWAP_BYTE,
   PIC30_BUILTIN_FLIM_16,
   PIC30_BUILTIN_FLIM_EXCESS_16,
   PIC30_BUILTIN_FLIMV_EXCESS_16,
   PIC30_BUILTIN_FLIM_32,
   PIC30_BUILTIN_FLIM_EXCESS_32,
   PIC30_BUILTIN_FLIMV_EXCESS_32,
   /* these are integer only */
   PIC30_BUILTIN_MIN_16,
   PIC30_BUILTIN_MAX_16,
   PIC30_BUILTIN_MIN_32,
   PIC30_BUILTIN_MAX_32,
   /* these are DSP only (original) */
   PIC30_BUILTIN_MIN,
   PIC30_BUILTIN_MIN_EXCESS,
   PIC30_BUILTIN_MINV_EXCESS,
   PIC30_BUILTIN_MAX,
   PIC30_BUILTIN_MAX_EXCESS,
   PIC30_BUILTIN_MAXV_EXCESS,
   PIC30_BUILTIN_DISICTL,
   PIC30_BUILTIN_DIVFSIHI,
   PIC30_BUILTIN_DIVFSI,
   PIC30_BUILTIN_DIVMODF,
   PIC30_BUILTIN_DIVMODFSIHI,
   PIC30_BUILTIN_DIVMODFSI,
   PIC30_BUILTIN_FBCL_16,
   PIC30_BUILTIN_FBCL_32,
   PIC30_BUILTIN_FF1L_16,
   PIC30_BUILTIN_FF1L_32,
   PIC30_BUILTIN_FF1R_16,
   PIC30_BUILTIN_FF1R_32,
   PIC30_BUILTIN_ADDAB_16,
   PIC30_BUILTIN_ADDAB_32,
   PIC30_BUILTIN_ADD_16,
   PIC30_BUILTIN_ADD_32,
   PIC30_BUILTIN_CLR_16,
   PIC30_BUILTIN_CLR_32,
   PIC30_BUILTIN_ED_16,
   PIC30_BUILTIN_ED_32,
   PIC30_BUILTIN_SET_FSR,
   PIC30_BUILTIN_SET_FCR,
   PIC30_BUILTIN_SET_FEAR,
   PIC30_BUILTIN_GET_FSR,
   PIC30_BUILTIN_GET_FCR,
   PIC30_BUILTIN_GET_FEAR,
   PIC30_BUILTIN_IOR_FCR,
   PIC30_BUILTIN_AND_FCR,
   PIC30_BUILTIN_ASHIFTRT3216,
   PIC30_BUILTIN_LSHIFTRT3216,
   PIC30_BUILTIN_BITCOPY
};

#define       TARGET_USE_A   1

enum xc16_processor_type {
  PROC_CLASSIC,          /* any non-pipelined architecture */
  PROC_PIPE_V1,          /* pipeline V1 */
};

extern enum xc16_processor_type xc16_tune;

/*
** A C string constant that tells the GNU CC driver program options to pass to
** the assembler. It can also specify how to translate options you give to GNU
** CC into options for GNU CC to pass to the assembler. See the file `sun3.h'
** for an example of this. 
*/
#define QUOTE2(X) #X
#define QUOTE(X) QUOTE2(X)

#define ASM_SPEC   " \
  %{mpartition=*:--partition %*} \
  %{!.s:%{!.S:--relax}} \
  %{mcpu=*:-p%*} \
  %{mdfp=*: -mdfp=%*} \
  -omf=" OMF

#ifndef MCHP_CCI_CC1_SPEC
#error MCHP_CCI_CC1_SPEC not defined
#endif

#define CC1_SPEC  " \
  %(mchp_cci_cc1_spec) \
  %{!mresource=: %{!mdfp=*: -mresource=%I-../../c30_device.info}} \
  %{mpa: %{save-temps: -save-temps} %{save-temps=*: -save-temps=%*}} \
  %{mpa=*: %{save-temps: -save-temps} %{save-temps=*: -save-temps=%*}} \
  -omf=" OMF

#define LINK_SPEC   " \
  %{mmemorysummary=*:--memorysummary %*} \
  %{mpartition=*:--partition %*} \
  %{mcpu=*:-p%*} \
  %{mdfp=*: --mdfp=%*} \
  %{mfill=*:--fill=%*} \
  %{mchp-stack-usage: -mchp-stack-usage} \
  %{mchp-stack-usage=*: -mchp-stack-usage=%*} \
  -omf=" OMF

/* for C30 put the linker script before the libraries in case there
 * are any optional libaries pulled in that might need libc (or other)
 * support.   Also it allows defines in the linker script to pull
 * other bits in from libraries... though I don't think this happens
 * for XC16/C30.
 */
#define LINK_COMMAND_SPEC "\
%{!fsyntax-only:%{!c:%{!M:%{!MM:%{!E:%{!S:\
    %(linker) " \
    LINK_PLUGIN_SPEC \
   "%{flto|flto=*:%<fcompare-debug*} \
    %{flto} %{fno-lto} %{flto=*} %l " LINK_PIE_SPEC \
   "%{fuse-ld=*:-fuse-ld=%*} " LINK_COMPRESS_DEBUG_SPEC \
   "%X %{o*} %{e*} %{N} %{n} %{r}\
    %{s} %{t} %{u*} %{z} %{Z} %{!nostdlib:%{!nostartfiles:%S}} \
    %{static|no-pie|static-pie:} %{L*} %(mfwrap) %(link_libgcc) " \
    VTABLE_VERIFICATION_SPEC " " SANITIZER_EARLY_SPEC " %o " CHKP_SPEC " \
    %{fopenacc|fopenmp|%:gt(%{ftree-parallelize-loops=*:%*} 1):\
        %:include(libgomp.spec)%(link_gomp)}\
    %{fgnu-tm:%:include(libitm.spec)%(link_itm)}\
    %(mflib) " STACK_SPLIT_SPEC "\
    %{fprofile-arcs|fprofile-generate*|coverage:-lgcov} " SANITIZER_SPEC " \
    %{T*} \
    %{!nostdlib:%{!nodefaultlibs:%(link_ssp) %(link_gcc_c_sequence)}}\
    %{!nostdlib:%{!nostartfiles:%E}}   \n%(post_link) }}}}}}"


/*
** A C string constant that tells the GNU CC driver program how to run any
** programs before the normal assembler.  This gets tacked on to the end
** of the compile line, in case any additions to the compiler line are needed.
**
** For dsPIC, this macro is used as a pre-assembly stage, for implementing
** procedure aggregation.
**
** The %(program_prefix) is used to distinguish COFF and ELF/DWARF tools.
*/
#if TARGET_USE_PA
#define       PRE_ASM_SPEC ""\
"%{pipe: %e-pipe and -mpa are incompatible} "\
"%{!mpa: %{save-temps: -o %b.s} %{!save-temps: %{!S:-o %g.s} %{S:%{!o*:-o %b.s}%{o*:%{o*}}}}\n}"  \
"%{mpa: %{save-temps: -o %b.p} %{!save-temps: %{S: -o %b.p\n} %{!S:-o %g.p}}\n" \
    "%(program_prefix)pa -omf=%(omf) %{mcpu=*:--mcpu=%*} %{mpa=*:-n%*} %{v:-v} "\
       "%{save-temps: -o%b.s}" \
       "%{!save-temps: %{!S:-o%g.s} %{S:%{!o*:-o%b.s}%{o*:%{o*}}}} "  \
       "%{save-temps: %b.p} %{!save-temps: %{S: %b.p} %{!S: %g.p}}\n}"
#endif

/*
** A C string constant that tells the GNU CC driver program how and when to
** place a reference to `libgcc.a' into the linker command line.
** This constant is placed both before and after the value of LIB_SPEC. 
** If this macro is not defined, the GNU CC driver provides a default that
** passes the string `-lgcc' to the linker unless the `-shared' option is
** specified. 
*/
#undef   LIBGCC_SPEC
#define   LIBGCC_SPEC   ""
/*
** If this macro is not defined, a default is provided that loads the
** standard C library from the usual place.
*/
#undef   LIB_SPEC
#define  ALT_LIB_SPECS
#if (PIC30_DWARF2)
#define   ALT_C99_LIB_SPEC  "-start-group -lc99-pic30-elf -lm-elf -lc99-elf -end-group"
#else
#define   ALT_C99_LIB_SPEC  "-start-group -lc99-pic30-coff -lm-coff -lc99-coff -end-group"
#endif

/*
** Another C string constant used much like LINK_SPEC. The difference between
** the two is that STARTFILE_SPEC is used at the very beginning of the command
** given to the linker. 
** If this macro is not defined, a default is provided that loads the standard
** C startup file from the usual place. See `gcc.c'.
*/
#undef   STARTFILE_SPEC
#define   STARTFILE_SPEC   ""

/* This macro defines names of additional specifications to put in the specs
   that can be used in various specifications like CC1_SPEC.  Its definition
   is an initializer with a subgrouping for each command option.

   Each subgrouping contains a string constant, that defines the
   specification name, and a string constant that used by the GCC driver
   program.  */
#undef EXTRA_SPECS
#define EXTRA_SPECS											\
  { "mchp_cci_cc1_spec", MCHP_CCI_CC1_SPEC },				\
  SUBTARGET_EXTRA_SPECS

#ifndef SUBTARGET_EXTRA_SPECS
#define SUBTARGET_EXTRA_SPECS
#endif

/* making STANDARD_EXEC_PREFIX and STANDARD_BINDIR_PREFIX point to the same
   directory will cause make_relative_paths to make no change - ie look in the 
   gcc executable's directory.  */
#undef STANDARD_EXEC_PREFIX
#define STANDARD_EXEC_PREFIX "/bin/"

#undef STANDARD_LIBEXEC_PREFIX
#define STANDARD_LIBEXEC_PREFIX "/bin/"

#undef STANDARD_BINDIR_PREFIX
#define STANDARD_BINDIR_PREFIX "/bin/bin"

/* By default, the GCC_EXEC_PREFIX_ENV prefix is "GCC_EXEC_PREFIX", however
   in a cross compiler, another environment variable might want to be used
   to avoid conflicts with the host any host GCC_EXEC_PREFIX */
#ifndef GCC_EXEC_PREFIX_ENV
#define GCC_EXEC_PREFIX_ENV "PIC30_EXEC_PREFIX"
#endif

/* By default, the COMPILER_PATH_ENV is "COMPILER_PATH", however
   in a cross compiler, another environment variable might want to be used
   to avoid conflicts with the host any host COMPILER_PATH */
#ifndef COMPILER_PATH_ENV
#define COMPILER_PATH_ENV "PIC30_COMPILER_PATH"
#endif

/* By default, the C_INCLUDE_PATH_ENV is "C_INCLUDE_PATH", however
   in a cross compiler, another environment variable might want to be used
   to avoid conflicts with the host any host C_INCLUDE_PATH */
#ifndef C_INCLUDE_PATH_ENV
#define C_INCLUDE_PATH_ENV "PIC30_C_INCLUDE_PATH"
#endif

/* By default, the LIBRARY_PATH_ENV is "LIBRARY_PATH", however
   in a cross compiler, another environment variable might want to be used
   to avoid conflicts with the host any host LIBRARY_PATH */
#ifndef LIBRARY_PATH_ENV
#define LIBRARY_PATH_ENV "PIC30_LIBRARY_PATH"
#endif


/* define PATH to be used if C_INCLUDE_PATH is not declared 
   (and CPLUS_INCLUDE_PATH for C++, &c).  The directories are all relative
   to the current executable's directory */

#ifndef TARGET_EXTRA_INCLUDES
extern void pic30_system_include_paths(const char *root, const char *system, 
                                       int nostdinc);

#define TARGET_EXTRA_INCLUDES pic30_system_include_paths
#endif

#ifdef PATH_SEPARATOR
#if PATH_SEPARATOR == ';'
#define PATH_SEPARATOR_STR ";"
#else 
#define PATH_SEPARATOR_STR ":"
#endif
#endif

#ifdef DIR_SEPARATOR
#if DIR_SEPARATOR == '\\'
#define DIR_SEPARATOR_STR "\\"
#else 
#define DIR_SEPARATOR_STR "/"
#endif
#endif

#define DEFAULT_LIB_PATH  \
         MPLABC30_COMMON_LIB_PATH PATH_SEPARATOR_STR \
         MPLABC30_PIC24E_LIB_PATH PATH_SEPARATOR_STR \
         MPLABC30_PIC24F_LIB_PATH PATH_SEPARATOR_STR \
         MPLABC30_PIC24H_LIB_PATH PATH_SEPARATOR_STR \
         MPLABC30_PIC30F_LIB_PATH PATH_SEPARATOR_STR \
         MPLABC30_PIC33C_LIB_PATH PATH_SEPARATOR_STR \
         MPLABC30_PIC33E_LIB_PATH PATH_SEPARATOR_STR \
         MPLABC30_PIC33A_LIB_PATH PATH_SEPARATOR_STR \
         MPLABC30_PIC33F_LIB_PATH
        
#define DEFAULT_ERRATA_LIB_PATH  \
         MPLABC30_ERRATA_COMMON_LIB_PATH PATH_SEPARATOR_STR \
         MPLABC30_PIC24E_LIB_PATH PATH_SEPARATOR_STR \
         MPLABC30_PIC24F_LIB_PATH PATH_SEPARATOR_STR \
         MPLABC30_PIC24H_LIB_PATH PATH_SEPARATOR_STR \
         MPLABC30_PIC30F_LIB_PATH PATH_SEPARATOR_STR \
         MPLABC30_PIC33C_LIB_PATH PATH_SEPARATOR_STR \
         MPLABC30_PIC33E_LIB_PATH PATH_SEPARATOR_STR \
         MPLABC30_PIC33A_LIB_PATH PATH_SEPARATOR_STR \
         MPLABC30_PIC33F_LIB_PATH
        
#define MASK_ARCH_PIC30F         (MASK_ARCH_PIC30FXXXX | MASK_ARCH_PIC30F202X)

/*
** Small code model means that calls to functions
** can be done using the short RCALL instruction.
*/
#define TARGET_SMALL_CODE   ((target_flags & MASK_LARGE_CODE) == 0)

#define TARGET_CONST_IN_CODE ((!TARGET_CONST_IN_DATA) && (!TARGET_CONST_IN_PSV))

/*
 *  Generic test to see if ARCHictecture version X is requested:
 *    current values for X are A and B
 */
#define TARGET_ARCH(X)  ((target_flags & MASK_ARCH_ ## X) != 0)

#define TARGET_CLASSIC (!TARGET_ISA32)
#define TARGET_ISA32 (pic30_isa32_target())

#define TARGET_ISA32_UNSUPPORTED_YET(X,message)             \
   if (TARGET_ISA32) {                                      \
     error(message " is not yet supported on this target"); \
     X;                                                     \
   }                                                        \
   (void)0

#define TARGET_ISA32_UNSUPPORTED(X,message)             \
   if (TARGET_ISA32) {                                  \
     error(message " is not supported on this target"); \
     X;                                                 \
   }                                                    \
   (void)0

#define TARGET_CLASSIC_UNSUPPORTED(X,message)           \
   if (TARGET_CLASSIC) {                                \
     error(message " is not supported on this target"); \
     X;                                                 \
   }                                                    \
   (void)0

#define PIC30_TARGET_GEN(X) (TARGET_CLASSIC ? JOIN(X,_16) : JOIN(X,_32))
#define PIC30_CLASSIC_GEN(X) (JOIN(X,_16))
#define PIC30_ISA32_GEN(X) (JOIN(X,_32))

#define PIC30_TARGET_EXPAND(X, PARAMS)    \
   { rtx x;                               \
     if (TARGET_CLASSIC) {                \
       x = JOIN(gen_,JOIN(X,_16))PARAMS;  \
     } else if (TARGET_ISA32) {           \
       x = JOIN(gen_,JOIN(X,_32))PARAMS;  \
     } else gcc_assert(0);                \
     if (x) {                             \
       emit(x);                           \
       DONE;                              \
     }                                    \
   }                                      \
   FAIL

#define PIC30_DSP_TARGET_EXPAND(X, PARAMS) \
   { rtx x;                                \
     if (pic30_dsp_target()) {             \
       if (TARGET_CLASSIC) {               \
         x = JOIN(gen_,JOIN(X,_16))PARAMS; \
       } else if (TARGET_ISA32) {          \
         x = JOIN(gen_,JOIN(X,_32))PARAMS; \
       } else gcc_assert(0);               \
       if (x) {                            \
         emit(x);                          \
         DONE;                             \
       }                                   \
     }                                     \
   }                                       \
   FAIL

#define PIC30_CLASSIC_EXPAND(X, PARAMS)   \
   { rtx x;                               \
     if (TARGET_CLASSIC) {                \
       x = JOIN(gen_,JOIN(X,_16))PARAMS;  \
       if (x) {                           \
         emit(x);                         \
         DONE;                            \
       }                                  \
     }                                    \
   }                                      \
   FAIL

#define PIC30_ISA32_EXPAND(X, PARAMS)     \
   { rtx x;                               \
     if (TARGET_ISA32) {                  \
       x = JOIN(gen_,JOIN(X,_32))PARAMS;  \
       if (x) {                           \
         emit(x);                         \
         DONE;                            \
       }                                  \
     }                                    \
   }                                      \
   FAIL

#define PIC30_TARGET_FN(X, PARAMS)        \
   if (TARGET_CLASSIC) {                  \
     (JOIN(X,_16))PARAMS;                 \
   } else if (TARGET_ISA32) {             \
     emit(JOIN(X,_32))PARAMS;             \
   } else gcc_assert(0);                  \
   DONE


/* 
** This macro is similar to TARGET_SWITCHES but defines names of command
** options that have values. Its definition is an initializer with a
** subgrouping for each command option. 
** Each subgrouping contains a string constant, that defines the fixed part
** of the option name, the address of a variable, and a description string.
** The variable, type char *, is set to the variable part of the given option
** if the fixed part matches. The actual option name is made by appending
** `-m' to the specified name. 
*/

extern int pic30_compiler_version;
extern int pic30_resource_version;
extern char *pic30_target_family;
enum errata_mask {
  retfie_errata =           1,
  retfie_errata_disi =      2,
  psv_errata =              4,
  exch_errata =             8,
  psv_address_errata =      16,
  ecc_errata =              32,
  busmaster_errata =        64,
  psrd_psrd_errata =        128,
  psrd_psrd_errata_movd =   256,
  repeat_gie_errata =       512,
  repeat_nstdis_errata =   1024
};
extern int pic30_errata_mask;

extern int         pic30_io_size_val;
extern int         pic30_clear_fn_list;
/************************************************************************/

/*----------------------------------------------------------------------*/
/*   Target machine storage layout               */
/*----------------------------------------------------------------------*/

/*
** Define this if most significant bit is lowest numbered
** in instructions that operate on numbered bit-fields.
*/
#define BITS_BIG_ENDIAN      0 

/*
** Define this if most significant byte of a word is the lowest numbered.
*/
#define BYTES_BIG_ENDIAN   0

/*
** Define this if most significant word of a multiword value is lowest numbered.
*/
#define WORDS_BIG_ENDIAN   0

/*
** Number of bits in an addressable storage unit
** #define BITS_PER_UNIT      8  (This is the default definition defined in
** gcc/gcc/coretypes.h:397
*/

/*
** Width in bits of a "word", which is the contents of a machine register.
** Note that this is not necessarily the width of data type `int';
** if using 16-bit ints on a 68000, this would still be 32.
** But on a machine with 16-bit registers, this would be 16.
*/
extern int pic30_isa32_target(void );
#define BITS_PER_WORD      (pic30_isa32_target() ? 32 : 16)

/*
** Width of a word, in units (bytes).
*/
#define UNITS_PER_WORD     (pic30_isa32_target() ? 4 : 2)

/* we now need to define this since BITS_PER_WORD is variable */
#define MAX_BITS_PER_WORD  32

/* we now need to define this since BITS_PER_WORD is variable */
#define MIN_UNITS_PER_WORD 2

/*
** A C expression for a string describing the name of the data type to use for
** the result of subtracting two pointers. The typedef name ptrdiff_t is
** defined using the contents of the string.
*/
#ifdef PTRDIFF_TYPE
#undef PTRDIFF_TYPE
#endif
#define PTRDIFF_TYPE      ((TARGET_EDS) ? \
                             "long int" : \
                             "int")

#ifdef UINTPTR_TYPE
#undef UINTPTR_TYPE
#endif
#define UINTPTR_TYPE      ((TARGET_EDS) ? \
                             "long unsigned int" : \
                             "unsigned int")
/*
** Type to use for `size_t'. If undefined, uses `long unsigned int'.
*/
#ifdef SIZE_TYPE
#undef SIZE_TYPE
#endif
#define SIZE_TYPE      ((TARGET_BIG || TARGET_EDS)? \
                           "long unsigned int" :    \
                           "unsigned int")

/*
** The name of the data type to use for wide characters.
*/
#define WCHAR_TYPE      "unsigned int"

/*
** The size in bits of the data type for wide characters
*/
#define WCHAR_TYPE_SIZE      BITS_PER_WORD

/*
** The name of the data type to use for wide characters passed to printf.
*/
#define   WINT_TYPE      "unsigned int"

/*
** dsPIC uses IEEE floating-point format
*/
#define TARGET_FLOAT_FORMAT   IEEE_FLOAT_FORMAT

/*
** Allocation boundary (in *bits*) for storing arguments in argument list.
*/
#define PARM_BOUNDARY      BITS_PER_WORD

/*
** Boundary (in *bits*) on which stack pointer should be aligned.
*/
#define STACK_BOUNDARY      BITS_PER_WORD

/*
**  Default mode for SFR accesses
*/ 
#define SFR_MODE            (pic30_isa32_target() ? SImode : HImode)

/*
** Allocation boundary (in *bits*) for the code of a function.
**   Now there is a distinction between program memory and data memory;
**   program memory is 16 bits wide (but really 24) and data memory is 8 bits
**   wide... reduce the function boundry, so that bits_per_unit remains at 8
*/
#define FUNCTION_BOUNDARY   BITS_PER_WORD

/*
** Alignment of field after `int : 0' in a structure.
*/
#define EMPTY_FIELD_BOUNDARY   BITS_PER_WORD

/*
** No data type wants to be aligned rounder than this.
*/
#define BIGGEST_ALIGNMENT   BITS_PER_WORD

/*
** Biggest alignment supported by the object file format of this machine.
** Use this macro to limit the alignment which can be specified using the
** __attribute__ ((aligned (n))) construct.
** If not defined, the default value is BIGGEST_ALIGNMENT.
*/
#undef MAX_OFILE_ALIGNMENT
#define MAX_OFILE_ALIGNMENT   (32768*BITS_PER_UNIT)

/*
** Define this to 1 if move instructions will actually fail to work
** when given unaligned data.
*/
#define STRICT_ALIGNMENT 1

/*
** Define number of bits in most basic integer type.
** (If undefined, default is BITS_PER_WORD).
*/
#define INT_TYPE_SIZE  BITS_PER_WORD

/*
** Define number of bits in short integer type.
** (If undefined, default is half of BITS_PER_WORD).
*/
#define SHORT_TYPE_SIZE 16

/*
** Define number of bits in long integer type.
** (If undefined, default is BITS_PER_WORD).
*/
#define LONG_TYPE_SIZE  32

/*
** Define number of bits in long long integer type.
** (If undefined, default is twice BITS_PER_WORD).
*/
#define LONG_LONG_TYPE_SIZE  64

/*
** Define number of bits in char type.
** (If undefined, default is one fourth of BITS_PER_WORD).
*/
#define CHAR_TYPE_SIZE  8

/*
** Define number of bits in float type.
** (If undefined, default is BITS_PER_WORD).
*/
#define FLOAT_TYPE_SIZE  32

/*
** Define number of bits in double type.
** (If undefined, default is twice BITS_PER_WORD).
*/
#define DOUBLE_TYPE_SIZE  64

/*
** Define number of bits in long double type.
** (If undefined, default is twice BITS_PER_WORD).
*/
#define LONG_DOUBLE_TYPE_SIZE  64

/*
** Adjust the alignment of a bitfield.
**  on dspic, usually the alignment of the underlieing type.
*/
#define PCC_BITFIELD_TYPE_MATTERS 1

/*
** On some platforms it is nice to increase alignment under certain conditions.
**  on dspic, the minimum alignment for structures is 2
*/
#define DATA_ALIGNMENT(type, align) pic30_data_alignment(type, align)

/*
 * Define the sizes of fixed point types 
 */
#define SHORT_FRACT_TYPE_SIZE 16
#define FRACT_TYPE_SIZE 16
#define LONG_FRACT_TYPE_SIZE 32
#define LONG_LONG_FRACT_TYPE_SIZE 32

#define SHORT_ACCUM_TYPE_SIZE 48
#define ACCUM_TYPE_SIZE 48
#define LONG_ACCUM_TYPE_SIZE 48

/************************************************************************/

/* Standard register usage.  */

/* Define register numbers */

/* Working/address registers */

#define WR0_REGNO    0
#define WR1_REGNO    1
#define WR2_REGNO    2
#define WR3_REGNO    3
#define WR4_REGNO    4
#define WR5_REGNO    5
#define WR6_REGNO    6
#define WR7_REGNO    7
#define WR8_REGNO    8
#define WR9_REGNO    9
#define WR10_REGNO   10
#define WR11_REGNO   11
#define WR12_REGNO   12
#define WR13_REGNO   13
#define WR14_REGNO   14
#define WR15_REGNO   15

#define F0_REGNO     16
#define F1_REGNO     17
#define F2_REGNO     18
#define F3_REGNO     19
#define F4_REGNO     20
#define F5_REGNO     21
#define F6_REGNO     22
#define F7_REGNO     23
#define F8_REGNO     24
#define F9_REGNO     25
#define F10_REGNO    26
#define F11_REGNO    27
#define F12_REGNO    28
#define F13_REGNO    29
#define F14_REGNO    30
#define F15_REGNO    31

#define F16_REGNO    32
#define F17_REGNO    33
#define F18_REGNO    34
#define F19_REGNO    35
#define F20_REGNO    36
#define F21_REGNO    37
#define F22_REGNO    38
#define F23_REGNO    39
#define F24_REGNO    40
#define F25_REGNO    41
#define F26_REGNO    42
#define F27_REGNO    43
#define F28_REGNO    44
#define F29_REGNO    45
#define F30_REGNO    46
#define F31_REGNO    47

#define RCOUNT_REGNO 48
#define A_REGNO      49
#define B_REGNO      50    /* we dont' allocate below here (or RCOUNT) */
#define PSVPAG       51
#define DSRPAG       51
#define PMADDR       52
#define PMMODE       53
#define PMDIN1       54
#define PMDIN2       55
#define DSWPAG       56
#define SINK0        57    /* a register that means we don't need the result */
#define SINK1        58    /* a register that means we don't need the result */
#define SINK2        59    /* a register that means we don't need the result */
#define SINK3        60    /* a register that means we don't need the result */
#define SINK4        61    /* a register that means we don't need the result */
#define SINK5        62    /* a register that means we don't need the result */
#define SINK6        63    /* a register that means we don't need the result */

#define SINK7        64    /* a register that means we don't need the result */
#define CORCON_SATA  65
#define CORCON_SATB  66
#define CORCON_RND   67
#define CORCON       68    /* some part of CORCON as yet TBD */
#define FSR          69    /* Floating point status register */

#define USR          70    /* ALU Status Register */
#define CC_REG       70    /* For cc0 -> MODE_CC migration */

/*
** Number of actual hardware registers.
** The hardware registers are assigned numbers for the compiler
** from 0 to just below FIRST_PSEUDO_REGISTER.
** All registers that the compiler knows about must be given numbers,
** even those that are not normally considered general registers.
*/
#define FIRST_PSEUDO_REGISTER 71

/* Mappings for dsPIC registers */

#define   FP_REGNO   WR14_REGNO
#define   SP_REGNO   WR15_REGNO

/*
** This dsPIC-specific constant defines the highest-numbered
** register that is available for parameter passing.
*/
#define   PIC30_LAST_PARAM_REG  (pic30_num_param_regs-1)

/*
** 1 for registers that have pervasive standard uses
** and are not available for the register allocator.
** W14 is the frame pointer. This is not fixed, since the frame pointer
** may not be used in certain functions.
** W15 is the stack pointer.
*/

#define FIXED_REGISTERS                      \
{                                            \
   /* 0  WREG0 */  0, 0, 0, 0, 0, 0, 0, 0,   \
   /* 8  WREG8 */  0, 0, 0, 0, 0, 0, 0, 1,   \
   /* 16 F0 */     0, 0, 0, 0, 0, 0, 0, 0,   \
   /* 24 F8 */     0, 0, 0, 0, 0, 0, 0, 0,   \
   /* 32 F16 */    0, 0, 0, 0, 0, 0, 0, 0,   \
   /* 40 F24 */    0, 0, 0, 0, 0, 0, 0, 0,   \
   /* 48 RCOUNT */ 1, 0, 0, 1, 0, 0, 0, 0,   \
   /* 56 DSWPAG */ 1, 1, 1, 1, 1, 1, 1, 1,   \
   /* 64 SINK7 */  1, 1, 1, 1, 1, 1, 1       \
}

/*
** 1 for registers not available across function calls.
** These must include the FIXED_REGISTERS and also any
** registers that can be used without being saved.
** The latter must include the registers where values are returned
** and the register where structure-value addresses are passed.
** Aside from that, you can include as many other registers as you like.
**
** If a register has 0 in CALL_USED_REGISTERS, the compiler automatically
** saves it on function entry and restores it on function exit, if
** the register is used within the function.
**
** In short:
**   1: Caller-saved
**   0: Callee-saved
**
** For dsPIC, it makes sense that the number of callee-saved
** registers should by a multiple of two, since storew/loadw operate
** on register pairs.
** If any registers are callee-saved, then many should be saved, since
** the register allocator gives preference to callee-saved registers.
*/

#define CALL_USED_REGISTERS             \
{                                       \
 /* WREG0 */  1, 1, 1, 1, 1, 1, 1, 1,   \
 /* WREG8 */  0, 0, 0, 0, 0, 0, 0, 1,   \
 /* F0 */     1, 1, 1, 1, 1, 1, 1, 1,   \
 /* F8 */     0, 0, 0, 0, 0, 0, 0, 0,   \
 /* F16 */    0, 0, 0, 0, 0, 0, 0, 0,   \
 /* F24 */    0, 0, 0, 0, 0, 0, 0, 0,   \
 /* RCOUNT */ 1, 0, 0, 1, 0, 0, 0, 0,   \
 /* DSWPAG */ 1, 1, 1, 1, 1, 1, 1, 1,   \
 /* SINK7 */  1, 1, 1, 1, 1, 1, 1       \
}

/*
** Order in which to allocate registers.  Each register must be
** listed once, even those in FIXED_REGISTERS.  List frame pointer
** late and fixed registers last.  Note that, in general, we prefer
** registers listed in CALL_USED_REGISTERS, keeping the others
** available for storage of persistent values.
*/

/* try allocating the scratch regsiters backwards to see if we can improve
   parameter access for small functions 
*/

#define ADJUST_REG_ALLOC_ORDER pic30_adjust_reg_alloc_order()

/* Tell IRA to use the order we define rather than messing it up with its
   own cost calculations.  */
#define HONOR_REG_ALLOC_ORDER 1

/* scratch - non parameter - first */
#define REG_ALLOC_ORDER                                   \
   {                                                      \
   /* W0-W7 */    0,  1,  2,  3, 4,  5,  6,  7,           \
   /* F0-F7 */   16, 17, 18, 19, 20, 21, 22, 23,          \
   /* W     */    8,  9, 10, 11, 12, 13, 14,              \
   /* F     */   24, 25, 26,                              \
                 27, 28, 29, 30, 31, 32, 33, 34,          \
                 35, 36, 37, 38, 39, 40, 41, 42,          \
                 43, 44, 45, 46, 47,                      \
   /* DSP   */   49, 50 }


/*
   ((GET_MODE_SIZE (MODE) + UNITS_PER_WORD - 1) / UNITS_PER_WORD)
*/

/*
** Specify the registers used for certain standard purposes.
** The values of these macros are register numbers.
*/

/*
** dsPIC pc isn't overloaded on a register.
*/
/* #define PC_REGNUM  */

/*
** Register to use for pushing function arguments.
*/
#define STACK_POINTER_REGNUM SP_REGNO

/*
** Base register for access to local variables of the function.
*/
#define FRAME_POINTER_REGNUM FP_REGNO

/*
** Base register for access to arguments of the function.
*/
#define ARG_POINTER_REGNUM FP_REGNO

/*
** Register in which static-chain is passed to a function.
*/
#define STATIC_CHAIN_REGNUM   13

/*
** On some architectures the place where the structure value address is found
** by the called function is not the same place that the caller put it. This
** can be due to register windows, or it could be because the function prologue
** moves it to a different place. If the incoming location of the structure
** value address is in a register, define this macro as the register number.
#define STRUCT_VALUE_INCOMING
*/

/*
** Register in which address to store a structure value
** is passed to a function.
#define STRUCT_VALUE_REGNUM   WR0_REGNO
*/

/*
** Define this to be 1 if all structure return values must be in memory.
*/
#define DEFAULT_PCC_STRUCT_RETURN 1

/************************************************************************/

/*
** Define the classes of registers for register constraints in the
** machine description.  Also define ranges of constants.
**
** One of the classes must always be named ALL_REGS and include all hard regs.
** If there is more than one class, another class must be named NO_REGS
** and contain no registers.
**
** The name GENERAL_REGS must be the name of a class (or an alias for
** another name such as ALL_REGS).  This is the class of registers
** that is allowed by "g" or "r" in a register constraint.
** Also, registers outside this class are allocated only when
** instructions express preferences for them.
**
** The classes must be numbered in nondecreasing order; that is,
** a larger-numbered class must never be contained completely
** in a smaller-numbered class.
**
** For any two classes, it is very desirable that there be another
** class that represents their union.
*/

enum reg_class
{
   NO_REGS,
   A_REGS,          /* 1 'a': accumulator regs (w0) */
   B_REGS,          /* 2 'b': accumulator regs (w1) */
   C_REGS,          /* 3 'c': accumulator regs (w2) */
   CC_REGS,         /* 4 'C': accumulator regs 32 bit (w2-w3) */
   ABC_REGS,        /* 5 'a+b+c': accumulator regs (w0..w2) */
   X_PREFETCH_REGS, /* 6  w8,w9: X prefetch registers */
   Y_PREFETCH_REGS, /* 7  w10,w11: Y prefetch registers */
   VERY_RESTRICTED_PRODUCT_REGS,  /* w7 */
   PRODUCT_REGS_UNSIGNED,         /* w4,w6: unsigned mode mac product reg */
   RESTRICTED_PRODUCT_REGS,       /* W4-W6 */
   PRODUCT_REGS_SIGNED,           /* 11 w5,w7: signed mode mac product reg */
   PRODUCT_REGS,    /* 12  w4 - w7: mac product registers  */
   AWB_REGS,        /* 13  w13 */
   QR_REGS,         /* 14 :) quad working registers (w0... w11) */
   ERHI_REGS,       /* 15 :) HI working registers even (w0...w12) */
   ER_REGS,         /* 16 even working regs (w0..w12) */
   E_REGS,          /* 17 'e': data regs (w2..w14) */
   AE_REGS,         /* 18 'a+e': data regs (w0,w2..w14) */
   D_REGS,          /* 19 'd': data regs (w1..w14) */
   SINK_REGS,       /* 20 sink */
   W_REGS,          /* 21 'w': working regs (w0..w15) */
   ACCUM_REGS,      /* 22  A,B: accumulators */
   W_ACCUM_REGS,    /* 23  ACCUM + W_REGS */
   SFR_REGS,        /* 24  PSVPAG &c */
   FLP_REGS,        /* 25  floating point registers */
   ALL_REGS,        /* 26 'r': (w0..w14) */
   LIM_REG_CLASSES
};

#define N_REG_CLASSES (int) LIM_REG_CLASSES

/*
** Since GENERAL_REGS is the same class as W_REGS,
** don't give it a different class number; just make it an alias.
*/
#define GENERAL_REGS W_REGS

/*
** The class value for index registers, and the one for base regs.
*/
#define BASE_REG_CLASS  W_REGS
#define INDEX_REG_CLASS W_REGS

/*
** Give names of register classes as strings for dump file.
*/

#define REG_CLASS_NAMES \
{ \
  "NOREG",      \
  "W0REG",      \
  "W1REG",      \
  "W2REG",      \
  "W2REG32",    \
  "W0+W1+W2",   \
  "W8..W9",     \
  "W10..W11",   \
  "W7",         \
  "w4,W6",      \
  "W4..W6",     \
  "W5,W7",      \
  "W4..W7",     \
  "AWB",        \
  "QRREGS",     \
  "ERHIREGS",   \
  "ERREGS",     \
  "W2..W14",    \
  "W0+W2..W14", \
  "W1..W14",    \
  "SINK",       \
  "W0..W15",    \
  "A..B",       \
  "AB..W",      \
  "SFR",        \
  "FLP",        \
  "ALL_REGS"    \
}

/*
** Define which registers fit in which classes.
** This is an initializer for a vector of HARD_REG_SET
** of length N_REG_CLASSES.
** All dsPIC registers may be used as either BASE or INDEX registers.
** Register W0 may be used as PIC-compatible working registers.
*/

/*
 * sink registers can be ypf/xpf/regs so that reload doesn't try to reload them
 *
 */

#define REG_CLASS_CONTENTS   \
{ \
/* NO_REGS */        { 0x00000000, 0x00000000, 0x00000000 }, \
/* A_REGS */         { 0x00000001, 0x00000000, 0x00000000 }, \
/* B_REGS */         { 0x00000002, 0x00000000, 0x00000000 }, \
/* C_REGS */         { 0x00000004, 0x00000000, 0x00000000 }, \
/* CC_REGS */        { 0x0000000c, 0x00000000, 0x00000000 }, \
/* ABC_REGS */       { 0x00000007, 0x00000000, 0x00000000 }, \
/* X_PREFETCH_REGS */{ 0x00000300, 0x00000000, 0x00000000 }, \
/* Y_PREFETCH_REGS */{ 0x00000c00, 0x00000000, 0x00000000 }, \
/* V.R.PRODUCT_REGS*/{ 0x00000080, 0x00000000, 0x00000000 }, \
/* PRODUCT_R.UNS*/   { 0x00000050, 0x00000000, 0x00000000 }, \
/* R.PRODUCt_REGS */ { 0x00000070, 0x00000000, 0x00000000 }, \
/* PRODUCT_R.SIGN*/  { 0x000000a0, 0x00000000, 0x00000000 }, \
/* PRODUCT_REGS */   { 0x000000f0, 0x00000000, 0x00000000 }, \
/* AWB_REGS */       { 0x00002000, 0x00000000, 0x00000000 }, \
/* QR_REGS */        { 0x00000FFF, 0x00000000, 0x00000000 }, \
/* ERHI_REGS */      { 0x00001555, 0x00000000, 0x00000000 }, \
/* ER_REGS */        { 0x00003FFF, 0x00000000, 0x00000000 }, \
/* E_REGS */         { 0x0000fffc, 0x00000000, 0x00000000 }, \
/* AE_REGS */        { 0x0000fffd, 0x00000000, 0x00000000 }, \
/* D_REGS */         { 0x0000fffe, 0x00000000, 0x00000000 }, \
/* SINK_REGS */      { 0x00000000, 0xfe000000, 0x00000001 }, \
/* W_REGS */         { 0x0000ffff, 0xff080000, 0x00000001 }, \
/* ACCUM_REGS */     { 0x00000000, 0x00060000, 0x00000000 }, \
/* W_ACCUM_REGS */   { 0x0000ffff, 0x00060000, 0x00000000 }, \
/* SFR_REGS */       { 0x00000000, 0x01F90000, 0x0000001E }, \
/* FLP_REGS */       { 0xffff0000, 0x0000ffff, 0x00000000 }, \
/* ALL_REGS */       { 0xffffffff, 0xffffffff, 0x0000007F }  \
}

/*
** The same information, inverted:
** Return the class number of the smallest class containing
** reg number REGNO.  This could be a conditional expression
** or could index an array.
*/
#define IS_EVEN_REG(r)              (((r)&1)==0)
#define IS_QUAD_REG(r)              (((r)&3)==0)
#define IS_PSEUDO_REG(r)            ((r) >= FIRST_PSEUDO_REGISTER)
#define IS_AREG_REG(r)              ((pic30_isa32_target() ? 0 : \
                                                             (r) == WR0_REGNO))
#define IS_BREG_REG(r)              ((r) == WR1_REGNO)
#define IS_CREG_REG(r)              ((r) == WR2_REGNO)
#define IS_AWB_REG(r)               ((r) == WR13_REGNO)
#define IS_DREG_REG(r)              (((r) >= WR1_REGNO) && IS_WREG_REG(r))
#define IS_EREG_REG(r)              (((r) >= WR2_REGNO) && IS_WREG_REG(r))
#define IS_WREG_REG(r)              ((r) < F0_REGNO)
#define IS_AREG_OR_PSEUDO_REG(r)    (IS_AREG_REG(r) || IS_PSEUDO_REG(r))
#define IS_BREG_OR_PSEUDO_REG(r)    (IS_BREG_REG(r) || IS_PSEUDO_REG(r))
#define IS_CREG_OR_PSEUDO_REG(r)    (IS_CREG_REG(r) || IS_PSEUDO_REG(r))
#define IS_EREG_OR_PSEUDO_REG(r)    (IS_EREG_REG(r) || IS_PSEUDO_REG(r))
#define IS_AREG_OR_PSEUDO_REGNO(op) (IS_AREG_OR_PSEUDO_REG(REGNO(op)))
#define IS_BREG_OR_PSEUDO_REGNO(op) (IS_BREG_OR_PSEUDO_REG(REGNO(op)))
#define IS_CREG_OR_PSEUDO_REGNO(op) (IS_CREG_OR_PSEUDO_REG(REGNO(op)))
#define IS_EREG_OR_PSEUDO_REGNO(op) (IS_EREG_OR_PSEUDO_REG(REGNO(op)))
#define IS_VRP_REG(r)               ((r) == WR7_REGNO)
#define IS_RP_REG(r)                (((r) >= WR4_REGNO) && ((r) <= WR6_REGNO))
#define IS_PRODUCT_REG(r)           (((r) >= WR4_REGNO) && ((r) <= WR7_REGNO))
#define IS_XPREFETCH_REG(r)         (((r) == WR8_REGNO) || ((r) == WR9_REGNO))
#define IS_YPREFETCH_REG(r)         (((r) == WR10_REGNO) || ((r) == WR11_REGNO))
#define IS_SFR_REG(r)               ((((r) >= PSVPAG) && ((r) <= DSWPAG)) || \
                                     (((r) >= CORCON_SATA) && ((r) <= FSR)))
#define IS_FLP_REG(r)               (((r) >= F0_REGNO) && ((r) <= F31_REGNO))
#ifdef IGNORE_ACCUM_CHECK
#define MAYBE_IS_ACCUM_REG(r)       (1)
#else
#define MAYBE_IS_ACCUM_REG(r)       (((r) == A_REGNO) || ((r) == B_REGNO))
#endif
#define IS_ACCUM_REG(r)             (((r) == A_REGNO) || ((r) == B_REGNO))
#define OTHER_ACCUM_REG(r)          ((r == A_REGNO) ? B_REGNO : A_REGNO)
#define IS_SINK_REG(r)              ((r >= SINK0) && (r <= SINK7))


#define REGNO_REG_CLASS(REGNO) (   \
   IS_AREG_REG(REGNO) ? A_REGS :   \
   IS_BREG_REG(REGNO) ? B_REGS :   \
   IS_CREG_REG(REGNO) ? C_REGS :   \
   IS_AWB_REG(REGNO) ? AWB_REGS : \
   IS_VRP_REG(REGNO) ? VERY_RESTRICTED_PRODUCT_REGS : \
   IS_RP_REG(REGNO) ? RESTRICTED_PRODUCT_REGS : \
   IS_PRODUCT_REG(REGNO) ? PRODUCT_REGS : \
   IS_XPREFETCH_REG(REGNO) ? X_PREFETCH_REGS : \
   IS_YPREFETCH_REG(REGNO) ? Y_PREFETCH_REGS : \
   IS_ACCUM_REG(REGNO) ? ACCUM_REGS : \
   IS_EREG_REG(REGNO) ? E_REGS :   \
   IS_DREG_REG(REGNO) ? D_REGS :   \
   IS_WREG_REG(REGNO) ? W_REGS :   \
   IS_SFR_REG(REGNO) ? SFR_REGS :  \
   IS_FLP_REG(REGNO) ? FLP_REGS : \
   IS_SINK_REG(REGNO) ? SINK_REGS : \
   ALL_REGS)

/*
 * USED CONSTRAINT LETTERS:
  
 =   - result/ output
 +   - read/write
 &   - early clobber
 *   - ignore constraint
 ?   - disparage
 %   - commutative
 !   - disparage more
 #   - ignore this alternative for reloading
 <   - pre/post decrement
 >   - pre/post increment
 0-9 - match opnd n
 a   - is the class of PIC-compatible accumulator registers (W0).
 A   - unused
 b   - is the class of divide support registers (W1).
 B   - is the class of blank sink registers used in DSP instructions.
 c   - is the class of multiply support registers (W2).
 C   - is the class of multiply support registers 32bit (W2-W3).
 d   - is the class of general-purpose data registers (W1..W14).
 D   - is the class of even general-purpose data registers (W0..W12).
 e   - is the class of non-divide support general registers (W2..W14).
 E   - const double vector
 f   - various fixed values
     - fA => A reg
     - fB => B reg
     - fO => 0.0
 F   - const double vector
 g   - general operand
 G   - const double
 h   - const int matching PN for P32UMM
 H   - const double
 i   - integer
 I   - const int matching CONST_tK_FOR_CONSTRAINT_P
 j   - various immediate int values - perhaps we can move some of the others
          to this and free up some letters?
     - i<mode> -> an uint that fits in mode (if its negative, its negated first)
 J   - const int matching CONST_OK_FOR_CONSTRAINT_P
 k   - unused
 K   - const int matching CONST_OK_FOR_CONSTRAINT_P
 l   - is the class of floating point registers (F0..F31)
 L   - const int matching CONST_OK_FOR_CONSTRAINT_P
 m   - memory operand
 M   - const int matching CONST_OK_FOR_CONSTRAINT_P
 n   - const int or const double
 N   - const int matching CONST_OK_FOR_CONSTRAINT_P
 o   - offsetable memory operand
 O   - const int matching CONST_OK_FOR_CONSTRAINT_P
 p   - pointer
 P   - const int matching CONST_OK_FOR_CONSTRAINT_P
 q   - synonym for 'qs'
 qs  - internal memory operand that fits pic30_symbolic_address_operand
 qm  - internal memory operand consisting of (MEM: q)
 Q   - internal memory operand consisting of a base plus displacement
 r   - general register
 R   - internal memory operand consisting of a base only
 s   - cosnt int or const double
 S   - internal memory operand consisting of a base plus index
 t   - is the class of very restricted product registes (W7)
 T   - internal memory operand consisting of a base plus index
 u   - is the class of restricted product registers (W4-W6)
 U   - internal near memory constraint
 v   - is the class of AWB registers (W13).
 V   - non-offsetable memory operand
 w   - is the class of ACCumulator registers (A..B).
 W   - const int matching CONST_OK_FOR_CONSTRAINT_P [C30]
 x   - is the class of x prefetch registers (W8..W9).
 X   - always a winner
 y   - is the class of y prefetch registers (W10..W11).
 Y   - const int matching CONST_OK_FOR_CONSTRAINT_P [C30]
 za  - is the class of mac product registers (W4..W7).
 zs  - is the class of mac signed product registers (W5,W7).
 zu  - is the class of mac unsigned product registers (W4,W6).
 Z   - const int matching CONST_OK_FOR_CONSTRAINT_P [C30]

 *
 */

/*
** Return the maximum number of consecutive registers
** needed to represent mode MODE in a register of class CLASS.
** On the dsPIC, this is the size of MODE in words,
** since class doesn't make any difference.
*/
#define CLASS_MAX_NREGS(CLASS,MODE) \
   pic30_class_max_nregs(CLASS,MODE)

/*
   ((GET_MODE_SIZE (MODE) + UNITS_PER_WORD - 1) / UNITS_PER_WORD)
*/

/*
** Return the register class of a scratch register needed to load
** or store a register of class CLASS in MODE.
** For dsPIC, we can only load/store QImode using WREG.
*/
/*
#define SECONDARY_RELOAD_CLASS(CLASS, MODE, X)         \
   pic30_secondary_reload_class(CLASS, MODE, X)
*/

/*
** On some machines, it is risky to let hard registers live across arbitrary
** insns. Typically, these machines have instructions that require values to
** be in specific registers (like an accumulator), and reload will fail if the
** required hard register is used for another purpose across such an insn.
** Define SMALL_REGISTER_CLASSES to be an expression with a non-zero value on
** these machines. When this macro has a non-zero value, the compiler will try
** to minimize the lifetime of hard registers. It is always safe to define this
** macro with a non-zero value, but if you unnecessarily define it, you will
** reduce the amount of optimizations that can be performed in some cases.
** If you do not define this macro with a non-zero value when it is required,
** the compiler will run out of spill registers and print a fatal error
** message. For most machines, you should not define this macro at all.
#define   SMALL_REGISTER_CLASSES   1
*/

/************************************************************************/
/* Macros to check register numbers against specific register classes.  */
/************************************************************************/

/*
** These assume that REGNO is a hard or pseudo reg number.
** They give nonzero only if REGNO is a hard reg of the suitable class
** or a pseudo reg currently allocated to a suitable hard reg.
** Since they use reg_renumber, they are safe only once reg_renumber
** has been allocated, which happens in local-alloc.c. 
*/

#define   IS_BASE_REG(REGNO) ((WR0_REGNO <= (REGNO)) && \
                              ((REGNO) < pic30_num_regs))
#define   IS_INDX_REG(REGNO) ((WR0_REGNO <= (REGNO)) && \
                              ((REGNO) < pic30_num_regs))

#define REGNO_OK_FOR_BASE_P(REGNO)  \
 (IS_BASE_REG(REGNO) || IS_BASE_REG(reg_renumber[REGNO]))
#define REGNO_OK_FOR_INDEX_P(REGNO) \
 (IS_INDX_REG(REGNO) || IS_INDX_REG(reg_renumber[REGNO]))

/*
** Maximum number of registers that can appear in a valid memory address.
*/
#define MAX_REGS_PER_ADDRESS 2

/*
** Given an rtx X being reloaded into a reg required to be
** in class CLASS, return the class of reg to actually use.
** In general this is just CLASS; but on some machines
** in some cases it is preferable to use a more restrictive class.
*/
#define PREFERRED_RELOAD_CLASS(X,CLASS) \
   pic30_preferred_reload_class(X,CLASS)

/*
** How to refer to registers in assembler output.
** This sequence is indexed by compiler's hard-register-number (see above).
*/
#define REGISTER_NAMES                                                   \
{                                                                        \
 "w0", "w1", "w2", "w3", "w4", "w5", "w6", "w7",                         \
 "w8", "w9","w10","w11","w12","w13","w14","w15",                         \
 "f0", "f1", "f2", "f3", "f4", "f5", "f6", "f7",                         \
 "f8", "f9","f10","f11","f12","f13","f14","f15",                         \
 "f16", "f17", "f18", "f19", "f20", "f21", "f22", "f23",                 \
 "f24", "f25", "f26", "f27", "f28", "f28", "f30", "f31",                 \
 "RCOUNT", "A", "B", "PSVPAG", "PMADDR", "PMMODE", "PMDIN1", "PMDIN2",   \
 "DSWPAG","SINK0", "SINK1", "SINK2", "SINK3", "SINK4", "SINK5", "SINK6", \
 "SINK7", "SATA", "SATB", "RND", "CORCON", "FSR", "USR"                  \
}

/* Constraint matching */

/* min(k) in [Wn+k] addressing   */
#define   PIC30_DISP_MIN  (pic30_isa32_target() ? -2048 :  -512)   
#define   PIC30_DISP_MAX  (pic30_isa32_target() ? 2047 :  511) 
#define   PIC30_LNK_MAX   (pic30_isa32_target() ? 65532 : 16382)   
#define   PIC30_ADD_MAX   (pic30_isa32_target() ? 65535 :  511)   

/************************************************************************/

/* Stack layout; function entry, exit and calling.  */

/*
** Define this if pushing a word on the stack
** makes the stack pointer a smaller address.
#define STACK_GROWS_DOWNWARD   1
*/

/*
** We use post increment on the dsPIC because the
** stack grows upward (toward higher addresses). This means that we
** assume the stack pointer always points at the next
** FREE location on the stack.
*/
#define STACK_PUSH_CODE POST_INC

#define STACK_POP_CODE PRE_DEC

/*
** Define this if the addresses of local variable slots
** are at negative offsets from the frame pointer.
** For the dsPIC, they are not.
#define FRAME_GROWS_DOWNWARD   1
*/

/*
** Define this if successive args to a function occupy decreasing addresses
** on the stack. 
** For the dsPIC, this is defined, since argumenst are pushed
** in the conventional right-to-left order, meaning that successive arguments
** (in left-to-right order) occupy decreasing addresses.
*/
#define ARGS_GROW_DOWNWARD   1

/*
** Offset from the stack pointer register to the first location at which
** outgoing arguments are placed. If not specified, the default value of
** zero is used. This is the proper value for most machines.
** If ARGS_GROW_DOWNWARD, this is the offset to the location above the
** first location at which outgoing arguments are placed.
*/
#define   STACK_POINTER_OFFSET   0

/*
** Offset of first parameter from the argument pointer register value.
** If ARGS_GROW_DOWNWARD, this is the offset to the location above the
** first argument's address (which is the case for dsPIC).

** 
** traditional dsPIC note:
** FP points to the 1st local. Let n be the size of the auto/temp area.
** [FP+0] = 1st auto/temp
** [FP-2] = old FP
** [FP-4] = return address MSW
** [FP-6] = return address LSW
** [FP-8] = 1st parameter

** dsPIC33A+ dsPIC note:
** FP points to the 1st local. Let n be the size of the auto/temp area.
** [FP+0] = 1st auto/temp
** [FP-4] = old FP
** [FP-8] = return address
** [FP-10] = 1st parameter
*/

#define FIRST_PARM_OFFSET(FNDECL) (pic30_isa32_target() ? -8 : -6)

/*
** Offset of the CFA from the argument pointer register value.
** The CFA (canonical frame address) is defined to be the value of the
** stack pointer in the previous frame, just before the call.
** Hence, 6, or 8.
*/
#define ARG_POINTER_CFA_OFFSET(FNDECL) (pic30_isa32_target() ? -8 :  -6)

/*
** If STACK_CHECK_BUILTIN is zero, and you define a named pattern called
** check_stack in your 'md' file, GCC will call that pattern with one
** argument which is the address to compare the stack value against.
** You must arrange for this pattern to report an error if the stack
** pointer is out of range.
** For dsPIC, stack checking is done in hardware.
*/
#define   STACK_CHECK_BUILTIN   1

/*
** The number of bytes of stack needed to recover from a stack overflow,
** for languages where such a recovery is supported. The default value of
** 75 words should be adequate for most machines.
*/
#define STACK_CHECK_PROTECT   0

/*
** The maximum size of a stack frame, in bytes. GCC will generate
** probe instructions in non-leaf functions to ensure that at least
** this many bytes of stack are available. If a stack frame is larger
** than this size, stack checking will not be reliable and GCC will issue
** a warning. The default is chosen so that GCC only generates one instruction
** on most systems. You should not normally change the value of this macro.
*/
#define   STACK_CHECK_MAX_FRAME_SIZE   512

/*
** Given an rtx for the address of a frame,
** return an rtx for the address of the word in the frame
** that holds the dynamic chain--the previous frame's address.
*/
#define DYNAMIC_CHAIN_ADDRESS(frameaddr) \
  gen_rtx_PLUS (Pmode, frameaddr, GEN_INT (-1 * UNITS_PER_WORD))

/*
** A C expression whose value is RTL representing the value of the
** return address for the frame count steps up from the current frame,
** after the prologue. FRAMEADDR is the frame pointer of the COUNT frame.
** dsPIC return addresses are canonically located at [FP-6].
*/
#define RETURN_ADDR_RTX(count, frameaddr) \
   pic30_return_addr_rtx(count, frameaddr)

/*
** A C expression whose value is RTL representing the location of the
** incoming return address at the begining of any function, before
** the prologue.
** For dsPIC, before the prologue, the return address is [SP-4] or [SP-2].
*/
#define INCOMING_RETURN_ADDR_RTX \
  gen_rtx_PLUS(Pmode, gen_rtx_REG(Pmode, SP_REGNO), \
               GEN_INT((pic30_isa32_target() ? -1 : -2) * UNITS_PER_WORD))

/*
** A C expression whose value is an integer giving the offset, in bytes,
** from the value of the stack pointer register to the top of the stack frame
** at the beginning of any function, before the prologue. The top of the frame
** is defined to be the value of the stack pointer in the previous frame, just
** before the call instruction. You only need to define this macro if you want
** to support call frame debugging information like that provided by DWARF 2.
*/
#define INCOMING_FRAME_SP_OFFSET -4

/*
** If we generate an insn to push BYTES bytes,
** this says how many the stack pointer really advances by.
** For dsPIC [classic], this is always an even number.
**     dsPIC [isa32], this is always a multiple of 4
*/

#define PUSH_ROUNDING(BYTES) pic30_push_rounding(BYTES)

/* Define this macro if functions should assume that stack space has
   been allocated for arguments even when their values are passed in
   registers.
   Size, in bytes, of the area reserved for arguments passed in
   registers for the function represented by FNDECL. 
   #define REG_PARM_STACK_SPACE(FNDECL) 14 */

/* Define this if it is the responsibility of the caller to allocate
   the area reserved for arguments passed in registers. 
   #define OUTGOING_REG_PARM_STACK_SPACE */

/*
** Define how to find the value returned by a function.
** VALTYPE is the data type of the value (as a tree).
** If the precise function being called is known, FUNC is its FUNCTION_DECL;
** otherwise, FUNC is 0.
*/


/*
** Define how to find the value returned by a library function
** assuming the value has mode MODE.
*/

#define LIBCALL_VALUE(MODE)  \
   ((pic30_isa32_target() && (((MODE) == SFmode) || ((MODE) == DFmode))) ? \
     gen_rtx_REG(MODE, F0_REGNO) /* Return in F0 */ :                      \
     gen_rtx_REG(MODE, WR0_REGNO) /* Return in W0 */)

/* Implicit Calls to Library Routines  */


/* 1 if N is a possible register number for a function value. */

#define FUNCTION_VALUE_REGNO_P(N)  \
        (((N) == WR0_REGNO) || \
         (pic30_isa32_target() && ((N) == F0_REGNO)))

/* 1 if the tree TYPE should be returned in memory instead of in regs. 
   #define RETURN_IN_MEMORY(TYPE) \
   (int_size_in_bytes(TYPE) > 12)
*/

/* Define this if PCC uses the nonreentrant convention for returning
   structure and union values. 
   #define PCC_STATIC_STRUCT_RETURN  */

/************************************************************************/

/*
** Define a data type for recording info about an argument list
** during the scan of that argument list.  This data type should
** hold all necessary information about the function itself
** and about the args processed so far, enough to enable macros
** such as FUNCTION_ARG to determine where the next arg should go.
**
** For dsPIC, this is a single integer, which is a number of words
** of arguments scanned so far.
*/
typedef struct pic30_args
{
   int   parmregs[32];
} CUMULATIVE_ARGS;

/*
** Initialize a variable CUM of type CUMULATIVE_ARGS
** for a call to a function whose data type is FNTYPE.
** For a library call, FNTYPE is 0.
**
** For dsPIC, the offset starts at 0.
*/
/* CW N_NAMED_ARGS is a new parameter */
#define INIT_CUMULATIVE_ARGS(CUM,FNTYPE,LIBNAME,INDIRECT,N_NAMED_ARGS) \
  (pic30_init_cumulative_args (&CUM, FNTYPE, LIBNAME))

/* 1 if N is a possible register number for function argument passing. */

#define FUNCTION_ARG_REGNO_P(N)  pic30_is_valid_param_reg(N)


/*
** Define the following macro if function calls on the target machine
** do not preserve any registers; in other words, if `CALL_USED_REGISTERS'
** has 1 for all registers. This macro enables `-fcaller-saves' by
** default. Eventually that option will be enabled by default on all
** machines and both the option and this macro will be eliminated.
#define DEFAULT_CALLER_SAVES
*/

/*
** A C statement or compound statement to output to file some assembler code
** to call the profiling subroutine mcount. Before calling, the assembler code
** must load the address of a counter variable into a register where mcount
** expects to find the address. The name of this variable is `LP' followed by
** the number labelno, so you would generate the name using `LP%d' in a
** fprintf. 
** The details of how the address should be passed to mcount are determined by
** your operating system environment, not by GNU CC. To figure them out,
** compile a small program for profiling using the system's installed C
** compiler and look at the assembler code that results. 
*/
#define FUNCTION_PROFILER(FILE, LABELNO) pic30_function_profiler(FILE, LABELNO)

/*
** Define this macro as a C expression that is nonzero if the return
** instruction or the function epilogue ignores the value of the stack pointer;
** in other words, if it is safe to delete an instruction to adjust the stack
** pointer before a return from the function. Note that this macro's value is
** relevant only for functions for which frame pointers are maintained. It is
** never safe to delete a final stack adjustment in a function that has no
** frame pointer, and the compiler knows this regardless of EXIT_IGNORE_STACK.
*/
#define EXIT_IGNORE_STACK 0

/*
** If defined, this macro specifies a table of register pairs used to eliminate
** unneeded registers that point into the stack frame. If it is not defined,
** the only elimination attempted by the compiler is to replace references to
** the frame pointer with references to the stack pointer. The definition of
** this macro is a list of structure initializations, each of which specifies
** an original and replacement register. On some machines, the position of the
** argument pointer is not known until the compilation is completed. In such a
** case, a separate hard register must be used for the argument pointer. This
** register can be eliminated by replacing it with either the frame pointer or
** the argument pointer, depending on whether or not the frame pointer has been
** eliminated.
*/
#define ELIMINABLE_REGS { \
   { ARG_POINTER_REGNUM, STACK_POINTER_REGNUM },  \
   { FRAME_POINTER_REGNUM, STACK_POINTER_REGNUM } }

/*
** This macro is similar to INITIAL_FRAME_POINTER_OFFSET.
** It specifies the initial difference between the specified pair of registers.
** This macro must be defined if ELIMINABLE_REGS is defined.
*/
#define INITIAL_ELIMINATION_OFFSET(FROM, TO, OFFSET) \
   (OFFSET) = pic30_initial_elimination_offset((FROM), (TO))

/*
** Length in units of the trampoline for entering a nested function.
*/
#define TRAMPOLINE_SIZE 2

/************************************************************************/
/* Addressing modes, and classification of registers for them.      */
/************************************************************************/

/*
** A C expression that is non-zero if the machine supports the addressing mode
*/
#define HAVE_POST_INCREMENT   1
#define HAVE_POST_DECREMENT   1 
#define HAVE_PRE_INCREMENT   1
#define HAVE_PRE_DECREMENT   1
/*
** A C expression that is non-zero if the machine supports 
** pre- or post-address side-effect generation involving constants
** other than the size of the memory operand.
*/
#define HAVE_PRE_MODIFY_DISP   0
#define HAVE_POST_MODIFY_DISP   0

/*
** Recognize any constant value that is a valid address.
*/
#define CONSTANT_ADDRESS_P(X)  CONSTANT_P(X)

/*
** The macros REG_OK_FOR..._P assume that the arg is a REG rtx
** and check its validity for a certain class.
** We have two alternate definitions for each of them.
** The usual definition accepts all pseudo regs; the other rejects
** them unless they have been allocated suitable hard regs.
** The symbol REG_OK_STRICT causes the latter definition to be used.
**
** Most source files want to accept pseudo regs in the hope that
** they will get allocated to the class that the insn wants them to be in.
** Source files for reload pass need to be strict.
** After reload, it makes no difference, since pseudo regs have
** been eliminated by then.
*/

#ifdef REG_OK_STRICT

/* Nonzero if X is a hard reg that can be used as an index.  */
#define REG_OK_FOR_INDEX_P(X) REGNO_OK_FOR_INDEX_P(REGNO(X))
/* Nonzero if X is a hard reg that can be used as a base reg.  */
#define REG_OK_FOR_BASE_P(X)  REGNO_OK_FOR_BASE_P(REGNO(X))

#else

/*
** Nonzero if X is a hard reg that can be used as an index
** or if it is a pseudo reg.
*/
#define REG_OK_FOR_INDEX_P(X) (REGNO(X) >= 0)

/*
** Nonzero if X is a hard reg that can be used as a base reg
** or if it is a pseudo reg.
*/
#define REG_OK_FOR_BASE_P(X) (REGNO(X) >= 0)

#endif

/*
** Define this macro if references to a symbol must be treated
** differently depending on something about the variable or
** function named by the symbol (such as what section it is in).
**
** The macro definition, if any, is executed immediately after the
** rtl for DECL or other node is created.
** The value of the rtl will be a `mem' whose address is a
** `symbol_ref'.
**
** The usual thing for this macro to do is to a flag in the
** `symbol_ref' (such as `SYMBOL_REF_FLAG') or to store a modified
** name string in the `symbol_ref' (if one bit is not enough
** information).
**
** On the dsPIC we use this to indicate if a symbol is in
** code or data or sfr space.
*/

/* the flags may be any length if surrounded by | */
#define PIC30_EXTENDED_FLAG  "|"
#define PIC30_PROG_FLAG       PIC30_EXTENDED_FLAG "pm"      PIC30_EXTENDED_FLAG
#define PIC30_DATA_FLAG       PIC30_EXTENDED_FLAG "dm"      PIC30_EXTENDED_FLAG
#define PIC30_X_FLAG          PIC30_EXTENDED_FLAG "xm"      PIC30_EXTENDED_FLAG
#define PIC30_Y_FLAG          PIC30_EXTENDED_FLAG "ym"      PIC30_EXTENDED_FLAG
#define PIC30_EXT_FLAG        PIC30_EXTENDED_FLAG "ext"     PIC30_EXTENDED_FLAG
#define PIC30_PMP_FLAG        PIC30_EXTENDED_FLAG "pmp"     PIC30_EXTENDED_FLAG
#define PIC30_APSV_FLAG       PIC30_EXTENDED_FLAG "apsv"    PIC30_EXTENDED_FLAG
#define PIC30_PRST_FLAG       PIC30_EXTENDED_FLAG "persist" PIC30_EXTENDED_FLAG
#define PIC30_PSV_FLAG        PIC30_EXTENDED_FLAG "psv"     PIC30_EXTENDED_FLAG
#define PIC30_EE_FLAG         PIC30_EXTENDED_FLAG "ee"      PIC30_EXTENDED_FLAG
#define PIC30_BSS_FLAG        PIC30_EXTENDED_FLAG "bss"     PIC30_EXTENDED_FLAG

#define PIC30_MERGE_FLAG      PIC30_EXTENDED_FLAG "mrg"     PIC30_EXTENDED_FLAG
#define PIC30_NOLOAD_FLAG     PIC30_EXTENDED_FLAG "nl"      PIC30_EXTENDED_FLAG
#define PIC30_ALGN_FLAG       PIC30_EXTENDED_FLAG "a"       PIC30_EXTENDED_FLAG
#define PIC30_RALGN_FLAG      PIC30_EXTENDED_FLAG "ra"      PIC30_EXTENDED_FLAG
#define PIC30_ADDR_FLAG       PIC30_EXTENDED_FLAG "addr"    PIC30_EXTENDED_FLAG

#define PIC30_FCNN_FLAG       PIC30_EXTENDED_FLAG "Nf"      PIC30_EXTENDED_FLAG
#define PIC30_FCNS_FLAG       PIC30_EXTENDED_FLAG "Sf"      PIC30_EXTENDED_FLAG
#define PIC30_EDS_FLAG        PIC30_EXTENDED_FLAG "eds"     PIC30_EXTENDED_FLAG
#define PIC30_PAGE_FLAG       PIC30_EXTENDED_FLAG "pge"     PIC30_EXTENDED_FLAG
#define PIC30_SFR_FLAG        PIC30_EXTENDED_FLAG "sfr"     PIC30_EXTENDED_FLAG
#define PIC30_NEAR_FLAG       PIC30_EXTENDED_FLAG "near"    PIC30_EXTENDED_FLAG
#define PIC30_DMA_FLAG        PIC30_EXTENDED_FLAG "dma"     PIC30_EXTENDED_FLAG
#define PIC30_BOOT_FLAG       PIC30_EXTENDED_FLAG "boot"
#define PIC30_SECURE_FLAG     PIC30_EXTENDED_FLAG "sec"
#define PIC30_AUXFLASH_FLAG   PIC30_EXTENDED_FLAG "aux"     PIC30_EXTENDED_FLAG
#define PIC30_AUXPSV_FLAG     PIC30_EXTENDED_FLAG "xpsv"    PIC30_EXTENDED_FLAG
#define PIC30_PACKEDFLASH_FLAG PIC30_EXTENDED_FLAG "pf"     PIC30_EXTENDED_FLAG
#define PIC30_KEEP_FLAG       PIC30_EXTENDED_FLAG "keep"    PIC30_EXTENDED_FLAG
#define PIC30_QLIBFN_FLAG     PIC30_EXTENDED_FLAG "qlib"    PIC30_EXTENDED_FLAG
#define PIC30_DATAFLASH_FLAG  PIC30_EXTENDED_FLAG "df"      PIC30_EXTENDED_FLAG
#define PIC30_CO_SHARED_FLAG  PIC30_EXTENDED_FLAG "cshrd"   PIC30_EXTENDED_FLAG
#define PIC30_PRESERVED_FLAG  PIC30_EXTENDED_FLAG "prsrv"   PIC30_EXTENDED_FLAG
#define PIC30_PRIORITY_FLAG   PIC30_EXTENDED_FLAG "prity"   PIC30_EXTENDED_FLAG
#define PIC30_UPDATE_FLAG     PIC30_EXTENDED_FLAG "upd"     PIC30_EXTENDED_FLAG

#define PIC30_SFR_NAME_P(NAME) (strstr(NAME, PIC30_SFR_FLAG))
#define PIC30_PGM_NAME_P(NAME) (strstr(NAME, PIC30_PROG_FLAG))
#define PIC30_FCNN_NAME_P(NAME) (strstr(NAME, PIC30_FCNN_FLAG))
#define PIC30_FCNS_NAME_P(NAME) (strstr(NAME, PIC30_FCNS_FLAG))
#define PIC30_FCN_NAME_P(NAME) (PIC30_FCNS_NAME_P(NAME) || \
                                 PIC30_FCNN_NAME_P(NAME))

#define PIC30_IS_NAME_P(NAME,IS) (strncmp(NAME, IS, sizeof(IS)-1) == 0)
#define PIC30_HAS_NAME_P(NAME,HAS) (strstr(NAME, HAS))

#define ENCODED_NAME_P(SYMBOL_NAME) \
  ((SYMBOL_NAME[0] == PIC30_EXTENDED_FLAG[0]) ? \
    (strrchr(SYMBOL_NAME,PIC30_EXTENDED_FLAG[0]) - SYMBOL_NAME) + 1 : 0)

/*
** Decode SYM_NAME and store the real name part in VAR, sans
** the characters that encode section info.  Define this macro if
** ENCODE_SECTION_INFO alters the symbol's name string.
*/
/* Note that we have to handle symbols like "%*start".  */
/*
#define STRIP_NAME_ENCODING(VAR, SYMBOL_NAME) \
 * now defined as a function
   const char *pic30_strip_name_encoding(const char *);
*/
/************************************************************************/

/*
** An alias for a machine mode name.
** This is the machine mode that elements of a jump-table should have.
*/
#define CASE_VECTOR_MODE SImode

/*
** Define as C expression which evaluates to nonzero if the tablejump
** instruction expects the table to contain offsets from the address of the
** table.
** Do not define this if the table should contain absolute addresses.
#define CASE_VECTOR_PC_RELATIVE 1
*/

/*
** Define this to be the smallest number of different values for which it
** is best to use a jump-table instead of a tree of conditional branches.
** The default is 4 for machines with a casesi instruction and 5 otherwise.
** For dsPIC, it may be best always to use a tree.
*/
#define CASE_VALUES_THRESHOLD (10)

/*
** Define this as 1 if `char' should by default be signed; else as 0.
*/
#define DEFAULT_SIGNED_CHAR 1

/*
** Max number of bytes we can move from memory to memory
** in one reasonably fast instruction.
*/
#define MOVE_MAX 4

/*
** If a memory-to-memory move would take MOVE_RATIO or more simple
** move-instruction pairs, we will do a movstr or libcall instead.
*/
#define MOVE_RATIO(optimize_size) 4

/*
** Define this if zero-extension is slow (more than one real instruction).
#define SLOW_ZERO_EXTEND
*/

/*
** Nonzero if access to memory by bytes is slow and undesirable.
*/
#define SLOW_BYTE_ACCESS 1

/* Define if shifts truncate the shift count
   which implies one can omit a sign-extension or zero-extension
   of a shift count. */
/* #define SHIFT_COUNT_TRUNCATED 1 */

/*
** We assume that the store-condition-codes instructions store 0 for false
** and some other value for true.  This is the value stored for true.
*/
#define STORE_FLAG_VALUE 1

/*
** When a prototype says `char' or `short', really pass an `int'.
#define PROMOTE_PROTOTYPES
*/

/*
** Specify the machine mode that pointers have.
** After generation of rtl, the compiler makes no further distinction
** between pointers and any other objects of this machine mode.
*/
/*
** Width in bits of a pointer.
** See also the macro `Pmode' defined below.
*/
#define TARGET_EDS_MODE P32UMMmode
extern unsigned int pic30_pointer_size;
extern scalar_int_mode pic30_pmode;
extern scalar_int_mode machine_Pmode;

#define POINTER_SIZE      pic30_pointer_size
#define Pmode             pic30_pmode

#if 1
#define STACK_Pmode machine_Pmode
#define FN_Pmode    machine_Pmode
#define TARGET_CONSTANT_PMODE (TARGET_TRACK_PSVPAG ? P16APSVmode : Pmode)
#endif


/*
** A function address in a call instruction is a byte address
** (for indexing purposes)
*/
#define FUNCTION_MODE QImode

#define REGISTER_TARGET_PRAGMAS()                        \
  do {                                                   \
    pic30_register_target_pragmas();                     \
  } while(0)

extern void pic30_cpu_cpp_builtins(void *);

#define TARGET_CPU_CPP_BUILTINS() pic30_cpu_cpp_builtins(pfile)

/*
** A C expression for the cost of moving data of mode MODE between a register
** of class CLASS and memory; IN is zero if the value is to be written to
** memory, non-zero if it is to be read in. This cost is relative to those in
** REGISTER_MOVE_COST. If moving between registers and memory is more expensive
** than between two registers, you should define this macro to express the
** relative cost. 
** If you do not define this macro, GNU CC uses a default cost of 4 plus the
** cost of copying via a secondary reload register, if one is needed. If your
** machine requires a secondary reload register to copy between memory and a
** register of class but the reload mechanism is more complex than copying via
** an intermediate, define this macro to reflect the actual cost of the move. 
**
** GNU CC defines the function memory_move_secondary_cost if secondary reloads
** are needed. It computes the costs due to copying via a secondary register.
** If your machine copies from memory using a secondary register in the
** conventional way but the default base value of 4 is not correct for your
** machine, define this macro to add some other value to the result of that
** function. The arguments to that function are the same as to this macro
**
** Memory move cost is double that of register to register.
*/

/*
** Branches costs.
** A C expression for the cost of a branch instruction.
** A value of 1 is the default; other values are interpreted relative to that. 
** dsPIC is typical.
*/
#define BRANCH_COST(speed,predictable)         1

/* A C statement (sans semicolon) to update the integer variable COST
** based on the relationship between INSN that is dependent on
** DEP_INSN through the dependence LINK.  The default is to make no
** adjustment to COST.
#define ADJUST_COST(INSN,LINK,DEP_INSN,COST)                          \
  (COST) = pic30_adjust_cost (INSN, LINK, DEP_INSN, COST)
*/

/*
** Define this macro to inhibit strength reduction of memory
** addresses.  (On some machines, such strength reduction seems to do
** harm rather than good.)
#define DONT_REDUCE_ADDR
*/

/*
** Define this macro if it is good or better to call a constant function
** address than to call an address kept in a register.
*/
#define NO_FUNCTION_CSE 1

/*
** A C compound statement to set the components of cc_status appropriately
** for an insn insn whose body is exp. It is this macro's responsibility to
** recognize insns that set the condition code as a byproduct of other activity
** as well as those that explicitly set (cc0). This macro is not used on
** machines that do not use cc0. If there are insns that do not set the
** condition code but do alter other machine registers, this macro must check
** to see whether they invalidate the expressions that the condition code is
** recorded as reflecting. For example, on the 68000, insns that store in
** address registers do not set the condition code, which means that usually
** NOTICE_UPDATE_CC can leave cc_status unaltered for such insns. But suppose
** that the previous insn set the condition code based on location `a4@(102)'
** and the current insn stores a new value in `a4'. Although the condition code
** is not changed by this, it will no longer be true that it reflects the
** contents of `a4@(102)'. Therefore, NOTICE_UPDATE_CC must alter cc_status in
** this case to say that nothing is known about the condition code value. The
** definition of NOTICE_UPDATE_CC must be prepared to deal with the results of
** peephole optimization: insns whose patterns are parallel RTXs containing
** various reg, mem or constants which are just the operands. The RTL structure
** of these insns is not sufficient to indicate what the insns actually do.
** What NOTICE_UPDATE_CC should do when it sees one is just to run
** CC_STATUS_INIT. A possible definition of NOTICE_UPDATE_CC is to call a
** function that looks at an attribute (see section Instruction Attributes)
** named, for example, `cc'. This avoids having detailed information about
** patterns in two places, the `md' file and in NOTICE_UPDATE_CC.
*/
#define NOTICE_UPDATE_CC(EXP, INSN) pic30_notice_update_cc(EXP, INSN)

/*
** Return non-zero if SELECT_CC_MODE will never return MODE for a
** floating point inequality comparison.
*/
#define REVERSIBLE_CC_MODE(MODE) (1)

/*
** A C expression to initialize the mdep field to "empty".
** The default definition does nothing, since most machines don't use the field
** anyway. If you want to use the field, you should probably define this macro
** to initialize it. 
** This macro is not used on machines that do not use cc0
*/
#define CC_STATUS_MDEP_INIT cc_status.mdep = 0

/*
** One some machines not all possible comparisons are defined, but you can
** convert an invalid comparison into a valid one. For example, the Alpha does
** not have a GT comparison, but you can use an LT comparison instead and swap
** the order of the operands. 
** On such machines, define this macro to be a C statement to do any required
** conversions. code is the initial comparison code and op0 and op1 are the
** left and right operands of the comparison, respectively. You should modify
** code, op0, and op1 as required. 
**
** GNU CC will not assume that the comparison resulting from this macro is
** valid but will see if the resulting insn matches a pattern in the `md' file. 
**
** You need not define this macro if it would never change the comparison code
** or operands.
*/

#define CANONICALIZE_COMPARISON(CODE,OP0,OP1)         \
if (pic30_near_operand(OP1, GET_MODE(OP1)))         \
{                        \
     rtx tmp = OP0; OP0 = OP1 ; OP1 = tmp;         \
     CODE = swap_condition (CODE);            \
}

/************************************************************************/
/* Macros for SDB and DWARF Output                        */
/************************************************************************/

/*
** A C expression that returns the type of debugging output GCC should produce
** when the user specifies just `-g'. Define this if you have arranged for GCC
** to support more than one format of debugging output. Currently, the
** allowable values are DBX_DEBUG, SDB_DEBUG, DWARF_DEBUG, DWARF2_DEBUG,
** and XCOFF_DEBUG. When the user specifies `-ggdb', GCC normally also uses
** the value of this macro to select the debugging output format, but with two
** exceptions.
** If DWARF2_DEBUGGING_INFO is defined and LINKER_DOES_NOT_WORK_WITH_DWARF2 is
** not defined, GCC uses the value DWARF2_DEBUG.
** Otherwise, if DBX_DEBUGGING_INFO is defined, GCC uses DBX_DEBUG.
** The value of this macro only affects the default debugging output;
** the user can always get a specific type of output by using `-gstabs',
** `-gcoff', `-gdwarf-1', `-gdwarf-2', or `-gxcoff'
*/

#if (!PIC30_DWARF2)
#define PREFERRED_DEBUGGING_TYPE      SDB_DEBUG
#endif

/*
** Define if your assembler supports .uleb128.
*/
#if (PIC30_DWARF2)
#if !defined(HAVE_AS_LEB128)
#define HAVE_AS_LEB128
#endif
#endif

/*
** Define this macro if GCC should produce COFF-style debugging output
** for SDB in response to the `-g' option.
*/
#if (!PIC30_DWARF2)
#define SDB_DEBUGGING_INFO   1
#endif

/*
** Define this macro if GCC should produce dwarf version 2 format debugging
** output in response to the `-g' option.
*/
#if (PIC30_DWARF2)
#define   DWARF2_DEBUGGING_INFO   1
#endif

/*
** Some assemblers do not support a semicolon as a delimiter, even between SDB
** assembler directives. In that case, define this macro to be the delimiter to
** use (usually `\n'). It is not necessary to define a new set of PUT_SDB_op
** macros if this is the only change required
*/

#define SDB_DELIM "\n"

/*
** Define this macro to allow references to structure, union, or enumeration
** tags that have not yet been seen to be handled. Some assemblers choke if
** forward tags are used, while some require it.
*/
#define SDB_ALLOW_FORWARD_REFERENCES   1

/*
** Define this macro to be a nonzero value if the assembler can generate
** Dwarf 2 line debug info sections. This will result in much more compact
** line number tables, and hence is desirable if it works.
*/
#define   DWARF2_ASM_LINE_DEBUG_INFO   1

/*
** The size in bytes of a DWARF field indicating an offset or length
** relative to a debug info section, specified to be 4 bytes in the
** DWARF-2 specification.  The SGI/MIPS ABI defines it to be the same
** as PTR_SIZE.
*/
#define DWARF2_ADDR_SIZE   4

/*
** Define this macro to 0 if your target supports DWARF 2 frame unwind
** information, but it does not yet work with exception handling.
** Otherwise, if your target supports this information (if it defines
** INCOMING_RETURN_ADDR_RTX and either UNALIGNED_INT_ASM_OP or
** OBJECT_FORMAT_ELF), GCC will provide a default definition of 1. 
**
** If this macro is defined to 1, the DWARF 2 unwinder will be the default
** exception handling mechanism; otherwise, setjmp/longjmp will be used by
** default. 
**
** If this macro is defined to anything, the DWARF 2 unwinder will be used
** instead of inline unwinders and __unwind_function in the non-setjmp case. 
**
#if defined(DWARF2_UNWIND_INFO)
#undef   DWARF2_UNWIND_INFO
#endif
*/

/*
 *  Debug target hook for starting source file for pic30
 */
#if (!PIC30_DWARF2)
#define SDBOUT_START_SOURCE_FILE pic30_start_source_file
#endif

/*
 *  How to convert a line number to store in sdb_begin_function_line so
 *    that relative offsets may be produced.  dsPIC30 doesn't want relative
 *    offsets; so don't store a valid value into sdb_begin_function_line
 */
#define SDB_BEGIN_FUNCTION_LINE(X) (0)

#define SDB_END_PROLOGUE pic30_sdb_end_prologue


/************************************************************************/
/* Control the assembler format that we output.                         */
/************************************************************************/

#undef SIZE_ASM_OP
#define SIZE_ASM_OP "\t.size\t"

#define GLOBAL_ASM_OP ".global\t"

/*
** We define this to prevent the name mangler from putting dollar signs
** into function names.
*/
#define NO_DOLLAR_IN_LABEL

/*
** Output to assembler file text saying following lines
** may contain character constants, extra white space, comments, etc.
*/
#define ASM_APP_ON   ""

/*
** Output to assembler file text saying following lines
** no longer contain unusual constructs.
*/
#define ASM_APP_OFF   ""

#define NAMED_SECTION_FUNCTION void pic30_no_section() { \
   in_section = in_named;                                \
   in_named_name = ""; }

#define NDATA_SECTION_FUNCTION                             \
void                                                       \
ndata_section ()                                           \
{                                                          \
   if (in_section != in_ndata)                             \
   {                                                       \
      fprintf(asm_out_file, "%s\n", NDATA_SECTION_ASM_OP); \
      in_section = in_ndata;                               \
   }                                                       \
}
      
/*
   CONST_SECTION_ASM_OP has been poisoned 
*/
#define CONST_SECTION_FUNCTION                                \
void                                                          \
const_section ()                                              \
{                                                             \
   if (in_section != in_const)                                \
   {                                                          \
      fprintf(asm_out_file, "%s\n", C30CONST_SECTION_ASM_OP); \
      in_section = in_const;                                  \
   }                                                          \
}
      
#define DCONST_SECTION_FUNCTION                             \
void                                                        \
dconst_section ()                                           \
{                                                           \
   if (in_section != in_dconst)                             \
   {                                                        \
      fprintf(asm_out_file, "%s\n", DCONST_SECTION_ASM_OP); \
      in_section = in_dconst;                               \
   }                                                        \
}
      
#define NDCONST_SECTION_FUNCTION                             \
void                                                         \
ndconst_section ()                                           \
{                                                            \
   if (in_section != in_ndconst)                             \
   {                                                         \
      fprintf(asm_out_file, "%s\n", NDCONST_SECTION_ASM_OP); \
      in_section = in_ndconst;                               \
   }                                                         \
}
      
/*
** On most machines, read-only variables, constants, and jump tables are
** placed in the text section. If this is not the case on your machine,
** this macro should be defined to be the name of a function (either
** data_section or a function defined in EXTRA_SECTIONS) that switches
** to the section to be used for read-only items. If these items should
** be placed in the text section, this macro should not be defined.
*/
#undef READONLY_DATA_SECTION_ASM_OP
#define READONLY_DATA_SECTION_ASM_OP   pic30_const_section_asm_op()

/*
** Output before program init section
*/
#define INIT_SECTION_ASM_OP  "\t.section\t.init"

/*
** Output before program text section
*/
#define TEXT_SECTION_ASM_OP pic30_text_section_asm_op()

/*
** Output before writable data.
*/
#define DATA_SECTION_ASM_OP pic30_data_section_asm_op()

/*
** Output before writable data (near).
*/
#define NDATA_SECTION_ASM_OP pic30_ndata_section_asm_op()

/*
** Output before read-only data in PSV space.
*/
#define C30CONST_SECTION_ASM_OP pic30_const_section_asm_op()

/*
** Output before read-only data in data space.
*/
#define DCONST_SECTION_ASM_OP pic30_dconst_section_asm_op()

/*
** Output before read-only data in near data space.
*/
#define NDCONST_SECTION_ASM_OP pic30_ndconst_section_asm_op()

/*
** A C statement to output DBX or SDB debugging information before code for
** line number line of the current source file to the stdio stream stream. 
** This macro need not be defined if the standard form of debugging information
** for the debugger in use is appropriate. 
*/
/* renamed */
#define SDB_OUTPUT_SOURCE_LINE(STREAM, LINE)         \
          fprintf(STREAM, "\t.ln\t%d\n", LINE)


/*
** How to renumber registers for dbx and gdb.
*/
#define DBX_REGISTER_NUMBER(REGNO) (REGNO == B_REGNO ?  0x14 : REGNO)

/******************  Assembler output formatting  **********************/

/*
** A C statement to output assembler commands which will identify the object
** file as having been compiled with GCC (or another GNU compiler). If you
** don't define this macro, the string `gcc_compiled.:' is output. This string
** is calculated to define a symbol which, on BSD systems, will never be
** defined for any other reason. GDB checks for the presence of this symbol
** when reading the symbol table of an executable. On non-BSD systems, you must
** arrange communication with GDB in some other fashion. If GDB is not used on
** your system, you can define this macro with an empty body.
#define ASM_IDENTIFY_GCC(FILE)  fputs ("gcc2_compiled:\n", FILE)
*/

/*
** A C string constant describing how to begin a comment in the target
** assembler language. The compiler assumes that the comment will end at
** the end of the line.
*/
#define ASM_COMMENT_START  ";"

/*
** Define this macro if you are using an unusual assembler that requires
** different names for the machine instructions. The definition is a C
** statement or statements which output an assembler instruction opcode to the
** stdio stream stream. The macro-operand ptr is a variable of type char *
** which points to the opcode name in its "internal" form--the form that is
** written in the machine description. The definition should output the opcode
** name to stream, performing any translation you desire, and increment the
** variable ptr to point at the end of the opcode so that it will not be output
** twice. In fact, your macro definition may process less than the entire
** opcode name, or more than the opcode name; but if you want to process text
** that includes `%'-sequences to substitute operands, you must take care of
** the substitution yourself. Just be sure to increment ptr over whatever text
** should not be output normally. If you need to look at the operand values,
** they can be found as the elements of recog_operand. If the macro definition
** does nothing, the instruction is output in the usual way.
**
** For dsPIC, the opcode is separated from the operands with a tab.
** If the 1st character is '.', this is a label, not an opcode.
*/
#define ASM_OUTPUT_OPCODE(FILE,PTR) \
    PTR = pic30_print_opcode(FILE,PTR)

/*
** A C statement (sans semicolon) to output to the stdio stream stream any text
** necessary for declaring the name NAME of a function which is being defined.
** This macro is responsible for outputting the label definition (perhaps using
** ASM_OUTPUT_LABEL). The argument decl is the FUNCTION_DECL tree node
** representing the function. 
*/
#undef  ASM_DECLARE_FUNCTION_NAME
#define ASM_DECLARE_FUNCTION_NAME(FILE, NAME, DECL)   \
  pic30_asm_declare_function_name(FILE, NAME, DECL)

/*
** A C statement (sans semicolon) to output to the stdio stream stream any text
** necessary for declaring the name NAME of an initialized variable which is
** being defined. This macro must output the label definition (perhaps using
** ASM_OUTPUT_LABEL). The argument DECL is the VAR_DECL tree node representing
** the variable.  
*/
#undef ASM_DECLARE_OBJECT_NAME
#define ASM_DECLARE_OBJECT_NAME(FILE, NAME, DECL)   \
  pic30_asm_declare_object_name(FILE, NAME, DECL)

/*
** A C expression that specifies the mnemonic for the SET assembler directive.
*/
#define   SET_ASM_OP   "\t.set\t"

/*
** A C statement to be executed just prior to the output of
** assembler code for INSN, to modify the extracted operands so
** they will be output differently.
**
** Here the argument OPVEC is the vector containing the operands
** extracted from INSN, and NOPERANDS is the number of elements of
** the vector which contain meaningful data for this insn.
** The contents of this vector are what will be used to convert the insn
** template into assembler code, so you can change the assembler output
** by changing the contents of the vector.
*/

#define FINAL_PRESCAN_INSN(INSN, OPVEC, NOPERANDS) \
  pic30_final_prescan_insn ((INSN), (OPVEC), (NOPERANDS))

/*
** A C statement (sans semicolon) to output to the stdio stream FILE the
** assembler definition of a label named NAME. Use the expression
** assemble_name (FILE, NAME) to output the name itself; before and after
** that, output the additional assembler syntax for defining the name,
** and a newline.
*/
#define ASM_OUTPUT_LABEL(FILE,NAME)   \
{ assemble_name (FILE, NAME); fputs (":\n", FILE); }

/*
** This is how to output a reference to a user-level label named NAME.
** `assemble_name' uses this.
*/
#define ASM_OUTPUT_LABELREF(FILE, NAME)              \
do {                                                 \
  const char * real_name;                            \
  real_name = pic30_strip_name_encoding ((NAME));    \
  if (((NAME)[0] != '*') && (real_name[0] != '*') && \
      (real_name[0] != '.'))                         \
    asm_fprintf (FILE, "%U", real_name);             \
  asm_fprintf (FILE, "%s", real_name);               \
} while (0)           

/*
** This is how we tell the assembler that a symbol is weak.
** We don't need to do this. Use the one in elfos.h
**  #define ASM_WEAKEN_LABEL(FILE,NAME)   \
**  do {               \
**   fputs ("\t.weak\t", FILE);      \
**   assemble_name(FILE, NAME);      \
**   fputc ('\n', FILE);         \
**  } while (0)
*/

/*
** The prefix to add to user-visible assembler symbols.
*/
#undef USER_LABEL_PREFIX
#define USER_LABEL_PREFIX   "_"
/*
** The prefix to add to immediate operands.
*/
#define IMMEDIATE_PREFIX   "#"
/*
** The prefix to add to local assembler symbols.
*/
#define LOCAL_LABEL_PREFIX   ".L"

/*
** This is how to output an internal numbered label where
** PREFIX is the class of label and NUM is the number within the class.
** A C statement to output to the stdio stream stream a label whose name is
** made from the string prefix and the number num. It is absolutely essential
** that these labels be distinct from the labels used for user-level functions
** and variables. Otherwise, certain programs will have name conflicts with
** internal labels. It is desirable to exclude internal labels from the symbol
** table of the object file. Most assemblers have a naming convention for
** labels that should be excluded; on many systems, the letter `L' at the
** beginning of a label has this effect. You should find out what convention
** your system uses, and follow it.
*/
#undef ASM_GENERATE_INTERNAL_LABEL
#define ASM_GENERATE_INTERNAL_LABEL(BUF,PREFIX,NUM)      \
   sprintf (BUF, "*.%s%d", PREFIX, NUM)

/*
** Define this if the label before a jump-table needs to be output specially.
** The first three arguments are the same as for ASM_OUTPUT_INTERNAL_LABEL;
** the fourth argument is the jump-table which follows (a jump_insn containing
** an addr_vec or addr_diff_vec).
**
** If this macro is not defined, these labels are output with
** ASM_OUTPUT_INTERNAL_LABEL.
*/
#undef ASM_OUTPUT_CASE_LABEL
#define ASM_OUTPUT_CASE_LABEL(STREAM, PREFIX, NUM, TABLE) \
      pic30_asm_output_case_label(STREAM, PREFIX, NUM, TABLE)

/*
** Epilogue for case labels.  This jump instruction is called by casesi
** to transfer to the appropriate branch instruction within the table.
**
** Define this if something special must be output at the end of a jump-table.
** The definition should be a C statement to be executed after the assembler
** code for the table is written. It should write the appropriate code to stdio
** stream STREAM. The argument TABLE is the jump-table insn, and NUM is the
** label-number of the preceding label.
*/

#define ASM_OUTPUT_CASE_END(STREAM, NUM, TABLE)            \
   pic30_asm_output_case_end(STREAM, NUM, TABLE)

/*
** Define this macro to be an expression with a non-zero value if jump tables
** (for tablejump insns) should be output in the text section, along with the
** assembler instructions. Otherwise, the readonly data section is used. This
** macro is irrelevant if there is no separate readonly data section.
*/
#define JUMP_TABLES_IN_TEXT_SECTION 1

/*
** This is how to output an assembler line defining a string constant.
** A C statement to output to the stdio stream stream an assembler instruction
** to assemble a string constant containing the len bytes at ptr. ptr will be
** a C expression of type char * and len a C expression of type int. If the
** assembler has a .ascii pseudo-op as found in the Berkeley Unix assembler,
** do not define the macro ASM_OUTPUT_ASCII.
*/
#undef ASM_OUTPUT_ASCII
#define ASM_OUTPUT_ASCII(FILE, PTR, LEN) \
   pic30_asm_output_ascii(FILE, PTR, LEN)

/*
** A C statement to output to the stdio FILE an assembler instruction
** to assemble an integer of 1 byte. The argument VALUE will be an RTL
** expression which represents a constant value.
*/
#define ASM_OUTPUT_CHAR(FILE,VALUE)  do {   \
   fprintf(FILE, "\t.byte "),      \
   output_addr_const (FILE, (VALUE)),   \
   fprintf(FILE, "\n");         \
  } while (0)

/*
** A C statement to output to the stdio FILE an assembler instruction
** to assemble an integer of 2 bytes. The argument VALUE will be an RTL
** expression which represents a constant value.
*/
#define ASM_OUTPUT_SHORT(FILE,VALUE) pic30_asm_output_short(FILE,VALUE)

/*
** A C statement to output to the stdio FILE an assembler instruction
** to assemble an integer of 4 bytes. The argument VALUE will be an RTL
** expression which represents a constant value.
*/
#define ASM_OUTPUT_INT(FILE,VALUE) do {     \
   fprintf(FILE, "\t.long\t");               \
   output_addr_const(FILE, VALUE);             \
   fprintf(FILE, "\n");                  \
  } while (0)

/*
** This is how to output an assembler line for a numeric constant byte.
*/
#define ASM_OUTPUT_BYTE(FILE,VALUE) do {     \
   fprintf(FILE, "\t.byte\t%d\n", VALUE);            \
  } while (0)

/*
** This is how to output an insn to push a register on the stack.
** It need not be very fast code (used only in profiling).
*/
#define ASM_OUTPUT_REG_PUSH(FILE,REGNO)  \
  fprintf (FILE, "\tmov %s,[w15++]\n", reg_names[REGNO])

/*
** This is how to output an insn to pop a register from the stack.
** It need not be very fast code (used only in profiling).
*/
#define ASM_OUTPUT_REG_POP(FILE,REGNO)  \
   fprintf (FILE, "\tmov [--w15],%s\n", reg_names[REGNO])

/*
** This is how to output an element of a case-vector that is absolute.
*/
#define ASM_OUTPUT_ADDR_VEC_ELT(FILE, VALUE)   \
    asm_fprintf (FILE, "\tbra\t%L%d\n", VALUE);

/*
** This is how to output an element of a case-vector that is relative.
*/
#define ASM_OUTPUT_ADDR_DIFF_ELT(FILE, BODY, VALUE, REL)  \
   fprintf (FILE, "\t.word\t%L%d-%L%d ;addr_diff_elt\n", VALUE,REL)

/*
** This is how to output an assembler line that says to advance the
** location counter to a multiple of 2**LOG bytes.
*/
#define ASM_OUTPUT_ALIGN(FILE,LOG)   \
  pic30_asm_output_align(FILE,LOG)

/*
** A C statement to output to the stdio stream FILE an assembler instruction
** to advance the location counter by SIZE bytes. Those bytes should be zero
** when loaded. SIZE will be a C expression of type int.
*/
#undef ASM_OUTPUT_SKIP
#define ASM_OUTPUT_SKIP(FILE, SIZE) \
   fprintf (FILE, "\t.skip\t%d\n", (SIZE))

/*
** A C statement (sans semicolon) to output to the stdio stream FILE the
** assembler definition of a common-label named NAME whose size is SIZE bytes.
** The variable ROUNDED is the size rounded up to whatever alignment the caller
** wants. Use the expression assemble_name (FILE, NAME) to output the name
** itself; before and after that, output the additional assembler syntax for
** defining the name, and a newline. This macro controls how the assembler
** definitions of uninitialized common global variables are output.
*/
#undef  ASM_OUTPUT_COMMON
#define ASM_OUTPUT_COMMON(FILE, NAME, SIZE, ROUNDED)  \
   pic30_asm_output_common(FILE, NAME, SIZE, ROUNDED)

/*
** Like ASM_OUTPUT_COMMON except takes the required alignment as a separate,
** explicit argument. If you define this macro, it is used in place of
** ASM_OUTPUT_COMMON, and gives you more flexibility in handling the required
** alignment of the variable. The alignment is specified as the number of bits.
*/
#undef ASM_OUTPUT_ALIGNED_DECL
#define ASM_OUTPUT_ALIGNED_DECL_COMMON(FILE, DECL, NAME, SIZE, ALIGN) \
        pic30_asm_output_aligned_common(FILE, DECL, NAME, SIZE, ALIGN)

#undef  ASM_OUTPUT_ALIGNED_COMMON
#define ASM_OUTPUT_ALIGNED_COMMON(FILE, NAME, SIZE, ALIGNMENT)  \
   pic30_asm_output_aligned_common(FILE, 0, NAME, SIZE, ALIGNMENT)

/*
** A C statement (sans semicolon) to output to the stdio stream FILE the
** assembler definition of a local-common-label named NAME whose size is SIZE
** bytes. The variable ROUNDED is the size rounded up to whatever alignment the
** caller wants. Use the expression assemble_name (FILE, NAME) to output the
** name itself; before and after that, output the additional assembler syntax
** for defining the name, and a newline. This macro controls how the assembler
** definitions of uninitialized static variables are output.
*/
#undef ASM_OUTPUT_LOCAL
#define ASM_OUTPUT_LOCAL(FILE, NAME, SIZE, ROUNDED)  \
   pic30_asm_output_local(FILE, NAME, SIZE, ROUNDED)

/*
** Like ASM_OUTPUT_LOCAL except takes the required alignment as a separate,
** explicit argument. If you define this macro, it is used in place of 
** ASM_OUTPUT_LOCAL, and gives you more flexibility in handling the required
** alignment of the variable. The alignment is specified as the number of bits.
*/
#undef ASM_OUTPUT_ALIGNED_LOCAL
#define ASM_OUTPUT_ALIGNED_LOCAL(FILE, NAME, SIZE, ALIGNMENT)  \
   pic30_asm_output_aligned_local(FILE, NAME, SIZE, ALIGNMENT)

/*
** Like ASM_OUTPUT_LOCAL except takes a decl, and the required alignment as 
** a separate, explicit argument. If you define this macro, it is used in 
** place of ASM_OUTPUT_LOCAL, and gives you more flexibility in handling the 
** required alignment of the variable. The alignment is specified as the number
** of bits.
*/
#undef ASM_OUTPUT_ALIGNED_DECL_LOCAL
#define ASM_OUTPUT_ALIGNED_DECL_LOCAL(FILE, DECL, NAME, SIZE, ALIGNMENT)  \
   pic30_asm_output_aligned_decl_local(FILE, DECL, NAME, SIZE, ALIGNMENT)

/*
** Store in OUTPUT a string (made with alloca) containing
** an assembler-name for a local static variable named NAME.
** LABELNO is an integer which is different for each call.
** The magic number 12 includes 1 for the period, 1 for the
** terminating null, and 10 for the maximum integer value of
** 4294967295.
*/
#define ASM_FORMAT_PRIVATE_NAME(OUTPUT, NAME, LABELNO)   \
( (OUTPUT) = (char *) alloca (strlen ((NAME)) + 10+1+1),   \
  sprintf ((OUTPUT), "%s.%u", (NAME), (LABELNO)))

/*
** Print operand X (an rtx) in assembler syntax to file FILE.
** CODE is a letter or dot (`z' in `%z0') or 0 if no letter was specified.
** For `%' followed by punctuation, CODE is the punctuation and X is null.
** dsPIC note:  there are these special CODE characters:
**   See pic30.c.
*/
#define PRINT_OPERAND(FILE, X, CODE)  pic30_print_operand(FILE,X,CODE)
#define PRINT_OPERAND_ADDRESS(FILE, ADDR) pic30_print_operand_address(FILE,ADDR)
#define PIC30_RPAIRSOVERLAP_P(idDst, idSrc) \
      (((idSrc+1) >= idDst) && ((idDst+1) >= idSrc))

/*
** Easy access check for function beginning 
**/
#define NOTE_INSN_FUNCTION_BEG_P(INSN) \
  ((GET_CODE(INSN) == NOTE) && (NOTE_KIND (INSN) == NOTE_INSN_FUNCTION_BEG))


/*
 *  On systems where we have a licence manager, call it
 */
#ifdef LICENSE_MANAGER
extern int pic30_license_valid;

#define NO_GCC_DRIVER_HOST_INITIALIZATION \
{  char *path;                                                             \
   char *exec;                                                             \
   char *args[] = { 0, "-k", 0 };                                          \
   char *c;                                                                \
   char *err_msg, *err_arg;                                                \
   int pid;                                                                \
   int status;                                                             \
                                                                           \
   pic30_license_valid = 0;                                                \
   path = make_relative_prefix(argv[0], standard_bindir_prefix,            \
                                        standard_exec_prefix);             \
                                                                           \
   if (!path) pfatal_pexecute("Could not locate `%s`\n", argv[0]);         \
   exec = xmalloc(strlen(path)+sizeof("pic30-lm.exe") + 1);                \
   sprintf(exec, "%spic30-lm.exe", path);                                  \
   args[0] = exec;                                                         \
   pid = pexecute(exec, args, progname, 0, &err_msg, &err_arg,          \
                  PEXECUTE_FIRST | PEXECUTE_LAST);                         \
   if (pid == -1) pfatal_pexecute (err_msg, exec);                         \
   pid = pwait(pid, &status, 0);                                           \
   if (pid < 0) abort();                                                   \
   if (WIFEXITED(status) && (WEXITSTATUS(status) == 0)) {                  \
     pic30_license_valid=1;                                                \
   }                                                                       \
   free(exec);                                                             \
}

#endif

/* define if any work needs to be done before the prologue is generated:
   prologue generation is a new sequence, and will therefore not have access
   to the current instruction flow */
#define PRE_PROLOGUE_FN \
  { \
    (void) pic30_asm_function_p(1); \
  }

#define SECTION_FLAGS_INT unsigned long long

#define NO1_GCC_DRIVER_HOST_INITIALIZATION { \
  int size;                              \
                                         \
  size = strlen(argv[0])+1;                                           \
  if (size < p-argv[0] + sizeof("c30_device.info")+1)                 \
    size = p-argv[0] + sizeof("c30_device.info")+1;                   \
  (char *) pic30_resource_file_f = xmalloc(size);                       \
                                                                      \
  sprintf(pic30_resource_file_f, "%s",argv[0]);                         \
  pic30_resource_file_f[p-argv[0]] = 0;                                 \
  strcat(pic30_resource_file_f,"c30_device.info");                      \
}

/*----------------------------------------------------------------------*/

/*END********************************************************************/

#define TARGET_APPLY_PRAGMA pic30_apply_pragmas
#define TARGET_LAYOUT_TYPE(TYPE)  pic30_layout_type(TYPE)
#define TARGET_POINTER_MODE(TYPE,DECL) (pic30_pointer_mode(TYPE,DECL))
#define TARGET_POINTER_SIZE(TYPE) (GET_MODE_SIZE(pic30_pointer_mode(TYPE)))
#define TARGET_CONVERT_POINTER    pic30_convert_pointer

enum pic30_address_space {
  /* ADDR_SPACE_GENERIC = 0 */
  pic30_space_psv = 1,
  pic30_space_prog,
  pic30_space_pmp,
  pic30_space_external,
  pic30_space_eds,
  pic30_space_packed,
  pic30_space_data_flash,
  pic30_space_const,           // for pic30_addr_space_subset_p
  pic30_space_stack,           // always on the stack
};

enum pic30_set_psv_results {
  pic30_set_nothing,
  pic30_set_on_call,
  pic30_set_on_return,
  pic30_set_for_tracking
};


extern tree pic30_expand_constant(tree);
#define TARGET_EXPAND_CONSTANT pic30_expand_constant
#define POINTERS_EXTEND_UNSIGNED 1

#define ASM_OUTPUT_FUNCTION_PRE(FILE, DECL, FNNAME) \
  pic30_function_pre(FILE,DECL,FNNAME)

extern void pic30_target_bind(tree name, tree decl);
#define TARGET_BIND pic30_target_bind

extern int pic30_emit_block_move(rtx x, rtx *y, rtx size, unsigned int align);
#define TARGET_EMIT_BLOCK_MOVE pic30_emit_block_move

extern int pic30_emit_block_set(rtx x, rtx size, unsigned int align);
#define TARGET_EMIT_BLOCK_SET pic30_emit_block_set

#define EXTRA_RTL_FILE "config/pic30/pic30-rtl.def"

/*
 *   object file signatures
 */

typedef union {
  unsigned int mask;
  struct {
    unsigned int unsigned_long_size_t:1;  /* true if size_t is unsigned long */
    unsigned int unified_memory:1;        /* true if using unified memory model */
    unsigned int no_short_double:1;       /* true if -fno-short-double is used */
    unsigned int dummy3:1;                /* place holder */
    unsigned int dummy4:1;                /* place holder */
    unsigned int dummy5:1;                /* place holder */
    unsigned int dummy6:1;                /* place holder */
    unsigned int dummy7:1;                /* place holder */
    unsigned int dummy8:1;                /* place holder */
    unsigned int dummy9:1;                /* place holder */
    unsigned int dummy10:1;               /* place holder */
    unsigned int dummy11:1;               /* place holder */
    unsigned int dummy12:1;               /* place holder */
    unsigned int dummy13:1;               /* place holder */
    unsigned int dummy14:1;               /* place holder */
    unsigned int dummy15:1;               /* place holder */
  } bits;
} object_signature_t;

extern object_signature_t options_set, external_options_mask;

#define C_COMMON_OVERRIDE_OPTIONS pic30_common_override_options()

#define USE_SELECT_SECTION_FOR_FUNCTIONS 1

#define TARGET_HAVE_SWITCHABLE_BSS_SECTIONS true

#ifdef MCHP_VERSION
#define PIC30_STRING1(x) #x
#define PIC30_STRING(x) PIC30_STRING1(x)
#define SET_MCHP_VERSION(x)                                        \
  { char *version = (char *)PIC30_STRING(MCHP_VERSION);            \
    char *c;                                                       \
    int mm = 0;                                                    \
    int major=0;                                                   \
    int minor=0;                                                   \
                                                                   \
    x = 0;                                                         \
    for (c = version; *c; c++) {                                   \
      if ((*c >= '0') && (*c <= '9')) {                            \
        if (mm == 1) minor = (minor * 10) + (*c - '0');            \
        else if (mm == 0) major = (major *10) + (*c - '0');        \
        /* else ignore it */                                       \
      } else if ((*c == '_') || (*c == '.')) mm++;                 \
    }                                                              \
    if (minor > 99) minor = 99;                                    \
    x = major*1000 + minor;                                        \
  }
#endif

#if 0
enum pic30_acceptible_regs_flags {
  parf_reg_only =        0,
  parf_subreg_ok =       1,
  parf_D_regs_only =     2,
  parf_int_regs_only =   4,   
  parf_int_subreg_ok =   5, /* subreg ok if int */
  parf_float_regs_only = 8,
  parf_float_subreg_ok = 9, /* subreg ok if float */
};
#endif

#define TARGET_CHECK_SECTION_FLAGS pic30_check_section_flags

#define MCHP_CONFIGURATION_DATA_FILENAME "configuration.data"
#define AUX_MCHP_CONFIGURATION_DATA_FILENAME "aux_configuration.data"
#define MCHP_CONFIGURATION_HEADER_MARKER "Configuration Word Definitions: "
#define MCHP_CONFIGURATION_HEADER_VERSION "0001"
#define MCHP_CONFIGURATION_HEADER_SIZE \
  (sizeof (MCHP_CONFIGURATION_HEADER_MARKER) + 5)

#define mchp_processor_string pic30_target_cpu
extern char *mchp_config_data_dir;

/*
 * some modes don't have a linear address map, so its not okay just to convert
 * them by gen_lowpart - there may be some 'work' required.
 */
/* all modes are linear, bar EDS and UMM */
#define TARGET_LINEAR_MODE(mode) \
  (!((mode == P32PEDSmode) || (mode == P32EDSmode) || (mode == P32UMMmode) || \
     (mode == P32EXTmode)))

#define TARGET_BUILD_VARIANT_TYPE_COPY pic30_build_variant_type_copy


#define LEGITIMIZE_RELOAD_ADDRESS(X,MODE,OPNUM,TYPE,IND_LEVELS,WIN)            \
  do {                                                                         \
    if (pic30_legitimize_reload_address(X,MODE,OPNUM,TYPE,IND_LEVELS,insn)) {  \
      goto WIN;                                                                \
    }                                                                          \
  } while(0)

#define TARGET_WARN_ADDRESS pic30_warn_address

enum pic30_fp_support_modes {
  pic30_none = 0,
  pic30_truncation,
  pic30_conventional,
  pic30_convergent,
  pic30_fastest
}; 

extern enum pic30_fp_support_modes pic30_fp_round_p(void);
#ifndef __cplusplus
#ifndef bool
#define bool char
#endif
#endif

extern bool pic30_fp_inline_p(void);

#define CORCON_SET_SATA (1<<7)
#define CORCON_SET_SATB (1<<6)

/* for "save()" function attribute use in .md file */
struct saved_list {
  rtx saved_value;
  tree decl;
  struct saved_list *next;
};

/* Memory sizes for current device; */
struct pic30_mem_info_ {
  int flash[2];      /* [0] -> main flash,    [1] -> aux flash */
  int ram[2];        /* [0] -> main ram,      [1] -> aux ram */
  int eeprom[2];     /* [0] -> eeprom size,   [1] -> unused */
  int dataflash[2];  /* [0] -> size in bytes, [1] -> page number */
  int sfr[2];        /* [0] -> end of SFR space, [1] -> unused */
};

extern struct pic30_mem_info_ pic30_mem_info;

#define MCHP_VALIDATE_SETTING_CHOICE(spec, setting, FAIL) \
  if (pic30_validate_config_setting(spec,setting) == 0) { FAIL; } (void)0

/* stealing this function from varasm.c */
extern bool bss_initializer_p (const_tree, bool);

extern tree pic30_target_pointer_sizetype(tree);
#define TARGET_POINTER_SIZETYPE pic30_target_pointer_sizetype

extern int pic30_type_suffix(tree type, int* is_long);
#define TARGET_TYPE_SUFFIX pic30_type_suffix

/* PIC30 section flag defines */
#define PIC30_LL(X) (JOIN2(X,LL))
                                /* 0x1000000 */
#define SECTION_READ_ONLY       (PIC30_LL(SECTION_MACH_DEP))          /* 24 */
#define SECTION_XMEMORY         (PIC30_LL(SECTION_MACH_DEP) << 1)     /* 25 */
#define SECTION_YMEMORY         (PIC30_LL(SECTION_MACH_DEP) << 2)     /* 26 */
#define SECTION_NEAR            (PIC30_LL(SECTION_MACH_DEP) << 3)     /* 27 */
#define SECTION_PERSIST         (PIC30_LL(SECTION_MACH_DEP) << 4)     /* 28 */
#define SECTION_PSV             (PIC30_LL(SECTION_MACH_DEP) << 5)     /* 29 */
#define SECTION_EEDATA          (PIC30_LL(SECTION_MACH_DEP) << 6)     /* 30 */
#define SECTION_NOLOAD          (PIC30_LL(SECTION_MACH_DEP) << 7)     /* 31 */
#define SECTION_REVERSE         (PIC30_LL(SECTION_MACH_DEP) << 8)     /* 32 */
#define SECTION_INFO            (PIC30_LL(SECTION_MACH_DEP) << 9)     /* 33 */
#define SECTION_ADDRESS         (PIC30_LL(SECTION_MACH_DEP) << 10)    /* 34 */
#define SECTION_ALIGN           (PIC30_LL(SECTION_MACH_DEP) << 11)    /* 35 */
#define SECTION_DMA             (PIC30_LL(SECTION_MACH_DEP) << 12)    /* 36 */
#define SECTION_PMP             (PIC30_LL(SECTION_MACH_DEP) << 13)    /* 37 */
#define SECTION_EXTERNAL        (PIC30_LL(SECTION_MACH_DEP) << 14)    /* 38 */
#define SECTION_EDS             (PIC30_LL(SECTION_MACH_DEP) << 15)    /* 39 */
#define SECTION_PAGE            (PIC30_LL(SECTION_MACH_DEP) << 16)    /* 40 */
#define SECTION_AUXFLASH        (PIC30_LL(SECTION_MACH_DEP) << 17)    /* 41 */
#define SECTION_AUXPSV          (PIC30_LL(SECTION_MACH_DEP) << 18)    /* 42 */
#define SECTION_PACKEDFLASH     (PIC30_LL(SECTION_MACH_DEP) << 19)    /* 43 */
#define SECTION_KEEP            (PIC30_LL(SECTION_MACH_DEP) << 20)    /* 44 */
#define SECTION_CONST_NAME      (PIC30_LL(SECTION_MACH_DEP) << 21)    /* 45 */
#define SECTION_CO_SHARED       (PIC30_LL(SECTION_MACH_DEP) << 22)    /* 46 */
#define SECTION_PRESERVED       (PIC30_LL(SECTION_MACH_DEP) << 23)    /* 47 */
#define SECTION_PRIORITY        (PIC30_LL(SECTION_MACH_DEP) << 24)    /* 48 */
#define SECTION_UPDATE          (PIC30_LL(SECTION_MACH_DEP) << 25)    /* 49 */

/* Add macros for code coverage */
#define TARGET_XCCOV_LIBEXEC_PATH "/bin/"
#define TARGET_XCCOV_SET_CC_BIT pic30_set_cc_bit

#undef TARGET_ASM_CODE_END
#define TARGET_ASM_CODE_END pic30_asm_code_end

extern char *pic30_cover_insn(unsigned bitno);

#undef TARGET_XCCOV_EMIT_SECTION
#define TARGET_XCCOV_EMIT_SECTION pic30_emit_cc_section

#undef TARGET_XCCOV_LICENSED
#define TARGET_XCCOV_LICENSED pic30_licensed_xccov_p

#define CLEAR_RATIO(speed) 2

/* LOAD/STORE preinc/dec are inefficient for large modes */
#define USE_LOAD_PRE_INCREMENT(mode)  (((mode) == QImode) || ((mode) == HImode))
#define USE_LOAD_PRE_DECREMENT(mode)  (((mode) == QImode) || ((mode) == HImode))
#define USE_STORE_PRE_INCREMENT(mode) (((mode) == QImode) || ((mode) == HImode))
#define USE_STORE_PRE_DECREMENT(mode) (((mode) == QImode) || ((mode) == HImode))

extern void record_psv_tracking(int, int, rtx);
rtx pic30_get_set_psv_value(rtx);

#define LIBGCC2_UNITS_PER_WORD 2

/* set path to linker for collect2 wrapper */
#undef COLLECT2_RELATIVE_LD_FILE_NAME
#define COLLECT2_RELATIVE_LD_FILE_NAME "../xc16-ld"

int pic30_reload_in_progress(void);

#define GENERIC_POINTER_TYPE_P(T) \
   (POINTER_TYPE_P(T) && (TYPE_ADDR_SPACE(TREE_TYPE(T)) == ADDR_SPACE_GENERIC))

#endif
