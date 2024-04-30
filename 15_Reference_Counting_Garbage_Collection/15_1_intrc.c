#include <stdio.h>
#include <stdlib.h>

typedef struct rc_int
{
  int refcount;
  int value;
} rc_int;

rc_int *
new_rc_int (int i)
{
  rc_int *p = malloc (sizeof *p);
  if (p)
    {
      *p = (rc_int){ .refcount = 1, .value = i };
    }
  return p;
}

void
free_rc_int (rc_int *i)
{
  printf ("Freeing %d.\n", i->value);
  free (i);
}

rc_int *
incref (rc_int *p)
{
  if (p)
    {
      p->refcount++;
    }
  return p;
}

void
decref (rc_int *p)
{
  if (p && --p->refcount == 0)
    {
      free_rc_int (p);
    }
}

int
main ()
{
  // ignoring malloc errors …
  rc_int *i = new_rc_int (42);
  rc_int *j = incref (i);
  decref (i);
  printf ("Decremented 'i' but nothing happened (as it should).\n");
  decref (j);
}
