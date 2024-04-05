#include <stdio.h>

int main() {
  printf("Sizes:\n");
  printf("------\n");
  printf("double = %zu\n", sizeof(double));
  printf("long   = %zu\n", sizeof(long));
  printf("int    = %zu\n", sizeof(int));
  printf("char   = %zu\n\n", sizeof(char));

  double d;
  double *dp = &d;
  long *lp = (long *)&d;
  int *ip = (int *)&d;
  char *cp = (char *)&d;

  printf("dp: %p\nlp: %p\nip: %p\ncp: %p\n\n", dp, lp, ip, cp);

  d = 65.31366;
  printf("*dp = %10f, *lp = %19ld, *ip = %8d, *cp = %c\n", *dp, *lp, *ip, *cp);
  *lp = 6531366;
  printf("*dp = %10f, *lp = %19ld, *ip = %8d, *cp = %c\n", *dp, *lp, *ip, *cp);
  *ip = 6531366;
  printf("*dp = %10f, *lp = %19ld, *ip = %8d, *cp = %c\n", *dp, *lp, *ip, *cp);
  *cp = 65;
  printf("*dp = %10f, *lp = %19ld, *ip = %8d, *cp = %c\n", *dp, *lp, *ip, *cp);
}
