#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --------------- Using Strings ----------------------------------------------------------------------------------- //

char const *slice(char const *x, int begin, int end) {
  assert(begin <= end);
  size_t len = end - begin;

  char *new_buf = malloc(len + 1);
  if (!new_buf) {
    return NULL;
  }

  strncpy(new_buf, x + begin, len);
  new_buf[len] = '\0';
  return new_buf;
}

// in both macros we have to make the term with SIZE_MAX
// smaller so we don't overflow the result
// always check size when you want to add to var, e.g. len as seen later in the code

// increment 'var' by 'x' and check size
#define size_check_inc(var, x)                                                                                         \
  {                                                                                                                    \
    if (SIZE_MAX - (var) < (x)) {                                                                                      \
      goto error;                                                                                                      \
    }                                                                                                                  \
    (var) += (x);                                                                                                      \
  }

// increment 'var' by 'x' * 'y' and check size
#define size_check_inc_product(var, x, y)                                                                              \
  {                                                                                                                    \
    if ((x) > 0 && (SIZE_MAX - (var)) / (x) < (y)) {                                                                   \
      goto error;                                                                                                      \
    }                                                                                                                  \
    (var) += (x) * (y);                                                                                                \
  }

// replace substring from 'begin' to 'end' in 'x' with 'y'
char *replace_string(char const *x, int begin, int end, char const *y) {
  size_t xlen = strlen(x);
  size_t ylen = strlen(y);
  size_t len = xlen - (end - begin);
  size_check_inc(len, ylen + 1); // add ylen + 1 - 1 for zero terminal - to len and check size

  // allocate result buffer
  char *new_buf = malloc(len);
  if (!new_buf)
    goto error;

  // copy strings into result buffer in the heap
  strncpy(new_buf, x, begin); // copy substr, not zero-terminated
  strcpy(new_buf + begin, y); // copy zero-terminated string
  strcpy(new_buf + begin + ylen, x + end);

  // return pointer to result buffer
  return new_buf;

error:
  return NULL;
}

char *insert_string(char const *x, int begin, char const *y) { return replace_string(x, begin, begin, y); }

char *delete_string(char const *x, int begin, int end) { return replace_string(x, begin, end, ""); }

char *join_strings(char const *sep, int n, char const *strings[n]) {
  size_t len = 1; // for zero terminal
  size_t sep_len = strlen(sep);
  size_t reps = (n > 1) ? (n - 1) : 0;
  size_check_inc_product(len, sep_len, reps); // increment len by sep_len * reps and check size

  // increment len by the individual string lengths in the strings array
  for (int i = 0; i < n; i++) {
    size_t string_len = strlen(strings[i]);
    size_check_inc(len, string_len);
  }

  // allocate result buffer
  char *new_buf = malloc(len);
  if (!new_buf) {
    goto error;
  }

#define append_string(src)                                                                                             \
  {                                                                                                                    \
    for (char const *p = src; *p; p++) {                                                                               \
      *dst++ = *p;                                                                                                     \
    }                                                                                                                  \
  }

  // copy strings into result buffer
  char *dst = new_buf;
  char const *xsep = "";
  for (int i = 0; i < n; i++) {
    append_string(xsep);
    append_string(strings[i]);
    xsep = sep;
  }
  *dst = '\0';
#undef append_string

  // return pointer to result buffer
  return new_buf;

error:
  return NULL;
}

// --------------- Using Substrings -------------------------------------------------------------------------------- //

typedef struct range {
  char *begin;
  char *end;
} Substr;

typedef Substr SubstrIter;

// Creating substr (these are the same functions and macros from chapter 7)
#define SUBSTR(b, e)                                                                                                   \
  (Substr) { .begin = (b), .end = (e) }
#define is_null_substr(x) ((x).begin == NULL)
#define substr_len(x) ((x).end - (x).begin)

static Substr const NULL_SUBSTR = {.begin = NULL, .end = NULL};

Substr as_substr(char *s) {
  char *x = (char *)s;
  while (*x) {
    x++;
  }
  return SUBSTR((char *)s, x);
}

Substr find_occurrence(Substr x, Substr y) {
  int n = substr_len(x);
  int m = substr_len(y);
  if (m > n) {
    return NULL_SUBSTR;
  }

  char *s = x.begin;
  char *end = x.end - m;
  for (; s < end; s++) {
    if (!strncmp(s, y.begin, m)) {
      return SUBSTR(s, s + m);
    }
  }
  return NULL_SUBSTR;
}

// Iterator for non-overlapping occurrences of s
Substr next_occurrence(SubstrIter *iter, Substr s, bool overlaps) {
  Substr occ = find_occurrence(*iter, s);
  // if there was an occurrence, we must update the iterator
  if (!is_null_substr(occ)) {
    iter->begin = overlaps ? occ.begin + 1 : occ.end;
  }
  return occ;
}

// this function is different from chapter 7 - it uses malloc!
// replaces all occurrences of x in z with y
// return a standard C string
char *replace_all_occurrences(char const *z, char const *x, char const *y) {
  Substr ssz = as_substr((char *)z);
  Substr ssx = as_substr((char *)x);
  Substr ssy = as_substr((char *)y);
  size_t zlen = substr_len(ssz);
  size_t xlen = substr_len(ssx);
  size_t ylen = substr_len(ssy);

  // Compute the new string's length
  size_t len = zlen + 1; // + 1 for zero-terminal
  SubstrIter iter = ssz;
  for (Substr occ = next_occurrence(&iter, ssx, false); !is_null_substr(occ);
       occ = next_occurrence(&iter, ssx, false)) {
    len -= xlen;
    size_check_inc(len, ylen);
  }

  char *new_buf = malloc(len);
  if (!new_buf)
    goto error;

#define copy_range(b, e)                                                                                               \
  {                                                                                                                    \
    for (char const *p = (b); p != (e); p++) {                                                                         \
      *dst++ = *p;                                                                                                     \
    }                                                                                                                  \
  }

  // copy strings from 'x', 'y' and 'z' into result buffer 'new_buf'
  char const *src = z;
  char *dst = new_buf;
  iter = ssz;
  for (Substr occ = next_occurrence(&iter, ssx, false); !is_null_substr(occ);
       occ = next_occurrence(&iter, ssx, false)) {
    copy_range(src, occ.begin);     // copy from src to occ.begin into dst(=new_buf)
    copy_range(ssy.begin, ssy.end); // copy y into dst
    src = occ.end;                  // adjust src - goto end of x in z
  }
  copy_range(src, z + zlen); // copy leftover of z
  *dst = '\0';
#undef copy_range

  return new_buf;

error:
  return NULL;
}

// --------------- main -------------------------------------------------------------------------------------------- //

int main() {

  char *y;
  char const *x;
  char *z;

  {
    x = "foobarbaz";
    y = (char *)slice(x, 3, 6);
    printf("y is %s\n", y);
    assert(!strcmp(y, "bar"));
    free(y);
  }

  {
    y = replace_string(x, 3, 6, "");
    printf("y is %s\n", y);
    assert(!strcmp(y, "foobaz"));
    free(y);
  }

  {
    y = replace_string(x, 3, 6, "quxqax");
    printf("y is %s\n", y);
    assert(!strcmp(y, "fooquxqaxbaz"));
    free(y);
  }

  {
    y = insert_string(x, 0, "XX");
    printf("y is %s\n", y);
    assert(!strcmp(y, "XXfoobarbaz"));
    free(y);
  }

  {
    y = delete_string(x, 0, 3);
    printf("y is %s\n", y);
    assert(!strcmp(y, "barbaz"));
    free(y);
  }

  {
    char const *strings[] = {"foo", "bar", "baz"};
    z = join_strings(":", sizeof strings / sizeof *strings, strings);
    printf("z = %s\n", z);
    assert(!strcmp(z, "foo:bar:baz"));
    free(z);
  }

  {
    char const *strings2[] = {""};
    z = join_strings(":", sizeof strings2 / sizeof *strings2, strings2);
    printf("z = %s\n", z);
    assert(!strcmp(z, ""));
    free(z);
  }

  {
    x = "foobarbazbax";
    z = replace_all_occurrences(x, "ba", "");
    printf("z = %s\n", z);
    assert(!strcmp(z, "foorzx"));
    free(z);
  }

  {
    z = replace_all_occurrences(x, "a", "OOOO");
    printf("z = %s\n", z);
    assert(!strcmp(z, "foobOOOOrbOOOOzbOOOOx"));
    free(z);
  }

  {
    z = replace_all_occurrences(x, "123", "OOOO");
    printf("z = %s\n", z);
    assert(!strcmp(z, "foobarbazbax"));
    free(z);
  }
}
