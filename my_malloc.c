#include <unistd.h>
#include <assert.h>
#include "my_malloc.h"

enum {_ALIGN = 8};

#define _ROUND_UP_BLK(__bytes) \
    (((__bytes) + (size_t)BLK_SIZE - 1) & ~((size_t)BLK_SIZE - 1))

#define _ROUND_UP_8(__bytes) \
    (((__bytes) + (size_t)_ALIGN - 1) & ~((size_t)_ALIGN - 1))

enum {
  USING = 0,
  FREE,
};

typedef struct block {
  const void*  BEGIN_ADDR;
  const size_t SIZE;

  size_t       remainder;
  int          padding;
  struct block *next;
} Block, *BlkPtr;

static BlkPtr __g_head        = NULL;
const size_t  BLK_SIZE        = sizeof(Block);
const size_t  EXP_MULTI       = 2;
const size_t  INIT_ALLOC_SIZE = 1024 * BLK_SIZE;

void* get_mem(size_t size);


BlkPtr ask_for_mem(const size_t);

void *ff_malloc(size_t size)
{
  BlkPtr p = NULL;

	if (size % BLK_SIZE != 0)
		size = ((size + BLK_SIZE) / BLK_SIZE) * BLK_SIZE;

  if (__g_head) 
  {
    p = __g_head;

    while (p->next && !(p->next->remainder == 0 && p->next->SIZE >= size))
      p = p->next;

    if (!p->next) 
    {
      BlkPtr new_block = (BlkPtr)get_mem(size);
      if (new_block == NULL)
        return NULL;
      p->next = new_block;
      return (void*)(new_block + 1);
    } 
    else if (p->next->SIZE > size) 
    {
      if (p->next->SIZE - size < 2 * BLK_SIZE)
      {
        p->next->remainder = 1;
        return (void*)(p->next + 1);
      } 
      else 
      { // split
        BlkPtr new_block = p->next + 1 + size / BLK_SIZE;
        new_block->SIZE = p->next->SIZE - size - BLK_SIZE;
        new_block->next = p->next->next;
        new_block->remainder = 0;

        p->next->SIZE = size;
        p->next->next = new_block;
        p->next->remainder = 1;
        return (void*)(p->next + 1);
      }
    } 
    else 
    { // p->next->size == size
      p->next->remainder = 1;
      return (void*)(p->next + 1);
    }
  }
  else // head == NULL
  {
      __g_head = sbrk(0);
      if (sbrk(BLK_SIZE) == (void*)-1)
        return NULL;
      __g_head->SIZE = 0;
      __g_head->next = NULL;
      __g_head->remainder = 1;

      return ff_malloc(size);
  }
}

void ff_free(void *ptr)
{
  BlkPtr addr = (BlkPtr)ptr - 1;
  BlkPtr p = __g_head;
  BlkPtr end = NULL;

  while (p->next && p->next != addr)
    p = p->next;

  if (!p->next) // not found
    return ;

  // merge left block and mid block
  if (p->remainder == 0 && ((p + 1 + p->SIZE / BLK_SIZE) == addr))
  {
    p->SIZE = p->SIZE + BLK_SIZE + addr->SIZE;
    p->next = addr->next;

    // merge left block, mid block and right block
    if (p->next
        && p->next->remainder == 0 
        && ((p + 1 + p->SIZE / BLK_SIZE) == p->next))
    {
      p->SIZE = p->SIZE + BLK_SIZE + p->next->SIZE;
      p->next = p->next->next;
    }
    end = p;
  }
  else
  {
    // merge mid block and right block
    if (addr->next 
        && addr->next->remainder == 0 
        && (addr + 1 + addr->SIZE / BLK_SIZE == addr->next))
    {
      addr->SIZE = addr->SIZE + BLK_SIZE + addr->next->SIZE;
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

void *bf_malloc(size_t size)
{
  assert(size > 0);
  ///! Modify the size
  size = _ROUND_UP_8(size);

  if (NULL == __g_head) {
    __g_head = ask_for_mem(EXP_MULTI * _ROUND_UP_BLK(size));
    if (NULL == __g_head)
      return NULL;
  }

  BlkPtr idx = __g_head;
  BlkPtr ref = NULL; // best fit block's position
  do {
    if ((idx->SIZE >= size)
        && ((NULL == ref) || (idx->SIZE < ref->SIZE))) {
        ref = idx;
    }

    idx = idx->next;
  } while (idx->next);

  while (idx->next) { // find best fit block
    if ((0 == idx->next->remainder)  && (idx->next->SIZE >= size)
        && ((NULL == ref) || (idx->next->SIZE < ref->SIZE))) {
        ref = idx;
    }

    idx = idx->next;
  } // end while

  if (ref) {
    if (ref->SIZE > size) {
      if (ref->SIZE - size < 2 * BLK_SIZE) {
        ref->remainder = 1;
        return (void*)(ref + 1);
      } else { // split
        BlkPtr new_block = ref + 1 + size / BLK_SIZE;
        new_block->SIZE = ref->SIZE - size - BLK_SIZE;
        new_block->next = ref->next;
        new_block->remainder = 0;

        ref->SIZE = size;
        ref->next = new_block;
        ref->remainder = 1;
        return (void*)(ref + 1);
      }
    } else { // index->size == size
      ref->remainder = 1;
      return (void*)(ref + 1);
    }
  } else {
    BlkPtr new_block = get_mem(size);
    if (new_block == NULL)
      return NULL;

    idx->next = new_block;
    return (void*)(new_block + 1);
  }
}

void bf_free(void *ptr)
{
  BlkPtr addr = (BlkPtr)ptr - 1;
  BlkPtr p = __g_head;
  BlkPtr end = NULL;

  while (p->next && p->next != addr)
    p = p->next;

  if (!p->next) // not found
    return ;

  // merge left block and mid block
  if (p->remainder == 0 && ((p + 1 + p->SIZE / BLK_SIZE) == addr))
  {
    p->SIZE = p->SIZE + BLK_SIZE + addr->SIZE;
    p->next = addr->next;

    // merge left block, mid block and right block
    if (p->next
        && p->next->remainder == 0 
        && ((p + 1 + p->SIZE / BLK_SIZE) == p->next))
    {
      p->SIZE = p->SIZE + BLK_SIZE + p->next->SIZE;
      p->next = p->next->next;
    }
    end = p;
  }
  else
  {
    // merge mid block and right block
    if (addr->next 
        && addr->next->remainder == 0 
        && (addr + 1 + addr->SIZE / BLK_SIZE == addr->next))
    {
      addr->SIZE = addr->SIZE + BLK_SIZE + addr->next->SIZE;
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

unsigned long get_data_segment_size()
{
  BlkPtr p = __g_head;
  unsigned long size = 0;

  while (p)
  {
    size += p->SIZE;
    p = p->next;
  }

  return size;
}

unsigned long get_data_segment_free_space_size()
{
  BlkPtr p = __g_head;
  unsigned long size = 0;

  while (p)
  {
    if (p->remainder == 0)
      size += p->SIZE;
    p = p->next;
  }

  return size;
}

BlkPtr ask_for_mem(const size_t ALLOC_SIZE) {
  BlkPtr _begin_addr = sbrk(ALLOC_SIZE + BLK_SIZE);
  if (_begin_addr == (void*)-1) return NULL;

  _begin_addr->BEGIN_ADDR = _begin_addr + BLK_SIZE;
  _begin_addr->next       = NULL;
  _begin_addr->SIZE       = ALLOC_SIZE;
  _begin_addr->remainder  = ALLOC_SIZE;
  return _begin_addr;
}

void *get_mem(size_t size) {
  const size_t ALLOC_SIZE = _ROUND_UP_8(EXP_MULTI * (size + BLK_SIZE));

  BlkPtr _begin_addr = sbrk(0);
  if (sbrk(ALLOC_SIZE) == (void*)-1)
    return NULL;

  if (ALLOC_SIZE > (size + BLK_SIZE)) {
    if (ALLOC_SIZE - BLK_SIZE - size < 2 * BLK_SIZE) {
      _begin_addr->SIZE = size;
      _begin_addr->next = NULL;
      _begin_addr->remainder = 1;
    } else { // split
      BlkPtr new_block = _begin_addr + 1 + size / BLK_SIZE;
      new_block->SIZE = ALLOC_SIZE - 2 * BLK_SIZE - size;
      new_block->next = NULL;
      new_block->remainder = 0;

      _begin_addr->SIZE = size;
      _begin_addr->next = new_block;
      _begin_addr->remainder = 1;
    }
  } else { // alloc_size == sizeof(block) + size
    _begin_addr->SIZE = size;
    _begin_addr->next = NULL;
    _begin_addr->remainder = 1;
  }

  return (void*)_begin_addr;
}
