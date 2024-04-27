#include "stree.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// To create a new tree `key` and `cmp` functions are necessary.
stree *
new_tree (stree_api api)
{
  if (!(api.key && api.cmp))
    {
      return NULL;
    }
  stree *tree = malloc (sizeof *tree);
  if (tree)
    {
      node root = {
        .parent = NULL,
        .left   = NULL,
        .right  = NULL,
      };
      *tree = (stree){
        .root = root,
        .api  = api,
      };
    }
  return tree;
}

static void
free_nodes_rec (free_node_fn free, node *n)
{
  if (!n)
    {
      return;
    }
  free_nodes_rec (free, n->left);
  free_nodes_rec (free, n->right);
  if (free)
    {
      free (n);
    }
}

void
free_tree (stree *t)
{
  free_nodes_rec (t->api.free, t->root.left);
  free (t);
}

// TODO: rewrite this function(?).
//       `node** n` and return value are the same.
//       `n` is always set to `node **real_tree = &parent->left;`
//       before being called.

// Finds parent and child.
node **
find_loc (stree *t, void const *key, node **n, node **p)
{
  key_node_fn  get_key = t->api.key;
  cmp_nodes_fn cmp     = t->api.cmp;

  while (*n)
    {
      int cmp_res = cmp (key, get_key (*n));
      if (cmp_res == 0)
        {
          return n;
        }
      *p = *n;
      if (cmp_res < 0)
        {
          n = &(*n)->left;
        }
      else
        {
          n = &(*n)->right;
        }
    }
  return n;
}

node *
find_node (stree *t, void const *key)
{
  node  *parent    = &t->root;
  node **real_tree = &parent->left;
  return *find_loc (t, key, real_tree, &parent);
}

void
insert_node (stree *t, node *n)
{
  node       *parent    = &t->root;
  node      **real_tree = &parent->left;
  void const *key       = t->api.key (n);
  node      **target    = find_loc (t, key, real_tree, &parent);
  if (*target)
    {
      delete_node (t, *target);
    }
  *target   = n;             // TODO: what is this good for? It's superfluous.
  n->parent = parent;
  n->left = n->right = NULL; // makes the node a leaf
}

node **
rightmost (node **n, node **parent)
{
  while ((*n)->right)
    {
      *parent = *n;
      n       = &(*n)->right;
    }
  return n;
}

void
remove_node (node *n)
{
  // dummy root has no parent
  if (!n->parent)
    {
      return;
    }

  // Get the location to replace. Address of left or right pointer of parent.
  node **loc = (n == n->parent->left) ? &n->parent->left : &n->parent->right;

  if (!(n->left && n->right))
    { // node has at least one empty child...
      *loc = n->left ? n->left : n->right;
      if (*loc)
        {
          (*loc)->parent = n->parent; // could be that both children are emtpy
        }
    }
  else
    { // both children are set; replace node with rightmost child of left tree
      node  *rm_parent = n;
      node **rm_ref    = rightmost (&n->left, &rm_parent);
      node  *rm        = *rm_ref;
      *rm_ref          = rm->left;
      if (*rm_ref)
        {
          (*rm_ref)->parent = rm_parent; // set parent of left of rightmost - if existing - to rightmost's parent
        }
      *loc = rm;
      *rm  = *n;
    }
  n->left = n->right = n->parent = NULL;
}

void
delete_node (stree *t, node *n)
{
  remove_node (n);
  if (t->api.free)
    {
      t->api.free (n);
    }
}

void
print_node (print_node_fn print, node *n)
{
  if (!n)
    {
      return;
    }
  putchar ('(');
  print_node (print, n->left);
  putchar (',');
  print (n);
  putchar (',');
  print_node (print, n->right);
  putchar (')');
}

static void
default_print_node (node *n)
{
  printf ("<node %p>", n);
}

void
print_tree (stree *t)
{
  print_node_fn print = t->api.print ? t->api.print : default_print_node;
  print_node (print, t->root.left);
}

static inline bool
contains (stree *t, void const *key)
{
  return !!find_node (t, key);
}

static inline void
delete_key (stree *t, void const *key)
{
  node *x = find_node (t, key);
  if (x)
    {
      delete_node (t, x);
    }
}
