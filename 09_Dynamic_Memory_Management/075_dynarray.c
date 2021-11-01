#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// dynamic array of integers
typedef struct
{
  size_t size;
  size_t used;
  int*   data;
} dynarray;

#define da_at(da, i)                (da)->data[(i)]
#define da_len(da)                  (da)->used
#define size_check(n, type)         ((SIZE_MAX / sizeof(type)) >= (n))
#define checked_malloc(n, type)     (size_check((n), (type)) ? malloc((n) * sizeof(type))       : NULL)
#define checked_realloc(p, n, type) (size_check((n), (type)) ? realloc((p), (n) * sizeof(type)) : NULL)
#define MAX(a, b)                   (((a) > (b)) ? (a) : (b))
#define MIN(a, b)                   (((a) < (b)) ? (a) : (b))
#define MIN_ARRAY_SIZE              1 /* We don't want to have issues with realloc(p, 0). See prev. paragraph on reallocations. */

bool
da_init(dynarray* da, size_t init_size, size_t init_used)
{
  assert(init_size >= init_used);
  init_size = MAX(init_size, MIN_ARRAY_SIZE);
  da->data  = checked_malloc(init_size, *da->data);
  da->size  = (da->data) ? init_size : 0;
  da->used  = (da->data) ? init_used : 0;
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
  int*   new_data   = checked_realloc(da->data, alloc_size, *da->data);
  if (!new_data) return false;

  da->data = new_data;
  da->size = alloc_size;
  da->used = MIN(da->used, new_size);
  return true;
}

#define max_array_len(type)    (SIZE_MAX / sizeof(type))
#define is_at_max_len(n, type) ((n) == max_array_len(type))

// returns new size of dynamic array
size_t
capped_dbl(size_t n, int type) {
  size_t new_size;
  new_size = n < max_array_len(type) / 2 ? (2 * n) : max_array_len(type);
  new_size = new_size ? new_size : 1; // da->size could be 0 after da_dealloc() or an error occurred in da_init()
  return new_size;
}

// #define capped_dbl(n, type) (((n) < max_array_len(type) / 2) ? (2 * (n)) : max_array_len(type))

bool
da_append(dynarray* da, int val)
{
  size_t new_size; bool resize;

  if (da->used == da->size) {
    if (is_at_max_len(da->size, *da->data)) return false;
    new_size = capped_dbl(da->size, *da->data);
    if (!da_resize(da, new_size)) return false;
    printf("New size is: %2zd\n", da->size);
  }
  da->data[da->used++] = val;
  return true;
}

int
main()
{
  dynarray da;

  // create dynamic array with 4 slots
  int success = da_init(&da, 4, 4);
  if (!success) printf("Allocation error!\n");

  printf("========== Original dynamic array ===============\n");
  printf("Current length: %zu\nValues: ", da_len(&da));
  for (int i = 0; i < da_len(&da); i++) da_at(&da, i) = i;             // initialize dynamic array
  for (int i = 0; i < da_len(&da); i++) printf("%d ", da_at(&da, i));  // print dynamic array

  printf("\n========== Dynamically extending array ==========\n");
  for (int i = 0; i < 500; i++)
    if (!da_append(&da, i + 4)) {
      printf("Allocation error!\n");
      break;
    }
  printf("Current length: %zu\nValues:\n", da_len(&da));
  for (int i = 0; i < da_len(&da); i++) printf("%d ", da_at(&da, i));  // print dynamic array
  putchar('\n');

  da_dealloc(&da);
}
