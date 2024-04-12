#pragma once

#include <stdbool.h>

typedef struct node
{
  struct node *parent; // allows removal just by having a node (see remove_node(…))
  struct node *left;
  struct node *right;
} node;

typedef struct stree_api
{
  void const *(*key) (node *n);
  int (*cmp) (void const *x, void const *y);
  void (*print) (node *n);
  void (*free) (node *n);
} stree_api;

typedef struct stree
{
  node      root; // dummy node -> every node has a parent: real tree starts with root's left child
  stree_api api;
} stree;

// `t` is `stree*`; returns bool
#define empty_tree(t) ((t)->root.left == NULL)

void   remove_node (node *n);
void   delete_node (stree *t, node *n);
void   insert_node (stree *t, node *n);
node  *find_node (stree *t, void const *key);
stree *new_tree (stree_api api);
void   print_tree (stree *t);
void   free_tree (stree *t);

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
