#include <stdio.h>
#include <stdlib.h>

int main() {
  void *ptr1 = malloc(1);
  void *ptr2 = malloc(1);
  void *ptr3 = malloc(1);
  free(ptr1);
  free(ptr2);
  void *ptr4 = malloc(1+32+1);
  return 0;
}
