// modifies input lists (in place); does not return (new) list pointers
// see e.g. append()

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// fake malloc
void *(*ml)(size_t) = malloc;

void *fake_malloc(size_t size) {
  if (rand() % 100 == 0)
    return 0;
  else
    return ml(size);
}

// comment out next line to use standard malloc()
// #define malloc fake_malloc

typedef struct link Link;

typedef struct link {
  int value;
  Link *next; // if you take the address of 'next' you get a list (e.g. in last_link_list());
} Link;

// A list is a pointer to a link pointer - not an array of link pointers. An array is a whole memory block.
// Here every single link is malloced and connected via 'next' pointers.
// Another view: a list is a reference to a link pointer and can therefore be changed.
// Another view: a list is the address of a link pointer, e.g.
// Link* link = new_link(5, NULL); List list = &link;
// Adding a level of indirection:
typedef Link **List;
// Generic List:
//     List -> Link* -> Link     |-> Link    |-> Link    |-> ...   |->Link    |-> NULL
//                      value    |   value   |   value   |   ...   |  value   |
//                      next   --|   next  --|   next  --|   ... --|  next  --|
//
// Emtpy List:
//     List -> Link* -> NULL

// x is a List
#define is_empty_list(x) (*(x) == NULL)
#define get_link_pointer(x) *(x)

// no change
Link *new_link(int val, Link *next) {
  Link *link = malloc(sizeof *link);
  if (!link)
    return NULL;
  link->value = val;
  link->next = next;
  return link;
}

// formerly called free_list (but otherwise the same)
void free_links(Link *link) {
  printf("Freeing links: %p\n", link);
  while (link) {
    printf("Freeing %d - %p\n", link->value, link);
    Link *next = link->next;
    free(link);
    link = next;
  }
}

List new_list() {
  // new allocation in comparison to 084_singly-linked-lists.c
  List x = malloc(sizeof *x);
  if (x)
    get_link_pointer(x) = NULL;
  return x;
}

void free_list(List x) {
  free_links(get_link_pointer(x));
  free(x);
}

List make_list(int n, int array[n]) {
  List x = new_list();
  if (!x)
    return NULL;

  for (int i = n - 1; i >= 0; i--) {
    Link *link = new_link(array[i], get_link_pointer(x));
    if (!link) {
      free_list(x);
      return NULL;
    }
    get_link_pointer(x) = link; // point list to new link
  }
  return x;
}

void print_list(List x) {
  printf("[ ");
  Link *link = get_link_pointer(x);
  while (link) {
    printf("%d ", link->value);
    link = link->next;
  }
  printf("]\n");
}

bool contains(List x, int val) {
  Link *link = get_link_pointer(x);
  while (link) {
    if (link->value == val)
      return true;
    link = link->next;
  }
  return false;
}

bool prepend(List x, int val) {
  Link *link = new_link(val, get_link_pointer(x));
  if (!link)
    return false;
  get_link_pointer(x) = link; // point list to new link
  return true;
}

// find last link in list x, return its address which by definition is a list
List last_link_list(List x) {
  Link *current = get_link_pointer(x);
  if (current == NULL)
    return x;
  while (current->next)
    current = current->next;
  return &current->next; // we return the address where current->next is stored!
}

bool append(List x, int val) {
  Link *val_link = new_link(val, NULL);
  if (!val_link)
    return false;
  get_link_pointer(last_link_list(x)) = val_link; // get_link_pointer can also be used as left-value
  return true;
}

void concatenate(List x, List y) {
  get_link_pointer(last_link_list(x)) = get_link_pointer(y);
  get_link_pointer(y) = NULL;
}

// put list on the stack, not on the heap
typedef Link *StackList;

// x is a StackList
#define stack_to_list(x) (&x)
#define free_stack_list(x)                                                                                             \
  do {                                                                                                                 \
    free_links(x);                                                                                                     \
    x = NULL;                                                                                                          \
  } while (0)

// overwrite List x with List y; move y to x
void move_list(List x, List y) {
  free_links(get_link_pointer(x)); // empty List x
  concatenate(x, y); // concatenate an empty list and a second list, effectively moving the second to the first list
}

void delete_value(List x, int val) {
  if (is_empty_list(x))
    return;
  Link *current = get_link_pointer(x);
  if (current->value == val) {
    Link *next = current->next;
    free(current);
    get_link_pointer(x) = next;
    delete_value(x, val);
  } else {
    delete_value(&current->next, val);
  }
}

void reverse(List x) {
  if (is_empty_list(x))
    return;
  Link *reversed = get_link_pointer(x);
  Link *next = reversed->next;
  reversed->next = NULL;
  while (next) {
    Link *next_next = next->next;
    next->next = reversed;
    reversed = next;
    next = next_next;
  }
  get_link_pointer(x) = reversed;
}

int main() {
  srand(time(0));
  int array[] = {1, 2, 3, 4, 5};
  int n = sizeof array / sizeof *array;

  {
    // ==================== Create ======================

    printf("\33[38;5;206mCreate:\033[0m\n");
    List x = new_list();
    append(x, 1);
    append(x, 2);
    append(x, 3);
    append(x, 4);
    append(x, 5);
    print_list(x);
    free_list(x);
  }

  {
    // ==================== Contains ====================

    printf("\33[38;5;206mCreate from array:\033[0m\n");
    List x = make_list(n, array);
    if (!x) {
      perror("List error: ");
      exit(1);
    }
    print_list(x);
    printf("\33[38;5;206mContains 0, 3, 6?\033[0m\n");
    printf("%d %d %d\n", contains(x, 0), contains(x, 3), contains(x, 6));
    free_list(x);
  }

  {
    // ==================== Prepend/Append ==============

    printf("\33[38;5;206mAppend 6, Prepend 0\033[0m\n");
    List x = make_list(n, array);
    if (!x) {
      perror("List error: ");
      exit(1);
    }
    if (!append(x, 6)) {
      perror("Append error:");
      exit(1);
    };
    print_list(x);
    if (!prepend(x, 0)) {
      perror("Prepend error:");
      exit(1);
    };
    print_list(x);
    free_list(x);
  }

  {
    // ==================== Concatenate =================

    printf("\33[38;5;206mConcatenate:\033[0m\n");
    List x = make_list(n, array);
    if (!x) {
      perror("List error: ");
      exit(1);
    }
    List y = make_list(n, array);
    if (!y) {
      perror("List error: ");
      exit(1);
    }
    concatenate(x, y);
    print_list(x);
    print_list(y);
    free_list(x);
    free_list(y);
  }

  {
    // ==================== Stack List ==================

    printf("\33[38;5;206mStack List\033[0m\n");
    List x = make_list(n, array);
    StackList z = NULL; // a stack list is created on the stack
    print_list(x);
    print_list(stack_to_list(z));

    printf("\33[38;5;206mMoving to stack list\033[0m\n");
    move_list(stack_to_list(z), x);
    print_list(x);
    print_list(stack_to_list(z));
    // free_list(x);
    free_stack_list(z);
  }

  {
    // ==================== Delete ======================

    printf("\33[38;5;206mDeleting 1, 3, 10:\033[0m\n");
    // List x = make_list(n, array); if (!x) { perror("List error: "); exit(1); }
    List x = new_list();
    append(x, 1);
    append(x, 1);
    append(x, 3);
    append(x, 1);
    append(x, 3);
    delete_value(x, 1);
    // delete_value(x,  3);
    delete_value(x, 10);
    print_list(x);
    free_list(x);
  }

  {
    // ==================== Reverse =====================

    printf("\33[38;5;206mReversing:\033[0m\n");
    List x = make_list(n, array);
    if (!x) {
      perror("List error: ");
      exit(1);
    }
    print_list(x);
    reverse(x);
    print_list(x);
    free_list(x);
  }
}
