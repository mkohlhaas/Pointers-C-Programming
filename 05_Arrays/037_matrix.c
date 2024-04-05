#include <stdio.h>

// C = A * B
void matrix_mult(int n, int m, int l, double C[n][m], double const A[n][l], double const B[l][m]) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      C[i][j] = 0.0;
      for (int k = 0; k < l; k++) {
        C[i][j] += A[i][k] * B[k][j];
      }
    }
  }
}

void print_matrix(int n, int m, double const A[n][m]) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      printf("%2.2f ", A[i][j]);
    }
    printf("\n");
  }
}

int main() {
  double A[2][3] = {{1, 2, 3}, {4, 5, 6}};
  double B[3][2] = {{1, 2}, {3, 4}, {5, 6}};
  double C[2][2];

  matrix_mult(2, 2, 3, C, A, B); // C = A * B
  print_matrix(2, 2, C);
}
