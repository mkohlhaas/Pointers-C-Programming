// search for embedded stack

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
  STree stack; // embedded stack!!!
} Node;

#define EMPTY NULL

STree node(int value, STree left, STree right) {
  STree t = malloc(sizeof *t); // allocate a Node on the heap
  if (t)
    *t = (Node){.value = value,
                .left = left,
                .right = right,
                .stack = EMPTY}; // copy literal Node on the stack to heap allocated memory
  return t;
}

#define leaf(value) node(value, EMPTY, EMPTY)

// Not tail recursive
// STree* is a pointer to a Node pointer
void print_stree(STree *tree) {
  if (!*tree)
    putchar('_');
  else {
    putchar('[');
    print_stree(&(*tree)->left);
    putchar(',');
    printf("%d", (*tree)->value);
    putchar(',');
    print_stree(&(*tree)->right);
    putchar(']');
  }
}

STree *new_stree(void) {
  STree *t = malloc(sizeof *t); // allocate a pointer to a Node
  if (t)
    *t = NULL; // NULL indicates error
  return t;
}

void free_nodes(STree tree);

// t is a pointer to STree (STree*)
#define clear_stree(t)                                                                                                 \
  do {                                                                                                                 \
    free_nodes(*t);                                                                                                    \
    *t = EMPTY;                                                                                                        \
  } while (0)

// t is a pointer to STree (STree*)
#define free_stree(t)                                                                                                  \
  do {                                                                                                                 \
    free_nodes(*t);                                                                                                    \
    free(t);                                                                                                           \
    t = NULL;                                                                                                          \
  } while (0)

// tail recursive
// find location of val or where we should place val
STree *find_loc(STree *tree, int val) {
  assert(tree);
  if (!*tree || val == (*tree)->value)
    return tree; // Node is empty or we found value
  else if (val < (*tree)->value)
    return find_loc(&(*tree)->left, val);
  else /* (val >  (*tree)->value) */
    return find_loc(&(*tree)->right, val);
}

bool contains(STree *tree, int val) { return !!*find_loc(tree, val); }

bool insert(STree *tree, int val) {
  STree *target = find_loc(tree, val);
  if (*target)
    return true; // already there
  else
    return !!(*target = leaf(val));
}

// tail recursive
STree *rightmost(STree *tree) {
  assert(tree && *tree);
  if (!(*tree)->right)
    return tree; // there is no more right tree; we reached the end
  else
    return rightmost(&(*tree)->right);
}

void delete(STree *tree, int val) {
  STree *target = find_loc(tree, val);
  if (*target) {
    STree t = *target;
    if (!(t->left && t->right)) {
      *target = t->left ? t->left : t->right;
      free(t);
    } else {
      STree *rm_ref = rightmost(&t->left);
      STree rm = *rm_ref;
      t->value = rm->value;
      *rm_ref = rm->left;
      free(rm);
    }
  }
}

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

// =================== Using the embedded stack ========================================

// we don't allocate new memory but instead use the existing one in the STree structure
// copy old stack value ('stack' = top of stack) into tree-stack
// and set 'stack' to tree which is the new top of the stack
void push(STree *stack, STree tree) {
  if (tree) {
    tree->stack = *stack; // embedded stack points to a Node
    *stack = tree;        // stack points to tree
  }
}

// copy next stack value into 'stack' and remove top of stack by setting it to NULL
STree pop(STree *stack) {
  STree top = *stack;
  *stack = top->stack;
  top->stack = NULL;
  return top;
}

void free_nodes(STree tree) {
  STree stack = EMPTY; // stack is the top of the stack in the tree
  push(&stack, tree);
  while (stack) {
    tree = pop(&stack);
    push(&stack, tree->left);
    push(&stack, tree->right);
    printf("%d\n", tree->value);
    free(tree);
  }
}

// =================== End of using the embedded stack =================================

// debugging function for embedded stack
void print_stack(STree stack) {
  printf("stack: [");
  while (stack) {
    printf("%d, ", stack->value);
    stack = stack->stack;
  }
  printf("]\n");
}

// =================== End of embedded stack ==========================================

int main() {
#if 0
  STree t =
    node(3,
      node(2, leaf(1), EMPTY),
      leaf(6));
#endif

  printf("\33[38;5;206m========== Original ========================\033[0m\n");
  STree t = EMPTY;
  insert(&t, 3);
  insert(&t, 2);
  insert(&t, 1);
  insert(&t, 6);
  assert(contains(&t, 2));
  assert(contains(&t, 1));
  assert(contains(&t, 3));
  assert(!contains(&t, 10));
  assert(!contains(&t, 0));
  print_stree(&t);

  printf("\n\33[38;5;206m========== Inserting 10 and 0 ==============\033[0m\n");
  insert(&t, 10);
  insert(&t, 0);
  assert(contains(&t, 10));
  assert(contains(&t, 0));
  print_stree(&t);

  printf("\n\33[38;5;206m========== Deleting 12, 6 and 3 ============\033[0m\n");
  delete (&t, 12);
  delete (&t, 3);
  delete (&t, 6);
  assert(!contains(&t, 3));
  assert(!contains(&t, 6));
  print_stree(&t);
  putchar('\n');
  clear_stree(&t);

  printf("\33[38;5;206m========== Tree from array =================\033[0m\n");
  int array[] = {1, 2, 3, 4, 6, 8, 10};
  int n = sizeof array / sizeof *array;
  STree *t2 = make_stree(n, array);
  print_stree(t2);
  putchar('\n');
  free_stree(t2);

  int array1[] = {4, 6, 8, 1, 2, 10, 3};
  n = sizeof array1 / sizeof *array1;
  t2 = make_stree(n, array1);
  print_stree(t2);
  putchar('\n');
  free_stree(t2);

  int array2[] = {
      3,
      2,
      1,
      6,
  };
  n = sizeof array2 / sizeof *array2;
  t2 = make_stree(n, array2);
  print_stree(t2);
  putchar('\n');
  free_stree(t2);
}
