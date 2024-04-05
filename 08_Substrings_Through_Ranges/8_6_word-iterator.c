#include "substr.h"
#include <stdio.h>

int main() {
  char x[] = "\tfoo    bar123baz\nqux321";
  SubstrIter iter = as_substr(x);
  for (Substr word = first_word(&iter); !is_null_substr(word); word = next_word(&iter)) {
    print_substr(word);
    printf(" ");

    char c = insert_zero_term(word);
    printf("%s\n", word.begin);
    restore_term(word, c);
    print_substr(word);
    printf("\n");
  }

  Substr xx = compact_words(as_substr(x));
  char c = insert_zero_term(xx);
  printf("compacted words: %s\n", x);
  restore_term(xx, c);
  print_substr(xx);
  printf("\n");
}
