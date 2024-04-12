#include <assert.h>
#include <stdio.h>
#include <string.h>

int
main ()
{
  char const *string = "Hello, world!\n";
  char const *cp     = string;
  int         n      = strlen (string);

  printf ("%s", string);
  printf ("%s", cp);
  printf ("length: %d\n", n);

  for (int i = 0; i < n; i++)
    {
      assert (string[i] == cp[i]);
      assert (string + i == cp + i);
      assert (*(string + i) == *(cp + i));
    }
}
