int
foo (int x, int y)
{
  return x + y;
}

int
main ()
{
  /* 1 */ int a = 13, b = 42;
  /* 2 */ int c = foo (a, b);
  /* 3 */ return 0;
}
