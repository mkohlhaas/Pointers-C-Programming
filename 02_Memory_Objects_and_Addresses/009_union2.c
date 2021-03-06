#include <stdio.h>

typedef union data
{
  char   c[9];
  double d;
} Data;

#define MAX(a, b) (((a) > (b)) ? (a) : (b))

int
main()
{
  Data data;
  printf("sizeof data = %zu\n", sizeof data);
  printf("size of max component = %zu\n", MAX(sizeof data.c, sizeof data.d));
  printf("data   at %p\n", &data);
  printf("data.c at %p\n", &data.c);
  printf("data.d at %p\n", &data.d);
}
