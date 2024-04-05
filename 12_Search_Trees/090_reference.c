// functions will now use STree* instead of STree
// using a reference to STree allows us to mutate the tree in-place instead of returning a new one

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct node Node;
typedef Node *STree;

typedef struct node {
  int value;
  STree left;
  STree right;
} Node;

#define EMPTY NULL

// same as before
STree node(int value, STree left, STree right) {
  STree t = malloc(sizeof *t);
  if (t)
    *t = (Node){.value = value, .left = left, .right = right};
  return t;
}

// same as before
// returns STree
#define leaf(value) node(value, EMPTY, EMPTY)

// tail recursive
bool contains(STree *tp, int val) {
  assert(tp);
  STree t = *tp;
  if (!t)
    return false;
  else if (val < t->value)
    return contains(&t->left, val); // pass address of left subtree; same as &(t->left); (p. 385)
  else if (val > t->value)
    return contains(&t->right, val);
  else /* (val == t->value) */
    return true;
}

// Tail recursive, and we handle errors
// change tree in-place
// returns true in case of successful insert (allocation of a new leaf could fail)
bool insert(STree *target, int val) {
  assert(target);
  STree t = *target;
  if (!t)
    return !!(*target = leaf(val)); // result of expression is right-hand side - leaf(val)
  else if (val < t->value)
    return insert(&t->left, val);
  else if (val > t->value)
    return insert(&t->right, val);
  else /* (val == t->value) */
    return true;
}

STree *new_stree() {
  STree *t = malloc(sizeof *t); // allocate memory for a pointer
  if (t)
    *t = EMPTY; // an empty tree is a pointer to a NULL pointer
  return t;
}

// Not tail recursive
void free_nodes(STree t) {
  if (!t)
    return;
  free_nodes(t->left);
  free_nodes(t->right);
  free(t);
}

// t is STree*
#define clear_stree(t)                                                                                                 \
  do {                                                                                                                 \
    free_nodes(*t);                                                                                                    \
    *t = EMPTY;                                                                                                        \
  } while (0)

#define free_stree(t)                                                                                                  \
  do {                                                                                                                 \
    free_nodes(*t);                                                                                                    \
    free(t);                                                                                                           \
    t = NULL;                                                                                                          \
  } while (0)

STree *make_stree(int n, int array[n]) {
  STree *t = new_stree();
  if (!t)
    return NULL;
  for (int i = 0; i < n; i++)
    if (!insert(t, array[i])) {
      free_stree(t);
      return NULL;
    }
  return t;
}

// tail recursive
// return address of pointer to rightmost tree
STree *rightmost(STree *t) {
  assert(t && *t);
  if (!(*t)->right)
    return t;
  else
    return rightmost(&(*t)->right); // &(*t)->right = 1. deref t, 2. get right tree, 3. take address (p. 388)
}

// Tail recursive
void delete(STree *target, int val) {
  assert(target);
  STree t = *target;
  if (!t)
    return;
  if (val < t->value)
    delete (&t->left, val);
  else if (val > t->value)
    delete (&t->right, val);
  else /* (val == t->value) */ {
    if (!(t->left && t->right)) { // at most one of the subtrees (0 or 1) exist
      STree subtree = t->left ? t->left : t->right;
      *target = subtree;
      free(t);
    } else { // both subtrees exist
      STree *rm_ref = rightmost(&t->left);
      STree rm = *rm_ref;
      t->value = rm->value;
      *rm_ref = rm->left;
      free(rm);
    }
  }
}

// Not tail recursive
void print_stree(STree *t) {
  if (!*t) {
    putchar('_');
  } else {
    putchar('[');
    print_stree(&(*t)->left); // &(*t)->left = 1. deref t, 2. get left tree, 3. take address (p. 388)
    putchar(',');
    printf("%d", (*t)->value);
    putchar(',');
    print_stree(&(*t)->right);
    putchar(']');
  }
}

int main() {
#if 0
  STree t = node(3, node(2, leaf(1), EMPTY), leaf(6));
#endif

  printf("\33[38;5;206m========== Original ========================\033[0m\n");
  STree t_ = EMPTY;
  STree *t = &t_;
  insert(t, 3);
  insert(t, 2);
  insert(t, 1);
  insert(t, 6);
  assert(contains(t, 2));
  assert(contains(t, 1));
  assert(contains(t, 3));
  assert(!contains(t, 10));
  assert(!contains(t, 0));
  print_stree(t);

  printf("\n\33[38;5;206m========== Inserting 10 and 0 ==============\033[0m\n");
  insert(t, 10);
  insert(t, 0);
  assert(contains(t, 10));
  assert(contains(t, 0));
  print_stree(t);

  printf("\n\33[38;5;206m========== Deleting 12, 6 and 3 ============\033[0m\n");
  delete (t, 12);
  delete (t, 6);
  delete (t, 3);
  print_stree(t);
  assert(!contains(t, 3));
  assert(!contains(t, 6));
  clear_stree(t); // not free_stree; the pointer itself (i.e. t) is on the stack

  printf("\n\33[38;5;206m========== Tree from array =================\033[0m\n");
  int array[] = {1, 2, 3, 4, 6, 8, 10};
  int n = sizeof array / sizeof *array;
  t = make_stree(n, array);
  print_stree(t);
  putchar('\n');
  free_stree(t); // delete all including pointer to STree* - see make_stree()

  int array1[] = {4, 6, 8, 1, 2, 10, 3};
  n = sizeof array1 / sizeof *array1;
  t = make_stree(n, array1);
  print_stree(t);
  putchar('\n');
  free_stree(t);
}
