// Modifies input lists in place by means of a reference pointer.
// → Does not return (new) list pointers.
// See e.g. append().

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

// Comment out next line to use standard `malloc`.
// #define malloc fake_malloc

typedef struct Link
{
  int          value;
  struct Link *next;
} Link;

// `List` points to the first `Link` in the chain of links.
// `List` is malloced → `List` is a pointer.
// Its content is also malloced (a `Link`) → pointer to a `Link` pointer.
// See e.g. `make_list_from_array()`.
typedef Link **List;

// Emtpy List:
//     List = NULL
//
// Non-Empty List:
//     List → Link   |→ Link   |→ Link   |→  ...  |→ Link   |→ NULL
//            next  -|  next  -|  next  -|   ... -|  next  -|
//            value  |  value  |  value  |   ...  |  value  |

#define is_empty_list(lst) (*(lst) == NULL)
#define first_link(lst)    *(lst)

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

void
free_links (Link *link)
{
  printf ("Freeing links:\n");
  while (link)
    {
      printf ("Freeing %p, val: %d\n", link, link->value);
      Link *next = link->next;
      free (link);
      link = next;
    }
}

List
new_empty_list ()
{
  List lst = malloc (sizeof (List)); // simply allocates a pointer
  if (lst)
    {
      first_link (lst) = NULL;
    }
  return lst;
}

void
free_list (List x)
{
  free_links (first_link (x));
  free (x);
}

List
make_list_from_array (int n, int array[n])
{
  List x = new_empty_list ();
  if (!x)
    {
      return NULL;
    }

  for (int i = n - 1; i >= 0; i--)
    {
      Link *newLink = new_link (array[i], first_link (x));
      if (!newLink)
        {
          free_list (x);
          return NULL;
        }
      first_link (x) = newLink;
    }
  return x;
}

void
print_list (List x)
{
  Link *link = first_link (x);
  printf ("List pointer: %p → %p\n", x, link);

  printf ("[ ");
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
  Link *link = first_link (x);
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
  Link *newLink = new_link (val, first_link (x));
  if (!newLink)
    {
      return false;
    }
  first_link (x) = newLink;
  return true;
}

// Returns address of last link in a list.
// It's a reference (read-write) to a `Link` pointer.
// By definition this is a `List`.
Link **
last_link_ref (List x)
{
  if (is_empty_list (x))
    {
      return x;
    }

  Link *current = first_link (x);
  while (current->next)
    {
      current = current->next;
    }

  return &current->next;
}

bool
append (List x, int val)
{
  Link *val_link = new_link (val, NULL);
  if (!val_link)
    {
      return false;
    }
  *(last_link_ref (x)) = val_link;
  return true;
}

void
concatenate (List x, List y)
{
  *(last_link_ref (x)) = first_link (y);
  first_link (y)       = NULL; // invalidate `y`; `y` is now an empty List.
}

// Put `List` on the stack, not on the heap.
typedef Link *StackList;

// `x` is a `List` on the stack.
#define stack_to_list(x) (&x)
#define free_stack_list(x)                                                                                             \
  do                                                                                                                   \
    {                                                                                                                  \
      free_links (x);                                                                                                  \
      x = NULL;                                                                                                        \
    }                                                                                                                  \
  while (0)

// Overwrite `List x` with `List y`; move `y` to `X`.
void
move_list (List x, List y)
{
  free_links (first_link (x)); // empty List x
  concatenate (x, y); // concatenate an empty list and a second list, effectively moving the second to the first list
}

void
delete_value (List x, int val)
{
  if (is_empty_list (x))
    {
      return;
    }

  Link *current = first_link (x);
  if (current->value == val)
    {
      Link *next = current->next;
      free (current);
      first_link (x) = next;
      delete_value (x, val);
    }
  else
    {
      delete_value (&current->next, val);
    }
}

void
reverse (List x)
{
  if (is_empty_list (x))
    {
      return;
    }

  Link *reversed = first_link (x);
  Link *next     = reversed->next;
  reversed->next = NULL;

  while (next)
    {
      Link *next_next = next->next;
      next->next      = reversed;
      reversed        = next;
      next            = next_next;
    }
  first_link (x) = reversed;
}

#define ARRAY_SIZE(a) (sizeof a / sizeof *a)

// Error Handling
#define EH(x, msg)                                                                                                     \
  ({                                                                                                                   \
    if (!x)                                                                                                            \
      {                                                                                                                \
        perror (msg);                                                                                                  \
        exit (EXIT_FAILURE);                                                                                           \
      }                                                                                                                \
  })

#define c(v) contains (x, v) ? "✓" : "✗"

int
main ()
{
  srand (time (0));
  int array[] = { 1, 2, 3, 4, 5, 1, 2, 3, 4, 5 };
  int n       = ARRAY_SIZE (array);

  {
    // -------------------- Create ----------------------

    printf ("\33[38;5;206mCreate:\033[0m\n\n");
    List x = new_empty_list ();
    EH (append (x, 1), "append error");
    EH (append (x, 2), "append error");
    EH (append (x, 3), "append error");
    EH (append (x, 4), "append error");
    EH (append (x, 5), "append error");
    print_list (x);
    free_list (x);
  }

  {
    // -------------------- Contains --------------------

    printf ("\n\33[38;5;206mCreate from array:\033[0m\n\n");
    List x = make_list_from_array (n, array);
    EH (x, "make list error");
    print_list (x);
    printf ("\n\33[38;5;206mContains\n0 3 6?\033[0m\n");
    printf ("%s %s %s\n\n", c (0), c (3), c (6));
    free_list (x);
  }

  {
    // -------------------- Prepend/Append --------------

    printf ("\n\33[38;5;206mAppend 6, Prepend 0\033[0m\n\n");
    List x = make_list_from_array (n, array);
    EH (x, "make list error");
    EH (append (x, 6), "append error");
    print_list (x);
    EH (prepend (x, 0), "prepend error");
    print_list (x);
    free_list (x);
  }

  {
    // -------------------- Concatenate -----------------

    printf ("\n\33[38;5;206mConcatenate:\033[0m\n\n");

    List x = make_list_from_array (n, array);
    EH (x, "make list error");

    List y = make_list_from_array (n, array);
    EH (y, "make list error");

    concatenate (x, y);

    print_list (x);
    print_list (y);

    free_list (x);
    free_list (y);
  }

  {
    // -------------------- Stack List ------------------

    printf ("\n\33[38;5;206mStack List\033[0m\n\n");
    List      x = make_list_from_array (n, array);
    StackList z = NULL; // List z is created on the stack
    print_list (x);
    print_list (stack_to_list (z));

    printf ("\n\33[38;5;206mMoving to stack list\033[0m\n\n");
    move_list (stack_to_list (z), x);
    print_list (x);
    print_list (stack_to_list (z));
    // free_list(x);
    free_stack_list (z);
  }

  {
    // -------------------- Delete ----------------------

    printf ("\n\33[38;5;206mDeleting 1, 3, 10:\033[0m\n\n");
    List x = new_empty_list ();

    EH (append (x, 1), "append error");
    EH (append (x, 1), "append error");
    EH (append (x, 3), "append error");
    EH (append (x, 1), "append error");
    EH (append (x, 3), "append error");
    print_list (x);

    delete_value (x, 1);
    delete_value (x, 10);
    print_list (x);
    free_list (x);
  }

  {
    // -------------------- Reverse ---------------------

    printf ("\n\33[38;5;206mReversing:\033[0m\n\n");
    List x = make_list_from_array (n, array);
    EH (x, "make list error");
    print_list (x);

    reverse (x);
    print_list (x);

    free_list (x);
  }
}
