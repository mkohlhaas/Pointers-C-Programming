// Turn recursive functions into loops; search for while loops
// see rightmost(), find_loc(), free_nodes()
// print_stree() has been replaced by print_stree_indent()

// To replace recursion with a loop, we make a loop condition that should
// terminate the loop when we would directly return from the recursion's base case.
// The inversion of the base case condition will usually do.
// Update the function values in the loop.

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

#define EMPTY_TREE NULL

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

#define leaf(value) node (value, EMPTY_TREE, EMPTY_TREE)

stree *
rightmost (stree *t)
{
  while ((*t)->right)
    {
      t = &(*t)->right;
    }
  return t;
}

// in-order traversal of the tree
// explained in paragraph "Morris Traversal", p. 399
// esp. paragraph "Freeing Nodes Without Recursion and Memory Allocation", p. 403
void
free_nodes (Node *n)
{
  Node *curr = n;
  while (curr)
    {
      if (!curr->left)
        {
          Node *right = curr->right;
          free (curr); // only deallocation point
          curr = right;
        }
      else
        {
          stree pred  = *rightmost (&curr->left);
          pred->right = curr;       // create thread pointer
          Node *left  = curr->left;
          curr->left  = EMPTY_TREE; // never recurse into the left subtree again
          curr        = left;
        }
    }
}

stree *
new_stree (void)
{
  stree *t = malloc (sizeof (stree));
  if (t)
    {
      *t = NULL;
    }
  return t;
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

stree *
find_loc (stree *t, int val)
{
  while (*t && val != (*t)->value)
    {
      if (val < (*t)->value)
        {
          t = &(*t)->left; // update function values; here just t
        }
      else                 /* (val > (*t)->value) */
        {
          t = &(*t)->right;
        }
    }
  return t;
}

bool
contains (stree *t, int val)
{
  return !!*find_loc (t, val);
}

bool
insert (stree *t, int val)
{
  stree *loc = find_loc (t, val);
  if (*loc)
    {
      return true; // we have been already here
    }
  else
    {
      return !!(*loc = leaf (val));
    }
}

void delete (stree *t, int val)
{
  stree *loc = find_loc (t, val);

  if (*loc)
    {
      stree t = *loc;
      if (!(t->left && t->right))
        {
          *loc = t->left ? t->left : t->right;
          free (t);
        }
      else
        {
          stree *rm_ref = rightmost (&t->left);
          stree  rm     = *rm_ref;
          t->value      = rm->value;
          *rm_ref       = rm->left;
          free (rm);
        }
    }
}

stree *
make_stree (int n, int array[n])
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

void
print_stree_ (stree *t)
{
  if (!*t)
    {
      putchar ('_');
    }
  else
    {
      putchar ('[');
      print_stree_ (&(*t)->left);
      putchar (',');
      printf ("%d", (*t)->value);
      putchar (',');
      print_stree_ (&(*t)->right);
      putchar (']');
    }
}
// Not tail recursive
// not in use any more
void
print_stree (stree *t)
{
  printf ("\n");
  print_stree_ (t);
  printf ("\n");
}

#define ARRAY_SIZE(a) sizeof a / sizeof *a

int
main ()
{
  printf ("---------- Original -----------------------------");
  stree t = EMPTY_TREE;
  insert (&t, 3);
  insert (&t, 2);
  insert (&t, 1);
  insert (&t, 6);
  print_stree (&t);

  printf ("---------- Inserting 10 and 0 -------------------");
  insert (&t, 10);
  insert (&t, 0);
  print_stree (&t);

  printf ("---------- Deleting 12, 6 and 3 -----------------");
  delete (&t, 12);
  print_stree (&t);
  delete (&t, 3);
  print_stree (&t);
  delete (&t, 6);
  print_stree (&t);
  clear_stree (&t);

  printf ("---------- Tree from array ----------------------");
  int    array[] = { 1, 2, 13, 4, 16, 8, 10 };
  int    n       = ARRAY_SIZE (array);
  stree *t2      = make_stree (n, array);
  print_stree (t2);
  free_stree (t2);

  int array1[] = { 4, 6, 8, 1, 2, 10, 3 };
  n            = ARRAY_SIZE (array1);
  t2           = make_stree (n, array1);
  print_stree (t2);
  free_stree (t2);

  int array2[] = { 4, 1, 5 };
  n            = ARRAY_SIZE (array2);
  t2           = make_stree (n, array2);
  print_stree (t2);
  free_stree (t2);

  int array3[] = { 1, 2, 13, 4, 16, 8, 10 };
  n            = ARRAY_SIZE (array3);
  t2           = make_stree (n, array3);
  print_stree (t2);
  free_stree (t2);
}
