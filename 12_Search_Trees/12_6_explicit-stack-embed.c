// "If you want to traverse a tree, and you don't want to allocate new memory doing it,
// you have several options. A general option you always have, regardless of whether we
// consider trees or other objects, is to set aside memory for a stack in your data structures."
//
// Here we use `free_nodes()` as an example. In the old version `push_free()` used `malloc()`.
//
// Diagrams: assets/1-free_nodes-embedded-stack.jpg
//           assets/2-free_nodes-embedded-stack.jpg
//           assets/3-free_nodes-embedded-stack.jpg

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// --------------------------- Tree ------------------------------------------------------------- //

// Every pushed node provides stack memory. We don't need to allocate it with malloc().
typedef struct node_
{
  int           value;
  struct node_ *left;
  struct node_ *right;
  struct node_ *stack; // stack memory
} node_t;

typedef node_t *stree;

#define EMPTY_TREE  NULL
#define EMPTY_STACK NULL

stree
new_node (int value, node_t *left, node_t *right)
{
  stree t = malloc (sizeof *t);

  if (t)
    {
      *t = (node_t){
        .value = value,
        .left  = left,
        .right = right,
        .stack = EMPTY_STACK,
      };
    }

  return t;
}

#define leaf(value) new_node (value, EMPTY_TREE, EMPTY_TREE)

static void
print_stree_ (stree *tree)
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
print_stree (stree *tree)
{
  print_stree_ (tree);
  printf ("\n");
}

stree *
new_stree (void)
{
  stree *t = malloc (sizeof (stree));

  if (t)
    {
      *t = EMPTY_TREE;
    }

  return t;
}

void free_nodes (node_t *node);

#define clear_stree(t)                                                                                                 \
  do                                                                                                                   \
    {                                                                                                                  \
      free_nodes (*t);                                                                                                 \
      *t = EMPTY_TREE;                                                                                                 \
    }                                                                                                                  \
  while (0)

#define free_stree(t)                                                                                                  \
  do                                                                                                                   \
    {                                                                                                                  \
      free_nodes (*t);                                                                                                 \
      free (t);                                                                                                        \
      t = EMPTY_TREE;                                                                                                  \
    }                                                                                                                  \
  while (0)

stree *
find_loc (stree *tree, int val)
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
contains (stree *tree, int val)
{
  return !!*find_loc (tree, val);
}

bool
insert (stree *tree, int val)
{
  stree *target = find_loc (tree, val);
  if (*target)
    {
      return true;
    }
  else
    {
      return !!(*target = leaf (val));
    }
}

stree *
rightmost (stree *tree)
{
  assert (tree && *tree);
  if (!(*tree)->right)
    {
      return tree;
    }
  else
    {
      return rightmost (&(*tree)->right);
    }
}

void delete (stree *tree, int val)
{
  stree *target = find_loc (tree, val);
  if (*target)
    {
      stree t = *target;
      if (!(t->left && t->right))
        {
          *target = t->left ? t->left : t->right;
          free (t);
        }
      else
        {
          stree *rm_ref = rightmost (&t->left);
          stree  rm     = *rm_ref;
          t->value      = rm->value;
          *rm_ref       = rm->left;
          free (rm);
        }
    }
}

stree *
make_stree_from_array (int n, int array[n])
{
  stree *t = new_stree ();
  if (!t)
    {
      return EMPTY_TREE;
    }
  for (int i = 0; i < n; i++)
    {
      if (!insert (t, array[i]))
        {
          free_stree (t);
          return EMPTY_TREE;
        }
    }
  return t;
}

// ------------------- Embedded Stack ----------------------------------------------------------- //

// We don't allocate new memory but instead use the existing one in the node structure.
// We use the `stack` field of the pushed node as stack memory instead of a `malloc()`.
void
push (node_t **stack, node_t *node)
{
  // don't push empty nodes
  if (node)
    {
      node->stack = *stack; // embedded stack points to a Node
      *stack      = node;   // stack points to tree
    }
}

// Copy next stack value into 'stack' and remove top of stack by setting it to EMPTY_STACK.
node_t *
pop (node_t **stack)
{
  stree top  = *stack;
  *stack     = top->stack;
  top->stack = EMPTY_STACK; // not necessary but cleaner/more logical
  return top;
}

void
free_nodes (node_t *node)
{
  node_t *stack = EMPTY_STACK; // stack is the top of the stack in the tree
  push (&stack, node);
  while (stack)
    {
      node = pop (&stack);
      push (&stack, node->left);
      push (&stack, node->right);
      free (node);
    }
}

// ------------------- End of Using the Embedded Stack ---------------------------------

// Debugging function for embedded stack.
void
print_stack (node_t *stack)
{
  printf ("stack: [");
  while (stack)
    {
      printf ("%d, ", stack->value);
      stack = stack->stack;
    }
  printf ("]\n");
}

// ---------------------------- Main --------------------------------------------------

#define ARRAY_SIZE(a) (sizeof a / sizeof *a)

int
main ()
{

#if 0
  STree t =
    node(3,
      node(2, leaf(1), EMPTY_TREE),
      leaf(6));
#endif

  {
    stree t;

    {
      printf ("== Original ================================\n");
      t = EMPTY_TREE;
      insert (&t, 3);
      insert (&t, 2);
      insert (&t, 1);
      insert (&t, 6);
      assert (contains (&t, 1));
      assert (contains (&t, 2));
      assert (contains (&t, 3));
      assert (!contains (&t, 10));
      assert (!contains (&t, 0));
      print_stree (&t);
    }

    {
      printf ("== Inserting 10 and 0 ======================\n");
      insert (&t, 10);
      insert (&t, 0);
      assert (contains (&t, 0));
      assert (contains (&t, 10));
      print_stree (&t);
    }
    {
      printf ("== Deleting 12, 6 and 3 ====================\n");
      delete (&t, 12);
      delete (&t, 3);
      delete (&t, 6);
      assert (!contains (&t, 3));
      assert (!contains (&t, 6));
      print_stree (&t);
      clear_stree (&t);
    }
  }

  {
    int    n;
    stree *t;

    printf ("== Tree from array =========================\n");
    int array[] = { 1, 2, 3, 4, 6, 8, 10 };
    n           = ARRAY_SIZE (array);
    t           = make_stree_from_array (n, array);
    print_stree (t);
    free_stree (t);

    int array1[] = { 4, 6, 8, 1, 2, 10, 3 };
    n            = ARRAY_SIZE (array1);
    t            = make_stree_from_array (n, array1);
    print_stree (t);
    free_stree (t);

    int array2[] = { 3, 2, 1, 6 };
    n            = ARRAY_SIZE (array2);
    t            = make_stree_from_array (n, array2);
    print_stree (t);
    free_stree (t);
  }
}
