#include <ctype.h>
#include <stdio.h>

char *find_word(char *x) {
  while (*x && !isalpha(*x)) {
    x++;
  }
  return x;
}

// `*p++`, which is `*(p++)`, dereferences and then increments the pointer,
// `(*p)++` increments the object the pointer holds the address of!
// `from` and `to` are references and are advanced.
void copy_word(char **from, char **to) {
  while (isalpha(**from)) {
    *(*to)++ = *(*from)++;
  }
}

void copy_words(char *from, char *to) {
  from = find_word(from);
  while (*from) {
    copy_word(&from, &to);  // `from` and `to` will be advanced
    from = find_word(from); // find next word
    if (*from) {
      *to++ = ' '; // insert space
    }
  }
  *to = '\0';
}

void compact_words(char *s) {
  // Works because 'to' <= 'from'.
  // Characters will be moved to the left or stay where they are.
  copy_words(s, s);
}

int main() {
  // We modify the string we wordize, so we cannot use a literate string. Those are immutable.
  // This initialises a mutable stack-allocated buffer instead. The string is copied into
  // the buffer, including the zero-terminal, when the buffer is initialised.

  // Compare pointer to literal string in `7_06_literal-string.c`.
  char words[] = "the 34 only 24 thing we 134 have to 76 fear is fear 666 itself";
  printf("Original string:  %s\n", words);
  compact_words(words);
  printf("Compacted string: %s\n", words);
}
