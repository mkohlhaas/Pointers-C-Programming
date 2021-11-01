#include <stddef.h>
#include <stdio.h>

typedef struct
{
  char c[2];
  int  array[];
} S;

typedef struct
{
  char c[2];
  char array[];
} T;

typedef struct
{
  void* p;
  int   i;
  char  array[];
} U;

int
main()
{
  printf("%zu %zu\n", sizeof(S), offsetof(S, array));
  printf("%zu %zu\n", sizeof(T), offsetof(T, array));
  printf("%zu %zu\n", sizeof(U), offsetof(U, array));

  U* u = NULL;
  printf("%zu %zu\n", sizeof *u, offsetof(__typeof__(*u), array));
  printf("%zu %zu\n", sizeof *u, (char*)&(*u).array - (char*)u);
}
