#include <stdio.h>
#include <string.h>

// In C, generic types mean void pointers!
void reverse(void *array, int n, int object_size) {
  if (n <= 0) {
    return;
  }

  char tmp[object_size];
  char *left = array;
  char *right = left + object_size * (n - 1);

  while (left < right) {
    memcpy(&tmp, left, object_size);
    memcpy(left, right, object_size);
    memcpy(right, &tmp, object_size);
    left += object_size;
    right -= object_size;
  }
}

void print_int_array(int n, int array[n]) {
  for (int i = 0; i < n; i++) {
    printf("%d ", array[i]);
  }
  printf("\n");
}

void print_char_array(int n, char array[n]) {
  for (int i = 0; i < n; i++) {
    printf("%c ", array[i]);
  }
  printf("\n");
}

int main() {

  // Integers
  {
    int int_array[] = {1, 2, 3, 4, 5};
    int object_size = sizeof *int_array;
    int n = sizeof int_array / object_size;
    print_int_array(n, int_array);
    reverse(int_array, n, object_size);
    print_int_array(n, int_array);
  }

  printf("\n");

  // Chars
  {
    char char_array[] = {'h', 'e', 'l', 'l', 'o'};
    int object_size = sizeof *char_array;
    int n = sizeof char_array / object_size;
    print_char_array(n, char_array);
    reverse(char_array, n, object_size);
    print_char_array(n, char_array);
  }
}
