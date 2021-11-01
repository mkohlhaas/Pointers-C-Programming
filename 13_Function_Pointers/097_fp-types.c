#include <stdio.h>

void f(void) {}
void (*fp)(void) = f;

double
g(int x, float y)
{
  return x + y;
}

double (*gp)(int, float) = &g;

// return value is a function taking void and returning void, like e.g. f
// the name of the function is 'complex'
// complex is a function (pointer) taking an int and a function taking an int and returning an int
// These do not work:
// void (complex(int x, int (*p)(int)))(void)
// void  complex(int x, int (*p)(int))(void)
void (*complex(int x, int (*p)(int)))(void)
{
  printf("%d\n", p(x));
  return f;
}

// typedefs: put the type name where you would put the pointer name
typedef void (*VoidFn)(void);
typedef int  (*IntFn)(int);

VoidFn
simple(int x, IntFn p)
{
  printf("%d\n", p(x));
  return f;
}

// h has type IntFn
int
h(int x)
{
  return 2 * x;
}

int
main()
{
  IntFn fp1;
  fp1 = h;
  // cast
  fp = (void (*)(void))g;
  fp(); // won't do anything
  double (*fp2)(int, float) = (double (*)(int, float))fp;
  // and back again
  printf("%d\n", fp1(1));

  // address or not? doesn't matter ... 
  fp = f;
  fp();
  fp = &f;  // use a function as a pointer and it becomes a pointer; like arrays
  fp();

  gp = g;
  gp(2, 3.0);
  gp = &g;
  gp(2, 3.0);

  // fp is a pointer to a function, &fp is a pointer to a pointer to a function.
  // So does not work:
  // void (*fp2)(void) = &fp;
  void (*fp3)(void) = fp;
  fp3();

  simple (42, h);
  complex(42, h);
}
