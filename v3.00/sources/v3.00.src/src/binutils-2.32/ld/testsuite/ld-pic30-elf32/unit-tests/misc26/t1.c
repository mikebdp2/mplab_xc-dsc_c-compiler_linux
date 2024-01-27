#include <stdio.h>
#include "xc.h"

int F0();
int F4();
int F8();
int F12();

int nib0(int);
int nib4(int);
int nib8(int);
int nib12(int);

int main()
{
  int val = 0xABCD;
  printf("Testing BFINS instruction:\n\n");
  
  printf("000F = %04x\n", F0());
  printf("00F0 = %04x\n", F4());
  printf("0F00 = %04x\n", F8());
  printf("F000 = %04x\n", F12());

  printf("\nTesting BFEXT instruction:\n\n");

  printf("%04x = %01x%01x%01x%01x\n\n", val, nib12(val), nib8(val), nib4(val), nib0(val));
}

