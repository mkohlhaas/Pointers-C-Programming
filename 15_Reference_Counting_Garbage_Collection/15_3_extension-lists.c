// with autoclean_list (cleanup attribute) you don't need to decref in the function definitions any more
// tail recursion optimization not supported by compilers at time of writing

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct link
{
  int                refcount;
  struct link *const next;
  int const          value;
} link;

typedef link *List;

link *
incref (link *link)
{
  link->refcount++;
  return link;
}

void free_link (link *link);

link *
decref (link *link)
{
  if (link && --link->refcount == 0)
    {
      free_link (link);
      return NULL;
    }
  return link;
}

void
free_link (link *link)
{
  decref (link->next);
  free (link);
}

// Empty lists and errors
link NIL_LINK = { .refcount = 1 };
#define is_error(x) ((x) == 0)
#define is_nil(x)   ((x) == &NIL_LINK)
#define NIL         incref (&NIL_LINK)

// Give a reference; you lose it yourself
#define takes
#define give(x) x

#if 0
// GCC compiler extension statement-expression for give()
#define give(x)                                                                                                        \
  ({                                                                                                                   \
    void *p = x;                                                                                                       \
    x       = 0;                                                                                                       \
    p;                                                                                                                 \
  })
#endif

// Let the function use your reference
#define borrows

void
list_cleanup (link **x)
{
  decref (*x);
}

#define autoclean_list List __attribute__ ((cleanup (list_cleanup)))

List
cons (int head, takes List tail_)
{
  // cleanup attribute doesn’t work for function arguments but local variables
  autoclean_list tail = tail_;

  if (is_error (tail))
    {
      return NULL;
    }
  List new_link = malloc (sizeof *new_link);
  if (new_link)
    {
      link link_data = { .refcount = 1,
                         .next     = incref (tail), // tail will be deleted, so remember it here
                         .value    = head };
      memcpy (new_link, &link_data, sizeof *new_link);
    }
  return give (new_link);
}

void
print_list (borrows List x)
{
  printf ("[ ");
  while (!is_nil (x))
    {
      printf ("%d[%d] ", x->value, x->refcount);
      x = x->next;
    }
  printf ("]\n");
}

// -- length ------------------------------------------------------------------------------------ //

int
length_rec (takes List x_, int acc)
{
  autoclean_list x = x_;

  if (is_nil (x))
    {
      return acc;
    }
  else
    {
      return length_rec (incref (x->next), acc + 1);
    }
}

#define length(x) length_rec (x, 0)

// -- reverse ----------------------------------------------------------------------------------- //

List
reverse_rec (takes List x_, takes List acc_)
{
  autoclean_list x = x_, acc = acc_;

  if (is_error (x) || is_error (acc))
    {
      return NULL;
    }
  else if (is_nil (x))
    {
      return incref (acc);
    }
  else
    {
      return reverse_rec (incref (x->next), cons (x->value, incref (acc)));
    }
}

#define reverse(x) reverse_rec (x, NIL)

// -- concat ------------------------------------------------------------------------------------ //

List
concat (takes List x_, takes List y_)
{
  autoclean_list x = x_, y = y_;

  if (is_error (x) || is_error (y))
    {
      return NULL;
    }
  else if (is_nil (x))
    {
      return incref (y);
    }
  else
    {
      return cons (x->value, concat (incref (x->next), incref (y)));
    }
}

// -- main -------------------------------------------------------------------------------------- //

int
main ()
{
  printf ("Construction:\n");
  printf ("-------------\n");
  List x = cons (1, cons (2, cons (3, NIL)));
  print_list (x);

  List y = cons (0, incref (x));
  print_list (y);

  decref (x);
  print_list (y);

  List z = cons (-1, give (y)); // consider y gone!
  print_list (z);
  decref (z);

  printf ("\nLength:\n");
  printf ("-------\n");
  x = cons (1, cons (2, cons (3, NIL)));
  print_list (x);
  printf ("len(x) = %d\n", length (incref (x)));
  print_list (x);
  // leaks if length doesn't free its input
  printf ("len(x) = %d\n", length (cons (1, cons (2, cons (3, NIL)))));

  // decref(x);
  // this frees x
  length (give (x));

  printf ("\nReverse:\n");
  printf ("--------\n");

  x = cons (1, cons (2, cons (3, NIL)));
  print_list (x);
  y = reverse (give (x));
  print_list (y);
  decref (y);

  // would leak if reverse and length didn't free their input
  printf ("%d\n", length (reverse (cons (1, cons (2, cons (3, NIL))))));

  printf ("\nConcat:\n");
  printf ("-------\n");
  x = cons (1, cons (2, cons (3, NIL)));
  y = cons (4, cons (5, NIL));
  z = concat (incref (x), incref (y));

  print_list (z);
  decref (x);
  decref (y);
  print_list (z);
  decref (z);

  z = concat (cons (1, cons (2, NIL)), cons (11, cons (12, cons (13, NIL))));
  print_list (z);
  decref (z);
}
