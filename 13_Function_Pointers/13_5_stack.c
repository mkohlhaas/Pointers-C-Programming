#include "dynarray.h"
#include "list.h"
#include <assert.h>
#include <stdio.h>

// ---------------- Typedefs ------------------------------------------------------------ //

// implementation (data structure)
typedef void *stack_impl; // (generic void) pointer to a data structure implementing a stack (list, dynamic array,…)
typedef int   elem_t;     // type of stack elements

// API
typedef struct
{
  stack_impl (*new) (void);
  elem_t (*top) (stack_impl);
  bool (*push) (stack_impl, elem_t);
  elem_t (*pop) (stack_impl);
  bool (*empty) (stack_impl);
  void (*free) (stack_impl);
} stack_api;

typedef struct stack
{
  stack_impl implementation;
  stack_api *api;
} *stack;

// ---------------- Generic Stack API --------------------------------------------------- //
// ---------- generic functions are delegated to specific ones -------------------------- //

stack
stack_new (stack_api *stack_api)
{
  stack_impl stack_impl = stack_api->new ();
  stack      stack      = malloc (sizeof *stack);
  if (!stack_impl || !stack)
    {
      goto error;
    }

  stack->implementation = stack_impl;
  stack->api            = stack_api;
  return stack;

error:
  free (stack);
  if (stack_impl)
    {
      stack_api->free (stack_impl);
    }
  return NULL;
}

bool
stack_empty (stack stack)
{
  return stack->api->empty (stack->implementation);
}

elem_t
stack_top (stack stack)
{
  return stack->api->top (stack->implementation);
}

bool
stack_push (stack stack, elem_t elem)
{
  return stack->api->push (stack->implementation, elem);
}

elem_t
stack_pop (stack stack)
{
  return stack->api->pop (stack->implementation);
}

void
stack_free (stack stack)
{
  stack->api->free (stack->implementation);
  free (stack);
}

// -------------------- List Stack ------------------------------------------------------ //

stack_impl
list_stack_new ()
{
  return new_list ();
}

bool
list_stack_empty (stack_impl stack)
{
  return is_empty ((List)stack);
}

elem_t
list_stack_top (stack_impl stack)
{
  return front ((List)stack)->value;
}

bool
list_stack_push (stack_impl stack, elem_t elem)
{
  return prepend (stack, elem);
}

elem_t
list_stack_pop (stack_impl stack)
{
  elem_t elem = front ((List)stack)->value;
  delete_link (front ((List)stack));
  return elem;
}

void
list_stack_free (stack_impl stack)
{
  free_list (stack);
}

stack_api list_stack = { .new   = list_stack_new,
                         .empty = list_stack_empty,
                         .top   = list_stack_top,
                         .push  = list_stack_push,
                         .pop   = list_stack_pop,
                         .free  = list_stack_free };

// -------------------- Dynamic Array Stack --------------------------------------------- //

stack_impl
da_stack_new ()
{
  Dynarray *da = malloc (sizeof *da);
  if (!da)
    {
      return NULL;
    }
  if (!da_init (da, 1, 0))
    {
      free (da);
      return NULL;
    }
  return da;
}

bool
da_stack_empty (stack_impl stack)
{
  return ((Dynarray *)stack)->used == 0;
}

elem_t
da_stack_top (stack_impl stack)
{
  Dynarray *da = stack;
  assert (da->used > 0);
  return da->data[da->used - 1];
}

bool
da_stack_push (stack_impl stack, elem_t elem)
{
  return da_append (stack, elem);
}

elem_t
da_stack_pop (stack_impl stack)
{
  Dynarray *da = stack;
  assert (da->used > 0);
  return da->data[--(da->used)];
}

void
da_stack_free (stack_impl stack)
{
  da_dealloc (stack);
  free (stack);
}

stack_api da_stack = { .new   = da_stack_new,
                       .empty = da_stack_empty,
                       .top   = da_stack_top,
                       .push  = da_stack_push,
                       .pop   = da_stack_pop,
                       .free  = da_stack_free };

// -------------------- Testing --------------------------------------------------------- //

void
test (stack_api *stack_api)
{
  stack stack = stack_new (stack_api);
  printf ("Is newly created stack empty? %s\n", stack_empty (stack) ? "✓" : "✗");
  puts ("Pushing some values on the stack …");

  for (int i = 1; i <= 15; i++)
    {
      stack_push (stack, i);
    }

  printf ("Is stack still empty? %s\n", stack_empty (stack) ? "✓" : "✗");
  printf ("Top of stack: %d\n", stack_top (stack));
  puts ("Popping all values from the stack and printing its values …");

  while (!stack_empty (stack))
    {
      printf ("%d\n", stack_pop (stack));
    }

  printf ("Is stack empty again? %s\n", stack_empty (stack) ? "✓" : "✗");
  stack_free (stack);
}

// ------------------------ Main -------------------------------------------------------- //

int
main ()
{
  {
    printf ("\n== List Stack ==============================================\n\n");
    test (&list_stack);
  }

  {
    printf ("\n== Dynamic array ===========================================\n\n");
    test (&da_stack);
  }
}
