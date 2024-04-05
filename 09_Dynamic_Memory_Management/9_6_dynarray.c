#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/param.h>

// dynamic array of integers
typedef struct {
  size_t capacity;
  size_t len;
  int *data; // can be NULL!
} dynarray;

// memory helpers
// We don't want to have issues with `realloc(p, 0)`. (See previous file on reallocations.)
#define MIN_ARRAY_SIZE 1
#define size_check(n, type) ((SIZE_MAX / sizeof(type)) >= (n))
#define checked_malloc(n, type) (size_check((n), (type)) ? malloc((n) * sizeof(type)) : NULL)
#define checked_realloc(p, n, type) (size_check((n), (type)) ? realloc((p), (n) * sizeof(type)) : NULL)

// dynamic array helpers
#define da_at(da, i) (da)->data[(i)]
// #define da_len(da) (da)->used

bool da_init(dynarray *da, size_t init_size, size_t init_used) {
  assert(init_size >= init_used);
  init_size = MAX(init_size, MIN_ARRAY_SIZE);
  da->data = checked_malloc(init_size, *da->data);
  da->capacity = da->data ? init_size : 0;
  da->len = da->data ? init_used : 0;
  return !!da->data;
}

void da_dealloc(dynarray *da) {
  free(da->data);
  da->data = NULL;
  da->capacity = 0;
  da->len = 0;
}

// Resizes to larger memory.
static bool da_extend(dynarray *da, size_t new_size) {
  assert(new_size > da->len);
  size_t alloc_size = MAX(new_size, MIN_ARRAY_SIZE);
  int *new_data = checked_realloc(da->data, alloc_size, *da->data);
  if (!new_data) {
    return false;
  }
  da->data = new_data;
  da->capacity = alloc_size;
  return true;
}

#define max_array_len(type) (SIZE_MAX / sizeof(type))
#define is_at_max_len(n, type) ((n) == max_array_len(type))

// Returns new size of dynamic array.
size_t capped_dbl(size_t n) {
  size_t new_size = n < max_array_len(int) / 2 ? (2 * n) : max_array_len(int);
  new_size = new_size ? new_size : MIN_ARRAY_SIZE;
  return new_size;
}

// #define capped_dbl(n, type) (((n) < max_array_len(type) / 2) ? (2 * (n)) : max_array_len(type))

bool da_append(dynarray *da, int val) {
  assert(da->len <= da->capacity);
  if (da->len == da->capacity) {
    if (is_at_max_len(da->capacity, *da->data)) {
      return false;
    }
    size_t new_size = capped_dbl(da->capacity);
    if (!da_extend(da, new_size)) {
      return false;
    }
    printf("size: %zd\n", da->capacity);
  }
  da->data[da->len++] = val;
  return true;
}

// -- Helpers

void da_print(dynarray *da) {
  printf("Values: ");
  for (int i = 0; i < da->len; i++) {
    printf("%d ", da_at(da, i));
  }
  printf("\n");
}

void da_fill(dynarray *da, size_t init_size) {
  for (int i = init_size; i < init_size + 500; i++) {
    if (!da_append(da, i)) {
      printf("Allocation error!\n");
      break;
    }
  }
}

// -------------- Main ------------------------------------------------------------------------------------ //

int main() {

  // create dynamic array with 4 slots
  dynarray da;
  size_t init_size = 1 << 5;
  if (!da_init(&da, init_size, init_size)) {
    printf("Allocation error!\n");
    exit(EXIT_FAILURE);
  }

  {
    printf("---------- Original Dynamic Array ---------------\n\n");
    printf("length: %zu\n", da.len);

    da_print(&da);
    // fill array
    for (int i = 0; i < da.len; i++) {
      da_at(&da, i) = i;
    }
    da_print(&da);
  }

  {
    printf("\n---------- Dynamically Extending Array ----------\n\n");
    printf("length: %zu\n\n", da.len);
    da_fill(&da, init_size);
    da_print(&da);
  }

  {
    printf("\n---------- Deallocating Dynamic Array ----------\n");
    da_dealloc(&da);
    printf("length: %zu\n", da.len);
    printf("size:   %zu\n", da.capacity);
  }

  {
    printf("\n---------- Extending Array ---------------------\n");
    da_fill(&da, init_size);
    printf("length: %zu\n\n", da.len);
    da_print(&da);
  }
}
