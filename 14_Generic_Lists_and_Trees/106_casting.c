#include "list.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct int_link
{
  Link     link;
  int      value;
} IntLink;

IntLink*
new_int_link(int value)
{
  IntLink* lnk = malloc(sizeof *lnk);
  if (lnk) lnk->value = value;
  return lnk;
}

// all the functions take a Link* and cast it to an IntLink* in the function
void
print_int_link(Link* lnk)
{
  // cast generic link to integer link
  printf("%d", ((IntLink*)lnk)->value);
}

void
free_int_link(Link* lnk)
{
  free(lnk);
}

ListType int_list = {
  .free  =  free_int_link,
  .print = print_int_link,
};

bool
is_even(Link* l)
{
  IntLink* link = (IntLink*)l;
  return link->value % 2 == 0;
}

int
main()
{
  printf("Create integer list from 0 to 9:\n");
  printf("--------------------------------\n");
  List* x = new_list(int_list);
  for (int i = 0; i < 10; i++) {
    IntLink* lnk = new_int_link(i);
    if (!lnk) abort();
    // to use generic list functions (e.g. append) cast integer link to generic link
    // the generic link is the first entry in the IntLink struct
    // cp. with virtual tables from chapter 13
    append(x, (Link*)lnk);
  }
  print_list(x);

  printf("\nFind first two even list members:\n");
  printf("---------------------------------\n");
  // cast Link to IntLink
  IntLink* lnk = (IntLink*)find_link(x, front(x), is_even);
  printf("%d\n", lnk->value);
  lnk = (IntLink*)find_link(x, lnk->link.next, is_even);
  printf("%d\n", lnk->value);

  printf("\nFind all even list members:\n");
  printf("---------------------------\n");
  for (Link* lnk = find_link(x, front(x), is_even);
             lnk;
             lnk = find_link(x, lnk->next, is_even)) {
    // cast Link to IntLink
    printf("%d ", ((IntLink*)lnk)->value);
  }
  printf("\n");

  printf("\nDelete all even list members:\n");
  printf("-----------------------------\n");
  delete_if(x, is_even);
  print_list(x);
  free_list(x);

  // using stack-allocated links
  printf("\nCreate a new list from stack-allocated links:\n");
  printf("---------------------------------------------\n");
  IntLink l1 = { .value = 13 };
  IntLink l2 = { .value = 42 };
  ListType type = {
    .print = print_int_link,
    .free  = NULL // Do not free stack allocated links!!!
  };
  x = new_list(type);
  append(x, (Link*)&l1);
  append(x, (Link*)&l2);
  print_list(x);
  free_list(x);
}
