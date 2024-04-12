#include "list.h"
#include <stdio.h>

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
  int array[] = { 1, 2, 3, 4, 5, 1, 2, 3, 4, 5 };
  int n       = ARRAY_SIZE (array);

  {
    // -------------------- Contains ----------------------------------------

    printf ("\33[38;5;206mContains\033[0m\n");
    List x = make_list_from_array (n, array);
    EH (x, "make list error");
    print_list (x);
    printf ("%d %d %d\n", 0, 3, 6);
    printf ("%s %s %s\n", c (0), c (3), c (6));
    free_list (x);
  }

  {
    // -------------------- Prepend/Append ----------------------------------

    int success;
    printf ("\33[38;5;206mAppend 6/Prepend 0:\033[0m\n");
    List x = make_list_from_array (n, array);
    EH (x, "make list error");
    print_list (x);
    EH (append (x, 6), "append list error");
    EH (prepend (x, 0), "prepend list error");
    print_list (x);
    free_list (x);
  }

  {
    // -------------------- Concatenate -------------------------------------

    printf ("\33[38;5;206mConcatenate:\033[0m\n");
    List x = make_list_from_array (n, array);
    EH (x, "make list error");
    print_list (x);
    List y = make_list_from_array (n, array);
    EH (y, "make list error");
    print_list (y);

    concatenate (x, y);
    assert (y->prev == y);
    assert (y->next == y);
    assert (is_empty (y));
    print_list (x);
    print_list (y);
    free_list (x);
    free_list (y);

    printf ("-------------------------------------------\n");
    x = make_list_from_array (n, array);
    print_list (x);
    y = new_list ();
    print_list (y);
    concatenate (x, y);
    print_list (x);
    print_list (y);
    free_list (x);
    free_list (y);

    printf ("-------------------------------------------\n");
    x = new_list ();
    print_list (x);
    y = make_list_from_array (n, array);
    print_list (y);
    concatenate (x, y);
    print_list (x);
    print_list (y);
    free_list (x);
    free_list (y);
  }

  {
    // -------------------- Deleting values ---------------------------------

    printf ("\33[38;5;206mDeleting values:\033[0m\n");
    List x = make_list_from_array (n, array);
    EH (x, "make list error");
    print_list (x);

    delete_value (x, 2);
    print_list (x);
    delete_value (x, 4);
    print_list (x);
    delete_value (x, 1);
    print_list (x);
    delete_value (x, 3);
    print_list (x);
    delete_value (x, 5);
    print_list (x);
    delete_value (x, 5);
    print_list (x);
    free_list (x);
  }

  {
    // -------------------- Deleting first link -----------------------------

    printf ("\33[38;5;206mDeleting first link:\033[0m\n");
    List x = make_list_from_array (n, array);
    print_list (x);
    delete_value (x, 1);
    print_list (x);
    free_list (x);
  }

  {
    // -------------------- Reversing ---------------------------------------

    printf ("\33[38;5;206mReversing:\033[0m\n");
    List x = make_list_from_array (n, array);
    print_list (x);
    reverse (x);
    print_list (x);
    free_list (x);
  }

  {
    // -------------------- List on Stack -----------------------------------

    printf ("\33[38;5;206mList on Stack:\033[0m\n");
    ListHead head = init_list_head (head);
    print_list (&head);
    append (&head, 1);
    append (&head, 2);
    print_list (&head);
    free_links (&head);
  }

  {
    // -------------------- Copy List ---------------------------------------

    printf ("\33[38;5;206mCopy List:\033[0m\n");
    List x = make_list_from_array (n, array);
    List z = copy_list (x);
    print_list (x);
    print_list (z);
    assert (equal (x, z));
    delete_value (z, 5);
    print_list (x);
    print_list (z);
    assert (!equal (x, z));
    free_list (x);
    free_list (z);
  }
}
