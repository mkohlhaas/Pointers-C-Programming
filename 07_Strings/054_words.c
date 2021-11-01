#include <ctype.h>
#include <stdio.h>

// Return type is char* - not char const* - b/c a user might
// want to modify the string they search for.
char*
skip_word(char* x)
{
  while (*x &&  isalpha(*x)) x++;
  return x;
}

char*
find_word(char* x)
{
  while (*x && !isalpha(*x)) x++;
  return x;
}

int
main()
{
  char  const* words = "asdlfkj 34 2345 2345 sdfk kj 230 asdo 13 aoanq 2424 eqycp";
  char*        x     = find_word((char*)words);

  {
    printf("\033[38;5;206m");
    printf("=========== Original string ==================================\n");
    printf("\033[0m");
    printf("%s\n", words);
  }

  {
    printf("\033[38;5;206m");
    printf("=========== Words ============================================\n");
    printf("\033[0m");

    while (*x) {
      printf("%s\n", x);
      x = find_word(skip_word(x));
    }
  }
}
