#include <stdio.h>

int
main ()
{
  char   c = 'c';
  int    i = 2;
  double d = 3.0;
  printf ("%p %10c\n", &c, c);
  printf ("%p %10d\n", &i, i);
  printf ("%p %10f\n", &d, d);
}
