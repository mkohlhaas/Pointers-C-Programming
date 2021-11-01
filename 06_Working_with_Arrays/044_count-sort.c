#include <stdio.h>

// must be large enough to index all chars; a-z = 97-122, A-Z = 65-90
#define BUCKET_SIZE 128

void
print_array(int n, char array[n])
{
  for (int i = 0; i < n; i++) printf("%c", array[i]);
  putchar('\n');
}

void
print_buckets(int buckets[BUCKET_SIZE])
{
  for (int i = 0; i < BUCKET_SIZE; i++) if (buckets[i]) printf("%c-%d ", i, buckets[i]);
  putchar('\n');
}

void
sort_chars(int n, char array[n])
{
  // init bucket table
  int buckets[BUCKET_SIZE];
  for (int i = 0; i < BUCKET_SIZE; i++) buckets[i] = 0;

  // count; go through input counting occurence of each character while building buckets table
  for (int i = 0; i < n; i++) {
    char bucket = array[i];
    buckets[bucket]++;     // buckets is sorted automatically
  }
  print_buckets(buckets);  // a-1 b-1 f-1 o-2 r-1

  // sort array using buckets table
  int k = 0;
  for   (int i = 0; i < BUCKET_SIZE ; i++)  // go through all buckets in the buckets table
    for (int j = 0; j < buckets[i]  ; j++)  // move all none-zero chars to beginning of array
      array[k++] = i;                       // store each char buckets[i] times
}

int
main()
{
  char array[] = { 'f', 'o', 'o', 'b', 'a', 'r' };
  int  n       = sizeof array / sizeof *array;

  print_array(n, array);  // foobar
  sort_chars (n, array);
  print_array(n, array);  // abfoor
}
