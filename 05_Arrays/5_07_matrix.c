#include <stdio.h>

// C = A * B
void
matrix_mult (int n, int m, int l, float C[n][m], float const A[n][l], float const B[l][m])
{
  for (int i = 0; i < n; i++)
    {
      for (int j = 0; j < m; j++)
        {
          C[i][j] = 0.0;
          for (int k = 0; k < l; k++)
            {
              C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

void
print_matrix (int n, int m, float const A[n][m])
{
  for (int i = 0; i < n; i++)
    {
      for (int j = 0; j < m; j++)
        {
          printf ("%2.2f ", A[i][j]);
        }
      printf ("\n");
    }
}

int
main ()
{
  float A[2][3] = {
    { 1, 2, 3 },
    { 4, 5, 6 },
  };

  float B[3][2] = {
    { 1, 2 },
    { 3, 4 },
    { 5, 6 },
  };

  float C[2][2];

  matrix_mult (2, 2, 3, C, A, B); // C = A * B
  printf ("== Matrix A\n");
  print_matrix (2, 3, A);
  printf ("== Matrix B\n");
  print_matrix (3, 2, B);
  printf ("== Matrix C\n");
  print_matrix (2, 2, C);

  float D[3][2] = { { 1 }, { 2 } };
  printf ("== Matrix D\n");
  print_matrix (3, 2, D);

  float E[3][2] = { 1, 2, 3, 4 };
  printf ("== Matrix E\n");
  print_matrix (3, 2, E);

  // C needs to know all except the first dimension to work out the size of the full table,
  // so it can initialize it correctly.
  float F[][2] = { { 1, 2 }, { 3, 4 }, { 5, 6 } };
  printf ("== Matrix F\n");
  print_matrix (3, 2, F);

  // Work from right to left.
  // This is innermost array.
  //          ↓
  // Then 3 of it.
  //       ↓
  // Then 2 of that.
  //    ↓
  int G[2][3][4] = {
    { { 1, 2, 3, 4 }, { 5, 6, 7, 8 }, { 9, 10, 11, 12 } },
    { { 1, 2, 3, 4 }, { 5, 6, 7, 8 }, { 9, 10, 11, 12 } },
  };
}
