#ifndef MY_MALLOC_H_
#define MY_MALLOC_H_

#include <stddef.h>

void* ts_malloc_lock(size_t);
void  ts_free_lock(void*);

void* ts_malloc_nolock(size_t);
void  ts_free_nolock(void*);

#endif // MY_MALLOC_H_
