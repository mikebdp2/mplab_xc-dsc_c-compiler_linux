/* 
   smart-io merge test: fprintf-cdnopsuxX-opt.c
   Invoke standard IO function fprintf and ensure the mangled symbol is generated.
   No checking of the actual mangling vs. arguments is performed.
*/
/* { dg-do compile } */
/* { dg-additional-options "-msmart-io=1" } */
#define PREFIX f
#define ROOT printf
#include "harness.h"
static void test (void)
{
  FN_ARG_DECL;
  FUNC(_quot( FMT_c FMT_s) _eval3( ARG_c ARG_s));
}

/* { dg-final { scan-assembler "__fprintf_\[a-zA-Z\]*" } } */