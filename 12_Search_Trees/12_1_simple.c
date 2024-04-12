#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// Tree of integers.
typedef struct node
{
  int          value;
  struct node *left;
  struct node *right;
} Node;

typedef Node *stree;

// Creates a new node on the heap and returns a pointer to it (= stree).
stree
node (int value, stree left, stree right)
{
  stree t = malloc (sizeof *t);
  if (t)
    {
      *t = (Node){ .value = value, .left = left, .right = right };
    }
  return t;
}

// Definition of an empty tree and creation of leaf node/stree.
#define EMPTY_TREE  NULL
#define leaf(value) node (value, EMPTY_TREE, EMPTY_TREE)

bool
contains (stree t, int val)
{
  if (!t)
    {
      return false;
    }
  else if (val < t->value)
    {
      return contains (t->left, val); // tail recursion
    }
  else if (val > t->value)
    {
      return contains (t->right, val); // tail recursion
    }
  else
    {                                  // (val == t->value)
      return true;
    }
}

#if 0
// Not tail recursive, and we don't handle allocation errors in `leaf()`.
stree
insert (stree t, int val)
{
  if (!t)
    {
      return leaf (val); // can fail, but we don't handle it
    }
  if (val < t->value)
    {
      t->left = insert (t->left, val);
    }
  else if (val > t->value)
    {
      t->right = insert (t->right, val);
    }
  return t;
}
#else
// Insert node `n` into tree `t`.
static stree
insert_node (stree t, Node *n)
{
  if (!t)
    {
      return n;
    }

  if (n->value == t->value)
    {
      free (n); // cannot have several nodes of the same value; `t` unchanged
    }
  else if (n->value < t->value)
    {
      t->left = insert_node (t->left, n); // not tail recursive
    }
  else if (n->value > t->value)
    {
      t->right = insert_node (t->right, n); // not tail recursive
    }

  return t;
}

// Returns `NULL` on allocation errors in `leaf()`.
// Will return a new `stree` if we have an empty tree and only then.
stree
insert (stree t, int val)
{
  // Create leaf node.
  stree n = leaf (val);

  if (!n)
    {
      return NULL; // indicates an error to the caller
    }

  return insert_node (t, n);
}
#endif

stree
make_stree_from_array (int n, int array[n])
{
  stree t = EMPTY_TREE;

  for (int i = 0; i < n; i++)
    {
      t = insert (t, array[i]);
    }
  return t;
}

// Returns rightmost value of tree `t`.
// `t` cannot be empty.
int
rightmost_val (stree t)
{
  assert (t);
  if (!t->right)
    { // current node doesn't have right sub-tree so it is the rightmost node
      return t->value;
    }
  else
    {
      return rightmost_val (t->right); // tail recursive
    }
}

// Delete value `val` from tree `t`.
stree delete (stree t, int val)
{
  if (!t)
    {
      return t;
    }

  if (val < t->value)
    {
      t->left = delete (t->left, val); // not tail recursive
    }
  else if (val > t->value)
    {
      t->right = delete (t->right, val); // not tail recursive
    }
  else                                   // val == t->value
    {
      if (t->left && t->right)
        {                                // both sub-trees exist
          // store rightmost value of left sub-tree (= largest value in left sub-tree) in current node …
          t->value = rightmost_val (t->left);
          // … and delete this value in the left sub-tree; not tail recursive
          t->left = delete (t->left, t->value);
        }
      else
        { // at most one sub-tree (min 0/max 1) exists
          stree subtree = t->left ? t->left : t->right;
          free (t);
          return subtree;
        }
    }
  return t;
}

static void
print_stree_ (stree t)
{
  if (!t)
    {
      putchar ('_');
    }
  else
    {
      putchar ('[');
      print_stree_ (t->left); // not tail recursive
      putchar (',');
      printf ("%d", t->value);
      putchar (',');
      print_stree_ (t->right); // not tail recursive
      putchar (']');
    }
}

void
print_stree (stree t)
{
  print_stree_ (t);
  printf ("\n");
}

void
free_stree (stree t)
{
  if (!t)
    {
      return;
    }
  free_stree (t->left);  // not tail recursive
  free_stree (t->right); // not tail recursive
  free (t);
}

#define pre  "\n\33[38;5;206m"
#define post "\033[0m\n\n"

int
main ()
{
  printf (pre "---------- Original ------------------------" post);
  stree t = node (3, node (2, leaf (1), EMPTY_TREE), leaf (6));
  print_stree (t);

  printf (pre "---------- Inserting 10 and 0 --------------" post);
  t = insert (t, 10);
  print_stree (t);
  t = insert (t, 0);
  print_stree (t);

  printf (pre "---------- Deleting 12 and 3 ---------------" post);
  t = delete (t, 12);
  // delete (t, 12); // would be the same → we are changing tree in place
  print_stree (t);
  t = delete (t, 3);
  // delete (t, 3); // would be the same → we are changing tree in place
  print_stree (t);
  free_stree (t);

  printf (pre "---------- Tree from Array -----------------" post);
  int array[] = { 1, 2, 3, 4, 6, 8, 10 };
  int n       = sizeof array / sizeof *array;
  t           = make_stree_from_array (n, array);
  print_stree (t);
  free_stree (t);

  int array1[] = { 4, 6, 8, 1, 2, 10, 3 };
  n            = sizeof array1 / sizeof *array1;
  t            = make_stree_from_array (n, array1);
  print_stree (t);
  free_stree (t);
  printf ("\n");
}
