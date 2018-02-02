#ifndef MY_MALLOC_H_
#define MY_MALLOC_H_

#include <stddef.h>

void* ts_malloc_lock(size_t s);
void  ts_free_lock(void* p);

void* ts_malloc_nolock(size_t s);
void  ts_free_nolock(void* p);

#endif // MY_MALLOC_H_
