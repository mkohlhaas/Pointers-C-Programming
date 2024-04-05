#include "list.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// append value v to list x, if unsuccesul return NULL
#define abort_append(x, v)                                                                                             \
  do {                                                                                                                 \
    if (!append((x), (v))) {                                                                                           \
      free_list(x);                                                                                                    \
      return NULL;                                                                                                     \
    }                                                                                                                  \
  } while (0)

// for each item var in x do something ...
#define for_each(var, x) for (Link *var = front(x); var != x; var = var->next)

List map(List x, int (*f)(int)) {
  List y = new_list();
  if (!y)
    return NULL;
  for_each(link, x) { abort_append(y, f(link->value)); }
  return y;
}

int add2(int x) { return 2 + x; }

List filter(List x, bool (*p)(int)) {
  List y = new_list();
  if (!y)
    return NULL;
  for_each(link, x) {
    if (p(link->value))
      abort_append(y, link->value);
  }
  return y;
}

bool is_even(int i) { return i % 2 == 0; }

int fold(List x, int (*f)(int, int), int x0) {
  List y = new_list();
  if (!y)
    return x0;
  int res = x0;
  for_each(link, x) { res = f(res, link->value); }
  return res;
}

int add(int x, int y) { return x + y; }

int sum(List x) { return fold(x, add, 0); }

int mul(int x, int y) { return x * y; }

int prod(List x) { return fold(x, mul, 1); }

int main() {
  int a[] = {1, 2, 3, 4, 10, 11, 12, 13, 14};
  int n = sizeof a / sizeof *a;

  printf("Make list from array:\n");
  printf("---------------------\n");
  List x = make_list(n, a);
  print_list(make_list(n, a));

  printf("\nMap list by adding 2:\n");
  printf("---------------------\n");
  List y = map(x, add2);
  print_list(y);

  printf("\nFilter list - keep only even numbers:\n");
  printf("-------------------------------------\n");
  List z = filter(x, is_even);
  print_list(z);

  printf("\nSum and multiply even numbered list:\n");
  printf("--------------------------------------\n");
  printf("Sum:\t%d\nProd:\t%d\n", sum(z), prod(z));

  free_list(x);
  free_list(y);
  free_list(z);
}
