#include <stdio.h>
#include "substr.h"

int
main()
{
  Substr     x     = as_substr("foobarbazqux");
  Substr     y     = as_substr("ba");
  SubstrIter iter  = x;

  {
    printf("---------- Searching for %s in %s -------------------------\n", y.begin, x.begin);
    for (Substr occ = next_occurrence(&iter, y, 0);
                !is_null_substr(occ);
                occ = next_occurrence(&iter, y, 0))
      { printf("Found an occurrence at index %ld\n", occ.begin - x.begin); }
  }

  {
    x    = as_substr("xaxaxaxaxaxa");
    y    = as_substr("xaxa");
    iter = x;
    printf("\n---------- Non-overlapping search for %s in %s ----------\n", y.begin, x.begin);
    for (Substr occ = next_occurrence(&iter, y, 0);
                !is_null_substr(occ);
                occ = next_occurrence(&iter, y, 0))
      { printf("Found an occurrence at index %ld\n", occ.begin - x.begin); }
  }

  {
    iter = x;
    printf("\n---------- Overlapping search for %s in %s --------------\n", y.begin, x.begin);
    for (Substr occ = next_occurrence(&iter, y, 1);
                !is_null_substr(occ);
                occ = next_occurrence(&iter, y, 1))
      { printf("Found an occurrence at index %ld\n", occ.begin - x.begin); }
  }
}
