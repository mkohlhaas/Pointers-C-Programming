#include "substr.h"
#include <stdbool.h>
#include <stdio.h>

int main() {
  char x[] = "foobarbazqux";
  SubstrIter iter = as_substr(x);
  Substr ba = as_substr("ba");
  for (Substr word = next_occurrence(&iter, ba, false); !is_null_substr(word); word = next_occurrence(&iter, ba, false)) {
    print_substr(word);
    printf("\n");
  }
}
