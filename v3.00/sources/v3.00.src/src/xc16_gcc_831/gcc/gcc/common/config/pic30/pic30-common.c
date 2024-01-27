/* Default common target hooks initializer.
   Copyright (C) 2011-2018 Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 3, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "common/common-target.h"
#include "common/common-target-def.h"

/* Do not include tm.h or tm_p.h here; if it is useful for a target to
   define some macros for the initializer in a header without defining
   targetm_common itself (for example, because of interactions with
   some hooks depending on the target OS and others on the target
   architecture), create a separate tm_common.h for only the relevant
   definitions.  */

/*
 * Called after -On option is parsed but before -f flag options are
 *   parsed.
 */
static const struct default_options pic30_option_optimization_table[] =
  {
    {OPT_LEVELS_ALL, OPT_ftree_slp_vectorize, NULL, 0},
    {OPT_LEVELS_ALL, OPT_ftree_loop_vectorize, NULL, 0},
    {OPT_LEVELS_ALL, OPT_ftree_vectorize, NULL, 0},
    {OPT_LEVELS_ALL, OPT_fschedule_insns, NULL, 0},
    {OPT_LEVELS_ALL, OPT_fschedule_insns2, NULL, 0},
    {OPT_LEVELS_SIZE, OPT_fmove_loop_invariants, NULL, 0},
    {OPT_LEVELS_SIZE, OPT_fsplit_wide_types, NULL, 0},
    {OPT_LEVELS_1_ONLY, OPT_fsplit_wide_types, NULL, 0},
#if (x_TARGET_NO_OVERRIDE_INLINE == 0)
    {OPT_LEVELS_SIZE, OPT_finline_functions_called_once, NULL, 0},
    {OPT_LEVELS_1_ONLY, OPT_finline_functions_called_once, NULL, 0},
    {OPT_LEVELS_SIZE, OPT_finline_small_functions, NULL, 0},
    {OPT_LEVELS_1_ONLY, OPT_finline_small_functions, NULL, 0},
    {OPT_LEVELS_SIZE, OPT_findirect_inlining, NULL, 0},
    {OPT_LEVELS_1_ONLY, OPT_findirect_inlining, NULL, 0},
#endif
    {OPT_LEVELS_SIZE, OPT_ftree_switch_conversion, NULL, 0},
    {OPT_LEVELS_1_ONLY, OPT_ftree_switch_conversion, NULL, 0},
    {OPT_LEVELS_SIZE, OPT_ftree_parallelize_loops_, NULL, 0},
    {OPT_LEVELS_1_ONLY, OPT_ftree_parallelize_loops_, NULL, 0},
    { OPT_LEVELS_NONE, 0, NULL, 0 }
  };
/*
 * Override the default optimization options, just before -f flag parsing
 *   takes effect but after -On parsing.
 */
#undef TARGET_OPTION_OPTIMIZATION_TABLE
#define TARGET_OPTION_OPTIMIZATION_TABLE \
   pic30_option_optimization_table
struct gcc_targetm_common targetm_common = TARGETM_COMMON_INITIALIZER;
