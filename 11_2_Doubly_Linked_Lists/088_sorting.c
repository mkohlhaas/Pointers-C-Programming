#include "list.h"
#include <assert.h>
#include <stdio.h>
#include <time.h>

bool
is_sorted(List x)
{
  Link* p = front(x);
  while (p->next != x) {
    if (p->value > p->next->value) return false;
    p = p->next;
  }
  return true;
}

// move y to x; new x == old y
// all previous items in x are deleted; y is emptied
void
move_links(List x, List y)
{
  free_links(x);
  if (!is_empty(y)) {
    *x = *y;
    connect_neighbours(x);
    clear_list(y);
  }
}

// Selection sort ------------------------------
// 1. Get smallest element
// 2. Remove it from original list
// 3. Append it to sorted/resultant list (initially empty)
// 4. Move sorted list to original list

Link*
get_smallest(List x)
{
  assert(!is_empty(x));
  Link* p   = front(x);
  Link* res = p;
  while (p != x) {
    if (p->value < res->value) res = p;
    p = p->next;
  }
  return res;
}

void
selection_sort(List x)
{
  ListHead sorted = init_list_head(sorted);
  while (!is_empty(x)) {
    Link* smallest = get_smallest(x);
    unlink(smallest);
    append_link(&sorted, smallest);
  }
  move_links(x, &sorted);
}

// Insertion sort -----------------------------------------------
// 1. Take first element from list
// 2. Insert it at correct location in sorted/resultant list (initially empty)

#if 0
// Insert from the beginning. Not the usual insertion sort. Not stable
void insert_sorted(list x, link *link)
{
  link *p = front(x);
  while (p != x && p->value < link->value) p = p->next;
  link_before(p, link);
}
#else
// Insert from the end; this is stable -> preserve the order of links with the same value
void
insert_sorted(List x, Link* link)
{
  Link* p = last(x);
  while (p != x && p->value > link->value) p = p->prev;
  link_after(p, link);
}
#endif

#if 0
// Insert from the front; this is unstable
void insertion_sort(List x)
{
  ListHead sorted = init_list_head(sorted);
  Link *p = front(x);
  while (p != x) {
    Link* next = p->next;
    unlink(p);
    insert_sorted(&sorted, p);
    p          = next;
  }
  move_links(x, &sorted);
}
#else
void
insertion_sort(List x)
{
  ListHead sorted = init_list_head(sorted);
  while (!is_empty(x)) {
    Link* p = front(x);
    unlink(p);
    insert_sorted(&sorted, p);
  }
  move_links(x, &sorted);
}
#endif

// Merge sort -----------------------------------------------
// 1. Split list equally into two lists
// 2. Merge sort each list recursively
// 3. Merge both lists

#if 0
void merge(List x, List y)
{
  ListHead merged = init_list_head(merged);
  Link*    p      = front(x), *q = front(y);

  while( (p != x) && (q != y) ) {
    Link* smallest;
    if (p->value < q->value) {
      unlink(p);
      smallest = p;
      p        = p->next;
    } else {
      unlink(q);
      smallest = q;
      q        = q->next;
    }
    append_link(&merged, smallest);
  }

  concatenate(&merged, x);
  concatenate(&merged, y); // also clears y
  move_links(x, &merged);
}
#else
// merge result into x
void
merge(List x, List y)
{
  ListHead merged = init_list_head(merged);
  while (!is_empty(x) && !is_empty(y)) {
    Link* smallest = (front(x)->value < front(y)->value) ? front(x) : front(y);
    unlink(smallest);
    append_link(&merged, smallest);
  }
  concatenate(&merged, x);  // append left-overs
  concatenate(&merged, y);  // also clears y
  move_links(x, &merged);
}
#endif

#if 1
void
split_list(List x, List y)
{
  assert(is_empty(y));
  Link* p = front(x);
  while (p != x) {
    Link* q = p->next;   // take every other link
    if (q == x) return;
    unlink(p);
    append_link(y, p);
    p = q->next;
  }
}
#else
void
split_list(List x, List y)
{
  assert(is_empty(y));

  ListHead z = init_list_head(z);
  bool flag = true;
  while (!is_empty(x)) {
    List target = ((flag = !flag)) ? y : &z;
    Link* p = front(x);
    unlink(p);
    append_link(target, p);
  }
  move_links(x, &z);
}
#endif

void
merge_sort(List x)
{
  if (is_empty(x) || front(x)->next == x) return; // length zero or one-item lists are sorted by definition

  ListHead y = init_list_head(y);
  split_list(x, &y);
  merge_sort(x);
  merge_sort(&y);
  merge(x, &y);
}

// Quick sort -----------------------------------------------
// 1. Split list in two based on pivot point
// 2. Quick sort both lists individually and recursively
// 3. Append both lists

// move all items > pivot in x to y (initially empty)
void
partition(List x, List y, int pivot)
{
  assert(is_empty(y));
  Link* p = front(x);
  while (p != x) {
    Link* next = p->next;
    if (p->value > pivot) {
      unlink(p);
      append_link(y, p);
    }
    p = next;
  }
}

void
quick_sort(List x)
{
  if (is_empty(x) || front(x)->next == x) return; // length zero or one lists are sorted

  // remove the pivot, to make sure that we reduce the problem size on each recursion!
  // If the pivot is the largest element in the list, e.g., all the elements are less than
  // or equal to it. Then we would recurse on the original data, entering an infinite recursion.
  Link* first = front(x);
  unlink(first);
  int      pivot = first->value;
  ListHead y     = init_list_head(y);
  partition(x, &y, pivot);
  quick_sort(x);
  quick_sort(&y);
  append_link(x, first); // get first back into the list
  concatenate(x, &y);
}

// Testing --------------------------------------------------

List
random_list(int n)
{
  List x = new_list();
  if (!x) abort();

  for (int i = 0; i < n; i++)
    if (!append(x, rand() % 10)) abort();
  return x;
}

void
test_sorting(int n)
{
  typedef void (*SortFn_)(List);

  typedef struct sort_fn {
	  SortFn_ sort_fun;
	  char*   sort_name;
  } SortFn;

  SortFn sort_funs[] = { { selection_sort, "Selection " },
                         { insertion_sort, "Insertion " },
                         { merge_sort,     "Merge     " },
                         { quick_sort,     "Quick     " }, };

  size_t n_funs = sizeof sort_funs / sizeof *sort_funs;

  List x = random_list(n);
  printf("Original  ");
  print_list(x);

  for (size_t i = 0; i < n_funs; i++) {
    List y = copy_list(x);
    sort_funs[i].sort_fun(y);
    printf("%s", sort_funs[i].sort_name);
    print_list(y);
    assert(is_sorted(y));
    free_list(y);
  }

  printf("-----------------------------------------------------\n");
  free_list(x);
}

int
main()
{
  unsigned random_seed;
  FILE *fp = fopen("/dev/urandom", "r");
  fread(&random_seed, sizeof random_seed, 1, fp);
  fclose(fp);
  srand(random_seed);
  test_sorting(20);
}
