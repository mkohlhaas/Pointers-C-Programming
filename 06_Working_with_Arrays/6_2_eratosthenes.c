#include <stdio.h>

#define NOT_PRIME 0

// ============== Print Result ==================
void print_array(int *begin, int *end) {
  while (begin < end) {
    printf("%d ", *begin++);
  }
  printf("\n");
}

// ============== Version 1 =====================
int compact0(int n, int array[n]) {
  int m = 0;
  for (int i = 0; i < n; i++) {
    if (array[i] != NOT_PRIME) {
      array[m++] = array[i];
    }
  }

  // printf("Compacting: %d primes\n", m);
  // print_array(array, array + n);

  return m;
}

// Returns number of primes.
int eratosthenes1(int n, int buf[n - 2]) {
  // init buffer
  for (int i = 2; i < n; i++) {
    buf[i - 2] = i;
  }

  // print_array(buf, buf + n - 2);

  // sieve
  for (int i = 0; i * i < n - 2; i++) {
    if (buf[i] == NOT_PRIME) {
      continue;
    }

    // print_array(buf, buf + n - 2);
    int p = buf[i];

    // p * p b/c we had already sieved all the numbers < p, e.g. 2 * p, 3 * p, 5 * p if p == 7
    for (int j = p * p; j < n; j += p) {
      buf[j - 2] = NOT_PRIME;
    }
  }
  return compact0(n - 2, buf);
}

// ============== Version 2 =====================
// `from` and `to` are read-only.
int *sieve_candidates1(int *from, int *to, int prime) {
  int *output = from;
  for (int *input = from; input < to; input++) {
    if (*input % prime != 0) {
      *output++ = *input;
    }
  }
  return output;
}

// Returns number of primes.
int eratosthenes2(int n, int buf[n - 2]) {
  // init
  for (int i = 2; i < n; i++) {
    buf[i - 2] = i;
  }

  // sieve
  int *candidates = buf;
  int *end = buf + n - 2;
  while (candidates < end) {
    if ((*candidates) * (*candidates) > n) {
      break;
    }
    // print_array(buf, buf + n - 2);
    int prime = *candidates++;
    end = sieve_candidates1(candidates, end, prime);
  }
  return end - buf;
}

// ============== Version 3 =====================

// `from` and `to` are read-write (passed as references).
// Updates `to`, doesn't update `from`.
// `to` moves from right to left.
// `from` is `int**` just for consistency with `to`, could be just `int*` as before.
void sieve_candidates2(int **from, int **to, int prime) {
  // printf("from: %2d-%p, to: %2d-%p\n", **from, *from, **to, *to);
  int *output = *from;
  for (int *input = *from; input < *to; input++) {
    if (*input % prime != 0) {
      *output++ = *input;
    }
  }
  *to = output; // updates `to` to last written prime + 1
}

// Returns number of primes.
int eratosthenes3(int n, int buf[n - 2]) {
  // init
  for (int i = 2; i < n; i++) {
    buf[i - 2] = i;
  }

  // sieve
  int *candidates = buf;
  int *end = buf + n - 2;
  while (candidates < end) {
    if ((*candidates) * (*candidates) > n) {
      break;
    }
    // print_array(buf, buf + n - 2);
    int prime = *candidates++;
    sieve_candidates2(&candidates, &end, prime); // 'end' will be updated in sieve_candidates2() (!)
  }
  return end - buf;
}

// ============== Version 4 =====================

// Gets the prime from `from`; no need to pass it as a parameter.
void sieve_candidates3(int **from, int **to) {
  int prime = *(*from)++;
  int *output = *from;
  for (int *input = *from; input < *to; input++) {
    if (*input % prime != 0) {
      *output++ = *input;
    }
  }
  *to = output;
}

// Returns number of primes.
int eratosthenes4(int n, int buf[n - 2]) {
  // init
  for (int i = 2; i < n; i++) {
    buf[i - 2] = i;
  }

  // sieve
  int *candidates = buf;
  int *end = buf + n - 2;
  while (candidates < end) {
    if ((*candidates) * (*candidates) > n) {
      break;
    }
    // print_array(buf, buf + n - 2);
    sieve_candidates3(&candidates, &end);
  }
  return end - buf;
}

// ============== Main Function ==================
int main() {
  int m;
  int n = 10000;  // don't overflow stack (on Linux it's normally around 3MB)
  int buf[n - 2]; // 0, 1 are not primes

  // Version 1
  printf("Version #1:\n");
  m = eratosthenes1(n, buf);
  printf("%d primes:\n", m);
  print_array(buf, buf + m);
  printf("\n");

  // Version 2
  printf("Version #2:\n");
  m = eratosthenes2(n, buf);
  printf("%d primes:\n", m);
  print_array(buf, buf + m);
  printf("\n");

  // Version 3
  printf("Version #3:\n");
  m = eratosthenes3(n, buf);
  printf("%d primes:\n", m);
  print_array(buf, buf + m);
  printf("\n");

  // Version 4
  printf("Version #4:\n");
  m = eratosthenes4(n, buf);
  printf("%d primes:\n", m);
  print_array(buf, buf + m);
}
