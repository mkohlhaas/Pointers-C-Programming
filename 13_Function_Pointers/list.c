// circular doubly linked list with dummy header

#include "list.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

Link *new_link(int val, Link *prev, Link *next) {
  Link *link = malloc(sizeof *link);
  if (!link)
    return NULL;

  link->value = val;
  link->prev = prev;
  link->next = next;
  return link;
}

List new_list(void) {
  Link *head = new_link(0, NULL, NULL);
  if (!head)
    return NULL;
  *head = init_list_head(*head);
  return head;
}

void free_links(ListHead *head) {
  Link *link = front(head);
  while (link != head) {
    Link *next = link->next;
    free(link);
    link = next;
  }
  clear_list(head);
}

bool insert_val_after(Link *after, int val) {
  Link *link = new_link(val, after, after->next);
  if (!link)
    return false;
  connect_neighbours(link);
  return true;
}

List make_list(int n, int array[n]) {
  List x = new_list();
  if (!x)
    return NULL;

  // going in the forward direction and appending
  for (int i = 0; i < n; i++) {
    if (!append(x, array[i])) {
      free_list(x);
      return NULL;
    }
  }
  return x;
}

void print_list(List x) {
  printf("[ ");
  for (Link *link = front(x); link != x; link = link->next)
    printf("%d ", link->value);
  printf("]\n");
}

bool contains(List x, int val) {
  for (Link *link = front(x); link != x; link = link->next)
    if (link->value == val)
      return true;
  return false;
}

// We don't delete y, but we empty it.
// The caller must free it if he no longer
// needs it. We could free it here, that
// just changes the API. It is a design
// choice.
void concatenate(List x, List y) {
  connect(last(x), front(y));
  connect(last(y), x);
  clear_list(y);
}

void delete_value(List x, int val) {
  Link *link = front(x);
  while (link != x) {
    Link *next = link->next;
    if (link->value == val)
      delete_link(link);
    link = next;
  }
}

#if 0
#define swap_int(x, y)                                                                                                 \
  do {                                                                                                                 \
    int tmp = x;                                                                                                       \
    x = y;                                                                                                             \
    y = tmp;                                                                                                           \
  } while (0)

void reverse(list x)
{
  link *left = front(x);
  link *right = last(x);
  while (left != right) {
    swap_int(left->value, right->value);
    left = left->next; right = right->prev;
  }
}
#else

#define swap_p(x, y)                                                                                                   \
  do {                                                                                                                 \
    Link *tmp = x;                                                                                                     \
    x = y;                                                                                                             \
    y = tmp;                                                                                                           \
  } while (0)

void reverse(List x) {
  Link *p = x;
  do {
    swap_p(p->prev, p->next);
    p = p->prev;
  } while (p != x);
}

#endif

List copy_list(List x) {
  List res = new_list();
  if (!res)
    return NULL;

  for (Link *p = front(x); p != x; p = p->next) {
    if (!append(res, p->value)) {
      free_list(res);
      return NULL;
    }
  }
  return res;
}

bool equal(List x, List y) {
  Link *p = front(x);
  Link *q = front(y);
  while ((p != x) && (q != y)) {
    if (p->value != q->value)
      return false;
    p = p->next;
    q = q->next;
  }
  return (p == x) && (q == y);
}
