// Functions return lists/links, see e.g. append().
// If `fake_malloc` is activated this program CRASHES(!!!) every now and then. This is on purpose!

#define ARRAY_SIZE(a) (sizeof a / sizeof *a)

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// ml = malloc
void *(*ml) (size_t) = malloc;

// Memory allocation fails on average every 100th time.
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

// Comment out next line to use standard `malloc`, if uncommented `fake_malloc` is used.
// #define malloc fake_malloc

// "Pointers are both necessary and sufficient to implement recursive data structures in C".
// "I have named the structure `link` rather than `list`. A `list` will be a pointer to a sequence, or chain, of such
// `links`".
typedef struct link
{
  int          value; // list of integers
  struct link *next;
} Link;

// 11_1_2_pointer-singly-linked-lists.c will provide a different definition.

// A list is a link (and vice-versa).
// Correctly freeing memory is practically always a concern with recursive data structures.
// The problem is that they tend to create aliases for the same memory.
typedef Link List;

// Returns link that points to `next`.
// Prepends `next`.
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

// Returns list that points to `next`.
// Prepends `next`.
List *
new_list (int val, List *list)
{
  return new_link (val, list);
}

void
free_list (List *list)
{
  while (list)
    {
      Link *next = list->next; // Save `list->next`. We cannot get it after calling `free`.
      free (list);
      list = next;
    }
}

List *
make_list_from_array (int n, int array[n])
{
  List *list = NULL;
  for (int i = n - 1; i >= 0; i--)
    {
      Link *newlist = new_list (array[i], list);
      if (!newlist)
        {
          free_list (list);
          return NULL;
        }
      list = newlist;
    }
  return list;
}

void
print_list (List *list)
{
  printf ("[ ");
  while (list)
    {
      printf ("%d ", list->value);
      list = list->next;
    }
  printf ("]\n");
}

bool
contains (List *list, int val)
{
  while (list)
    {
      if (list->value == val)
        {
          return true;
        }
      list = list->next;
    }
  return false;
}

List *
prepend (List *list, int val)
{
  List *newlist = new_list (val, list);
  if (!newlist)
    {
      free_list (list);
    }
  return newlist;
}

// `append()` returns the original list pointer.
// Unless there was an error (returns NULL) or the original list was empty (returns new list).
List *
append (List *list, int val)
{
  Link *val_link = new_link (val, NULL);
  if (!val_link)
    {
      free_list (list);
      return NULL;
    }
  if (!list)
    {
      return val_link;
    }

  Link *last = list;
  while (last->next)
    {
      last = last->next;
    }
  last->next = val_link;
  return list;
}

List *
concatenate (List *x, List *y)
{
  if (!x)
    {
      return y;
    }
  Link *last = x;
  while (last->next)
    {
      last = last->next;
    }
  last->next = y;
  return x;
}

// Alternative `append` version (not a very good one).
List *
append1 (List *list, int val)
{
  // `new_link` could fail!
  return concatenate (list, new_link (val, NULL));
}

List *
delete_value (List *list, int val)
{
  if (!list)
    {
      return NULL;
    }

  if (list->value == val)
    {
      Link *next = list->next;
      free (list);
      return delete_value (next, val);
    }
  else
    {
      list->next = delete_value (list->next, val);
      return list;
    }
}

List *
reverse (List *list)
{
  if (!list)
    {
      return NULL;
    }

  Link *next     = list->next;
  List *reversed = list;
  reversed->next = NULL;

  while (next)
    {
      Link *next_next = next->next;
      next->next      = reversed;
      reversed        = next;
      next            = next_next;
    }
  return reversed;
}

#define c(v) contains (list, v) ? "✓" : "✗"

int
main ()
{
  srand (time (0));
  int array[] = { 1, 2, 3, 4, 5, 1, 2, 3, 4, 5 };
  int n       = ARRAY_SIZE (array);

  {
    printf ("-- Containing ------------------------------------\n\n");
    List *list = make_list_from_array (n, array);
    if (!list)
      {
        perror ("List error: ");
        exit (EXIT_FAILURE);
      }
    print_list (list);
    printf ("contains\n");
    printf ("%d %d %d\n", 0, 3, 6);

    printf ("%s %s %s\n", c (0), c (3), c (6));
    free_list (list);
  }

  {
    printf ("\n-- Prepend/Append --------------------------------\n\n");
    List *list = make_list_from_array (n, array);
    if (!list)
      {
        perror ("List error: ");
        exit (EXIT_FAILURE);
      }
    print_list (list);

    // This is the natural way to write code ...
    list = append (list, 6);
    list = prepend (list, 0);
    print_list (list);

    // ... but error handling forces us to keep the old list around.
    List *new_list = append (list, 6);
    if (!new_list)
      {
        perror ("List error: ");
        exit (EXIT_FAILURE);
      }
    list = new_list;

    new_list = prepend (list, 0);
    if (!new_list)
      {
        perror ("List error: ");
        exit (EXIT_FAILURE);
      }
    list = new_list;

    print_list (list);
    free_list (list);
  }

  {
    printf ("\n-- Memory Issues ---------------------------------\n\n");
    List *list     = make_list_from_array (n, array);
    List *new_list = prepend (list, -1);
    if (!new_list)
      {
        exit (EXIT_FAILURE);
      }
    List *list2 = new_list;

    print_list (list);
    print_list (list2);

    free_list (list);
    // WARNING! → `list2->next` points to garbage!
    // free_list (list2); // But we cannot delete the list.
    free (list2); // But we can delete the link.
  }

  {
    printf ("\n-- Concatenate -----------------------------------\n\n");
    Link *list1 = make_list_from_array (n, array);
    if (!list1)
      {
        perror ("List error: ");
        exit (EXIT_FAILURE);
      }
    printf ("list1: ");
    print_list (list1);

    Link *list2 = make_list_from_array (n, array);
    if (!list2)
      {
        perror ("List error: ");
        exit (EXIT_FAILURE);
      }
    printf ("list2: ");
    print_list (list2);

    List *list3 = concatenate (list1, list2);
    printf ("after concatenation:\nlist1: ");
    print_list (list1);
    printf ("list2: ");
    print_list (list2);
    printf ("list3: ");
    print_list (list3);
    free_list (list3); // this also deletes list and list2!
    // If we had deleted list2, we couldn't delete list/list2 now!
  }

  {
    printf ("\n-- Alternative Append ----------------------------\n\n");
    List *list = make_list_from_array (n, array);
    if (!list)
      {
        perror ("List error: ");
        exit (EXIT_FAILURE);
      }
    print_list (list);

    list = append1 (list, 6);
    if (!list)
      {
        perror ("List error: ");
        exit (EXIT_FAILURE);
      }

    print_list (list);
    free_list (list);
  }

  {

    // delete-print macro
#define dp(v)                                                                                                          \
  list = delete_value (list, v);                                                                                       \
  print_list (list)

    printf ("\n-- Deleting Values -------------------------------\n\n");
    List *list = make_list_from_array (n, array);
    if (!list)
      {
        perror ("List error: ");
        exit (EXIT_FAILURE);
      }
    print_list (list);

    dp (1);
    dp (2);
    dp (3);
    dp (4);
    dp (5);
    free_list (list);

    printf ("\nin reverse order:\n");
    list = make_list_from_array (n, array);
    if (!list)
      {
        perror ("List error: ");
        exit (EXIT_FAILURE);
      }
    print_list (list);

    dp (5);
    dp (4);
    dp (3);
    dp (2);
    dp (1);
    free_list (list);
  }

  {
    printf ("\n-- Deleting First Link ---------------------------\n\n");
    List *list = make_list_from_array (n, array);
    print_list (list);
    List *list2 = delete_value (list, 1);
    print_list (list2);

    // The first link in `list` was freed, so we cannot use it.
    // This would lead to a segmentation fault:
    // print_list(list);

    free_list (list2);
    // Even if we hadn't deleted the first link in delete_value(),
    // we would have deleted the rest of the list now, so ...
    // free_list(list);
    // ... isn't possible.
  }

  {
    printf ("\n-- Reversing -------------------------------------\n\n");
    List *list;
    list = make_list_from_array (n, array);
    print_list (list);
    list = reverse (list);
    print_list (list);
    free_list (list);
  }
}
