// circular doubly linked list with dummy header

#pragma once

#include <stdbool.h>
#include <stdlib.h>
typedef struct link Link;

typedef struct link
{
  int   value;
  Link* prev;
  Link* next;
} Link;

typedef Link ListHead;

#define init_list_head(x)                                                      \
  (ListHead) { .prev = &(x), .next = &(x) }

#define front(x)    (x)->next
#define last(x)     (x)->prev
#define is_empty(x) ((x) == front(x))

// Doesn't free, links, but set the list to empty
#define clear_head(head)                                                       \
  do {                                                                         \
    (head) = init_list_head(head);                                             \
  } while (0)

// Doesn't free, links, but set the list to empty
#define clear_list(x) clear_head(*(x))

typedef ListHead* List;
List  new_list(void);
Link* new_link(int val, Link* prev, Link* next);
void  free_links(ListHead* head);

#define free_list(x)                                                           \
  do {                                                                         \
    free_links(x);                                                             \
    free(x);                                                                   \
    x = 0;                                                                     \
  } while (0)

// Connect x and y so x's next is y and y's prev is x
static inline void
connect(Link* x, Link* y)
{
  x->next = y;
  y->prev = x;
}

// If x already has the right next and prev, make those
// point to x
static inline void
connect_neighbours(Link* x)
{
  x->next->prev = x;
  x->prev->next = (x);
}

// Make y's prev and next point to x and x->next,
// then connect it in so its neighbours match
static inline void
link_after(Link* x, Link* y)
{
  y->prev = x;
  y->next = x->next;
  connect_neighbours(y);
}

#define link_before(x, y) link_after((x)->prev, y)

// Remove x from the list, but leave its
// pointers so we still have access to its
// neighbours if we need them
static inline void
unlink(Link* x)
{
  x->next->prev = x->prev;
  x->prev->next = x->next;
}

static inline void
delete_link(Link* x)
{
  unlink(x);
  free(x);
}

bool insert_val_after(Link* after, int val);
#define insert_val_before(before, val) insert_val_after((before)->prev, val)

#define prepend_link link_after
#define append_link link_before
#define prepend insert_val_after
#define append insert_val_before

List make_list(int n, int array[n]);
void print_list(List x);
bool contains(List x, int val);
void concatenate(List x, List y);
void delete_value(List x, int val);
void reverse(List x);
List copy_list(List x);
bool equal(List x, List y);
