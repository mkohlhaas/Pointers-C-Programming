// Arrays and Function Arguments

// If you use arrays as function parameters, you get pointers.
// When you call a function with an array argument, it decays
// into a pointer, and the function will treat it as such.

#include <stdio.h>

// standard pointer
// a is a pointer
void pointer(int *a) {
  // struct S* b = a;  // just how to see what the type of a is
  printf("pointer: %zu %zu\n", sizeof a, sizeof *a);
}

// array without size
// a decays into int*
void array(int a[]) {
  // struct S* b = a;  // just how to see what the type of a is
  printf("array: %zu %zu\n", sizeof a, sizeof *a);
}

// array with size
// a decays into int*
// function does not check size
void array_with_size(int a[50]) {
  // struct S* b = a;  // just how to see what the type of a is
  printf("array[50]: %zu %zu\n", sizeof a, sizeof *a);
}

// array with variable size
// a decays into int*
// function does not check size
void array_with_parameter_size(int n, int a[n]) {
  // struct S* b = a;  // just how to see what the type of a is
  printf("array[n]: %zu %zu\n", sizeof a, sizeof *a);
}

// array with constrained size
// a decays into int*
// function does check size, but only if you pass an array and not an int pointer (int*)
void size_constrained(int a[static 4]) {
  // struct S* b = a;  // just how to see what the type of a is
  printf("size constrained a[0] == %d\n", a[0]);
}

// calling constrained arrays with different sizes
// a decays into int*
// function does not check size
void indirect_size_constrained(int a[static 2]) {
  // struct S* b = a;  // just how to see what the type of a is
  size_constrained(a); // No warning, though 2 < 4 and function checks size
}

// array pointer
// a is int (*)[3]
// function does check size; accepts different sizes but they must be >= 3
void pointer_to_array(int (*a)[3]) {
  // struct S* b = a;  // just how to see what the type of a is
  printf("*a: %zu = %zu x %zu\n", sizeof *a, sizeof *a / sizeof **a, sizeof **a);
}

// pointer to variable array
// a is int (*a)[n]
// function does not check size
void pointer_to_array_n(int n, int (*a)[n]) {
  // struct S* b = a;  // just how to see what the type of a is
  printf("*a with n = %d: %zu = %zu x %zu\n", n, sizeof *a, sizeof *a / sizeof **a, sizeof **a);
}

// pointer to array
// a is int (*a)[1]
void indirect_pointer_to_array(int (*a)[1]) {
  // struct S* b = a;  // just how to see what the type of a is
  pointer_to_array(a); // Warning, ok bcause 1 < 3
}

int main() {
  // array a
  int n = 100;
  int a[n];
  a[0] = 42;

  // array b
  int b[2];
  b[0] = 13;
  int *p = b;

  printf("================\n");
  printf("Original arrays:\n");
  printf("================\n");
  printf("array a: %zu %zu\n", sizeof a, sizeof *a);
  printf("array b: %zu %zu\n", sizeof b, sizeof *b);

  printf("============================================\n");
  printf("Array function argument decays into pointer:\n");
  printf("============================================\n");
  pointer(a);
  array(a);
  array_with_size(a);
  array_with_size(b);
  array_with_parameter_size(n, a);
  array_with_parameter_size(n, b);

  printf("============================================\n");
  printf("Array function argument has size constraint:\n");
  printf("============================================\n");
  size_constrained(b);          // Warning (correct, actual size 2 < constrained size 4)
  size_constrained(p);          // No warning, even though p == b
  indirect_size_constrained(b); // No warning...
  pointer_to_array(&a);         // Ok, 100 > 3
  pointer_to_array(&b);         // Warning (correct 2 < 3)
  pointer_to_array(p);          // Warning, ok since p is not an array pointer
  pointer_to_array_n(10, &a);   // size is not checked
  pointer_to_array_n(50, &b);   // size is not checked
  pointer_to_array_n(10, p);    // Warning, ok since p is not a variable array pointer
  indirect_pointer_to_array(b);
}
