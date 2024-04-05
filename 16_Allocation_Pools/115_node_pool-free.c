#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// ============================ Data Definitions ===============================

// just a test data structure - will not be filled with actual data in this example code
typedef struct node Node;

typedef struct node {
  Node *left;
  Node *right;
  int val;
} Node;

// is basically a node; but if not used will contain a pointer to next free node
typedef union node_free_list NodeFreeList;

typedef union node_free_list {
  NodeFreeList *next;
  Node node;
} NodeFreeList;

// linked list of subpools; each subpool contains an array of nodes
typedef struct subpool SubPool;

typedef struct subpool {
  SubPool *next;
  NodeFreeList nodes[]; // flexible array member must be at end of struct
} SubPool;

// a list of subpools and free nodes
typedef struct node_pool {
  size_t top_capacity; // capacity of biggest subpool
  NodeFreeList *next_free;
  SubPool *subpools;
} NodePool;

// ============================= Code ==========================================

SubPool *new_subpool(size_t capacity, SubPool *next) {
  SubPool *sub_pool = NULL;
  size_t size = offsetof(SubPool, nodes) + (sizeof *sub_pool->nodes) * capacity;
  sub_pool = malloc(size);
  if (!sub_pool)
    return NULL;
  sub_pool->next = next;
  // setup the next pointers in the array
  for (size_t i = 0; i < capacity - 1; i++)
    sub_pool->nodes[i].next = &sub_pool->nodes[i + 1];
  sub_pool->nodes[capacity - 1].next = NULL;
  return sub_pool;
}

NodePool *new_pool(size_t init_capacity) {
  NodePool *pool = malloc(sizeof *pool);
  if (!pool)
    return NULL;
  SubPool *subpool = new_subpool(init_capacity, NULL);
  if (!subpool) {
    free(pool);
    return NULL;
  }
  pool->subpools = subpool;
  pool->top_capacity = init_capacity;
  pool->next_free = subpool->nodes;
  return pool;
}

Node *node_alloc(NodePool *pool) {
  if (pool->next_free == NULL) {
    // add a pool twice as large as the current largest pool - if we can.
    if (SIZE_MAX / 2 / sizeof(Node) < pool->top_capacity)
      return NULL;
    size_t new_size = 2 * pool->top_capacity;
    SubPool *new_sub_pool = new_subpool(new_size, pool->subpools);
    if (!new_sub_pool)
      return NULL;
    // add new sub pool to pool
    pool->top_capacity = new_size;
    pool->next_free = new_sub_pool->nodes;
  }
  Node *node = (Node *)pool->next_free;
  pool->next_free = pool->next_free->next;
  return node;
}

void free_node(NodePool *pool, Node *node) {
  NodeFreeList *free_list;
  free_list = (NodeFreeList *)node;
  free_list->next = pool->next_free;
  pool->next_free = free_list;
}

void free_pool(NodePool *pool) {
  SubPool *sp = pool->subpools;
  while (sp) {
    SubPool *next = sp->next;
    free(sp);
    sp = next;
  }
  free(pool);
}

// ============================= Main ==========================================

int main() {
  NodePool *pool;
  pool = new_pool(3);
  Node *n1 = node_alloc(pool);
  Node *n2 = node_alloc(pool);
  assert(n2 - n1 == 1);
  Node *n3 = node_alloc(pool);
  assert(n3 - n2 == 1);
  Node *n4 = node_alloc(pool);
  assert(n4 - n3 != 1); // Technically they could sit like that, but it is highly unlikely...
  Node *n5 = node_alloc(pool);
  assert(n5 - n4 == 1);
  Node *n6 = node_alloc(pool);
  assert(n6 - n5 == 1);
  Node *n7 = node_alloc(pool);
  assert(n7 - n6 == 1);
  Node *n8 = node_alloc(pool);
  assert(n8 - n7 == 1);
  Node *n9 = node_alloc(pool);
  assert(n9 - n8 == 1);
  Node *n10 = node_alloc(pool);
  assert(n10 - n9 != 1); // Technically they could sit like that, but it is highly unlikely...

  free_node(pool, n2);
  free_node(pool, n10);
  Node *reused10 = node_alloc(pool);
  assert(n10 == reused10);
  Node *reused2 = node_alloc(pool);
  assert(n2 == reused2);

  free_pool(pool);
}
