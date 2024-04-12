#include <ctype.h>
#include <stdio.h>

// Return type is char* - not char const* - b/c a user might
// want to modify the string they search for.
char *
skip_word (char *x)
{
  while (*x && isalpha (*x))
    {
      x++;
    }
  return x;
}

// Returns pointer to next word.
char *
find_word (char *x)
{
  while (*x && !isalpha (*x))
    {
      x++;
    }
  return x;
}

int
main ()
{
  char const *words = "the only thing we have to fear is fear itself";

  {
    printf ("----------- original string -------------------\n");
    printf ("%s\n\n", words);
  }

  {
    printf ("---------------- words ------------------------\n");

    char *x = find_word ((char *)words); // find first word
    while (*x)
      {
        printf ("%s\n", x);
        x = find_word (skip_word (x));
      }
  }
}
