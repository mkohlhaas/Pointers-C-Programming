#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "substr.h"

int
cmp_func(void const* x, void const* y)
{
  Substr const* a = x;
  Substr const* b = y;
  return substr_cmp(*a, *b);
}

int
main()
{
  char const* x = "foobarbaz";
  int         n = strlen(x);

  // create suffixes from base string 'x'
  Substr suffixes[n];
  for (int i = 0; i < n; ++i)
    suffixes[i] = slice((char*)x, i, n);

  printf("Suffixes before being sorted:\n");
  for (int i = 0; i < n; ++i) {
    print_substr(suffixes[i]);
    putchar('\n');
  }

  qsort(suffixes, n, sizeof *suffixes, cmp_func);

  printf("\nSuffixes after being sorted:\n");
  for (int i = 0; i < n; ++i) {
    print_substr(suffixes[i]);
    putchar('\n');
  }
}
