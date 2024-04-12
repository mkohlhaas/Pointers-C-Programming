#include "list.h"
#include <assert.h>
#include <stdio.h>

bool
is_sorted (List lst)
{
  Link *p = front (lst);
  while (p->next != lst)
    {
      if (p->value > p->next->value)
        {
          return false;
        }
      p = p->next;
    }
  return true;
}

// Move `y` to `x`; `new x` == `old y`.
// All previous items in `x` are deleted; `y` is emptied.
void
move_links (List x, List y)
{
  free_links (x);

  if (!is_empty (y))
    {
      *x = *y;
      connect_neighbours (x);
      clear_list (y);
    }
}

// -- Selection sort ------------------------------------------------------------------------------------------------ //

// Returns pointer to smallest element in `lst`.
Link *
get_smallest (List lst)
{
  assert (!is_empty (lst));
  Link *p   = front (lst);
  Link *res = p;

  while (p != lst)
    {
      if (p->value < res->value)
        {
          res = p;
        }
      p = p->next;
    }

  return res;
}

// 1. Get smallest element in list.
// 2. Remove it from list.
// 3. Append it to sorted/result list.
// 4. Move sorted list elements to original list.
void
selection_sort (List lst)
{
  // Create empty list on stack.
  ListHead sorted = init_list_head (sorted);

  while (!is_empty (lst))
    {
      Link *smallest = get_smallest (lst);
      unlink (smallest);
      append_link (&sorted, smallest);
    }

  // Move `sorted` list to `lst`.
  move_links (lst, &sorted);
}

// -- Insertion sort ------------------------------------------------------------------------------------------------ //

// Loop over sorted list from end to beginning.
// This is stable, i.e. preserve the order of links with the same value.
void
insert_sorted (List lst, Link *link)
{
  Link *p = last (lst);

  while (p != lst && p->value > link->value)
    {
      p = p->prev;
    }

  link_after (p, link);
}

// 1. Take first element from list.
// 2. Insert it at correct location in sorted/result list.
void
insertion_sort (List lst)
{
  // Create empty list on stack.
  ListHead sorted = init_list_head (sorted);

  while (!is_empty (lst))
    {
      Link *p = front (lst);
      unlink (p);
      insert_sorted (&sorted, p);
    }

  // Move `sorted` list to `lst`.
  move_links (lst, &sorted);
}

// -- Merge sort ---------------------------------------------------------------------------------------------------- //

// Merge result into `x`.
void
merge (List lst1, List lst2)
{
  // Create empty list on stack.
  ListHead merged = init_list_head (merged);

  while (!is_empty (lst1) && !is_empty (lst2))
    {
      Link *smaller = (front (lst1)->value < front (lst2)->value) ? front (lst1) : front (lst2);
      unlink (smaller);
      append_link (&merged, smaller);
    }

  concatenate (&merged, lst1); // append left-overs from lst1; also clears lst1
  concatenate (&merged, lst2); // append left-overs from lst2; also clears lst2
  move_links (lst1, &merged);
}

// Split up list `x`.
// List `y` is initially empty and will receive every other node from list `x`.
void
split_list (List x, List y)
{
  assert (is_empty (y));

  Link *p = front (x);

  while (p != x)
    {
      Link *q = p->next;
      if (q == x)
        {
          return; // we reached end of list `x`
        }
      unlink (p);
      append_link (y, p);
      p = q->next; // // skipping `q` (`q` stays in `x`) → take every other link
    }
}

// 1. Split list equally into two lists.
// 2. Mergesort each list recursively.
// 3. Merge both lists.
void
merge_sort (List lst1)
{
  if (is_empty (lst1) || front (lst1)->next == lst1)
    {
      return; // length zero or one-item lists are sorted
    }

  // Create empty list on the stack.
  ListHead lst2 = init_list_head (lst2);

  split_list (lst1, &lst2);
  merge_sort (lst1);
  merge_sort (&lst2);
  merge (lst1, &lst2);
}

// -- Quick sort ---------------------------------------------------------------------------------------------------- //

// Move all items > pivotVal in lst1 to lst2 (initially empty).
void
partition (List lst1, List lst2, int pivotVal)
{
  assert (is_empty (lst2));
  Link *p = front (lst1);

  while (p != lst1)
    {
      Link *next = p->next;
      if (p->value > pivotVal)
        {
          unlink (p);
          append_link (lst2, p);
        }
      p = next;
    }
}

// 1. Split list in two based on pivot point.
// 2. Quick sort both lists individually and recursively.
// 3. Concatenate sorted lists.
void
quick_sort (List lst1)
{
  if (is_empty (lst1) || front (lst1)->next == lst1)
    {
      return; // `lst1` is sorted
    }

  // Remove the pivot, to make sure that we reduce the problem size on each recursion!
  // If the pivot is the largest element in the list, e.g., all the elements are less than
  // or equal to it. Then we would recurse on the original data, entering an infinite recursion.
  Link *first = front (lst1); // first entry is pivot point
  unlink (first);
  int pivotVal = first->value;

  // Create empty list on stack.
  ListHead lst2 = init_list_head (lst2);
  partition (lst1, &lst2, pivotVal);
  quick_sort (lst1);
  quick_sort (&lst2);

  append_link (lst1, first); // get `first` back into `lst1`
  concatenate (lst1, &lst2);
}

// Testing ---------------------------------------------------------------------------------------------------------- //

List
random_list (int n)
{
  List lst = new_list ();
  if (!lst)
    {
      abort ();
    }

  for (int i = 0; i < n; i++)
    {
      if (!append (lst, rand () % 10))
        {
          abort ();
        }
    }
  return lst;
}

void
test_sorting (int n)
{
  typedef void (*SortFun) (List);

  typedef struct sortfn_name
  {
    SortFun sort_fun;
    char   *sort_name;
  } SortfnName;

  SortfnName sort_funs[] = {
    { selection_sort, "Selection " },
    { insertion_sort, "Insertion " },
    { merge_sort, "Merge     " },
    { quick_sort, "Quick     " },
  };

  size_t n_funs = sizeof sort_funs / sizeof *sort_funs;

  List x = random_list (n);
  printf ("Original  ");
  print_list (x);

  for (size_t i = 0; i < n_funs; i++)
    {
      List y = copy_list (x);
      sort_funs[i].sort_fun (y);
      printf ("%s", sort_funs[i].sort_name);
      print_list (y);
      assert (is_sorted (y));
      free_list (y);
    }

  printf ("-----------------------------------------------------\n");
  free_list (x);
}

int
main ()
{
  unsigned random_seed;
  FILE    *fp = fopen ("/dev/urandom", "r");
  fread (&random_seed, sizeof random_seed, 1, fp);
  fclose (fp);
  srand (random_seed);
  test_sorting (20);
}
