#include <stdio.h>

int
factorial (int n)
{
  if (n <= 1)
    {
      return 1;
    }
  else
    {
      return n * factorial (n - 1);
    }
}

void
print_factorial (int n)
{
  printf ("Printing first %d factorials:\n", n);
  printf ("-----------------------------\n");
  for (int i = 0; i < n; i++)
    {
      printf ("%d! = %d\n", i, factorial (i));
    }
}

int
main ()
{
  print_factorial (10);
}
