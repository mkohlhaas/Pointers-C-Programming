#include "list.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

List *new_list(ListType type) {
  List *list = malloc(sizeof *list);
  if (list)
    *list = (List){.head = {.next = &list->head, .prev = &list->head}, .type = type};
  return list;
}

void free_list(List *x) {
  void (*free_link)(Link *) = x->type.free;
  // We can only free if we have a free function.
  // Otherwise, assume that we shouldn't free.
  if (free_link) {
    Link *lnk = front(x);
    while (lnk != head(x)) {
      Link *next = lnk->next;
      free_link(lnk);
      lnk = next;
    }
  }
  free(x);
}

// Default print function
static void print_link(Link *lnk) { printf("<link %p>", (void *)lnk); }

void print_list(List *x) {
  void (*print)(Link *) = (x->type.print) ? x->type.print : print_link;
  printf("[ ");
  for (Link *lnk = front(x); lnk != head(x); lnk = lnk->next) {
    print(lnk);
    putchar(' ');
  }
  printf("]\n");
}

Link *find_link(List *x, Link *from, bool (*p)(Link *)) {
  for (Link *lnk = from; lnk != head(x); lnk = lnk->next)
    if (p(lnk))
      return lnk;
  return NULL;
}

void delete_if(List *x, bool (*p)(Link *)) {
  void (*free)(Link *) = x->type.free;
  Link *lnk = front(x);
  while (lnk != head(x)) {
    Link *next = lnk->next;
    if (p(lnk)) {
      unlink(lnk);
      if (free)
        free(lnk);
    }
    lnk = next;
  }
}
