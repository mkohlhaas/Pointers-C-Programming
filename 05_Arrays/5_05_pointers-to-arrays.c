#include <assert.h>
#include <stdio.h>

int
main ()
{
  int array[10];

  printf ("%p, sizeof array = %zu\n", array, sizeof array);

  // For an array `array`, `&array` is typically the same address as the array itself.
  // The type, however, differs. Since `&array` is the address of an array,
  // naturally the type should be a pointer to an array.
  assert (array == &array); // same values but different types
  printf ("%p %p\n", array, &array);

  // Specifying that type requires a slightly off-putting syntax.
  // assert(42 == &array); // check type and its syntax in error/warning message

  int (*ap1)[] = &array;
  printf ("%p, *ap1 is an incomplete type\n", *ap1);
  // We cannot get sizeof *ap1, it is an incomplete type.
  // printf("%p, sizeof *ap1  = %2zu (%2zu)\n",
  //        *ap1,
  //        sizeof *ap1,     // invalid application to an incomplete type (size wasn't specified)
  //        10 * sizeof(int));

  int (*ap2)[10] = &array;
  printf ("%p %p %p\n", ap2, &array, *ap2);
  printf ("%p, sizeof *ap2  = %2zu (%2zu)\n",
          *ap2,        // you could use *ap2 or ap2
          sizeof *ap2, // you'll get a warning when using ap2 - using *ap2 is more consistent
          10 * sizeof (int));

  // wrong size will be reported
  int (*ap3)[5] = &array; // incompatible pointer types
  printf ("%p, sizeof *ap3  = %2zu (%2zu)\n", *ap3, sizeof *ap3, 5 * sizeof (int));

  // wrong size will be reported
  int (*ap4)[20] = &array; // incompatible pointer types
  printf ("%p, sizeof *ap4  = %2zu (%2zu)\n", *ap4, sizeof *ap4, 20 * sizeof (int));

  int *ip = array;
  printf ("%p, sizeof *ip   = %2zu (%2zu)\n", ip, sizeof ip, sizeof (int *));
}
