#pragma once

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef void (*cleanup_fn) (void *, void *);

typedef struct refcount
{
  union
  {
    size_t rc;
    void  *stack;
  };
  cleanup_fn  cleanup;
  max_align_t user_data[];
} refcount;

void *rc_alloc (size_t size, cleanup_fn cleanup);
void *incref (void *p);
// Use this one when decref'ing from a callback
void *decref_ctx (void *p, void *ctx);
// Use this one otherwise
#define decref(p) decref_ctx (p, NULL)

// Annotation macros
#define borrows
#define takes
#define give(x) x

#define container(p, type, member) (type *)((char *)p - offsetof (type, member))
#define get_refcount(p)            container (p, refcount, user_data)
#define user_mem(rc)               (void *)(rc->user_data)
#define RCSIZE                     offsetof (refcount, user_data)
