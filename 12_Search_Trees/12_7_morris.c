#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// ------------------------- Tree  ------------------------------------------------- //

typedef struct node
{
  int          value;
  struct node *left;
  struct node *right;
} node_t;

typedef node_t *STree;

#define EMPTY_STREE NULL
#define EMPTY_NODE  NULL

int allocated;

STree
node (int value, STree left, STree right)
{
  STree t = malloc (sizeof *t);
  if (t)
    {
      allocated++;
      *t = (node_t){
        .value = value,
        .left  = left,
        .right = right,
      };
    }
  return t;
}

#define leaf(value) node (value, EMPTY_STREE, EMPTY_STREE)

STree *
new_stree ()
{
  STree *t = malloc (sizeof (node_t));
  if (t)
    {
      *t = NULL;
    }
  return t;
}

static STree *
rightmost_to (STree *tree, node_t *n)
{
  assert (tree && *tree);

  if ((*tree)->right == NULL || (*tree)->right == n)
    {
      return tree; // `tree` is the rightmost tree as there is no right subtree or it's a threaded pointer.
    }
  else
    {
      return rightmost_to (&(*tree)->right, n);
    }
}

#define rightmost(t) rightmost_to (t, EMPTY_NODE)

// Cp. with 12_4_iterative.c.
// Freeing nodes without recursion and memory allocation.
// Only nodes with empty left pointer but right pointer are deleted.
void
free_nodes (node_t *n)
{
  node_t *curr = n;
  while (curr)
    {
      if (!curr->left)
        {
          // free current node and recurse right
          node_t *right = curr->right;
          free (curr);
          allocated--;
          curr = right;
        }
      else
        { // marks current node for deletion → has no left sub-tree but right pointer
          (*rightmost (&curr->left))->right = curr;       // set threaded pointer (= right pointer)
          node_t *left                      = curr->left; // save left pointer before we change it
          curr->left                        = EMPTY_NODE; // we won't recurse into left sub-tree again
          curr                              = left;       // recurse left
        }
    }
}

// t is STree*
#define clear_stree(t)                                                                                                 \
  do                                                                                                                   \
    {                                                                                                                  \
      free_nodes (*t);                                                                                                 \
      *t = EMPTY_STREE;                                                                                                \
    }                                                                                                                  \
  while (0)

// t is STree*
#define free_stree(t)                                                                                                  \
  do                                                                                                                   \
    {                                                                                                                  \
      free_nodes (*t);                                                                                                 \
      free (t);                                                                                                        \
      t = NULL;                                                                                                        \
    }                                                                                                                  \
  while (0)

STree *
find_loc (STree *tree, int val)
{
  assert (tree);
  if (!*tree || val == (*tree)->value)
    {
      return tree;
    }
  else if (val < (*tree)->value)
    {
      return find_loc (&(*tree)->left, val);
    }
  else
    {
      return find_loc (&(*tree)->right, val);
    }
}

bool
contains (STree *tree, int val)
{
  return !!*find_loc (tree, val);
}

bool
insert (STree *tree, int val)
{
  STree *target = find_loc (tree, val);
  if (*target)
    {
      return true;
    }
  else
    {
      return !!(*target = leaf (val));
    }
}

void delete (STree *tree, int val)
{
  STree *target = find_loc (tree, val);
  if (*target)
    {
      STree t = *target;
      if (!(t->left && t->right))
        {
          *target = t->left ? t->left : t->right;
          free (t);
          allocated--;
        }
      else
        {
          STree *rm_ref = rightmost (&t->left);
          STree  rm     = *rm_ref;
          t->value      = rm->value;
          *rm_ref       = rm->left;
          free (rm);
          allocated--;
        }
    }
}

STree *
make_stree_from_array (int n, int array[n])
{
  STree *t = new_stree ();
  if (!t)
    {
      return NULL;
    }
  for (int i = 0; i < n; i++)
    {
      if (!insert (t, array[i]))
        {
          free_stree (t);
          return NULL;
        }
    }
  return t;
}

static void
print_stree_ (STree *tree)
{
  if (!*tree)
    {
      putchar ('_');
    }
  else
    {
      putchar ('[');
      print_stree_ (&(*tree)->left);
      putchar (',');
      printf ("%d", (*tree)->value);
      putchar (',');
      print_stree_ (&(*tree)->right);
      putchar (']');
    }
}

void
print_stree (STree *tree)
{
  print_stree_ (tree);
  printf ("\n");
}

// Morris traversal simulates an inorder traversal.
void
inorder (node_t *n)
{
  if (!n)
    {
      return;
    }
  inorder (n->left);
  printf ("%d ", n->value);
  inorder (n->right);
}

void
morris (STree *tree)
{
  STree curr = *tree;
  while (curr)
    {
      if (!curr->left)
        {                     // no left sub-tree → visit node and go right
          printf ("%d ", curr->value);
          curr = curr->right; // follows normal right pointer or threaded pointer
        }
      else
        {                     // left sub-tree → find rightmost node of left tree to mark it
          // with a threaded pointer to ourself (the current node)
          STree pred = *rightmost_to (&curr->left, curr);
          assert (pred->right == EMPTY_STREE || pred->right == curr); // this is how rightmost_to works
          if (pred->right == EMPTY_STREE)
            {
              pred->right = curr;       // set threaded pointer (marks end of recursion for current node)
              curr        = curr->left; // keep on recursing left
            }
          else
            {
              // we found the mark previously set → we followed the threaded pointer
              // It's as if we came back from "recursion".
              printf ("%d ", curr->value); // visit node
              pred->right = EMPTY_STREE;   // remove threaded pointer (our mark)
              curr        = curr->right;   // recurse right
            }
        }
    }
}

// ------------------------- Iterators --------------------------------------------- //

typedef struct stree_iter
{
  node_t *curr;
} StreeIter;

StreeIter
stree_iter (STree *t)
{
  return (StreeIter){ .curr = *t };
}

// returns next node or NULL if there are no nodes left
node_t *
next_node (StreeIter *iter)
{
  node_t *curr = iter->curr;
  while (curr)
    {
      if (!curr->left)
        {
          iter->curr = curr->right;
          return curr; // Yield node
        }
      else
        {
          STree pred = *rightmost_to (&curr->left, curr);
          assert (pred->right == EMPTY_STREE || pred->right == curr);
          if (pred->right == EMPTY_STREE)
            {
              pred->right = curr;
              curr        = curr->left;
            }
          else
            {
              iter->curr  = curr->right;
              pred->right = EMPTY_STREE;
              return curr; // Yield node
            }
        }
    }
  return NULL;
}

void
cleanup_iter (StreeIter *iter)
{
  while (next_node (iter))
    {
      ;
    }
}

// ------------------------- Main -------------------------------------------------- //

int
main ()
{
  printf ("========== Tree from array ===================\n");
  int    array[] = { 1, 2, 13, 4, 16, 8, 10 };
  int    n       = sizeof array / sizeof *array;
  STree *t       = make_stree_from_array (n, array);
  print_stree (t);

  printf ("========== Inorder ===========================\n");
  inorder (*t);

  printf ("\n========== Morris ============================\n");
  morris (t);

  printf ("\n========== Iterator ==========================\n");
  StreeIter iter = stree_iter (t);
  node_t   *node = NULL;
  while ((node = next_node (&iter)))
    {
      printf ("%d ", node->value);
    }
  cleanup_iter (&iter);

  printf ("\n========== Free ==============================\n");
  free_stree (t);
  t = make_stree_from_array (n, array);
  print_stree (t);
  free_stree (t);

  printf ("==============================================\n");
  printf ("net allocations = %d\n", allocated);
}
