#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
main()
{
  int* ip = malloc(10 * sizeof *ip);
  if (!ip) { /* handle error */ }

  int* new_ip = realloc(ip, 100 * sizeof *new_ip);
  if (!new_ip) { /* handle error */ }
  ip = new_ip;

  free(ip);

  size_t old_size = 10 * sizeof *ip;
  size_t new_size = 10 * old_size;
  ip = malloc(old_size);
  if (!ip) exit(1);

  // ip = realloc(ip, new_size); // possible memory leak
  // does the same
  new_ip = malloc(new_size);
  memmove(new_ip, ip, old_size);
  free(ip);
  ip = new_ip;

  free(ip);

  // usual pattern is to check the return value and then write it to the old pointer
  ip     = malloc(10 * sizeof *ip);
  new_ip = realloc(ip, new_size);
  // free(ip); // BIG NO-NO!!! If realloc is succesful and
  // 1.) allocates new memory, it will free old pointer's memory
  // 2.) extends old memory, ip and new_ip are the same and must not be freed
  if (!new_ip) { /* handle error */ }
  ip = new_ip; // idiomatic; now ip has the new memory chunk

  free(ip);

  ip = malloc(10 * sizeof *ip);
  // do not
  // realloc(ip, 0);
  // is allowed to return a NULL pointer or a valid pointer in case of success!
  // you don't know what it was!

  // do this instead
  free(ip);
  ip = NULL;
}
