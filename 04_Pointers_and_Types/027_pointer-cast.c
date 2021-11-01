#include <assert.h>

void
f1()
{
  int*  ip  = (int*)0x12345;
  void*  p  = ip;
  int*  ip2 = p;
  assert(ip == ip2);
}

void
f2()
{
  int*          ip  = (int*)0x12345;
  int volatile* ip2 = ip;
  int*          ip3 = (int*)ip2;
  assert(ip == ip2);
  assert(ip == ip3);
}

void
f3()
{
  int i;
  char* x = (char*)&i;
  assert((char*)&i == x);
}

void
f4()
{
  int        i   = 42;
  int const* ip  = &i;           // Adding qualified, fine
  int*       ip2 = (int*)ip;     // Removing it again, fine
  *ip2           = 13;           // Changing i, no problem, i isn't const

  int const i2 = 13;
   ip2         = (int*)&i2;      // Removing qualifier ok, but
  *ip2         = 42;             // UNDEFINED BEHAVIOUR, i2 is const
}

void
f5()
{
  union U
  {
    int    i;
    double d;
  };

  union U u;
  int*    ip = (int*)&u;
  double* dp = (double*)&u;

  assert((union U*)ip == &u);
  assert((union U*)dp == &u);
}

void
f6()
{
  struct S* s = (struct S*)0x12345;
  struct T* t = (struct T*)s;
  assert((struct S*)t == s);
}

void
f7()
{
  int i;
  char* x = (char*)&i;
  assert((char*)&i == x);
}

void
f8()
{
  int     i  = 42;
  int*    ip = &i;
  double* dp = (double*)ip;
  int*   ip2 = (int*)dp;
  assert(ip == ip2);          // Maybe ip == ip2, or maybe not.
}

void
f9()
{
  double d;
  // float* f = &d;
  // *f = 1.34;
}

int
main()
{
  f1();
  f2();
  f3();
  f4();
  f5();
  f6();
  f7();
  f8();
  f9();
}
