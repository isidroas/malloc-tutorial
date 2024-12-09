#include <stdio.h>
#include <stdlib.h>

int main() {

  /* void *ptr1 = malloc(1); */
  /* void *ptr2 = malloc(1); */
  /* void *ptr3 = malloc(1); */
  /* free(ptr1); */
  /* free(ptr2); */
  /* void *ptr4 = malloc(1+32+1); */
  /* return 0; */

  // test not merged if previous used. fixed with
  /*   diff --git a/malloc.c b/malloc.c */
  /*   --- a/malloc.c */
  /*   +++ b/malloc.c */
  /*   @@ -149,7 +149,7 @@ void free(void *ptr) { */
  /*       block_ptr->free = 1; */
  /*       block_ptr->magic = 0x55555555;   */
  /*       merge_with_next(block_ptr); */
  /*   -   if (block_ptr-> prev) */
  /*   +   if (block_ptr-> prev && block_ptr->prev->free) */
  /*           merge_with_next(block_ptr->prev); */
  malloc(1);
  void *ptr2 = malloc(1);
  free(ptr2);
  malloc(1);

}
