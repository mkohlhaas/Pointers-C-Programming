#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// ---------------------------- Data Definitions ------------------------------------------------ //

// just a test data structure - will not be filled with actual data in this example code
typedef struct node
{
  struct node *left;
  struct node *right;
  int          val;
} node;

// is basically a node; but if not used will contain a pointer to next free node

typedef union node_free_list
{
  union node_free_list *next;
  node                  node;
} node_free_list;

// linked list of subpools; each subpool contains an array of nodes
typedef struct subpool
{
  struct subpool *next;
  node_free_list  nodes[]; // flexible array member must be at end of struct
} subpool;

// a list of subpools and free nodes
typedef struct node_pool
{
  size_t          top_capacity; // capacity of biggest subpool
  node_free_list *next_free;
  subpool        *subpools;
} node_pool;

// ----------------------------- Code ----------------------------------------------------------- //

subpool *
new_subpool (size_t capacity, subpool *next)
{
  subpool *sub_pool = NULL;
  size_t   size     = offsetof (subpool, nodes) + (sizeof *sub_pool->nodes) * capacity;
  sub_pool          = malloc (size);
  if (!sub_pool)
    {
      return NULL;
    }
  sub_pool->next = next;
  // setup the next pointers in the array
  for (size_t i = 0; i < capacity - 1; i++)
    {
      sub_pool->nodes[i].next = &sub_pool->nodes[i + 1];
    }
  sub_pool->nodes[capacity - 1].next = NULL;
  return sub_pool;
}

node_pool *
new_pool (size_t init_capacity)
{
  node_pool *pool = malloc (sizeof *pool);
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
  pool->subpools     = subpool;
  pool->top_capacity = init_capacity;
  pool->next_free    = subpool->nodes;
  return pool;
}

node *
node_alloc (node_pool *pool)
{
  if (pool->next_free == NULL)
    {
      // add a pool twice as large as the current largest pool - if we can.
      if (SIZE_MAX / 2 / sizeof (node) < pool->top_capacity)
        {
          return NULL;
        }
      size_t   new_size     = 2 * pool->top_capacity;
      subpool *new_sub_pool = new_subpool (new_size, pool->subpools);
      if (!new_sub_pool)
        {
          return NULL;
        }
      // add new sub pool to pool
      pool->top_capacity = new_size;
      pool->next_free    = new_sub_pool->nodes;
    }
  node *node1     = (node *)pool->next_free;
  pool->next_free = pool->next_free->next;
  return node1;
}

void
free_node (node_pool *pool, node *node)
{
  node_free_list *free_list;
  free_list       = (node_free_list *)node;
  free_list->next = pool->next_free;
  pool->next_free = free_list;
}

void
free_pool (node_pool *pool)
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

// ----------------------------- Main ----------------------------------------------------------- //

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

  free_node (pool, n2);
  free_node (pool, n10);
  node *reused10 = node_alloc (pool);
  assert (n10 == reused10);
  node *reused2 = node_alloc (pool);
  assert (n2 == reused2);

  free_pool (pool);
}
