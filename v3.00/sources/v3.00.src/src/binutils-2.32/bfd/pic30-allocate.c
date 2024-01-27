/*
** pic30-allocate.c
**
** Copyright (c) 2009 Microchip Technology, Inc.
**
** This file contains pic30-specifc memory allocation
** processing for the linker.
**
** It is included by the following target-specific
** emulation files:
**
**   ld/emultmpl/pic30_coff.em
**   ld/emultmpl/pic30_elf32.em
**
*/

/*****************************************************************************
**
** Improved Memory Allocation Routines
**
** These functions implement the improved
** memory allocator introduced in MPLAB C30 v1.30.
**
*/

#if 0
  Best Fit Allocator - How it Works

  1) For each region of memory, compile a list of free memory blocks sorted in order
     of increasing size. There could be several free blocks in any particular region,
     separated by absolute sections (at fixed addresses) and/or sections that were placed
     by the sequential allocator (which means they were explicitly mapped in the linker script.)

  2) Using an attribute mask, build an allocation list of sections to be placed, sorted
     in order of decreasing size. The attribute mask is used to select sections for
     allocation that are most constrained; these are allocated first. For example,
     PSV-type sections are more constrained than non-PSV sections.

  3) In each category (defined by the attribute mask) the strategy is to place
     larger sections first, and consume smaller free blocks first.

  4) For each section to be allocated, qualify free blocks based on size and type (such as
     the extent of Y-memory available within the block, etc.)

  5) When a free block has been qualified, scan it from the beginning (low address boundary)
     and stop at the first solution that works. Generally, this means that the alignment
     requirement has been satisfied, in addition to the other constraints. This position
     in the block becomes option 1.

  6) Next, scan the free block from the end (high address boundary) and stop at the first
     solution that works. This position becomes option 2.

  7) If neither option is valid, continue with the next (larger) free block.

  8) If one or both options are valid, select one that leaves the largest remainder.

  9) Annotate the section as placed, and add the remainder to the free blocks list in
     the proper (sorted) location.
#endif

/* useful constants and macros */
#define NEAR_BOUNDARY   (pic30_is_isav32_machine(global_PROCESSOR) ? \
                          0xFFFF : 0x2000)
#define EDS_BOUNDARY    (pic30_is_isav32_machine(global_PROCESSOR) ? \
                          0x0000 : 0x8000)
#define PSV_BOUNDARY    (pic30_is_isav32_machine(global_PROCESSOR) ? \
                          0x0000 : 0x8000)
#define PROG_BOUNDARY   (pic30_is_isav32_machine(global_PROCESSOR) ? \
                          0x800000 : 0x10000)

#define LOCAL_DATA_LIMIT 0x8000
#define DATA_BUS_LIMIT   0x10000
#define ISAV32_DATA_BUS_LIMIT ((1<<24)-1)

#define EDS_OFFSET_BITS 0x7FFF
#define PSV_OFFSET_BITS 0x7FFF
#define PROG_OFFSET_BITS 0xFFFF

#define NEXT_EDS_PAGE(a)      (((a) & ~ EDS_OFFSET_BITS) + EDS_BOUNDARY)
#define PREV_EDS_PAGE(a)       ((a) & ~ EDS_OFFSET_BITS)

#define NEXT_PSV_PAGE(a)      (((a) & ~ PSV_OFFSET_BITS) + PSV_BOUNDARY)
#define PREV_PSV_PAGE(a)       ((a) & ~ PSV_OFFSET_BITS)

#define NEXT_PROG_PAGE(a)      (((a) & ~ PROG_OFFSET_BITS) + PROG_BOUNDARY)
#define PREV_PROG_PAGE(a)       ((a) & ~ PROG_OFFSET_BITS)

#define VALID_X(addr, len)    (((bfd_vma)(addr)+(len) <= ydata_base) || \
                               ((ydata_end == 0) || \
                                (((bfd_vma)(addr)+(len)) > ydata_end)))
#define VALID_Y(addr, len)    (((bfd_vma)(addr) >= ydata_base) && \
                               ((ydata_end == 0) || \
                                (((bfd_vma)(addr)+(len)) <= ydata_end)))
#define VALID_NEAR(addr, len) ((bfd_vma)(addr)+(len) <= NEAR_BOUNDARY)
#define VALID_PSV(addr, len)  (((addr)+(len)) <= NEXT_PSV_PAGE(addr))
#define VALID_EDS_PAGE(addr, len)  (((addr)+(len)) <= NEXT_EDS_PAGE(addr))
#define VALID_PROG_PAGE(addr, len) (((addr)+(len)) <= NEXT_PROG_PAGE(addr))
#define VALID_DMA(addr, len)  (((bfd_vma)(addr) >= dma_base) &&       \
                               ((bfd_vma)(addr)+(len) <= dma_end + 1))
#define VALID_LOCAL_DATA(addr, len) ((bfd_vma)(addr)+(len) <= EDS_BOUNDARY)
#define VALID_LOW_ADDR(addr, len)  ((bfd_vma)(addr)+(len) <= exclude_addr)
#define VALID_HIGH_ADDR(addr, len)  ((bfd_vma)(addr) >= exclude_addr)

#define VALID_BOOT(addr, len)  \
  (((bfd_vma)(addr)         >= base_address[BOOTx][alloc_region_index]) &&  \
   ((bfd_vma)(addr) + (len) <=  end_address[BOOTx][alloc_region_index] + 1))

#define OVERLAPS_BOOT(addr, len)  \
  (((bfd_vma)(addr) + (len) >= base_address[BOOTx][alloc_region_index] + 1) &&  \
   ((bfd_vma)(addr)         <=  end_address[BOOTx][alloc_region_index]))

#define VALID_SECURE(addr, len)  \
  (((bfd_vma)(addr)         >= base_address[SECUREx][alloc_region_index]) &&  \
   ((bfd_vma)(addr) + (len) <=  end_address[SECUREx][alloc_region_index] + 1))

#define OVERLAPS_SECURE(addr, len)  \
  (((bfd_vma)(addr) + (len) >= base_address[SECUREx][alloc_region_index] + 1) &&  \
   ((bfd_vma)(addr)         <=  end_address[SECUREx][alloc_region_index]))


/* message strings */
static const char *str1 = "%X Link Error: The used device does not support DPSRAM \n";
static const char *str2 = "__YDATA_BASE";
static const char *str2e = "__YDATA_END";
static const char *str3 = "__DMA_BASE";
static const char *str4 = "__DMA_END";

/* Create an id for each section attribute */
#define ATTR(a,b,c)  _##a,
 enum {
#include "pic30-attributes.h"
 } ;

/* Create a bit mask for each section attribute */
#undef ATTR
#define ATTR(a,b,c)  a = (1<<_##a),
 enum {
#include "pic30-attributes.h"
 } ;

/* Create a bit mask to match any attribute */
#define all_attr 0xFFFFFFFF

/* Memory allocator options  */
enum {
  NO_LOCATE_OPTION,
  FAVOR_LOW_ADDR,     /* scan free blocks from low addr only  */
  FAVOR_HIGH_ADDR,    /* scan free blocks from high addr only */
  EXCLUDE_LOW_ADDR,   /* avoid memory below LIMIT             */
  EXCLUDE_HIGH_ADDR,  /* avoid memory above LIMIT - 1         */
};

/* Data structure for free program memory blocks */
extern struct pic30_memory *program_memory_free_blocks;
extern struct pic30_memory *auxflash_memory_free_blocks;
extern struct pic30_memory *aivt_free_blocks;

struct usable_region_list {
  struct memory_region_struct *region;
  struct pic30_memory *free_blocks;
  struct usable_region_list *next;
};

/* External function prototypes */
extern int pic30_is_ecore_machine(const bfd_arch_info_type *);
extern int pic30_is_isav4_machine(const bfd_arch_info_type *);

/* prototypes */
static bfd_vma get_max_stack(void);
void update_section_addr(asection *,bfd_vma );

static int locate_options = 0;
static bfd_vma exclude_addr = 0;

static bfd_vma get_max_stack() {
  bfd_vma max_stack = LOCAL_DATA_LIMIT;

  /* No longer testing 'has_psvpag_reference' here (xc16-1769) */
  if (!pic30_local_stack) {
    if (!pic30_is_eds_machine(global_PROCESSOR) ||
        pic30_is_ecore_machine(global_PROCESSOR) ||
        pic30_is_isav4_machine(global_PROCESSOR)) {
      max_stack = DATA_BUS_LIMIT;
    }
  }

  if (pic30_is_isav32_machine(global_PROCESSOR)) {
    max_stack = ISAV32_DATA_BUS_LIMIT;
  }

  return max_stack;
}

static void
set_locate_options(int the_code, bfd_vma addr) {
  locate_options = the_code;
  exclude_addr = addr;
}

static void
reset_locate_options(void) {
  locate_options = NO_LOCATE_OPTION;
  exclude_addr = 0;
}

#define IS_LOCATE_OPTION(a)  (locate_options == (a))

static void
remove_section_from_bfd(bfd *abfd, asection *sec)
{
  asection *s, *prev;
  
  s = abfd->sections;
  if ((s == NULL) || (s->next == NULL)) return;

  prev = s;
  s = s->next; /* never remove the first section */
  for (; s != NULL; s = s->next) {
      if (s == sec) {
        prev->next = s->next;
        abfd->section_count -= 1;
        if (pic30_debug)
          printf("    removing section %s\n", s->name);
      }
      else
        prev = s;
    }
  return;
} /* static void remove_section_from_bfd (...)*/

static int
report_allocation_error(struct pic30_section *s) {
#define PREFIX "/tmp"
  char *colon = "";
  char *filename = "";
  char *secname = "*";
  struct pic30_section *t, *next;
  int error_reported = 0;

  for ( t = s; t != NULL; t = next ) {
    next = t->next;
    if ( t->sec && t->sec->linked == 0 ) {
      char *s1 = pic30_section_size_string(t->sec);
      char *s2 = pic30_section_attr_string(t->sec);

      if ( t->sec->size == 0 ) /* don't report zero-length sections */
        continue;
      
      error_reported++;
      if (t->alloc_error_reported) continue;

      t->alloc_error_reported++;
      /* report the file name, unless its a temp file */
      if (strncmp(t->sec->owner->filename, PREFIX, strlen(PREFIX)) != 0) {
        colon = ":";
        filename = (char *) t->sec->owner->filename;
      }

      /* report the section name, unless its based on a temp file */
      if (strncmp(t->sec->name, PREFIX, strlen(PREFIX)) != 0)
        secname = (char *) t->sec->name;

      einfo(_("%X%s%s Link Error: Could not allocate section %s, %s, %s\n"),
            filename, colon, secname, s1 ? s1 : "", s2 ? s2 : "");

      if (s1) free(s1);  /* free the malloc'ed strings */
      if (s2) free(s2);

      if (pic30_debug)
        printf("\n    Error: Could not allocate section %s\n", t->sec->name);
    }
  }
  return error_reported;
}

/*
 * allocate_memory()
 *
 * This function attempts to locate sections using
 * a best-fit algorithm. The term "best-fit" implies
 * that the smallest suitable free block will be used
 * for each section. This approach makes efficient use
 * of the free block list and reduces fragmentation.
 *
 * Called by: ldemul_after_allocation()
 *
 * Calls:     allocate_program_memory()
 *            allocate_data_memory()
 *            allocate_eedata_memory()
 *            bfd_map_over_sections()
 *            allocate_auxflash_memory()
 *
 * If any of the sub-processes fail, report it
 * and continue. This helps to suppress misleading
 * messages, and follows the general philosophy
 * of doing as much work as we can, despite the
 * occurrence of fatal errors.
 */
static void
allocate_memory() {
  int result;
  lang_output_section_statement_type **last_os;

#define ERR_STR " Link Error: Could not allocate "

  /* save the last output section statement
     after sequential allocation */
  last_os = (lang_output_section_statement_type **) statement_list.tail;

  /*
   * Build an ordered list of output sections
   * that were placed by sequential allocator.
   * It will help identify any gaps between
   * output sections that are available.
   */
  pic30_init_section_list (&pic30_section_list);
  bfd_map_over_sections (link_info.output_bfd, &pic30_build_section_list, NULL);

  if (pic30_debug) {
    pic30_print_section_list(unassigned_sections, "unassigned");
    pic30_print_section_list(memory_region_list, "memory region");
  }
  
  result = allocate_data_memory();
  if (result != 0)
    einfo(_("%F%sdata memory\n"), ERR_STR );

  result = allocate_program_memory();
  if (result != 0)
    einfo(_("%F%sprogram memory\n"), ERR_STR );

  result = allocate_eedata_memory();
  if (result != 0)
    einfo(_("%F%seedata memory\n"), ERR_STR );

  result = allocate_auxflash_memory();
  if (result != 0)
    einfo(_("%F%sauxflash memory\n"), ERR_STR );

  if (has_user_defined_memory) {
    result = allocate_user_memory();
    if (result != 0)
      einfo(_("%F%suser-defined memory region\n"), ERR_STR );
  }

  /* allocate the heap, if required */

  /* allocate the stack, unless the user has defined one */

  /* free the output section list */
  pic30_free_section_list(&pic30_section_list);
   
  /*
   * Scan over the output section statements
   * and merge any contiguous sections
   * with the same name, unless the
   * --unique option tells us not to.
   *
   * We start where the sequential allocator
   * finished, so that any merges are well understood.
   * For example, all of the best-fit output sections
   * contain a single input section.
   */
  {
    lang_statement_union_type *os, *next;
    asection *sec, *next_sec;
    unsigned int opb = bfd_octets_per_byte (link_info.output_bfd);
    unsigned int len, match, merging = 0;
    bfd_vma sec_len, next_len, merge_len = 0;
    char *p,*p2;
    

    if (pic30_debug)
      printf("\nScanning the output statements\n");
      
    for (os = (lang_statement_union_type *) *last_os;
         os != (lang_statement_union_type *) NULL;
         os = next) {

      /* clear the accumulator, if we didn't just merge */
      if (!merging) merge_len = 0;

      merging = 0;
      next = os->header.next;
      if (os->header.type == lang_output_section_statement_enum) {

        if (os->output_section_statement.bfd_section == NULL)
          // --gc-sections has discarded this section
          continue;

        sec = os->output_section_statement.bfd_section;
        sec_len = os->output_section_statement.children.head
                  ->input_section.section->size / opb;
        
        if (!sec || !sec->name) continue;

        if (sec->linked) continue;
        
        if (next && (next->header.type == lang_output_section_statement_enum)) {
          next_sec = next->output_section_statement.bfd_section;
	  if (next->output_section_statement.children.head) {		
            next_len = next->output_section_statement.children.head->
                         input_section.section->size / opb;
          }
          
          if (!next_sec || !next_sec->name) continue;

          if (next_sec->linked) continue;
          
          if (next->output_section_statement.children.head->
                input_section.section->size == 0) continue;
            
          /* if section address and len don't match, continue */
          if ((sec->lma + sec_len + merge_len) != next_sec->lma) continue;
          
          p = strchr(sec->name, '%');
          p2 = strchr(next_sec->name, '%');
          if (p && p2) {
            len = p - sec->name;
            if (len != (unsigned) (p2 - next_sec->name))
              continue;
            match = (strncmp(sec->name, next_sec->name, len) == 0);
          } else
            match = (strcmp(sec->name, next_sec->name) == 0);
            
          if (match && !config.unique_orphan_sections &&
              !pic30_unique_section(sec->name) &&
              (pic30_attribute_map(sec) == pic30_attribute_map(next_sec))) {
              
            if (pic30_debug) {
              printf("  Merging output sections %s and %s\n",
                      sec->name, next_sec->name);
              printf("    %s: addr = %lx, len = %lx\n",
                     sec->name, sec->lma, sec_len + merge_len);
              printf("    %s: addr = %lx, len = %lx\n",
                     next_sec->name, next_sec->lma, next_len);
            }
                     
            merging = 1; /* set a flag to indicate we're merging */
            merge_len += next_len;
            /* Update the section we've merged into */
            next->output_section_statement.children.head->
              input_section.section->output_section = NULL;
            
            lang_add_section (&os->output_section_statement.children,
                              next->output_section_statement.children.head
                              ->input_section.section, NULL,
                              &os->output_section_statement);
            /* remove the merged section from output_bfd */
            remove_section_from_bfd(link_info.output_bfd, next_sec);
          }
        }
      }

      if (merging) {
        if (next == (lang_statement_union_type *) statement_list.tail) {
          /* merging with the end of the list */
          statement_list.tail = &os->header.next;
        }
        os->header.next = next->header.next;  /* unlink the merged statement */
        next = os;                            /* try to merge another one */
      }
    }
  }

} /* allocate_memory() */


static void
report_codeguard_segment_activity(unsigned int memory_index)
{
  if (BOOT_IS_ACTIVE(memory_index))
    printf("  -> boot segment is active:   0x%lx to 0x%lx\n",
           base_address[BOOTx][memory_index], end_address[BOOTx][memory_index]);
  if (SECURE_IS_ACTIVE(memory_index))
    printf("  -> secure segment is active: 0x%lx to 0x%lx\n",
          base_address[SECUREx][memory_index], end_address[SECUREx][memory_index]);
}

/*
 * allocate_program_memory()
 *
 * This function attempts to allocate program memory sections.
 *
 * Called by: allocate_memory()
 *
 * Calls:     build_alloc_section_list()
 *            build_free_block_list()
 *            locate_sections()
 *            pic30_section_list_length() 
 *
 * Returns:   status code
 *            (0 = success)
 */
static int
allocate_program_memory() {
  struct memory_region_struct *region;
  unsigned int has_mask = code|psv|packedflash;
  unsigned int hasnot_mask = auxflash;
  int result = 0;
  struct usable_region_list *usable_regions = NULL, *tail = NULL;
  struct usable_region_list *entry;
  struct pic30_memory *mem_block;

  if (pic30_debug)
    printf("\nBuilding allocation list for region \"program\"\n"
           "  attribute has_mask = 0x%x "
           " hasnot_mask = 0x%x\n", has_mask, hasnot_mask);

  build_alloc_section_list(has_mask,hasnot_mask);
  if (pic30_section_list_length(alloc_section_list) == 0)
    return result;

  region = pic30_all_regions();
  for (; region; region = region->next) {
    /* look for data region */
    if ((region->flags & (SEC_CODE|SEC_READONLY))==(SEC_CODE|SEC_READONLY)) {
       if (pic30_debug)
         printf("\nUsing '%s' as program region\n", region->name_list.name);
       build_free_block_list(region, has_mask, hasnot_mask);
       entry = xmalloc(sizeof(struct usable_region_list));
       if (entry == NULL) { // FAIL
         return 1;
       }
       entry->region = region;
       entry->free_blocks = free_blocks;
       free_blocks = NULL;
       entry->next = NULL;
       if (usable_regions) {
         tail->next = entry;
       } else {
         usable_regions = entry;
       }
       tail = entry;
    }
  }

  /* save the region index because CodeGuard needs it later */
  alloc_region_index = FLASHx;

  if (pic30_debug) {
    report_codeguard_segment_activity(FLASHx);
    pic30_print_section_list(alloc_section_list, "allocation");
  }

#if 0
  if (pic30_debug)
    pic30_print_section_list(unassigned_sections, "unassigned");
#endif

#define FOR_EACH_USABLE_PROGRAM_REGION(X)                            \
  for (entry = usable_regions; entry; entry = entry->next) {         \
    if (pic30_debug)                                                 \
      printf("\nAllocating in region '%s'\n",entry->region->name_list.name);   \
    free_blocks = entry->free_blocks;                                \
    region = entry->region;                                          \
    X;                                                               \
    entry->free_blocks = free_blocks;                                \
  }

  reset_locate_options();
  FOR_EACH_USABLE_PROGRAM_REGION(
    /* most restrictive */
    int tmp_result = 0;
    tmp_result |= locate_sections(address, 0, 0, region,(entry->next!=NULL));
    if (entry->next == NULL) result |= tmp_result;
  )
  FOR_EACH_USABLE_PROGRAM_REGION(
    /* :                 */
    int tmp_result = 0;
    tmp_result |= locate_sections(psv, 0, 0, region,(entry->next != NULL));
    if (entry->next == NULL) result |= tmp_result;
  )
  FOR_EACH_USABLE_PROGRAM_REGION(
    /* least restrictive */
    int tmp_result = 0;
    tmp_result |= locate_sections(all_attr, 0, 0, region,(entry->next != NULL));
    if (entry->next == NULL) result |= tmp_result;
  )

  if (pic30_debug) {
    printf("\nCreating unified free block list for 'program' type memory...\n");
  }
  /* save the free blocks list */
  program_memory_free_blocks = 0;
  mem_block = 0;
  FOR_EACH_USABLE_PROGRAM_REGION(
    if (free_blocks && free_blocks->next) {
      if (program_memory_free_blocks == 0) {
        program_memory_free_blocks = free_blocks;
        mem_block = free_blocks;
      } else {
        mem_block->next = free_blocks->next;
      }
      while (mem_block->next) {
        mem_block = mem_block->next;
        if (pic30_debug) {
          printf("  attaching %lx (%lx bytes) from region '%s'\n",
                 mem_block->addr, mem_block->size,
                 region->name_list.name);
        }
      }
    }
  )

  free_blocks = 0;

  return result;
} /* allocate_program_memory() */

/*
 * allocate_auxflash_memory()
 *
 * This function attempts to allocate auxflash memory sections.
 *
 * Called by: allocate_memory()
 *
 * Calls:     build_alloc_section_list()
 *            build_free_block_list()
 *            locate_sections()
 *            pic30_section_list_length() 
 *
 * Returns:   status code
 *            (0 = success)
 */
static int
allocate_auxflash_memory() {
  struct memory_region_struct *region;
  unsigned int has_mask = auxflash | auxpsv;
  unsigned int hasnot_mask = 0;
  int result = 0;

  if (pic30_debug)
    printf("\nBuilding allocation list for region \"auxflash\"\n"
           "  attribute has_mask = 0x%x "
           "  hasnot_mask = 0x%x\n", has_mask, hasnot_mask);

  build_alloc_section_list(has_mask, hasnot_mask);
  if (pic30_section_list_length(alloc_section_list) == 0)
    return result;

  region = region_lookup ("auxflash");
  build_free_block_list(region, has_mask, hasnot_mask);

  /* save the region index because CodeGuard needs it later */
  alloc_region_index = FLASHx;

  if (pic30_debug) {
    report_codeguard_segment_activity(FLASHx);
    pic30_print_section_list(alloc_section_list, "allocation");
  }

#if 0
  if (pic30_debug)
    pic30_print_section_list(unassigned_sections, "unassigned");
#endif

  reset_locate_options();
  result |= locate_sections(address, 0, 0, region,0);   /* most restrictive  */
  result |= locate_sections(all_attr, 0, 0, region,0);  /* least restrictive */

/* save the free blocks list */
  auxflash_memory_free_blocks = free_blocks;
  free_blocks = 0;

  return result;
} /* allocate_auxflash_memory() */



/*
 * allocate_data_memory()
 *
 * This function attempts to allocate data memory sections.
 *
 * Called by: allocate_memory()
 *
 * Calls:     build_section_list()
 *            build_free_block_list()
 *            locate_sections() 
 *
 * Returns:   status code
 *            (0 = success)
 *
 * Notes: List "data_memory_free_blocks" is needed by
 *        bfd_pic30_finish() to allocate the stack
 *        and heap, so don't exit this function early.
 *
 *        EDS allocation is tricky. Although these sections
 *        can be allocated anywhere, we need to preserve
 *        low memory for the stack. In the first pass,
 *        we exclude memory below the stack limit altogether.
 *        In the second pass, we scan free blocks in reverse
 *        only, which helps when a block crosses the
 *        stack limit boundary. Eventually we'll take
 *        whatever memory we need to, and leave the rest
 *        for the stack and heap.
 *
 *        We also support allocation of stack and heap
 *        that are defined with section attributes.
 *
 *        See comments below for additional info
 *        about stack allocation.
 */
static int
allocate_data_memory() {
  struct memory_region_struct *region;
  struct pic30_section *s;
  unsigned int has_mask = data|bss|persist|stack|heap;
  unsigned int hasnot_mask = code|eedata|auxflash;
  int result = 0;
  bfd_vma max_stack = get_max_stack();
  bfd_vma max_heap;
  struct usable_region_list *usable_regions = NULL, *tail = NULL;
  struct usable_region_list *entry;
  struct pic30_memory *mem_block;

  /* heap */
  if (has_unified_data_model) {
    max_heap = data_end_address();
  } else if (has_psvpag_reference) {
    max_heap = LOCAL_DATA_LIMIT;
  } else max_heap = DATA_BUS_LIMIT;

  if (pic30_debug)
    printf("\nBuilding allocation list for data type regions\n"
           "  attribute has_mask = 0x%x "
           " hasnot_mask = 0x%x\n", has_mask, hasnot_mask);

  build_alloc_section_list(has_mask, hasnot_mask);
  region = pic30_all_regions();
  for (; region; region = region->next) {
    /* look for data region */
    if ((region->flags & SEC_ALLOC) && 
        ((region->not_flags & (SEC_CODE|SEC_READONLY))==(SEC_CODE|SEC_READONLY))) {

       if (pic30_debug)
         printf("\nUsing '%s' as data region\n", region->name_list.name);
       build_free_block_list(region, has_mask, hasnot_mask);
       entry = xmalloc(sizeof(struct usable_region_list));
       if (entry == NULL) { // FAIL
         return 1;
       }
       entry->region = region;
       entry->free_blocks = free_blocks;
       free_blocks = NULL;
       entry->next = NULL;
       if (usable_regions) {
         tail->next = entry;
       } else {
         usable_regions = entry;
       }
       tail = entry;
    }
  }
   
  /*
  ** an initially empty list of blocks that can be shared in data memory
  */
#ifdef PIC30ELF
  pic30_init_memory_list (&shared_data_memory_blocks);
#endif

  /* save the region index because CodeGuard needs it later */
  alloc_region_index = RAMx;

  if (pic30_debug) {
    report_codeguard_segment_activity(RAMx);
    pic30_print_section_list(alloc_section_list, "allocation");
  }

#define FOR_EACH_USABLE_DATA_REGION(X)                               \
  for (entry = usable_regions; entry; entry = entry->next) {         \
    if (pic30_debug)                                                 \
      printf("\nAllocating in region '%s'\n",entry->region->name_list.name);   \
    free_blocks = entry->free_blocks;                                \
    region = entry->region;                                          \
    X;                                                               \
    entry->free_blocks = free_blocks;                                \
  }
    
  
  reset_locate_options();
  FOR_EACH_USABLE_DATA_REGION(
    /* preserved         */
    int tmp_result = 0;
    tmp_result |= locate_sections(all_attr, 0, 1, region,(entry->next!=NULL));
    if (entry->next == NULL) result |= tmp_result;
  )
  FOR_EACH_USABLE_DATA_REGION(
    /* most restrictive  */
    int tmp_result = 0;
    tmp_result |= locate_sections(address, 0, 0, region, (entry->next!=NULL));
    if (entry->next == NULL) result |= tmp_result;
  )
  FOR_EACH_USABLE_DATA_REGION(
    /* :                 */
    int tmp_result = 0;
    tmp_result |= locate_sections(dma, 0, 0, region,(entry->next!=NULL));
    if (entry->next == NULL) result |= tmp_result;
  )
  FOR_EACH_USABLE_DATA_REGION(
    int tmp_result = 0;
    tmp_result |= locate_sections(near, 0, 0, region,(entry->next!=NULL));
    if (entry->next == NULL) result |= tmp_result;
  )
  FOR_EACH_USABLE_DATA_REGION(
    /* :                 */
    int tmp_result = 0;
    tmp_result |= locate_sections(xmemory,0, 0,  region,(entry->next!=NULL));
    if (entry->next == NULL) result |= tmp_result;
  )
  FOR_EACH_USABLE_DATA_REGION(
    /* :                 */
    int tmp_result = 0;
    tmp_result |= locate_sections(ymemory, 0, 0, region,(entry->next!=NULL));
    if (entry->next == NULL) result |= tmp_result;
  )
  /* less restrictive  */
  FOR_EACH_USABLE_DATA_REGION(
    int tmp_result = 0;
    tmp_result |= locate_sections(all_attr, eds|stack|heap, 0, region,(entry->next!=NULL));
    if (entry->next == NULL) result |= tmp_result;
  )

  /* user-defined heap */
  set_locate_options(has_psvpag_reference ? EXCLUDE_HIGH_ADDR : 0, max_heap);
  FOR_EACH_USABLE_DATA_REGION(
    int tmp_result = 0;
    tmp_result |= locate_sections(heap, 0, 0, region,(entry->next!=NULL));
    if (entry->next == NULL) result |= tmp_result;
  )

  /* user-defined stack */
  set_locate_options(has_psvpag_reference ? EXCLUDE_HIGH_ADDR : 0, max_stack);
  FOR_EACH_USABLE_DATA_REGION(
    int tmp_result = 0;
    tmp_result |= locate_sections(stack, 0, 0, region,(entry->next!=NULL));
    if (entry->next == NULL) result |= tmp_result;
  )

  /* EDS, first pass */
  if (pic30_debug)
    printf("\nEDS, first pass:\n");
  set_locate_options(EXCLUDE_LOW_ADDR, max_stack);
  FOR_EACH_USABLE_DATA_REGION(
    int tmp_result = 0;
    tmp_result |= locate_sections(all_attr, stack|heap, 0, region,(entry->next!=NULL));
    if (entry->next == NULL) result |= tmp_result;
  )

  /* EDS, second pass */
  if (pic30_debug)
    printf("\nEDS, second pass:\n");
  set_locate_options(FAVOR_HIGH_ADDR, 0);
  FOR_EACH_USABLE_DATA_REGION(
    int tmp_result = 0;
    tmp_result |= locate_sections(all_attr, stack|heap, 0, region,(entry->next!=NULL));
    if (entry->next == NULL) result |= tmp_result;
  )

  /* if any not previously linked sections are left in the allocation list,
     report an error */
  s = alloc_section_list;
  if ( s && s->next ) {
    if (report_allocation_error(s->next)) result |= 1; 
  }

  /* save the free blocks list, concat all regions together */
  if (pic30_debug) {
    printf("\nCreating unified free block list for 'data' type memory...\n");
  }
  mem_block = 0;
  data_memory_free_blocks = 0;
  FOR_EACH_USABLE_DATA_REGION(
    if (free_blocks && free_blocks->next) {
      if (data_memory_free_blocks == 0) {
        data_memory_free_blocks = free_blocks;
        mem_block = free_blocks;
      } else {
        mem_block->next = free_blocks->next;
      }
      while (mem_block->next) {
        mem_block = mem_block->next;
        if (pic30_debug) {
          printf("  attaching %lx (%lx bytes) from region '%s'\n",
                 mem_block->addr, mem_block->size,
                 region->name_list.name);
        }
      }
    }
  )
  free_blocks = 0;

  return result;
} /* allocate_data_memory() */


/*
 * allocate_default_stack()
 *
 * If a stack section was not explicitly defined,
 * and the stack init symbols were not defined,
 * then this function is called to allocate the
 * largest stack possible from data_memory_free_blocks.
 *
 * Called by: bfd_pic30_finish()
 *
 * Calls:     pic30_add_to_memory_list()
 *            pic30_remove_from_memory_list()
 *
 * Returns:   sets global vars stack_base, stack_limit
 *
 * Notes: The free blocks list is sorted in order
 *        of increasing size
 *
 *        The calling function will confirm that
 *        the resulting stack satisfies the minimum
 *        size requirements.
 *
 *        On non-eds machines, the full 16-bit address
 *        space is available for the stack.
 *
 *        On early eds machines such as 24F..DA, the
 *        stack is limited to local (non-paged) data memory.
 *
 */

static void
allocate_default_stack() {
  struct pic30_memory *big_block = 0;
  struct pic30_memory *b, *next;
  bfd_vma under,over;

  bfd_vma max_addr = get_max_stack();
  
  if (pic30_debug) {
    printf("  maximum stack address is %#lx\n", max_addr);
  }

  if (data_memory_free_blocks == 0)
    einfo(_("%P%X%F:  Link Error: Could not allocate default stack"
            " because no data memory is available\n\n"));

  /* if a free block straddles the upper limit, divide it */
  for (b = data_memory_free_blocks; b != NULL; b = next) {
    next = b->next;
    if ((b->addr < max_addr) && ((b->addr + b->size) > max_addr)) {

      if (pic30_debug)
        printf("  free block at %lx crosses upper stack limit (%lx)\n",
               b->addr, max_addr);

      under = max_addr - b->addr;
      over  = (b->addr + b->size) - max_addr;

      pic30_add_to_memory_list(data_memory_free_blocks, b->addr, under);
      pic30_add_to_memory_list(data_memory_free_blocks, max_addr, over);
      pic30_remove_from_memory_list(data_memory_free_blocks, b);
      break;
    }
  }

  /* find the largest block that qualifies */
  for (b = data_memory_free_blocks; b != NULL; b = next) {
    if (pic30_debug) {
      printf("  consider free block at %#lx, len = %#x\n",
             b->addr, b->size);
    }
    if (b->addr < max_addr)
      big_block = b;
    next = b->next;
  }

  if (pic30_is_isav32_machine(global_PROCESSOR)) {
    stack_base  = (big_block->addr + 3) & ~3;  /* must be quad-byte aligned */
    stack_limit = (big_block->addr + big_block->size
                   - pic30_stackguard_size + 3) & ~3;
  } else {
  stack_base  = (big_block->addr + 1) & ~1;  /* must be even */
    stack_limit = (big_block->addr + big_block->size
                 - pic30_stackguard_size + 1) & ~1;
  }

  if (pic30_debug) {
    printf("  selecting block at %lx\n", big_block->addr);
    printf("  stack base = %x, stack limit = %x\n\n", stack_base, stack_limit);
  }

  /* remove the block that we just used */
  pic30_remove_from_memory_list(data_memory_free_blocks, big_block);
}


/*
 * allocate_eedata_memory()
 *
 * This function attempts to allocate eedata memory sections.
 *
 * Called by: allocate_memory()
 *
 * Calls:     build_section_list()
 *            build_free_block_list()
 *            locate_sections() 
 *
 * Returns:   status code
 *            (0 = success)
 */
static int
allocate_eedata_memory() {
  struct memory_region_struct *region;
  unsigned int has_mask = eedata;
  unsigned int hasnot_mask = 0;
  int result = 0;

  if (pic30_debug)
    printf("\nBuilding allocation list for region \"eedata\"\n"
           "  attribute has_mask = 0x%x "
           " hasnot_mask = 0x%x\n", has_mask, hasnot_mask);

  build_alloc_section_list(has_mask, hasnot_mask);
  if (pic30_section_list_length(alloc_section_list) == 0)
    return result;

  region = region_lookup ("eedata");
  build_free_block_list(region, has_mask, hasnot_mask);

  /* save the region index because CodeGuard needs it later */
  alloc_region_index = EEPROMx;

  if (pic30_debug) {
    report_codeguard_segment_activity(EEPROMx);
    pic30_print_section_list(alloc_section_list, "allocation");
  }

  reset_locate_options();
  result |= locate_sections(address, 0, 0, region,0);   /* most restrictive  */
  result |= locate_sections(all_attr, 0, 0, region,0);  /* least restrictive */

  return result;
} /* allocate_eedata_memory() */


/*
 * allocate_user_memory()
 *
 * This function attempts to allocate sections
 * in user-defined memory regions.
 *
 * Called by: allocate_memory()
 *
 * Calls:     build_section_list()
 *            build_free_block_list()
 *            locate_sections() 
 *
 * Returns:   status code
 *            (0 = success)
 */
static int
allocate_user_memory() {
  unsigned int opb = bfd_octets_per_byte (link_info.output_bfd);
  struct memory_region_struct temp_region;
  struct pic30_section *r, *rnext, *s, *snext, *ss, *ssnext, *ssprev;
  const char *region_name;
  int result = 0;

  /* loop through any user-defined regions */
  for (r = memory_region_list; r != NULL; r = rnext) {
  
    rnext = r->next;
    if (r->sec == 0) continue;
    
    region_name = r->sec->name + strlen(memory_region_prefix);
    if (pic30_debug)
      printf("\nBuilding allocation list for user-defined region \"%s\""
             " origin = %lx, length = %lx\n",
             region_name, r->sec->vma, r->sec->lma); /* region length is stored in lma field */

    /* the assembler will not accept a memory region
       definition with length 0... if we see one here,
       it indicates that a section has referenced a
       region that was never defined */
    if (r->sec->lma == 0)
      einfo(_("%s: Link Error: Memory region \'%s\' has been referenced"
              " but not defined\n"),
              r->sec->owner->filename, region_name);

    /* loop through the list of memory-type sections..
       note that only two fields of this list are
       used (file, sec) and they are both (char *) */
    for (s = user_memory_sections; s != NULL; s = snext) {

      snext = s->next;
      if (!s->file || !s->sec) continue;
      
#if 0
      printf("s->file =%s, s->sec = %s \n", (char *) s->file, (char *) s->sec);
#endif
      if (strcmp((char *)s->file, region_name) == 0) {
        /* if this section has been assigned to the current region,
           add it to the allocation list */
        ssprev = unassigned_sections;
        for (ss = ssprev; ss != NULL; ss = ssnext) {
          ssnext = ss->next;
          if (ss->sec && strcmp((char *) s->sec, ss->sec->name) == 0) {
            if (pic30_debug)
              printf("  input section \"%s\", len = %lx, flags = %x, attr = %x\n",
                     ss->sec->name, ss->sec->size/opb,
                     ss->sec->flags, ss->attributes);

            insert_alloc_section(alloc_section_list, ss);
            ssprev->next = ssnext; /* unlink it from unassigned_sections */
          } else
            ssprev = ss;
        }
      }
    }
    
    if (pic30_section_list_length(alloc_section_list) == 0) {
      if (pic30_debug)
        printf("\n  (none)\n");
      continue;
    }

    temp_region.name_list.name = (char *) region_name;
    temp_region.next = &temp_region;
    temp_region.origin_exp = NULL; /* new in binutils-v2.32 */
    temp_region.origin = r->sec->vma;
    temp_region.length = r->sec->lma; /* region length is stored in lma field */
    /* don't expect to need these.. */
    temp_region.length_exp = NULL; /* new in binutils-v2.32 */
    temp_region.current = r->sec->vma;
    temp_region.last_os = NULL; /* new in binutils-v2.32 */
    temp_region.flags = 0;
    temp_region.not_flags = 0;
    temp_region.had_full_message = FALSE;

    if (free_blocks)
      pic30_free_memory_list(&free_blocks);
    pic30_init_memory_list(&free_blocks);
  
    /* start with entire region as a free block */
    pic30_add_to_memory_list(free_blocks, temp_region.origin,
                             temp_region.length);

    if (pic30_debug) {
      pic30_print_section_list(alloc_section_list, "allocation");
    }
  
    reset_locate_options();
    result |= locate_sections(address, 0, 0, &temp_region,0);
    result |= locate_sections(all_attr, 0, 0, &temp_region,0);
  }

  return result;
} /* allocate_user_memory() */


/*
 * A list of sections, other than PSV sections,
 * that must always be grouped.
 */
static char * group_sections[] = {
  ".user_init",
  0, };

static int
is_group_section(asection *sec)
{
  int i = 0;
#if 0
  if (PIC30_IS_ABSOLUTE_ATTR(sec) && !pic30_reserve_const)
    return 0;
#endif
  if (PIC30_IS_PSV_ATTR(sec))
    return 1;

  if (PIC30_IS_AUXPSV_ATTR(sec))
    return 1;

  while (group_sections[i]) {  /* null terminated list */
    if (strcmp(sec->name, group_sections[i++]) == 0)
      return 1;
  }
  return 0;
}

static bfd_vma
group_section_size(struct pic30_section *g)
{
  unsigned int opb = bfd_octets_per_byte (link_info.output_bfd);
  struct pic30_section *s,*next;
  bfd_vma result = 0;
  
  for (s = alloc_section_list; s != NULL; s = next) {
    next = s->next;
    if (s->sec == 0)
      continue;
    if (s->sec && (strcmp(g->sec->name, s->sec->name) == 0)) 
      result += s->sec->size / opb;
  }
  return result;
}

static bfd_vma
group_section_alignment(struct pic30_section *g)
{
  struct pic30_section *s,*next;
  bfd_vma result = g->sec->alignment_power;

  for (s = alloc_section_list; s != NULL; s = next) {
    next = s->next;
    if (s->sec == 0)
      continue;
    if (s->sec && (strcmp(g->sec->name, s->sec->name) == 0)) {

      if (s->sec->alignment_power > result)
        result = s->sec->alignment_power;
    }
  }
  return result;
}

#define OUTSIDE_REGION(addr, len, region)          \
  (((addr + len) <= region->origin)                \
   || (addr >= (region->origin + region->length)))

#define ACROSS_REGION(addr, len, region)                            \
  (((addr < region->origin) && ((addr + len) > region->origin)) ||  \
   ((addr < (region->origin + region->length)) &&                   \
    ((addr + len) > (region->origin + region->length))))

/*
 * locate_group_section()
 *
 * Helper function to locate a grouped section
 * using the free_blocks list.
 *
 * Called by: locate_sections()
 *
 * Returns:   status code
 *            (0 = success)
 */
 
 static int
 locate_group_section(struct pic30_section *s,
                        struct memory_region_struct *region,
                        int more_regions_to_scan) {

  struct pic30_memory *b;
  bfd_vma len = group_section_size(s);
  bfd_vma align_val = group_section_alignment(s);
  bfd_vma the_addr = s->sec->lma;
  int result = 0;

  if (pic30_debug)
    printf("  group section \"%s\", total size = %lx\n",
           s->sec->name, len);
  if ((PIC30_IS_PSV_ATTR(s->sec) || PIC30_IS_AUXPSV_ATTR(s->sec)) && 
      (len > 0x8000)) {
    einfo(_("%X Link Error: PSV or AUXPSV section \'%s\' exceeds 32K bytes"
            " (actual size = %u).\n"),
          s->sec->name, len);
    pic30_remove_group_from_section_list(alloc_section_list,s->sec->name);
    return 1;
  }

  /* validate boot segment */
  if (PIC30_IS_BOOT_ATTR(s->sec) && !BOOT_IS_ACTIVE(alloc_region_index) &&
      (s->sec->linked ==0)) {
    einfo(_("%X Link Error: Could not allocate section \'%s\'"
            " because boot segment has not been defined\n"),s->sec->name);
    pic30_remove_group_from_section_list(alloc_section_list,s->sec->name);
    return 1;
  }

  /* validate secure segment */
  if (PIC30_IS_SECURE_ATTR(s->sec) && !SECURE_IS_ACTIVE(alloc_region_index) &&
      (s->sec->linked ==0)) {
    einfo(_("%X Link Error: Could not allocate section \'%s\'"
            " because secure segment has not been defined\n"),s->sec->name);
    pic30_remove_group_from_section_list(alloc_section_list,s->sec->name);
    return 1;
  }
#ifdef PIC30ELF  
  if (pic30_reserve_const) {
    if (reserve_const_arg == 0)
      reserve_const_arg = 0x8000;
    len = reserve_const_arg;
  }
#endif
  /* validate absolute, paged sections */
  if (PIC30_IS_ABSOLUTE_ATTR(s->sec) && PIC30_IS_PAGE_ATTR(s->sec)) {
    int valid;
    bfd_vma limit;

    if (PIC30_IS_CODE_ATTR(s->sec) || PIC30_IS_AUXFLASH_ATTR(s->sec)) {
      limit  = NEXT_PROG_PAGE(s->sec->lma);
      valid = VALID_PROG_PAGE(s->sec->lma, len);
    } else {
      limit  = NEXT_EDS_PAGE(s->sec->lma);
      valid = VALID_EDS_PAGE(s->sec->lma, len);
    }
    if (!valid) {
      einfo(_("%X Link Error: Absolute, paged section \'%s\' crosses"
              " page boundary at 0x%v.\n"), s->sec->name, limit);
      pic30_remove_group_from_section_list(alloc_section_list,s->sec->name);
      return 1;
    }
  }

  /* look for tricky user error */
  if (PIC30_IS_ABSOLUTE_ATTR(s->sec) && ACROSS_REGION(the_addr, len, region))
        einfo(_(" Link Warning: absolute section \'%s\' crosses"
                " the boundary of region %s.\n"),
              s->sec->name, region->name_list.name);

  if (len == 0)
    update_group_section_info(0,s,region); /* trivial case */
  else if (PIC30_IS_ABSOLUTE_ATTR(s->sec) &&
           OUTSIDE_REGION(the_addr, len, region)) {
    update_group_section_info(the_addr, s, region);  /* falls outside region */
  }
  else {                          /* locate using free_blocks list */
    b = select_free_block(s, len, align_val, region,more_regions_to_scan);
    if (b) {
      the_addr = b->addr + b->offset;
      update_group_section_info(the_addr,s,region);
      create_remainder_blocks(free_blocks, b, len, region,s);
      remove_free_block(b);
    } else {
      if (locate_options != NO_LOCATE_OPTION) {
        if (pic30_debug)
          printf("    \"%s\" location declined\n", s->sec->name);
        return 0;
      }
      result |= 1;
    }
  }

  if (pic30_debug)
    printf("    removing group from allocation list\n");
  if ((result == 0) || (more_regions_to_scan == 0)) {
    pic30_remove_group_from_section_list(alloc_section_list,s->sec->name);
  }

  return result;
} /* locate_group_section() */
 
 
/*
 * locate_single_section()
 *
 * Helper function to locate a single section
 * using the free_blocks list.
 *
 * Called by: locate_sections()
 *
 * Returns:   status code
 *            (0 = success)
 */
 
 static int
 locate_single_section(struct pic30_section *s,
                       struct memory_region_struct *region,
                       int more_regions_to_scan) {

  unsigned int opb = bfd_octets_per_byte (link_info.output_bfd);
  bfd_vma len;
  struct pic30_memory *b;

#if 0 
  if (PIC30_IS_PACKEDFLASH_ATTR(s->sec))
   s->sec->size = s->sec->size =((s->sec->size * 3) / 4);
#endif

  len = s->sec->size / opb;
  bfd_vma align_val = s->sec->alignment_power;
  bfd_vma addr = s->sec->lma;
  int result = 0;
  /* validate the size of PSV sections */
  if ((PIC30_IS_PSV_ATTR(s->sec) || PIC30_IS_AUXPSV_ATTR(s->sec)) && 
      (len > 0x8000)) {
    einfo(_("%X Link Error: PSV or AUXPSV section \'%s\' exceeds 32K bytes"
            " (actual size = %u).\n"),
          s->sec->name, len);
    pic30_remove_from_section_list(alloc_section_list,s);
    return 1;
  }

  /* validate boot segment */
  if (PIC30_IS_BOOT_ATTR(s->sec) && !BOOT_IS_ACTIVE(alloc_region_index)) {
    einfo(_("%X Link Error: Could not allocate section \'%s\'"
            " because boot segment has not been defined\n"),s->sec->name);
    pic30_remove_from_section_list(alloc_section_list,s);
    return 1;
  }

  /* validate secure segment */
  if (PIC30_IS_SECURE_ATTR(s->sec) && !SECURE_IS_ACTIVE(alloc_region_index)) {
    einfo(_("%X Link Error: Could not allocate section \'%s\'"
            " because secure segment has not been defined\n"),s->sec->name);
    pic30_remove_from_section_list(alloc_section_list,s);
    return 1;
  }

  /* validate absolute, paged sections */
  if (PIC30_IS_ABSOLUTE_ATTR(s->sec) && PIC30_IS_PAGE_ATTR(s->sec)) {
    int valid;
    bfd_vma limit;

    if (PIC30_IS_CODE_ATTR(s->sec) || PIC30_IS_AUXFLASH_ATTR(s->sec)) {
      limit  = NEXT_PROG_PAGE(s->sec->lma);
      valid = VALID_PROG_PAGE(s->sec->lma, len);
    } else {
      limit  = NEXT_EDS_PAGE(s->sec->lma);
      valid = VALID_EDS_PAGE(s->sec->lma, len);
    }
    if (!valid) {
      einfo(_("%X Link Error: Absolute, paged section \'%s\' crosses"
              " page boundary at 0x%v.\n"), s->sec->name, limit);
      pic30_remove_from_section_list(alloc_section_list,s);
      return 1;
    }
  }

  /* look for tricky user error */
  if (PIC30_IS_ABSOLUTE_ATTR(s->sec) && ACROSS_REGION(addr, len, region))
        einfo(_(" Link Warning: absolute section \'%s\' crosses"
                " the boundary of region %s.\n"),
              s->sec->name, region->name_list.name);

  if (len == 0)
    update_section_info(0,s,region);       /* trivial case */
  else if (PIC30_IS_ABSOLUTE_ATTR(s->sec) &&
           OUTSIDE_REGION(addr, len, region)) {
    update_section_info(addr, s, region);  /* falls outside region */
  }
  else {                          /* locate using free_blocks list */
    b = select_free_block(s, len, align_val, region, more_regions_to_scan);
    if (b) {
      addr = b->addr + b->offset;
      update_section_info(addr,s,region);
      create_remainder_blocks(free_blocks, b, len, region, s);
#ifdef PIC30ELF
      if (PIC30_IS_SHARED_ATTR(s->sec) && PIC30_IS_ABSOLUTE_ATTR(s->sec)) {
        pic30_add_to_memory_list(shared_data_memory_blocks,
                                 b->addr+b->offset, len);
      }
#endif
      remove_free_block(b);
    } else {
      if (locate_options != NO_LOCATE_OPTION) {
        if (pic30_debug)
          printf("    \"%s\" location declined\n", s->sec->name);
        return 0;
      }
      result |= 1;
    }
  }

  if ((result == 0) || (more_regions_to_scan == 0)) {
    if (pic30_debug && s->sec->size)
      printf("    removing from allocation list\n");
    pic30_remove_from_section_list(alloc_section_list,s);
  }

  return result;
} /* locate_single_section() */
 
static int pic30_symbol_names_match(char *sym1, char *sym2) {
  // the symbol name has a genral form of
  // <symbol_name>[.local variant]
  // the local variant is an integer and will be different depending upon
  //   the position of the definition (it is a unique identifier for function-
  //   local statics
  //
  // ignore this in the comparison of the symbol, because it could have moved
  //   scope (or otherwise have a different number)
  //
  // when we use this function, we have already validated that there is a 
  // section match
  // 
  // if the source code puts multiple same named function-local static into
  //   the same section, this code will fail to match properly (but will
  //   be detected)
  //
  // C variable names don't have 'dots' in them, but try to validate it anyway
  char *dot1, *dot2;
  char *c,*dot;
  int pass = 0;

  c = sym1;
  do {
    dot = 0;
    for (; *c; c++) {
      if (dot == 0) {
        if (*c == '.') {
          dot = c;
        }
      } else {
        if ((*c >= '0') && (*c <= '9')) {
         (void)(0);  // keep looking
        } else {
          dot = 0;
          c--; // check this char again
        }
      }
    }
    if (pass == 0) {
      dot1 = dot;
      c = sym2;
      pass++;
    } else {
      dot2 = dot;
      c = 0;
    }
  } while (c);
  if (dot1) *dot1 = 0;
  if (dot2) *dot2 = 0;
  pass = strcmp(sym1,sym2);
  if (dot1) *dot1 = '.';
  if (dot2) *dot2 = '.';
  return (pass == 0);
}

static void locate_preserved_section(struct pic30_section *s,
                                     struct memory_region_struct *region) {
#ifndef PIC30ELF
  return;
#else

  if ((s->sec->user_set_vma != 0) || (s->sec->absolute != 0)) {
    /* user set VMA (from gld file) or otherwise absolute... we have an
       address - already allocated */
    return;
  }
  if (PIC30_IS_UPDATE_ATTR(s->sec)) {
    /* marked with update, definately not preserved */
    return;
  }

  if ((preserved_sections) && (PIC30_IS_PRESERVED_ATTR(s->sec))) {
    asymbol **preserved_symbols;
    long num;
    long int i,j;
    asymbol *section_symbol;  /* ignore warning */
    long int section_lma = 0; /* should be bfd_vma, but we're using -1 as a flag, below */
    asymbol *last_file_symbol = 0;
    asymbol *matched_symbol = 0;
    asymbol *matched_file = 0;
    int section_has_symbols = 0;

    preserved_symbols = bfd_pic30_load_symbol_table(s->sec->owner, &num);
    if (pic30_debug) {
      printf("   preserving symbols for %s\n",s->sec->name);
    }
    for (i = 0; i < num; i++) {
      if (preserved_symbols[i]->flags & BSF_FILE) {
        last_file_symbol = preserved_symbols[i];
        continue;
      }
      if (preserved_symbols[i]->section != s->sec) {
        continue;
      }
      if (preserved_symbols[i]->flags & BSF_SECTION_SYM) {
        section_symbol = preserved_symbols[i];
      } else {
        /* try to validate the symbol address */
        struct pic30_section *p;
        
        if (pic30_debug) {
          printf("   looking for %s (0x%8.8x)\n",
                 preserved_symbols[i]->name, preserved_symbols[i]->flags);
        }
        section_has_symbols = 1;
        // matched_symbol = 0;
        for (p = preserved_sections; p; p = p->next) {
          asymbol **previous_syms;
          long int previous_num;
          asymbol *last_previous_file_symbol = 0;
          int warning = 0;
#define FLAGS_THAT_MATTER (SEC_READONLY | SEC_CODE | SEC_DATA | SEC_ROM)

          if (p->sec == 0) continue;
          if ((p->sec->flags & FLAGS_THAT_MATTER) != 
              (s->sec->flags & FLAGS_THAT_MATTER)) {
            if (pic30_debug) {
              printf("Flags do not match: %s (%8.8x) != %s (%8.8x)\n", 
                     p->sec->name ? p->sec->name : "NULL", p->sec->flags, 
                     s->sec->name ? s->sec->name : "NULL", s->sec->flags);
            }
            continue;
          }
          previous_syms = bfd_pic30_load_symbol_table(p->sec->owner,
                                                            &previous_num);
          for (j = 0; j < previous_num; j++) {
            /* consider symbols in the current preserved section */
            if (previous_syms[j]->flags & BSF_FILE) {
              last_previous_file_symbol = previous_syms[j];
            }
            if (strcmp(previous_syms[j]->section->name,
                       p->sec->name) != 0) continue;
            if (pic30_symbol_names_match(previous_syms[j]->name,
                                         preserved_symbols[i]->name)) {
              if (pic30_debug) {
                printf("   match %s 0x%lx from %s\n",
                       previous_syms[j]->name, previous_syms[j]->value,
                       previous_syms[j]->section->name);
              }
#if 0
              /* merging sections in a previous output file can make this
                 appear to be true.... */
              if (s->sec->size != previous_syms[j]->section->size) {
                einfo("Warning: matched preserved section '%s' has changed "
                      "size\n", s->sec->name);
              }
#endif
              if (section_lma == 0) {
                /* work-out preserved section lma based upon
                     the current preserved symbol offset and the
                     matched symbols offset within the previous section */
                section_lma = previous_syms[j]->value +
                              previous_syms[j]->section->lma -
                              preserved_symbols[i]->value;
                matched_symbol = previous_syms[j];
                matched_file = last_previous_file_symbol;
                if (pic30_debug)
                  printf("  currently assigned lma: 0x%lx\n", section_lma);
              } else {
                /* we already picked a section lma; make sure all symbols
                   fit */
                if ( (bfd_vma) section_lma != previous_syms[j]->value +
                              previous_syms[j]->section->lma -
                              preserved_symbols[i]->value) {
                  if (warning == 0) {
                    fprintf(stderr,
                            "Warning: cannot match address for symbol '%s'\n"
                            "\t in preserved section '%s'\n"
                            "\t from file '%s'\n", 
                            preserved_symbols[i]->name,
                            s->sec->name,
                            last_file_symbol ? last_file_symbol->name : 
                                               "unknown");
                    warning = 1;
                    if (pic30_debug)
                    fprintf(stderr,"Choices are:\n"
                                   "\taddress: 0x%4.4lx file: '%s'\n",
                                   matched_symbol->section->lma,
                                   matched_file->name);
                  }
                  if (pic30_debug)
                  fprintf(stderr,"\taddress: 0x%4.4lx file: '%s'\n",
                                  previous_syms[j]->section->lma + previous_syms[j]->value,
                                  last_previous_file_symbol->name);

                  if (pic30_debug) {
                    printf("Warning: cannot match address for symbol"
                           " '%s'\n\tin preserved section %s\n",
                           previous_syms[j]->name,s->sec->name);
                    printf("  currently assigned lma: 0x%lx\n", section_lma);
                    printf("  calculated lma: 0x%lx\n",
                              previous_syms[j]->value +
                                previous_syms[j]->section->lma -
                                preserved_symbols[i]->value);
                  }
                  section_lma = -1;
                }
              }
            }
          }
        }
      }
      if (pic30_debug) {
        printf("   %s 0x%lx %s\n",
          preserved_symbols[i]->name, preserved_symbols[i]->value,
          preserved_symbols[i]->flags & BSF_SECTION_SYM ?
            "SECTION SYM" : "");
      }
    }
    if (section_has_symbols == 0) return;
    if (section_lma == 0) {
      einfo("Warning: cannot find address for preserved section:"
                     " '%s'\n",
              s->sec->name ? s->sec->name : "unnamed section");
    } else if (section_lma == -1) {
      einfo("%FError: cannot match an address for preserved section\n\t"
            "'%s'\n", s->sec->name);
    } else {
      /* matched addresses */
      if (pic30_debug) {
         printf("   assigning lma %lx\n", section_lma);
      }
      s->sec->lma = section_lma;
      PIC30_SET_ABSOLUTE_ATTR(s->sec);
      s->sec->linker_generated = 1;
      locate_single_section(s, region, 0);
    }
  }
  return;
#endif
}
 
/*
 * locate_sections()
 *
 * This function attempts to locate all sections
 * in the section list that match a specified
 * attribute mask. The block argument specifies
 * attributes that disqualify a section.
 *
 * Called by: allocate_program_memory()
 *            allocate_data_memory()
 *            allocate_eedata_memory()
 *            allocate_auxflash_memory()
 *
 * Calls:     select_free_block()
 *            update_section_info()
 *            create_remainder_blocks()
 *            remove_free_block()
 *            remove_alloc_section()
 *
 * Returns:   status code
 *            (0 = success)
 */
static int
locate_sections(unsigned int mask, unsigned int block,
                unsigned int is_preserved, struct memory_region_struct *region,
                int more_regions_to_scan) {

  unsigned int opb = bfd_octets_per_byte (link_info.output_bfd);
  struct pic30_section *s,*next;
  int result = 0;

  if (pic30_debug) {
    printf("\nLocating sections with mask %x, but not %x\n", mask, block);
    printf("  locate_options = %x, exclude_addr = %lx\n",
           locate_options, exclude_addr);
  }

  for (s = alloc_section_list; s != NULL; s = next) {
  
    next = s->next;
    if (s->sec == 0) continue;
    if (s->sec && 
          (PIC30_IS_DATA_ATTR(s->sec) ||PIC30_IS_BSS_ATTR(s->sec)) &&
          !PIC30_IS_ABSOLUTE_ATTR(s->sec) && 
          (s->sec->linked == 1)) {
      update_section_info(s->sec->lma, s, region);
    } else {
      if (is_preserved) {
        locate_preserved_section(s,region);
      } else if (s->sec && (s->attributes & mask) &&
          ((s->attributes & block) == 0)) {
        bfd_vma len = s->sec->size / opb;

        if (PIC30_IS_PSV_ATTR(s->sec) || PIC30_IS_AUXPSV_ATTR(s->sec)) {
          /* Make sure we don't allocate an odd-length section */
          /* binutils-v2.32 required this code.. why? - GM 9/7/21 */
          if ((len % 2) != 0) {
            s->sec->size += 2;
            len += 1;
          }
        }

        if ((pic30_debug) && (len > 0)) {
          char *attr = pic30_section_attr_string(s->sec);

          printf("  input section \"%s\", len = %lx, flags = %x, %s\n",
                 s->sec->name, len, s->sec->flags, attr ? attr : "");
          if (attr) free(attr);
        }
      
        if (is_group_section(s->sec))
          result |= locate_group_section(s, region, more_regions_to_scan);
        else
          result |= locate_single_section(s, region, more_regions_to_scan);
      }
    }
  }
  return result;
} /* locate_sections() */


/*
 * next_aligned_address()
 */
static bfd_vma
next_aligned_address( bfd_vma start, unsigned int align_power) {

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshift-negative-value"
  bfd_vma mask = ~((~0) << align_power);
#pragma GCC diagnostic pop
  return (bfd_vma) ((start + 1 + mask) & (~mask));
}


/*
 * prev_aligned_address()
 */
static bfd_vma
prev_aligned_address( bfd_vma start, unsigned int align_power) {

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshift-negative-value"
  bfd_vma mask = ~((~0) << align_power);
#pragma GCC diagnostic pop
  if (start < mask)
    return (bfd_vma) 0;
  else
    return (bfd_vma) ((start - 1 - mask) & (~mask));
}

static void
confirm_dma_range_defined() {
  struct bfd_link_hash_entry *h;
  static bfd_boolean err_reported = FALSE;

  if ((!dma_base_defined) || (!dma_end_defined)) {

    h = bfd_pic30_is_defined_global_symbol(str3);
    if (h) {
      dma_base = h->u.def.value;
      dma_base_defined = TRUE;
    }

    h = bfd_pic30_is_defined_global_symbol(str4);
    if (h) {
      dma_end = h->u.def.value;
      dma_end_defined = TRUE;
    }
    if ((!dma_base_defined) || (!dma_end_defined)) {
      if (!err_reported) {
        einfo(_(str1));
        err_reported = TRUE;
      }
    }
  }
}

/*
 * select_free_block()
 *
 * This function attempts to select a suitable
 * free block for a specified section. For each
 * free block, the section is positioned two ways:
 *  (1) as close to the starting address as possible,
 *  (2) as close to the ending address as possible.
 *
 * The position that leaves the largest unused
 * remainder space is selected. If a suitable free
 * block is found, the block offset is written into
 * the free block structure and the structure is returned.
 *
 * The selection algorithm can be fine-tuned by previous
 * calls to set_locate_options().
 *
 * Called by: locate_sections()
 *            locate_group_sections()
 *
 * Calls:     pic30_static_assign_memory()
 *            next_aligned_address()
 *            prev_aligned_address()
 *
 * Returns:   suitable free block, or 0
 */

static struct pic30_memory *
select_free_block(struct pic30_section *s, unsigned int len,
                  unsigned int align_power, struct memory_region_struct *region,
                  int more_regions_to_scan) {

  unsigned int opb = bfd_octets_per_byte (link_info.output_bfd);
  const char *err_str1 = "Link Error: Could not allocate section";
  const char *err_str2 = "Link Warning: Allocating section";
  struct bfd_link_hash_entry *h;
  struct pic30_memory *b = 0;
  bfd_vma option1, old_option1, new_option1, limit1;
  bfd_vma option2, old_option2, new_option2, limit2;
  bfd_boolean option1_valid, option2_valid;

  /*
   * If the section is absolute, call the static function
   */
  if (PIC30_IS_ABSOLUTE_ATTR(s->sec)) {

    /* avoid BOOT if we don't belong there */
    /* (CAW) linker_generated currently implies a vector table; the aivt
         belongs at the end of the boot sector
       This test didn't happen before... */
    if (BOOT_IS_ACTIVE(alloc_region_index) &&
        (s->sec->linker_generated == 0) &&
        !PIC30_IS_BOOT_ATTR(s->sec) &&
        OVERLAPS_BOOT(s->sec->lma, len))
      einfo(_("%s \'%s\' at 0x%v"
              " overlaps the boot segment\n"),
            err_str2, s->sec->name, s->sec->lma);

    /* avoid SECURE if we don't belong there */
    if (SECURE_IS_ACTIVE(alloc_region_index) &&
        !PIC30_IS_SECURE_ATTR(s->sec) &&
        OVERLAPS_SECURE(s->sec->lma, len))
      einfo(_("%s \'%s\' at 0x%v"
              " overlaps the secure segment\n"),
            err_str2, s->sec->name, s->sec->lma);

     /* This check is necessary to prevent the linker from
        placing code at the last word of program memory */

     if ((PIC30_IS_CODE_ATTR(s->sec) || PIC30_IS_AUXFLASH_ATTR(s->sec)) &&
         (!PIC30_IS_ABSOLUTE_ATTR(s->sec)))
        {
         if ((s->sec->lma + len) == (region->origin + region->length))
          {
           einfo(_("%X Link Error: Could not allocate section \'%s\'"
                   " at 0x%v because it is illegal to use the"
                   " last word of program memory.\n"),
                 s->sec->name, s->sec->lma);
           return (struct pic30_memory *) NULL;
          }
        }

#ifdef PIC30ELF
    if (PIC30_IS_SHARED_ATTR(s->sec) && PIC30_IS_ABSOLUTE_ATTR(s->sec)) {
      b = pic30_static_assign_memory(shared_data_memory_blocks,len,s->sec->lma);
    }
    if ((!b) && (PIC30_IS_CODE_ATTR(s->sec) || PIC30_IS_PSV_ATTR(s->sec))) {
      /* see if this space at the end of the aivt */
      b = pic30_static_assign_memory(aivt_free_blocks, len, s->sec->lma);
    }
#endif
    if (!b) b = pic30_static_assign_memory(free_blocks, len, s->sec->lma);
    if (!b) {
        if ((s->sec->flags & SEC_NEVER_LOAD) ||
            (command_line.check_section_addresses == FALSE))
          /* OK, don't report as error */;
        else
          einfo(_("%X %s \'%s\' at 0x%v\n"),
                err_str1, s->sec->name, s->sec->lma);
        return (struct pic30_memory *) NULL;
    } else 
      return b;
  }

   if (PIC30_IS_PSV_ATTR(s->sec) || PIC30_IS_AUXPSV_ATTR(s->sec) ||
       (PIC30_IS_EDS_ATTR(s->sec) && PIC30_IS_PAGE_ATTR(s->sec))) {
     if ((len == 0x8000) && pic30_has_psv_trap_errata) {
       einfo(_("%X Link Error: Could not allocate section \'%s\'"
               " because it is illegal to place data at the first 4 bytes"
               " of a page  when psv_trap errata option is enabled \n"),
                 s->sec->name, s->sec->lma);
           return (struct pic30_memory *) NULL;
     }
   }


  /*
   * If X or Y attributes are set, make sure
   * we have a definition for __YDATA_BASE.
   * Usually this appears in the linker script.
   */
  if ((!ydata_base_defined) &&
      (PIC30_IS_XMEMORY_ATTR(s->sec) || PIC30_IS_YMEMORY_ATTR(s->sec))) {

    h = bfd_pic30_is_defined_global_symbol(str2);
    if (h) {
      ydata_base = h->u.def.value;
      ydata_base_defined = TRUE;
    }
    else
      einfo(_(str1), str2);
    /* its OKAY for __YDATA_END to not be defined; if this is the case then
       XMEMORY ends at __YDATA_BASE ... */
    h = bfd_pic30_is_defined_global_symbol(str2e);
    if (h) {
      ydata_end = h->u.def.value;
    } else {
      ydata_end = 0;
    }
  }

  /*
   * If DMA attribute is set, make sure
   * we have a definition for __DMA_BASE and __DMA_END
   * Usually this appears in the linker script.
   */
  if (PIC30_IS_DMA_ATTR(s->sec)) {
    confirm_dma_range_defined();
  }

  /*
   * If Y and NEAR attributes are set, make sure
   * the combination is valid for the current device.
   *
   * Note: This is not a valid combination for sim30.
   */
  if ((PIC30_IS_YMEMORY_ATTR(s->sec) && PIC30_IS_NEAR_ATTR(s->sec))
      && (ydata_base > NEAR_BOUNDARY))
      einfo(_("%X %s \'%s\', because"
              " \'ymemory,near\' is not a valid combination"
              " on this device\n"), err_str1, s->sec->name);

  /*
   * Loop through the free blocks list
   */
  for (b = free_blocks; b != NULL; b = b->next) {
    int option1_base = b->addr;
    int option2_base; /* ignore warning */

    if ((b->addr == 0) && (b->size == 0))
      continue;

    if (pic30_debug)
      printf("    consider block at %lx, len = %lx\n", b->addr, b->size);

    /*
     * Qualify the block first, so we don't waste
     *  time scanning for alignment.
     */
    if (b->size < len)
      continue;

    if (PIC30_IS_XMEMORY_ATTR(s->sec)) {
      /* do we have len bytes worth of X memory in this block??? */
      unsigned int block_len1 = 0, block_len2 = 0;
 
      if (b->addr < ydata_base) {
        if (b->addr + b->size < ydata_base) block_len1 = b->size;
        else {
          block_len1 = ydata_base - b->addr;
          if ((ydata_end) && (b->addr + b->size > ydata_end)) {
            block_len2 = b->addr+b->size - ydata_end;
          }
        }
      } else if (ydata_end) { 
        if (ydata_end <= b->addr) {
          block_len1 = b->size;
        } else block_len1 = b->addr+b->size - ydata_end;
      }
      if ((len > block_len1) && (len > block_len2)) continue;
    }

    if (PIC30_IS_YMEMORY_ATTR(s->sec)) {
      /* do we have len bytes worth of Y memory in this block??? */
      bfd_vma ystart, yend;

      if (b->addr < ydata_base) {
        ystart = ydata_base;
        if (ystart > b->addr + b->size) continue;
      } else ystart = b->addr;

      if (ydata_end) {
        if (b->addr + b->size < ydata_end) 
          yend = b->addr + b->size;
        if (b->size + b->size >= ydata_end) 
          yend = ydata_end;
      } else yend = b->addr+b->size;
      if (yend-ystart < len) continue; /* ignore warning */
    }

    /* qualify NEAR with leftmost position in free block */
    if (PIC30_IS_NEAR_ATTR(s->sec) && !VALID_NEAR(b->addr, len))
      continue;

    /* CAW - pic30_psv_override allows us to force all 'data' style
             definitions to be placed anywhere in RAM */
    if ((pic30_psv_override == FALSE) &&
        (pic30_is_eds_machine(global_PROCESSOR) || has_psv_section)) {
      /* qualify LOCAL_DATA with leftmost position in free block */
      if (PIC30_IS_LOCAL_DATA(s->sec) && !VALID_LOCAL_DATA(b->addr, len))
        continue;
    }

    /* qualify LOW_ADDR with leftmost position in free block */
    if (IS_LOCATE_OPTION(EXCLUDE_HIGH_ADDR) &&
        !VALID_LOW_ADDR(b->addr, len))
      continue;

    /* qualify HIGH_ADDR with rightmost position in free block */
    if (IS_LOCATE_OPTION(EXCLUDE_LOW_ADDR) &&
        !VALID_HIGH_ADDR((b->addr + b->size - len), len))
      continue;

    /* qualify DMA with leftmost and rightmost position in dma address range  */
    if (PIC30_IS_DMA_ATTR(s->sec) &&
        (((b->addr + b->size - len) < dma_base) ||
         ((b->addr + len - 1 ) > dma_end)))
      continue;

    /* qualify BOOT */
    if (PIC30_IS_BOOT_ATTR(s->sec) &&
        (OVERLAPS_BOOT(b->addr, b->size) == FALSE)) {
      continue; /* not an eligible block */
    }

    if (PIC30_IS_BOOT_ATTR(s->sec) && OVERLAPS_BOOT(b->addr, b->size)) {
      bfd_vma low = b->addr >
        base_address[BOOTx][alloc_region_index] ?
        b->addr : base_address[BOOTx][alloc_region_index];
      bfd_vma high = (b->addr + b->size) >
        end_address[BOOTx][alloc_region_index] ?
        end_address[BOOTx][alloc_region_index] : b->addr + b->size;

      if (len > (high - low)) /* not enough BOOT space */
        continue;
    }

    /* qualify SECURE */
    if (PIC30_IS_SECURE_ATTR(s->sec) &&
        (OVERLAPS_SECURE(b->addr, b->size) == FALSE)) {
      continue; /* not an eligible block */
    }

    if (PIC30_IS_SECURE_ATTR(s->sec) && OVERLAPS_SECURE(b->addr, b->size)) {
      bfd_vma low = b->addr >
        base_address[SECUREx][alloc_region_index] ?
        b->addr : base_address[SECUREx][alloc_region_index];
      bfd_vma high = (b->addr + b->size) >
        end_address[SECUREx][alloc_region_index] ?
        end_address[SECUREx][alloc_region_index] : b->addr + b->size;

      if (len > (high - low)) /* not enough SECURE space */
        continue;
    }

    /* fall through if block passes pre-qual tests */

    /* Compute option1: scan from the start of the block */
    option1 = b->addr - 2;
    limit1  = b->addr + (b->size - len);
    option1_valid = TRUE; /* set default */
    if (pic30_debug)
      printf("    option1 [advancing from %lx]\n", option1 + 2);
    while (1) {

      if (IS_LOCATE_OPTION(FAVOR_HIGH_ADDR) &&
          !(PIC30_IS_BOOT_ATTR(s->sec) || PIC30_IS_SECURE_ATTR(s->sec))) {
        option1_valid = FALSE;
        break;
      }

      /* find the next aligned address */
      if (PIC30_IS_REVERSE_ATTR(s->sec)) {
        option1 = next_aligned_address(option1 + len, align_power);
        option1 -= len;
      }
      else
        option1 = next_aligned_address(option1, align_power);

      if (option1 > limit1) {
        option1_valid = FALSE;
        break;
      }
      
      
      if (pic30_debug)
        printf("    option1 aligned at %lx\n", option1);

      /* aligned address is valid, check other attributes */

      if ((PIC30_IS_XMEMORY_ATTR(s->sec) && !VALID_X(option1, len)) ||
          (PIC30_IS_NEAR_ATTR(s->sec)    && !VALID_NEAR(option1, len))) {
        option1_valid = FALSE;  /* scanning forward won't help */
        break;
      }

      if ((pic30_is_eds_machine(global_PROCESSOR)  || has_psv_section) &&
          PIC30_IS_LOCAL_DATA(s->sec) && !VALID_LOCAL_DATA(option1, len)) {
        option1_valid = FALSE;  /* scanning forward won't help */
        break;
      }

      /* this check is necessary for Co-resident applications */
      /* to make sure persistent data is not over written at any time */

      if (PIC30_IS_PERSIST_ATTR(s->sec) || PIC30_IS_SHARED_ATTR(s->sec)) {
        old_option1 = option1;
        new_option1 = option1;
        struct pic30_section *ls;
        for (ls = alloc_section_list; ls != NULL; ls = ls->next) {
           if (ls->sec && (ls->sec->linked == 1)) {
            if (((ls->sec->lma <= old_option1) && (old_option1 < (ls->sec->lma +
                                           (ls->sec->size / opb))))) {
               new_option1 = ls->sec->lma + (ls->sec->size / opb);
               break;
             }
           }
        }
        if (new_option1 != old_option1)
          continue;
        else option1= new_option1;
      }

      if (PIC30_IS_YMEMORY_ATTR(s->sec) && !VALID_Y(option1, len)) {
        option1 = ydata_base - 2;  /* skip ahead */
        if (pic30_debug)
          printf("    approaching Y boundary from %lx\n", option1);
        continue;
      }

      if ((PIC30_IS_PSV_ATTR(s->sec) || PIC30_IS_AUXPSV_ATTR(s->sec)) && 
           ((option1 % PSV_BOUNDARY)< 4 )) {
        if (pic30_has_psv_trap_errata) {
          option1 += 4 - (option1 % PSV_BOUNDARY) - 2;

          if (pic30_debug)
            printf("    approaching PSV boundary from %lx%s\n", option1,
                   pic30_has_psv_trap_errata ? " (psv trap errata enabled)": "");
          continue;
        }
      }

      if ((PIC30_IS_PSV_ATTR(s->sec) || PIC30_IS_AUXPSV_ATTR(s->sec))
          && !VALID_PSV((bfd_vma)option1, len)) {
        option1 = NEXT_PSV_PAGE(option1) - 2;
        if (pic30_has_psv_trap_errata)
          option1 += 4;
        
        if (pic30_debug)
          printf("    approaching PSV boundary from %lx%s\n", option1,
                 pic30_has_psv_trap_errata ? " (psv trap errata enabled)": "");
        continue;
      }

      if (IS_LOCATE_OPTION(EXCLUDE_LOW_ADDR) &&
          !VALID_HIGH_ADDR(option1, len)) {
        option1 = exclude_addr - 2;  /* skip ahead */
        if (pic30_debug)
          printf("    approaching EXCLUDE boundary from %lx\n", option1);
        continue;
      }

      if (PIC30_IS_EDS_ATTR(s->sec) && PIC30_IS_PAGE_ATTR(s->sec) &&
          ((option1 % EDS_BOUNDARY)< 4 )) {
        if (pic30_has_psv_trap_errata) {
          option1 += 4 - (option1 % EDS_BOUNDARY) - 2;
          if (pic30_debug)
            printf("    approaching EDS boundary from %lx%s\n", option1,
                   pic30_has_psv_trap_errata ? " (psv trap errata enabled)": "");
          continue;
        }
      }

      if (PIC30_IS_EDS_ATTR(s->sec) && PIC30_IS_PAGE_ATTR(s->sec) &&
          !VALID_EDS_PAGE((bfd_vma)option1, len)) {
        option1 = NEXT_EDS_PAGE(option1) - 2;
        if (pic30_has_psv_trap_errata)
          option1 += 4;
        
        if (pic30_debug)
          printf("    approaching EDS boundary from %lx%s\n", option1,
                 pic30_has_psv_trap_errata ? " (psv trap errata enabled)": "");
        continue;
      }

      if ((PIC30_IS_CODE_ATTR(s->sec) || 
	   PIC30_IS_EEDATA_ATTR(s->sec) ||
	   PIC30_IS_AUXFLASH_ATTR(s->sec))
          && PIC30_IS_PAGE_ATTR(s->sec) &&
          !VALID_PROG_PAGE((bfd_vma)option1, len)) {
        option1 = NEXT_PROG_PAGE(option1) - 2;
        
        if (pic30_debug)
          printf("    approaching PROG boundary from %lx\n", option1);
        continue;
      }

      if (PIC30_IS_DMA_ATTR(s->sec) && !VALID_DMA((bfd_vma)option1, len)) {
        option1 = dma_base - 2;
        if (pic30_debug)
          printf("    option1 set to lower DMA boundary\n");
        continue;
      }

      if (PIC30_IS_BOOT_ATTR(s->sec) && !VALID_BOOT(option1, len)) {
        if (option1 < base_address[BOOTx][alloc_region_index])
          option1 = base_address[BOOTx][alloc_region_index] - 2;
        continue;
      }

      if (PIC30_IS_SECURE_ATTR(s->sec) && !VALID_SECURE(option1, len)) {
        if (option1 < base_address[SECUREx][alloc_region_index])
          option1 = base_address[SECUREx][alloc_region_index] - 2;
        continue;
      }

      /* avoid BOOT if we don't belong there */
      if (BOOT_IS_ACTIVE(alloc_region_index) && !PIC30_IS_BOOT_ATTR(s->sec) &&
          OVERLAPS_BOOT(option1, len)) {
        option1 = end_address[BOOTx][alloc_region_index];
        option1_base = end_address[BOOTx][alloc_region_index]+2;
        if (pic30_debug)
          printf("    avoiding boot segment\n");
        continue;
      }

      /* ditto for SECURE */
      if (SECURE_IS_ACTIVE(alloc_region_index) && 
          !PIC30_IS_SECURE_ATTR(s->sec) &&
          OVERLAPS_SECURE(option1, len)) {
        option1 = end_address[SECUREx][alloc_region_index];
        option1_base = end_address[SECUREx][alloc_region_index]+2;
        if (pic30_debug)
          printf("    avoiding secure segment\n");
        continue;
      }

      /* if we get here, the option is valid */
      break;
    } /* while (!done) */

    /* Compute option2: scan from the end of the block       */
    /*                  unless the section is BOOT or SECURE */
    option2 = limit1 + 2;
    limit2  = b->addr;
    if (PIC30_IS_BOOT_ATTR(s->sec) || PIC30_IS_SECURE_ATTR(s->sec)) {
      option2_valid = FALSE;
    }

    else if (PSV_BOUNDARY &&
             (0 <= (option2 % PSV_BOUNDARY)) && 
             ((option2 % PSV_BOUNDARY) < 4) && pic30_has_psv_trap_errata)
      option2_valid = FALSE;

    else {
      option2_valid = TRUE; /* set default */
      if (pic30_debug)
        printf("    option2 [descending from %lx]\n", option2 - 2);
      while (1) {

        if (IS_LOCATE_OPTION(FAVOR_LOW_ADDR)) {
          option2_valid = FALSE;
          break;
        }

        /* find the next aligned address */
        if (PIC30_IS_REVERSE_ATTR(s->sec)) {
          option2 = prev_aligned_address(option2 + len, align_power);
          option2 -= len;
        }
        else
          option2 = prev_aligned_address(option2, align_power);

        if (option2 < limit2) {
          option2_valid = FALSE;
          break;
        }

        if (pic30_debug)
          printf("    option2 aligned at %lx\n", option2);

        /* aligned address is valid, check other attributes */

      /* this check is necessary for Co-resident applications */
      /* to make sure persistent data is not over written at any time */
      if (PIC30_IS_PERSIST_ATTR(s->sec) || PIC30_IS_SHARED_ATTR(s->sec)) {
        old_option2 = option2;
        new_option2 = option2;
        struct pic30_section *ls;
        for (ls = alloc_section_list; ls != NULL; ls = ls->next) {
           if (ls->sec && (ls->sec->linked == 1)) {
            if (((ls->sec->lma <= old_option2) && (old_option2 < (ls->sec->lma +
                                           (ls->sec->size / opb))))) {
               new_option2 = ls->sec->lma - len;
               break;
             }
           }
        }
        if (new_option2 != old_option2) 
          continue;
        else option2= new_option2;
      }

        if (PIC30_IS_XMEMORY_ATTR(s->sec) && !VALID_X(option2, len)) {
          option2 = ydata_base - len + 2;  /* skip back */
          if (pic30_debug)
            printf("    approaching X boundary from %lx\n", option2);
          continue;
        }

        if  (PIC30_IS_NEAR_ATTR(s->sec)    && !VALID_NEAR(option2, len)) {
          option2 = NEAR_BOUNDARY - len + 2;  /* skip back */
          if (pic30_debug)
            printf("    approaching Near boundary from %lx\n", option2);
          continue;
        }

        if ((pic30_psv_override == FALSE) &&
            (pic30_is_eds_machine(global_PROCESSOR) || has_psv_section) &&
            PIC30_IS_LOCAL_DATA(s->sec) && !VALID_LOCAL_DATA(option2, len)) {
          option2 = EDS_BOUNDARY - len + 2;  /* skip back */
          if (pic30_debug)
            printf("    approaching EDS boundary from %lx\n", option2);
          continue;
        }

        if  (PIC30_IS_YMEMORY_ATTR(s->sec) && !VALID_Y(option2, len)) {
          option2_valid = FALSE;  /* scanning backward won't help */
          break;
        }
        
        if ((PIC30_IS_PSV_ATTR(s->sec) || PIC30_IS_AUXPSV_ATTR(s->sec)) 
            && !VALID_PSV((bfd_vma)option2, len)) {
          if (option2 < PSV_BOUNDARY) {
            option2_valid = FALSE;
            break;
          }
          option2 = PREV_PSV_PAGE(option2) - len + 2;  /* skip back */
          if (pic30_debug)
            printf("    approaching PSV boundary from %lx\n", option2);
          continue;
        }

        if (IS_LOCATE_OPTION(EXCLUDE_HIGH_ADDR) &&
            !VALID_LOW_ADDR(option2, len)) {
          option2 = exclude_addr - len + 2;  /* skip back */
          if (pic30_debug)
            printf("    approaching EXCLUDE boundary from %lx\n", option2);
          continue;
        }

        if (PIC30_IS_EDS_ATTR(s->sec) && PIC30_IS_PAGE_ATTR(s->sec) &&
            !VALID_EDS_PAGE((bfd_vma)option2, len)) {
          if (option2 < EDS_BOUNDARY) {
            option2_valid = FALSE;
            break;
          }
          bfd_vma option = option2;
          option2 = NEXT_EDS_PAGE(option) - len + 2;  /* skip back */
          if (pic30_debug)
            printf("    approaching EDS boundary from %lx\n", option2);
          continue;
        }

        if ((PIC30_IS_CODE_ATTR(s->sec)|| 
	     PIC30_IS_EEDATA_ATTR(s->sec) ||
	     PIC30_IS_AUXFLASH_ATTR(s->sec)) &&
            PIC30_IS_PAGE_ATTR(s->sec) &&
            !VALID_PROG_PAGE((bfd_vma)option2, len)) {
          if (option2 < PROG_BOUNDARY) {
            option2_valid = FALSE;
            break;
          }
          option2 = PREV_PROG_PAGE(option2) - len + 2;  /* skip back */
          if (pic30_debug)
            printf("    approaching PROG boundary from %lx\n", option2);
          continue;
        }

        if (PIC30_IS_DMA_ATTR(s->sec) && !VALID_DMA((bfd_vma)option2, len)) {
          if (option2 < dma_end) {
            option2_valid = FALSE;
            break;
          }
          option2 = dma_end + 1 - len + 2;
          if (pic30_debug)
            printf("    option2 set to upper DMA boundary\n");
          continue;
        }

        /* avoid BOOT if we don't belong there */
        if (BOOT_IS_ACTIVE(alloc_region_index) && !PIC30_IS_BOOT_ATTR(s->sec) &&
            OVERLAPS_BOOT(option2, len)) {

          bfd_vma boot_base = base_address[BOOTx][alloc_region_index];
          
          if ((b->addr + len) >= boot_base) {
            option2_valid = FALSE;  /* exit if block can't work */
            break;
          }
          option2 = boot_base - len + 2;  /* else skip back */
          option2_base = boot_base;
          if (pic30_debug)
            printf("    avoiding boot segment\n");
          continue;
        }

        /* ditto for SECURE */
        if (SECURE_IS_ACTIVE(alloc_region_index) && !PIC30_IS_SECURE_ATTR(s->sec) &&
            OVERLAPS_SECURE(option2, len)) {

          bfd_vma secure_base = base_address[SECUREx][alloc_region_index];
          
          if ((b->addr + len) >= secure_base) {
            option2_valid = FALSE;  /* exit if block can't work */
            break;
          }
          option2 = secure_base - len + 2;
          option2_base = secure_base;
          if (pic30_debug)
            printf("    avoiding secure segment\n");
          continue;
        }

        /* if we get here, the option is valid */
        break;
      } /* while (!done) */
    } /* else */

    if ((option1_valid == FALSE) && (option2_valid == FALSE))
      continue;  /* try the next free block */

    if (pic30_debug) {
      printf("    ");
      if (option1_valid)
        printf("option1 = %lx, ", option1);
      if (option2_valid)
        printf("option2 = %lx", option2);
      printf("\n");
    }

    /* we have 1 or 2 valid options */
    if (option1_valid == FALSE)
      b->offset = option2 - b->addr;
    else if (option2_valid == FALSE)
      b->offset = option1 - b->addr;
    else {
      /* compare the two "outer" remainders */
      bfd_vma rem1 = option1 - option1_base;
      bfd_vma rem2 = (b->addr + b->size) - option2 - len;

      b->offset = (rem1 > rem2) ?
                  (option2 - b->addr) : (option1 - b->addr);
    }

  /* This check is necessary to prevent the linker from 
     placing code at the last word of program memory */

   if (PIC30_IS_CODE_ATTR(s->sec) || PIC30_IS_AUXFLASH_ATTR(s->sec))
     {
       if ((b->addr + b->offset + len) == (region->origin + region->length))
        { if (config.map_file)
            fprintf(config.map_file, "/n Could not use block"
                    " at address 0x%lx and of size 0x%lx to locate section %s"
                    " because this will lead to having executable code"
                    " at the last word of program memory./n",
                    b->addr, b->size, s->sec->name); 
         continue;
        }
     }

    return b;
  }
  if (more_regions_to_scan) return NULL;

  /* If we get here, a suitable block could not be found */
  if (locate_options != NO_LOCATE_OPTION)
    return (struct pic30_memory *) NULL; /* exit quietly */
  else
    (void) report_allocation_error(s);

#if 0
  {
#define PREFIX "/tmp"
    char *colon = "";
    char *filename = "";
    char *secname = "*";
    char *s1 = pic30_section_size_string(s->sec);
    char *s2 = pic30_section_attr_string(s->sec);

    /* report the file name, unless its a temp file */
    if (strncmp(s->sec->owner->filename, PREFIX, strlen(PREFIX)) != 0) {
      colon = ":";
      filename = (char *) s->sec->owner->filename;
    }

    /* report the section name, unless its based on a temp file */
    if (strncmp(s->sec->name, PREFIX, strlen(PREFIX)) != 0)
      secname = (char *) s->sec->name;

    einfo(_("%X%s%s Link Error: Could not allocate section %s, %s, %s\n"),
          filename, colon, secname, s1 ? s1 : "", s2 ? s2 : "");

    if (s1) free(s1);  /* free the malloc'ed strings */
    if (s2) free(s2);

    if (pic30_debug)
      printf("\n    Error: Could not allocate section %s\n", s->sec->name);
  }
#endif

  return (struct pic30_memory *) NULL;
} /* select_free_block() */


/*
 * Diagnostic function
 */
static void
print_output_section_statement(lang_output_section_statement_type *os) {

  unsigned int opb = bfd_octets_per_byte (link_info.output_bfd);

  /* Note: binutils-v2.32 adds some additional fields not included here */
  printf("    dump of output section statement at %lx:\n", (bfd_vma) os);
  printf("      header = %lx\n", (bfd_vma) &(os->header));
  printf("      addr_tree = %lx\n", (bfd_vma) os->addr_tree);
  printf("      load_base = %lx\n", (bfd_vma) os->load_base);
  printf("      name = %s\n", os->name);
  printf("      bfd_section = %lx\n", (bfd_vma) os->bfd_section);
  if ((os->header.type == lang_output_section_statement_enum) &&
       os->bfd_section) {
    printf("        name = %s\n", os->bfd_section->name);
    printf("        attr = %x\n", pic30_attribute_map(os->bfd_section));
    printf("        lma  = %lx\n", (bfd_vma) os->bfd_section->lma);
    printf("        vma  = %lx\n", (bfd_vma) os->bfd_section->vma);
    printf("        len  = %lx\n", (bfd_vma) os->bfd_section->size / opb);
  }
} /* print_output_section_statement() */


static char *
unique_section_name(const char *s) {
  static int auto_sec_cnt = 0;
  char *name;
  int len;

  len = strlen(s) + 1 + 5;
  name = xmalloc (len + 1);
  snprintf(name, len, "%s%s%d", s, "%", ++auto_sec_cnt);
  return name;
}


static char *
unique_zero_length_section_name(const char *s) {
  static int auto_zero_sec_cnt = 0;
  char *name;
  int len;

  len = strlen(s) + 2 + 5;
  name = xmalloc (len + 1);
  snprintf(name, len, "%s%s%d", s, "%z", ++auto_zero_sec_cnt);
  return name;
}


/*
 * finish_section_info()
 *
 * called by:
 *  update_section_info()
 *  update_grouped_section_info()
 */
static void
finish_section_info(struct pic30_section *s, lang_output_section_statement_type *os)
{
  etree_type *addr_tree, *load_addr_tree;

  if ((os->bfd_section == NULL) ||
      (os->bfd_section->flags & SEC_EXCLUDE)) // --gc-sections discarded this, so done
    return;
  os->bfd_section->vma = s->sec->vma;
  os->bfd_section->lma = s->sec->lma;
  os->bfd_section->size = s->sec->size;
  addr_tree = xmalloc(sizeof(etree_type));
  addr_tree->value.type.node_class = etree_value;
  addr_tree->value.value = s->sec->vma;
  os->addr_tree = addr_tree;

  if (PIC30_IS_PSV_ATTR(s->sec) || PIC30_IS_EEDATA_ATTR(s->sec) ||
      PIC30_IS_PACKEDFLASH_ATTR(s->sec) || PIC30_IS_AUXPSV_ATTR(s->sec)) {
    load_addr_tree = xmalloc(sizeof(etree_type));
    load_addr_tree->value.type.node_class = etree_value;
    load_addr_tree->value.value = s->sec->lma;
    os->load_base = load_addr_tree;
  }
} /* finish_section_info() */


void update_section_addr(asection *s,bfd_vma addr)  {
  s->lma = addr;
  if (PIC30_IS_PSV_ATTR(s) || PIC30_IS_EEDATA_ATTR(s) ||
      PIC30_IS_AUXPSV_ATTR(s))
    s->vma = PSV_BASE + (addr & 0x7FFF);
  else if (PIC30_IS_PACKEDFLASH_ATTR(s))
    s->vma = addr * 3 / 2;
  else
    s->vma = addr;
}

/*
 * update_section_info()
 *
 * Update section info after allocation
 *
 * If section was a heap or stack, set some globals.
 *
 * Note: argument region is currently unused.
 *
 */
#define _UNUSED_ __attribute__((unused))
static void
update_section_info(bfd_vma alloc_addr,
                    struct pic30_section *s,
                    struct memory_region_struct *region _UNUSED_) {

  lang_output_section_statement_type *os;
  char *name;

  update_section_addr(s->sec, alloc_addr);

  if (pic30_debug && s->sec->size) {
    printf("    updating section info:"
           "  vma = %lx, lma = %lx\n", s->sec->vma, s->sec->lma);
  }

  /* create a unique name for the output section */
  name = (s->sec->size > 0) ?
    unique_section_name(s->sec->name) :
    unique_zero_length_section_name(s->sec->name);

  /* create an output section (statement) */
  os = lang_output_section_statement_lookup (name, 0, TRUE);

  if (pic30_debug && s->sec->size) {
    char *attr = pic30_section_attr_string(s->sec);
    printf("    creating output section statement '%s' %s\n",
           os->name, attr ? attr : "");
    if (attr) free(attr);
  }
  s->sec->alignment_power = 1;
  /* lang_add_section() will call init_os() if needed */
  lang_add_section (&os->children, s->sec, NULL, os);

  finish_section_info(s, os);
} /* update_section_info() */


/*
 * update_group_section_info()
 *
 * Update group section info after allocation
 *
 * Note: argument region is currently unused.
 */
static void
update_group_section_info(bfd_vma alloc_addr,
                    struct pic30_section *g,
                    struct memory_region_struct *region _UNUSED_) {

  unsigned int opb = bfd_octets_per_byte (link_info.output_bfd); /* slooow */
  struct pic30_section *s, *next;
  asection *sec;
  char *name;
  bfd_vma addr = alloc_addr;
  lang_output_section_statement_type *os;

  /* create a unique name for the output section, if necessary */
  sec = bfd_get_section_by_name(link_info.output_bfd, g->sec->name);
  if (sec) {
    name = (g->sec->size > 0) ?
            unique_section_name(g->sec->name) :
            unique_zero_length_section_name(g->sec->name);
  }
  else
    name = (char *) g->sec->name;

  /* create an output section (statement) */
  os = lang_output_section_statement_lookup (name, 0, TRUE);
  if (pic30_debug)
    printf("    creating output section statement \"%s\"\n", os->name);

  /* The first absolute section in the group if any */
  update_section_addr(g->sec, addr);
  addr += g->sec->size / opb;
  g->sec->alignment_power = 1;
  /* lang_add_section() will call init_os() if needed */
  lang_add_section (&os->children, g->sec, NULL, os);

  if (pic30_debug)
  printf("    updating grouped section info:"
               "  vma = %lx, lma = %lx\n", g->sec->vma, g->sec->lma);

  /* loop through the input sections in this group */
  for (s = alloc_section_list; s != NULL; s = next) {
    next = s->next;
    if (s->sec && (s == g))
      continue;
    if (s->sec && (strcmp(g->sec->name, s->sec->name) == 0)) {
      update_section_addr(s->sec, addr);
      addr += s->sec->size / opb;
      s->sec->alignment_power = 1;
      /* lang_add_section() will call init_os() if needed */
      lang_add_section (&os->children, s->sec, NULL, os);

      if (pic30_debug)
        printf("    updating grouped section info:"
               "  vma = %lx, lma = %lx\n", s->sec->vma, s->sec->lma);
    }
  }
  finish_section_info(g, os);
} /* update_group_section_info() */


/*
 * create_remainder_blocks()
 *
 * Create [0,1,2] remainder blocks based on
 * block->offset and section->size.
 */
static void
create_remainder_blocks(struct pic30_memory *lst,
                        struct pic30_memory *b, unsigned int len,
                        struct memory_region_struct *region,
                        struct pic30_section *s)
{

  bfd_vma remainder;

  /* (GM) Does this need to be generalized for multiple "program" regions? */
  if ((strcmp(region->name_list.name,"program") == 0) && aivt_base && 
      (s->sec->linker_generated == 0)) {
    /* if we create a block that uses part of the aivt, then fill the rest...*/
    if ((b->addr + b->offset >= aivt_base) &&
        (b->addr + b->offset + len <= aivt_base + aivt_len)) {
      unsigned int updated_len;

      updated_len = aivt_base + aivt_len - b->addr - b->offset;
      if (updated_len <= b->size) len = updated_len;
    }
  }
  remainder = b->size - (len + b->offset);

  if (pic30_debug)
    printf("    creating remainder blocks: %lx, %lx\n",
           b->offset, remainder);

  if (b->offset > 0)  /* gap at beginning */
    pic30_add_to_memory_list(lst, b->addr, b->offset);

  if (remainder > 0)  /* gap at end */
    pic30_add_to_memory_list(lst,
                             b->addr + b->offset + len, remainder);
} /* create_remainder_blocks() */


/*
 * insert_alloc_section()
 *
 * Insert a pic30_section into the list
 *  in order of decreasing size or alignment power.
 *
 * Note: a new copy of the pic30_section is made
 *
 */
static void
insert_alloc_section (struct pic30_section *lst,
                      struct pic30_section *as)
{
  struct pic30_section *new, *prev, *s;
  int done = 0;

  /* create a new element */
  new = ((struct pic30_section *) 
         xmalloc(sizeof(struct pic30_section)));
  new->sec  = as->sec;
  new->attributes = as->attributes;
  new->file = as->file;

  /* insert it at the right spot */
  prev = lst;
  for (s = prev; s != NULL; s = s->next)
    {
      if (s->sec && ((new->sec->size > s->sec->size) ||
          ((bfd_vma)(1 << new->sec->alignment_power) > s->sec->size)) ) {
        prev->next = new;
        new->next = s;
        done++;
        break;
      }
      prev = s;
    }

  if (!done)
    {
      prev->next = new;
      new->next = NULL;
    }
} /* insert_alloc_section() */


/*
 * build_alloc_section_list()
 *
 * Build alloc_section_list from the
 * list of unassigned_sections, based on
 * an attribute mask.
 *
 * Sort the list in order of decreasing S,
 * where S is the greater of size and
 * alignment requirement.
 *
 * Note: If a section has length zero,
 * we allocate it anyway, although it
 * is stripped from the output file.
 * The linker needs to have an output
 * section statement created for it.
 *
 * Future enhancement:
 *   If --re-order-sections is set, also
 *   add explicity mapped output sections
 *   unless they have an absolute address.
 */
static void
build_alloc_section_list(unsigned int has_mask, unsigned hasnot_mask) {

  unsigned int opb = bfd_octets_per_byte (link_info.output_bfd);
  struct pic30_section *s, *prev, *next;
  long int total_length = 0;

  if (alloc_section_list)
    pic30_free_section_list(&alloc_section_list);

  pic30_init_section_list(&alloc_section_list);

  /* insert the unassigned sections */
  prev = unassigned_sections;
  for (s = prev; s != NULL; s = next) {

    next = s->next;
    if (((s->attributes & has_mask)  && ((s->attributes & hasnot_mask) == 0)) &&
        (!s->sec->linker_generated || ((s->sec->flags & SEC_NEVER_LOAD) == 0))){
      if ((pic30_debug) && (s->sec->size)) {
        char *attr = pic30_section_attr_string(s->sec);
        printf("  input section \"%s\", len = %lx, flags = %x, %s\n",
               s->sec->name,s->sec->size/opb,s->sec->flags,
               attr ? attr : "");
        if (attr) free(attr);

        total_length += s->sec->size/opb;
      }
      insert_alloc_section(alloc_section_list, s);
      prev->next = next; /* unlink it from unassigned_sections */
    } else
      prev = s;
  }
  if (pic30_debug)
    printf("  --> Total amount requested: %ld bytes\n", total_length);
} /* build_alloc_section_list() */


void
build_aivt_free_block_list(bfd_vma start, bfd_vma length) {

  if (aivt_free_blocks)
     pic30_free_memory_list(&aivt_free_blocks);

  pic30_init_memory_list(&aivt_free_blocks);

  pic30_add_to_memory_list(aivt_free_blocks, start, length);
}

/*
 * build_free_block_list()
 *
 * Build a list of free memory blocks
 * in a specified region.
 *
 * Sort the list in order of increasing size.
 *
 * Future enhancement:
 *   If --re-order-sections is set, add free blocks
 *   for explicity mapped output sections unless
 *   they have an absolute address.
 */
static void
build_free_block_list(struct memory_region_struct *region,
                      unsigned int has_mask, unsigned int hasnot_mask) {

  unsigned int opb = bfd_octets_per_byte (link_info.output_bfd);
  int cnt = 0;
  bfd_vma len, dot, limit;
  struct pic30_section *s;

  if (free_blocks)
    pic30_free_memory_list(&free_blocks);

  pic30_init_memory_list(&free_blocks);

  if (pic30_debug)
    printf("\nBuilding free block list for region \"%s\"\n"
           "  origin = %lx, length = %lx, current = %lx\n",
           region->name_list.name, region->origin, region->length, region->current );

  /* find any gaps left by sequential allocator */
  dot = region->origin;
  limit = dot + region->length;
  for (s = pic30_section_list; s != NULL; s = s->next) {
    if ((s->sec) && (s->sec->size > 0) &&
        (s->attributes & has_mask) && ((s->attributes & hasnot_mask) == 0)) {
                                    /* this section matches our region mask, */
                                    /* but could have been assigned elsewhere  */

      if ((s->sec->lma < region->origin) || (s->sec->lma >= limit))
        continue; /* section belongs to another region */

      if (pic30_debug)
        printf("    (section \"%s\", addr = %lx, len = %lx, attr = %x)\n",
               s->sec->name, s->sec->lma,
               s->sec->size / opb, s->attributes);

      if (s->sec->lma == dot) {     /* no gap, just advance dot */
        dot += s->sec->size / opb;
        continue;
      }

      if (s->sec->lma > dot) {      /* a gap preceeds section */
        len = s->sec->lma - dot;            /* compute length */

        if (pic30_debug)
          printf("  block %d, addr = %lx, len = %lx", ++cnt, dot, len);

        pic30_add_to_memory_list(free_blocks, dot, len);  /* add free block */
        dot += len + (s->sec->size / opb); /* advance dot past gap and section */

        if (pic30_debug)
          printf(" (new dot is %lx)\n", dot);
      }
    }
  }
     
  /* DEBUG */
#if 0
  {
    lang_statement_union_type *s;
    for (s = statement_list.head;
         s != (lang_statement_union_type *) NULL;
         s = s->header.next)
      if (s->header.type == lang_output_section_statement_enum)
        print_output_section_statement((lang_output_section_statement_type *)s);
  }
#endif

  /* consume rest of aivt space, if required */
  if ((region->flags & (SEC_CODE|SEC_READONLY))==(SEC_CODE|SEC_READONLY)) {
    if ((dot >= aivt_base) && (dot < aivt_base + aivt_len)) {
      dot = aivt_base + aivt_len;
    }
  }

  /* add a block for any free space remaining in this region */
  /* .. use dot, because region->current may have been set for CodeGuard */
  len = region->length - (dot - region->origin);
  if (len > 0) {
    if (pic30_debug)
      printf("  block %d, addr = %lx, len = %lx\n",
             ++cnt, dot, len);
    pic30_add_to_memory_list(free_blocks, dot, len);
  }      
} /* build_free_block_list() */


/*
 * remove_free_block()
 *
 * Remove an item from the free memory blocks list.
 *
 */
static void
remove_free_block(struct pic30_memory *b) {
  pic30_remove_from_memory_list(free_blocks, b);
}

