#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int
main()
{
  int*    ip = calloc(10, sizeof *ip);  // allocate 10 integers
  double* dp = calloc(20, sizeof *dp);  // allocate 20 doubles

  free(ip); free(dp);

  // calloc checks for overflow on its own!
  // calloc zero-bits the allocated memory
  char* x = calloc(10, SIZE_MAX / 5);
  if   (x) printf("Success!n");        // won't happen
  else     perror("Allocation error");
}
