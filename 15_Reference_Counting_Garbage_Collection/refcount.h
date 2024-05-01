#pragma once

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef void (*cleanup_fn) (void *mem, void *context);

typedef struct rc_struct
{
  union
  {
    size_t rc;
    void  *stack;
  };
  cleanup_fn  cleanup;
  max_align_t user_data[];
} rc_struct;

void *rc_alloc (size_t size, cleanup_fn cleanup);
void *incref (void *p);
void *decref_ctx (void *p, void *ctx); // Starts deletion process (used in cleanup_fn) …
#define decref(p) decref_ctx (p, NULL) // … use this macro otherwise.

// Annotation macros
#define borrows
#define takes
#define give(x) x

#define container(p, type, member) (type *)((char *)p - offsetof (type, member))
#define get_rc_struct(p)           container (p, rc_struct, user_data)
#define user_mem(rc_struct)        (void *)(rc_struct->user_data)
#define RCSIZE                     offsetof (rc_struct, user_data)
