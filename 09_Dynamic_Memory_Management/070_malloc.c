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
  int *ip = malloc(sizeof *ip);
  double *dp = malloc(sizeof *dp);
  S *sp = malloc(sizeof(S));
  T *r = malloc(sizeof(S)); // no error!!!
  T *p = malloc(sizeof *p);

  int n = 10;
  int *arr = malloc(n * sizeof *arr);

  char *x = malloc(SIZE_MAX);
  if (x) {
    printf("Success!\n"); // won't happen
    free(x);
  } else {
    perror("Allocation error");
    printf("Couldn't allocate %zu bytes.\n", SIZE_MAX);
  }

  free(ip);
  free(dp);
  free(sp);
  free(r);
  free(p);
  free(arr);
}
