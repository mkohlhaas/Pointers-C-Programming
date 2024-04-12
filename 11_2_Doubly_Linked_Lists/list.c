#include "list.h"
#include <stdio.h>

Link *
new_link (int val, Link *prev, Link *next)
{
  Link *pLink = malloc (sizeof *pLink);
  if (!pLink)
    {
      return NULL;
    }
  pLink->value = val;
  pLink->prev  = prev;
  pLink->next  = next;
  return pLink;
}

// create new empty list (consists only of dummy)
List
new_list ()
{
  Link *head;
  head = new_link (314159, NULL, NULL);
  if (!head)
    {
      return NULL;
    }
  *head = init_list_head (*head); // update next, prev pointers; create head on the stack and copy it to heap
#ifndef NDEBUG
  printf ("[DEBUG] head: %p next: %p prev: %p val: %d\n", head, head->next, head->prev, head->value);
#endif
  return head;
}

// Free real links, but not head/dummy; result is an empty list.
// Use free_links() for stack-allocated head.
// Use free_list()  for lists on the heap.
void
free_links (List head)
{
  Link *link = front (head);
  while (link != head)
    {
#ifndef NDEBUG
      printf ("[DEBUG] Freeing link %p, val: %d \n", link, link->value);
#endif
      Link *next = link->next;
      free (link);
      link = next;
    }
  clear_list (head);
}

// Insert new link after `after` with val `val`.
bool
insert_val_after (Link *after, int val)
{
  Link *link = new_link (val, after, after->next);
  if (!link)
    {
      return false;
    }
  connect_neighbours (link);
  return true;
}

List
make_list_from_array (int n, int array[n])
{
  List x = new_list ();
  if (!x)
    {
      return NULL;
    }

  for (int i = 0; i < n; i++)
    {
      if (!append (x, array[i]))
        {
          free_list (x);
          return NULL;
        } // append after head
    }
  return x;
}

void
print_list (List x)
{
  printf ("[ ");
  for (Link *link = front (x); link != x; link = link->next)
    {
      printf ("%d ", link->value);
    }
  printf ("]\n");
}

bool
contains (List x, int val)
{
  for (Link *link = front (x); link != x; link = link->next)
    {
      if (link->value == val)
        {
          return true;
        }
    }
  return false;
}

// We don't delete y, but we empty it.  The caller must free it if he no longer needs it.
// We could free it here, but that just changes the API. It is a design choice.
void
concatenate (List x, List y)
{
  connect (last (x), front (y));
  connect (last (y), x);
  clear_list (y);
}

void
delete_value (List x, int val)
{
  Link *link = front (x);
  while (link != x)
    {
      Link *next = link->next;
      if (link->value == val)
        {
          delete_link (link);
        }
      link = next;
    }
}

#if 0
#define swap_int(x, y)                                                                                                 \
  do                                                                                                                   \
    {                                                                                                                  \
      int tmp;                                                                                                         \
      tmp = x;                                                                                                         \
      x   = y;                                                                                                         \
      y   = tmp;                                                                                                       \
    }                                                                                                                  \
  while (0)

void reverse(list x)
{
  link *left  = front(x);
  link *right = last(x);
  while (left != right) {
    swap_int(left->value, right->value);
    left = left->next; right = right->prev;
  }
}
#else
// swap x and y
#define swap_pointers(x, y)                                                                                            \
  do                                                                                                                   \
    {                                                                                                                  \
      Link *tmp = x;                                                                                                   \
      x         = y;                                                                                                   \
      y         = tmp;                                                                                                 \
    }                                                                                                                  \
  while (0)

void
reverse (List x)
{
  Link *p = x;
  do
    {
      swap_pointers (p->prev, p->next); // exchange prev and next pointers of link p
      p = p->prev;                      // prev is the old next
    }
  while (p != x);
}
#endif

List
copy_list (List x)
{
  List newlist = new_list ();
  if (!newlist)
    {
      return NULL;
    }

  for (Link *p = front (x); p != x; p = p->next)
    {
      if (!append (newlist, p->value))
        {
          free_list (newlist);
          return NULL;
        }
    }

  return newlist;
}

bool
equal (List x, List y)
{
  Link *p = front (x);
  Link *q = front (y);

  while ((p != x) && (q != y))
    {
      if (p->value != q->value)
        {
          return false;
        }
      p = p->next;
      q = q->next;
    }

  return (p == x) && (q == y);
}
