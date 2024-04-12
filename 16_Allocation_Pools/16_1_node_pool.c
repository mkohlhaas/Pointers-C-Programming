#include <assert.h>
#include <stdlib.h>

typedef struct node Node;

typedef struct node
{
  Node *left;
  Node *right;
  int   val;
} Node;

typedef struct node_pool
{
  size_t size;
  size_t used;
  Node  *pool;
} NodePool;

NodePool *
new_pool (size_t capacity)
{
  NodePool *pool = malloc (sizeof *pool);
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

Node *
node_alloc (NodePool *pool)
{
  if (pool->used == pool->size)
    {
      return NULL; // Pool is used up
    }
  return &pool->pool[pool->used++];
}

void
free_pool (NodePool *pool)
{
  free (pool->pool);
  free (pool);
}

int
main ()
{
  NodePool *pool;
  pool     = new_pool (3);
  Node *n1 = node_alloc (pool);
  Node *n2 = node_alloc (pool);
  assert (n2 - n1 == 1);
  Node *n3 = node_alloc (pool);
  assert (n3 - n2 == 1);
  Node *n4 = node_alloc (pool);
  assert (n4 == NULL);

  free_pool (pool);
}
