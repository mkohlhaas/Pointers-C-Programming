// Adding a dummy element.

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// fake malloc
void *(*ml) (size_t) = malloc;

void *
fake_malloc (size_t size)
{
  if (rand () % 100 == 0)
    {
      return NULL;
    }
  else
    {
      return ml (size);
    }
}

// comment out next line to use standard malloc()
// #define malloc fake_malloc

typedef struct link
{
  int          value;
  struct link *next;
} Link;

// A list is a pointer to the first link in the list, the so-called dummy link.
// The dummy link is always present.
// Link → Dummy Link → Link → ... → Link → NULL
typedef Link *List;

Link *
new_link (int val, Link *next)
{
  Link *link = malloc (sizeof *link);
  if (!link)
    {
      return NULL;
    }
  link->value = val;
  link->next  = next;
  return link;
}

// Free all links in a list including the dummy link.
void
free_links (List x)
{
  while (x)
    {
      Link *next = x->next;
      free (x);
      x = next;
    }
}

// Removes all links except the dummy link.
void
clear_links (List x)
{
  List y = x;
  x      = x->next; // skip dummy link
  while (x)
    {
      Link *next = x->next;
      free (x);
      x = next;
    }
  y->next = NULL;
}

// New list is an empty list with a single dummy link; value could be anything.
#define new_empty_list() new_link (314159, NULL)
#define free_list(x)     free_links (x)

List
make_list_from_array (int n, int array[n])
{
  List x = new_empty_list (); // create dummy link
  if (!x)
    {
      return NULL;
    }

  for (int i = n - 1; i >= 0; i--)
    {
      Link *newLink = new_link (array[i], x->next);
      if (!newLink)
        {
          free_list (x);
          return NULL;
        }
      x->next = newLink; // connect dummy link to new link
    }

  return x;
}

void
print_list (List x)
{
  printf ("[ ");
  Link *link = x->next; // skip dummy link and get first real link
  while (link)
    {
      printf ("%d ", link->value);
      link = link->next;
    }
  printf ("]\n");
}

bool
contains (List x, int val)
{
  Link *link = x->next; // skip dummy link and get first real link
  while (link)
    {
      if (link->value == val)
        {
          return true;
        }
      link = link->next;
    }
  return false;
}

bool
prepend (List x, int val)
{
  Link *link = new_link (val, x->next);
  if (!link)
    {
      return false;
    }
  x->next = link; // connect dummy link to new link
  return true;
}

// There is always a last link b/c of the dummy link.
Link *
last_link (List x)
{
  while (x->next)
    {
      x = x->next;
    }
  return x;
}

bool
append (List x, int val)
{
  Link *link = new_link (val, NULL);
  if (!link)
    {
      return false;
    }
  last_link (x)->next = link;
  return true;
}

void
concatenate (List x, List y)
{
  last_link (x)->next = y->next; // `y` belongs to `x` from now on
  y->next             = NULL;    // list `y` can still be freed but is empty - list consists only of dummy link
}

void
delete_value (List x, int val)
{
  Link *front = x; // pointer to dummy link
  Link *next;      // front and next are neighbors - front comes before next

  while (front)
    {
      while ((next = front->next) && next->value == val)
        {
          front->next = next->next;
          free (next);
        }
      front = next; // move to the next link
    }
}

void
reverse (List x)
{
  Link *next = x->next; // skip dummy; get first real link
  x->next    = NULL;
  while (next)
    {
      Link *next_next;
      next_next  = next->next;
      next->next = x->next;
      x->next    = next;
      next       = next_next;
    }
}

#define ARRAY_SIZE(a) (sizeof a / sizeof *a)
#define c(v)          contains (x, v) ? "✓" : "✗"
#define EH(x, msg)                                                                                                     \
  ({                                                                                                                   \
    if (!x)                                                                                                            \
      {                                                                                                                \
        perror (msg);                                                                                                  \
        exit (EXIT_FAILURE);                                                                                           \
      }                                                                                                                \
  })

int
main ()
{
  bool success;
  srand (time (0));
  int array[] = { 1, 2, 3, 4, 5, 1, 2, 3, 4, 5 };
  int n       = ARRAY_SIZE (array);

  {
    // -------------------- Contains --------------------------------------------------

    List x = make_list_from_array (n, array);
    EH (x, "make list error");
    print_list (x);
    printf ("\ncontains:\n0 3 6:\n");
    printf ("%s %s %s\n", c (0), c (3), c (6));
    free_list (x);
  }

  {
    // -------------------- Prepend/Append --------------------------------------------

    printf ("\nAppend 6/Prepend 0\n");
    List x = make_list_from_array (n, array);
    EH (x, "make list error");
    EH (append (x, 6), "append list error");
    EH (prepend (x, 0), "prepend list error");
    print_list (x);
    clear_links (x);
    print_list (x);
    EH (append (x, 6), "append list error");
    EH (prepend (x, 0), "prepend list error");
    print_list (x);
    free_list (x);
  }

  {
    // -------------------- Concatenate -----------------------------------------------

    printf ("\nConcatenate:\n");
    List x = make_list_from_array (n, array);
    print_list (x);
    List y = make_list_from_array (n, array);
    print_list (y);

    concatenate (x, y);

    print_list (x);
    print_list (y);
    free_list (x);
    free_list (y);
  }

  {
    // -------------------- Deleting Values -------------------------------------------

    printf ("\nDeleting values:\n");
    List x = make_list_from_array (n, array);
    print_list (x);
    delete_value (x, 2);
    print_list (x);
    delete_value (x, 4);
    print_list (x);
    delete_value (x, 1);
    print_list (x);
    delete_value (x, 5);
    print_list (x);
    delete_value (x, 3);
    print_list (x);
    delete_value (x, 3);
    print_list (x);
    free_list (x);
  }

  {
    // -------------------- Deleting First Link ---------------------------------------

    printf ("\nDeleting first link:\n");
    List x = make_list_from_array (n, array);
    delete_value (x, 1);
    print_list (x);
    free_list (x);
  }

  {
    // -------------------- Reversing -------------------------------------------------

    printf ("\nReversing:\n");
    List x = make_list_from_array (n, array);
    reverse (x);
    print_list (x);
    free_list (x);
  }
}
