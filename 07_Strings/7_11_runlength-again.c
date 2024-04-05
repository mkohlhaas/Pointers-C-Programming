// use safer string functions like `snprintf()` instead of `sprintf()`

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

char const *skip(char const *x) {
  char c = *x;
  assert(c != '\0');
  while (*x == c) {
    x++;
  }
  return x;
}

bool runlength_encode_n(char const *restrict input, char *restrict output, int n) {
  while (*input) {
    printf("Current buffer left: n = %d\n", n);
    char c = *input;
    char const *next = skip(input);
    int length = next - input;
    int used = snprintf(output, n, "%d%c", length, c); // safe function
    output += used;
    n -= used;
    if (n < 0) {
      printf("No buffer space left!\n");
      return false;
    }
    input = next;
  }
  return true;
}

int main() {
  char const *x = "aaabbbbbaabbbccccccccc"; // ✓
  // char const *x = "aaaabbbbbbbaabbbcbbccccc"; // ✗

  int n = 11; // including terminal zero
  char buffer[n];

  printf("Original string: %s\n", x);
  printf("Initial buffer size: %d\n", n);

  if (runlength_encode_n(x, buffer, n)) {
    printf("We encoded the entire string.\n");
  } else {
    printf("We only got a prefix.\n");
  }
  printf("Encoding: %s\n", buffer);
}
