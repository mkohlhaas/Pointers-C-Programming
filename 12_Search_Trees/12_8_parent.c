#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct node
{
  int          value;
  struct node *parent;
  struct node *left;
  struct node *right;
} node_t;

typedef node_t *stree;

#define EMPTY NULL

int allocated;

stree
node (int value, stree parent, stree left, stree right)
{
  allocated++;
  stree t = malloc (sizeof *t);
  if (t)
    {
      *t = (node_t){
        .value  = value,
        .parent = parent,
        .left   = left,
        .right  = right,
      };
    }
  return t;
}

#define leaf(value, parent) node (value, parent, EMPTY, EMPTY)

stree *
new_stree ()
{
  stree *t = malloc (sizeof (stree));
  if (t)
    {
      *t = NULL;
    }
  return t;
}

// Finds and returns rightmost node and `parent`.
stree *
rightmost (stree *t, stree *parent)
{
  while ((*t)->right)
    {
      *parent = *t;
      t       = &(*t)->right;
    }
  return t;
}

void
free_nodes (node_t *n)
{
  stree   parent; // just as parameter for rightmost(), otherwise not used
  node_t *curr = n;
  while (curr)
    {
      if (!curr->left)
        {
          node_t *right = curr->right;
          free (curr);
          allocated--;
          curr = right;
        }
      else
        {
          // Since we delete the left links, left will be NULL
          // when we return to node, so we never find curr again
          (*rightmost (&curr->left, &parent))->right = curr;
          // Remove the left link so we don't go down that tree
          // again...
          node_t *left = curr->left;
          curr->left   = EMPTY;
          curr         = left;
        }
    }
}

#define clear_stree(t)                                                                                                 \
  do                                                                                                                   \
    {                                                                                                                  \
      free_nodes (*t);                                                                                                 \
      *t = NULL;                                                                                                       \
    }                                                                                                                  \
  while (0)

#define free_stree(t)                                                                                                  \
  do                                                                                                                   \
    {                                                                                                                  \
      free_nodes (*t);                                                                                                 \
      free (t);                                                                                                        \
      t = NULL;                                                                                                        \
    }                                                                                                                  \
  while (0)

// Returns location (or where it should be) of `val` and its parent in `parent`.
stree *
find_loc (stree *t, int val, node_t **parent)
{
  *parent = EMPTY; // parent of root is NULL pointer
  while (*t && (*t)->value != val)
    {
      *parent = *t;
      if (val < (*t)->value)
        {
          t = &(*t)->left;
        }
      else
        {
          t = &(*t)->right;
        }
    }
  return t;
}

bool
contains (stree *t, int val)
{
  stree parent; // not used but we need it for find_loc()
  return !!*find_loc (t, val, &parent);
}

bool
insert (stree *t, int val)
{
  stree  parent;
  stree *target = find_loc (t, val, &parent);
  if (*target)
    {
      return true; // already existent
    }
  else
    {
      return !!(*target = leaf (val, parent));
    }
}

void delete (stree *t, int val)
{
  stree  parent = EMPTY;
  stree *loc    = find_loc (t, val, &parent);

  if (*loc)
    {
      // we found a (possible) location
      stree t = *loc;
      if (!(t->left && t->right))
        {
          // not both sub-trees are set
          *loc = t->left ? t->left : t->right;
          if (*loc)
            {
              (*loc)->parent = parent;
            }
          free (t);
          allocated--;
        }
      else
        {
          // both sub-trees are set
          parent        = t;
          stree *rm_ref = rightmost (&t->left, &parent);
          stree  rm     = *rm_ref;
          t->value      = rm->value;
          *rm_ref       = rm->left;
          if (*rm_ref)
            {
              (*rm_ref)->parent = parent;
            }
          free (rm);
          allocated--;
        }
    }
}

stree *
make_stree_from_array (int n, int array[n])
{
  stree *t = new_stree ();
  if (!t)
    {
      return NULL;
    }
  for (int i = 0; i < n; i++)
    {
      if (!insert (t, array[i]))
        {
          free_stree (t);
          return NULL;
        }
    }
  return t;
}

#if 1
void
print_stree (stree *t)
{
  if (!*t)
    {
      putchar ('_');
    }
  else
    {
      putchar ('[');
      print_stree (&(*t)->left);
      putchar (',');
      printf ("%d", (*t)->value);
      putchar (',');
      print_stree (&(*t)->right);
      putchar (']');
    }
}
#else
#define indent(I)                                                                                                      \
  do                                                                                                                   \
    {                                                                                                                  \
      for (int i = 0; i < I; i++)                                                                                      \
        putchar (' ');                                                                                                 \
    }                                                                                                                  \
  while (0)
void
print_stree_indent (STree *t, int indent)
{
  if (!*t)
    {
      return;
    }
  indent (indent);
  printf ("[%d] <%p>\n", (*t)->value, (void *)*t);
  print_stree_indent (&(*t)->left, indent + 2);
  print_stree_indent (&(*t)->right, indent + 2);
}
#define print_stree(t) print_stree_indent (t, 0)
#endif

#define left_child(t) ((t)->parent && (t)->parent->left == (t))

void
parent_traverse (stree t)
{
  enum
  {
    DOWN,
    UP
  } state
      = DOWN;

  while (t)
    {
      switch (state)
        {
        case DOWN:
          // Recurse as far left as we can...
          while (t->left)
            {
              putchar ('[');
              t = t->left;
            }
          // Emit the leaf we find there
          printf ("[_,%d,", t->value); // VISIT
          // Then go right, if we can, or up if we can't.
          if (t->right)
            {
              t = t->right;
            }
          else
            {
              printf ("_]");
              state = UP;
            }
          break;

        case UP:
          if (!t->parent)
            {
              return; // we have returned to the root...
            }
          if (left_child (t))
            {
              // Returning from a left child, we emit the parent
              t = t->parent;
              printf (",%d,", t->value); // VISIT
              // Then we go right if we can, or continue up
              // (t is already the parent) if we cannot.
              if (t->right)
                {
                  t     = t->right;
                  state = DOWN;
                }
              else
                {
                  printf ("_]");
                }
            }
          else
            {
              // Returning from a right child just means going up
              putchar (']');
              t = t->parent;
            }
          break;
        }
    }
}

void
check_tree (stree t)
{
  if (!t)
    {
      return;
    }
  if (t->left)
    {
      assert (t->left->parent == t);
    }
  if (t->right)
    {
      assert (t->right->parent == t);
    }
  check_tree (t->left);
  check_tree (t->right);
}

void
parent_free (stree t)
{
  node_t *p;
  enum
  {
    DOWN,
    UP
  } state
      = DOWN;

  while (t)
    {
      switch (state)
        {
        case DOWN:
          while (t->left)
            {
              t = t->left;
            }
          if (t->right)
            {
              t = t->right;
            }
          else
            {
              state = UP;
            }
          break;

        case UP:
          if (!t->parent)
            {
              free (t);
              allocated--;
              return;
            }
          if (left_child (t))
            {
              p = t->parent;
              free (t);
              allocated--;
              t = p;
              if (t->right)
                {
                  t     = t->right;
                  state = DOWN;
                }
            }
          else
            {
              p = t->parent;
              free (t);
              allocated--;
              t = p;
            }
          break;
        }
    }
}

#define free_nodes parent_free

int
main ()
{
  printf ("========== Original ==========================\n");
  stree t = EMPTY;
  insert (&t, 2);
  insert (&t, 3);
  insert (&t, 1);
  insert (&t, 6);
  assert (contains (&t, 2));
  assert (contains (&t, 1));
  assert (contains (&t, 3));
  assert (!contains (&t, 10));
  assert (!contains (&t, 0));
  print_stree (&t);
  putchar ('\n');
  check_tree (t);
  parent_traverse (t);

  printf ("\n========== Inserting 10 and 0 ================\n");
  insert (&t, 10);
  insert (&t, 0);
  check_tree (t);
  assert (contains (&t, 10));
  assert (contains (&t, 0));
  print_stree (&t);
  putchar ('\n');
  parent_traverse (t);
  check_tree (t);

  printf ("\n========== Deleting 12, 6 and 3 ==============\n");
  delete (&t, 12);
  delete (&t, 3);
  delete (&t, 6);
  assert (!contains (&t, 3));
  assert (!contains (&t, 6));
  print_stree (&t);
  putchar ('\n');
  parent_traverse (t);
  check_tree (t);
  clear_stree (&t);

  printf ("\n========== Tree from array ===================\n");
  int    array[] = { 1, 2, 13, 4, 16, 8, 10 };
  int    n       = sizeof array / sizeof *array;
  stree *t2      = make_stree_from_array (n, array);
  print_stree (t2);
  putchar ('\n');
  parent_traverse (*t2);
  free_stree (t2);

  printf ("\n==============================================\n");
  printf ("net allocations = %d\n", allocated);
}
