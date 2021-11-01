// using void*
// only three changes to ../09_Dynamic_Memory_Management/075_dynarray.c; search for the word Update
// good for storing pointers, not so good for storing values
// "If your application works with heap-allocated objects, then this dynamic array implementation is ideal."

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct
{
  size_t  size;  // number of elements = number of void pointers
  size_t  used; 
  void**  data;  // Update int* => void** (but we store void* in data and cast it); array of ints -> array of void pointers; we cannot have array of voids
} dynarray;

#define da_at(da, i)                 (da)->data[(i)]
#define da_len(da)                   (da)->used
#define size_check(n, type)          ((SIZE_MAX / sizeof(type)) >= (n))
#define checked_malloc(n, type)      (size_check((n), (type)) ? malloc((n) * sizeof(type))       : NULL)
#define checked_realloc(p, n, type)  (size_check((n), (type)) ? realloc((p), (n) * sizeof(type)) : NULL)
#define max_array_len(type)          (SIZE_MAX / sizeof(type))
#define is_at_max_len(n, type)       ((n) == max_array_len(type))
#define capped_dbl(n, type)          (((n) < max_array_len(type) / 2) ? (2 * (n)) : max_array_len(type))
#define MAX(a, b)                    (((a) > (b)) ? (a) : (b))
#define MIN(a, b)                    (((a) < (b)) ? (a) : (b))
#define MIN_ARRAY_SIZE               1

void
print_da(dynarray* da) {
  printf("size: %zd\n", da->size);
  printf("used: %zd\n", da->used);
  for (int i = 0; i < da_len(da); i++) printf("%p ", da_at(da, i));
  putchar('\n');
}

bool
da_init(dynarray* da, size_t init_size, size_t init_used)
{
  assert(init_size >= init_used);
  init_size = MAX(init_size, MIN_ARRAY_SIZE);
  da->data  = checked_malloc(init_size, *da->data);
  da->size  = (da->data) ? init_size : 0;
  da->used  = (da->data) ? init_used : 0;
  return !!da->data;
}

void
da_dealloc(dynarray* da)
{
  free(da->data);
  da->data = NULL;
  da->size = 0;
  da->used = 0;
}

bool
da_resize( dynarray* da, size_t new_size)
{
  // alloc_size is the number of elements
  size_t alloc_size = MAX(new_size, MIN_ARRAY_SIZE);
  void** new_data   = checked_realloc(da->data, alloc_size, *da->data);  // Update int* => void**
  if (!new_data) return false;

  da->data = new_data;
  da->size = alloc_size;
  da->used = MIN(da->used, new_size);
  return true;
}

// Update val of type int => void*
bool
da_append(dynarray* da, void* val)
{
  if (da->used == da->size) {
    if (is_at_max_len(da->size, *da->data)) return false;
    size_t new_size       = capped_dbl(da->size, *da->data);
    int    resize_success = da_resize(da, new_size);
    if (!resize_success) return false;
  }
  da->data[da->used++] = val;
  return true;
}

int
main()
{
  dynarray da;

  int success = da_init(&da, 0, 0);
  if (!success) printf("Allocation error!\n");

#if 0
  // Can't use values as da is an array of void pointers...
  da_append(&da, 43);  // we need an address
  // Can't take addresses of values
  da_append(&da, &42); // cannot take address of an r-value
#endif

  {
    printf("\33[38;5;206m============ A ============================================================\033[0m\n\n");

    // We need a variable and take its address
    int i = 13;
    da_append(&da, &i);
    da_append(&da, &(int){ 42 }); // you can also take a compound literal, a stack variable, and take its address

    for (int j = 0; j < 5; ++j) da_append(&da, &j); // yes nice, but five times the same address which is a local loop variable! Very bad!

    // not pretty...
    for (int j = 0; j < da_len(&da); j++)
      printf("%p %d\n",
              da_at(&da, j),         // print the void pointer (no conversion needed - we are not deferencing the pointer)
              *(int*)da_at(&da, j)); // now we are dereferencing the void pointer which must first be cast to a real pointer
  }

  {
    printf("\n\33[38;5;206m============ B ============================================================\033[0m\n\n");

    for (int i = 0; i < 5; ++i) da_append(&da, &(int){ i });        // These are all the same local integer! We are filling the array with five copies of 4.
    printf("Address of loop variable: %p\n", da.data[da.used - 1]);
    for (int j = 0; j < 5; ++j) da_append(&da, &(int){ j });        // These are all the same local integer! We are filling the array with five copies of 4.
    printf("Address of loop variable: %p\n", da.data[da.used - 1]); // reuses same address for local loop variable!!!

    // not pretty...
    for (int i = 0; i < da_len(&da); i++) printf("%d ", *(int*)da_at(&da, i)); // void pointer needs a cast because of dereferencing
  }

  {
    printf("\n\n\33[38;5;206m============ C ============================================================\033[0m\n\n");

#define da_get_deref(da, i, type) *(type*)(da_at((da), (i)))

    // not much prettier
    for (int i = 0; i < da_len(&da); i++) printf("%d ", da_get_deref(&da, i, int));
  }

  {
    printf("\n\n\33[38;5;206m============ D ============================================================\033[0m\n\n");

	// print dynamic array structure
	print_da(&da);
    putchar('\n');
    // need this to avoid freeing stack objects
    printf("Resize succeeded? %s\n", (da_resize(&da, 0) ? "true" : "false"));
    // if we didn't get rid of the stack objects above, then freeing the integers below would be a problem

	print_da(&da);  // print dynamic array structure

    for (int i = 0; i < 5; ++i) {
      int* p = malloc(sizeof *p); // dynamic allocation: now we get some pointers!
      if (!p) continue;
      *p = i;
      da_append(&da, p);          // store integer pointer in void pointer
    }

	// print dynamic array
    for (int i = 0; i < da_len(&da); i++) printf("%d ", *(int*)da_at(&da, i)); // cast void pointer to integer pointer

	// print dynamic array structure
    putchar('\n');
	print_da(&da);
  }

  {
    printf("\n\n\33[38;5;206m============ E ============================================================\033[0m\n\n");
	// free malloced integers
    for (int i = 0; i < da_len(&da); i++) free(da_at(&da, i)); // freeing pointer but reference is still there

	// print dynamic array structure
	// dynamic array is still the same! Bad!
	printf("\nAfter freeing memory dynamic array structure itself did not change:\n");
	print_da(&da);
    putchar('\n');

	// print dynamic array
	printf("Will print garbage:\n");
    for (int i = 0; i < da_len(&da); i++) printf("%d ", *(int*)da_at(&da, i));

	// but now we resize and everything will be dandy
    da_resize(&da, 1); // we will overwrite first entry shortly
    printf("\n\nResizing dynamic array:\n");
	print_da(&da);
    putchar('\n');
	int* p;
    da_at(&da, 0) = &(double){ 3.14 }; // first entry is overwritten
    p             = da_at(&da, 0);
    printf("Casting a double to an integer pointer is not a good idea! %d\n\n", *p);
  }

  print_da(&da);
  putchar('\n');
  printf("%p\n", da.data); // this has been first allocated in da_init, but could have been changed later when resizing the dynamic array
  da_dealloc(&da);
}
