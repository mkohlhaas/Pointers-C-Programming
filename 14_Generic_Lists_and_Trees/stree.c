#include "stree.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

stree *
new_tree (stree_api api)
{
  // `key` and `cmp` functions are mandatory.
  if (!(api.key && api.cmp))
    {
      return NULL;
    }
  stree *tree = malloc (sizeof *tree);
  if (tree)
    {
      *tree = (stree){
        .root = { .parent = NULL, .left = NULL, .right = NULL },
        .api  = api,
      };
    }
  return tree;
}

// Find parent and child
node **
find_loc (stree *t, void const *key, node **n, node **p)
{
  void const *(*get_key) (node *n)          = t->api.key;
  int (*cmp) (void const *x, void const *y) = t->api.cmp;

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
      delete_node (t, *target); // remove the old node and replace it with the new one
    }
  *target   = n;
  n->parent = parent;
  n->left = n->right = NULL; // make the node a leaf
}

node **
rightmost (node **n, node **p)
{
  while ((*n)->right)
    {
      *p = *n;
      n  = &(*n)->right;
    }
  return n;
}

void
remove_node (node *n)
{
  // only dummy root has no parent
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
          (*loc)->parent = n->parent; // could be both children are emtpy
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

// Just recursion this time; the techniques for avoid it hasn't changed.
static void
default_print (node *n)
{
  printf ("<node %p>", n);
}

void
print_node (void (*print) (node *n), node *n)
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

void
print_tree (stree *t)
{
  void (*print) (node *) = t->api.print ? t->api.print : default_print;
  print_node (print, t->root.left);
}

void
free_nodes_rec (void (*free) (node *n), node *n)
{
  if (!n)
    {
      return;
    }
  free_nodes_rec (free, n->left);
  free_nodes_rec (free, n->right);
  if (free)
    {
      n->left = n->right = n->parent = NULL; // remove pointers before callback
      free (n);
    }
}

void
free_tree (stree *t)
{
  free_nodes_rec (t->api.free, t->root.left);
  free (t);
}
