#include <stdio.h>

typedef union data
{
  char   c;
  int    i;
  double d;
} Data;

#define MAX(a, b)     (((a) > (b)) ? (a) : (b))
#define MAX3(a, b, c) MAX((a), MAX((b), (c)))

int
main()
{
  Data data;
  printf("sizeof data = %zu\n", sizeof data);
  printf("max size of components = %zu\n",
         MAX3(sizeof data.c, sizeof data.i, sizeof data.d));
  printf("data   at %p\n", &data);
  printf("data.c at %p\n", &data.c);
  printf("data.i at %p\n", &data.i);
  printf("data.d at %p\n", &data.d);
}
