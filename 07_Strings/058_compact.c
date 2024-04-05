#include <ctype.h>
#include <stdio.h>
#include <string.h>

char *find_word(char *x) {
  while (*x && !isalpha(*x))
    x++;
  return x;
}

void copy_word(char **from, char **to) {
  while (isalpha(**from))
    *(*to)++ = *(*from)++;
}

void copy_words(char *from, char *to) {
  from = find_word(from);
  while (*from) {
    copy_word(&from, &to);  // from,to will be advanced
    from = find_word(from); // find next word
    if (*from)
      *to++ = ' '; // insert space
  }
  *to = '\0';
}

void compact_words(char *s) {
  // works because 'to' <= 'from'
  // characters will be moved to the left or stay where they are.
  copy_words(s, s);
}

int main() {
  // We modify the string we wordize, so we cannot use a literate string. Those are immutable.
  // This initialises a mutable stack-allocated buffer instead. The string is copied into
  // the buffer, including the zero-terminal, when the buffer is initialised.

  char words[] = "First  \t asdlfkj  \t   34  \t  2345\t  \t  2345  \t  sdfk  \t  kj \t   230 \t   asdo  \t  13  \t  "
                 "aoanq \t   2424 \t   eqycp";
  printf("\033[38;5;206mOriginal string:\033[0m  %s\n", words);
  compact_words(words);
  printf("\033[38;5;206mCompacted string:\033[0m %s\n", words);
}
