#include <float.h>
#include <math.h>
#include <stdio.h>

typedef struct
{
  double x;
  double y;
  double z;
} Vector;

// We assume that v is never NULL.
double
vector_length (Vector *v)
{
  double x = v->x;
  double y = v->y;
  double z = v->z;
  return sqrt (x * x + y * y + z * z);
}

// `v` can be NULL.
void
print_vector (Vector const *v)
{
  if (!v)
    {
      printf ("%p\n", v);
    }
  else
    {
      double x = v->x;
      double y = v->y;
      double z = v->z;
      printf ("<%.2e, %.2e, %.2e>\n", x, y, z);
    }
}

Vector *
shortest (int n, Vector *vectors[n])
{
  if (n < 1)
    {
      return NULL;
    }
  Vector *shortest        = vectors[0];
  double  shortest_length = vector_length (shortest);
  for (int i = 1; i < n; ++i)
    {
      Vector *v      = vectors[i];
      double  length = vector_length (v);
      if (length < shortest_length)
        {
          shortest        = v;
          shortest_length = length;
        }
    }
  return shortest;
}

int
main ()
{
  Vector *vectors[] = {
    &(Vector){ .x = 10.0, .y = 13.0, .z = 42.0 },
    &(Vector){ .x = -1.0, .y = 32.0, .z = 15.0 },
    &(Vector){ .x = 0.0, .y = 3.0, .z = 1.0 },
  };

  print_vector (shortest (3, vectors)); // .x =  0.0, .y =  3.0, .z =  1.0
  print_vector (shortest (2, vectors)); // .x = -1.0, .y = 32.0, .z = 15.0
  print_vector (shortest (1, vectors)); // .x = 10.0, .y = 13.0, .z = 42.0
  print_vector (shortest (0, vectors)); // (nil)

  Vector const longest = {
    .x = DBL_MAX,
    .y = DBL_MAX,
    .z = DBL_MAX,
  };
  Vector const *v = shortest (0, vectors);
  v               = (v ? v : &longest);
  print_vector (v); // .x = 1.80e+308, .y = 1.80e+308, .z = 1.80e+308
}
