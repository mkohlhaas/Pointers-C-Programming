#include <stdio.h>

// =============== Add ========================================

// 'array' is int*
void
add_array(int n, int array[n], int x)
{
  // struct S* s = array;
  for (int i = 0; i < n; i++) array[i] += x;
}

void
add_pointers (int* begin, int* end, int x)
{
  while (begin < end) *begin++ += x;
}

void
add_pointers_(int* begin, int* end, int x)
{
  for (; begin < end; begin++) *begin += x;
}

// =============== Swap =======================================

// 'array' is int*
void
swap_array(int array[], int i, int j)
{
  // struct S* s = array;
  int tmp  = array[j];
  array[j] = array[i];
  array[i] = tmp;
}

void
swap_pointers(int* left, int* right)
{
  // printf("i: %p, j: %p\n", i, j);
  int temp;
   temp  = *left;
  *left  = *right;
  *right =  temp;
}

// =============== Reverse ====================================

// 'array' is int*
void
reverse_array(int n, int array[n])
{
  // struct S* s = array;
  for (int i = 0; i < n / 2; i++) swap_array(array, i, n - i - 1);
}


void
reverse_pointers(int* begin, int* end)
{
  // convention is to point 'end' one past last element
  // printf("begin: %p, end: %p\n", begin, end);
  if (end == begin) return;         // C standard does not allow to point outside the array or one
  end--;                            // element past the last one; 'end' points to last element
  while (begin < end)
    swap_pointers(begin++, end--);  // begin/end will be inc-/decremented AFTER calling swap_pointers()!!!
}

// =============== Sum ========================================

// 'array' is int*
int
sum_array(int n, int array[n])
{
  // struct S* s = array;
  int sum = 0;
  for (int i = 0; i < n; i++) sum += array[i];
  return sum;
}

int
sum_pointers (int* begin, int* end)
{
  int sum = 0;
  while (begin < end) sum += *begin++;
  return sum;
}

int
sum_pointers_(int* begin, int* end)
{
  int sum = 0;
  for (; begin < end; begin++) sum += *begin;
  return sum;
}

// =============== Print ======================================

// 'array' is int*
void
print_array(int n, int array[n])
{
  // struct S* s = array;
  printf("[ ");
  for (int i = 0; i < n; i++) printf("%d ", array[i]);
  printf("]\t");
}

// =============== Main =======================================

int
main()
{
  int array[] = { 1, 2, 3, 4, 5 };
  int n       = sizeof array / sizeof *array;
  // struct S* s = array;
  
  print_array      (n, array);                                               printf("Original array\n");
  add_array        (n, array, 2);                    print_array(n, array);  printf("Added      2 to   each element (array    fn)\n");
  add_pointers     (array, array + n, -2);           print_array(n, array);  printf("Subtracted 2 from each element (pointers fn)\n");
  reverse_array    (n, array);                       print_array(n, array);  printf("Reversed array       (array fn)\n");
  reverse_pointers (array, array + n);               print_array(n, array);  printf("Reversed array again (pointers fn)\n");
  printf("%d %d\t\t", sum_array(n, array), sum_pointers(array, array + n));  printf("Summed array twice (array & pointers fn)\n");
}
