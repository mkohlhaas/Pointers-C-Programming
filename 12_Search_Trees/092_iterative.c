// turn recursive functions into loops; search for while loops
// see rightmost(), find_loc(), free_nodes()
// print_stree() has been replaced by print_stree_indent()

// To replace recursion with a loop, we make a loop condition that should
// terminate the loop when we would directly return from the recursion’s base case.
// The inversion of the base case condition will usually do.
// Update the function values in the loop.

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct node  Node;
typedef        Node* STree;

typedef struct node
{
  int   value;
  STree left;
  STree right;
} Node;

#define EMPTY NULL

STree
node(int value, STree left, STree right)
{
  STree   t = malloc(sizeof *t);
  if (t) *t = (Node){ .value = value, .left = left, .right = right };
  return  t;
}

#define leaf(value) node(value, EMPTY, EMPTY)

STree*
rightmost(STree* t)
{
  while ((*t)->right) t = &(*t)->right;
  return t;
}

// in-order traversal of the tree
// explained in paragraph "Morris Traversal", p. 399
// esp. paragraph "Freeing Nodes Without Recursion and Memory Allocation", p. 403
void
free_nodes(Node* n)
{
  Node* curr = n;
  while (curr) {
    if (!curr->left) {
      Node* right = curr->right;
      printf("%d ", curr->value);
      free(curr);                             // only deallocation point
      curr = right;
    } else {
      STree pred  = *rightmost(&curr->left);
      pred->right = curr;                     // create thread pointer
      Node* left  = curr->left;
      curr->left  = EMPTY;                    // never recurse into the left subtree again
      curr        = left;
    }
  }
  putchar('\n');
}

STree*
new_stree(void)
{
  STree*  t = malloc(sizeof *t);
  if (t) *t = NULL;
  return  t;
}

#define clear_stree(t)                                                         \
  do {                                                                         \
    free_nodes(*t);                                                            \
    *t = NULL;                                                                 \
  } while (0)

#define free_stree(t)                                                          \
  do {                                                                         \
    free_nodes(*t);                                                            \
    free(t);                                                                   \
    t = NULL;                                                                  \
  } while (0)

STree*
find_loc(STree* t, int val)
{
  while  (*t && val != (*t)->value) {
    if         (val < (*t)->value)     t = &(*t)->left;   // update function values; here just t
    else    /* (val > (*t)->value) */  t = &(*t)->right;
  }
  return t;
}

bool
contains(STree* t, int val)
{
  return !!*find_loc(t, val);
}

bool
insert(STree* t, int val)
{
  STree* loc = find_loc(t, val);
  if   (*loc) return true; // we have been already here
  else        return !!(*loc = leaf(val));
}

void delete(STree* t, int val)
{
  STree* loc = find_loc(t, val);

  if (*loc) {
    STree t = *loc;
    if (!(t->left && t->right)) {
      *loc = t->left ? t->left : t->right;
      free(t);
    } else {
      STree* rm_ref   = rightmost(&t->left);
      STree  rm       = *rm_ref;
             t->value = rm->value;
             *rm_ref  = rm->left;
      free(rm);
    }
  }
}

STree*
make_stree(int n, int array[n])
{
  STree* t = new_stree();
  if (!t) return NULL;
  for (int i = 0; i < n; i++)
    if (!insert(t, array[i])) { free_stree(t); return NULL; }
  return t;
}

// Not tail recursive
// not in use any more
void
print_stree(STree* t)
{
  if (!*t) {
    putchar('_');
  }
  else {
    putchar('[');
    print_stree(&(*t)->left);
    putchar(',');
    printf("%d", (*t)->value);
    putchar(',');
    print_stree(&(*t)->right);
    putchar(']');
  }
}

#define indent(I)                                                              \
  do {                                                                         \
    for (int i = 0; i < I; i++)                                                \
      putchar(' ');                                                            \
  } while (0)

void
print_stree_indent(STree* t, int indent)
{
  indent(indent);
  if (!*t)
    printf("[_]\n");
  else {
    printf("[%d]\n", (*t)->value);
    // printf("[%d] <%p>\n", (*t)->value, (void*)*t);
    print_stree_indent(&(*t)->left,  indent + 2);
    print_stree_indent(&(*t)->right, indent + 2);
  }
}

#define print_stree(t) print_stree_indent(t, 0)

int
main()
{
#if 0
    STree t =
      node(3,
        node(2, leaf(1), NULL),
        leaf(6));
#endif
  printf("\33[38;5;206m========== Original ========================\033[0m\n");
  STree t = EMPTY;
  insert(&t, 3);
  insert(&t, 2);
  insert(&t, 1);
  insert(&t, 6);
  assert( contains(&t,  2));
  assert( contains(&t,  1));
  assert( contains(&t,  3));
  assert(!contains(&t, 10));
  assert(!contains(&t,  0));
  print_stree(&t);

  printf("\n\33[38;5;206m========== Inserting 10 and 0 ==============\033[0m\n");
  insert(&t, 10);
  insert(&t,  0);
  assert(contains(&t, 10));
  assert(contains(&t,  0));
  print_stree(&t);

  printf("\n\33[38;5;206m========== Deleting 12, 6 and 3 ============\033[0m\n");
  delete(&t, 12);
  delete(&t,  3);
  delete(&t,  6);
  assert(!contains(&t, 3));
  assert(!contains(&t, 6));
  print_stree(&t);
  clear_stree(&t);

  printf("\n\33[38;5;206m========== Tree from array =================\033[0m\n");
  int    array[] = { 1, 2, 13, 4, 16, 8, 10 };
  int    n       = sizeof array / sizeof *array;
  STree* t2      = make_stree(n, array);
  print_stree(t2);
  free_stree(t2);
  putchar('\n');

  int array1[] = { 4, 6, 8, 1, 2, 10, 3};
      n        = sizeof array1 / sizeof *array1;
      t2       = make_stree(n, array1);
  print_stree(t2);
  free_stree(t2);
  putchar('\n');

  int array2[] = { 4, 1, 5, };
      n        = sizeof array2 / sizeof *array2;
      t2       = make_stree(n, array2);
  print_stree(t2);
  free_stree(t2);
  putchar('\n');

  // this array is used in 095_morris.c where one node is deleted in the second free() case: printf("2-%d ", curr->value);
  int array3[] = { 1, 2, 13, 4, 16, 8, 10 };
      n        = sizeof array3 / sizeof *array3;
      t2       = make_stree(n, array3);
  print_stree(t2);
  free_stree(t2);
  putchar('\n');
}
