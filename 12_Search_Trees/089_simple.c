#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct node  Node;
typedef        Node* STree;

typedef struct node
{
  int    value;
  STree  left;
  STree  right;
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

// tail recursive
bool
contains(STree t, int val)
{
  if      (!t)                 return false;
  else if (val <  t->value)    return contains(t->left,  val);
  else if (val >  t->value)    return contains(t->right, val);
  else /* (val == t->value) */ return true;
}

#if 0
// Not tail recursive, and we don't handle allocation errors
STree insert(STree t, int val)
{
  if (!t)             return leaf(val); // can fail, but we don't handle it
  if (val < t->value) t->left  = insert(t->left, val);
  else                t->right = insert(t->right, val);
  return t;
}
#else
// Not tail recursive
STree
insert_node(STree t, Node* n)
{
  if (!t) return n;
  if      (n->value == t->value) free(n);                                         // cannot have several nodes of the same value; t unchanged
  else if (n->value <  t->value) t->left  = insert_node(t->left,  n);
  else if (n->value >  t->value) t->right = insert_node(t->right, n);
  return t;
}

STree
insert(STree t, int val)
{
  STree n = leaf(val); if (!n) return NULL;
  return insert_node(t, n);
}
#endif

STree
make_stree(int n, int array[n])
{
  STree t = EMPTY;
  for (int i = 0; i < n; i++) t = insert(t, array[i]);
  return t;
}

// tail recursive
int
rightmost_val(STree t)
{
  assert(t);
  if   (!t->right) return t->value;
  else             return rightmost_val(t->right);
}

// not tail recursive
STree
delete(STree t, int val)
{
  if (!t) return t;
  if      (val <  t->value)     t->left  = delete(t->left,  val);
  else if (val >  t->value)     t->right = delete(t->right, val);
  else /* (val == t->value) */  { if (t->left && t->right) {                      // both subtrees exist
                                  t->value = rightmost_val(t->left);
                                  t->left  = delete(t->left, t->value);
                                } else {                                          // at most one subtree (0 or 1) exist
                                  STree subtree = t->left ? t->left : t->right;
                                  free(t);
                                  return subtree;
                                }}
  return t;
}

// Not tail recursive
void
print_stree(STree t)
{
  if (!t) {
    putchar('_');
  } else {
    putchar('[');
    print_stree(t->left);
    putchar(',');
    printf("%d", t->value);
    putchar(',');
    print_stree(t->right);
    putchar(']');
  }
}

// Not tail recursive
void
free_stree(STree t)
{
  if (!t) return;
  free_stree(t->left);
  free_stree(t->right);
  free(t);
}

int
main()
{
  printf("\33[38;5;206m========== Original ========================\033[0m\n");
  STree t = node(3, node(2, leaf(1), EMPTY), leaf(6));
  print_stree(t);

  printf("\n\33[38;5;206m========== Inserting 10 and 0 ==============\033[0m\n");
  t = insert(t, 10);
  t = insert(t,  0);
  print_stree(t);

  printf("\n\33[38;5;206m========== Delete 12 and 3 =================\033[0m\n");
  t = delete(t, 12);
  t = delete(t,  3);
  print_stree(t);
  free_stree(t);

  printf("\n\33[38;5;206m========== Tree from array =================\033[0m\n");
  int array[]  = { 1, 2, 3, 4, 6, 8, 10 };
  int n = sizeof array / sizeof *array;
  t = make_stree(n, array);
  print_stree(t); putchar('\n');
  free_stree(t);

  int array1[] = { 4, 6, 8, 1, 2, 10, 3};
  n = sizeof array1 / sizeof *array1;
  t = make_stree(n, array1);
  print_stree(t); putchar('\n');
  free_stree(t);
}
