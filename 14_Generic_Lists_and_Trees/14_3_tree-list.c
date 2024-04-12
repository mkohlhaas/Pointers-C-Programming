#include "list.h"
#include "stree.h"
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// returns Ostring*
#define container(p, type, member) ((type *)((char *)p - offsetof (type, member)))

typedef struct ordered_string
{
  node        node; // tree-like
  link        link; // list-like
  char const *str;
} OString;

void
print_ordered_string (OString *str)
{
  printf ("\"%s\"", str->str);
}

void
free_ordered_string (OString *str)
{

  unlink (&str->link); // Remove from data structures...
  remove_node (&str->node);
  free (str);          // and then free...
}

// ==============================================
// =============== Tree interface ===============
// ==============================================
void const *
strnode_key (node *n)
{
  return container (n, OString, node)->str;
}

int
strnode_cmp (void const *x, void const *y)
{
  return strcmp (x, y);
}

void
strnode_print (node *n)
{
  print_ordered_string (container (n, OString, node));
}

void
strnode_free (node *n)
{
  free_ordered_string (container (n, OString, node));
}

stree_api strnode_type = {
  .key   = strnode_key,
  .cmp   = strnode_cmp,
  .print = strnode_print,
  .free  = strnode_free,
};

// ==============================================
// =============== List interface ===============
// ==============================================
void
strlink_print (link *lnk)
{
  print_ordered_string (container (lnk, OString, link));
}

void
strlink_free (link *lnk)
{
  free_ordered_string (container (lnk, OString, link));
}

list_api strlink_type = {
  .print = strlink_print,
  .free  = strlink_free,
};

// ==============================================

OString *
new_ostring (char const *str)
{
  OString *n = malloc (sizeof *n);
  if (!n)
    {
      abort ();
    }
  n->str = str;
  return n;
}

typedef struct ordered_strings
{
  stree *map;
  list  *order;
} OrderedStrings;

// Helper Functions for API
link *
take_front (list *x, int idx)
{
  for (link *lnk = front (x); lnk != head (x); lnk = lnk->next)
    {
      if (idx-- == 0)
        {
          return lnk;
        }
    }
  return NULL;
}

link *
take_back (list *x, int idx)
{
  for (link *lnk = back (x); lnk != head (x); lnk = lnk->prev)
    {
      if (idx-- == 0)
        {
          return lnk;
        }
    }
  return NULL;
}

// ===================================
// =============== API ===============
// ===================================
OrderedStrings *
new_ordered_strings ()
{
  OrderedStrings *os = malloc (sizeof *os);
  if (!os)
    {
      abort (); // handle alloc errors
    }
  os->map   = new_tree (strnode_type);
  os->order = new_list (strlink_type);
  if (!os->map || !os->order)
    {
      abort ();
    }
  return os;
}

void
add_string (OrderedStrings *os, char const *str)
{
  OString *ostr = new_ostring (str);
  if (!ostr)
    {
      abort ();
    }
  insert_node (os->map, &ostr->node);
  append (os->order, &ostr->link);
}

void
remove_string (OrderedStrings *os, char const *str)
{
  node *n = find_node (os->map, str);
  if (n)
    {
      OString *x = container (n, OString, node);
      free_ordered_string (x);
    }
}

void
remove_index (OrderedStrings *os, int idx)
{
  link *lnk;
  if (idx < 0)
    {
      lnk = take_back (os->order, -idx - 1);
    }
  else
    {
      lnk = take_front (os->order, idx);
    }
  if (!lnk)
    {
      return; // report an error...
    }
  OString *x = container (lnk, OString, link);
  free_ordered_string (x);
}

void
free_ordered_strings (OrderedStrings *os)
{
  // Freeing through the tree, and unlinking through
  // the links, is linear time. The other way requires
  // searching for rightmost, and would be O(n log n).
  // So free the tree first, then the (now empty) list.
  free_tree (os->map);
  free_list (os->order);
  free (os);
}

// ====================================
// =============== Main ===============
// ====================================
int
main ()
{
  OrderedStrings *os = new_ordered_strings ();

  add_string (os, "foo");
  add_string (os, "bar");
  add_string (os, "baz");
  add_string (os, "qux");
  add_string (os, "qax");

  printf ("Original list:\n");
  printf ("--------------\n");
  print_list (os->order);
  print_tree (os->map);

  printf ("\n\nRemoving 'bar':\n");
  printf ("---------------\n");
  remove_string (os, "bar");
  print_list (os->order);
  print_tree (os->map);

  printf ("\n\nRemoving index 1 (baz):\n");
  printf ("-----------------------\n");
  remove_index (os, 1);
  print_list (os->order);
  print_tree (os->map);

  printf ("\n\nRemoving index -3 (foo):\n");
  printf ("------------------------\n");
  remove_index (os, -3);
  print_list (os->order);
  print_tree (os->map);

  printf ("\n\nAll done!\n");
  free_ordered_strings (os);
}
