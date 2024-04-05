#include <assert.h>
#include <stdio.h>

int main() {
  int array[] = {0, 1, 2, 3, 4};
  int n = sizeof array / sizeof *array;
  int *jp = array;

  for (int i = 0; i < 5; i++) {
    assert(array + i == jp + i);
    assert(array[i] == jp[i]);
    assert(array[i] == *(array + i));
    assert(jp[i] == *(jp + i));
    assert(i[array] == i[jp]); // ???
  }

  int *ip = array;
  char *p = (char *)array;
  for (int i = 0; i < n; i++)
    printf("%p %p %p\n",
           array + i,            // int array has the correct offset
           ip + i,               // int*      has the correct offset
           p + i * sizeof(int)); // char*     jumps in bytes...

  putchar('\n');

  char *end = (char *)array + sizeof array;
  for (ip = array, p = (char *)array; p != end; ip++, p += sizeof *ip)
    printf("%p %p\n", ip, p);
}
