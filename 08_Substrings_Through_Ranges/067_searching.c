#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "substr.h"

int
main()
{
  {
    char        x[]  = "foobarbazqux";
    SubstrIter iter  = as_substr(x);
    Substr      ba   = as_substr("ba");
    for (Substr word = next_occurrence(&iter, ba, 0);
                !is_null_substr(word);
                word = next_occurrence(&iter, ba, 0)) {
      print_substr(word);
      printf("\n");
    }
  }
}
