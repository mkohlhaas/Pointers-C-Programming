#include <stdalign.h>
#include <stdio.h>

int
main ()
{
  printf ("chars   align at %zu and have size %zu.\n", alignof (char), sizeof (char));
  printf ("ints    align at %zu and have size %zu.\n", alignof (int), sizeof (int));
  printf ("doubles align at %zu and have size %zu.\n", alignof (double), sizeof (double));
}
