#include "refcount.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// immutable tree (consisting of immutable nodes)
typedef struct node Node;

typedef struct node {
  int const val;
  Node *const left;
  Node *const right;
} Node;

// t is Node*
#define EMPTY incref(empty_node)
#define is_empty(t) (t == empty_node)
#define is_error(t) (t == NULL)
