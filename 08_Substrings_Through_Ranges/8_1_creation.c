#include "substr.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

int main() {

  {
    char *string = "";
    assert(is_empty_substr(as_substr(string)));
  }

  {
    char buf[] = "foobarbaz"; // string on the stack (not literal in read-only memory)
    Substr string = as_substr(buf);
    Substr bar = slice(buf, 3, 6);

    print_substr(string);
    printf("\n");

    print_substr(bar);
    printf("\n");

    printf("My favorite string is: \"%s\"\n", string.begin);

    char tmp[substr_len(bar) + 1];
    substr_to_buf(tmp, bar);
    printf("My second favorite string is: \"%s\"\n", tmp);

    char c = insert_zero_term(bar);
    printf("My second favorite string is: \"%s\"\n", bar.begin);
    restore_term(bar, c);
    print_substr(bar);
  }

  printf("\n\n");

  {
    char s[] = "foobarbaz";
    Substr ss = as_substr(s);
    printf("length of %s is %zu (as string), %zu (as substring)\n", s, strlen(s), substr_len(ss));

    printf("Slicing ");
    print_substr(ss);
    ss = slice(s, 3, 6);
    printf(" gives us ");
    print_substr(ss);
    printf(" with lengths %zu and %zu.\n\n", strlen(s), substr_len(ss));

    char buf[substr_len(ss) + 1];
    substr_to_buf(buf, ss);
    printf("%s\n", buf);

    char c = insert_zero_term(ss);
    printf("%s\n", ss.begin);
    restore_term(ss, c);
    print_substr(ss);
  }

  printf("\n");
}
