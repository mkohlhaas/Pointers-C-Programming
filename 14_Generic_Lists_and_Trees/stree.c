#include "stree.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

STree*
new_tree(STreeType type)
{
  // key and cmp are always needed. The rest are optional.
  if (!(type.key && type.cmp)) return NULL;
  STree*     tree = malloc(sizeof *tree);
  if (tree) *tree = (STree){ .root = { .parent = NULL, .left = NULL, .right = NULL },
                             .type = type };
  return tree;
}

// Find parent and child
Node**
find_loc(STree* tree, void const* key, Node** n, Node** p)
{
  void const* (*get_key) (Node* n)                      = tree->type.key;
  int         (*cmp)     (void const* x, void const* y) = tree->type.cmp;

  while (*n) {
    int cmpres = cmp(key, get_key(*n));
    if   (cmpres == 0) return n;
    *p = *n;
    if   (cmpres <  0) n = &(*n)->left;
    else               n = &(*n)->right;
  }
  return n;
}

Node*
find_node(STree* tree, void const* key)
{
  Node*  parent    = &tree->root;
  Node** real_tree = &parent->left;
  return *find_loc(tree, key, real_tree, &parent);
}

void
insert_node(STree* tree, Node* n)
{
  Node*        parent    = &tree->root;
  Node**       real_tree = &parent->left;
  void const*  key       = tree->type.key(n);
  Node**       target    = find_loc(tree, key, real_tree, &parent);
  if (*target) delete_node(tree, *target);  // remove the old node and replace it with the new one
               *target   = n;
               n->parent = parent;
               n->left   = n->right = NULL; // make the node a leaf
}

Node**
rightmost(Node** n, Node** p)
{
  while ((*n)->right) {
    *p =   *n;
     n = &(*n)->right;
  }
  return n;
}

void
remove_node(Node* n)
{
  // only dummy root has no parent
  if (!n->parent) return;

  // Get the location to replace. Address of left or right pointer of parent.
  Node** loc = (n == n->parent->left) ? &n->parent->left : &n->parent->right;

  if (!(n->left && n->right)) { // node has at least one empty child...
    *loc = n->left ? n->left : n->right;
    if (*loc) (*loc)->parent = n->parent;  // could be both children are emtpy
  } else {  // both children are set; replace node with rightmost child of left tree
    Node*   rm_parent = n;
    Node**  rm_ref    = rightmost(&n->left, &rm_parent);
    Node*   rm        = *rm_ref;
           *rm_ref    = rm->left;
    if (*rm_ref) (*rm_ref)->parent = rm_parent;  // set parent of left of rightmost - if existing - to rightmost's parent
           *loc       =  rm;
           *rm        = *n;
  }
  n->left = n->right = n->parent = NULL;
}

void
delete_node(STree* tree, Node* n)
{
  remove_node(n);
  if (tree->type.free) tree->type.free(n);
}

// Just recursion this time; the techniques for avoid it hasn't changed.
static void
default_print(Node* n)
{
  printf("<node %p>", n);
}

void
print_node(void (*print)(Node* n), Node* n)
{
  if (!n) return;
  putchar('(');
  print_node(print, n->left);
  putchar(','); print(n); putchar(',');
  print_node(print, n->right);
  putchar(')');
}

void
print_tree(STree* tree)
{
  void (*print)(Node*) = tree->type.print ? tree->type.print : default_print;
  print_node(print, tree->root.left);
}

void
free_nodes_rec(void (*free)(Node* n), Node* n)
{
  if (!n) return;
  free_nodes_rec(free, n->left);
  free_nodes_rec(free, n->right);
  if (free) {
    n->left = n->right = n->parent = NULL;  // remove pointers before callback
    free(n);
  }
}

void
free_tree(STree* tree)
{
  free_nodes_rec(tree->type.free, tree->root.left);
  free(tree);
}
