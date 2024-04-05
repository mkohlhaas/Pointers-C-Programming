#include <assert.h>
#include <stdio.h>
#include <string.h>

// Returns pointer to next different char (from the current one).
char const *skip(char const *x) {
  char c = *x;
  assert(c != '\0');
  while (*x == c) {
    x++;
  }
  return x;
}

// Use of `restrict` to signal non-overlapping buffers. (`sprintf` also demands restrict.)
void runlength_encode(char const *restrict input, char *restrict output) {
  while (*input) {
    char c = *input;
    char const *next = skip(input);
    int length = next - input;
    // `sprintf` returns the number of characters written; writes also terminating-zero.
    output += sprintf(output, "%d%c", length, c);
    input = next;
  }
}

int main() {
  char const *x = "aaaabbbbbbbaabbbcbbccccc";
  char buffer[2 * strlen(x) + 1]; // works only when repetitions < 10
  runlength_encode(x, buffer);
  printf("%s\n", buffer); // 4a7b2a3b1c2b5c
}
