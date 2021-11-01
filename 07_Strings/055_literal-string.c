// Unfortunately C considers literal strings non-const when it type-checks.
// Just because C does this doesn’t mean that you should. Declare them const!

#include <stdio.h>

int
main()
{
  // This would be the correct definition (as you should): char const* string = "foo";
  char* string = "foo";
  string[1] = 'O';                                  // Changing foo to fOo?
  printf("Success! string is now '%s'\n", string);  // won't happen - segmentation fault!
}
