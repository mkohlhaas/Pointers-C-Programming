// "C takes a simple approach and allows you to add const to the object you point to,
// but otherwise the type must be the same!!!"

#include <stdio.h>

int
main ()
{
  int             *i_p   = NULL;
  int const       *ic_p  = NULL;
  int *const       i_pc  = NULL;
  int const *const ic_pc = NULL;
  // the last two one's are pretty uninteresting:
  // we can never  assign to   them (left-hand  side)
  // we can always assign from them (right-hand side)

  //  All permutations (2^4 = 16):
  i_p  = i_p;   // trivial (same types)
  ic_p = ic_p;  // trivial (same types)
  i_p  = i_pc;  // you can remove const from pointer
  ic_p = ic_pc; // you can remove const from pointer
  ic_p = i_p;   // you can add const to target
  ic_p = i_pc;  // you can remove const from pointer; you can add const to target
                //   i_p  = ic_p;          // you can't remove const from target
                //   i_p  = ic_pc;         // you can't remove const from target
                //   i_pc =  i_p;          // cannot assign to constant pointer
                //   i_pc = ic_p;          // cannot assign to constant pointer
                //   i_pc =  i_pc;         // cannot assign to constant pointer
                //   i_pc = ic_pc;         // cannot assign to constant pointer
                //  ic_pc =  i_p;          // cannot assign to constant pointer
                //  ic_pc = ic_p;          // cannot assign to constant pointer
                //  ic_pc =  i_pc;         // cannot assign to constant pointer
                //  ic_pc = ic_pc;         // cannot assign to constant pointer

  i_p  = i_p;   // Ok, T * => T *; trivial
  ic_p = ic_p;  // Ok, T * => T * (T = U const); trivial
                //   i_pc =  i_pc;         // No! You cannot assign to T const; you can't assign to const pointer
                //  ic_pc = ic_pc;         // No! You cannot assign to T const; you can't assign to const pointer

  //   i_p  = ic_p;          // No! T const * => T *; you can't remove const from target
  i_p = i_pc; // Ok, T * const => T *; you can remove const from pointer
              //   i_p  = ic_pc;         // No, there is a T const * => T *; you can't remove const from target

  //  These are the 3 rules to remember!!!
  ic_p = i_p; // Ok, T * => T const *; you can add const to target
              //  New rule for pointers:
              //  You can remove qualifiers from the pointer and add them to the type you point at, but not the reverse.
              //  T * const => T const *
  ic_p = i_pc;  // Ok, T const => T then U * => U const *; you can add const to target
  ic_p = ic_pc; // Ok, T * const => T * (T = U const); you can remove const from pointer

// Another layer of indirection
#if 0
    int **               i_p_p   = NULL;
    int const **         ic_p_p  = NULL;
    int * const *        i_pc_p  = NULL;
    int const * const *  ic_pc_p = NULL;
#endif

  typedef int       *T;
  typedef int const *U;
  T                 *i_p_p   = NULL;
  U                 *ic_p_p  = NULL;
  T const           *i_pc_p  = NULL;
  U const           *ic_pc_p = NULL;
  // plus the ones with const last, but they are simply const versions and we learn nothing new from them.
  // we can never  assign to   them (left-hand  side)
  // we can always assign from them (right-hand side)

  // All permutations (4^2 = 16):
  i_p_p   = i_p_p;   // trivial (same types)
  ic_p_p  = ic_p_p;  // trivial (same types)
  i_pc_p  = i_pc_p;  // trivial (same types)
  ic_pc_p = ic_pc_p; // trivial (same types)
  i_pc_p  = i_p_p;   // you can add const to target
  ic_pc_p = ic_p_p;  // you can add const to target
                     //   i_p_p =  ic_p_p;      // all others are not allowed
                     //   i_p_p =  i_pc_p;      // ...
                     //   i_p_p = ic_pc_p;      // ...
                     //  ic_p_p =   i_p_p;      // ... (see 021_const.c why this is not allowed)
                     //  ic_p_p =  i_pc_p;      // ...
                     //  ic_p_p = ic_pc_p;      // ...
                     //  i_pc_p =  ic_p_p;      // ...
                     //  i_pc_p = ic_pc_p;      // ...
                     // ic_pc_p =   i_p_p;      // ...
                     // ic_pc_p =  i_pc_p;      // ... (see 021_const.c why this is not allowed)

  // We do not have const pointers here, so we can only apply
  // T * => T * and T * => T const *, i.e. add const to the
  // pointed at object. If the pointed at objects have different
  // types then we cannot assign. Call the pointed at objects T and
  // U for the right-hand side and left-hand side, respectively

  // No, T = int *, U = int const *, T != U
  // i_p_p = ic_p_p;
  // No, T = int *, U = int * const, removing const not allowed
  // i_p_p = i_pc_p;
  // No, T = int *, U = int const * const, T != U
  // i_p_p = ic_pc_p;

  // No, T = int const *, U = int *, T != U
  // ic_p_p = i_p_p;
  // No, T = int const *, U = int * const, T != U
  // ic_p_p = i_pc_p;
  // No, removing const
  // ic_p_p = ic_pc_p;

  // Ok, T const * => T *, T = int *, U = int *
  i_pc_p = i_p_p;
  // No, T = int * const, U = int const *, T != U
  // i_pc_p = ic_p_p;
  // No, T = int const *, U = int * const, T != U
  // i_pc_p = ic_pc_p;

  // No, int const * const != int *
  // ic_pc_p = i_p_p;
  // Ok, Adding a const to int const *
  ic_pc_p = ic_p_p;
  // No, int const * const != int * const
  // ic_pc_p = i_pc_p;

  // Suppressing compiler warnings
  (void)i_p;
  (void)ic_p;
  (void)i_pc;
  (void)ic_pc;
  (void)i_p_p;
  (void)ic_p_p;
  (void)i_pc_p;
  (void)ic_pc_p;
}
