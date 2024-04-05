// Unfortunately C considers literal strings non-const when it type-checks.
// Just because C does this doesn't mean that you should. Declare them const!
// Literal strings will be placed in the read-only text section of the executable.

#include <stdio.h>

int main() {
  // This would be the correct definition (as you should!): `char const* string = "foo";`
  char *string = "foo";
  printf("Next code line will segfault!\n");
  string[1] = 'O';                                 // changing foo to fOo?
  printf("Success! string is now '%s'\n", string); // won't happen - segmentation fault!
}
