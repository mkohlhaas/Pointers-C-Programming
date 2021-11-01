#include "list.h"
#include <assert.h>
#include <stddef.h> // for offsetof
#include <stdio.h>
#include <stdlib.h>

#define container(p, type, member) (type*)((char*)p - offsetof(type, member))

typedef struct double_link
{
  Link forward_link;
  Link backward_link;
  int  value;
} DLink;

DLink*
new_dlink(int value)
{
  DLink* link = malloc(sizeof *link);
  if (link) link->value = value;
  return link;
}


void
print_dlink(DLink* link)
{
  printf("%d", link->value);
}

void
print_forward(Link* link)
{
  print_dlink(container(link, DLink, forward_link));
}

void
print_backward(Link* link)
{
  print_dlink(container(link, DLink, backward_link));
}

void
free_dlink(DLink* link)
{
  // We have to unlink from both lists before we can safely free the link.
  unlink(&link->forward_link);
  unlink(&link->backward_link);
  free(link);
}

void
free_forward(Link* link)
{
  free_dlink(container(link, DLink, forward_link));
}

void
free_backward(Link* link)
{
  free_dlink(container(link, DLink, backward_link));
}

ListType forward_type  = { .free = free_forward,  .print = print_forward  };
ListType backward_type = { .free = free_backward, .print = print_backward };

bool
is_forward_even(Link* l)
{
  DLink* link = container(l, DLink, forward_link);
  return link->value % 2 == 0;
}

int
main()
{
  printf("Creating two lists - a forward and a backward type list.\n");
  List* forward  = new_list(forward_type);
  List* backward = new_list(backward_type);
  if (!forward || !backward) abort();

  for (int i = 0; i < 10; i++) {
    DLink* link = new_dlink(i);
    if (!link) abort();
    append (forward,  &link->forward_link);
    prepend(backward, &link->backward_link);
  }

  printf("\nPrint forward type list:\n");
  printf("------------------------\n");
  print_list(forward);

  printf("\nPrint backward type list:\n");
  printf("-------------------------\n");
  print_list(backward);

  printf("\nReplacing 0 with 42.\n");
  DLink* link = container(front(forward), DLink, forward_link);
  link->value = 42;
  printf("Both lists have changed because the links are shared.\n");
  printf("\nPrint forward type list:\n");
  printf("------------------------\n");
  print_list(forward);

  printf("\nPrint backward type list:\n");
  printf("-------------------------\n");
  print_list(backward);

  printf("\nDelete all even numbers.\n");
  // removes them from both lists
  delete_if(forward, is_forward_even);
  printf("Print forward type list:\n");
  printf("------------------------\n");
  print_list(forward);

  printf("\nPrint backward type list:\n");
  printf("-------------------------\n");
  print_list(backward);

  printf("\nBoth lists are empty after freeing one of them:\n");
  printf("-----------------------------------------------\n");
  free_list(forward);
  print_list(forward);
  print_list(backward);
  free_list(backward);
}
