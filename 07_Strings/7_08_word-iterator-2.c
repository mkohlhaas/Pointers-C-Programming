#include <ctype.h>
#include <stdio.h>

// NULL pointer instead of pointer to zero terminal
#define NULLIFY(x) ((*x) ? x : NULL)

// -- Macros -------------------------------------------------------------------------------------------------------- //

// create skip and find functions
#define GEN_FIND_SKIP(name, predicate)                                                                                 \
  char *skip_##name (char *x)                                                                                          \
  {                                                                                                                    \
    while (*x && (predicate)(*x))                                                                                      \
      {                                                                                                                \
        x++;                                                                                                           \
      }                                                                                                                \
    return x;                                                                                                          \
  }                                                                                                                    \
  char *find_##name (char *x)                                                                                          \
  {                                                                                                                    \
    while (*x && !(predicate)(*x))                                                                                     \
      {                                                                                                                \
        x++;                                                                                                           \
      }                                                                                                                \
    return x;                                                                                                          \
  }

// create first and next iterators based on skip and find functions
#define GEN_FIND_SKIP_ITER(name, find, skip)                                                                           \
  char *first_##name (char *x) { return NULLIFY ((find)(x)); }                                                         \
  char *next_##name (char *x) { return NULLIFY ((find)((skip)(x))); }

// create everything: find, skip functions and first, next iterators
#define GEN_ITER(name, predicate)                                                                                      \
  GEN_FIND_SKIP (name, predicate)                                                                                      \
  GEN_FIND_SKIP_ITER (name, find_##name, skip_##name)

// -- Macro Calls --------------------------------------------------------------------------------------------------- //

GEN_ITER (word, isalpha)
GEN_ITER (int, isdigit)

// -- Main ---------------------------------------------------------------------------------------------------------- //

int
main ()
{
  char const *words = "the 34 only 24 thing we 134 have to 76 fear is fear 666 itself";

  {
    printf ("----------- Original string ----------------------------------\n");
    printf ("%s\n\n", words);
  }

  {
    printf ("----------- Words --------------------------------------------\n");

    for (char *iter = first_word ((char *)words); iter; iter = next_word (iter))
      {
        printf ("%s\n", iter);
      }
  }

  {
    printf ("\n----------- Integers -----------------------------------------\n");

    for (char *iter = first_int ((char *)words); iter; iter = next_int (iter))
      {
        printf ("%s\n", iter);
      }
  }
}
