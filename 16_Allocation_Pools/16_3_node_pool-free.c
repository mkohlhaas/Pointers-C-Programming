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

// basically a node; but if a node is not used, will contain a pointer to the next free node
typedef union node_free_list
{
  union node_free_list *next_free_node;
  node                  node;
} node_free_list;

// linked list of subpools; each subpool contains an array of nodes
typedef struct subpool
{
  struct subpool *next_subpool; // linked list of subpools
  node_free_list  nodes[];      // flexible array member must be at end of struct
} subpool;

// a list of subpools and free nodes
typedef struct nodepool
{
  size_t          capacity;       // capacity of subpool
  node_free_list *next_free_node; // points to first free node in subpool
  subpool        *subpool;        // points to begin of subpool linked list
} nodepool;

// ---------------------------------- Code ----------------------------------------------------------- //

// `next_subpool` is an existing subpool or NULL.
subpool *
new_subpool (size_t capacity, subpool *next_subpool)
{
  subpool *new_sub_pool = NULL;
  size_t   size         = offsetof (subpool, nodes) + (sizeof *new_sub_pool->nodes) * capacity;
  new_sub_pool          = malloc (size);
  if (!new_sub_pool)
    {
      return NULL;
    }
  new_sub_pool->next_subpool = next_subpool;

  // nodes are not used; setup the `next_free_node` pointers as a linked list
  for (size_t i = 0; i < capacity - 1; i++)
    {
      new_sub_pool->nodes[i].next_free_node = &new_sub_pool->nodes[i + 1];
    }
  new_sub_pool->nodes[capacity - 1].next_free_node = NULL; // NULL marks end of linked list

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

  pool->capacity       = capacity;
  pool->next_free_node = new_sub_pool->nodes;
  pool->subpool        = new_sub_pool;

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
  if (pool->next_free_node == NULL)
    {
      // add a pool twice as large as the current largest pool - if we can.
      if (SIZE_MAX / 2 / sizeof (node) < pool->capacity)
        {
          return NULL;
        }
      size_t   new_size     = 2 * pool->capacity;
      subpool *new_sub_pool = new_subpool (new_size, pool->subpool);
      if (!new_sub_pool)
        {
          return NULL;
        }
      // add new sub pool to pool
      pool->capacity       = new_size;
      pool->next_free_node = new_sub_pool->nodes;
      pool->subpool        = new_sub_pool;
    }
  node *node1          = (node *)pool->next_free_node;
  pool->next_free_node = pool->next_free_node->next_free_node;
  return node1;
}

void
free_node (nodepool *pool, node *node)
{
  node_free_list *free_list = (node_free_list *)node;
  free_list->next_free_node = pool->next_free_node;
  pool->next_free_node      = free_list;
}

// ---------------------------------- Main ----------------------------------------------------------- //

int
main ()
{
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

  free_node (pool, n2);
  free_node (pool, n10);
  node *reused10 = node_alloc (pool);
  assert (n10 == reused10);
  node *reused2 = node_alloc (pool);
  assert (n2 == reused2);

  free_node_pool (pool);
}
