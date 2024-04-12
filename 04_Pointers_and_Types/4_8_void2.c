
int
main ()
{
  int  i  = 42;
  int *ip = &i;

  // You do not need an explicit cast to convert to and from void and data pointers,
  // Here we cast an `int *` to `char *` without getting any warnings.
  void *vp = ip;
  char *cp = vp;
}
