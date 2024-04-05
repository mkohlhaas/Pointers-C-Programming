#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int int_compare(void const *x, void const *y) {
  int const *a = x;
  int const *b = y;
  return *a - *b;
}

int string_compare(void const *x, void const *y) {
  char *const *a = x;
  char *const *b = y;
  return strcmp(*a, *b);
}

bool is_sorted(void const *array, size_t len, size_t obj_size, __compar_fn_t cmp) {
  for (int i = 1; i < len; i++) {
    void const *a = (char *)array + (i - 1) * obj_size;
    void const *b = (char *)array + i * obj_size;
    if (cmp(a, b) > 0) {
      return false;
    }
  }
  return true;
}

void print_int_array(int n, int array[n]) {
  for (int i = 0; i < n; i++) {
    printf("%d ", array[i]);
  }
}

void print_string_array(int n, char *array[n]) {
  for (int i = 0; i < n; i++) {
    printf("%s ", array[i]);
  }
}

int main() {

  // Integers
  {
    // == Check Unsorted Integer Array
    int int_array[] = {10, 5, 30, 15, 20, 30};
    int int_array_length = sizeof int_array / sizeof *int_array;
    print_int_array(int_array_length, int_array);
    if (is_sorted(int_array, int_array_length, sizeof *int_array, int_compare)) {
      printf(" is sorted\n");
    } else {
      printf(" is not sorted\n");
    }

    // == Sort & Check Integer Array
    qsort(int_array, int_array_length, sizeof *int_array, int_compare);
    print_int_array(int_array_length, int_array);
    if (is_sorted(int_array, int_array_length, sizeof *int_array, int_compare)) {
      printf(" is sorted\n");
    } else {
      printf(" is not sorted\n");
    }
  }

  // Chars
  {
    // == Check Unsorted Char Array
    char *string_array[] = {"foo", "bar", "baz"};
    int string_array_length = sizeof string_array / sizeof *string_array;
    print_string_array(string_array_length, string_array);
    if (is_sorted(string_array, string_array_length, sizeof *string_array, string_compare)) {
      printf(" is sorted\n");
    } else {
      printf(" is not sorted\n");
    }

    // == Sort & Check Char Array
    qsort(string_array, string_array_length, sizeof *string_array, string_compare);
    print_string_array(string_array_length, string_array);
    if (is_sorted(string_array, string_array_length, sizeof *string_array, string_compare)) {
      printf(" is sorted\n");
    } else {
      printf(" is not sorted\n");
    }
  }
}
