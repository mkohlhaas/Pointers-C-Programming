#include "list.h"
#include <stdio.h>
#include <stdlib.h>

list *
new_list (list_api api)
{
  list *lst = malloc (sizeof *lst);
  if (lst)
    {
      *lst = (list){
        .head = { .next = &lst->head, .prev = &lst->head },
        .api  = api,
      };
    }
  return lst;
}

void
free_list (list *lst)
{
  free_link_fn free_link = lst->api.free;
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
default_print_link (link *lnk)
{
  printf ("<link %p>", (void *)lnk);
}

void
print_list (list *lst)
{
  print_link_fn print = lst->api.print ? lst->api.print : default_print_link;
  printf ("[ ");
  for (link *lnk = front (lst); lnk != head (lst); lnk = lnk->next)
    {
      print (lnk);
      putchar (' ');
    }
  printf ("]\n");
}

link *
find_link (list *lst, link *from, pred_fn pred_fn)
{
  for (link *lnk = from; lnk != head (lst); lnk = lnk->next)
    {
      if (pred_fn (lnk))
        {
          return lnk;
        }
    }
  return NULL;
}

void
delete_if (list *lst, pred_fn pred_fn)
{
  free_link_fn free = lst->api.free;
  link        *lnk  = front (lst);
  while (lnk != head (lst))
    {
      link *next = lnk->next;
      if (pred_fn (lnk))
        {
          unlink (lnk);
          if (free)
            {
              free (lnk);
            }
        }
      lnk = next;
    }
}
