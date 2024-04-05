#pragma once

#include <stdbool.h>

typedef struct node Node;

typedef struct node {
  Node *parent; // allows removal just by having a node (remove_node)
  Node *left;
  Node *right;
} Node;

typedef struct stree_type {
  void const *(*key)(Node *n);
  int (*cmp)(void const *x, void const *y);
  void (*print)(Node *n);
  void (*free)(Node *n);
} STreeType;

typedef struct stree {
  Node root; // dummy node -> every node has a parent: real tree starts with root's left child
  STreeType type;
} STree;

// t is STree*; returns bool
#define empty_tree(t) ((t)->root.left == NULL)

void remove_node(Node *n);
void delete_node(STree *tree, Node *n);
void insert_node(STree *tree, Node *n);
Node *find_node(STree *tree, void const *key);
STree *new_tree(STreeType type);
void print_tree(STree *tree);
void free_tree(STree *tree);

static inline bool contains(STree *tree, void const *key) { return !!find_node(tree, key); }

static inline void delete_key(STree *tree, void const *key) {
  Node *x = find_node(tree, key);
  if (x)
    delete_node(tree, x);
}
