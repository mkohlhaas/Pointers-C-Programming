// adding a dummy element

#include <assert.h>
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

typedef struct link {
  int value;
  Link *next;
} Link;

// new definition of a list; a list is just a pointer to a link - actually to the first link in the list, the dummy link
typedef Link *List; // Link* -> Dummy Link -> Link -> ... -> Link -> NULL

Link *new_link(int val, Link *next) {
  Link *link = malloc(sizeof *link);
  if (!link)
    return NULL;
  link->value = val;
  link->next = next;
  return link;
}

// take a list - which is just a pointer to the first link in a list - and free all its links including the dummy link
void free_links(List x) {
  while (x) {
    Link *next = x->next;
    free(x);
    x = next;
  }
}

// new list is an empty list with a single dummy link; value could be anything
#define new_list() new_link(314159, NULL)
#define free_list(x) free_links(x)

List make_list(int n, int array[n]) {
  List x = new_list(); // create dummy link
  if (!x)
    return NULL;

  for (int i = n - 1; i >= 0; i--) {
    Link *link = new_link(array[i], x->next);
    if (!link) {
      free_list(x);
      return NULL;
    }
    x->next = link; // connect dummy to new link
  }
  return x;
}

void print_list(List x) {
  printf("[ ");
  Link *link = x->next; // skip dummy link
  while (link) {
    printf("%d ", link->value);
    link = link->next;
  }
  printf("]\n");
}

bool contains(List x, int val) {
  Link *link = x->next; // skip dummy link
  while (link) {
    if (link->value == val)
      return true;
    link = link->next;
  }
  return false;
}

bool prepend(List x, int val) {
  Link *link = new_link(val, x->next);
  if (!link)
    return false;
  x->next = link; // connect dummy link to new link
  return true;
}

// there is always a last link b/c of dummy link
Link *last_link(List x) {
  Link *prev = x; // points to dummy
  while (prev->next)
    prev = prev->next;
  return prev;
}

bool append(List x, int val) {
  Link *link = new_link(val, NULL);
  if (!link)
    return false;
  last_link(x)->next = link;
  return true;
}

void concatenate(List x, List y) {
  last_link(x)->next = y->next;
  y->next = NULL; // list y can still be freed but is empty - list consists only of dummy link
}

void delete_value(List x, int val) {
  Link *front = x; // pointer to dummy link
  Link *next;      // front and next are neighbors - front comes before next
  while (front) {
    while ((next = front->next) &&
           next->value == val) { // first expression will always be evaluated; will be NULL at end of list
      front->next = next->next;
      free(next);
    }
    front = next; // move to the next link
  }
}

void reverse(List x) {
  Link *next = x->next; // skip dummy; get first real link
  x->next = NULL;
  while (next) {
    Link *next_next;
    next_next = next->next;
    next->next = x->next;
    x->next = next;
    next = next_next;
  }
}

int main() {
  int success;
  srand(time(0));
  int array[] = {1, 2, 3, 4, 5};
  int n = sizeof(array) / sizeof(array[0]);

  {
    // ==================== Contains ==================================================

    List x = make_list(n, array);
    if (!x) {
      perror("Make list error:");
      exit(2);
    }
    print_list(x);
    printf("Contains 0, 3, 6:\n");
    printf("%d %d %d\n", contains(x, 0), contains(x, 3), contains(x, 6));
    free_list(x);
  }

  {
    // ==================== Prepend/Append ============================================

    printf("Append 6/Prepend 0\n");
    List x = make_list(n, array);
    if (!x) {
      perror("Make list error:");
      exit(1);
    }
    success = append(x, 6);
    if (!success) {
      perror("List error:");
      exit(1);
    }
    success = prepend(x, 0);
    if (!success) {
      perror("List error:");
      exit(1);
    }
    print_list(x);
    free_list(x);
  }

  {
    // ==================== Concatenate ===============================================

    printf("Concatenate:\n");
    List x = make_list(n, array);
    List y = make_list(n, array);
    concatenate(x, y);
    print_list(x);
    print_list(y);
    free_list(x);
    free_list(y);
  }

  {
    // ==================== Deleting values ===========================================

    printf("Deleting values 2, 3:\n");
    List x = make_list(n, array);
    print_list(x);
    delete_value(x, 2);
    delete_value(x, 3);
    print_list(x);
    free_list(x);
  }

  {
    // ==================== Deleting first link =======================================

    printf("Deleting first link:\n");
    List x = make_list(n, array);
    delete_value(x, 1);
    print_list(x);
    free_list(x);
  }

  {
    // ==================== Reversing =================================================

    printf("Reversing:\n");
    List x = make_list(n, array);
    reverse(x);
    print_list(x);
    free_list(x);
  }
}
