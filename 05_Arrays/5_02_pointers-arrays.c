#include <assert.h>
#include <stdio.h>

void not_what_you_want(int array[]) { // Type of array: `int *` (!)
  // will return size of 'int*' not of 'int[]'
  printf("%zu\n", sizeof array);
  // The array and the address of the array are different.
  // array is a local variable that holds a pointer to the array!
  printf("%p %p\n", array, &array);
}

int main() {
  int array[] = {1, 2, 3, 4, 5};
  int *ap = array;
  int(*ap2)[] = &array;

  printf("sizeof array = %zu, sizeof ap = %zu, sizeof ap2 = %zu\n", sizeof array, sizeof ap, sizeof ap2);

  printf("------------------------------------------------------------\n");
  printf("%p\n%p\n%p\n%p\n%p\n%p\n", array, &array, ap, ap2, &ap, &ap2);

  // You can use indexes on pointers and pointer arithmetic on arrays.
  size_t n = sizeof array / sizeof *array;
  for (int i = 0; i < n; i++) {
    assert(array[i] == ap[i]);
    assert(ap[i] == *(ap + i));
    assert(array[i] == *(array + i));
    assert(array + i == ap + i);
  }

  printf("------------------------------------------------------------\n");
  // Pointing one past the last element of an array - which is explicitly guaranteed
  // by the standard - is a useful pattern for looping through a range.
  int *range_begin = array;
  int *range_end = array + n; // n is the length of the array
  for (int *p = range_begin; p < range_end; p++) {
    printf("%d\n", *p);
  }

  printf("------------------------------------------------------------\n");
  not_what_you_want(array); // Type of `array`: `int[5]`
}
