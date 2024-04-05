#include "substr.h"
#include <stdio.h>

int main() {
  printf("aa %c a\n", substr_cmp(as_substr("aa"), as_substr("a")) == 1   ? '>'
                      : substr_cmp(as_substr("aa"), as_substr("a")) == 0 ? '='
                                                                         : '<');
  printf("a  %c aa\n", substr_cmp(as_substr("a"), as_substr("aa")) == 1   ? '>'
                       : substr_cmp(as_substr("a"), as_substr("aa")) == 0 ? '='
                                                                          : '<');
  printf("aa %c ab\n", substr_cmp(as_substr("aa"), as_substr("ab")) == 1   ? '>'
                       : substr_cmp(as_substr("aa"), as_substr("ab")) == 0 ? '='
                                                                           : '<');
  printf("aa %c aa\n", substr_cmp(as_substr("aa"), as_substr("aa")) == 1   ? '>'
                       : substr_cmp(as_substr("aa"), as_substr("aa")) == 0 ? '='
                                                                           : '<');
}
