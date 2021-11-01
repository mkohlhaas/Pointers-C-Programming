#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "substr.h"

int
main()
{
  {
    printf("----- 1 -----\n");
    char w[] = "foobar";
    Substr x = slice(w, 0, 3);
    Substr y = slice(w, 0, 3);
    Substr z = copy_substr(x, y);
    assert(substr_len(z) == 0); // we copied all, so it should be empty
    printf("%s\n", z.end);      // the rest of w, so bar
    assert(strcmp(z.end, "bar") == 0);
  }

  {
    printf("----- 2 -----\n");
    char w[] = "foobar";
    Substr x = slice(w, 0, 6);
    Substr y = slice(w, 3, 6);
    Substr z = copy_substr(x, y);
    assert(substr_len(z) == 3); // bar is still there
    assert(substr_cmp(z, as_substr("bar")) == 0);
    assert(strcmp(w, "barbar") == 0);
    printf("%s\n", w);          // barbar
    print_substr(z);            // bar
    printf("\n");
  }

  {
    printf("----- 3 -----\n");
    char w[] = "foobar";
    Substr x = slice(w, 0, 2);
    Substr y = slice(w, 3, 6);
    Substr z = copy_substr(x, y);
    assert(substr_len(z) == 0); // we filled the input
    assert(strcmp(w, "baobar") == 0);
    printf("%s\n", w);          // baobar
  }

  {
    printf("----- 4 -----\n");
    char w[] = "foobar";
    Substr x = slice(w, 0, 3);
    Substr y = slice(w, 1, 4);
    Substr z = copy_substr(x, y);
    assert(substr_len(z) == 0); // we filled the input
    assert(strcmp(w, "oobbar") == 0);
    printf("%s\n", w);          // oobbar
  }

  {
    printf("----- 5 -----\n");
    char w[] = "foobar";
    Substr x = slice(w, 1, 4);
    Substr y = slice(w, 0, 3);
    Substr z = copy_substr(x, y);
    assert(substr_len(z) == 0); // we filled the input
    assert(strcmp(w, "ffooar") == 0);
    printf("%s\n", w);          // ffooar
  }

  {
    printf("----- 6 -----\n");
    char w[] = "foobar";
    Substr x = slice(w, 0, 3);
    Substr y = slice(w, 3, 6);
    swap_substr(x, y);
    assert(strcmp(w, "barfoo") == 0);
    printf("%s\n", w);
  }
}
