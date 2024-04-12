#include "substr.h"
#include <stdio.h>

int
main ()
{
  {
    char   x[] = "abc";
    Substr y   = as_substr (x);
    printf ("%p %c, %p %c\n", y.begin, *y.begin, y.end, *y.end);
  }

  {
    char x[] = "abc";
    substr_reverse (as_substr (x));
    printf ("%s\n", x);
  }

  {
    char x[] = "";
    substr_reverse (as_substr (x));
    printf ("%s\n", x);
  }

  {
    char x[] = "a";
    substr_reverse (as_substr (x));
    printf ("%s\n", x);
  }

  {
    char x[] = "ab";
    substr_reverse (as_substr (x));
    printf ("%s\n", x);
  }
}
