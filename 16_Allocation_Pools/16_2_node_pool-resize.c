#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct node
{
  struct node *left;
  struct node *right;
  int          val;
} node;

typedef struct subpool
{
  struct subpool *next;
  node            nodes[];
} subpool;

subpool *
new_subpool (size_t capacity, subpool *next)
{
  subpool *pool = NULL;
  size_t   size = offsetof (subpool, nodes) + (sizeof *pool->nodes) * capacity;
  pool          = malloc (size);
  if (pool)
    {
      pool->next = next;
    }
  printf ("Size and capacity of new subpool: %zu, %zu\n", size, capacity);
  return pool;
}

typedef struct nodepool
{
  size_t   top_capacity;
  size_t   top_used;
  subpool *subpools;
} nodepool;

nodepool *
new_pool (size_t init_capacity)
{
  nodepool *pool = malloc (sizeof *pool);
  if (!pool)
    {
      return NULL;
    }

  subpool *subpool = new_subpool (init_capacity, NULL);
  if (!subpool)
    {
      free (pool);
      return NULL;
    }

  pool->top_capacity = init_capacity;
  pool->top_used     = 0;

  return pool;
}

node *
node_alloc (nodepool *pool)
{
  if (pool->top_used == pool->top_capacity)
    {
      // We want to resize by adding a pool twice as large as the
      // current largest pool. But first check if we can.
      if (SIZE_MAX / 2 / sizeof (node) < pool->top_capacity)
        {
          return NULL;
        }
      size_t   new_size = 2 * pool->top_capacity;
      subpool *new_sub  = new_subpool (new_size, pool->subpools);
      if (!new_sub)
        {
          return NULL;
        }

      // Success, so add new pool to list
      pool->subpools     = new_sub;
      pool->top_capacity = new_size;
      pool->top_used     = 0;
    }

  return &pool->subpools->nodes[pool->top_used++];
}

void
free_pool (nodepool *pool)
{
  subpool *sp = pool->subpools;
  while (sp)
    {
      subpool *next = sp->next;
      free (sp);
      sp = next;
    }
  free (pool);
}

int
main ()
{
  nodepool *pool;
  pool     = new_pool (3);
  node *n1 = node_alloc (pool);
  node *n2 = node_alloc (pool);
  assert (n2 - n1 == 1);
  node *n3 = node_alloc (pool);
  assert (n3 - n2 == 1);
  node *n4 = node_alloc (pool);
  assert (n4 - n3 != 1); // technically they could sit like that, but it is highly unlikely …
  node *n5 = node_alloc (pool);
  assert (n5 - n4 == 1);
  node *n6 = node_alloc (pool);
  assert (n6 - n5 == 1);
  node *n7 = node_alloc (pool);
  assert (n7 - n6 == 1);
  node *n8 = node_alloc (pool);
  assert (n8 - n7 == 1);
  node *n9 = node_alloc (pool);
  assert (n9 - n8 == 1);
  node *n10 = node_alloc (pool);
  assert (n10 - n9 != 1); // technically they could sit like that, but it is highly unlikely …

  free_pool (pool);
}
