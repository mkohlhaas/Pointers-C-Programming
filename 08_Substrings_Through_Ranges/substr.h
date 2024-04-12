#pragma once

#include <stdbool.h>
#include <stdio.h>

typedef struct range
{
  char *begin;
  char *end; // points at one past last character (often `0`)
} Substr;

typedef Substr SubstrIter;

static Substr const NULL_SUBSTR = { .begin = NULL };

#define SUBSTR(b, e)                                                                                                   \
  (Substr) { .begin = (b), .end = (e) }

#define is_null_substr(x)  ((x).begin == NULL)
#define is_empty_substr(x) ((x).begin == (x).end)
#define substr_len(x)      ((x).end - (x).begin)

Substr as_substr (char *s);
Substr compact_words (Substr s);
Substr copy_substr (Substr to, Substr from);
Substr copy_words (Substr to, Substr from);
Substr delete_substr (Substr out, Substr x, Substr y);
Substr delete_substr_inplace (Substr x, Substr y);
Substr find_occurrence (Substr x, Substr y);
Substr first_occurrence (SubstrIter *iter, Substr s, bool overlaps);
Substr first_word (SubstrIter *iter);
Substr insert_substr (Substr out, Substr x, size_t index, Substr y);
Substr insert_substr_inplace (Substr x, size_t index, Substr y);
Substr next_occurrence (SubstrIter *iter, Substr s, bool overlaps);
Substr next_word (SubstrIter *iter);
Substr replace_substr (Substr out, Substr z, Substr x, Substr y);
Substr replace_substr_inplace (Substr z, Substr x, Substr y);
Substr slice (char *s, size_t begin, size_t end);
char  *substr_to_buf (char *to, Substr from);
char   insert_zero_term (Substr s);
int    substr_cmp (Substr x, Substr y);
void   print_substr (Substr s);
void   restore_term (Substr s, char c);
void   substr_reverse (Substr s);
void   swap_substr (Substr x, Substr y);
