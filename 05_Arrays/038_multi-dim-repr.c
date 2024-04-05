#include <assert.h>
#include <stdio.h>

int main() {
  int A[2][3] = {{1, 2, 3}, {4, 5, 6}};
  int A2[][3] = {{1, 2, 3}, {4, 5, 6}};
  double A3[3][2] = {{1}, {3}};
  // 1 _
  // 3 _
  // _ _
  // values are filled row-wise
  double A4[3][2] = {1, 3};
  // 1 3
  // _ _
  // _ _

  assert(sizeof A == 2 * 3 * sizeof(int));
  assert(sizeof *A == 3 * sizeof(int));
  assert(sizeof A[0] == 3 * sizeof(int));
  assert(sizeof A[0][0] == sizeof(int));

  int *p = (int *)A;
  for (int i = 0; i < 2; i++) {
    assert(p == A[i]); // p points to the first element in row i
    for (int j = 0; j < 3; j++) {
      assert(p == A[i] + j); // p points to j-th column in i-th row
      assert(p == &A[i][j]);
      assert(*p == A[i][j]);
      p++;
    }
  } // incrementing p means going to next column

  int A5[][3] = {1, 2, 3, 4, 5, 6};
  // 1 2 3
  // 4 5 6
  int A6[2][2] = {1, 2, 3};
  // 1 2
  // 3 _
  int A7[2][2] = {{1}, {2, 3}};
  // 1 _
  // 2 3

  int B[2][2][3] = {{{1, 2, 3}, {4, 5, 6}}, {{7, 8, 9}, {10, 11, 12}}};
  int B2[][2][3] = {{{1, 2, 3}, {4, 5, 6}}, {{7, 8, 9}, {10, 11, 12}}};

  // C needs to know all except the first dimension to work out the size of the full table
  // int B3[][][3] = { { { 1, 2, 3 }, {  4,  5,  6 } },  // array has incomplete element type
  //                   { { 7, 8, 9 }, { 10, 11, 12 } } };

  assert(sizeof B == 2 * 2 * 3 * sizeof(int));
  assert(sizeof *B == 2 * 3 * sizeof(int));
  assert(sizeof B[0] == 2 * 3 * sizeof(int));
  assert(sizeof B[0][0] == 3 * sizeof(int));
  assert(sizeof B[0][0][0] == sizeof(int));

  p = (int *)B;
  for (int i = 0; i < 2; i++) {
    assert(p == (int *)B[i]); // p points to row i
    for (int j = 0; j < 2; j++) {
      assert(p == (int *)(B[i] + j)); // p points to j-th column in i-th row
      for (int k = 0; k < 3; k++) {
        assert(p == B[i][j] + k); // p points to k-th element in B[i][j]
        assert(p == &B[i][j][k]);
        assert(*p == B[i][j][k]);
        p++;
      }
    }
  }

  char x[4][3] = {
      {'a', 'b', 'c'}, // complete row
      {'d', 'e'},      // first 2 columns
      {'f'},           // only the first column
                       // the fourth row is not initialised
  };
  // 'a' 'b' 'c'
  // 'd' 'e'  _
  // 'f'  _   _
  //  _   _   _
  printf("Element [0][2]: %c\n", x[0][2]); // 'c'
  printf("Element [1][1]: %c\n", x[1][1]); // 'e'
}
