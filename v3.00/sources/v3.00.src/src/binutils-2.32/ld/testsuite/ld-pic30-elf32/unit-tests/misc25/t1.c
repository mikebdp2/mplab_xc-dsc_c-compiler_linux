#include <stdio.h>

unsigned long start,end,section_size;

/* These refer to arrays defined here */
#define START 0x800
#define START_HI 0x18000
#define LEN 0x100

/* These refer to a section defined in file t2.s*/
#define START2 0xc00
#define LEN_HI 0x12000

extern void keep();

int main()
{
  start = __builtin_section_begin("foo");
  end   = __builtin_section_end("foo");
  section_size = __builtin_section_size("foo");
  
  printf("Testing low address range:\n");
  printf("0x%lx = 0x%x\n", start, START);
  printf("0x%lx = 0x%x\n", section_size, LEN);
  printf("0x%lx = 0x%x\n", end, (START + LEN));
  
  start = __builtin_section_begin("bar");
  end   = __builtin_section_end("bar");
  section_size = __builtin_section_size("bar");
  
  printf("\nTesting high address range:\n");
  printf("0x%lx = 0x%lx\n", start, START_HI);
  printf("0x%lx   = 0x%x\n", section_size, LEN);
  printf("0x%lx = 0x%lx\n", end, (START_HI + LEN));

  start = __builtin_section_begin("beep");
  end   = __builtin_section_end("beep");
  section_size = __builtin_section_size("beep");
  
  printf("\nTesting large section size:\n");
  printf("0x%lx   = 0x%x\n", start, START2);
  printf("0x%lx = 0x%lx\n", section_size, LEN_HI);
  printf("0x%lx = 0x%lx\n", end, (START2 + LEN_HI));
}

unsigned char __attribute__((section("foo"), address(START))) foo[LEN];
unsigned char __attribute__((section("bar"), address(START_HI))) bar[LEN];


