#include "substr.h"
#include <stdbool.h>
#include <stdio.h>

int main() {
  Substr x;
  Substr y;
  SubstrIter iter;

  {
    x = as_substr("foobarbazqux");
    y = as_substr("ba");
    iter = x;

    printf("---------- Searching for %s in %s -------------------------\n", y.begin, x.begin);
    for (Substr occ = first_occurrence(&iter, y, false); !is_null_substr(occ); occ = next_occurrence(&iter, y, false)) {
      printf("Found an occurrence at index %ld\n", occ.begin - x.begin);
    }
  }

  {
    x = as_substr("xaxaxaxaxaxa");
    y = as_substr("xaxa");
    iter = x;

    printf("\n---------- Non-overlapping search for %s in %s ----------\n", y.begin, x.begin);
    for (Substr occ = first_occurrence(&iter, y, false); !is_null_substr(occ); occ = next_occurrence(&iter, y, false)) {
      printf("Found an occurrence at index %ld\n", occ.begin - x.begin);
    }
  }

  {
    x = as_substr("xaxaxaxaxaxa");
    y = as_substr("xaxa");
    iter = x;

    printf("\n---------- Overlapping search for %s in %s --------------\n", y.begin, x.begin);
    for (Substr occ = first_occurrence(&iter, y, true); !is_null_substr(occ); occ = next_occurrence(&iter, y, true)) {
      printf("Found an occurrence at index %ld\n", occ.begin - x.begin);
    }
  }
}
