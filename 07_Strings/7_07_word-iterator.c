// iterator pattern: first, next, stop, (cleanup) functions

#include <ctype.h>
#include <stdio.h>

// turns a pointer to `0` into a NULL pointer
#define NULLIFY(x) ((*x) ? x : NULL)

// -- Words ----------------------------------------------------------------------------------------------- //

char *
skip_word (char *x)
{
  while (*x && isalpha (*x))
    {
      x++;
    }
  return x;
}

char *
find_word (char *x)
{
  while (*x && !isalpha (*x))
    {
      x++;
    }
  return x;
}

// == Word Iterato - first, next, stopr

char *
first_word (char *x)
{
  return NULLIFY (find_word (x));
}
char *
next_word (char *x)
{
  return NULLIFY (find_word (skip_word (x)));
}
#define WORD_ITER_END NULL

// -- Integers -------------------------------------------------------------------------------------------- //

char *
skip_integer (char *x)
{
  while (*x && isdigit (*x))
    {
      x++;
    }
  return x;
}

char *
find_integer (char *x)
{
  while (*x && !isdigit (*x))
    {
      x++;
    }
  return x;
}

// -- Integer Iterator - first, next, stop

char *
first_integer (char *x)
{
  return NULLIFY (find_integer (x));
}
char *
next_integer (char *x)
{
  return NULLIFY (find_integer (skip_integer (x)));
}
#define INTEGER_ITER_END NULL

// -- Main ------------------------------------------------------------------------------------------------ //

int
main ()
{
  char *words = "the 34 only 24 thing we 134 have to 76 fear is fear 666 itself";

  {
    printf ("----------- Original string ----------------------------------\n");
    printf ("%s\n", words);
  }

  {
    printf ("\n----------- Words --------------------------------------------\n");
    for (char *iter = first_word (words); iter != WORD_ITER_END; iter = next_word (iter))
      {
        printf ("%s\n", iter);
      }
  }

  {
    printf ("\n----------- Integers -----------------------------------------\n");
    for (char *iter = first_integer (words); iter != INTEGER_ITER_END; iter = next_integer (iter))
      {
        printf ("%s\n", iter);
      }
  }
}
