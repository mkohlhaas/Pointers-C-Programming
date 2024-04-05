#include <stdio.h>

void doesnt_mutate(int i) { i += 42; }

void mutates(int *i) { *i += 42; }

void foo(int *ip) { ip++; }

void bar(int **ip) { (*ip)++; }

int main() {
  int i = 0;
  printf("i = %d\n", i); // 0
  doesnt_mutate(i);
  printf("i = %d\n", i); // 0
  mutates(&i);
  printf("i = %d\n", i); // 42
  int *ip = NULL;
  printf("%p\n", ip); // (nil)
  foo(ip);
  printf("%p\n", ip); // (nil)
  bar(&ip);
  printf("%p\n", ip); // 0x4
}
