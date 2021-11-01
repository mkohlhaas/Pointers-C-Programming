#include <stdio.h>

int
main()
{
  int array[10];
  int(*ap1)[]   = &array; // name of variable is *ap1 - NOT ap1!!!
  int(*ap2)[10] = &array;
  int(*ap3)[5]  = &array; // incompatible pointer types
  int(*ap4)[20] = &array; // incompatible pointer types
  int* ip       = array;

  // =============================================
  // ===== array =================================
  // =============================================
  printf("%p, sizeof array = %zu\n", array, sizeof array);
  
  // =============================================
  // ===== *ap1 ===================================
  // =============================================
  printf("%p, *ap1 is an incomplete type\n", *ap1);
  // We cannot get sizeof *ap1, it is an incomplete type.
  // printf("%p, sizeof *ap1  = %2zu (%2zu)\n",
  //        *ap1,
  //        sizeof *ap1,     // invalid application to an incomplete type
  //        10 * sizeof(int));

  // =============================================
  // ===== *ap2 ===================================
  // =============================================
  printf("%p, sizeof *ap2  = %2zu (%2zu)\n",
         *ap2,               // you could use *ap2 or ap2
         sizeof *ap2,        // you'll get a warning when using ap2 - using *ap2 is more consistent
         10 * sizeof(int));
  // =============================================
  // ===== ap3 ===================================
  // =============================================
  printf("%p, sizeof *ap3  = %2zu (%2zu)\n",
         *ap3,
         sizeof *ap3,
         5 * sizeof(int));
  // =============================================
  // ===== *ap4 ===================================
  // =============================================
  printf("%p, sizeof *ap4  = %2zu (%2zu)\n",
         *ap4,
         sizeof *ap4,
         20 * sizeof(int));
  // =============================================
  // ===== ip ====================================
  // =============================================
  printf("%p, sizeof *ip   = %2zu (%2zu)\n",
         ip,
         sizeof ip,
         sizeof(int*));
}
