#include <stdio.h>

void
f (void)
{
  printf ("function f\n");
}

void (*fp) (void) = f;

double
g (int x, float y)
{
  printf ("function g\n");
  return x + y;
}

double (*gp) (int, float) = &g;

void (*complex (int x, int (*p) (int))) (void)
{
  printf ("complex: %d\n", p (x));
  return f;
}

// typedefs: put the type name where you would put the pointer name
typedef void (*void_fn) (void);
typedef int (*int_fn) (int);

void_fn
simple (int x, int_fn p)
{
  printf ("simple: %d\n", p (x));
  return f;
}

// h has type IntFn
int
h (int x)
{
  printf ("function h ");
  return 2 * x;
}

int
main ()
{
  int_fn fp1;
  fp1 = h;

  // casts
  fp = (void (*) (void))g;
  fp (); // "function g"
  double (*fp2) (int, float) = (double (*) (int, float))fp;
  // and back again
  printf ("main: %d\n", fp1 (1)); // "function h main: 2"

  // address or not? doesn't matter …
  fp = f;
  fp ();       // "function f"
  fp = &f;     // use a function as a pointer and it becomes a pointer; like arrays
  fp ();       // "function f"

  gp = g;
  gp (2, 3.0); // "function g"
  gp = &g;
  gp (2, 3.0); // "function g"

  // fp is a pointer to a function, &fp is a pointer to a pointer to a function (i.e. the address of a pointer to a
  // function). this does not work: void (*fp2)(void) = &fp;
  void (*fp3) (void) = fp;
  fp3 ();          // "function f"

  simple (42, h);  // "function h simple : 84"
  complex (42, h); // "function h complex : 84"
}
