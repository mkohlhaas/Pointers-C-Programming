#include <math.h>
#include <stdio.h>
#include <strings.h>

// -- Version 1 --------------------------------------------------------------------- //

// Returns number of digits in `n`.
int
no_digits (int n)
{
  int digits = 0;
  for (; n; n /= 10)
    {
      digits++;
    }
  return digits;
}

// Filling string `s` from right to left.
// Precondition: `n` < 0
void
neg_int_to_string1 (int n, char *s)
{
  char const *digits = "0123456789";
  for (; n; n /= 10)
    {
      *s-- = digits[-(n % 10)];
    }
}

// `n` is the number to convert to a string `s`.
// `s` is a char buffer big enough to hold the result.
// We fill the buffer from right to left.
void
int_to_string1 (int n, char *s)
{
  if (n == 0)
    {
      s[0] = '0';
      s[1] = '\0';
      return;
    }

  if (n < 0)
    {
      *s++ = '-';
    }
  if (n > 0)
    {
      n = -n;
    }
  s += no_digits (n);
  *s-- = '\0';
  neg_int_to_string1 (n, s);
}

// -- Version 2 --------------------------------------------------------------------- //

void
swap (char *left, char *right)
{
  char tmp = *left;
  *left    = *right;
  *right   = tmp;
}

// `right` points at terminating '0'.
// We fill the buffer from left to right.
void
reverse_string (char *left, char *right)
{
  if (right <= left)
    {
      return;
    }

  right--;
  for (; left < right; left++, right--)
    {
      swap (left, right);
    }
}

void
neg_int_to_string_rev (int n, char *s)
{
  char       *start  = s; // remember beginning for reversal
  char const *digits = "0123456789";
  for (; n; n /= 10)
    {
      *s++ = digits[-(n % 10)];
    }
  *s = '\0';
  reverse_string (start, s);
}

void
int_to_string_rev (int n, char *s)
{
  if (n == 0)
    {
      s[0] = '0';
      s[1] = '\0';
      return;
    }

  if (n < 0)
    {
      *s++ = '-';
    }

  if (n > 0)
    {
      n = -n;
    }
  neg_int_to_string_rev (n, s);
}

// -- Version 3 --------------------------------------------------------------------- //

// -n     | return | last digit (n % 10)
// -------+--------+---------------------
// 12345  | s + 4  |     5
//  1234  | s + 3  |     4
//   123  | s + 2  |     3
//    12  | s + 1  |     2
//     1  | s + 0  |     1

// Recursive solution.
// Returns location (`s` + 1) for next digit.
char *
neg_int_to_string3 (int n, char *s)
{
  char const *digits = "0123456789";
  if (n <= -10)
    {
      s = neg_int_to_string3 (n / 10, s);
      ;
    }
  *s = digits[-(n % 10)];
  return s + 1; // returns next position
}

void
int_to_string3 (int n, char *s)
{
  if (n == 0)
    {
      s[0] = '0';
      s[1] = '\0';
      return;
    }

  if (n < 0)
    {
      *s++ = '-';
    }
  if (n > 0)
    {
      n = -n;
    }
  s  = neg_int_to_string3 (n, s);
  *s = '\0';
}

// -- Version 4 --------------------------------------------------------------------- //

// Passing the string buffer `s` as a reference to make it read-write.
void
neg_int_to_string4 (int n, char **s)
{
  char const *digits = "0123456789";
  if (n <= -10)
    {
      neg_int_to_string4 (n / 10, s);
    }
  *(*s)++ = digits[-(n % 10)]; // updates `s`; points to `0`
}

void
int_to_string4 (int n, char *s)
{
  if (n == 0)
    {
      s[0] = '0';
      s[1] = '\0';
      return;
    }

  if (n < 0)
    {
      *s++ = '-';
    }
  if (n > 0)
    {
      n = -n;
    }
  neg_int_to_string4 (n, &s); // `s` is updated by the called function
  *s = '\0';
}

// -- Main -------------------------------------------------------------------------- //

int
main ()
{
  int n      = 20;
  int digits = log10 (n) + 2; // additional space for `-`-sign and terminating '0'
  printf ("%d %f\n", digits, log10 (n));
  char buf1[digits], buf2[digits], buf3[digits], buf4[digits];

  printf ("%p %p %p %p\n", buf1, buf2, buf3, buf4);
  // buf1 is not big enough by definition but compiler provides still enough space (by chance)
  int_to_string3 (1234567901, buf1);
  printf ("%4s\n\n", buf1);

  for (int i = -n; i < n; i++)
    {
      int_to_string1 (i, buf1);
      int_to_string_rev (i, buf2);
      int_to_string3 (i, buf3);
      int_to_string4 (i, buf4);
      printf ("%4d = %4s | %4s | %4s | %4s\n", i, buf1, buf2, buf3, buf4);
    }
}
