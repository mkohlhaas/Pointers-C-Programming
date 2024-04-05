#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int int_compare(void const *x, void const *y) {
  // Get the objects, and interpret them as integers
  int const *a = x;
  int const *b = y;
  return *a - *b;
}

int string_compare(void const *x, void const *y) {
  // Get the objects and interpet them as strings
  char *const *a = x;
  char *const *b = y;
  return strcmp(*a, *b);
}

typedef int (*CompareFN)(void const *, void const *);

bool is_sorted(void const *array, size_t len, size_t obj_size, CompareFN cmp) {
  for (int i = 1; i < len; i++) {
    void const *a = (char *)array + (i - 1) * obj_size;
    void const *b = (char *)array + i * obj_size;
    if (cmp(a, b) > 0)
      return false;
  }
  return true;
}

int main() {
  {
    // ========== INTEGERS
    // ==============================================================================================================
    // ========== Check Unsorted Integer Array
    // ==========================================================================================
    int int_array[] = {10, 5, 30, 15, 20, 30};
    int int_array_length = sizeof int_array / sizeof *int_array;
    if (is_sorted(int_array, int_array_length, sizeof *int_array, int_compare))
      printf("int_array    is sorted\n");
    else
      printf("int_array    is not sorted\n");
    // ========== Sort & Check Integer Array
    // ============================================================================================
    qsort(int_array, int_array_length, sizeof *int_array, int_compare);
    if (is_sorted(int_array, int_array_length, sizeof *int_array, int_compare))
      printf("int_array    is sorted\n");
    else
      printf("int_array    is not sorted\n");
    // ==================================================================================================================================
  }

  {
    // ========== CHARS
    // =================================================================================================================
    // ========== Check Unsorted Char Array
    // =============================================================================================
    char *string_array[] = {"foo", "bar", "baz"};
    int string_array_length = sizeof string_array / sizeof *string_array;
    if (is_sorted(string_array, string_array_length, sizeof *string_array, string_compare))
      printf("string_array is sorted\n");
    else
      printf("string_array is not sorted\n");
    // ========== Sort & Check Char Array
    // ===============================================================================================
    qsort(string_array, string_array_length, sizeof *string_array, string_compare);
    if (is_sorted(string_array, string_array_length, sizeof *string_array, string_compare))
      printf("string_array is sorted\n");
    else
      printf("string_array is not sorted\n");
    // ==================================================================================================================================
  }
}
