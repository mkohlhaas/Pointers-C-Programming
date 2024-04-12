#pragma once

#define SI static inline /* for link operations */

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// List is implemented as a circular structure with a dummy link at the beginning where dummy->next points to first
// real link and prev to last real link of list. Last link's next points to dummy. An empty List consists of a dummy
// link where next and prev point to itself, the dummy. Head is synonym for dummy. The dummy link is the head. List is
// just the address of the head/dummy. Address of head/dummy stays the same all the time!

// Showing next pointers:
// List → Head/Dummy → link → link → ... → link
//        ↑                                  |
//        |----------------------------------|
//
// Showing previous pointers:
// List → Head/Dummy ← link ← link ← ... ← link
//        |                                   ↑
//        |-----------------------------------|
//
// Empty list:
// Showing next pointer:
// List → Head/Dummy
//        ↑        |
//        |--------|
//
// Showing previous pointer:
// List → Head/Dummy
//        |        ↑
//        |--------|

typedef struct link
{
  int          value;
  struct link *prev;
  struct link *next;
} Link;

typedef Link      ListHead; // ListHead is a link, the dummy link.
typedef ListHead *List;     // List is a pointer to the head/dummy link.

Link   *new_link (int val, Link *prev, Link *next);
List    new_list ();
List    make_list_from_array (int n, int array[n]);
bool    insert_val_after (Link *after, int val);
void    free_links (List head);
void    print_list (List x);
bool    contains (List x, int val);
void    concatenate (List x, List y);
void    delete_value (List x, int val);
void    reverse (List x);
List    copy_list (List x);
bool    equal (List x, List y);
SI void connect (Link *x, Link *y);
SI void connect_neighbours (Link *x);
SI void link_after (Link *x, Link *y);
SI void unlink (Link *x);
SI void delete_link (Link *x);

#define init_list_head(lnk)                                                                                            \
  (ListHead) { .prev = &(lnk), .next = &(lnk) }

#define free_list(lst)                                                                                                 \
  ({                                                                                                                   \
    free_links (lst);                                                                                                  \
    free (lst);                                                                                                        \
    lst = NULL;                                                                                                        \
  })

#define front(lst)                     (lst)->next
#define last(lst)                      (lst)->prev
#define is_empty(lst)                  ((lst) == front (lst))
#define clear_head(head)               ({ (head) = init_list_head (head); })
#define clear_list(lst)                clear_head (*(lst))
#define insert_val_before(before, val) insert_val_after ((before)->prev, val)
#define prepend                        insert_val_after
#define append                         insert_val_before
#define link_before(x, y)              link_after ((x)->prev, y)
#define prepend_link                   link_after
#define append_link                    link_before
#define link                           connect

// -------------------- Link operations ------------------------------------------------------------

// connect links x and y
SI void
connect (Link *x, Link *y)
{
  x->next = y;
  y->prev = x;
}

// Removes link `x` from list, but leave its pointers so we still have access to its
// old neighbours in the list if we need them.
SI void
unlink (Link *x)
{
  x->next->prev = x->prev;
  x->prev->next = x->next;
}

SI void
delete_link (Link *x)
{
  unlink (x);
  free (x);
}

// Make x consistent with its neighbors.
SI void
connect_neighbours (Link *x)
{
  x->next->prev = x;
  x->prev->next = x;
}

// Place link `y` after link `x`.
SI void
link_after (Link *x, Link *y)
{
  y->prev = x;
  y->next = x->next;
  connect_neighbours (y);
}
