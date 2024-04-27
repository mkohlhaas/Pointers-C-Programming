#include "list.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define container(p, type, member) (type *)((char *)p - offsetof (type, member))

typedef struct double_link
{
  link forward_link;
  link backward_link;
  int  value;
} double_link;

double_link *
new_double_link (int value)
{
  double_link *link = malloc (sizeof *link);
  if (link)
    {
      link->value = value;
    }
  return link;
}

void
print_double_link (double_link *link)
{
  printf ("%d", link->value);
}

void
print_forward (link *link)
{
  print_double_link (container (link, double_link, forward_link));
}

void
print_backward (link *link)
{
  print_double_link (container (link, double_link, backward_link));
}

static void
free_double_link (double_link *link)
{
  unlink (&link->forward_link);
  unlink (&link->backward_link);
  free (link);
}

void
free_forward (link *link)
{
  free_double_link (container (link, double_link, forward_link));
}

void
free_backward (link *link)
{
  free_double_link (container (link, double_link, backward_link));
}

bool
is_forward_even (link *l)
{
  double_link *link = container (l, double_link, forward_link);
  return link->value % 2 == 0;
}

int
main ()
{
  printf ("Creating two lists - a forward and a backward type list.\n");
  list_api forward_type = {
    .free  = free_forward,
    .print = print_forward,
  };

  list_api backward_type = {
    .free  = free_backward,
    .print = print_backward,
  };
  list *forward  = new_list (forward_type);
  list *backward = new_list (backward_type);
  if (!forward || !backward)
    {
      abort ();
    }

  for (int i = 0; i < 10; i++)
    {
      double_link *link = new_double_link (i);
      if (!link)
        {
          abort ();
        }
      // same link in both lists (shared links)
      append (forward, &link->forward_link);
      prepend (backward, &link->backward_link);
    }

  printf ("\nPrint forward type list:\n");
  printf ("------------------------\n");
  print_list (forward);

  printf ("\nPrint backward type list:\n");
  printf ("-------------------------\n");
  print_list (backward);

  printf ("\nReplacing 0 with 42.\n");
  double_link *link = container (front (forward), double_link, forward_link);
  link->value       = 42;
  printf ("Both lists have changed because the links are shared.\n");
  printf ("\nPrint forward type list:\n");
  printf ("------------------------\n");
  print_list (forward);

  printf ("\nPrint backward type list:\n");
  printf ("-------------------------\n");
  print_list (backward);

  printf ("\nDelete all even numbers.\n");
  delete_if (forward, is_forward_even);
  printf ("Print forward type list:\n");
  printf ("------------------------\n");
  print_list (forward);

  printf ("\nPrint backward type list:\n");
  printf ("-------------------------\n");
  print_list (backward);

  printf ("\nBoth lists are empty after freeing one of them:\n");
  printf ("-----------------------------------------------\n");
  free_list (forward);
  print_list (forward);
  print_list (backward);
  free_list (backward);
}
