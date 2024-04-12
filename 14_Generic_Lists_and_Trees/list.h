#pragma once

#include <stdbool.h>
#include <stdlib.h>

typedef struct link
{
  struct link *prev;
  struct link *next;
} link;

typedef struct list_api
{
  void (*free) (link *);
  void (*print) (link *);
} list_api;

typedef struct list
{
  link     head;
  list_api api;
} list;

// connect `x` to `y` (x ↔ y)
static inline void
connect (link *x, link *y)
{
  x->next = y;
  y->prev = x;
}

static inline void
connect_neighbours (link *lnk)
{
  lnk->next->prev = lnk;
  lnk->prev->next = lnk;
}

// insert `y` after `x` (x ↔ y)
static inline void
link_after (link *x, link *y)
{
  y->prev = x;
  y->next = x->next;
  connect_neighbours (y);
}

// insert y before x (y ↔ x)
#define link_before(x, y) link_after ((x)->prev, y)

// This time, unlink will set x's pointers to NULL.
// We don't want to risk the callback function modifying the list after the link is removed.
static inline void
unlink (link *lnk)
{
  if (!lnk->prev || !lnk->next)
    {
      return;
    }
  lnk->next->prev = lnk->prev;
  lnk->prev->next = lnk->next;
  lnk->prev       = NULL;
  lnk->next       = NULL;
}

// `x` is a `list*`; returns `link*`
#define head(x)          (&(x)->head)
#define front(x)         (head (x)->next)
#define back(x)          (head (x)->prev)
#define is_empty(x)      (head (x) == front (x))
#define append(x, link)  link_before (head (x), link)
#define prepend(x, link) link_after (head (x), link)

list *new_list (list_api api);
void  free_list (list *lst);
void  print_list (list *lst);
link *find_link (list *lst, link *from, bool (*pred) (link *));
void  delete_if (list *lst, bool (*pred) (link *));
