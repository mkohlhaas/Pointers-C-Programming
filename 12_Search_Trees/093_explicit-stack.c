// two singly linked lists: search for FreeFrame and PrintStack

#include <assert.h>
#include <setjmp.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// Declarations
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
new_stree(void)
{
  STree*  t = malloc(sizeof *t);  // allocate memory for a pointer
  if (t) *t = NULL;
  return  t;
}

// Explicit Stack - singly linked list of STree's 
typedef struct free_frame FreeFrame;

typedef struct free_frame
{
  STree      tree;
  FreeFrame* next;
} FreeFrame;

// push tree on stack
// stack is passed as a pointer reference so we can change it
void
push(FreeFrame** stack, STree tree)
{
  if (!tree) return;
  FreeFrame* frame = malloc(sizeof *frame);  // allocate a FreeFrame structure
  if (!frame) abort();                       // we do not tolerate errors!
  *frame = (FreeFrame){ .tree = tree, .next = *stack };
  *stack = frame;
}

// pass stack as a pointer reference
// free first tree in the stack and change stack to point to the next entry
// return pointer to popped tree
STree
pop(FreeFrame** stack)
{
  FreeFrame*  top   = *stack;
  STree       t     = top->tree;
             *stack = top->next;
  free(top);
  return t;
}

void
free_nodes(STree tree)
{
  FreeFrame* stack = NULL;
  push(&stack, tree);
  while (stack) {
    tree = pop(&stack);
    push(&stack, tree->left);
    push(&stack, tree->right);
    printf("%d ", tree->value);
    free(tree);
  }
  putchar('\n');
}

// tree is STree*
#define clear_stree(tree)                                                      \
  do {                                                                         \
    free_nodes(*tree);                                                         \
    *tree = EMPTY;                                                             \
  } while (0)

// tree is STree*
#define free_stree(tree)                                                       \
  do {                                                                         \
    free_nodes(*tree);                                                         \
    free(tree);                                                                \
    tree = EMPTY;                                                              \
  } while (0)

// tail recursive
STree*
find_loc(STree* tree, int val)
{
  assert(tree);
  if      (!*tree || val == (*tree)->value)    return tree;
  else if           (val <  (*tree)->value)    return find_loc(&(*tree)->left,  val);
  else           /* (val >  (*tree)->value) */ return find_loc(&(*tree)->right, val);
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
  if   (*target) return true;                     // we have already been here
  else           return !!(*target = leaf(val));
}

// tail recursive
STree*
rightmost(STree* tree)
{
  assert(tree && *tree);
  if   (!(*tree)->right) return tree;
  else                   return rightmost(&(*tree)->right);
}

void delete(STree* tree, int val)
{
  STree* target = find_loc(tree, val);
  if (*target) {
    STree t = *target;
    if (!(t->left && t->right)) {
      *target = t->left ? t->left : t->right;
      free(t);  // free Node
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
  STree* tree = new_stree();
  if (!tree) return NULL;
  for (int i = 0; i < n; i++)
    if (!insert(tree, array[i])) {
      free_stree(tree);
      return NULL;
    }
  return tree;
}

typedef enum print_op
{
  LPAR,
  TREE,
  COMMA,
  VALUE,
  RPAR
} PrintOp;

// singly linked list of STree's with print operations
typedef struct print_frame PrintFrame;

typedef struct print_frame
{
  PrintOp     op;
  STree       tree;
  PrintFrame* next;
} PrintFrame;

typedef struct print_stack
{
  PrintFrame* frame;
  jmp_buf     env;
} PrintStack;

void
print_push(PrintStack* stack, PrintOp op, STree tree)
{
  PrintFrame* frame = malloc(sizeof *frame);
  if (!frame) longjmp(stack->env, 1); // bail; restore cpu registers (see setjmp)
  *frame = (PrintFrame){ .op = op, .tree = tree, .next = stack->frame };
  stack->frame = frame;
}

// add print operations for tree on the print stack
void
schedule_print(PrintStack* stack, STree tree)
{
  // stack is first-in first-out; read this function from bottom to top
  print_push(stack, RPAR, EMPTY);
  if (tree->right) print_push(stack, TREE, tree->right);
  print_push(stack, COMMA, EMPTY);
  print_push(stack, VALUE, tree);
  print_push(stack, COMMA, EMPTY);
  if (tree->left) print_push(stack, TREE, tree->left);
  print_push(stack, LPAR, EMPTY);
}

void
handle_print_op(PrintOp op, STree tree, PrintStack* stack)
{
  if     (op == LPAR)  putchar('[');
  else if(op == RPAR)  putchar(']');
  else if(op == COMMA) putchar(',');
  else if(op == VALUE) printf("%d", tree->value);
  else if(op == TREE)  schedule_print(stack, tree);
}

// print_stree -> handle_print_op -> schedule_print -> print_push
// in print_push could be an allocation error; then it just jmp's back
// to print_stree going into the error section.
void
print_stree(STree* tree)
{
  if (!*tree) return;

  PrintOp    op;
  STree      n     = EMPTY;
  PrintStack stack = { .frame = NULL };
  if (setjmp(stack.env) != 0) goto error;  // save cpu registers (p.395)

  schedule_print(&stack, *tree);
  while (stack.frame) {
    PrintFrame* top         = stack.frame;
                op          = top->op;
                n           = top->tree;
                stack.frame = top->next;
    free(top);
    handle_print_op(op, n, &stack);
  }
  return;

error:
  // Cleanup stack memory
  // We cannot undo the damage we have done to the output, though.
  // We have written parts of the tree, and then bailed...
  while (stack.frame) {
    PrintFrame* top         = stack.frame;
                stack.frame = top->next;
    free(top);
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

  printf("Original:\n");
  STree t = EMPTY;
  insert(&t, 3);
  insert(&t, 2);
  insert(&t, 1);
  insert(&t, 6);
  assert (contains(&t,  2));
  assert (contains(&t,  1));
  assert (contains(&t,  3));
  assert(!contains(&t, 10));
  assert(!contains(&t,  0));
  print_stree(&t);

  printf("\n============================================================\n");

  printf("Inserting 10 and 0:\n");
  insert(&t, 10);
  insert(&t,  0);
  assert(contains(&t, 10));
  assert(contains(&t,  0));
  print_stree(&t);

  printf("\n============================================================\n");

  printf("Deleting 12:\n");
  delete(&t, 12);
  print_stree(&t);

  printf("\n============================================================\n");

  printf("Deleting 3:\n");
  delete(&t, 3);
  print_stree(&t);

  printf("\n============================================================\n");

  printf("Deleting 6:\n");
  delete(&t, 6);
  assert(!contains(&t, 3));
  assert(!contains(&t, 6));
  print_stree(&t);
  putchar('\n');
  clear_stree(&t);

  printf("============================================================\n");

  printf("Tree from array:\n");
  int    array[] = { 1, 2, 3, 4, 6, 8, 10 };
  int    n       = sizeof array / sizeof *array;
  STree* t2      = make_stree(n, array);
  print_stree(t2);
  putchar('\n');
  free_stree(t2);

  int array1[] = { 4, 6, 8, 1, 2, 10, 3};
      n        = sizeof array1 / sizeof *array1;
      t2       = make_stree(n, array1);
  print_stree(t2);
  putchar('\n');
  free_stree(t2);
}
