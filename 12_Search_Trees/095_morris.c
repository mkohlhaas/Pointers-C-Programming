// search for morris, inorder and Iterators

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

#define EMPTY_STREE NULL
#define EMPTY_NODE  NULL

int allocated;

STree
node(int value, STree left, STree right)
{
  STree t = malloc(sizeof *t);
  if (t) {
    allocated++;
    *t = (Node){ .value = value, .left = left, .right = right };
  }
  return t;
}

#define leaf(value) node(value, EMPTY_STREE, EMPTY_STREE)

STree*
new_stree(void)
{
  STree*  t = malloc(sizeof *t);
  if (t) *t = NULL;
  return  t;
}

STree*
rightmost_to(STree* tree, Node* n)
{
  assert(tree && *tree);
  if   ((*tree)->right == NULL || (*tree)->right == n)  return tree; // there is no right subtree or we have been here before
  else                                                  return rightmost_to(&(*tree)->right, n);
}

#define rightmost(t) rightmost_to(t, EMPTY_NODE)

// cp. with 092_iterative.c
void
free_nodes(Node* n)
{
  Node* curr = n;
  while (curr) {
    if (!curr->left) {
      Node* right = curr->right;
      free(curr);
      allocated--;
      curr = right;
    } else {
      // Add pointer so we can go back
      (*rightmost(&curr->left))->right = curr;
      // Recurse left, but make sure we never go left again
      Node* left       = curr->left;
            curr->left = EMPTY_NODE;  // never recurse into the left subtree again
            curr       = left;
    }
  }
}

// t is STree*
#define clear_stree(t)                                                         \
  do {                                                                         \
    free_nodes(*t);                                                            \
    *t = EMPTY_STREE;                                                          \
  } while (0)

// t is STree*
#define free_stree(t)                                                          \
  do {                                                                         \
    free_nodes(*t);                                                            \
    free(t);                                                                   \
    t = NULL;                                                                  \
  } while (0)

// tail recursive
STree*
find_loc(STree* tree, int val)
{
  assert(tree);
  if      (!*tree || (*tree)->value == val) return tree;
  else if (val < (*tree)->value)            return find_loc(&(*tree)->left,  val);
  else                                      return find_loc(&(*tree)->right, val);
}

bool
contains(STree* tree, int val)
{
  return !!*find_loc(tree, val);
}

bool
insert(STree* tree, int val)
{
  STree* target = find_loc(tree, val);
  if   (*target) return true; // already there
  else           return !!(*target = leaf(val));
}

void delete(STree* tree, int val)
{
  STree* target = find_loc(tree, val);
  if (*target) {
    STree t = *target;
    if (!(t->left && t->right)) {
      *target = t->left ? t->left : t->right;
      free(t);
      allocated--;
    } else {
      STree* rm_ref   = rightmost(&t->left);
      STree  rm       = *rm_ref;
             t->value = rm->value;
             *rm_ref  = rm->left;
      free(rm);
      allocated--;
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
void
print_stree(STree* tree)
{
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

void
inorder(Node* n)
{
  if (!n) return;
  inorder(n->left);
  printf("%d ", n->value);
  inorder(n->right);
}

void
morris(STree* tree)
{
  STree curr = *tree;
  while (curr) {
    if (!curr->left) {
      printf("1-%d ", curr->value);
      curr = curr->right;
    } else {
      STree pred = *rightmost_to(&curr->left, curr);
      assert(pred->right == EMPTY_STREE || pred->right == curr);
      if (pred->right == EMPTY_STREE) {  // first time we are here
        pred->right = curr;
        curr        = curr->left;
      } else {  // we have here been before
        printf("2-%d ", curr->value);
        pred->right = EMPTY_STREE;  // undo rightmost node
        curr        = curr->right;
      }
    }
  }
}

// ========================= Iterators =========================

typedef struct stree_iter
{
  Node* curr;
} StreeIter;

StreeIter
stree_iter(STree* t)
{
  return (StreeIter){ .curr = *t };
}

// returns next node or NULL if there are no nodes left
Node*
next_node(StreeIter* iter)
{
  Node* curr = iter->curr;
  while (curr) {
    if (!curr->left) {
      iter->curr = curr->right;
      return curr; // Yield node
    } else {
      STree pred = *rightmost_to(&curr->left, curr);
      assert(pred->right == EMPTY_STREE || pred->right == curr);
      if (pred->right == EMPTY_STREE) {
        pred->right = curr;
        curr        = curr->left;
      } else {
        iter->curr  = curr->right;
        pred->right = EMPTY_STREE;
        return curr; // Yield node
      }
    }
  }
  return NULL;
}

void
cleanup_iter(StreeIter* iter)
{
  // Run to completion to clean up
  while (next_node(iter)) { }
}

int
main()
{
#if 0
  STree t =
    node(3,
      node(2, leaf(1), EMPTY_NODE),
      leaf(6));
#endif

  printf("Original:\n");
  STree t = EMPTY_STREE;
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

  printf("\n============================================================\n");

  printf("Inserting 10 and 0:\n");
  insert(&t, 10);
  insert(&t, 0);
  assert(contains(&t, 10));
  assert(contains(&t, 0));
  print_stree(&t);

  printf("\n============================================================\n");

  printf("Deleting 12, 3 and 6:\n");
  delete(&t, 12);
  delete(&t, 3);
  delete(&t, 6);
  assert(!contains(&t, 3));
  assert(!contains(&t, 6));
  print_stree(&t);

  printf("\n============================================================\n");

  clear_stree(&t);

  printf("Tree from array:\n");
  int    array[] = { 1, 2, 13, 4, 16, 8, 10 };
  int    n       = sizeof array / sizeof *array;
  STree* t2      = make_stree(n, array);
  print_stree(t2);

  printf("\n============================================================\n");

  printf("Inorder:\n");
  inorder(*t2);

  printf("\n============================================================\n");

  printf("Morris:\n");
  morris(t2);

  printf("\n============================================================\n");

  printf("Iterator:\n");
  StreeIter iter = stree_iter(t2);
  Node*     node = NULL;
  while ((node = next_node(&iter))) printf("%d ", node->value);
  cleanup_iter(&iter);

  printf("\n============================================================\n");

  free_stree(t2);
  assert(t2 == NULL);
  t2 = make_stree(n, array);
  print_stree(t2);
  // same as free_stree(t2); but it's a macro that also sets t2 to NULL
  free_nodes(*t2);
  free(t2);

  printf("\n============================================================\n");

  printf("allocated = %d\n", allocated);
}
