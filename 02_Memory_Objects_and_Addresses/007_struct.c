#include <stdio.h>

typedef struct data
{
  char   c;
  int    i;
  double d;
} Data;

int
main()
{
  Data data;
  printf("sizeof data = %zu\n", sizeof data);
  printf("sum of components sizes = %zu\n", sizeof data.c + sizeof data.i + sizeof data.d);
  printf("data   at %p\n", &data);
  printf("data.c at %p\n", &data.c);
  printf("data.i at %p\n", &data.i);
  printf("data.d at %p\n", &data.d);
}
