#include <stdio.h>

int
main ()
{
  int        i  = 42;
  int       *ip = &i;
  int const *cp = &i;

  for (int j = 0; j < 10; j++)
    {
      i++;
      printf ("*ip == %d, *cp == %d\n", *ip, *cp);
    }
}
