#include <stdio.h>

#define SIZE 5

int
main()
{
  int array[SIZE];
  printf("array           = %p\n", array);
  for (int i = 0; i < SIZE; i++)
    printf("&array[%d]       = %p\n", i, &array[i]);
  printf("sizeof(array)   = %zu\n", sizeof array);
  printf("%d * sizeof(int) = %zu\n", SIZE, SIZE * sizeof(int));
}
