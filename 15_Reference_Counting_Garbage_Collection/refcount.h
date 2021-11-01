#pragma once

#include <assert.h>
#include <stddef.h>   // for max_align_t
#include <stdlib.h>
#include <stdio.h>

typedef void(*CleanupFn)(void*, void*);  // callback function for user-defined objects

typedef struct refcount
{
  union                        
  {
    size_t     rc;
    void*      stack;
  };
  CleanupFn    cleanup;
  max_align_t  user_data[];
} RefCount;

void* rc_alloc   (size_t size, CleanupFn cleanup);
void* incref     (void* p);
void* decref_ctx (void* p, void* ctx);  // Use this one when decref'ing from a callback
// Use this one otherwise
#define decref(p)   decref_ctx(p, NULL)

// Annotation macros
#define borrows
#define takes
#define give(x) x

#define container(p, type, member) (type*)((char*)p - offsetof(type, member))
#define get_refcount(p)            container(p, RefCount, user_data)
#define user_mem(rc)               (void*)(rc->user_data)
#define RCSIZE                     offsetof(RefCount, user_data)
