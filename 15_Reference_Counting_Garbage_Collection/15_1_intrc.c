#include <stdio.h>
#include <stdlib.h>

typedef struct rc_int {
  int refcount;
  int value;
} RCInt;

RCInt *new_rc_int(int i) {
  RCInt *p = malloc(sizeof *p);
  if (p)
    *p = (RCInt){.refcount = 1, .value = i};
  return p;
}

void free_rc_int(RCInt *i) {
  printf("Freeing %d.\n", i->value);
  free(i);
}

RCInt *incref(RCInt *p) {
  if (p)
    p->refcount++;
  return p;
}

void decref(RCInt *p) {
  if (p && --p->refcount == 0)
    free_rc_int(p);
}

int main() {
  // Ignoring malloc errors...
  RCInt *i = new_rc_int(42);
  RCInt *j = incref(i);
  decref(i); // decrements... (though still referenced)
  printf("Decremented 'i' but nothing happened (as it should).\n");
  decref(j); // decrements and deletes...
}
