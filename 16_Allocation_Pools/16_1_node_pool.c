#include <assert.h>
#include <stdlib.h>

typedef struct node
{
  struct node *left;
  struct node *right;
  int          val;
} node;

typedef struct node_pool
{
  size_t size;
  size_t used;
  node  *pool;
} node_pool;

node_pool *
new_pool (size_t capacity)
{
  node_pool *pool = malloc (sizeof *pool);
  if (!pool)
    {
      return NULL;
    }

  pool->pool = malloc (capacity * sizeof *pool->pool);
  if (!pool->pool)
    {
      free (pool);
      return NULL;
    }
  pool->size = capacity;
  pool->used = 0;
  return pool;
}

node *
node_alloc (node_pool *pool)
{
  if (pool->used == pool->size)
    {
      return NULL; // pool is used up
    }
  return &pool->pool[pool->used++];
}

void
free_pool (node_pool *pool)
{
  free (pool->pool);
  free (pool);
}

int
main ()
{
  node_pool *pool;
  pool     = new_pool (3);
  node *n1 = node_alloc (pool);
  node *n2 = node_alloc (pool);
  assert (n2 - n1 == 1);
  node *n3 = node_alloc (pool);
  assert (n3 - n2 == 1);
  node *n4 = node_alloc (pool);
  assert (n4 == NULL);

  free_pool (pool);
}
