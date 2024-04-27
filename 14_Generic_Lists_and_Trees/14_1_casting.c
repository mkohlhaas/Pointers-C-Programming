#include "list.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct int_link
{
  link link;
  int  value;
} int_link;

int_link *
new_int_link (int value)
{
  int_link *lnk = malloc (sizeof *lnk);
  if (lnk)
    {
      lnk->value = value;
    }
  return lnk;
}

void
print_int_link (link *lnk)
{
  printf ("%d", ((int_link *)lnk)->value);
}

void
free_int_link (link *lnk)
{
  free (lnk);
}

bool
is_even (link *lnk)
{
  int_link *link = (int_link *)lnk;
  return link->value % 2 == 0;
}

int
main ()
{
  printf ("Create integer list from 0 to 9:\n");
  printf ("--------------------------------\n");
  list_api int_list = {
    .free  = free_int_link,
    .print = print_int_link,
  };
  list *x = new_list (int_list);

  for (int i = 0; i < 10; i++)
    {
      int_link *lnk = new_int_link (i);
      if (!lnk)
        {
          abort ();
        }
      append (x, (link *)lnk);
    }
  print_list (x);

  printf ("\nFind first two even list members:\n");
  printf ("---------------------------------\n");
  int_link *lnk = (int_link *)find_link (x, front (x), is_even);
  printf ("%d ", lnk->value);
  lnk = (int_link *)find_link (x, lnk->link.next, is_even);
  printf ("%d\n", lnk->value);

  printf ("\nFind all even list members:\n");
  printf ("---------------------------\n");
  for (link *lnk = find_link (x, front (x), is_even); lnk; lnk = find_link (x, lnk->next, is_even))
    {
      printf ("%d ", ((int_link *)lnk)->value);
    }

  printf ("\n\nDelete all even list members:\n");
  printf ("-----------------------------\n");
  delete_if (x, is_even);
  print_list (x);
  free_list (x);

  printf ("\nCreate a new list from stack-allocated links:\n");
  printf ("---------------------------------------------\n");
  int_link l1 = {
    .value = 13,
  };
  int_link l2 = {
    .value = 42,
  };
  list_api type = {
    .print = print_int_link,
    .free  = NULL,
  };
  x = new_list (type);
  append (x, (link *)&l1);
  append (x, (link *)&l2);
  print_list (x);
  free_list (x);
}
