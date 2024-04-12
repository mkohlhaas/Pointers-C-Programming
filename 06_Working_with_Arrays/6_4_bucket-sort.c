#include <assert.h>
#include <limits.h>
#include <stdio.h>

// clang-format off
//
// Array of words to be sorted (8 words): "foo", "qux", "qoo", "qaz", "boo", "bar", "qar", "baz"
// Output of print_buckets ('_' → 0)
// Buckets tables:
//     A B C D E F G H I J K L M N O P Q R S T U V W X Y Z [ \ ] ^ _ ` a b c d e f g h i j k l m n o p q r s t u v w x y z { | } ~
// 1.  _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ 3 _ _ _ 1 _ _ _ _ _ _ _ _ _ _ 4 _ _ _ _ _ _ _ _ _ _ _ _ _
// 2.  _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ 3 3 3 3 4 4 4 4 4 4 4 4 4 4 4 8 8 8 8 8 8 8 8 8 8 8 8 8
// 3.  compute_buckets is finished.
// 4.  _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ 3 3 3 4 4 4 4 4 4 4 4 4 4 4 4 8 8 8 8 8 8 8 8 8 8 8 8 8
// 5.  _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ 3 3 3 4 4 4 4 4 4 4 4 4 4 4 5 8 8 8 8 8 8 8 8 8 8 8 8 8
// 6.  _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ 3 3 3 4 4 4 4 4 4 4 4 4 4 4 6 8 8 8 8 8 8 8 8 8 8 8 8 8
// 7.  _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ 3 3 3 4 4 4 4 4 4 4 4 4 4 4 7 8 8 8 8 8 8 8 8 8 8 8 8 8
// 8.  _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ 1 3 3 3 4 4 4 4 4 4 4 4 4 4 4 7 8 8 8 8 8 8 8 8 8 8 8 8 8
// 9.  _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ 2 3 3 3 4 4 4 4 4 4 4 4 4 4 4 7 8 8 8 8 8 8 8 8 8 8 8 8 8
// 10. _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ 2 3 3 3 4 4 4 4 4 4 4 4 4 4 4 8 8 8 8 8 8 8 8 8 8 8 8 8 8
// 11. _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ 3 3 3 3 4 4 4 4 4 4 4 4 4 4 4 8 8 8 8 8 8 8 8 8 8 8 8 8 8

// Cleansed version of bucket list (only relevant information; '_' = 0):
// 3 strings have b, 1 has f and 4 have q as their first character.
// 1.  _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ 3 _ _ _ 1 _ _ _ _ _ _ _ _ _ _ 4 _ _ _ _ _ _ _ _ _ _ _ _ _
// 2.  _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ 0 _ _ _ 3 _ _ _ _ _ _ _ _ _ _ 4 _ _ _ _ _ _ _ _ _ _ _ _ _
//
// This means that all words starting with 'b' occupy 3 indexes (0, 1, 2)    in the sorted output array.
//                                         'f' occupy 1 index   (3)          in the sorted output array.
//                                         'q' occupy 4 indexes (4, 5, 6, 7) in the sorted output array.

// Cleansed version of bucket list (only relevant information):
// Array of words to be sorted (8 words): "foo", "qux", "qoo", "qaz", "boo", "bar", "qar", "baz"
// First chars in words: f, q, q, q, b, b, q, b
// Tracking the index in the sorted output array:
// 3.  _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ 0 _ _ _ 4 _ _ _ _ _ _ _ _ _ _ 4 _ _ _ _ _ _ _ _ _ _ _ _ _  (increase f)
// 4.  _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ 0 _ _ _ 4 _ _ _ _ _ _ _ _ _ _ 5 _ _ _ _ _ _ _ _ _ _ _ _ _  (increase q)
// 5.  _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ 0 _ _ _ 4 _ _ _ _ _ _ _ _ _ _ 6 _ _ _ _ _ _ _ _ _ _ _ _ _  (increase q)
// 6.  _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ 0 _ _ _ 4 _ _ _ _ _ _ _ _ _ _ 7 _ _ _ _ _ _ _ _ _ _ _ _ _  (increase q)
// 7.  _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ 1 _ _ _ 4 _ _ _ _ _ _ _ _ _ _ 7 _ _ _ _ _ _ _ _ _ _ _ _ _  (increase b)
// 8.  _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ 2 _ _ _ 4 _ _ _ _ _ _ _ _ _ _ 7 _ _ _ _ _ _ _ _ _ _ _ _ _  (increase b)
// 9.  _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ 2 _ _ _ 4 _ _ _ _ _ _ _ _ _ _ 8 _ _ _ _ _ _ _ _ _ _ _ _ _  (increase q)
// 10. _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ 3 _ _ _ 4 _ _ _ _ _ _ _ _ _ _ 8 _ _ _ _ _ _ _ _ _ _ _ _ _  (increase b)
// clang-format on

// bucket of chars
#define BUCKET_SIZE CHAR_MAX

void
print_buckets_header ()
{
  printf ("\nBuckets tables:\n");
  for (char i = 'A'; i < BUCKET_SIZE; i++)
    {
      printf ("%c ", i);
    }
}

void
print_buckets (int buckets[BUCKET_SIZE])
{
  printf ("\n");
  for (size_t i = 'A'; i < BUCKET_SIZE; i++)
    {
      if (buckets[i])
        {
          printf ("%d ", buckets[i]);
        }
      else
        {
          printf ("_ ");
        }
    }
}

// search for first char in string i of array
#define SEARCH_EXPR array[i][0]

// `n` is the number of words in `array`.
void
compute_buckets (int n, char *array[n], int buckets[BUCKET_SIZE])
{
  // Bucket table contains counts of first chars of the words in `array`.
  for (size_t i = 0; i < n; i++)
    {
      char bucket = SEARCH_EXPR;
      buckets[bucket]++;
    }
  print_buckets (buckets);

  // go from right to left
  int m = n;
  for (int i = BUCKET_SIZE - 1; i >= 0; i--)
    {
      int count = buckets[i];
      m -= count;
      buckets[i] = m;
    }
  assert (m == 0);
  print_buckets (buckets);
  printf ("\ncompute_buckets is finished.");
}

// 'array' IS ONLY SORTED ON THE FIRST CHAR OF EACH STRING!!!
void
sort_strings (int n, char *array[n], char *output[n])
{
  int buckets[BUCKET_SIZE] = {}; // all initial counts = 0
  compute_buckets (n, array, buckets);

  // loop over all words in `array`
  for (size_t i = 0; i < n; i++)
    {
      char bucket   = SEARCH_EXPR;       // bucket = first char of word at `i`
      int  index    = buckets[bucket]++; // index = current index; increment new index in buckets table
      output[index] = array[i];          // put word at index i at output[index]
      print_buckets (buckets);
    }
}

void
print_array (int n, char *array[n])
{
  for (size_t i = 0; i < n; i++)
    {
      printf ("%s ", array[i]);
    }
  printf ("\n");
}

int
main ()
{
  char *array[] = { "foo", "qux", "qoo", "qaz", "boo", "bar", "qar", "baz" };
  int   n       = sizeof array / sizeof *array;

  printf ("Original array:\n");
  print_array (n, array); // foo qux qoo qaz boo bar qar baz

  char *output[n];
  print_buckets_header ();
  sort_strings (n, array, output);                                        // sort 'array' into 'output'
  printf ("\n\n"
          "Sorted array: "
          "Array' is only sorted on the first char of each string!!!\n"); // bucket sort is stable → important for radix
                                                                          // sort
  print_array (n, output); // boo bar baz foo qux qoo qaz qar
}
