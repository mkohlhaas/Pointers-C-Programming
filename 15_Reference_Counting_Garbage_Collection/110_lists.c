#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// immutable list link
typedef struct link Link;

typedef struct link
{
  int          refcount;
  int   const  value;
  Link* const  next;
} Link;

typedef Link* List;

// x is a List
Link    NIL_LINK  = { .refcount = 1 };
#define is_error(x) ((x) == NULL)
#define is_nil(x)   ((x) == &NIL_LINK)
#define NIL          incref(&NIL_LINK)

Link*
incref(Link* link)
{
  link->refcount++;
  return link;
}

void free_link(Link* link);

void
decref(Link* link)
{
  if (link && --link->refcount == 0) free_link(link);
}

void
free_link(Link* link)
{
  decref(link->next);
  free(link);
}

// Annotations:
// 'takes'   takes reference ownership (and can decrement reference count and give away ownership)
// 'borrows' leaves reference ownership untouched and does not give away ownership
//           you can get a new reference - thereby owning it - by incref'ing an existing one and give that away,
// 'give'    gives away reference ownership
#define takes
#define borrows
#define give(x) x

#if 0
// GCC compiler extension statement-expression for give()
#define give(x)                                                                \
  ({                                                                           \
    void* p = x;                                                               \
    x       = NULL;                                                            \
    p;                                                                         \
  })
#endif

List
new_link(int head, takes List tail)
{
  if (is_error(tail)) return NULL;

  List link = malloc(sizeof *link);
  if (!link) {
    decref(tail);  // new_link() owns tail and has to decref() it
    return NULL;
  }

  Link link_data = { .refcount = 1,
                     .next     = give(tail), // gives away the reference to the link
                     .value    = head };
  // getting around const'ness of value and next by memcpy'ing
  memcpy(link, &link_data, sizeof *link);
  return give(link);
}

void
print_list(borrows List x)
{
  printf("[ ");
  while (!is_nil(x)) {
    printf("%d[%d] ", x->value, x->refcount);
    x = x->next;
  }
  printf("]\n");
}

// LENGTH ------------------------------------------
#if 0
int length_rec(borrows list x, int acc)
{
  assert(!is_error(x));
  if (is_nil(x))
    return acc;
  else
    return length_rec(x->next, acc + 1);
}

#define length(x) length_rec(x, 0)

#else

int
length_rec(takes List x, int acc)
{
  assert(!is_error(x));
  if (is_nil(x)) {
    decref(x);  // decref a taken list - in this case the empty list (NIL-LINK) - before returning
    return acc;
  } else {
    Link* next = incref(x->next);  // we need a reference to x->next as we will pass ownership in recursive call
    decref(x);  // same here
    return length_rec(give(next), acc + 1);  // pass ownership of next
  }
}
#define length(x) length_rec(x, 0)

#endif

// REVERSE --------------------------------------
// returns a new list as lists are immutable

#if 0
list reverse_rec(borrows list x, borrows list acc)
{
  if (is_error(x) || is_error(acc)) {
    return NULL;
  }
  if (is_nil(x)) {
    return incref(acc);  // incref a borrowed reference when returning
  } else {
    return reverse_rec(x->next, new_link(x->value, incref(acc))); // new_link takes list, so we need to incref before
  }
}

#define reverse(x) reverse_rec(x, NIL)

#else

List
reverse_rec(takes List x, takes List acc)
{
  if (is_error(x) || is_error(acc)) {
    decref(x);    // decref taken lists
    decref(acc);
    return NULL;
  }
  if (is_nil(x)) {
    decref(x);  // we don't give away x so we must decref it as we are the owner
    return give(acc);
  } else {
    int   value = x->value;
    Link* next  = incref(x->next);
    decref(x);
    return reverse_rec(give(next), new_link(value, acc));
  }
}

#define reverse(x) reverse_rec(x, NIL)

#endif

// CONCAT --------------------------------------
#if 0
list concat(borrows list x, borrows list y)
{
  if (is_error(x) || is_error(y)) {
    return NULL;
  }
  if (is_nil(x)) {
    return incref(y); // we return a new ref, so we must incref here
  } else {
    return new_link(x->value, concat(x->next, incref(y)));
  }
}

#else

List
concat(takes List x, takes List y)
{
  if (is_error(x) || is_error(y)) {
    decref(x);
    decref(y);
    return NULL;
  }
  if (is_nil(x)) {
    decref(x);
    return give(y);
  } else {
    int   value = x->value;
    Link* next  = incref(x->next);
    decref(x);
    return new_link(value, concat(give(next), give(y)));
  }
}

#endif

int
main()
{
  printf("Construction:\n");
  printf("-------------\n");
  List x = new_link(1, new_link(2, new_link(3, NIL)));
  print_list(x);

  List y = new_link(0, incref(x));
  print_list(y);

  decref(x);
  print_list(y);

  List z = new_link(-1, give(y)); // consider y gone!
  print_list(z);
  decref(z);

  printf("\nLength:\n");
  printf("-------\n");
  x = new_link(1, new_link(2, new_link(3, NIL)));
  print_list(x);
  printf("len(x) = %d\n", length(incref(x)));
  print_list(x);
  // leaks if length doesn't free its input
  printf("len(x) = %d\n", length(new_link(1, new_link(2, new_link(3, NIL)))));
  // decref(x);
  // this frees x
  length(give(x));

  printf("\nReverse:\n");
  printf("--------\n");
  x = new_link(1, new_link(2, new_link(3, NIL)));
  print_list(x);
  y = reverse(give(x));
  print_list(y);
  decref(y);

  // would leak if reverse and length didn't free their input
  printf("%d\n", length(reverse(new_link(1, new_link(2, new_link(3, NIL))))));

  printf("\nConcat:\n");
  printf("-------\n");
  x = new_link(1, new_link(2, new_link(3, NIL)));
  y = new_link(4, new_link(5, NIL));
  z = concat(incref(x), incref(y));

  print_list(z);
  decref(x);
  decref(y);
  print_list(z);
  decref(z);

  z = concat(new_link(1, new_link(2, NIL)), new_link(11, new_link(12, new_link(13, NIL))));
  print_list(z);
  decref(z);
}
