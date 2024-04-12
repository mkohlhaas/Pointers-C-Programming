// New function find_loc().

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

#define EMPTY_TREE NULL

typedef Node *stree;

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

// tail recursive
// `t` is address of where current node is stored; `*t` is address of current node.
// find tree - return address of an stree - where `val` is or should be (e.g. if inserted)
stree *
find_loc (stree *t, int val)
{
  assert (t);

  if (!*t || val == (*t)->value)
    {
      return t;
    }
  else if (val < (*t)->value)
    {
      return find_loc (&(*t)->left, val);  // &(*t)->left = address of `left` pointer
    }
  else                                     // val > (*t)->value)
    {
      return find_loc (&(*t)->right, val); // &(*t)->right = address of `right` pointer
    }
}

bool
contains (stree *t, int val)
{
  return !!*find_loc (t, val);
}

bool
insert (stree *t, int val)
{
  stree *target = find_loc (t, val);
  if (*target)
    {
      return true; // already there
    }
  else
    {
      return !!(*target = leaf (val));
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

// Not tail recursive
void
free_nodes (Node *n)
{
  if (!n)
    {
      return;
    }
  free_nodes (n->left);
  free_nodes (n->right);
  free (n);
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

// tail recursive
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
      return rightmost (&(*t)->right);
    }
}

void delete (stree *t, int val)
{
  stree *target = find_loc (t, val);
  if (*target)
    {
      stree t = *target;
      if (!(t->left && t->right))
        {
          *target = t->left ? t->left : t->right;
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
void
print_stree (stree *t)
{
  print_stree_ (t);
  printf ("\n");
}

#define pre  "\n\33[38;5;206m"
#define post "\033[0m\n\n"

#define ARRAY_SIZE(a) sizeof a / sizeof *a

int
main ()
{
#if 0
  stree t =
    node(3,
      node(2, leaf(1), EMPTY),
      leaf(6));
#endif

  printf (pre "---------- Original ------------------------" post);
  stree  t_ = EMPTY_TREE;
  stree *t  = &t_;
  insert (t, 3);
  insert (t, 2);
  insert (t, 1);
  insert (t, 6);
  assert (contains (t, 2));
  assert (contains (t, 1));
  assert (contains (t, 3));
  assert (!contains (t, 10));
  assert (!contains (t, 0));
  print_stree (t);

  printf (pre "---------- Inserting 10 and 0 --------------" post);
  insert (t, 10);
  insert (t, 0);
  assert (contains (t, 10));
  assert (contains (t, 0));
  print_stree (t);

  printf (pre "---------- Deleting 12, 6 and 3 ------------" post);
  delete (t, 12);
  delete (t, 6);
  delete (t, 3);
  assert (!contains (t, 3));
  assert (!contains (t, 6));
  print_stree (t);
  clear_stree (t);

  printf (pre "---------- Tree from Array -----------------" post);
  int    array[] = { 1, 2, 3, 4, 6, 8, 10 };
  int    n       = ARRAY_SIZE (array);
  stree *t2      = make_stree (n, array);
  print_stree (t2);
  free_stree (t2);

  int array1[] = { 4, 6, 8, 1, 2, 10, 3 };
  n            = ARRAY_SIZE (array);
  t            = make_stree (n, array1);
  print_stree (t);
  free_stree (t);
}
