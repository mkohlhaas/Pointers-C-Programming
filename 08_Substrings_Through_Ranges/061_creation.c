#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "substr.h"

int
main()
{
  {
    char* string = "";
    assert(is_empty_substr(as_substr(string)));
  }

  {
    char buf[]    = "foobarbaz";
    Substr string = as_substr(buf);
    Substr bar    = slice(buf, 3, 6);

    print_substr(string); putchar('\n');
    print_substr(bar);    putchar('\n');

    printf("my favorite string is \"%s\"\n", string.begin);

    char tmp[substr_len(bar) + 1];
    substr_to_buf(tmp, bar);
    printf("my second favorite string is \"%s\"\n", tmp);

    char c = insert_zero_term(bar);
    printf("my second favorite string is \"%s\"\n", bar.begin);
    restore_term(bar, c);
    print_substr(bar);
    putchar('\n');
  }

  {
    char   s[] = "foobarbaz";
    Substr ss  = as_substr(s);
    printf("%zu %zu\n", strlen(s), substr_len(ss));

    printf("slicing \"");
    print_substr(ss);
    printf("\"");
    ss = slice(s, 3, 6);
    printf(" gives us \"");
    print_substr(ss);
    printf("\"\n");
    printf("%zu %zu\n", strlen(s), substr_len(ss));

    char buf[substr_len(ss) + 1];
    substr_to_buf(buf, ss);
    printf("%s\n", buf);

    char c = insert_zero_term(ss);
    printf("%s\n", ss.begin);
    restore_term(ss, c);
    print_substr(ss);
    putchar('\n');
  }
}
