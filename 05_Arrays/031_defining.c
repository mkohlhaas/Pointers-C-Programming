#include <assert.h>
#include <stdio.h>
#include <string.h>

int main() {
  int a1[5];
  int a2[5] = {1, 2, 3}; // only init first three elements
  int a3[] = {1, 2, 3, 4, 5};
  size_t i = sizeof a3 / sizeof *a3;
  printf("Size of a3: %zu\n", i);

  // int a4[5] = { 1, 2, 3, 4, 5, 6 }; // excess elements in array initializer

  // assert(a1 == &a1); // comparison of distinct pointer types
  assert(a1 == &a1[0]); // a1[0] does not need to exist; only types are checked

  size_t n = strlen("string");
  char buffer[n + 1]; // variable length array (VLA): its length depends on a runtime variable
}
