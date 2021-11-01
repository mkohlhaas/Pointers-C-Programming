#include <stdio.h>

int*
bin_search(int* left, int* right, int x)
{
  while (left < right) {
    // does not work (you CANNOT ADD pointers):
    // int* mid = (left + right) / 2; 
    int* mid = left + (right - left) / 2;
    if (*mid == x) return mid;
    if (*mid <  x) left  = mid + 1;
    else           right = mid;
  }
  return NULL;
}

int
main()
{
  int a[] = { 1, 2, 4, 8, 16 };    // sorted array
  int n   = sizeof a / sizeof *a;

  for (int i = 0; i < 10; i++) {
    int* res = bin_search(a, a + n, i);
    printf("bin_search(%d): %2d, %p\n", i, res ? *res : -1, res);
  }
}
