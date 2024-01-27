#include <stdio.h>

int foo(int);
float __floatsisf(int);

void main()
{
  volatile float f = __floatsisf(42);
}
