#include <assert.h>
#include <stdio.h>
#include <string.h>

char const *skip(char const *x) {
  char c = *x;
  assert(c != '\0');
  while (*x == c)
    x++;
  return x;
}

// use of 'restrict' to signal non-overlapping buffers
// sprintf() returns the number of characters written; writes also terminating-zero
void runlength_encode(char const *restrict input, char *restrict output) {
  while (*input) {
    char c = *input;
    char const *next = skip(input);
    int length = next - input;
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
