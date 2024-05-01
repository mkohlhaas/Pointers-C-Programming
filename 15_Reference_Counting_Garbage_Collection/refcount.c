#include "refcount.h"
#include "strees.h"
#include <stdio.h>

void *
rc_alloc (size_t size, cleanup_fn cleanup)
{
  rc_struct *rc = malloc (RCSIZE + size);
  if (!rc)
    {
      return NULL;
    }
  rc->rc      = 1;
  rc->cleanup = cleanup;
  return user_mem (rc);
}

void *
incref (void *p)
{
  if (!p)
    {
      return p;
    }

  rc_struct *rc = get_rc_struct (p);
  rc->rc++;
  return p;
}

// Calls user-defined cleanup_fn and frees refcounter.
void
cleanup (rc_struct *stack)
{
  // NULL is the end marker for the stack
  while (stack)
    {
      if (stack->cleanup)
        {
          stack->cleanup (user_mem (stack), stack);
        }
      rc_struct *next = stack->stack;
      free (stack);
      stack = next;
    }
}

// -- Debugging ---------------------------------------------------------------------------------------------  //

#define get_node(p) (node *)((char *)p + offsetof (rc_struct, user_data))

void
print_stack (rc_struct *stack)
{
  printf ("stack: ");
  while (stack)
    {
      node *n = get_node (stack);
      printf ("%d (@ %p) ", n->val, stack);
      stack = stack->stack;
    }
  puts ("");
}

// ----------------------------------------------------------------------------------------------------------  //

// decref (e.g. in main) → decref_ctx → cleanup() → user-defined cleanup_fn, e.g. free_node() → decref_ctx()

// `p` is pointer to user_data.
// `ctx` points to top of stack.
// Returns `p` or …
// `NULL` in case memory was freed.
void *
decref_ctx (void *p, void *prev_ctx)
{
  // printf ("user data: %p, ctx: %p\n", p, ctx);

  if (!p)
    {
      return p;
    }

  rc_struct *current = get_rc_struct (p);
  if (--current->rc == 0)
    {
      if (!prev_ctx) // decref calls decref_ctx with NULL as prev_ctx
        {
          // start deletion/cleanup process
          current->stack = NULL; // NULL is the end marker for the stack.
          cleanup (current);     // cleanup is only called from here
        }
      else
        {
          // we are already in deletion/cleanup process
          rc_struct *prev = prev_ctx;
          current->stack  = prev->stack; // push on stack marking for deletion
          prev->stack     = current;
          print_stack (prev_ctx);
        }
      return NULL;
    }
  return p;
}
