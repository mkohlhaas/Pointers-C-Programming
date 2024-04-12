#include <stdio.h>
#include <string.h>

int
strlen_index (char const *s)
{
  int i = 0;
  while (s[i])
    {
      i++;
    }
  return i;
}

int
strlen_pointer (char const *s)
{
  char const *x = s;
  while (*x)
    {
      x++;
    }
  return x - s;
}

// -- Version 1 ------------------------------------------------------------------------------- //

// Returns end of string in `output` buffer (points at terminating `0`).
// Standard `strcpy()` returns beginning of string.
char *
string_copy1 (char *output, char const *input)
{
  char const *x = input;
  char       *y = output;
  for (; *x; x++, y++)
    {
      *y = *x;
    }
  *y = '\0';
  return y;
}

// -- Version 2 ------------------------------------------------------------------------------- //

char *
string_copy2 (char *output, char const *input)
{
  while (*input)
    {
      *output++ = *input++;
    }
  *output = '\0';
  return output;
}

// -- Version 3 ------------------------------------------------------------------------------- //

char *
string_copy3 (char *output, char const *input)
{
  // If you take an assignment and look at its value, it is the object that you assigned, in
  // this case, the byte we copied.  You have to use parens to do so.
  while ((*output = *input))
    { // copies also `0`
      output++;
      input++;
    }
  return output;
}

// -- Version 4 (favorite) -------------------------------------------------------------------- //

char *
string_copy4 (char *output, char const *input)
{
  while ((*output++ = *input++)) // copies also `0`
    ;
  return output;
}

// -- Version 5 ------------------------------------------------------------------------------- //

char *
string_copy5 (char *output, const char *input)
{
  do
    {
      *output++ = *input; // `input` string contains at least `0` (which is copied)
    }
  while (*input++);
  return output;
}

// -- Reverse String -------------------------------------------------------------------------- //

void
swap (char *left, char *right)
{
  char c = *left;
  *left  = *right;
  *right = c;
}

void
reverse_string (char *s)
{
  char *left  = s;
  char *right = s + strlen_pointer (s) - 1;
  for (; left < right; left++, right--)
    {
      swap (left, right);
    }
}

// -- Main ------------------------------------------------------------------------------------ //

int
main ()
{
  char const *string = "Hello, world!";
  int         n      = strlen (string);
  char        buffer[n + 1];
  char       *end_of_string;

  {
    end_of_string = string_copy1 (buffer, string);
    printf ("String copy #1: %s\n", buffer);
    printf ("End of string: %d\n", *end_of_string);
    printf ("Length (array): %d\n", strlen_index (buffer));
    printf ("Length (pointer): %d\n", strlen_pointer (buffer));
    printf ("\n");
  }

  {
    end_of_string = string_copy2 (buffer, string);
    printf ("String copy #2: %s\n", buffer);
    printf ("End of string: %d\n", *end_of_string);
    printf ("Length (array): %d\n", strlen_index (buffer));
    printf ("Length (pointer): %d\n", strlen_pointer (buffer));
    printf ("\n");
  }

  {
    end_of_string = string_copy3 (buffer, string);
    printf ("String copy #3: %s\n", buffer);
    printf ("End of string: %d\n", *end_of_string);
    printf ("Length (array): %d\n", strlen_index (buffer));
    printf ("Length (pointer): %d\n", strlen_pointer (buffer));
    printf ("\n");
  }

  {
    end_of_string = string_copy4 (buffer, string);
    printf ("String copy #4: %s\n", buffer);
    printf ("End of string: %d\n", *end_of_string);
    printf ("Length (array): %d\n", strlen_index (buffer));
    printf ("Length (pointer): %d\n", strlen_pointer (buffer));
    printf ("\n");
  }

  {
    end_of_string = string_copy5 (buffer, string);
    printf ("String copy #5: %s\n", buffer);
    printf ("End of string: %d\n", *end_of_string);
    printf ("Length (array): %d\n", strlen_index (buffer));
    printf ("Length (pointer): %d\n", strlen_pointer (buffer));
    printf ("\n");
  }

  {
    reverse_string (buffer);
    printf ("Reverse String: %s\n", buffer);
    printf ("Length (array): %d\n", strlen_index (buffer));
    printf ("Length (pointer): %d\n", strlen_pointer (buffer));
    printf ("\n");
  }

  {
    char const *string1 = "lagerregal";
    n                   = strlen (string1);
    char buffer1[n + 1];

    end_of_string = string_copy4 (buffer, string1);
    printf ("String:  %s\n", buffer);
    reverse_string (buffer);
    printf ("Reverse: %s\n", buffer);
  }
}
