#include <ctype.h>
#include <stdio.h>

// Base Higher-Order Functions
char*
find(int (*char_class)(int), char* x)
{
  while (*x && !char_class(*x)) x++;
  return x;
}

char*
skip(int (*char_class)(int), char* x)
{
  while (*x && char_class(*x)) x++;
  return x;
}

// Iterator
typedef struct
{
  char*  x;  // acts like some sort of cursor into the string to be searched
  int  (*char_class)(int);
} FindSkipIter;

// Iterator functions
#define NULLIFY(x) ((*x) ? x : NULL)

char*
init_iter(FindSkipIter* iter, char* x, int (*char_class)(int))
{
  iter->char_class = char_class;
  iter->x          = find(iter->char_class, x);
  return NULLIFY(iter->x);
}

char*
iter_next(FindSkipIter* iter)
{
  iter->x = skip(iter->char_class, iter->x);
  iter->x = find(iter->char_class, iter->x);
  return NULLIFY(iter->x);
}

#define init_word_iter(itr, x) init_iter((itr), (x), isalpha)
#define init_int_iter(itr, x)  init_iter((itr), (x), isdigit)

int
main()
{
  FindSkipIter iter;
  char* x = "123 sss 321 xxx 314";

  printf("Original string:\n");
  printf("----------------\n");
  puts(x);

  printf("\nWord iteration:\n");
  printf("---------------\n");
  for (char* y = init_word_iter(&iter, x);
             y;
             y = iter_next(&iter)) {
    printf("%s\n", y);
  }

  printf("\nInteger iteration:\n");
  printf("------------------\n");
  for (char* y = init_int_iter(&iter, x);
             y;
             y = iter_next(&iter)) {
    printf("%s\n", y);
  }
}
