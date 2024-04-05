
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 20

bool validate_password(char const *password) {
  char buffer[BUFFER_SIZE];

  printf("Password: ");
  fgets(buffer, BUFFER_SIZE, stdin); // fgets actually reads max. BUFFER_SIZE - 1 chars

  // Note: use the function `strncpy()` if you want to safely copy strings of unknown length.

  // `strcspn`: The function name stands for "complementary span" because the function searches for characters not found
  // in src, that is the complement of src.
  //
  // Returns the length of the maximum initial segment of the null-terminated byte string pointed to by dest, that
  // consists of only the characters not found in the null-terminated byte string pointed to by src.
  //
  // The behavior is undefined if either dest or src is not a pointer to a null-terminated byte string.
  buffer[strcspn(buffer, "\n")] = 0; // removes newline; strcspn = length of buffer till newline
  if (!strcmp(password, buffer)) {
    return true;
  } else {
    return false;
  }
}

int main() {
  if (validate_password("foobar")) {
    printf("The password is VALID!\n");
  } else {
    printf("The password is INVALID!\n");
    return EXIT_FAILURE;
  }
}
