// strict alias rule: whatever pointers of different types point at, they are different objects.
// strict alias rule does not apply to char pointers.
// Turn on your compiler’s optimization, and it will exploit this:
// cc -O2 028_strict-alias.c -o 028_strict-alias
// The general rule is that you should never alias objects of different types.

#include <stdalign.h>
#include <stdio.h>

int f(int *i, long *l) {
  *i = -1; // strict alias rule says that i and l cannot point to the same object,
  *l = 0;  // because they do not have compatible types
  return *i;
}

int g(char *c, long *l) {
  *c = -1;   // With a char pointer, the strict aliasing rule does not apply. Character pointers
  *l = 0;    // are special in that they are always allowed to point to any other object, and
  return *c; // if the object is mutable, we are allowed to modify the object we point to.
}

int h(double *c, long *l) {
  *c = -1;
  *l = 0;
  return (int)*c;
}

int main() {
  long x;
  int i;

  i = f((int *)&x, &x);
  printf("x = %ld, f(&x,&x) = %2d\n", x, i);
  i = g((char *)&x, &x);
  printf("x = %ld, g(&x,&x) = %2d\n", x, i);
  i = h((double *)&x, &x);
  printf("x = %ld, h(&x,&x) = %2d\n", x, i);
}
