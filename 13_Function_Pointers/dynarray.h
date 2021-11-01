#pragma once

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct dynarray
{
  size_t size;
  size_t used;
  int*   data;
} Dynarray;

#define da_at(da, i) (da)->data[(i)]
#define da_len(da)   (da)->used

bool da_init(Dynarray* da, size_t init_size, size_t init_used);
void da_dealloc(Dynarray* da);
bool da_append(Dynarray* da, int val);
