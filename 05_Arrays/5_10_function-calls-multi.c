// Multidimensional arrays work the same way as one-dimensional arrays.
// They also decay to pointers of its immediately underlying type, i.e. pointers to arrays.

// 'A' BECOMES: int (*)[10]
void array_full_size(int A[10][10]) {
  // uncomment to see type of A
  struct S* s = A;
}

// 'A' BECOMES: int (*)[10]
void array_incomplete_size(int A[][10]) {
  struct S* s = A;
}

// 'A' IS EXPLICITLY: int (*)[10]
void pointer(int (*A)[10]) {
  struct S* s = A;
}

int main() {
  {
    int A[10][10];
    array_full_size(A);
    array_incomplete_size(A);
    pointer(A);
  }

  {
    // B's first dimension is wrong, but no warnings as second dimension is correct
    int B[5][10];
    array_full_size(B);
    array_incomplete_size(B);
    pointer(B);
  }

  {
    // You get warnings here, because the second dimension doesn't match
    int C[10][5];
    array_full_size(C);       // W
    array_incomplete_size(C); // W
    pointer(C);               // W
  }
}
