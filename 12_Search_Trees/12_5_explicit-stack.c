// In 12_4_iterative.c `free_nodes()` and `print_stree()` still use the call stack which could overflow.
// We want to use a heap-allocated explicit stack for these functions.
// Explicit stacks implemented as singly linked lists:
//   - `freeframe`  used by `free_nodes()`
//   - `printstack` used by `print_stree()`

#include <assert.h>
#include <setjmp.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// --------------------------- Tree ------------------------------------------------------------- //

typedef struct node_
{
  int           value;
  struct node_ *left;
  struct node_ *right;
} node;

typedef node *stree;

#define EMPTY_TREE  NULL
#define EMPTY_STACK NULL

node *
new_node (int value, stree left, stree right)
{
  stree t = malloc (sizeof *t);
  if (t)
    {
      *t = (node){
        .value = value,
        .left  = left,
        .right = right,
      };
    }
  return t;
}

#define leaf(value) new_node (value, EMPTY_TREE, EMPTY_TREE)

stree *
new_stree (void)
{
  stree *t = malloc (sizeof (stree));
  if (t)
    {
      *t = NULL;
    }
  return t;
}

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

// Returns address of rightmost node pointer.
node **
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
          free (t); // free Node
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

// -------------------- Explicit Stack for Freeing Nodes ---------------------------------------- //

// Stack is a singly linked list of nodes.
typedef struct free_frame_
{
  node               *node;
  struct free_frame_ *next;
} free_frame;

void
print_freeframe_stack (free_frame **stack)
{
  printf ("free_frame stack: ");
  free_frame *top = *stack;
  while (top)
    {
      printf ("%d ", top->node->value);
      top = top->next;
    }
  printf ("\n");
}

// Pushes node on stack.
// `stack` represents the top of the stack.
// `stack` is passed as a pointer reference so we can set it to the newly allocated frame.
void
push_free (free_frame **stack, node *node)
{
  // don't push empty nodes
  if (!node)
    {
      return;
    }
  free_frame *frame = malloc (sizeof *frame);
  if (!frame)
    {
      abort (); // We do not tolerate errors!
    }
  *frame = (free_frame){
    .node = node,
    .next = *stack,
  };
  *stack = frame;
}

// Frees first tree in the stack and change stack to point to the next entry.
// Returns pointer to popped tree.
node *
pop_free (free_frame **stack)
{
  free_frame *top = *stack;
  node       *t   = top->node;
  *stack          = top->next;
  free (top);
  return t;
}

void
free_nodes (stree node)
{
  free_frame *stack = EMPTY_STACK;
  push_free (&stack, node);
  while (stack)
    {
      print_freeframe_stack (&stack);
      node = pop_free (&stack);
      push_free (&stack, node->left);
      push_free (&stack, node->right);
      free (node);
    }
}

// `tree` is stree*
#define clear_stree(tree)                                                                                              \
  do                                                                                                                   \
    {                                                                                                                  \
      free_nodes (*tree);                                                                                              \
      *tree = EMPTY_TREE;                                                                                              \
    }                                                                                                                  \
  while (0)

// `tree` is stree*
#define free_stree(tree)                                                                                               \
  do                                                                                                                   \
    {                                                                                                                  \
      free_nodes (*tree);                                                                                              \
      free (tree);                                                                                                     \
      tree = EMPTY_TREE;                                                                                               \
    }                                                                                                                  \
  while (0)

// -------------------- Tree Constructor -------------------------------------------------------- //

stree *
make_stree_from_array (int n, int array[n])
{
  stree *tree = new_stree ();
  if (!tree)
    {
      return NULL;
    }
  for (int i = 0; i < n; i++)
    {
      if (!insert (tree, array[i]))
        {
          free_stree (tree);
          return NULL;
        }
    }
  return tree;
}

// -------------------- Explicit Stack for Printing Trees --------------------------------------- //

typedef enum print_op
{
  LPAR,  // print '('
  TREE,  // print tree
  COMMA, // print ','
  VALUE, // print tree's value
  RPAR   // print')'
} printop;

// Singly linked list of nodes and print operations.
typedef struct print_frame
{
  printop             op;
  node               *node; // needed for TREE and VALUE printops
  struct print_frame *next;
} printframe;

// we will have only one instance of printstack
typedef struct print_stack
{
  printframe *frame; // print stack
  jmp_buf     env;
} printstack;

void
push_print (printstack *stack, printop op, node *node)
{
  printframe *frame = malloc (sizeof *frame);
  if (!frame)
    {
      longjmp (stack->env, 1); // go to error handler
    }
  *frame = (printframe){
    .op   = op,
    .node = node,
    .next = stack->frame,
  };
  stack->frame = frame;
}

// Add print operations for tree on the print stack.
void
schedule_print (printstack *stack, stree tree)
{
  // Stack is fifo → read this function from bottom to top.
  push_print (stack, RPAR, EMPTY_TREE);
  if (tree->right)
    {
      push_print (stack, TREE, tree->right);
    }
  push_print (stack, COMMA, EMPTY_TREE);
  push_print (stack, VALUE, tree);
  push_print (stack, COMMA, EMPTY_TREE);
  if (tree->left)
    {
      push_print (stack, TREE, tree->left);
    }
  push_print (stack, LPAR, EMPTY_TREE);
}

void
handle_print_op (printop op, node *node, printstack *stack)
{
  switch (op)
    {
    case LPAR:
      putchar ('[');
      break;
    case TREE:
      schedule_print (stack, node);
      break;
    case COMMA:
      putchar (',');
      break;
    case VALUE:
      printf ("%d", node->value);
      break;
    case RPAR:
      putchar (')');
      break;
    }
}

// `print_stree_` → `handle_print_op` → `schedule_print` → `push_print`.
// In `push_print` could be an allocation error; then it just jmp's back to print_stree_ going into the error section.
static void
print_stree_ (stree *tree)
{
  if (!*tree)
    {
      return;
    }

  printop    op;
  node      *n     = EMPTY_TREE;
  printstack stack = { .frame = EMPTY_STACK };

  // setup error handling
  if (setjmp (stack.env) != 0)
    {
      goto error;
    }

  schedule_print (&stack, *tree);
  while (stack.frame)
    {
      // pop from stack
      printframe *top = stack.frame;
      op              = top->op;
      n               = top->node;
      stack.frame     = top->next;
      free (top);
      handle_print_op (op, n, &stack);
    }
  return;

error:
  // Cleanup stack memory. We cannot undo the damage we have done to the output, though.
  // We have written parts of the tree, and then bailed.
  while (stack.frame)
    {
      printframe *top = stack.frame;
      stack.frame     = top->next;
      free (top);
    }
}

void
print_stree (stree *tree)
{
  print_stree_ (tree);
  printf ("\n");
}

// -------------------- Main -------------------------------------------------------------------- //

int
main ()
{

#if 0
  stree t =
    node(3,
      node(2, leaf(1), EMPTY_TREE),
      leaf(6));
#endif

  printf ("== Original ========================\n");
  stree t = EMPTY_TREE;
  insert (&t, 3);
  insert (&t, 2);
  insert (&t, 1);
  insert (&t, 6);
  assert (contains (&t, 1));
  assert (contains (&t, 2));
  assert (contains (&t, 3));
  assert (!contains (&t, 0));
  assert (!contains (&t, 10));
  print_stree (&t);

  printf ("== Inserting 10 and 0 ==============\n");
  insert (&t, 10);
  insert (&t, 0);
  assert (contains (&t, 0));
  assert (contains (&t, 10));
  print_stree (&t);

  printf ("== Deleting 12, 6 and 3 ============\n");
  delete (&t, 12);
  delete (&t, 3);
  delete (&t, 6);
  assert (!contains (&t, 3));
  assert (!contains (&t, 6));
  print_stree (&t);
  clear_stree (&t);

  printf ("== Tree from array =================\n");
  int    array[] = { 1, 2, 3, 4, 6, 8, 10 };
  int    n       = sizeof array / sizeof *array;
  stree *t2      = make_stree_from_array (n, array);
  print_stree (t2);
  free_stree (t2);

  int array1[] = { 4, 6, 8, 1, 2, 10, 3 };
  n            = sizeof array1 / sizeof *array1;
  t2           = make_stree_from_array (n, array1);
  print_stree (t2);
  free_stree (t2);
}
