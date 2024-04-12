#include <stdio.h>

int
main ()
{
  int        *p = NULL;
  int const **q = NULL;
  int const   i = 42;

  printf ("Before: i = %d, p = %p\n", i, p);

  // You think you restrict what you can do, and therfore that cannot cause problems...
  q = (int const **)&p; // is like: ic_p_p = i_p_p (forbidden; but we do an explicit cast to see what could happen)
                        // p = &i;               // not allowed
  *q = &i;              // same as line before but is allowed

  // ... but now we have an int alias/reference to an int const!
  printf ("&i = %p\n p = %p\n", &i, p); // p = &i (which was not allowed)

  *p = 5;                               // DANGER: We are trying to change const int i to 5.
  // This may or may not actually change i. It is up to the C compiler.
  printf ("After:  i = %d, *p = %d\n", i, *p); // gcc changes the value of i; i is not so constant after all!

  // Summary: if ic_p_p = i_p_p would be allowed we could create aliases and could change the constant integer!
}
