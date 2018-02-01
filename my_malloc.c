#include <unistd.h>
#include <assert.h>
#include "my_malloc.h"

enum {_ALIGN = 8};
#define _ROUND_UP_8(__bytes) \
    (((__bytes) + (size_t)_ALIGN - 1) & ~((size_t)_ALIGN - 1))

enum {
  USING = 0,
  FREE,
};

typedef struct __Block {
  ///! The total bytes.
  size_t          size;
  ///! The remainder bytes
  size_t          remainder;
  ///! The address of next Block
  struct __Block *next;
  ///! The address of prev Block
  struct __Block *prev;
  ///! The real starting address.
  char            mem[0];
} Block, *BlkPtr;

static Block __g_head         = {0, 0, NULL, NULL};
const size_t  BLK_SIZE        = sizeof(Block);
const size_t  EXP_MULTI       = 8;

BlkPtr ask_for_mem(const size_t);

void *ts_malloc_lock(size_t size) {
  assert(size > 0);
  ///! Modify the size
  size_t _m_size = _ROUND_UP_8(size + BLK_SIZE);

  BlkPtr idx = &__g_head;
  BlkPtr ref = NULL; // best fit block's position
  while (idx->next) {
    if ((idx->next->remainder >= _m_size)
        && ((NULL == ref) || (idx->next->remainder < ref->remainder))) {
        ref = idx;
    }

    idx = idx->next;
  } // end while

  ///! No availiable memery
  if (NULL == ref) {
    idx->next       = ask_for_mem(EXP_MULTI * _m_size);
    idx->next->prev = idx;
    if (NULL == idx->next) return NULL;

    ///! recurse call
    return ts_malloc_lock(_m_size);
  }

  BlkPtr _new_blk = (BlkPtr) (ref->mem + (ref->size - ref->remainder));
  _new_blk->size      = ref->remainder - BLK_SIZE;
  _new_blk->remainder = ref->remainder - _m_size;
  ref->size      = ref->size - ref->remainder;
  ref->remainder = 0;

  ///! Insert the new block to list
  _new_blk->next  = ref->next;
  if (ref->next) ref->next->prev = _new_blk;
  _new_blk->prev  = ref;
  ref->next       = _new_blk;

  return _new_blk->mem;
}

void ts_free_lock(void *ptr) {

  BlkPtr ref  = &__g_head;
  while (ref && ptr != ref->mem) ref = ref->next;
  // not found
  if (NULL == ref) return;

  ///! take back the memory.
  ref->remainder = ref->size;

  // merge to the left block
  while ((NULL != ref->prev) && (ref->prev->size == ref->prev->remainder)) {
    ref->prev->size      += (ref->size + BLK_SIZE);
    ref->prev->remainder += (ref->size + BLK_SIZE);
    ref->prev->next       = ref->next;
    if (ref->next) ref->next->prev = ref->prev;
    ref = ref->prev;
  }

  // merge to the right block
  while ((NULL != ref->next) && (ref->next->size == ref->next->remainder)) {
    ref->size      += (ref->next->size + BLK_SIZE);
    ref->remainder += (ref->next->size + BLK_SIZE);
    ref->next       =  ref->next->next;
    if (ref->next)     ref->next->prev = ref;
  }
}

BlkPtr ask_for_mem(const size_t ALLOC_SIZE) {
  BlkPtr _new_mem = sbrk(ALLOC_SIZE);
  if (_new_mem == (void*)-1) return NULL;

  _new_mem->prev       = NULL;
  _new_mem->next       = NULL;
  _new_mem->size       = ALLOC_SIZE;
  _new_mem->remainder  = ALLOC_SIZE;
  return _new_mem;
}
