#include <unistd.h>
#include <assert.h>
#include "my_malloc.h"

enum {_ALIGN = 8};

#define _ROUND_UP_BLK(__bytes) \
    (((__bytes) + (size_t)BLK_SIZE - 1) & ~((size_t)BLK_SIZE - 1))

#define _ROUND_UP_8(__bytes) \
    (((__bytes) + (size_t)_ALIGN - 1) & ~((size_t)_ALIGN - 1))

#define _ADD_BLK(__bytes) \
    ( (__bytes) + BLK_SIZE )

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
} Block, *BlkPtr;

static Block __g_head         = {0, 0, NULL};
const size_t  BLK_SIZE        = sizeof(Block);
const size_t  EXP_MULTI       = 8;

BlkPtr ask_for_mem(const size_t);

void *ts_malloc_lock(size_t size) {
  assert(size > 0);
  ///! Modify the size
  size = _ROUND_UP_8(size + BLK_SIZE);

  BlkPtr idx = &__g_head;
  BlkPtr ref = NULL; // best fit block's position
  while (idx->next) {
    if ((idx->next->remainder >= size)
        && ((NULL == ref) || (idx->next->remainder < ref->remainder))) {
        ref = idx;
    }

    idx = idx->next;
  } // end while

  ///! No availiable memery
  if (NULL == ref) {
    idx->next = ask_for_mem(EXP_MULTI * size);
    if (NULL == idx->next) return NULL;

    ///! recurse call
    return ts_malloc_lock(size);
  }

  BlkPtr _new_blk = (void*)ref + BLK_SIZE + (ref->size - ref->remainder);
  _new_blk->size      = ref->remainder - BLK_SIZE;
  _new_blk->remainder = _new_blk->size - size;
  ref->size      -= size;
  ref->remainder -= size;

  if (ref->size - size < 2 * BLK_SIZE) {
    ref->remainder = 1;
    return (void*)(ref + 1);
  } else { // split
    BlkPtr new_block = ref + 1 + size / BLK_SIZE;
    new_block->size = ref->size - size - BLK_SIZE;
    new_block->next = ref->next;
    new_block->remainder = 0;

    ref->size = size;
    ref->next = new_block;
    ref->remainder = 1;
    return (void*)(ref + 1);
  }
}

void ts_free_lock(void *ptr)
{
  BlkPtr addr = (BlkPtr)ptr - 1;
  BlkPtr p = __g_head;
  BlkPtr end = NULL;

  while (p->next && p->next != addr)
    p = p->next;

  if (!p->next) // not found
    return ;

  // merge left block and mid block
  if (p->remainder == 0 && ((p + 1 + p->size / BLK_SIZE) == addr))
  {
    p->size = p->size + BLK_SIZE + addr->size;
    p->next = addr->next;

    // merge left block, mid block and right block
    if (p->next
        && p->next->remainder == 0
        && ((p + 1 + p->size / BLK_SIZE) == p->next))
    {
      p->size = p->size + BLK_SIZE + p->next->size;
      p->next = p->next->next;
    }
    end = p;
  }
  else
  {
    // merge mid block and right block
    if (addr->next 
        && addr->next->remainder == 0
        && (addr + 1 + addr->size / BLK_SIZE == addr->next))
    {
      addr->size = addr->size + BLK_SIZE + addr->next->size;
      addr->next = addr->next->next;
      addr->remainder = 0;
    }
    else
      addr->remainder = 0;

    end = addr;
  }

  if (end && end->next == NULL)
  {
    brk(end);

    p = __g_head;
    while (p->next && p->next != end)
      p = p->next;

    p->next = NULL;
  }
}

BlkPtr ask_for_mem(const size_t ALLOC_SIZE) {
  BlkPtr _begin_addr = sbrk(ALLOC_SIZE);
  if (_begin_addr == (void*)-1) return NULL;

  _begin_addr->next       = NULL;
  _begin_addr->size       = ALLOC_SIZE;
  _begin_addr->remainder  = ALLOC_SIZE - BLK_SIZE;
  return _begin_addr;
}
