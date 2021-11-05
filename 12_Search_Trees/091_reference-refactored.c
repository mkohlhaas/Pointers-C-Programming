// new function find_loc()

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

// tail recursive
// find tree - return address of an STree - where val is or should be (e.g. if inserted)
STree*
find_loc(STree* t, int val)
{
  assert(t);
  if       (!*t || val == (*t)->value)     return t;
  else if         (val <  (*t)->value)     return find_loc(&(*t)->left,  val);
  else         /* (val >  (*t)->value) */  return find_loc(&(*t)->right, val);
}

bool
contains(STree* t, int val)
{
  return !!*find_loc(t, val);
}

bool
insert(STree* t, int val)
{
  STree* target = find_loc(t, val);
  if   (*target) return true; // already there
  else           return !!(*target = leaf(val));
}

STree*
new_stree(void)
{
  STree*  t = malloc(sizeof *t);
  if (t) *t = NULL;
  return  t;
}

// Not tail recursive
void
free_nodes(Node* n)
{
  if (!n) return;
  free_nodes(n->left);
  free_nodes(n->right);
  free(n);
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

// tail recursive
STree*
rightmost(STree* t)
{
  assert(t && *t);
  if   (!(*t)->right) return t;
  else                return rightmost(&(*t)->right);
}

void
delete(STree* t, int val)
{
  STree* target = find_loc(t, val);
  if (*target) {
    STree t = *target;
    if (!(t->left && t->right)) {
      *target = t->left ? t->left : t->right;
      free(t);
    } else {
      STree* rm_ref   = rightmost(&t->left);
      STree  rm       = *rm_ref;
             t->value = rm->value;
            *rm_ref   = rm->left;
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
    if (!insert(t, array[i])) {
      free_stree(t);
      return NULL;
    }
  return t;
}

// Not tail recursive
void
print_stree(STree* t)
{
  if (!*t) {
    putchar('_');
  } else {
    putchar('[');
    print_stree(&(*t)->left);
    putchar(',');
    printf("%d", (*t)->value);
    putchar(',');
    print_stree(&(*t)->right);
    putchar(']');
  }
}

int
main()
{
#if 0
  STree t =
    node(3,
      node(2, leaf(1), EMPTY),
      leaf(6));
#endif

  printf("\33[38;5;206m========== Original ========================\033[0m\n");
  STree  t_ = EMPTY;
  STree* t  = &t_;
  insert(t, 3);
  insert(t, 2);
  insert(t, 1);
  insert(t, 6);
  assert( contains(t,  2));
  assert( contains(t,  1));
  assert( contains(t,  3));
  assert(!contains(t, 10));
  assert(!contains(t,  0));
  print_stree(t);

  printf("\n\33[38;5;206m========== Inserting 10 and 0 ==============\033[0m\n");
  insert(t, 10);
  insert(t,  0);
  assert(contains(t, 10));
  assert(contains(t,  0));
  print_stree(t);

  printf("\n\33[38;5;206m========== Deleting 12, 6 and 3 ============\033[0m\n");
  delete(t, 12);
  delete(t,  6);
  delete(t,  3);
  assert(!contains(t, 3));
  assert(!contains(t, 6));
  print_stree(t);
  clear_stree(t);

  printf("\n\33[38;5;206m========== Tree from array =================\033[0m\n");
  int    array[] = { 1, 2, 3, 4, 6, 8, 10 };
  int    n       = sizeof array / sizeof *array;
  STree* t2      = make_stree(n, array);
  print_stree(t2);
  putchar('\n');
  free_stree(t2);

  int array1[]   = { 4, 6, 8, 1, 2, 10, 3};
      n          = sizeof array1 / sizeof *array1;
      t          = make_stree(n, array1);
  print_stree(t);
  putchar('\n');
  free_stree(t);
}
