#ifndef MY_MALLOC_H_
#define MY_MALLOC_H_

#include <stddef.h>

void *ff_malloc(size_t size);
void ff_free(void *ptr);

void *bf_malloc(size_t size);
void bf_free(void *ptr);

unsigned long get_data_segment_size();
unsigned long get_data_segment_free_space_size();

#endif // MY_MALLOC_H_
