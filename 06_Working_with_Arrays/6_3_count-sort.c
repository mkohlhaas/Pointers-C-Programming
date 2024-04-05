#include <stdio.h>

// must be large enough to index all chars; a-z = 97-122, A-Z = 65-90 → largest index 122
#define BUCKET_SIZE 128

void print_array(int n, char array[n]) {
  for (int i = 0; i < n; i++) {
    printf("%c", array[i]);
  }
  printf("'\n");
}

void print_buckets(int buckets[BUCKET_SIZE]) {
  printf("Bucket table:\n");
  for (int i = 0; i < BUCKET_SIZE; i++) {
    printf("%3d ", i);
    if (buckets[i]) {
      printf("= '%c' → %d ", i, buckets[i]);
    }
    printf("\n");
  }
  printf("\n");
}

void sort_chars(int n, char array[n]) {
  // init bucket table
  int buckets[BUCKET_SIZE] = {};

  // count character occurences
  for (int i = 0; i < n; i++) {
    char bucket = array[i];
    buckets[bucket]++;
  }
  print_buckets(buckets);

  // sort array using buckets table
  int k = 0;
  for (int i = 0; i < BUCKET_SIZE; i++) {  // go through all buckets in the buckets table
    for (int j = 0; j < buckets[i]; j++) { // move all none-zero chars to beginning of array
      array[k++] = i;                      // store each char buckets[i] times
    }
  }
}

int main() {
  char array[] = {'O', 'f', 'O', 'b', 'A', 'r', ' ', 'z', 'o', 'F', 'o', 'B', 'a', 'R'};
  int n = sizeof array / sizeof *array;

  printf("Array to be sorted: '");
  print_array(n, array); // 'OfObAr zoFoBaR'
  sort_chars(n, array);
  printf("Sorted array: '");
  print_array(n, array); // ' ABFOORabfoorz'
}
