// iterator pattern: first, next, stop, (cleanup) functions

#include <ctype.h>
#include <stdio.h>

// NULL pointer instead of pointer to zero-terminal
#define NULLIFY(x)       ((*x) ? x : NULL)
#define WORD_ITER_END    NULL
#define INTEGER_ITER_END NULL

// ------------------------------------------------------------------------------
// --------------- Words --------------------------------------------------------
// ------------------------------------------------------------------------------
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

// --------------- Word Iterator ------------------------------------------------
char*
first_word(char* x)
{
  return NULLIFY(find_word(x));
}

char*
next_word(char* x)
{
  return NULLIFY(find_word(skip_word(x)));
}

// ------------------------------------------------------------------------------
// ---------------- Integers ----------------------------------------------------
// ------------------------------------------------------------------------------
char*
skip_integer(char* x)
{
  while (*x &&  isdigit(*x)) x++;
  return x;
}

char*
find_integer(char* x)
{
  while (*x && !isdigit(*x)) x++;
  return x;
}

// --------------- Integer Iterator ---------------------------------------------
char*
first_integer(char* x)
{
  return NULLIFY(find_integer(x));
}

char*
next_integer(char* x)
{
  return NULLIFY(find_integer(skip_integer(x)));
}

// ------------------------------------------------------------------------------
// ---------------- Main --------------------------------------------------------
// ------------------------------------------------------------------------------
int
main()
{
  char const* words = "asdlfkj 34 2345 2345 sdfk kj 230 asdo 13 aoanq 2424 eqycp";

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
    for (char const* iter  = first_word((char*)words);
                     iter != WORD_ITER_END;
                     iter  = next_word((char*)iter))
      printf("%s\n", iter);
  }

  {
    printf("\033[38;5;206m");
    printf("=========== Integers =========================================\n");
    printf("\033[0m");
    for (char const* iter  = first_integer((char*)words);
                     iter != INTEGER_ITER_END;
                     iter  = next_integer((char*)iter))
      printf("%s\n", iter);
  }
}
