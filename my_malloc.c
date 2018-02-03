#include "my_malloc.h"
#include <unistd.h>
#include <pthread.h>

typedef struct block {
  unsigned long size;
  struct block *next;
  struct block *prev;
  int used;
  int padding;
}block;

typedef struct {
  int flag;
  int used;
  unsigned long size;
}BlockLockFree, *BlockLockFreePtr;

typedef block* block_ptr;

static block           _g_head = {0, NULL, NULL, 1, 0};
static pthread_mutex_t _g_lock = {{0}, 0};

static BlockLockFree   _g_head_lf = {1, 1, 0};
const  size_t BLK_LF_SIZE = sizeof(BlockLockFree);

const size_t blk_size    = sizeof(block);
const size_t ALLOC_MULTI = 8;


#define ALLOC_SIZE (100000 * blk_size)
void *get_mem(size_t size);

void* get_mem_lockfree(block* head, size_t size);

int compare_exchange_weak(int* ptr, int oldval, int newval);

void *ts_malloc_lock(size_t size) {
//  if (NULL == _g_head.next) {
//    pthread_mutex_init(&_g_lock, NULL);
//  }
  ///! Modify the size
  if (size % blk_size != 0)
    size = ((size + blk_size) / blk_size) * blk_size;

  // while (0 != pthread_mutex_trylock(&_g_lock)) ;
  pthread_mutex_lock(&_g_lock);

  block_ptr p     = &_g_head;
  block_ptr index = NULL; // best fit block's position
  while (p->next) { // find best fit block
    if ((p->next->used == 0) && (p->next->size >= size)
        && ((NULL == index) || (p->next->size < index->size) )) {
          index = p->next;
    }
    p = p->next;
  }

  if (NULL == index) { ///! No available memory
    block_ptr new_block = get_mem(size);
    if (new_block == NULL) return NULL;

    p->next         = new_block;
    new_block->prev = p;
    index           = new_block;
  } else if (index->size - size < 2 * blk_size) {
    index->used = 1;
  } else {
    block_ptr new_block = index + 1 + size / blk_size;
    new_block->next     = index->next;
    if (index->next) index->next->prev = new_block;
    new_block->prev     = index;
    index->next         = new_block;
    new_block->used     = 0;
    new_block->size     = index->size - size - blk_size;
    index->size         = size;
    index->used         = 1;
  }

  pthread_mutex_unlock(&_g_lock);
  return (void*)(index + 1);
}

void ts_free_lock(void *ptr) {
  block_ptr addr = (block_ptr)ptr - 1;
  block_ptr p    = &_g_head;
  // block_ptr end  = NULL;

  // while (0 != pthread_mutex_trylock(&_g_lock)) ;
  pthread_mutex_lock(&_g_lock);

  while (p->next && p->next != addr) p = p->next;
  if (!p->next) return; // not found

  ///! take back the memory.
  addr->used = 0;

  block_ptr ref = p->next;
  // merge to the left block
  if ((NULL != ref->prev) && (0 == ref->prev->used)) {
    ref->prev->size      += (ref->size + blk_size);
    ref->prev->next       = ref->next;
    if (ref->next) ref->next->prev = ref->prev;
    ref = ref->prev;
  }

  // merge to the right block
  if ((NULL != ref->next) && (0 == ref->next->used)) {
    ref->size      += (ref->next->size + blk_size);
    ref->next       =  ref->next->next;
    if (ref->next)     ref->next->prev = ref;
  }
  pthread_mutex_unlock(&_g_lock);
///! It's not necessary.
//  if (end && end->next == NULL) {
//    brk(end);
//
//    p = head;
//    while (p->next && p->next != end)
//      p = p->next;
//
//    p->next = NULL;
//  }
}

void* ts_malloc_nolock(size_t size) {
  ///! Modify the size
  if (size % blk_size != 0)
    size = ((size + blk_size) / blk_size) * blk_size;

  block_ptr p     = &_g_head;
  block_ptr index = NULL; // best fit block's position

  do {
    p     = &_g_head;
    index = NULL;
    while (p->next) { // find best fit block
      if ((p->next->used == 0) && (p->next->size >= size)
          && ((NULL == index) || (p->next->size < index->size) )) {
        index = p->next;
      }
      p = p->next;
    }

    if (NULL == index) { ///! No available memory
      return get_mem_lockfree(p, size) + blk_size;
    }
  } while (compare_exchange_weak(&(index->used), 0, 1));

  return (void*)(index + 1);
}

void  ts_free_nolock(void* ptr) {
  block_ptr addr = (block_ptr)ptr - 1;
  block_ptr p    = &_g_head;

  while (p->next && p->next != addr) p = p->next;
  if (!p->next) return; // not found

  ///! take back the memory.
  addr->used = 0;
}

void *get_mem(size_t size) {
  unsigned long alloc_size = size + blk_size;
  if (alloc_size < ALLOC_SIZE) alloc_size = ALLOC_SIZE;

  block_ptr p = sbrk(0);
  if (sbrk(alloc_size) == (void*)-1)
    return NULL;

  if (alloc_size - blk_size - size < 2 * blk_size) {
    p->size = size;
    p->next = NULL;
    p->prev = NULL;
    p->used = 1;
  }

  block_ptr new_block = p + 1 + size / blk_size;
  new_block->size = alloc_size - 2 * blk_size - size;
  new_block->next = NULL;
  new_block->prev = p;
  new_block->used = 0;

  p->size = size;
  p->next = new_block;
  p->used = 1;

  return (void*)p;
}

void* get_mem_lockfree(block* head, size_t size) {
  unsigned long alloc_size = size + blk_size;
  // if (alloc_size < ALLOC_SIZE) alloc_size = ALLOC_SIZE;

  pthread_mutex_lock(&_g_lock);
  block_ptr p = sbrk(0);
  if (sbrk(alloc_size) == (void*)-1) {
    pthread_mutex_unlock(&_g_lock);
    return NULL;
  }
  pthread_mutex_unlock(&_g_lock);
  p->size = size;
  p->next = NULL;
  p->prev = head;
  p->used = 1;

  if (NULL != head->next) {
    p->next          = head->next;
    head->next->prev = p;
    head->next       = p;
  }

  return (void*)p;
}

int compare_exchange_weak(int* ptr, int oldval, int newval) {
  if (oldval == *ptr) {
    *ptr = newval;
    return 1;
  }

  return 0;
}
