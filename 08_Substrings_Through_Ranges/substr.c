#include "substr.h"
#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <sys/param.h>

Substr as_substr(char *s) {
  char *x = s;
  while (*x) {
    x++;
  }
  return SUBSTR(s, x);
}

Substr slice(char *s, size_t begin, size_t end) {
  assert(begin >= 0 && begin <= end);
  assert(end >= 0 && end <= strlen(s));
  return SUBSTR(s + begin, s + end);
}

// Creating a string from a Substr.
// The caller is responsible for ensuring that `to` is large enough (i.e. at least substr_len(from) + 1 ).
// Returns pointer to zero-terminal of the string.
char *substr_to_buf(char *to, Substr from) {
  while (from.begin < from.end) {
    *to++ = *from.begin++;
  }
  *to = '\0';
  return to;
}

void print_substr(Substr s) {
  while (s.begin < s.end) {
    putchar(*s.begin++);
  }
}

// Mutates substrings to create zero-terminated strings.
// Overwrites last character with '\0' and returns character.
char insert_zero_term(Substr s) {
  char c = *s.end;
  *s.end = '\0';
  return c;
}

// Writes char `c` at end and possibly overwrites '\0'.
// Reverse operation of `insert_zero_term()`.
void restore_term(Substr s, char c) { *s.end = c; }

// -- Basic operations -------------------------------------------------------------------------------------- //

// x == y →  0
// x >  y → +1
// x <  y → -1
int substr_cmp(Substr x, Substr y) {
  while (x.begin < x.end && y.begin < y.end) {
    if (*x.begin > *y.begin) {
      return +1;
    }
    if (*x.begin < *y.begin) {
      return -1;
    }
    x.begin++;
    y.begin++;
  }
  // We've reached the end of one of the substrings. If they had the same length, they are equal,
  // otherwise, the shorter string is the smallest.
  if (x.begin < x.end) { // x is the longer string
    return +1;
  }
  if (y.begin < y.end) { // y is the longer string
    return -1;
  }
  return 0; // same length, same chars
}

void swap(char *a, char *b) {
  char c;
  c = *a;
  *a = *b;
  *b = c;
}

void substr_reverse(Substr s) {
  if (is_empty_substr(s)) {
    return;
  }
  char *restrict x = s.begin;
  char *restrict y = s.end - 1;
  for (; x < y; x++, y--) {
    swap(x, y);
  }
}

// It is the caller's responsibility to check that x and y have the same length and do not overlap
void swap_substr(Substr x, Substr y) {
  char *restrict p = x.begin;
  char *restrict q = y.begin;
  for (; p < x.end && q < y.end; p++, q++) {
    swap(p, q);
  }
}

// We write the characters in `from` to `to`, but only part of `from` if `to` is shorter than `from`.
// Returns left-over `Subst` in `to`.
Substr copy_substr(Substr to, Substr from) {
  size_t n = MIN(substr_len(to), substr_len(from));
  // copy from right cannot handle empty strings, so bail out here
  if (n == 0) {
    return to;
  }

  // Technically, the comparison is only well defined if the pointers
  // are into the same allocated buffer, but we are always allowed to
  // test, and we do not care about the result if they are not in the
  // same buffer, so it doesn't matter.
  if (to.begin < from.begin) { // copy from left
    char *restrict to_beg = to.begin;
    char *restrict from_beg = from.begin;
    char *from_end = from.begin + n;
    while (from_beg < from_end) {
      *to_beg++ = *from_beg++;
    }
  } else { // copy from right
    char *restrict to_beg_end = to.begin + n;
    char *restrict from_beg_end = from.begin + n;
    char *from_beg = from.begin;
    do {
      *(--to_beg_end) = *(--from_beg_end);
    } while (from_beg_end > from_beg);
  }
  return SUBSTR(to.begin + n, to.end); // return left-over Substr of `to`
}

// Replaces subrange `x` of `z` with `y`. Result is in `out`.
// It is the caller's responsibility to ensure that `x` is a subrange of `z` and `y` is not contained in `out`.
// Returns `Substr` that begins at the next free character in the destination string and goes to the end of `out`.
Substr replace_substr(Substr out, Substr z, Substr x, Substr y) {
  Substr z_before = SUBSTR(z.begin, x.begin);
  Substr z_after = SUBSTR(x.end, z.end);
  size_t ylen = substr_len(y);
  size_t outlen = substr_len(out);
  size_t x_beg_idx = x.begin - z.begin;
  size_t y_end_idx = MIN(x_beg_idx + ylen, outlen);
  Substr out_before = SUBSTR(out.begin, out.begin + x_beg_idx);
  Substr out_after = SUBSTR(out.begin + y_end_idx, out.end);
  Substr out_y = SUBSTR(out.begin + x_beg_idx, out.begin + y_end_idx);

  copy_substr(out_before, z_before);
  char *ret_end = copy_substr(out_after, z_after).begin;
  copy_substr(out_y, y);

  return SUBSTR(out.begin, ret_end);
}

// It is the caller's responsibility to ensure that `x` is a subrange of `z` and that `y` is not contained in `z`.
// Returns Substr that begins at the next free character in the destination string and goes to the end of the
// destination.
Substr replace_substr_inplace(Substr z, Substr x, Substr y) {
  size_t zlen = substr_len(z);
  size_t ylen = substr_len(y);
  size_t x_beg_idx = x.begin - z.begin;
  size_t y_end_idx = MIN(x_beg_idx + ylen, zlen);
  char *y_end = z.begin + y_end_idx;
  Substr in_after = SUBSTR(x.end, z.end);
  Substr out_after = SUBSTR(y_end, z.end);
  Substr out_y = SUBSTR(x.begin, y_end);

  char *ret_end = copy_substr(out_after, in_after).begin;
  copy_substr(out_y, y);

  return SUBSTR(z.begin, ret_end);
}

// Insert Substr `y` in Substr `z` at `index`. Result in `out`.
// It is the caller's responsibility to check that `index` is contained in `z`.
Substr insert_substr(Substr out, Substr z, size_t index, Substr y) {
  assert(index <= substr_len(z));
  char *p = z.begin + index;
  return replace_substr(out, z, SUBSTR(p, p), y);
}

// Insert `Substr y` in `Substr x` at `index`.
// It is the caller's responsibility to check that `index` is contained in `x`.
Substr insert_substr_inplace(Substr x, size_t index, Substr y) {
  assert(index < substr_len(x));
  char *p = x.begin + index;
  return replace_substr_inplace(x, SUBSTR(p, p), y);
}

// Deletes subrange `y` in `x`. Result in `out`.
// It is the caller's responsibility to check that `y` is contained in `x`.
Substr delete_substr(Substr out, Substr x, Substr y) { return replace_substr(out, x, y, as_substr("")); }

// Note: You are modifying `x`, so other references to it, and substrings in it will also be affected!
Substr delete_substr_inplace(Substr x, Substr y) { return replace_substr_inplace(x, y, as_substr("")); }

// -- Word iteration ---------------------------------------------------------------------------------------- //

// Returns pointer to first non-alpha char. Could be '\0'.
char *skip_word(char *x) {
  while (*x && isalpha(*x)) {
    x++;
  }
  return x;
}

// Returns pointer to first char of a word or to end of string ('\0') if no word found.
char *find_word(char *x) {
  while (*x && !isalpha(*x)) {
    x++;
  }
  return x;
}

// Returns next word as a Substr or NULL_SUBSTR if no word found.
Substr next_word(SubstrIter *iter) {
  char *begin = find_word(iter->begin);
  if (*begin == '\0') {
    return NULL_SUBSTR; // no more words
  }

  char *end = skip_word(begin);
  // `iter` was passed by reference
  iter->begin = end; // move the beginning of the iterator!; end is not needed for iterator
  return SUBSTR(begin, end);
}

Substr first_word(SubstrIter *iter) { next_word(iter); }

Substr copy_words(Substr to, Substr from) {
  char *begin = to.begin;
  Substr sep = as_substr("");
  SubstrIter iter = from;

  // `iter` is updated on-the-fly
  for (Substr word = first_word(&iter); !is_null_substr(word); word = next_word(&iter)) {
    to = copy_substr(to, sep);
    to = copy_substr(to, word);
    sep = as_substr(" ");
  }
  return SUBSTR(begin, to.begin);
}

Substr compact_words(Substr s) { return copy_words(s, s); }

// Finds the first occurrence of the substring `y` in the substring `x`.
// Returns NULL_SUBSTR if not found.
Substr find_occurrence(Substr x, Substr y) {
  int n = substr_len(x);
  int m = substr_len(y);
  if (m > n) {
    return NULL_SUBSTR;
  }

  char *s = x.begin;
  char *end = x.end - m + 1;
  for (; s < end; s++) {
    if (!strncmp(s, y.begin, m)) {
      return SUBSTR(s, s + m);
    }
  }
  return NULL_SUBSTR;
}

// Iterator for non-overlapping occurrences of `s`.
Substr next_occurrence(SubstrIter *iter, Substr s, bool overlaps) {
  Substr occ = find_occurrence(*iter, s);
  // if there was an occurrence, we must update the iterator
  if (!is_null_substr(occ)) {
    iter->begin = overlaps ? occ.begin + 1 : occ.end;
  }
  return occ;
}

Substr first_occurrence(SubstrIter *iter, Substr s, bool overlaps) { return next_occurrence(iter, s, overlaps); }
