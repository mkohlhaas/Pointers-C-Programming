#include <stdio.h>
#include <string.h>

int
strlen_index(char const* s)
{
  int i = 0;
  while (s[i]) i++;
  return i;
}

int
strlen_pointer(char const* s)
{
  char const* x = s;
  while (*x) x++;
  return x - s;
}

//------------------- Version 1 --------------------------------------------------------------------- 
// return end of string in 'output' buffer (points at terminating '0')
// standard strcpy() returns output (beginning of string)
char*
string_copy(char* output, char const* input)
{
  // no point defining 'x', we could use 'input' directly
  char const* x = input;
  char*       y = output;
  for (; *x; x++, y++) *y = *x;
  *y = '\0';
  return y;
}

//------------------- Version 2 --------------------------------------------------------------------- 
// return end of string in 'output' buffer (points at terminating '0')
char*
string_copy_(char* output, char const* input)
{
  while (*input) *output++ = *input++;
  *output = '\0';
  return output;
}

//------------------- Version 3 --------------------------------------------------------------------- 
// return end of string in 'output' buffer (points at terminating '0')
char*
string_copy__(char* output, char const* input)
{
  while ((*output = *input)) {
    output++;
    input++;
  }
  return output;
}

//------------------- Version 4 (favorite) ----------------------------------------------------------- 
// return end of string in 'output' buffer (points at terminating '0')
char*
string_copy___(char* output, char const* input)
{
   // If you take an assignment and look at its value, it is the object that you assigned, in
   // this case, the byte we copied.  You have to use parens to do so.
  while ((*output++ = *input++)) ;
  return output;
}

//------------------- Version 5 --------------------------------------------------------------------- 
// return end of string in 'output' buffer (points at terminating '0')
char*
string_copy____(char* output, const char* input)
{
  do {
    *output++ = *input;
  } while (*input++);
  return output;
}

//------------------- Reverse String ---------------------------------------------------------------- 
void
reverse_string(char* s)
{
  char* left  = s;
  char* right = s + strlen(s) - 1;
  for (; left < right; left++, right--) {
    char c;
     c     = *left;   // swap values
    *left  = *right;
    *right =  c;
  }
}

//------------------- Main -------------------------------------------------------------------------- 
int
main()
{
  char const* string = "Hello, world!";
  int         n      = strlen(string);
  char        buffer[n + 1];              // VLA (variable length array)
  string_copy    (buffer, string); printf("String copy: %s\n",    buffer);
  string_copy_   (buffer, string); printf("String copy: %s\n",    buffer);
  string_copy__  (buffer, string); printf("String copy: %s\n",    buffer);
  string_copy___ (buffer, string); printf("String copy: %s\n",    buffer);
  string_copy____(buffer, string); printf("String copy: %s\n",    buffer);
  reverse_string (buffer);         printf("Reverse String: %s\n", buffer);
}
