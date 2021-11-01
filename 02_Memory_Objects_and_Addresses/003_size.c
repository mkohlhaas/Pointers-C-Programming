#include <stdio.h>

int
main()
{
  char c; int i; double d;
  printf("%zu %zu\n", sizeof(char),   sizeof c);
  printf("%zu %zu\n", sizeof(int),    sizeof i);
  printf("%zu %zu\n", sizeof(double), sizeof d);
}
