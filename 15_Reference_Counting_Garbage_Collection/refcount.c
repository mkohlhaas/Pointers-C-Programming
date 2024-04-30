#include "refcount.h"
#include "strees.h"

void *
rc_alloc (size_t size, cleanup_fn cleanup)
{
  refcount *rc = malloc (RCSIZE + size);
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
  refcount *rc = get_refcount (p);
  rc->rc++;
  return p;
}

void
cleanup (refcount *stack)
{
  while (stack)
    {
      // empty node does not have a cleanup function
      if (stack->cleanup)
        {
          stack->cleanup (user_mem (stack), stack);
        }
      refcount *next = stack->stack;
      free (stack);
      stack = next;
    }
}

// debugging

#define get_node(p) (node *)((char *)p + offsetof (refcount, user_data))

void
print_stack (refcount *ctx)
{
  printf ("stack: ");
  while (ctx)
    {
      node *n = get_node (ctx);
      printf ("%p: %d, ", ctx, n->val);
      ctx = ctx->stack;
    }
  putchar ('\n');
}

// p is pointer to user_data
// ctx->stack points to top of stack
// returns p or NULL in case memory was freed
void *
decref_ctx (void *p, void *ctx)
{
  printf ("ctx: %p\n", ctx);
  if (!p)
    {
      return p;
    }
  refcount *rc = get_refcount (p);
  if (--rc->rc == 0)
    {
      if (ctx)
        {
          refcount *stack = ctx;
          rc->stack       = stack->stack; // push on stack
          stack->stack    = rc;
          print_stack (ctx);
        }
      else
        {
          rc->stack = NULL; // empty the stack
          cleanup (rc);
        }
      return NULL;
    }
  return p;
}
