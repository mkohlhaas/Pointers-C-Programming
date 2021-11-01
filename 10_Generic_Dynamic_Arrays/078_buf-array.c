// using char*
// search for the words Update, obj_size, memcpy, malloc, realloc and free

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
  size_t size;
  size_t used;
  size_t obj_size;  // Update: added size of objects
  char*  data;      // Update int* => char*; data is basically a byte pointer
} dynarray;

// Compute the address instead of index into array
// cast char* -> void* -> type*
// from the void* we can cast to any type of pointer
#define da_at(da, i)                     (void*)((da)->data + (i) * (da)->obj_size)
#define da_at_as(da, i, type)            *(type*)da_at(da, i)
#define da_len(da)                       (da)->used
#define size_check(n, obj_size)          ((SIZE_MAX / (obj_size)) >= (n)) /* Use object size instead of type */
#define checked_malloc(n, obj_size)      (size_check((n), (obj_size)) ? malloc((n) * (obj_size))       : NULL)
#define checked_realloc(p, n, obj_size)  (size_check((n), (obj_size)) ? realloc((p), (n) * (obj_size)) : NULL)
#define max_array_len(obj_size)          (SIZE_MAX / obj_size)
#define is_at_max_len(n, obj_size)       ((n) == max_array_len(obj_size))
#define capped_dbl(n, obj_size)          (((n) < max_array_len(obj_size) / 2) ? (2 * (n)) : max_array_len(obj_size))
#define MAX(a, b)                        (((a) > (b)) ? (a) : (b))
#define MIN(a, b)                        (((a) < (b)) ? (a) : (b))
#define MIN_ARRAY_SIZE                   1

bool
da_init(dynarray* da, size_t init_size, size_t init_used, size_t obj_size)
{
  assert(init_size >= init_used);
  init_size    = MAX(init_size, MIN_ARRAY_SIZE);
  da->data     = checked_malloc(init_size, obj_size);
  da->size     = (da->data) ? init_size : 0;
  da->used     = (da->data) ? init_used : 0;
  da->obj_size = obj_size;
  return !!da->data;
}

void
da_dealloc(dynarray* da)
{
  free(da->data);
  da->data = NULL;
  da->size = 0;
  da->used = 0;
}

bool
da_resize(dynarray* da, size_t new_size)
{
  size_t alloc_size = MAX(new_size, MIN_ARRAY_SIZE);
  char*  new_data   = checked_realloc(da->data, alloc_size, da->obj_size);  // Update int* => char*
  if (!new_data) return false;

  da->data = new_data;
  da->size = alloc_size;
  da->used = MIN(da->used, new_size);  // we can shrink the memory -> MIN
  return true;
}

// Update val type int => void*
// takes a pointer to the value to be added and memory copies it in our data array
bool
da_append(dynarray* da, void* val)
{
  if (da->used == da->size) {
    if (is_at_max_len(da->size, da->obj_size)) return false;
    size_t new_size       = capped_dbl(da->size, da->obj_size);
    int    resize_success = da_resize(da, new_size);
    if (!resize_success) return false;
  }
  // copy val into array
  memcpy(da->data + da->used * da->obj_size, val, da->obj_size);
  da->used++;
  return true;
}

int
main()
{
  dynarray da;

  // include sizeof here
  int success = da_init(&da, 0, 0, sizeof(int));
  if (!success) printf("allocation error\n");

  for (int i = 0; i < 5; ++i) da_append(&da, &i);

  // append a compound literal
  da_append(&da, &(int){ 314 });

  for (int i = 0; i < 5          ; i++) da_at_as(&da, i, int) += 10;          // da_at_as macro can also be used as left-value
  for (int i = 0; i < da_len(&da); i++) printf("%d ", da_at_as(&da, i, int)); // da_at_as used as right-value
  putchar('\n');

  da_dealloc(&da);

  // reallocations work fine
  for (int i = 0; i < 5          ; i++) da_append(&da, &i);
  for (int i = 0; i < da_len(&da); i++) printf("%d ", da_at_as(&da, i, int)); // da_at_as used as right-value
  putchar('\n');
}
