#include "refcount.h"
#include "strees.h"    // just for debugging purposes

void*
rc_alloc(size_t size, CleanupFn cleanup)
{
  RefCount* rc = malloc(RCSIZE + size);
  if (!rc) return NULL;
  rc->rc       = 1;
  rc->cleanup  = cleanup;
  return user_mem(rc);
}

void*
incref(void* p)
{
  if (!p) return p;
  RefCount* rc = get_refcount(p);
  rc->rc++;
  return p;
}

void
cleanup(RefCount* stack)
{
  while (stack) {
    // empty node does not have a cleanup function
    if (stack->cleanup) stack->cleanup(user_mem(stack), stack);
    RefCount* next = stack->stack;
    free(stack);
    stack = next;
  }
}

// Debugging
#define get_node(p) (Node*)((char*)p + offsetof(RefCount, user_data))

void
print_stack(RefCount* ctx)
{
  printf("stack: ");
  while (ctx) {
    Node* n = get_node(ctx);
    printf("%p: %d, ", ctx, n->val);
    ctx = ctx->stack;
  }
  putchar('\n');
}

// p is pointer to user_data
// ctx->stack points to top of stack
// returns p or NULL in case memory was freed
void*
decref_ctx(void* p, void* ctx)
{
  printf("ctx: %p\n", ctx);
  if (!p) return p;
  RefCount* rc = get_refcount(p);
  if (--rc->rc == 0) {
    if (ctx) {
      RefCount* stack        = ctx;
                rc->stack    = stack->stack;  // push on stack
                stack->stack = rc;
      print_stack(ctx);
    } else {
      rc->stack = NULL;  // empty the stack
      cleanup(rc);
    }
    return NULL;
  }
  return p;
}
