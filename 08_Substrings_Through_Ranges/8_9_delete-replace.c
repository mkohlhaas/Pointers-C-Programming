#include "substr.h"
#include <assert.h>
#include <stdio.h>

int
main ()
{
  {
    printf ("------------------- Deleting -------------------\n");

    char   x_buf[] = "foobar", z_buf[] = "foobar";
    Substr x = as_substr (x_buf);
    Substr z = as_substr (z_buf);
    Substr y = slice (x_buf, 1, 3);

    printf ("Deleting oo from %s:\n", x.begin);
    print_substr (x);
    printf (" → ");
    Substr res = delete_substr (z, x, y);
    print_substr (res);
    printf (" # ");
    print_substr (z);
    putchar ('\n');

    printf ("Then inserting XX at position zero:\n");
    res = insert_substr (z, x, 0, as_substr ("XX"));
    print_substr (res);
    printf (" # ");
    print_substr (z);
    putchar ('\n');

    print_substr (x);
    printf (" → ");
    res = delete_substr_inplace (x, y);
    print_substr (res);
    printf (" # ");
    print_substr (x);
    putchar ('\n');

    res = insert_substr_inplace (x, 0, as_substr ("YY"));
    print_substr (res);
    printf (" # ");
    print_substr (x);
    printf ("\n\n");
  }

  {
    printf ("------------------- Replacing ------------------\n");

    char   out_buf[] = "foobarbazqux";
    Substr out       = as_substr (out_buf);
    char   z_buf[]   = "foobarbaz";
    Substr z         = as_substr (z_buf);
    Substr x         = find_occurrence (z, as_substr ("bar"));
    Substr y         = as_substr ("qux");

    print_substr (z);
    printf (" → ");
    Substr res = replace_substr (out, z, x, y);
    print_substr (res);
    printf (" # ");
    print_substr (out); // fooquxbaz # fooquxbazqux
    putchar ('\n');
  }

  {
    char   out_buf[] = "foobarbazqux";
    Substr out       = as_substr (out_buf);
    char   z_buf[]   = "foobarbaz";
    Substr z         = as_substr (z_buf);
    Substr x         = find_occurrence (z, as_substr ("bar"));
    Substr y         = as_substr ("X");

    print_substr (z);
    printf (" → ");
    Substr res = replace_substr (out, z, x, y);
    print_substr (res);
    printf (" # ");
    print_substr (out); // fooXbaz # fooXbazazqux
    putchar ('\n');
  }

  {
    char   out_buf[] = "foobarbazqux";
    Substr out       = as_substr (out_buf);
    char   z_buf[]   = "foobarbaz";
    Substr z         = as_substr (z_buf);
    Substr x         = find_occurrence (z, as_substr ("bar"));
    Substr y         = as_substr ("XXXX");

    print_substr (z);
    printf (" → ");
    Substr res = replace_substr (out, z, x, y);
    print_substr (res);
    printf (" # ");
    print_substr (out); // fooXXXXbaz # fooXXXXazqux
    putchar ('\n');
  }

  {
    char   z_buf[] = "foobarbazqux";
    Substr z       = as_substr (z_buf);
    Substr x       = find_occurrence (z, as_substr ("bar"));
    Substr y       = as_substr ("qax");
    Substr res;

    printf ("starting string: %s\n", z_buf);

    printf ("inplace bar/qax → ");
    res = replace_substr_inplace (z, x, y);
    print_substr (res);
    printf (" # ");
    print_substr (z); // fooqaxbazqux # fooqaxbazqux
    assert (substr_cmp (res, as_substr ("fooqaxbazqux")) == 0);
    putchar ('\n');

    printf ("inplace fo/XXXX → ");
    x   = slice (z_buf, 0, 2);
    y   = as_substr ("XXXX");
    res = replace_substr_inplace (z, x, y);

    print_substr (res);
    printf (" # ");
    print_substr (z);
    assert (substr_cmp (res, as_substr ("XXXXoqaxbazq")) == 0);
    putchar ('\n');

    printf ("inplace XXXX/YYY → ");
    x   = slice (z_buf, 0, 4);
    y   = as_substr ("YYY");
    res = replace_substr_inplace (z, x, y);

    print_substr (res);
    printf (" # ");
    print_substr (z);
    assert (substr_cmp (res, as_substr ("YYYoqaxbazq")) == 0);
    assert (substr_cmp (z, as_substr ("YYYoqaxbazqq")) == 0);
    printf ("\n\n");
  }

  {
    printf ("----------------- Appending --------------------\n");

    char   x_buf[] = "foobar", z_buf[] = "foobarxxx";
    Substr x   = as_substr (x_buf);
    Substr z   = as_substr (z_buf);
    Substr res = insert_substr (z, x, substr_len (x), as_substr ("QUX"));
    print_substr (res);
    printf (" # ");
    print_substr (z);
    putchar ('\n');
  }
}
