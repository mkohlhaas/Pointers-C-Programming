#include "dynarray.h"
#include "list.h"
#include <assert.h>
#include <stdio.h>

typedef void *ImplStack; // pointer to the data structure; as it could be anything, e.g. a list, a dynamic array, we use
                         // a void pointer
typedef int Elem;        // stacks of integers

// a list of functions a certain implementation has to implement/provide, e.g. a list stack, a dynamic array stack
typedef struct {
  ImplStack (*new_stack)(void);
  bool (*empty_stack)(ImplStack);
  Elem (*top)(ImplStack);
  bool (*push)(ImplStack, Elem);
  Elem (*pop)(ImplStack);
  void (*free_stack)(ImplStack);
} StackType;

// this is the central structure that we get when calling new_stack() and is a parameter for every stack function, eg.
// stack_pop(), stack_push(), ...
typedef struct stack_ {
  ImplStack implementation; // Data Structure: singly linked list, doubly linked list, dynamic array, ...
  StackType *type;          // Functions:      implementation of the stack API: top, push, pop, ...
} Stack_;

typedef Stack_ *Stack;

// ----------------------------------------------------
// ---------------- Generic Stack API -----------------
// ----------------------------------------------------
//
// -------- Functions used by the stack user ----------

// The generic functions call the implementation specific ones.

// in type are all the implementation specific functions
Stack new_stack(StackType *type) {
  ImplStack impl_stack = type->new_stack(); // will be a list, a dynamic array, ...
  Stack stack = malloc(sizeof *stack);
  if (!impl_stack || !stack)
    goto error;

  stack->implementation = impl_stack;
  stack->type = type;
  return stack;

error:
  free(stack);
  if (impl_stack)
    type->free_stack(impl_stack);
  return NULL;
}

// in stack are all the implementation specific functions and the used data structure
// these functions basically just forward to the implementations specific ones
bool empty_stack(Stack stack) { return stack->type->empty_stack(stack->implementation); }

Elem stack_top(Stack stack) { return stack->type->top(stack->implementation); }

bool stack_push(Stack stack, Elem elem) { return stack->type->push(stack->implementation, elem); }

Elem stack_pop(Stack stack) { return stack->type->pop(stack->implementation); }

void free_stack(Stack stack) {
  stack->type->free_stack(stack->implementation);
  free(stack);
}

// ----------------------------------------------------
// -------------------- List Stack --------------------
// ----------------------------------------------------
ImplStack list_stack_new() {
  return new_list(); // this will return a List but we return a generic pointer
}

bool list_stack_empty(ImplStack stack) { return is_empty((List)stack); }

Elem list_stack_top(ImplStack stack) { return front((List)stack)->value; }

bool list_stack_push(ImplStack stack, Elem elem) { return prepend(stack, elem); }

Elem list_stack_pop(ImplStack stack) {
  Elem elem = front((List)stack)->value;
  delete_link(front((List)stack));
  return elem;
}

void list_stack_free(ImplStack stack) { free_list(stack); }

StackType list_stack = {.new_stack = list_stack_new,
                        .empty_stack = list_stack_empty,
                        .top = list_stack_top,
                        .push = list_stack_push,
                        .pop = list_stack_pop,
                        .free_stack = list_stack_free};

// --------------------------------------------------------
// -------------------- Dynarray Stack --------------------
// --------------------------------------------------------
ImplStack da_stack_new() {
  Dynarray *da = malloc(sizeof *da);
  if (!da)
    return NULL;
  if (!da_init(da, 1, 0)) {
    free(da);
    return NULL;
  }
  return da; // this will return a dynamic array but we return a generic pointer
}

bool da_stack_empty(ImplStack stack) { return ((Dynarray *)stack)->used == 0; }

Elem da_stack_top(ImplStack stack) {
  Dynarray *da = stack;
  assert(da->used > 0);
  return da->data[da->used - 1];
}

bool da_stack_push(ImplStack stack, Elem elem) { return da_append(stack, elem); }

Elem da_stack_pop(ImplStack stack) {
  Dynarray *da = stack;
  assert(da->used > 0);
  return da->data[--(da->used)];
}

void da_stack_free(ImplStack stack) {
  da_dealloc(stack);
  free(stack);
}

StackType da_stack = {.new_stack = da_stack_new,
                      .empty_stack = da_stack_empty,
                      .top = da_stack_top,
                      .push = da_stack_push,
                      .pop = da_stack_pop,
                      .free_stack = da_stack_free};

// --------------------------------------------------------
// -- Common test function using generic stack functions --
// --------------------------------------------------------
void test(StackType *type) {
  Stack stack = new_stack(type);
  for (int i = 0; i < 5; i++)
    stack_push(stack, i);
  while (!empty_stack(stack))
    printf("%d\n", stack_pop(stack));
  free_stack(stack);
}

// --------------------------------------------------------
// ------------------------ main --------------------------
// --------------------------------------------------------
int main() {
  // we use both types of stacks the same way
  printf("List Stack:\n");
  test(&list_stack);

  printf("\nDynamic array:\n");
  test(&da_stack);
}
