#include <assert.h>
#include <limits.h>
#include <stdio.h>

#define BUCKET_SIZE            (1 << CHAR_BIT)
// shift 'offset' bytes to the right into the first byte
#define SEARCH_EXPR(i, offset) (input[(i)] >> CHAR_BIT * (offset)) & (BUCKET_SIZE - 1);

typedef unsigned char UChar;

// ================================================================================================================
// =============== Radix Sort =====================================================================================
// ================================================================================================================

void
bucket_sort(int n, int offset, int const input[n], int output[n])
{
  // init buckets
  int buckets[BUCKET_SIZE];
  for (size_t i = 0; i < BUCKET_SIZE; i++) buckets[i] = 0;

  // count
  for (size_t i = 0; i < n; i++) {
    UChar bucket = SEARCH_EXPR(i, offset);
    buckets[bucket]++;
  }

  // go from right to left
  int m = n;
  for (int i = (BUCKET_SIZE - 1); i >= 0; i--) {
    int count   = buckets[i];
    buckets[i]  = m - count;
    m          -= count;
  }

  // sort
  for (size_t i = 0; i < n; i++) {
    UChar bucket;
    int   index;
    bucket        = SEARCH_EXPR(i, offset);
    index         = buckets[bucket]++;
    output[index] = input[i];
  }
}

void
print_array(int n, int array[n])
{
  for (size_t i = 0; i < n; i++) printf("%3d |", array[i]);
  putchar('\n');
}

// radix sort is a series of bucket sorts on every single byte starting with LSB; for integers we have 4 bytes
void
radix_sort(int n, int array[n])
{
  // It is VERY unlikely that sizeof an integer is odd, but if it is, you need to move the results from helper
  // to array. I assume that we have an even number of bytes because that is practically always true for int
  static_assert(sizeof *array % 2 == 0, "integer sizes must be powers of two");
  if (n <= 0) return;
  int  helper[n];                            // Helper buffer; handle input/output switches when bucket sorting
  int* buffers[]    = { array, helper };     // For switching between the buffers
  int  bucket_input = 0;                     // buffer switch (0/1)

  // If integers have size of 4 we move 4 times in between buffers:
  // 1. array -> helper, 2. helper -> array, 3. array -> helper, 4. helper -> array 
  // -> result is in 'array'
  for (size_t offset = 0; offset < sizeof *array; offset++) {
    printf("offset %zd: ", offset);
    bucket_sort(n, offset, buffers[bucket_input], buffers[!bucket_input]);
    print_array(n, buffers[!bucket_input]);
    bucket_input = !bucket_input;
  }
}

// ================================================================================================================
// =============== Insertion Sort =================================================================================
// ================================================================================================================

// Both left and right must point to legal addresses
// scan from left to right, return left
int*
leftmost_positive_number(int* left, int* right)
{
  while (left < right && *left < 0) left++;
  printf("left:  %p, %3d\n", left, *left);
  return left;
}

// Both left and right must point to legal addresses
// scan from right to left, return right
int*
rightmost_negative_number(int* left, int* right)
{
  while (left < right && *right >= 0) right--;
  printf("right: %p, %3d\n", right, *right);
  return right;
}

// Both left and right must point to legal addresses
void
swap(int* left, int* right)
{
  int temp;
   temp  = *left;
  *left  = *right;
  *right =  temp;
}

// move negativ numbers to the front
// return number of negative numbers (= index of first positive number)
int
split(int n, int array[n])
{
  int* left  = array;
  int* right = array + n - 1;
  while (left < right) {
    left  =  leftmost_positive_number(left, right);
    right = rightmost_negative_number(left, right);
    swap(left, right);
  }
  return left - array;
}

// not needed
void
reverse(int n, int array[n])
{
  int* left  = array;
  int* right = array + n - 1;
  while (left < right) swap(left++, right--);
}

void
sort_int(int n, int array[n])
{
  if (n <= 0) return;
  // move negative numbers to the front
  int m = split(n, array);
  printf("\nNegative numbers: %d\n", m);
  print_array(n, array);

  // sort negative numbers
  printf("\nSorting negative numbers:\n");
  radix_sort(m, array);
  print_array(n, array);

  // sort positive numbers
  printf("\nSorting positive numbers:\n");
  radix_sort(n - m, array + m);
  print_array(n, array);
}

int
main()
{
  {
    int array[] = { 43, -13, 6, -14, 3, -42, 13, 1, -2, 13, 12, 4, };
    int n       = sizeof array / sizeof *array;

    printf("\n\033[38;5;206m");
    printf("|-------------------------------------------------------------------------|\n");
    printf("|                       1. Radix Sort                                     |\n");
    printf("|-------------------------------------------------------------------------|\n\n");
    printf("\033[0m");
	printf("Original array: ");
    print_array(n, array);

    printf("\n===========================================================================\n");
    printf("Radix sort");
    printf("\n===========================================================================\n");
    radix_sort(n, array);
    printf("\nResult (negatives are wrong):\n");
    print_array(n, array);
  }

  {
    int array[] = { 43, -13, 6, -14, 3, -42, 13, 1, -2, 13, 12, 4, };
    int n       = sizeof array / sizeof *array;

    printf("\n\033[38;5;206m");
    printf("|-------------------------------------------------------------------------|\n");
    printf("|                       2. Integer Sort                                   |\n");
    printf("|-------------------------------------------------------------------------|\n\n");
    printf("\033[0m");
	printf("Original array: ");
    print_array(n, array);

    printf("\n===========================================================================\n");
    printf("Integer sort");
    printf("\n===========================================================================\n");
    sort_int(n, array);
    printf("\nResult:\n");
    print_array(n, array);
    putchar('\n');
  }
}
