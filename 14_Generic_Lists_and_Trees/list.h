#pragma once

#include <stdbool.h>
#include <stdlib.h>
typedef struct link Link;

typedef struct link {
  Link *prev;
  Link *next;
} Link;

static inline void connect(Link *x, Link *y) {
  x->next = y;
  y->prev = x;
}

static inline void connect_neighbours(Link *x) {
  x->next->prev = x;
  x->prev->next = x;
}

// insert y after x
static inline void link_after(Link *x, Link *y) {
  y->prev = x;
  y->next = x->next;
  connect_neighbours(y);
}

// insert y before x
#define link_before(x, y) link_after((x)->prev, y)

// This time, unlink will set x's pointers to NULL.
// We don't want to risk the callback function modifying
// the list after the link is removed.
static inline void unlink(Link *x) {
  if (!x->prev || !x->next)
    return;
  x->next->prev = x->prev;
  x->prev->next = x->next;
  x->prev = NULL;
  x->next = NULL;
}

typedef struct list_type {
  // similar to virtual function table (vtbl)
  void (*free)(Link *);
  void (*print)(Link *);
} ListType;

typedef struct list {
  Link head;     // similar to object variables
  ListType type; // vtbl is part of a list
} List;

// x ix list*
// returns link*
#define head(x) (&(x)->head)
#define front(x) (head(x)->next)
// we are using circular lists
#define back(x) (head(x)->prev)
#define is_empty(x) (head(x) == front(x))

#define append(x, link) link_before(head(x), link)
#define prepend(x, link) link_after(head(x), link)

List *new_list(ListType type);
void free_list(List *x);
void print_list(List *x);
Link *find_link(List *x, Link *from, bool (*p)(Link *));
void delete_if(List *x, bool (*p)(Link *));
