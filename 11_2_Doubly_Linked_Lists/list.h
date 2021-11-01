#pragma once

#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

// List is implemented as a circualar structure with a dummy link at the beginning where next of dummy points to first real link and prev to last real link of list.
// Last link's next points to dummy.
// An empty List consists of a dummy link where next and prev point to itself, the dummy.
// Head is synonym for dummy. The dummy link is the head. List is just the address of the head/dummy.
// Address of head/dummy stays the same all the time!

// Showing next pointers:
// List -> head/dummy -> link -> link -> ... -> link
//         ^                                    |
//         |------------------------------------|                                          
//
// Showing previous pointers:
// List -> head/dummy <- link <- link <- ... <- link
//         |                                    ^
//         |------------------------------------|                                          
//
// Empty list:
// Showing next pointer:
// List -> head/dummy
//         ^        |
//         |--------|                                          
//
// Showing previous pointer:
// List -> head/dummy
//         |        ^
//         |--------|                                          

typedef struct link Link;
typedef Link        ListHead;  // ListHead is just a mnemonic to the first link in a list
typedef ListHead*   List;      // List is a pointer to the first link in the list - the head/dummy!

typedef struct link
{
  int    value;
  Link*  prev;
  Link*  next;
} Link;

#define SI static inline  /* for link operations */
   Link* new_link            (int val, Link* prev, Link* next);
   List  new_list            ();
   List  make_list           (int n, int array[n]);
   bool  insert_val_after    (Link* after, int val);
   void  free_links          (List head);
   void  print_list          (List x);
   bool  contains            (List x, int val);
   void  concatenate         (List x, List y);
   void  delete_value        (List x, int val);
   void  reverse             (List x);
   List  copy_list           (List x);
   bool  equal               (List x, List y);
SI void  connect             (Link* x, Link* y);
SI void  connect_neighbours  (Link* x);
SI void  link_after          (Link* x, Link* y);
SI void  unlink              (Link* x);
SI void  delete_link         (Link* x);
#undef SI

#define init_list_head(x)               (ListHead){ .prev = &(x), .next = &(x) }         /* x is a link. Can be used to create an empty List.                                         */
#define front(x)                        (x)->next                                        /* x is a list. front returns the first real data link or the dummy link if list is empty    */
#define last(x)                         (x)->prev                                        /* last returns the last real data link or the dummy link if list is empty                   */
#define is_empty(x)                    ((x) == front(x))
#define clear_head(head)                 do {(head) = init_list_head(head); } while (0)  /* 'head' is a link. Doesn't free links. Create dummy on the stack and copy it into 'head'.  */
#define clear_list(x)                    clear_head(*(x))                                /* same for a List, which is a pointer to a head; x is a List                                */
#define insert_val_before(before, val)   insert_val_after((before)->prev, val)
#define prepend                          insert_val_after
#define append                           insert_val_before                               /* returns bool                                                                              */
#define link_before(x, y)                link_after((x)->prev, y)                        /* place link y before link x                                                                */
#define prepend_link                     link_after
#define append_link                      link_before
#define link                             connect

#define free_list(x)          \
  do {                        \
    free_links(x);            \
    free(x);                  \
    x = NULL;                 \
  } while (0)

// ==================== Link operations ============================================================ 

// connect links x and y
static inline void
connect(Link* x, Link* y)
{
  x->next = y;
  y->prev = x;
}

// removes link x from list, but leave its pointers so we still have access to its
// old neighbours in the list if we need them.
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

// make x consistent with its neighbors
static inline void
connect_neighbours(Link* x)
{
  x->next->prev = x;
  x->prev->next = x;
}

// place link y after link x
static inline void
link_after(Link* x, Link* y)
{
  y->prev = x;
  y->next = x->next;
  connect_neighbours(y);
}
