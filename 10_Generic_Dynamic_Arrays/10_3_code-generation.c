// generate code using macros

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define size_check(n, type) ((SIZE_MAX / sizeof(type)) >= (n))
#define checked_malloc(n, type) (size_check((n), (type)) ? malloc((n) * sizeof(type)) : NULL)
#define checked_realloc(p, n, type) (size_check((n), (type)) ? realloc((p), (n) * sizeof(type)) : NULL)
#define max_array_len(type) (SIZE_MAX / sizeof(type))
#define is_at_max_len(n, type) ((n) == max_array_len(type))
#define capped_dbl(n, type) (((n) < max_array_len(type) / 2) ? (2 * (n)) : max_array_len(type))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MIN_ARRAY_SIZE 1
#define da_at(da, i) (da)->data[(i)]
#define da_len(da) (da)->used

// ====================================================================================
// ==================== Macro Definitions =============================================
// ====================================================================================

#define GEN_DYNARRAY_DECLARATIONS(TYPE)                                                                                \
  typedef struct TYPE##_dynarray {                                                                                     \
    size_t size;                                                                                                       \
    size_t used;                                                                                                       \
    TYPE *data;                                                                                                        \
  } TYPE##_dynarray;                                                                                                   \
                                                                                                                       \
  bool TYPE##_da_init(TYPE##_dynarray *da, size_t init_size, size_t init_used);                                        \
  void TYPE##_da_dealloc(TYPE##_dynarray *da);                                                                         \
  bool TYPE##_da_resize(TYPE##_dynarray *da, size_t new_size);                                                         \
  bool TYPE##_da_append(TYPE##_dynarray *da, TYPE val);

#define GEN_DYNARRAY_IMPLEMENTATIONS(TYPE)                                                                             \
  bool TYPE##_da_init(TYPE##_dynarray *da, size_t init_size, size_t init_used) {                                       \
    assert(init_size >= init_used);                                                                                    \
    init_size = MAX(init_size, MIN_ARRAY_SIZE);                                                                        \
    da->data = checked_malloc(init_size, *da->data);                                                                   \
    da->size = (da->data) ? init_size : 0;                                                                             \
    da->used = (da->data) ? init_used : 0;                                                                             \
    return !!da->data;                                                                                                 \
  }                                                                                                                    \
                                                                                                                       \
  void TYPE##_da_dealloc(TYPE##_dynarray *da) {                                                                        \
    free(da->data);                                                                                                    \
    da->data = 0;                                                                                                      \
    da->size = da->used = 0;                                                                                           \
  }                                                                                                                    \
                                                                                                                       \
  bool TYPE##_da_resize(TYPE##_dynarray *da, size_t new_size) {                                                        \
    size_t alloc_size = MAX(new_size, MIN_ARRAY_SIZE);                                                                 \
    TYPE *new_data = checked_realloc(da->data, alloc_size, *da->data);                                                 \
    if (!new_data)                                                                                                     \
      return false;                                                                                                    \
    da->data = new_data;                                                                                               \
    da->size = alloc_size;                                                                                             \
    da->used = MIN(da->used, new_size);                                                                                \
    return true;                                                                                                       \
  }                                                                                                                    \
                                                                                                                       \
  bool TYPE##_da_append(TYPE##_dynarray *da, TYPE val) {                                                               \
    if (da->used == da->size) {                                                                                        \
      if (is_at_max_len(da->size, *da->data))                                                                          \
        return false;                                                                                                  \
      size_t new_size = capped_dbl(da->size, *da->data);                                                               \
      int resize_success = TYPE##_da_resize(da, new_size);                                                             \
      if (!resize_success)                                                                                             \
        return false;                                                                                                  \
    }                                                                                                                  \
    da->data[da->used++] = val;                                                                                        \
    return true;                                                                                                       \
  }

// ===================================================================================
// ==================== Macro Calls ==================================================
// ===================================================================================

// ========== Integer Dynarray =======================================================
GEN_DYNARRAY_DECLARATIONS(int)    // goes in .h file
GEN_DYNARRAY_IMPLEMENTATIONS(int) // goes in .c file
// ========== Double Dynarray ========================================================
GEN_DYNARRAY_DECLARATIONS(double)
GEN_DYNARRAY_IMPLEMENTATIONS(double)
// ========== Pointer Dynarray =======================================================
typedef struct {
  double x, y;
} point; // typedefs for custom types
GEN_DYNARRAY_DECLARATIONS(point)
GEN_DYNARRAY_IMPLEMENTATIONS(point)
// ===================================================================================

// These will fail b/c of space character in type definition (would need typedefs)
#if 0
GEN_DYNARRAY_TYPE(long long)

struct foo { int i, j; };
GEN_DYNARRAY_TYPE(struct foo)
#endif

// ===================================================================================
// ==================== Main =========================================================
// ===================================================================================
int main() {
  {
    printf("\33[38;5;206m============== INT DYNAMIC ARRAY ===================================\033[0m\n\n");

    int_dynarray da;

    int success = int_da_init(&da, 4, 4); // init dynarray
    if (!success)
      printf("allocation error\n");
    for (int i = 0; i < da_len(&da); i++)
      da_at(&da, i) = i; // insert some values into dynarray
    for (int i = 0; i < da_len(&da); i++)
      printf("%d ", da_at(&da, i)); // print dynarray
    printf("\ncurrent length: %zu\n", da_len(&da));
    for (int i = 0; i < 10; i++) // append more values into dynarray
      if (!int_da_append(&da, 10 * (i + 1))) {
        printf("allocation error");
        break;
      }
    for (int i = 0; i < da_len(&da); i++)
      printf("%d ", da_at(&da, i)); // print dynarray
    printf("\ncurrent length: %zu\n", da_len(&da));
    int_da_dealloc(&da); // clean up
  }

  {
    printf("\n\33[38;5;206m============== DOUBLE DYNAMIC ARRAY ================================\033[0m\n\n");

    double_dynarray dda;

    int success = double_da_init(&dda, 4, 4); // init dynarray
    if (!success)
      printf("allocation error\n");
    for (int i = 0; i < da_len(&dda); i++)
      da_at(&dda, i) = (double)i; // insert some values into dynarray
    for (int i = 0; i < da_len(&dda); i++)
      printf("%.1f ", da_at(&dda, i)); // print dynarray
    printf("\ncurrent length: %zu\n", da_len(&dda));
    for (int i = 0; i < 10; i++) // append more values into dynarray
      if (!double_da_append(&dda, 10.0 * (i + 1))) {
        printf("allocation error");
        break;
      }
    for (int i = 0; i < da_len(&dda); i++)
      printf("%.1f ", da_at(&dda, i)); // print dynarray
    printf("\ncurrent length: %zu\n", da_len(&dda));
    double_da_dealloc(&dda); // clean up
  }

  {
    printf("\n\33[38;5;206m============== POINT DYNAMIC ARRAY =================================\033[0m\n\n");

    point_dynarray pda;
    point_da_init(&pda, 0, 0); // init dynarray
    for (int i = 0; i < 5; i++)
      point_da_append(&pda, (point){.x = i, .y = i}); // insert some values into dynarray
    for (int i = 0; i < da_len(&pda); i++) {          // print dynarray
      point *p = &da_at(&pda, i);
      printf("<%.1f, %.1f>\n", p->x, p->y);
    }
    point_da_dealloc(&pda); // clean up
  }
}
