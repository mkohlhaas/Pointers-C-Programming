#include <assert.h>
#include <stdio.h>

int
main()
{
  // 'A' is an array of 3 int pointers: int* [3]
  // Arrays decay to pointers when we use them as such. And that is what we are doing here.
  // We define the rows as arrays, but we define A as an array of pointers,
  // so what we get is pointers to the three arrays we use as rows.
  int* A[] = { (int[]){ 1 }, (int[]){ 2, 3 }, (int[]){ 4, 5, 6 } };

  // struct S* s = A[0];
  // struct S* s = A[0][0];
  int n = sizeof A / sizeof *A;
  for   (int i = 0; i <  n; i++) {
    for (int j = 0; j <= i; j++) { printf("%d-(%p) ", A[i][j], &A[i][j]); }
    printf("\n");
  }

  assert(sizeof A       == 3 * sizeof(int*));
  assert(sizeof A[0]    ==     sizeof(int*));
  assert(sizeof A[1]    ==     sizeof(int*));
  assert(sizeof A[2]    ==     sizeof(int*));
  assert(sizeof A[0][0] ==     sizeof(int));

  int row0[] = { 1 };
  int row1[] = { 2, 3 };
  int row2[] = { 4, 5, 6 };
  int* B[]   = { row0, row1, row2 };

  assert(sizeof B    == sizeof A);
  assert(sizeof B[0] == sizeof A[0]);

  // struct S* s = A;
  // int** can be interpreted as an array of integers
  // When we use A as a pointer, it decays to one. It is an array of int*, so it decays to
  // a pointer to that type -> it becomes an int**.
  int** p_A = A;
  assert(p_A[0]    == A[0]);
  assert(p_A[1]    == A[1]);
  assert(p_A[0][0] == A[0][0]);

  int* p_A1 = A[1];
  assert(p_A1[0] == A[1][0]);
  assert(p_A1[1] == A[1][1]);
}
