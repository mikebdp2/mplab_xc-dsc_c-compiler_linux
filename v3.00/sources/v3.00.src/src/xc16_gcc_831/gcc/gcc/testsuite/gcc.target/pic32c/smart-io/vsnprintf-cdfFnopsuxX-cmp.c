/* smart-io output test: vsnprintf-cdfFnopsuxX-cmp.c 
   Invoke stdio function with appropriate arguments. To be compiled
   with and without -mno-smart-io. 
*/
#define PREFIX vsn
#define ROOT printf
#include "harness.h"
static void test (void)
{
  FN_ARG_DECL;
  FUNC(_quot( FMT_c FMT_s FMT_fF) _eval3( ARG_c ARG_s ARG_fF));
}
