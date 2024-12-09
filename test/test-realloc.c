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
  

  /* // normal memcpy */
  /* void *ptr1 = malloc(1); */
  /* malloc(1); */
  /* ptr1 = realloc(ptr1, 2); // memcpy */
  /* malloc(1); // test previous location is free */

  /* // test split when shrink */
  /* void *ptr1 = malloc(34); // 32 + one extra for old and one for new */
  /* ptr1 = realloc(ptr1, 1); // shrink => split */
  /* malloc(1); */

  // remains the same. brk grows
  void *ptr1 = malloc(1);
  realloc(ptr1, 2);

  /* // test merge. En realidad antes vendría el que tuviera espacio de sobra. Para que el student puede fallar posteriormente en el edge case */
  /* void *ptr1 = malloc(1); */
  /* void *ptr2 = malloc(1); */
  /* malloc(1); */
  /* free(ptr2); */
  /* ptr1 = realloc(ptr1, 1 + 32 +1); // should be merged with next */

  /* // test merge and split */
  /* void *ptr1 = malloc(1); */
  /* void *ptr2 = malloc(2); */
  /* malloc(1); */
  /* free(ptr2); */
  /* ptr1 = realloc(ptr1, 2); // should split the remaining after merging */
  /* malloc(1); // it should fit peferctily in the split */

  return 0;
}
