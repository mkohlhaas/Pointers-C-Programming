#include "list.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

list *
new_list (list_api api)
{
  list *lst = malloc (sizeof *lst);
  if (lst)
    {
      *lst = (list)
        {
          .head =
            {
              .next = &lst->head,
              .prev = &lst->head,
            },
          .api = api,
        };
    }
  return lst;
}

void
free_list (list *lst)
{
  void (*free_link) (link *) = lst->api.free;
  if (free_link)
    {
      link *lnk = front (lst);
      while (lnk != head (lst))
        {
          link *next = lnk->next;
          free_link (lnk);
          lnk = next;
        }
    }
  free (lst);
}

static void
print_link (link *lnk)
{
  printf ("<link %p>", (void *)lnk);
}

void
print_list (list *lst)
{
  void (*print) (link *) = (lst->api.print) ? lst->api.print : print_link;
  printf ("[ ");
  for (link *lnk = front (lst); lnk != head (lst); lnk = lnk->next)
    {
      print (lnk);
      putchar (' ');
    }
  printf ("]\n");
}

link *
find_link (list *lst, link *from, bool (*pred) (link *))
{
  for (link *lnk = from; lnk != head (lst); lnk = lnk->next)
    {
      if (pred (lnk))
        {
          return lnk;
        }
    }
  return NULL;
}

void
delete_if (list *lst, bool (*pred) (link *))
{
  void (*free) (link *) = lst->api.free;
  if (!free)
    {
      link *lnk = front (lst);
      while (lnk != head (lst))
        {
          link *next = lnk->next;
          if (pred (lnk))
            {
              unlink (lnk);
              free (lnk);
            }
          lnk = next;
        }
    }
}
