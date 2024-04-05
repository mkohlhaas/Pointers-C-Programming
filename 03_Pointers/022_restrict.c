// use compiler optimization to see a difference: cc -O2 022_restrict.c -lm -o 022_restrict
//
// When we use restrict with a pointer ptr, it tells the compiler that ptr is
// the only way to access the object pointed by it, in other words, there’s no
// other pointer pointing to the same object i.e. restrict keyword specifies
// that a particular pointer argument does not alias any other and the compiler
// doesn’t need to add any additional checks.

// If a programmer uses restrict keyword and violates the above condition, the
// result is undefined behavior.

#include <stdio.h>

void abc(int *a, int *b, int *c) {
  *a += *c;
  *b += *c;
}

void abc_restrict(int *a, int *b, int *restrict c) {
  *a += *c;
  *b += *c;
}

int main() {
  int x, y;
  x = y = 13;
  // No problem here. We haven't made any restrict promises
  abc(&x, &y, &x);
  printf("%d %d\n", x, y);

  // we break the promise here
  x = y = 13;
  abc_restrict(&x, &y, &x);
  printf("%d %d\n", x, y);
}
