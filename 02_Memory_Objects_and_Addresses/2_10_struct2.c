#include <stdalign.h>
#include <stdio.h>

typedef struct data
{
  int  i;
  char c[9];
} Data;

int
main ()
{
  printf ("sizeof components    = %zu\n", sizeof (char[9]) + sizeof (int));
  printf ("sizeof Data          = %zu\n", sizeof (Data));
  printf ("alignof Data         = %zu\n", alignof (Data));
  printf ("alignment of int     = %zu\n", alignof (int));
  printf ("alignment of char[9] = %zu\n", alignof (char[9]));
}
