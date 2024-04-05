#include <stdalign.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
  // `malloc()` is guaranteed to give you an address where ALL TYPES can be aligned.
  // Unless you are writing very specialized code, you do not need to use `aligned_alloc()`.
  int *ip = aligned_alloc(alignof(int), sizeof *ip);
  double *dp = aligned_alloc(alignof(double), sizeof *dp);

  // use pointers...

  free(ip);
  free(dp);

  // You can safely call `free()` with NULL pointers!
  // `free()` never fails and does not return a value according to the standard.
  free(NULL);
}
