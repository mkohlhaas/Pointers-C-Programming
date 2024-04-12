#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int
main ()
{
  int    *ip = calloc (10, sizeof *ip); // allocate 10 integers
  double *dp = calloc (20, sizeof *dp); // allocate 20 doubles

  for (int i = 0; i < 10; i++)
    {                                   // integers initialized to 0
      printf ("%d\n", ip[i]);
    }

  printf ("\nOn my platform the doubles are 0.0.\nBut not guaranteed by the C standard.\n");
  for (int i = 0; i < 20; i++)
    { // doubles initialized to 0.0
      printf ("%f\n", dp[i]);
    }

  free (ip);
  free (dp);

  // calloc checks for overflow on its own!
  // calloc zero-bits the allocated memory
  char *x = calloc (10, SIZE_MAX / 50);
  if (x)
    {
      printf ("Success!n"); // won't happen
    }
  else
    {
      perror ("Allocation error");
    }
}
