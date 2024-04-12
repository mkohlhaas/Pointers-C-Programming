// Generate code using macros.

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/param.h>

#define size_check(n, type)         ((SIZE_MAX / sizeof (type)) >= (n))
#define checked_malloc(n, type)     (size_check ((n), (type)) ? malloc ((n) * sizeof (type)) : NULL)
#define checked_realloc(p, n, type) (size_check ((n), (type)) ? realloc ((p), (n) * sizeof (type)) : NULL)
#define max_array_len(type)         (SIZE_MAX / sizeof (type))
#define is_at_max_len(n, type)      ((n) == max_array_len (type))
#define capped_dbl(n, type)         (((n) < max_array_len (type) / 2) ? (2 * (n)) : max_array_len (type))
#define MIN_ARRAY_SIZE              1
#define da_at(da, i)                (da)->data[(i)]
#define da_len(da)                  (da)->used

// -------------------- Macro Definitions ---------------------------------------------------------------------------

#define GEN_DYNARRAY_DECLARATIONS(TYPE)                                                                                \
  typedef struct dynarray_##TYPE                                                                                       \
  {                                                                                                                    \
    size_t size;                                                                                                       \
    size_t used;                                                                                                       \
    TYPE  *data;                                                                                                       \
  } dynarray_##TYPE;                                                                                                   \
                                                                                                                       \
  bool da_##TYPE##_init (dynarray_##TYPE *da, size_t init_size, size_t init_used);                                     \
  void da_##TYPE##_dealloc (dynarray_##TYPE *da);                                                                      \
  bool da_##TYPE##_resize (dynarray_##TYPE *da, size_t new_size);                                                      \
  bool da_##TYPE##_append (dynarray_##TYPE *da, TYPE val);

#define GEN_DYNARRAY_IMPLEMENTATIONS(TYPE)                                                                             \
  bool da_##TYPE##_init (dynarray_##TYPE *da, size_t init_size, size_t init_used)                                      \
  {                                                                                                                    \
    assert (init_size >= init_used);                                                                                   \
    init_size = MAX (init_size, MIN_ARRAY_SIZE);                                                                       \
    da->data  = checked_malloc (init_size, *da->data);                                                                 \
    da->size  = (da->data) ? init_size : 0;                                                                            \
    da->used  = (da->data) ? init_used : 0;                                                                            \
    return !!da->data;                                                                                                 \
  }                                                                                                                    \
                                                                                                                       \
  void da_##TYPE##_dealloc (dynarray_##TYPE *da)                                                                       \
  {                                                                                                                    \
    free (da->data);                                                                                                   \
    da->data = 0;                                                                                                      \
    da->size = da->used = 0;                                                                                           \
  }                                                                                                                    \
                                                                                                                       \
  bool da_##TYPE##_resize (dynarray_##TYPE *da, size_t new_size)                                                       \
  {                                                                                                                    \
    size_t alloc_size = MAX (new_size, MIN_ARRAY_SIZE);                                                                \
    TYPE  *new_data   = checked_realloc (da->data, alloc_size, *da->data);                                             \
    if (!new_data)                                                                                                     \
      return false;                                                                                                    \
    da->data = new_data;                                                                                               \
    da->size = alloc_size;                                                                                             \
    da->used = MIN (da->used, new_size);                                                                               \
    return true;                                                                                                       \
  }                                                                                                                    \
                                                                                                                       \
  bool da_##TYPE##_append (dynarray_##TYPE *da, TYPE val)                                                              \
  {                                                                                                                    \
    if (da->used == da->size)                                                                                          \
      {                                                                                                                \
        if (is_at_max_len (da->size, *da->data))                                                                       \
          return false;                                                                                                \
        size_t new_size       = capped_dbl (da->size, *da->data);                                                      \
        int    resize_success = da_##TYPE##_resize (da, new_size);                                                     \
        if (!resize_success)                                                                                           \
          return false;                                                                                                \
      }                                                                                                                \
    da->data[da->used++] = val;                                                                                        \
    return true;                                                                                                       \
  }

// -------------------- Macro Calls --------------------------------------------------------------------------------

// ---------- Integer Dynarray -------------------------------------------------------------------------------------

GEN_DYNARRAY_DECLARATIONS (int)    // goes in .h file
GEN_DYNARRAY_IMPLEMENTATIONS (int) // goes in .c file

// ---------- Double Dynarray --------------------------------------------------------------------------------------

GEN_DYNARRAY_DECLARATIONS (double)
GEN_DYNARRAY_IMPLEMENTATIONS (double)

// ---------- Point Dynarray ---------------------------------------------------------------------------------------

// typedefs for custom types
typedef struct
{
  double x, y;
} point;

GEN_DYNARRAY_DECLARATIONS (point)
GEN_DYNARRAY_IMPLEMENTATIONS (point)

// -----------------------------------------------------------------------------------------------------------------

// These will fail b/c of space character in type definition (would need typedefs)
#if 0
GEN_DYNARRAY_TYPE(long long)

struct foo { int i, j; };
GEN_DYNARRAY_TYPE(struct foo)
#endif

// print dynarray
void
da_int_print (dynarray_int *da)
{
  for (int i = 0; i < da_len (da); i++)
    {
      printf ("%d ", da_at (da, i));
    }
  printf ("\ncurrent length: %zu\n\n", da_len (da));
}

void
da_double_print (dynarray_double *da)
{
  for (int i = 0; i < da_len (da); i++)
    {
      printf ("%.1f ", da_at (da, i));
    }
  printf ("\ncurrent length: %zu\n\n", da_len (da));
}

void
da_point_print (dynarray_point *da)
{
  for (int i = 0; i < da_len (da); i++)
    {
      point *p = &da_at (da, i);
      printf ("<%.1f, %.1f>\n", p->x, p->y);
    }
  printf ("current length: %zu\n\n", da_len (da));
}

// -------------------- Main ---------------------------------------------------------------------------------------

int
main ()
{
  {
    printf ("-------------- Int Dynamic Array -----------------------------------\n\n");

    dynarray_int da;

    // initialize dynarray
    int success = da_int_init (&da, 4, 4); // init dynarray
    if (!success)
      {
        printf ("allocation error\n");
      }

    // update values
    for (int i = 0; i < da_len (&da); i++)
      {
        da_at (&da, i) = i;
      }

    da_int_print (&da);

    // append more values into dynarray
    for (int i = 0; i < 10; i++)
      {
        if (!da_int_append (&da, 10 * (i + 1)))
          {
            printf ("allocation error");
            break;
          }
      }

    // print dynarray
    da_int_print (&da);
    da_int_dealloc (&da); // clean up
  }

  {
    printf ("-------------- Double Dynamic Array --------------------------------\n\n");

    dynarray_double da;

    // init dynarray
    int success = da_double_init (&da, 4, 4);
    if (!success)
      {
        printf ("allocation error\n");
      }

    // update values
    for (int i = 0; i < da_len (&da); i++)
      {
        da_at (&da, i) = (double)i;
      }

    da_double_print (&da);

    // append more values into dynarray
    for (int i = 0; i < 10; i++)
      {
        if (!da_double_append (&da, 10.0 * (i + 1)))
          {
            printf ("allocation error");
            break;
          }
      }

    da_double_print (&da);
    da_double_dealloc (&da);
  }

  {
    printf ("-------------- Point Dynamic Array ---------------------------------\n\n");

    dynarray_point da;

    // init dynarray
    da_point_init (&da, 0, 0);

    // insert some values into dynarray
    for (int i = 0; i < 5; i++)
      {
        da_point_append (&da, (point){
                                  .x = i,
                                  .y = i,
                              });
      }

    da_point_print (&da);

    // append more values into dynarray
    for (int i = 0; i < 10; i++)
      {
        da_point_append (&da, (point){
                                  .x = 10.0 * (i + 1),
                                  .y = 10.0 * (i + 1),
                              });
      }

    da_point_print (&da);

    da_point_dealloc (&da); // clean up
  }
}
