#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef int T;

typedef struct {
  int i;
  double d;
} S;

int main() {
  // You can assign a void pointer to any other data pointer type without casting.
  int *ip = malloc(sizeof *ip);    // one integer
  double *dp = malloc(sizeof *dp); // one double
  S *sp = malloc(sizeof(S));       // one struct
  T *r = malloc(sizeof(S));        // no error(!)
  T *p = malloc(sizeof *p);        // one T

  int n = 10;
  int *arr = malloc(n * sizeof *arr); // 10 integers

  char *x = malloc(SIZE_MAX); // malloc can fail
  if (x) {
    printf("Success!\n"); // won't happen
    free(x);
  } else {
    perror("Allocation error");
    printf("Couldn't allocate %zu bytes.\n", SIZE_MAX);
  }

  free(ip); // memory is freed anyways by OS on exit
  free(dp); // but asan (address sanitizer) or Valgrind might complain if activated
  free(sp);
  free(r);
  free(p);
  free(arr);
}
