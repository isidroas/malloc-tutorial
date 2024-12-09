#include <assert.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
// Don't include stdlb since the names will conflict?

// TODO: align

// sbrk some extra space every time we need it.
// This does no bookkeeping and therefore has no ability to free, realloc, etc.
void *nofree_malloc(size_t size) {
  void *p = sbrk(0);
  void *request = sbrk(size);
  if (request == (void*) -1) { 
    return NULL; // sbrk failed
  } else {
    assert(p == request); // Not thread safe.
    return p;
  }
}

struct block_meta {
  size_t size;
  struct block_meta *next;
  struct block_meta *prev;
  int free;
  int magic;    // For debugging only. TODO: remove this in non-debug mode.
};

#define META_SIZE sizeof(struct block_meta) // = 8 +8 +4 +4 =24 bytes

void *global_base = NULL;

// Iterate through blocks until we find one that's large enough.
// TODO: split block up if it's larger than necessary
struct block_meta *find_free_block(struct block_meta **last, size_t size) {
  struct block_meta *current = global_base;
  while (current && !(current->free && current->size >= size)) {
    *last = current;
    current = current->next;
  }
  return current;
}

struct block_meta *request_space(struct block_meta* last, size_t size) {
  struct block_meta *block;
  block = sbrk(0); //pa que hacer esto si ya lo obtienes en la siguiente?
  void *request = sbrk(size + META_SIZE);
  assert((void*)block == request); // Not thread safe.
  if (request == (void*) -1) {
    return NULL; // sbrk failed.
  }
  
  if (last) { // NULL on first request.
    last->next = block;
  }
  block->prev = last;
  block->size = size;
  block->next = NULL;
  block->free = 0;
  block->magic = 0x12345678;
  return block;
}


struct block_meta * split(struct block_meta * block, size_t size){
      if ((block->size - size) <=  META_SIZE ){ // TODO: create variable with difference that below will be used?
        return NULL;
      }
      struct block_meta *new_block = (void *)(block + 1) + size;
      new_block->next = block->next;
      new_block->prev = block;
      new_block->free = 1;
      new_block->magic = 0x66666666;
      new_block->size = block->size - size -  META_SIZE;
      block->size = size;
      block->next = new_block;
      return new_block;
}

// If it's the first ever call, i.e., global_base == NULL, request_space and set global_base.
// Otherwise, if we can find a free block, use it.
// If not, request_space.
void *malloc(size_t size) {
  struct block_meta *block;
  // TODO: align size?

  if (size <= 0) {
    return NULL;
  }

  if (!global_base) { // First call.
    block = request_space(NULL, size);
    if (!block) {
      return NULL;
    }
    global_base = block;
  } else {
    struct block_meta *last = global_base;
    block = find_free_block(&last, size);
    if (!block) { // Failed to find free block.
      block = request_space(last, size);
      if (!block) {
        return NULL;
      }
    } else {      // Found free block
      split(block, size);
      block->free = 0;
      block->magic = 0x77777777;
    }
  }
  
  return(block+1);
}

void *calloc(size_t nelem, size_t elsize) {
  size_t size = nelem * elsize;
  void *ptr = malloc(size);
  memset(ptr, 0, size);
  return ptr;
}

// TODO: maybe do some validation here.
struct block_meta *get_block_ptr(void *ptr) {
  return (struct block_meta*)ptr - 1;
}

void merge_with_next(struct block_meta *block_ptr){
  struct block_meta * next = block_ptr->next;
  if ( next && next->free){
    assert (next->prev == block_ptr);
    block_ptr->next = next->next;
    block_ptr->size += next->size + META_SIZE;
    if (next->next)
      next->next->prev = block_ptr;
    /* block_ptr == */
  }
}

void free(void *ptr) {
  if (!ptr) {
    return;
  }

  // TODO: consider merging blocks once splitting blocks is implemented.
  struct block_meta* block_ptr = get_block_ptr(ptr);
  assert(block_ptr->magic == 0x77777777 || block_ptr->magic == 0x12345678); // this fails if program uses aligned_alloc()
  assert(block_ptr->free == 0);
  block_ptr->free = 1;
  block_ptr->magic = 0x55555555;  
  merge_with_next(block_ptr);
  if (block_ptr-> prev && block_ptr->prev->free)
    merge_with_next(block_ptr->prev);
}

void *realloc(void *ptr, size_t size) {
  if (!ptr) { 
    // NULL ptr. realloc should act like malloc.
    return malloc(size);
  }

  struct block_meta* block_ptr = get_block_ptr(ptr);
  if (block_ptr->size >= size) {
    // We have enough space. Could free some
    struct block_meta* new_block = split(block_ptr, size);
    if (new_block){
      merge_with_next(new_block);
    }
    return ptr;
  }

  /* if(!block_ptr->next) */
  /*   // grow brk */
  if (block_ptr->next->free && block_ptr->next->size > size-block_ptr->size){ // TODO: negative overflow?
      merge_with_next(block_ptr);
      /* split(block_ptr, size); */
  }

  // Need to really realloc. Malloc new space and free old space.
  // Then copy old data to new space.
  void *new_ptr;
  new_ptr = malloc(size);
  if (!new_ptr) {
    return NULL; // TODO: set errno on failure.
  }
  memcpy(new_ptr, ptr, block_ptr->size);
  free(ptr);  
  return new_ptr;
}
