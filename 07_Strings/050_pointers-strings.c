#include <assert.h>
#include <stdio.h>
#include <string.h>

int main() {
  char const *string = "hello, world!\n";
  char const *cp = string;
  int n = strlen(string);

  for (int i = 0; i < n; i++) {
    assert(string[i] == cp[i]);
    assert(string + i == cp + i);
    assert(*(string + i) == *(cp + i));
  }
}
