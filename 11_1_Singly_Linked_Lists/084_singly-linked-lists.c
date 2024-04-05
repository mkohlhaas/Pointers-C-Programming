// functions return lists/links; see e.g. append()
// Because of fake_malloc() this program CRASHES(!!!) every now and then. This is on purpose!

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// fake malloc
void *(*ml)(size_t) = malloc;

void *fake_malloc(size_t size) {
  if (rand() % 100 == 0)
    return NULL;
  else
    return ml(size);
}

// comment out next line to use standard malloc()
// #define malloc fake_malloc

typedef struct link Link;

// "Pointers are both necessary and sufficient to implement recursive data structures in C."
typedef struct link {
  int value;
  Link *next;
} Link;

// a list is a link
// 085_pointer-singly-linked-lists.c will provide a better definition!
typedef Link List;

Link *new_link(int val, Link *next) {
  Link *link = malloc(sizeof *link);
  if (!link)
    return NULL;
  link->value = val;
  link->next = next;
  return link;
}

List *new_list(int val, List *list) { return new_link(val, list); }

void free_list(List *list) {
  while (list) {
    Link *next = list->next; // remember next, we cannot get it after free(link)
    free(list);
    list = next;
  }
}

List *make_list(int n, int array[n]) {
  List *list = NULL;
  for (int i = n - 1; i >= 0; i--) {
    Link *newlink = new_link(array[i], list);
    if (!newlink) {
      free_list(list);
      return NULL;
    }
    list = newlink;
  }
  return list;
}

void print_list(List *list) {
  printf("[ ");
  while (list) {
    printf("%d ", list->value);
    list = list->next;
  }
  printf("]\n");
}

bool contains(List *list, int val) {
  while (list) {
    if (list->value == val)
      return true;
    list = list->next;
  }
  return false;
}

List *prepend(List *list, int val) __attribute__((warn_unused_result));

List *prepend(List *list, int val) {
  List *newlist = new_list(val, list);
  if (!newlist)
    free_list(list);
  return newlist;
}

// append() returns the original list pointer
// unless there was an error (returns NULL) or the original list was empty (returns new list)
List *append(List *list, int val) __attribute__((warn_unused_result));

List *append(List *list, int val) {
  Link *val_link = new_link(val, NULL);
  if (!val_link) {
    free_list(list);
    return NULL;
  }
  if (!list)
    return val_link;

  Link *last = list;
  while (last->next)
    last = last->next;
  last->next = val_link;
  return list;
}

List *concatenate(List *x, List *y) {
  if (!x)
    return y;
  Link *last = x;
  while (last->next)
    last = last->next;
  last->next = y;
  return x;
}

// alternative append version
List *append_(List *list, int val) { return concatenate(list, new_link(val, NULL)); }

List *delete_value(List *list, int val) __attribute__((warn_unused_result));

List *delete_value(List *list, int val) {
  if (!list)
    return NULL;
  if (list->value == val) {
    Link *next = list->next;
    free(list);
    return delete_value(next, val);
  } else {
    list->next = delete_value(list->next, val);
    return list;
  }
}

List *reverse(List *list) {
  if (!list)
    return NULL;
  Link *next = list->next;
  List *reversed = list;
  reversed->next = NULL;
  while (next) {
    Link *next_next = next->next;
    next->next = reversed;
    reversed = next;
    next = next_next;
  }
  return reversed;
}

int main() {
  srand(time(0));
  int array[] = {1, 2, 3, 4, 5};
  int n = sizeof array / sizeof *array;

  {
    // ==================== Contains ====================

    List *list = make_list(n, array);
    if (!list) {
      perror("List error: ");
      exit(1);
    }
    printf("Contains:\n");
    printf("%d %d %d\n", contains(list, 0), contains(list, 3), contains(list, 6));
    free_list(list);
  }

  {
    // ==================== Prepend/Append ==============

    printf("Prepend/Append\n");
    List *list = make_list(n, array);
    if (!list) {
      perror("List error: ");
      exit(1);
    }
    // This is the natural way to write code...
    list = append(list, 6);
    list = prepend(list, 0);

    // The error handling forces us to keep the old list around
    List *new_list = append(list, 6);
    if (!new_list) {
      perror("List error: ");
      exit(1);
    }
    list = new_list;

    new_list = prepend(list, 0);
    if (!new_list) {
      perror("List error: ");
      exit(1);
    }
    list = new_list;

    print_list(list);
    free_list(list);
  }

  {
    // ==================== Memory issues ===============

    printf("Memory issues:\n");
    List *list = make_list(n, array);
    List *new_list = prepend(list, -1);
    if (!new_list)
      exit(1);
    List *list2 = new_list;

    print_list(list);
    print_list(list2);

    free_list(list);
    // WARNING WARNING WARNING!
    // Now, list2->next points to garbage!
    free(list2); // but we can delete just the link...
  }

  {
    // ==================== Concatenate =================

    printf("Concatenate:\n");
    Link *list = make_list(n, array);
    if (!list) {
      perror("List error: ");
      exit(1);
    }
    Link *list2 = make_list(n, array);
    if (!list2) {
      perror("List error: ");
      exit(1);
    }

    List *list3 = concatenate(list, list2);
    print_list(list);
    print_list(list3);
    free_list(list3); // this also deletes list and list2!
    // If we had deleted list2, we couldn't delete list/list2 now!
  }

  {
    // ==================== Alternate append ============

    printf("Alternative append:\n");
    List *list = make_list(n, array);
    if (!list) {
      perror("List error: ");
      exit(1);
    }

    list = append_(list, 6);
    if (!list) {
      perror("List error: ");
      exit(1);
    }

    print_list(list);
    free_list(list);
  }

  {
    // ==================== Deleting values =============

    printf("Deleting values:\n");
    List *list = make_list(n, array);
    if (!list) {
      perror("List error: ");
      exit(1);
    }
    list = delete_value(list, 2);
    list = delete_value(list, 3);
    print_list(list);
    free_list(list);
  }

  {
    // ==================== Deleting first link =========

    printf("Deleting first link:\n");
    List *list = make_list(n, array);
    List *list2 = delete_value(list, 1);
    print_list(list2);

    // the first link in list was freed, so we cannot use it.
    // This would be an error: print_list(list);

    free_list(list2);
    // Even if we hadn't deleted the first link in
    // delete_value(), we would have deleted the rest
    // of the list now, so free_list(list) isn't possible
  }

  {
    // ==================== Reversing ===================

    printf("Reversing:\n");
    List *list;
    list = make_list(n, array);
    list = reverse(list);
    print_list(list);
    free_list(list);
  }
}
