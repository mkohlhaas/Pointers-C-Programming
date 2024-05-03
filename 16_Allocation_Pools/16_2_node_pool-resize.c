#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// --------------------------------- Data Structures ------------------------------------------------- //

typedef struct node
{
  struct node *left;
  struct node *right;
  int          val;
} node;

typedef struct subpool
{
  struct subpool *next_subpool;
  node            nodes[];
} subpool;

typedef struct nodepool
{
  size_t   capacity;
  size_t   used;
  subpool *subpool;
} nodepool;

// ---------------------------------- Code ----------------------------------------------------------- //

// `next_subpool` is an existing subpool or NULL.
subpool *
new_subpool (size_t capacity, subpool *next_subpool)
{
  subpool *new_sub_pool = NULL;
  size_t   size         = offsetof (subpool, nodes) + (sizeof *new_sub_pool->nodes) * capacity;
  new_sub_pool          = malloc (size);
  if (new_sub_pool)
    {
      new_sub_pool->next_subpool = next_subpool;
    }
  printf ("Size and capacity of new subpool: %3zu , %2zu\n", size, capacity);
  return new_sub_pool;
}

// Creates new `nodepool` with initial `capacity`.
nodepool *
new_node_pool (size_t capacity)
{
  nodepool *pool = malloc (sizeof *pool);
  if (!pool)
    {
      return NULL;
    }

  subpool *new_sub_pool = new_subpool (capacity, NULL);
  if (!new_sub_pool)
    {
      free (pool);
      return NULL;
    }

  pool->capacity = capacity;
  pool->used     = 0;
  pool->subpool  = new_sub_pool;

  return pool;
}

void
free_node_pool (nodepool *pool)
{
  subpool *sp = pool->subpool;
  while (sp)
    {
      subpool *next = sp->next_subpool;
      free (sp);
      sp = next;
    }
  free (pool);
}

// Get node from nodepool.
node *
node_alloc (nodepool *pool)
{
  if (pool->used == pool->capacity)
    {
      // add a pool twice as large as the current largest pool - if we can.
      if (SIZE_MAX / 2 / sizeof (node) < pool->capacity)
        {
          return NULL;
        }
      size_t   capacity     = 2 * pool->capacity;
      subpool *new_sub_pool = new_subpool (capacity, pool->subpool);
      if (!new_sub_pool)
        {
          return NULL;
        }

      // Success, so add new pool to list
      pool->capacity = capacity;
      pool->used     = 0;
      pool->subpool  = new_sub_pool;
    }

  return &pool->subpool->nodes[pool->used++];
}

// ---------------------------------- Main ----------------------------------------------------------- //

int
main ()
{
  int offset    = offsetof (subpool, nodes);
  int node_size = sizeof (node);
  printf ("Offset of first node in subpool: %d\n", offset);
  printf ("Size of node:                    %d\n", node_size);
  printf ("Size of nodepool:                %d + %d * capacity\n\n", offset, node_size);

  nodepool *pool;
  pool     = new_node_pool (3);
  node *n1 = node_alloc (pool);
  node *n2 = node_alloc (pool);
  assert (n2 - n1 == 1);
  node *n3 = node_alloc (pool);
  assert (n3 - n2 == 1);
  node *n4 = node_alloc (pool); // will create a new subpool
  assert (n4 - n3 != 1);        // technically they could sit like that, but it is highly unlikely …
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
  node *n10 = node_alloc (pool); // will create a new subpool
  assert (n10 - n9 != 1);        // technically they could sit like that, but it is highly unlikely …

  free_node_pool (pool);
}
