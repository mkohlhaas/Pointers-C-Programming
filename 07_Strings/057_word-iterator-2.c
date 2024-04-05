#include <ctype.h>
#include <stdio.h>

// NULL pointer instead of pointer to zero terminal
#define NULLIFY(x) ((*x) ? x : NULL)

// ========== Macros =========================================================

// create skip and find functions
#define GEN_FIND_SKIP(name, predicate)                                                                                 \
  char *skip_##name(char *x) {                                                                                         \
    while (*x && (predicate)(*x))                                                                                      \
      x++;                                                                                                             \
    return x;                                                                                                          \
  }                                                                                                                    \
  char *find_##name(char *x) {                                                                                         \
    while (*x && !(predicate)(*x))                                                                                     \
      x++;                                                                                                             \
    return x;                                                                                                          \
  }

// create first and next iterators based on skip and find functions
#define GEN_FIND_SKIP_ITER(name, find, skip)                                                                           \
  char *first_##name(char *x) { return NULLIFY((find)(x)); }                                                           \
  char *next_##name(char *x) { return NULLIFY((find)((skip)(x))); }

// create everything: find, skip functions and first, next iterators
#define GEN_ITER(name, predicate)                                                                                      \
  GEN_FIND_SKIP(name, predicate)                                                                                       \
  GEN_FIND_SKIP_ITER(name, find_##name, skip_##name)

// ========== Macro Calls ====================================================

GEN_ITER(wrd, isalpha)
GEN_ITER(int, isdigit)

// ========== Main ===========================================================
int main() {
  char const *words = "asdlfkj 34 2345 2345 sdfk kj 230 asdo 13 aoanq 2424 eqycp";

  {
    printf("\033[38;5;206m");
    printf("=========== Original string ==================================");
    printf("\n\033[0m");
    printf("%s\n", words);
  }

  {
    printf("\033[38;5;206m");
    printf("=========== Words ============================================");
    printf("\n\033[0m");

    for (char *iter = first_wrd((char *)words); iter; iter = next_wrd(iter))
      printf("%s\n", iter);
  }

  {
    printf("\033[38;5;206m");
    printf("=========== Integers =========================================");
    printf("\n\033[0m");

    for (char *iter = first_int((char *)words); iter; iter = next_int(iter))
      printf("%s\n", iter);
  }
}
