#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
int_compare (void const *x, void const *y)
{
  int const *a = x;
  int const *b = y;
  return *a - *b;
}

int
string_compare (void const *x, void const *y)
{
  char *const *a = x;
  char *const *b = y;
  return strcmp (*a, *b);
}

void
swap (void *a, void *b, size_t obj_size)
{
  char tmp[obj_size];
  memcpy (&tmp, a, obj_size);
  memcpy (a, b, obj_size);
  memcpy (b, &tmp, obj_size);
}

void
swap_down (char *start, char *current, size_t obj_size, __compar_fn_t cmp)
{
  char *prev;
  while (current != start)
    {
      prev = current - obj_size;
      if (cmp (prev, current) <= 0)
        {
          break; // done swapping if prev <= current
        }
      swap (prev, current, obj_size);
      current = prev;
    }
}

void
insertion_sort (void *array, size_t len, size_t obj_size, __compar_fn_t cmp)
{
  char *start = array;
  for (int i = 1; i < len; i++)
    {
      swap_down (start, start + i * obj_size, obj_size, cmp);
    }
}

void
print_int_array (int n, int array[n])
{
  for (int i = 0; i < n; i++)
    {
      printf ("%d ", array[i]);
    }
  printf ("\n");
}

void
print_string_array (int n, char *array[n])
{
  for (int i = 0; i < n; i++)
    {
      printf ("%s ", array[i]);
    }
  printf ("\n");
}

#define ARRAY_SIZE(a) (sizeof a / sizeof *a)

int
main ()
{

  // Sort Integer Array
  {
    int int_array[]      = { 10, 5, 30, 15, 20, 30 };
    int int_array_length = ARRAY_SIZE (int_array);
    printf ("Original integer array:\n");
    print_int_array (int_array_length, int_array);

    insertion_sort (int_array, int_array_length, sizeof *int_array, int_compare);
    printf ("Sorted array:\n");
    print_int_array (int_array_length, int_array);
  }

  printf ("\n");

  // Sort String Array
  {
    char *string_array[]      = { "foo", "bar", "baz" };
    int   string_array_length = ARRAY_SIZE (string_array);
    printf ("Original string array:\n");
    print_string_array (string_array_length, string_array);

    insertion_sort (string_array, string_array_length, sizeof *string_array, string_compare);
    printf ("Sorted array:\n");
    print_string_array (string_array_length, string_array);
  }
}
