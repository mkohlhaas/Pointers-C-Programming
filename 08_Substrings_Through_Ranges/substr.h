#pragma once

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

typedef struct range
{
  char*   begin;
  char*   end;    // points at one past last character! (Often the zero-terminal.)
} Substr;

typedef struct range SubstrIter;

static Substr const
NULL_SUBSTR = { .begin = NULL, .end = NULL };

// x is a Substr
#define SUBSTR(b, e)          (Substr){ .begin = (b), .end = (e) }
#define is_null_substr(x)     ((x).begin == NULL)
#define is_empty_substr(x)    ((x).begin == (x).end)
#define substr_len(x)         ((x).end - (x).begin)
#define MIN(a, b)             (((a) < (b)) ? (a) : (b))

char*  substr_to_buf          (char* to, Substr from);
void   print_substr           (Substr s);
char   insert_zero_term       (Substr s);
void   restore_term           (Substr s, char c);
int    substr_cmp             (Substr x, Substr y);
void   substr_reverse         (Substr s);
void   swap_substr            (Substr x, Substr y);
Substr as_substr              (char* s);
Substr slice                  (char* s, size_t begin, size_t end);
Substr copy_substr            (Substr to, Substr from);
Substr delete_substr          (Substr out, Substr x, Substr y);
Substr delete_substr_inplace  (Substr x, Substr y);
Substr replace_substr_inplace (Substr z, Substr x, Substr y);
Substr replace_substr         (Substr out, Substr z, Substr x, Substr y);
Substr insert_substr_inplace  (Substr x, size_t index, Substr y);
Substr insert_substr          (Substr out, Substr x, size_t index, Substr y);
Substr copy_words             (Substr to, Substr from);
Substr compact_words          (Substr s);
Substr next_word              (SubstrIter* iter);
Substr find_occurrence        (Substr x, Substr y);
Substr next_occurrence        (SubstrIter* iter, Substr s, int overlaps);
