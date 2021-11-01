// heap-allocated inlined arrays; search for Point; everything else is verbatimely the same

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

typedef struct da_meta
{
  size_t size;
  size_t used;
} DaMeta;

#define dynarr(TYPE)                     \
  struct                                 \
  {                                      \
    DaMeta meta;                         \
    TYPE   data[];                       \
  }

// "The following functions can work with dynamic arrays without knowing the underlying type
// because we have extracted the meta-information as a separate type and because we treat
// the pointer to a dynamic array as a pointer to a da_meta structure (and safely so, as long
// as the meta struct is always the first member in the dynamic array struct)."

void*
realloc_dynarray_mem(DaMeta* p, size_t meta_size, size_t obj_size, size_t new_len)
{
  if (((SIZE_MAX - meta_size) / obj_size < new_len)) goto fail;          // is there a size overflow?
  DaMeta* da = realloc(p, meta_size + obj_size * new_len);
  if (!da) goto fail;
  da->size = new_len;
  da->used = MIN(da->used, new_len);
  return da;

fail:
  free(p);                                                               // always free if we cannot reallocate
  return NULL;
}

void*
new_dynarray_mem(size_t meta_size, size_t obj_size, size_t len)
{
  DaMeta* array = realloc_dynarray_mem(NULL, meta_size, obj_size, len);
  if (array) array->used = 0;
  return array;
}

void*
grow_dynarray_mem(DaMeta* p, size_t meta_size, size_t obj_size)
{
  size_t used   = meta_size + obj_size * p->size;
  size_t adding = MAX(1, p->size);
  if ((SIZE_MAX - used) / obj_size < adding)  { free(p); return NULL; }
  return realloc_dynarray_mem(p, meta_size, obj_size, p->size + adding);
}

#define da_data_offset(da)    ((char*)&(da)->data - (char*)(da))  /* size of DaMeta */
#define da_at(da, i)          (da->data[(i)])
#define da_len(da)            (da->meta.used)
#define new_da(da, init_size) ((da) = NULL, new_dynarray_mem(da_data_offset(da), sizeof *(da)->data, (init_size)))  /* using comma-operator */

#define da_free(da)                                                                                                \
  do {                                                                                                             \
    free(da);                                                                                                      \
    (da) = NULL;                                                                                                   \
  } while (0)

#define da_append(da, ...)                                                                                         \
  do {                                                                                                             \
    if ((da)->meta.used == (da)->meta.size) {                                                                      \
      (da) = grow_dynarray_mem((DaMeta*)(da), da_data_offset(da), sizeof *(da)->data);                             \
      if (!(da)) break;                                                                                            \
    }                                                                                                              \
    (da)->data[(da)->meta.used++] = __VA_ARGS__;                                                                   \
  } while (0)

// =============== Point ==================================================
typedef struct
{
  double x, y;
} point;

typedef dynarr(point) point_array;

// Use a pointer to the dynamic array pointer (this gives the file its name *-ref.c)
void
add_points(size_t n, double xs[n], double ys[n], point_array** da)
{
  for (int i = 0; i < n; i++) {
    da_append(*da, (point){ .x = xs[i], .y = ys[i] });
    if (!*da) break;
  }
}

// =============== End Point ==============================================

int
main()
{
  point_array* points = new_da(points, 0);

  size_t n     = 5;
  double xs[5] = { 0.0, 1.0, 2.0, 3.0, 4.0 };
  double ys[5] = { 4.0, 3.0, 2.0, 1.0, 0.0 };
  // call by reference
  add_points(n, xs, ys, &points);
  if (!points) goto error;

  for (int i = 0; i < da_len(points); i++) printf("<%.1f,%.1f> ", da_at(points, i).x, da_at(points, i).y);
  putchar('\n');

  da_free(points);
  return 0;

error:
  return 1;
}
