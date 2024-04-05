#include <assert.h>
#include <limits.h>
#include <stdio.h>

// Output of print_buckets (three strings have b, one has f and four have q as their first character); "foo", "qux",
// "qoo", "qaz", "boo", "bar", "qar", "baz"
//                                                                    X       X                     X
//  A B C D E F G H I J K L M N O P Q S T U V W X X Y Z             a b c d e f g h i j k l m n o p q r s t u v w x y z
//  0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 3 0 0 0 1 0 0 0 0 0 0 0 0 0 0 4 0 0 0 0 0 0 0 0 0
//  0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 3 3 3 3 4 4 4 4 4 4 4 4 4 4 4 8 8 8 8
//  8 8 8 8 8 8 8 8 8 8 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 3 3 3 4 4 4 4 4 4 4 4 4 4 4
//  4 8 8 8 8 8 8 8 8 8 8 8 8 8 8 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 3 3 3 4 4 4 4 4 4
//  4 4 4 4 4 5 8 8 8 8 8 8 8 8 8 8 8 8 8 8 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 3 3 3 4
//  4 4 4 4 4 4 4 4 4 4 6 8 8 8 8 8 8 8 8 8 8 8 8 8 8 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
//  0 3 3 3 4 4 4 4 4 4 4 4 4 4 4 7 8 8 8 8 8 8 8 8 8 8 8 8 8 8 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
//  0 0 0 0 0 1 3 3 3 4 4 4 4 4 4 4 4 4 4 4 7 8 8 8 8 8 8 8 8 8 8 8 8 8 8 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
//  0 0 0 0 0 0 0 0 0 0 2 3 3 3 4 4 4 4 4 4 4 4 4 4 4 7 8 8 8 8 8 8 8 8 8 8 8 8 8 8 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
//  0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 2 3 3 3 4 4 4 4 4 4 4 4 4 4 4 8 8 8 8 8 8 8 8 8 8 8 8 8 8 8 0 0 0 0 0 0 0 0 0 0 0 0 0
//  0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 3 3 3 3 4 4 4 4 4 4 4 4 4 4 4 8 8 8 8 8 8 8 8 8 8 8 8 8 8 8

// must be large enough to index all chars; a-z = 97-122, A-Z = 65-90
// CHAR_BIT is the number of bits in char; normally 8
#define BUCKET_SIZE (1 << (CHAR_BIT - 1))

void print_buckets(int buckets[BUCKET_SIZE]) {
  printf("\nBuckets table (size: %d). Starting with 'A': ", BUCKET_SIZE);
  for (size_t i = 'A'; i < BUCKET_SIZE; i++)
    printf("%d ", buckets[i]);
}

// search for first char in string i of array
#define SEARCH_EXPR array[i][0]

void compute_buckets(int n, char *array[n], int buckets[BUCKET_SIZE]) {
  // init buckets
  for (size_t i = 0; i < BUCKET_SIZE; i++)
    buckets[i] = 0;

  // count; collect first char of each word
  for (size_t i = 0; i < n; i++) {
    char bucket = SEARCH_EXPR;
    buckets[bucket]++;
  }
  print_buckets(buckets);

  // go from right to left
  int m = n;
  for (int i = BUCKET_SIZE - 1; i >= 0; i--) {
    int count = buckets[i];
    buckets[i] = m - count;
    m -= count;
  }
  assert(m == 0);
  print_buckets(buckets);
}

// 'array' IS ONLY SORTED ON THE FIRST CHAR OF EACH STRING!!!
void sort_strings(int n, char *array[n], char *output[n]) {
  int buckets[BUCKET_SIZE];
  compute_buckets(n, array, buckets);
  for (size_t i = 0; i < n; i++) {
    char bucket = SEARCH_EXPR;
    int index = buckets[bucket]++;
    output[index] = array[i];
    print_buckets(buckets);
  }
}

void print_array(int n, char *array[n]) {
  for (size_t i = 0; i < n; i++)
    printf("%s ", array[i]);
}

int main() {
  char *array[] = {"foo", "qux", "qoo", "qaz", "boo", "bar", "qar", "baz"};
  int n = sizeof array / sizeof *array;

  printf("Original array: ");
  print_array(n, array); // foo qux qoo qaz boo bar qar baz

  char *output[n];
  sort_strings(n, array, output); // sort 'array' into 'output'
  printf("\n'Sorted' array: ");   // bucket sort is stable (important for radix sort)
  print_array(n, output);         // boo bar baz foo qux qoo qaz qar
  putchar('\n');
}
