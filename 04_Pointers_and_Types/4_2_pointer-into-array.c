#include <assert.h>
#include <stdio.h>

int
main ()
{
  int a[] = { 1, 2, 3, 4, 5 };
  int n   = sizeof a / sizeof *a;

  // get a pointer to the beginning of a
  int  *ip = a;
  char *cp = (char *)a;

  for (int i = 0; i < n; i++)
    {
      printf ("a[%d] sits at %p | %p | %p\n", i, &a[i], ip + i, cp + i * sizeof *a);
    }

  for (int i = 0; i < n; i++)
    {
      assert (ip + i == a + i);   // Add an integer to a pointer/array to get an element at an offset
      assert (ip + i == &a[i]);   // The offset is the address at that index
      assert (*(ip + i) == a[i]); // Dereference and you get the value
      assert (ip[i] == a[i]);     // The index operator is also valid for pointers!!!
    }
}
