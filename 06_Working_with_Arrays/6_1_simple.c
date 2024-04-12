#define ARRAY_SIZE(a) (sizeof a / sizeof *a)

#include <stdio.h>

// =============== Add ========================================

void
add_array (int n, int array[n], int x)
{
  for (int i = 0; i < n; i++)
    {
      array[i] += x;
    }
}

void
add_pointers (int *start, int *end, int x)
{
  while (start < end)
    {
      *start++ += x;
    }
}

void
add_pointers1 (int *start, int *end, int x)
{
  for (; start < end; start++)
    {
      *start += x;
    }
}

// =============== Swap =======================================

void
swap_array (int array[], int i, int j)
{
  int tmp  = array[j];
  array[j] = array[i];
  array[i] = tmp;
}

void
swap_pointers (int *left, int *right)
{
  // printf("i: %p, j: %p\n", i, j);
  int temp;
  temp   = *left;
  *left  = *right;
  *right = temp;
}

// =============== Reverse ====================================

void
reverse_array (int n, int array[n])
{
  for (int i = 0; i < n / 2; i++)
    {
      swap_array (array, i, n - i - 1);
    }
}

void
reverse_pointers (int *start, int *end)
{
  // convention is to point 'end' one past last element
  // printf("start: %p, end: %p\n", start, end);
  if (start == end)
    {
      return;
    }
  // C standard does not allow to point outside the array or one.
  // Therefore the previous test. We could underflow `end`.
  end--;                              // element past the last one; 'end' points to last element after decement
  while (start < end)
    {
      swap_pointers (start++, end--); // start/end will be inc-/decremented AFTER calling swap_pointers()!!!
    }
}

// =============== Sum ========================================

int
sum_array (int n, int array[n])
{
  int sum = 0;
  for (int i = 0; i < n; i++)
    {
      sum += array[i];
    }
  return sum;
}

int
sum_pointers (int *start, int *end)
{
  int sum = 0;
  while (start < end)
    {
      sum += *start++;
    }
  return sum;
}

int
sum_pointers1 (int *start, int *end)
{
  int sum = 0;
  for (; start < end; start++)
    {
      sum += *start;
    }
  return sum;
}

// =============== Print ======================================

void
print_array (int n, int array[n])
{
  printf ("[ ");
  for (int i = 0; i < n; i++)
    {
      printf ("%d ", array[i]);
    }
  printf ("]\t");
}

// =============== Main =======================================

int
main ()
{
  int array[] = { 1, 2, 3, 4, 5 };
  int n       = ARRAY_SIZE (array);

  print_array (n, array);
  printf ("Original array\n");

  add_array (n, array, 2);
  print_array (n, array);
  printf ("Added 2 to each element (array fn)\n");

  add_pointers (array, array + n, -2);
  print_array (n, array);
  printf ("Subtracted 2 from each element (pointers fn)\n");

  reverse_array (n, array);
  print_array (n, array);
  printf ("Reversed array (array fn)\n");

  reverse_pointers (array, array + n);
  print_array (n, array);
  printf ("Reversed array again (pointers fn)\n");

  printf ("%d %d\t\t", sum_array (n, array), sum_pointers (array, array + n));
  printf ("Summed array twice (array & pointers fn)\n");
}
