// "const doesn’t actually mean constant, it means that you want the type­checker
// to remind you to be explicit about your intent before you change the value."

// For any type T, `T const` is a constant of that type.
// For any type T, `T *`     is a pointer  to that type.

// Compile with higher optimization level to see a change:
// $ cc -O2 019_const.c -o 019_const

#include <stdio.h>

void
foo (int const *cip)
{
  int *ip;
  ip  = (int *)cip; // `const` is lost
  *ip = 5;
}

int
main ()
{
  int const i  = 42;
  int      *ip = (int *)&i;
  *ip          = 13;
  // i == 42 or i == 13?
  // Depends on optimization level of compiler!
  printf ("i = %2d, *ip = %2d\n", i, *ip);

  int j = 39;
  foo (&i); // undefined behavior as above; result depends on optimization level
  foo (&j); // you are allowed to change a non-onstant variable even if you have a const pointer to it
  printf ("i = %2d,   j = %2d\n", i, j);
}
