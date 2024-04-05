// You cannot assign to an array, so in that way arrays resemble const pointers.

#include <assert.h>
#include <stdio.h>
#include <string.h>

int main() {
  // a1
  printf("== a1\n");
  int a1[5];
  for (int i = 0; i < sizeof a1 / sizeof a1[0]; i++) {
    printf("%d\n", a1[i]);
  }

  // a2
  printf("== a2\n");
  int a2[5] = {1, 2, 3}; // only init first three elements
  for (int i = 0; i < sizeof a2 / sizeof a2[0]; i++) {
    printf("%d\n", a2[i]);
  }

  // a3
  printf("== a3\n");
  int a3[] = {1, 2, 3, 4, 5};
  size_t size_a3 = sizeof a3 / sizeof *a3;
  printf("Size of a3: %zu\n", size_a3);

  for (int i = 0; i < size_a3; i++) {
    printf("%d\n", a3[i]);
  }

  // a4
  printf("== a4\n");
  int a4[5] = {1, 2, 3, 4, 5, 6}; // excess elements in array initializer (will be ignored)
  for (int i = 0; i < sizeof a4 / sizeof a4[0]; i++) {
    printf("%d\n", a4[i]);
  }

  assert(a1 == &a1); // different pointer types, but same values(!)
  printf("%p %p\n", a1, &a1);

  // You can use an array as if it is a pointer to its first element,
  // and it degrades to that type automatically if you use it that way.
  assert(a1 == &a1[0]); // a1[0] does not need to exist; only types are checked

  size_t n = strlen("string");
  char buffer[n + 1]; // variable length array (VLA): its length depends on a runtime variable
}
