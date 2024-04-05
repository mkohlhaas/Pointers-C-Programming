#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 20

bool validate_password(char const *password) {
  char buffer[BUFFER_SIZE];

  printf("Password: ");
  fgets(buffer, BUFFER_SIZE, stdin); // fgets actually reads max. BUFFER_SIZE - 1 chars

  buffer[strcspn(buffer, "\n")] = 0; // remove newline; strcspn = length of buffer till newline
  if (!strcmp(password, buffer))
    return true; // !strcmp => both strings are the same (== 0)
  else
    return false;
}

int main() {
  if (validate_password("foobar")) {
    printf("The password is VALID!\n");
  } else {
    printf("The password is INVALID!\n");
    return 1;
  }
}
