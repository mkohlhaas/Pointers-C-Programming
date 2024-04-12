#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
main ()
{
  int   *ip;
  int   *new_ip;
  size_t old_size;
  size_t new_size;

  {
    ip = malloc (10 * sizeof *ip);
    if (!ip)
      {
        /* handle error */
        // ...
      }
  }

  {
    new_ip
        = realloc (ip, 100 * sizeof *new_ip); // realloc doesn't free memory being pointed to by `ip` in case of failure
    if (!new_ip)
      {
        /* handle error */
        // ...
      }
    ip = new_ip; // new_ip could be the old pointer but not guaranteed

    free (ip);
  }

  {
    old_size = 10 * sizeof *ip; // 10 integers
    new_size = 10 * old_size;   // 100 integers

    ip = malloc (old_size);
    if (!ip)
      {
        exit (EXIT_FAILURE);
      }
  }

  {
    old_size = 10 * sizeof *ip;  // 10 integers
    new_size = 10 * old_size;    // 100 integers

    ip = realloc (ip, new_size); // possible memory leak(?)

    {                            // does the same as realloc if successful:
      new_ip = malloc (new_size);
      memmove (new_ip, ip, old_size);
      free (ip);                 // when realloc is successful it frees memory being pointed at by `ip`
    }

    ip = new_ip;
    free (ip);
  }

  {
    old_size = 10 * sizeof *ip;    // 10 integers
    new_size = 10 * old_size;      // 100 integers

    ip = malloc (10 * sizeof *ip); // 10 integers
    // Usual pattern is to check the return value and then write it to the old pointer.
    new_ip = realloc (ip, new_size);
    // free(ip); // BIG NO-NO!!! If realloc is succesful and
    // 1.) allocates new memory, it will free old pointer's memory
    // 2.) extends old memory, ip and new_ip are the same and must not be freed
    if (!new_ip)
      {          /* handle error */
      }
    ip = new_ip; // idiomatic; now ip has the new memory chunk

    free (ip);
  }

  {
    ip = malloc (10 * sizeof *ip); // 10 integers
    // Do not `realloc(ip, 0);`
    // `realloc` is allowed to return a NULL pointer or a valid pointer in case of success!
    // You don't know what it was!
    // Do this instead:
    free (ip);
    ip = NULL;
  }
}
