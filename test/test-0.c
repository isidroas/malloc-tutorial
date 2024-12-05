#include <stdio.h>
#include <stdlib.h>

int main() {
  long *ptr = malloc(29);
  if (ptr == NULL) { 
    printf("Failed to malloc a single int\n"); // this will call malloc(1024) and won't be freed until the end of the program. Curioso que el contenido es el mismo string fmt que le paso. TODO: check through ltrace
    return 1;
  }

  *ptr = 1;
  *ptr = 100;

  free(ptr);

  /* printf("malloc'd an int, assigned to it, and free'd it\n"); */

  /* int *ptr2 = malloc(sizeof(int)); */
  /* if (ptr2 == NULL) {  */
  /*   printf("Failed to malloc a single int\n"); */
  /*   return 1; */
  /* } */

  /* *ptr2 = 2; */
  /* *ptr2 = 200; */

  /* free(ptr2); */
  /* printf("malloc'd an int, assigned to it, and free'd it #2\n"); */

  malloc(1); // Screw up alignment.

  malloc(5);
  int *ptr3 = malloc(sizeof(int)-1);
  if (ptr3 == NULL) { 
    printf("Failed to malloc a single int\n");
    return 1;
  }

  /* *ptr3 = 3; */
  /* *ptr3 = 300; */

  free(ptr3);
  /* printf("malloc'd an int, assigned to it, and free'd it #3\n"); */
  malloc(4);

  return 0;
}
