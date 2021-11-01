#include <float.h>
#include <math.h>
#include <stdio.h>

typedef struct vector
{
  double x;
  double y;
  double z;
} Vector;

void
print_vector(Vector const* v)
{
  double x = v->x;
  double y = v->y;
  double z = v->z;
  printf("<%.2e, %.2e, %.2e>\n", x, y, z);
}

double
vector_length(Vector* v)
{
  double x = v->x;
  double y = v->y;
  double z = v->z;
  return sqrt(x * x + y * y * z * z);
}

Vector*
shortest(int n, Vector* vectors[n])
{
  Vector* shortest        = &(Vector){ .x = DBL_MAX, .y = DBL_MAX, .z = DBL_MAX };
  double  shortest_length = vector_length(shortest);

  for (int i = 0; i < n; ++i) {
    Vector* v      = vectors[i];
    double  length = vector_length(v);
    if (length < shortest_length) {
      shortest        = v;
      shortest_length = length;
    }
  }
  return shortest;
}

void
trash_stack()
{
  // volatile to not optimize the array away
  volatile char x[1000];
  for (int i = 0; i < 1000; i++)
    x[i] = 0;
}

int
main()
{
  Vector* vectors[] = { &(Vector){ .x = 10.0, .y = 13.0, .z = 42.0 },
                        &(Vector){ .x = -1.0, .y = 32.0, .z = 15.0 },
                        &(Vector){ .x =  0.0, .y =  3.0, .z =  1.0 } };

  print_vector(shortest(3, vectors)); // .x =  0.0,       .y =  3.0,       .z =  1.0
  print_vector(shortest(2, vectors)); // .x = -1.0,       .y = 32.0,       .z = 15.0
  print_vector(shortest(1, vectors)); // .x = 10.0,       .y = 13.0,       .z = 42.0
  print_vector(shortest(0, vectors)); // .x =  1.80e+308, .y =  1.80e+308, .z =  1.80e+308
  Vector* v =  shortest(0, vectors);
  print_vector(v);                    // .x =  1.80e+308, .y =  1.80e+308, .z =  1.80e+308
  trash_stack(); print_vector(v);     // .x =  0.0,       .y =  0.0,       .z =  0.0
}
