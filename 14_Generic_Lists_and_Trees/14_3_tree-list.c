#include "list.h"
#include "stree.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define container(p, type, member) ((type *)((char *)p - offsetof (type, member)))

typedef struct ordered_string
{
  node        node; // tree-like
  link        link; // list-like
  char const *str;
} ordered_string;

void
print_ordered_string (ordered_string *ostr)
{
  printf ("\"%s\"", ostr->str);
}

void
free_ordered_string (ordered_string *ostr)
{

  unlink (&ostr->link);
  remove_node (&ostr->node);
  free (ostr);
}

// --------------- stree_api ----------------------------------------------------- //

void const *
strnode_key (node *n)
{
  return container (n, ordered_string, node)->str;
}

int
strnode_cmp (void const *x, void const *y)
{
  return strcmp (x, y);
}

void
strnode_print (node *n)
{
  print_ordered_string (container (n, ordered_string, node));
}

void
strnode_free (node *n)
{
  free_ordered_string (container (n, ordered_string, node));
}

stree_api strnode_api = {
  .key   = strnode_key,
  .cmp   = strnode_cmp,
  .print = strnode_print,
  .free  = strnode_free,
};

// --------------- list_api ------------------------------------------------------ //

void
strlink_print (link *lnk)
{
  print_ordered_string (container (lnk, ordered_string, link));
}

void
strlink_free (link *lnk)
{
  free_ordered_string (container (lnk, ordered_string, link));
}

list_api strlink_api = {
  .print = strlink_print,
  .free  = strlink_free,
};

// --------------- ordered string ------------------------------------------------ //

ordered_string *
new_ordered_string (char const *str)
{
  ordered_string *n = malloc (sizeof *n);
  if (!n)
    {
      abort ();
    }
  n->str = str;
  return n;
}

link *
take_front (list *lst, int idx)
{
  for (link *lnk = front (lst); lnk != head (lst); lnk = lnk->next)
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

// --------------- ordered strings ----------------------------------------------- //

typedef struct ordered_strings
{
  stree *map;
  list  *order;
} ordered_strings;

ordered_strings *
new_ordered_strings ()
{
  ordered_strings *os = malloc (sizeof *os);
  if (!os)
    {
      abort ();
    }
  os->map   = new_tree (strnode_api);
  os->order = new_list (strlink_api);
  if (!os->map || !os->order)
    {
      abort ();
    }
  return os;
}

void
add_string (ordered_strings *oss, char const *str)
{
  ordered_string *ostr = new_ordered_string (str);
  if (!ostr)
    {
      abort ();
    }
  insert_node (oss->map, &ostr->node);
  append (oss->order, &ostr->link);
}

void
remove_string (ordered_strings *oss, char const *str)
{
  node *n = find_node (oss->map, str);
  if (n)
    {
      ordered_string *x = container (n, ordered_string, node);
      free_ordered_string (x);
    }
}

void
remove_index (ordered_strings *oss, int idx)
{
  link *lnk;
  if (idx < 0)
    {
      lnk = take_back (oss->order, -idx - 1);
    }
  else
    {
      lnk = take_front (oss->order, idx);
    }
  if (!lnk)
    {
      return; // TODO: report an error …
    }
  ordered_string *x = container (lnk, ordered_string, link);
  free_ordered_string (x);
}

void
free_ordered_strings (ordered_strings *oss)
{
  // Freeing through the tree, and unlinking through
  // the links, is linear time. The other way requires
  // searching for rightmost, and would be O(n log n).
  // So free the tree first, then the (now empty) list.
  free_tree (oss->map);
  free_list (oss->order);
  free (oss);
}

// --------------- Main ---------------------------------------------------------- //

int
main ()
{
  ordered_strings *oss = new_ordered_strings ();

  add_string (oss, "foo");
  add_string (oss, "bar");
  add_string (oss, "baz");
  add_string (oss, "qux");
  add_string (oss, "qax");

  printf ("Original list:\n");
  printf ("--------------\n");
  print_list (oss->order);
  print_tree (oss->map);

  printf ("\n\nRemoving 'bar':\n");
  printf ("---------------\n");
  remove_string (oss, "bar");
  print_list (oss->order);
  print_tree (oss->map);

  printf ("\n\nRemoving index 1 (baz):\n");
  printf ("-----------------------\n");
  remove_index (oss, 1);
  print_list (oss->order);
  print_tree (oss->map);

  printf ("\n\nRemoving index -3 (foo):\n");
  printf ("------------------------\n");
  remove_index (oss, -3);
  print_list (oss->order);
  print_tree (oss->map);

  printf ("\n\nAll done!\n");
  free_ordered_strings (oss);
}
