#include <ctype.h>
#include <stdio.h>

// e.g. isalpha, isdigit, …
typedef int (*char_class) (int);

// -------------------------------------------------------------------- //

char *
find (char_class char_class, char *x)
{
  while (*x && !char_class (*x))
    {
      x++;
    }
  return x;
}

char *
skip (char_class char_class, char *x)
{
  while (*x && char_class (*x))
    {
      x++;
    }
  return x;
}

// -------------------------------------------------------------------- //

typedef struct
{
  char      *x; // points to next char in iteration
  char_class char_class;
} iter;

// Returns NULL pointer if at end of string.
// x is char*: if (*x == `\0`) → NULL
#define NULLIFY(x) ((*x) ? x : NULL)

char *
init_iter (iter *iter, char *x, char_class char_class)
{
  iter->x          = find (char_class, x);
  iter->char_class = char_class;
  return NULLIFY (iter->x);
}

char *
iter_next (iter *iter)
{
  iter->x = skip (iter->char_class, iter->x);
  iter->x = find (iter->char_class, iter->x);
  return NULLIFY (iter->x);
}

#define init_word_iter(itr, x) init_iter ((itr), (x), isalpha)
#define init_int_iter(itr, x)  init_iter ((itr), (x), isdigit)

// -------------------------------------------------------------------- //

int
main ()
{
  iter  iter;
  char *x = "123 sss 321 xxx 314";

  puts ("Original string:");
  puts ("----------------");
  puts (x);

  puts ("\nWord iteration:");
  puts ("---------------");
  for (char *y = init_word_iter (&iter, x); y; y = iter_next (&iter))
    {
      puts (y);
    }

  puts ("\nInteger iteration:");
  puts ("------------------");
  for (char *y = init_int_iter (&iter, x); y; y = iter_next (&iter))
    {
      puts (y);
    }
}
