#include "refcount.h"
#include "strees.h"

// global refcounted vars must be initialised
Node *empty_node;

void
init_empty_node (void)
{
  if (empty_node)
    {
      return;
    }
  empty_node = rc_alloc (sizeof *empty_node, NULL);
  memcpy (empty_node, &(Node){ .val = 314159, .left = NULL, .right = NULL }, sizeof *empty_node);
  if (!empty_node)
    {
      abort ();
    }
}

// counting number of nodes
size_t n_nodes;

void
free_node (void *p, void *ctx)
{
  n_nodes--;
  Node *n = p;
  printf ("Freeing node %d.\n", n->val);
  decref_ctx (n->left, ctx);
  decref_ctx (n->right, ctx);
}

Node *
new_node (int val, takes Node *left, takes Node *right)
{
  if (is_error (left) || is_error (right))
    {
      goto error;
    }
  Node *n = rc_alloc (sizeof *n, free_node);
  if (!n)
    {
      goto error;
    }
  n_nodes++;
  memcpy (n, &(Node){ .val = val, .left = give (left), .right = give (right) }, sizeof *n);
  return n;

error:
  decref (left);
  decref (right);
  return NULL;
}

bool
contains (borrows Node *tree, int val)
{
  assert (!is_error (tree));
  if (is_empty (tree))
    {
      return false;
    }
  if (tree->val == val)
    {
      return true;
    }
  if (val < tree->val)
    {
      return contains (tree->left, val);
    }
  else
    {
      return contains (tree->right, val);
    }
}

Node *
insert (takes Node *tree, int val)
{
  if (is_error (tree))
    {
      return NULL;
    }
  if (is_empty (tree))
    {
      decref (tree);
      return new_node (val, EMPTY, EMPTY);
    }
  if (val == tree->val)
    {
      return give (tree);
    }
  // int tval = tree->val;
  Node *left  = incref (tree->left);
  Node *right = incref (tree->right);
  decref (tree);
  if (val < tree->val)
    {
      return new_node (tree->val, insert (give (left), val), give (right));
    }
  else
    {
      return new_node (tree->val, give (left), insert (give (right), val));
    }
}

int
rightmost_value (borrows Node *tree)
{
  assert (!is_empty (tree));
  while (!is_empty (tree->right))
    {
      tree = tree->right;
    }
  return tree->val;
}

Node *delete (takes Node *tree, int val)
{
  if (is_empty (tree))
    {
      return give (tree);
    }

  int   tval  = tree->val;
  Node *left  = incref (tree->left);
  Node *right = incref (tree->right);
  decref (tree);

  if (val < tval)
    {
      return new_node (tval, delete (give (left), val), give (right));
    }
  else if (val > tval)
    {
      return new_node (tval, give (left), delete (give (right), val));
    }
  else
    {
      if (is_empty (left))
        {
          decref (left);
          return give (right);
        }
      if (is_empty (right))
        {
          decref (right);
          return give (left);
        }
      int rmval = rightmost_value (left);
      return new_node (rmval, delete (give (left), rmval), give (right));
    }
}

void
print_tree_ (borrows Node *n)
{
  if (is_empty (n))
    {
      return;
    }
  putchar ('(');
  print_tree_ (n->left);
  printf (",%d[%zd],", n->val, (get_refcount (n))->rc);
  print_tree_ (n->right);
  putchar (')');
}

void
print_tree (borrows Node *n)
{
  print_tree_ (n);
  putchar ('\n');
}

int
main ()
{
  Node *x, *y, *z;
  init_empty_node ();

  printf (" =============== Case 1 ===============\n");
  printf ("             Introduction.\n");
  printf (" ======================================\n");
  x = new_node (1, new_node (0, EMPTY, EMPTY), new_node (3, new_node (2, EMPTY, EMPTY), EMPTY));
  print_tree (x);

  y = new_node (10, new_node (9, EMPTY, EMPTY), new_node (11, EMPTY, EMPTY));
  z = new_node (5, incref (x), incref (y));
  print_tree (z);

  decref (x);
  print_tree (z);
  decref (z);
  print_tree (y);
  decref (y); // free the rest

  printf ("%zu nodes.\n", n_nodes);

  printf ("  =============== Case 2 ===============\n");
  printf ("               Building x.\n");
  printf ("  ======================================\n");
  x = insert (EMPTY, 5);
  print_tree (x);
  x = insert (x, 2);
  print_tree (x);
  x = insert (x, 4);
  print_tree (x);
  x = insert (x, 8);
  print_tree (x);

  printf ("%zu nodes.\n", n_nodes);
  decref (x);
  printf ("%zu nodes.\n", n_nodes);

  printf ("  =============== Case 3 ===============\n");
  printf ("              Rebuilding x.\n");
  printf ("  ======================================\n");
  x = insert (EMPTY, 5);
  print_tree (x);
  x = insert (x, 2);
  print_tree (x);
  x = insert (x, 4);
  print_tree (x);
  x = insert (x, 8);
  print_tree (x);
  printf ("%zu nodes.\n", n_nodes);

  printf ("  =============== Case 4 ===============\n");
  printf ("             Deleting from x.\n");
  printf ("  ======================================\n");
  x = delete (x, 1);
  print_tree (x);
  x = delete (x, 2);
  print_tree (x);
  x = delete (x, 4);
  print_tree (x);
  printf ("%zu nodes.\n", n_nodes);

  printf ("  =============== Case 5 ===============\n");
  printf ("             Building y.\n");
  printf ("  ======================================\n");
  // incref x here to keep it!
  y = insert (incref (x), 1);
  print_tree (x);
  print_tree (y);
  y = insert (y, 2);
  print_tree (x);
  print_tree (y);
  y = insert (y, 3);
  print_tree (x);
  print_tree (y);
  printf ("%zu nodes.\n", n_nodes);

  printf ("  =============== Case 6 ===============\n");
  printf ("    Deleting 5 (it is in both lists).\n");
  printf ("  ======================================\n");
  x = delete (x, 5);
  printf ("Didn't trigger deletion...\n");
  y = delete (y, 5);
  printf ("But this did!\n");
  print_tree (x);
  print_tree (y);
  printf ("%zu nodes.\n", n_nodes);

  printf ("  =============== Case 7 ===============\n");
  printf ("            Cleaning up...\n");
  printf ("  ======================================\n");
  printf ("%zu nodes.\n", n_nodes);
  decref (x);
  printf ("%zu nodes.\n", n_nodes);
  decref (y);
  printf ("%zu nodes.\n", n_nodes);

  printf ("  =============== Case 8 ===============\n");
  printf ("          Chapter example...\n");
  printf ("  ======================================\n");
  x = insert (EMPTY, 5);
  x = insert (x, 1);
  x = insert (x, 2);
  x = insert (x, 3);

  y = insert (incref (x), 10);
  y = insert (y, 9);

  printf ("Do I delete now?\n");
  x = delete (x, 2);
  printf ("%zu nodes.\n", n_nodes);
}
