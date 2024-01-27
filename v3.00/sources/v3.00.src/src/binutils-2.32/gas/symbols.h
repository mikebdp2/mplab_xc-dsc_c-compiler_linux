/* symbols.h -
   Copyright (C) 1987-2019 Free Software Foundation, Inc.

   This file is part of GAS, the GNU Assembler.

   GAS is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   GAS is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with GAS; see the file COPYING.  If not, write to the Free
   Software Foundation, 51 Franklin Street - Fifth Floor, Boston, MA
   02110-1301, USA.  */

extern struct obstack notes;	/* eg FixS live here.  */

extern struct obstack cond_obstack;	/* this is where we track .ifdef/.endif
				       (if we do that at all).  */

extern symbolS *symbol_rootP;	/* all the symbol nodes */
extern symbolS *symbol_lastP;	/* last struct symbol we made, or NULL */

extern symbolS abs_symbol;
extern symbolS dot_symbol;

extern int symbol_table_frozen;

/* This is non-zero if symbols are case sensitive, which is the
   default.  */
extern int symbols_case_sensitive;

char * symbol_relc_make_expr  (expressionS *);
char * symbol_relc_make_sym   (symbolS *);
char * symbol_relc_make_value (offsetT);
char *decode_local_label_name (char *s);
symbolS *symbol_find (const char *name);
symbolS *symbol_find_noref (const char *name, int noref);
symbolS *symbol_find_exact (const char *name);
symbolS *symbol_find_exact_noref (const char *name, int noref);
symbolS *symbol_find_or_make (const char *name);
symbolS *symbol_make (const char *name);
symbolS *symbol_new (const char *name, segT segment, valueT value,
		     fragS * frag);
symbolS *symbol_create (const char *name, segT segment, valueT value,
			fragS * frag);
struct local_symbol *local_symbol_make (const char *name, segT section,
					valueT val, fragS *frag);
symbolS *symbol_clone (symbolS *, int);
#undef symbol_clone_if_forward_ref
symbolS *symbol_clone_if_forward_ref (symbolS *, int);
#define symbol_clone_if_forward_ref(s) symbol_clone_if_forward_ref (s, 0)
symbolS *symbol_temp_new (segT, valueT, fragS *);
symbolS *symbol_temp_new_now (void);
symbolS *symbol_temp_make (void);

symbolS *colon (const char *sym_name);
void local_colon (int n);
void symbol_begin (void);
void dot_symbol_init (void);
void symbol_print_statistics (FILE *);
void symbol_table_insert (symbolS * symbolP);
valueT resolve_symbol_value (symbolS *);
void resolve_local_symbol_values (void);
int snapshot_symbol (symbolS **, valueT *, segT *, fragS **);

void print_symbol_value (symbolS *);
void print_expr (expressionS *);
void print_expr_1 (FILE *, expressionS *);
void print_symbol_value_1 (FILE *, symbolS *);

int dollar_label_defined (long l);
void dollar_label_clear (void);
void define_dollar_label (long l);
char *dollar_label_name (long l, int augend);

void fb_label_instance_inc (long label);
char *fb_label_name (long n, long augend);

extern void copy_symbol_attributes (symbolS *, symbolS *);

/* Get and set the values of symbols.  These used to be macros.  */
extern valueT S_GET_VALUE (symbolS *);
extern void S_SET_VALUE (symbolS *, valueT);

extern int S_IS_FUNCTION (symbolS *);
extern int S_IS_EXTERNAL (symbolS *);
extern int S_IS_WEAK (symbolS *);
extern int S_IS_WEAKREFR (symbolS *);
extern int S_IS_WEAKREFD (symbolS *);
extern int S_IS_COMMON (symbolS *);
extern int S_IS_DEFINED (symbolS *);
extern int S_FORCE_RELOC (symbolS *, int);
extern int S_IS_DEBUG (symbolS *);
extern int S_IS_LOCAL (symbolS *);
extern int S_IS_STABD (symbolS *);
extern int S_CAN_BE_REDEFINED (const symbolS *);
extern int S_IS_VOLATILE (const symbolS *);
extern int S_IS_FORWARD_REF (const symbolS *);
extern const char *S_GET_NAME (symbolS *);
extern segT S_GET_SEGMENT (symbolS *);
extern void S_SET_SEGMENT (symbolS *, segT);
extern void S_SET_EXTERNAL (symbolS *);
extern void S_SET_NAME (symbolS *, const char *);
extern void S_CLEAR_EXTERNAL (symbolS *);
extern void S_SET_WEAK (symbolS *);
extern void S_SET_WEAKREFR (symbolS *);
extern void S_CLEAR_WEAKREFR (symbolS *);
extern void S_SET_WEAKREFD (symbolS *);
extern void S_CLEAR_WEAKREFD (symbolS *);
extern void S_SET_THREAD_LOCAL (symbolS *);
extern void S_SET_VOLATILE (symbolS *);
extern void S_CLEAR_VOLATILE (symbolS *);
extern void S_SET_FORWARD_REF (symbolS *);

#ifndef WORKING_DOT_WORD
struct broken_word
  {
    /* Linked list -- one of these structures per ".word x-y+C"
       expression.  */
    struct broken_word *next_broken_word;
    /* Segment and subsegment for broken word.  */
    segT seg;
    subsegT subseg;
    /* Which frag is this broken word in?  */
    fragS *frag;
    /* Where in the frag is it?  */
    char *word_goes_here;
    /* Where to add the break.  */
    fragS *dispfrag;		/* where to add the break */
    /* Operands of expression.  */
    symbolS *add;
    symbolS *sub;
    offsetT addnum;

    int added;			/* nasty thing happened yet? */
    /* 1: added and has a long-jump */
    /* 2: added but uses someone elses long-jump */

    /* Pointer to broken_word with a similar long-jump.  */
    struct broken_word *use_jump;
  };
extern struct broken_word *broken_words;
#endif /* ifdef WORKING_DOT_WORD */

#if PIC30
/* Additional information we keep for each symbol.  */
/* - copied from obj-elf.h                          */
#ifndef HAS_ELF_OBJ_SY
#define HAS_ELF_OBJ_SY
struct elf_obj_sy
{
  /* Whether the symbol has been marked as local.  */
  int local;

  /* Use this to keep track of .size expressions that involve
     differences that we can't compute yet.  */
  expressionS *size;

  /* The name specified by the .symver directive.  */
  char *versioned_name;
};
#endif

/* Moved from symbols.c  */
#define OBJ_SYMFIELD_TYPE struct elf_obj_sy

struct symbol_flags
{
  /* Whether the symbol is a local_symbol.  */
  unsigned int sy_local_symbol : 1;

  /* Weather symbol has been written.  */
  unsigned int sy_written : 1;

  /* Whether symbol value has been completely resolved (used during
     final pass over symbol table).  */
  unsigned int sy_resolved : 1;

  /* Whether the symbol value is currently being resolved (used to
     detect loops in symbol dependencies).  */
  unsigned int sy_resolving : 1;

  /* Whether the symbol value is used in a reloc.  This is used to
     ensure that symbols used in relocs are written out, even if they
     are local and would otherwise not be.  */
  unsigned int sy_used_in_reloc : 1;

  /* Whether the symbol is used as an operand or in an expression.
     NOTE:  Not all the backends keep this information accurate;
     backends which use this bit are responsible for setting it when
     a symbol is used in backend routines.  */
  unsigned int sy_used : 1;

  /* Whether the symbol can be re-defined.  */
  unsigned int sy_volatile : 1;

  /* Whether the symbol is a forward reference.  */
  unsigned int sy_forward_ref : 1;

  /* This is set if the symbol is defined in an MRI common section.
     We handle such sections as single common symbols, so symbols
     defined within them must be treated specially by the relocation
     routines.  */
  unsigned int sy_mri_common : 1;

  /* This is set if the symbol is set with a .weakref directive.  */
  unsigned int sy_weakrefr : 1;

  /* This is set when the symbol is referenced as part of a .weakref
     directive, but only if the symbol was not in the symbol table
     before.  It is cleared as soon as any direct reference to the
     symbol is present.  */
  unsigned int sy_weakrefd : 1;
};

/* The information we keep for a symbol.  Note that the symbol table
   holds pointers both to this and to local_symbol structures.  See
   below.  */

struct symbol
{
  /* Symbol flags.  */
  struct symbol_flags sy_flags;

  /* BFD symbol */
  asymbol *bsym;

  /* The value of the symbol.  */
  expressionS sy_value;

  /* Forwards and (optionally) backwards chain pointers.  */
  struct symbol *sy_next;
  struct symbol *sy_previous;

  /* Pointer to the frag this symbol is attached to, if any.
     Otherwise, NULL.  */
  struct frag *sy_frag;

#ifdef OBJ_SYMFIELD_TYPE
  OBJ_SYMFIELD_TYPE sy_obj;
#endif

#ifdef TC_SYMFIELD_TYPE
  TC_SYMFIELD_TYPE sy_tc;
#endif

#ifdef TARGET_SYMBOL_FIELDS
  TARGET_SYMBOL_FIELDS
#endif
};

/* A pointer in the symbol may point to either a complete symbol
   (struct symbol above) or to a local symbol (struct local_symbol
   defined here).  The symbol code can detect the case by examining
   the first field.  It is always NULL for a local symbol.

   We do this because we ordinarily only need a small amount of
   information for a local symbol.  The symbol table takes up a lot of
   space, and storing less information for a local symbol can make a
   big difference in assembler memory usage when assembling a large
   file.  */

struct local_symbol
{
  /* Symbol flags.  Only sy_local_symbol and sy_resolved are relevant.  */
  struct symbol_flags lsy_flags;

  /* The symbol section.  This also serves as a flag.  If this is
     reg_section, then this symbol has been converted into a regular
     symbol, and lsy_sym points to it.  */
  segT lsy_section;

  /* The symbol name.  */
  const char *lsy_name;

  /* The symbol frag or the real symbol, depending upon the value in
     lsy_section.  */
  union
  {
    fragS *lsy_frag;
    symbolS *lsy_sym;
  } u;

  /* The value of the symbol.  */
  valueT lsy_value;

#ifdef TC_LOCAL_SYMFIELD_TYPE
  TC_LOCAL_SYMFIELD_TYPE lsy_tc;
#endif
};
#endif

/*
 * Current means for getting from symbols to segments and vice verse.
 * This will change for infinite-segments support (e.g. COFF).
 */
extern const segT N_TYPE_seg[];	/* subseg.c */

#define	SEGMENT_TO_SYMBOL_TYPE(seg)  ( seg_N_TYPE [(int) (seg)] )
extern const short seg_N_TYPE[];/* subseg.c */

#define	N_REGISTER	30	/* Fake N_TYPE value for SEG_REGISTER */

void symbol_clear_list_pointers (symbolS * symbolP);

void symbol_insert (symbolS * addme, symbolS * target,
		    symbolS ** rootP, symbolS ** lastP);
void symbol_remove (symbolS * symbolP, symbolS ** rootP,
		    symbolS ** lastP);

extern symbolS *symbol_previous (symbolS *);

extern int symbol_on_chain (symbolS *s, symbolS *rootPP, symbolS *lastPP);

void verify_symbol_chain (symbolS * rootP, symbolS * lastP);

void symbol_append (symbolS * addme, symbolS * target,
		    symbolS ** rootP, symbolS ** lastP);

extern symbolS *symbol_next (symbolS *);

extern expressionS *symbol_get_value_expression (symbolS *);
extern void symbol_set_value_expression (symbolS *, const expressionS *);
extern offsetT *symbol_X_add_number (symbolS *);
extern void symbol_set_value_now (symbolS *);
extern void symbol_set_frag (symbolS *, fragS *);
extern fragS *symbol_get_frag (symbolS *);
extern void symbol_mark_used (symbolS *);
extern void symbol_clear_used (symbolS *);
extern int symbol_used_p (symbolS *);
extern void symbol_mark_used_in_reloc (symbolS *);
extern void symbol_clear_used_in_reloc (symbolS *);
extern int symbol_used_in_reloc_p (symbolS *);
extern void symbol_mark_mri_common (symbolS *);
extern void symbol_clear_mri_common (symbolS *);
extern int symbol_mri_common_p (symbolS *);
extern void symbol_mark_written (symbolS *);
extern void symbol_clear_written (symbolS *);
extern int symbol_written_p (symbolS *);
extern void symbol_mark_resolved (symbolS *);
extern int symbol_resolved_p (symbolS *);
extern int symbol_section_p (symbolS *);
extern int symbol_equated_p (symbolS *);
extern int symbol_equated_reloc_p (symbolS *);
extern int symbol_constant_p (symbolS *);
extern int symbol_shadow_p (symbolS *);
extern symbolS *symbol_symbolS (symbolS *);
extern asymbol *symbol_get_bfdsym (symbolS *);
extern void symbol_set_bfdsym (symbolS *, asymbol *);
extern int symbol_same_p (symbolS *, symbolS *);

#ifdef OBJ_SYMFIELD_TYPE
OBJ_SYMFIELD_TYPE *symbol_get_obj (symbolS *);
void symbol_set_obj (symbolS *, OBJ_SYMFIELD_TYPE *);
#endif

#ifdef TC_SYMFIELD_TYPE
TC_SYMFIELD_TYPE *symbol_get_tc (symbolS *);
void symbol_set_tc (symbolS *, TC_SYMFIELD_TYPE *);
#endif
