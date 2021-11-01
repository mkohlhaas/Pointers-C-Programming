#include "list.h"

int
main()
{
  int array[] = { 1, 2, 3, 4, 5 };
  int n       = sizeof(array) / sizeof(array[0]);

  {
    // ==================== Contains ========================================

    printf("\33[38;5;206mContains:\033[0m\n");
    List x = make_list(n, array); if (!x) { perror("List error: "); exit(1); }
    printf("%d %d %d\n", contains(x, 0), contains(x, 3), contains(x, 6));
    free_list(x);
  }

  {
    // ==================== Prepend/Append ==================================

    int success;
    printf("\33[38;5;206mAppend 6/Prepend 0:\033[0m\n");
    List x  = make_list(n, array); if (!x) { perror("List error: "); exit(1); }
    success = append (x, 6); if (!success) { perror("List error: "); exit(1); }
    success = prepend(x, 0); if (!success) { perror("List error: "); exit(1); }
    print_list(x);
    free_list(x);
  }

  {
    // ==================== Concatenate =====================================

    printf("\33[38;5;206mConcatenate:\033[0m\n");
    List x = make_list(n, array); if (!x) { perror("List error: "); exit(1); }
    List y = make_list(n, array); if (!y) { perror("List error: "); exit(1); }
    concatenate(x, y);  // concatenate() doesn't free y, it only empties it 
    assert(y->prev == y);
    assert(y->next == y);
    assert(is_empty(y));
    print_list(x); print_list(y);
    free_list(x);  free_list(y);

    x = make_list(n, array);
    y = new_list();
    concatenate(x, y);
    print_list(x); print_list(y);
    free_list(x);  free_list(y);

    x = new_list();
    y = make_list(n, array);
    concatenate(x, y);
    print_list(x); print_list(y);
    free_list(x);  free_list(y);
  }

  {
    // ==================== Deleting values =================================

    printf("\33[38;5;206mDeleting values:\033[0m\n");
    List x = make_list(n, array); if (!x) { perror("List error: "); exit(1); }
    delete_value(x, 2);
    delete_value(x, 3);
    print_list(x);
    free_list(x);
  }

  {
    // ==================== Deleting first link =============================

    printf("\33[38;5;206mDeleting first link:\033[0m\n");
    List x = make_list(n, array);
    delete_value(x, 1);
    print_list(x);
    free_list(x);
  }

  {
    // ==================== Reversing =======================================

    printf("\33[38;5;206mReversing:\033[0m\n");
    List x = make_list(n, array);
    reverse(x);
    print_list(x);
    free_list(x);
  }

  {
    // ==================== List on Stack ===================================

    printf("\33[38;5;206mList on Stack:\033[0m\n");
    ListHead head = init_list_head(head);
    append(&head, 1);
    append(&head, 2);
    print_list(&head);
    free_links(&head);
  }

  {
    // ==================== Copy List =======================================

    printf("\33[38;5;206mCopy List:\033[0m\n");
    List x = make_list(n, array);
    List z = copy_list(x);
    print_list(x); print_list(z);
    assert(equal(x, z));
    delete_value(z, 5);
    print_list(x); print_list(z);
    assert(!equal(x, z));
    free_list(x); free_list(z);
  }
}
