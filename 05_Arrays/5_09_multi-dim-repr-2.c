#include <stdio.h>

int
main ()
{
  int C[2][2][3] = {
    { { 1, 2, 3 }, { 4, 5, 6 } },
    { { 7, 8, 9 }, { 10, 11, 12 } },
  };
  int D[2][2][3] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };

  int dim0 = sizeof C;                           // 48           [12 (elements)]
  int dim1 = sizeof C / sizeof C[0];             // 48 / 24 = 2  [12 / 6]
  int dim2 = sizeof C[0] / sizeof C[0][0];       // 24 / 12 = 2  [6  / 3]
  int dim3 = sizeof C[0][0] / sizeof C[0][0][0]; // 12 / 4  = 3  [3  / 1]

  printf ("C dimensions (%d): %d x %d x %d\n\n", dim0, dim1, dim2, dim3);

  printf ("C and D are identical:\n");
  for (int i = 0; i < 2; i++)
    {
      for (int j = 0; j < 2; j++)
        {
          for (int k = 0; k < 3; k++)
            {
              printf ("%2d (%p) %2d (%p)\n", C[i][j][k], &C[i][j][k], D[i][j][k], &D[i][j][k]);
            }
        }
    }

  printf ("\n");

  // First Element in each row
  int (*first_dim_p)[2][3] = C;        // C decays to a pointer of its underlying type → (int(*)[2][3])
  int (*first_end)[2][3]   = C + dim1; // C jumps in sizes of 6 integers
  printf ("first_dim_p:   %p, ", first_dim_p);
  printf ("first_end:   %p, ", first_end);
  printf ("diff1: %zu, ", first_end - first_dim_p);
  printf ("diff2: %zu", (char *)first_end - (char *)first_dim_p);
  printf ("\nFirst element in each row:    ");
  for (; first_dim_p < first_end; first_dim_p++)
    {
      printf ("%d (%p) ", *(int *)first_dim_p, first_dim_p); // 1 7
    }

  // First Element in each column
  int (*second_dim_p)[3]
      = (int (*)[3])C; // here type is different from underlying type → we need an explicit cast for C
  int (*second_end)[3] = (int (*)[3])C + dim1 * dim2; // C jumps in sizes of 3 integers
  printf ("\n\nsecond_dim_p:  %p, ", second_dim_p);
  printf ("second_end:  %p, ", second_end);
  printf ("diff1: %zu, ", second_end - second_dim_p);
  printf ("diff2: %zu", (char *)second_end - (char *)second_dim_p);
  printf ("\nFirst element in each column: ");
  for (; second_dim_p < second_end; second_dim_p++)
    {
      printf ("%d (%p) ", *(int *)second_dim_p, second_dim_p); // 1 4 7 10
    }

  printf ("\n");
}
