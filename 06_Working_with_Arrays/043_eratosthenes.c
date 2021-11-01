#include <stdio.h>

#define NOT_PRIME 0

// ============== Print Result ================== 
void
print_array(int* begin, int* end)
{
  while (begin < end) printf("%2d ", *begin++);  // begin will be incremented after calling printf
  putchar('\n');
}

// ============== Version 1 ===================== 
int
compact0(int n, int array[n])
{
  int m = 0;
  for (int i = 0; i < n; i++) if (array[i] != NOT_PRIME) array[m++] = array[i];
  return m;
}

int
eratosthenes(int n, int buf[n - 2])
{
  // Init
  for (int i = 2; i < n; i++) buf[i - 2] = i;

  // Sieve
  for (int i = 0; i * i < n - 2; i++) {
    // printf("i: %d\n", i);
    if (buf[i] == NOT_PRIME) continue;
    print_array(buf, buf + n - 2);
    int p = buf[i];
    for (int j = p * p; j < n; j += p)  // p * p b/c we had already sieved all the numbers < p, e.g. 2 * p, 3 * p, 5 * p if p == 7
      buf[j - 2] = NOT_PRIME;
  }
  return compact0(n - 2, buf);
}

// ============== Version 2 ===================== 
int*
sieve_candidates_(int* from, int* to, int p)
{
  int* output = from;
  for (int* input = from; input < to; input++) if (*input % p != 0) *output++ = *input;
  return output;
}

int
eratosthenes_(int n, int buf[n - 2])
{
  // Init
  for (int i = 2; i < n; i++) buf[i - 2] = i;

  // Sieve
  int* candidates = buf;
  int* end        = buf + n - 2;
  while (candidates < end) {
    if ((*candidates) * (*candidates) > n) break;
    print_array(buf, buf + n - 2);
    int p     = *candidates++;
        end   =  sieve_candidates_(candidates, end, p);
  }
  return end - buf;
}

// ============== Version 3 ===================== 

// 'to' moves to the left, 'from' is not changed (it's int** just for consistency with 'to')
void
sieve_candidates__(int** from, int** to, int p)
{
  // printf("from: %2d-%p, to: %2d-%p\n", **from, *from, **to, *to);
  int* output = *from;
  for (int* input = *from; input < *to; input++) if (*input % p != 0) *output++ = *input;
  *to = output;  // update 'to' to past the last written prime
}

int
eratosthenes__(int n, int buf[n - 2])
{
  // Init
  for (int i = 2; i < n; i++) buf[i - 2] = i;

  // Sieve
  int* candidates = buf;
  int* end        = buf + n - 2;
  while (candidates < end) {
    if ((*candidates) * (*candidates) > n) break;  // parens not needed but easier to read
    print_array(buf, buf + n - 2);
    int p = *candidates++;
    sieve_candidates__(&candidates, &end, p);      // 'end' will be updated in sieve_candidates__()
  }
  return end - buf;
}

// ============== Version 4 ===================== 

// get the prime from 'from'; no need to pass it as a parameter
void
sieve_candidates___(int** from, int** to)
{
  int  p      = *(*from)++;
  int* output = *from;
  for (int* input = *from; input < *to; input++) if (*input % p != 0) *output++ = *input;
  *to = output;
}

int
eratosthenes___(int n, int buf[n - 2])
{
  // Init
  for (int i = 2; i < n; i++) buf[i - 2] = i;

  // Sieve
  int* candidates = buf;
  int* end        = buf + n - 2;
  while (candidates < end) {
    if ((*candidates) * (*candidates) > n) break;
    print_array(buf, buf + n - 2);
    sieve_candidates___(&candidates, &end);
  }
  return end - buf;
}

// ============== Main Function ==================
int
main()
{
  int m,  n = 50;
  int buf[n - 2];

  m = eratosthenes   (n, buf); print_array(buf, buf + m); putchar('\n');
  m = eratosthenes_  (n, buf); print_array(buf, buf + m); putchar('\n');
  m = eratosthenes__ (n, buf); print_array(buf, buf + m); putchar('\n');
  m = eratosthenes___(n, buf); print_array(buf, buf + m);
}
