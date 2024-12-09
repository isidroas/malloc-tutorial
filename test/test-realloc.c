#include <stdio.h>
#include <stdlib.h>

int main() {

  /* void *ptr1 = malloc(1); */
  /* ptr1 = realloc(ptr1, 2); // remains the same. brk grows */
  /* void * ptr2 = malloc(1); */
  /* free(ptr2); */
  /* ptr1 = realloc(ptr1, 3); // the next block is fully ocupied? */
  /* void * ptr3 = malloc(2); */
  /* free(ptr3); */
  /* ptr1=realloc(ptr1, 4); // the next block is split */

  // shrink time:
  /* ptr=realloc(ptr, 5); */
  // TODO: test split and no split if too small
  
  // test split when shrink
  void *ptr1 = malloc(34); // 32 + one extra for old and one for new
  ptr1 = realloc(ptr1, 1); // shrink => split
  malloc(1);

  return 0;
}