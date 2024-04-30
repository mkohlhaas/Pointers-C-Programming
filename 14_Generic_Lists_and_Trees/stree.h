#pragma once

#include <stdbool.h>

typedef struct node
{
  struct node *parent;
  struct node *left;
  struct node *right;
} node;

typedef void const *(*key_node_fn) (node *n);
typedef int         (*cmp_nodes_fn) (void const *x, void const *y);
typedef void        (*print_node_fn) (node *n);
typedef void        (*free_node_fn) (node *n);

typedef struct stree_api
{
  key_node_fn   key;
  cmp_nodes_fn  cmp;
  print_node_fn print;
  free_node_fn  free;
} stree_api;

typedef struct stree
{
  // `root` is a dummy node.
  // Every node - except the root node - has a parent.
  // Real tree starts with left child of `root`.
  node      root;
  stree_api api;
} stree;

void   remove_node (node *n);
void   delete_node (stree *t, node *n);
void   insert_node (stree *t, node *n);
node  *find_node (stree *t, void const *key);
stree *new_tree (stree_api api);
void   print_tree (stree *t);
void   free_tree (stree *t);
