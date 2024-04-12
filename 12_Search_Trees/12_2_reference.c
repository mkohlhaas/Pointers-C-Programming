// For consistency all functions will take `stree*` arguments instead of `stree`.
// Using a reference to stree allows us to mutate the tree in-place instead of returning a new one.
// Was a problem in `12_1_simple.c` for inserting into an empty tree.

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct node
{
  int          value;
  struct node *left;
  struct node *right;
} Node;

typedef Node *stree;

#define EMPTY_TREE  NULL
#define leaf(value) node (value, EMPTY_TREE, EMPTY_TREE)

// Creates a node on the heap.
stree
node (int value, stree left, stree right)
{
  // creates node on the heap
  stree t = malloc (sizeof *t);

  if (t)
    {
      *t = (Node){ .value = value, .left = left, .right = right };
    }

  return t;
}

bool
contains (stree *tp, int val)
{
  assert (tp);

  stree t = *tp;

  if (!t)
    {
      return false;
    }
  else if (val < t->value)
    {
      return contains (&t->left, val); // address of left subtree; same as &(t->left); (p. 385)
    }
  else if (val > t->value)
    {
      return contains (&t->right, val);
    }
  else
    { // val == t->value
      return true;
    }
}

// Tail recursive and handles errors.
// Change tree in-place.
// Returns `true` in case of successful insertion (allocation of a new leaf could fail).
bool
insert (stree *tp, int val)
{
  assert (tp);

  stree t = *tp;

  if (!t)
    {
      // empty tree
      // update in-place (was a problem in `12_1_simple.c`
      return !!(*tp = leaf (val)); // result of expression is right-hand side → `leaf(val)`
    }
  else if (val < t->value)
    {
      return insert (&t->left, val);
    }
  else if (val > t->value)
    {
      return insert (&t->right, val);
    }
  else // val == t->value
    {
      return true;
    }
}

// Creates new empty tree on the heap.
stree *
new_stree ()
{
  stree *t = malloc (sizeof (stree)); // allocate memory for a pointer

  if (t)
    {
      *t = EMPTY_TREE; // an empty tree is a pointer with a NULL pointer
    }

  return t;
}

void
free_nodes (stree t)
{
  if (!t)
    {
      return;
    }

  free_nodes (t->left);  // free left sub-tree
  free_nodes (t->right); // free right sub-tree
  free (t);              // free current node
}

// t is an stree*
#define clear_stree(t)                                                                                                 \
  do                                                                                                                   \
    {                                                                                                                  \
      free_nodes (*t);                                                                                                 \
      *t = EMPTY_TREE;                                                                                                 \
    }                                                                                                                  \
  while (0)

// t is stree*
#define free_stree(t)                                                                                                  \
  do                                                                                                                   \
    {                                                                                                                  \
      free_nodes (*t);                                                                                                 \
      free (t);                                                                                                        \
      t = EMPTY_TREE;                                                                                                  \
    }                                                                                                                  \
  while (0)

// Creates a new stree from array on the heap.
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

// Returns address of pointer to rightmost tree.
stree *
rightmost (stree *t)
{
  assert (t && *t);

  if (!(*t)->right)
    {
      return t;
    }
  else
    {
      // tail recursive
      return rightmost (&(*t)->right); // &(*t)->right ⇒ 1. deref t, 2. get right tree, 3. take address (p. 388)
    }
}

// tail recursive
void delete (stree *target, int val)
{
  assert (target);

  stree t = *target; // t: node* (current node)

  if (!t)
    {
      return;
    }

  if (val < t->value)
    {
      delete (&t->left, val);
    }
  else if (val > t->value)
    {
      delete (&t->right, val);
    }
  else // val == t->value
    {
      if (!(t->left && t->right))
        {
          // not both sub-trees exist (at most 1; 0 or 1)
          stree subtree = t->left ? t->left : t->right;
          *target       = subtree;
          free (t);
        }
      else                                          // both subtrees exist
        {
          stree *rm_val_ref = rightmost (&t->left); // rm_val_ref: node** (= address of node*); Fig. 12.4, p. 387
          stree  rm         = *rm_val_ref;          // rm:         node*  (= address of rightmost node)
          t->value          = rm->value;            // rightmost value of left sub-tree
          *rm_val_ref       = rm->left;             // replace rightmost node with its left node; Fig. 12.3, p. 375
          free (rm);
        }
    }
}

static void
print_stree_ (stree *t)
{
  if (!*t)
    {
      putchar ('_');
    }
  else
    {
      putchar ('[');
      print_stree_ (&(*t)->left); // &(*t)->left = 1. deref t, 2. get left tree, 3. take address (p. 388)
      putchar (',');
      printf ("%d", (*t)->value);
      putchar (',');
      print_stree_ (&(*t)->right);
      putchar (']');
    }
}

#define c(n) contains (t, n) ? "✓" : "✗"

void
print_contains (stree *t)
{
  printf ("contains %d: %s\n", 0, c (0));
  printf ("contains %d: %s\n", 1, c (1));
  printf ("contains %d: %s\n", 2, c (2));
  printf ("contains %d: %s\n", 3, c (3));
  printf ("contains %d: %s\n", 6, c (6));
  printf ("contains %d: %s\n", 9, c (9));
  printf ("\n");
}

// not tail recursive
void
print_stree (stree *t)
{
  print_stree_ (t);
  printf ("\n\n");
  print_contains (t);
}

#define ARRAY_SIZE(a) sizeof a / sizeof *a

#define pre  "\33[38;5;206m"
#define post "\033[0m\n\n"

int
main ()
{
#if 0
  stree t = node(3, node(2, leaf(1), EMPTY_TREE), leaf(6));
#endif

  // create tree on the stack
  stree  t_ = EMPTY_TREE;
  stree *t  = &t_;

  printf (pre "---------- Original ------------------------" post);
  insert (t, 3);
  insert (t, 2);
  insert (t, 1);
  insert (t, 6);
  print_stree (t);

  printf (pre "---------- Inserting 0 and 9 ---------------" post);
  insert (t, 0);
  print_stree (t);
  insert (t, 9);
  print_stree (t);

  printf (pre "---------- Deleting 12, 6 and 3 ------------" post);
  delete (t, 12);
  print_stree (t);
  delete (t, 6);
  print_stree (t);
  delete (t, 3);
  print_stree (t);

  printf (pre "---------- Tree from Array -----------------" post);
  clear_stree (t); // cannot use `free_stree` → `t` points to the stack (`t_`)

  int array[] = { 1, 2, 3, 4, 6, 8, 10 };
  int n       = ARRAY_SIZE (array);
  t           = make_stree_from_array (n, array); // `t` points to the heap
  print_stree (t);
  free_stree (t);

  int array1[] = { 4, 6, 8, 1, 2, 10, 3 };
  n            = ARRAY_SIZE (array);
  t            = make_stree_from_array (n, array1);
  print_stree (t);
  free_stree (t);
}
