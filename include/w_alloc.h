// DEBUG ONLY

#ifndef __W_ALLOC_H__
#define __W_ALLOC_H__

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define MONITOR_MEMORY false

#if MONITOR_MEMORY

#define DEBUG true

extern int w_alloc_count;
extern int w_alloc_total;

void *w_malloc(size_t size);
void w_free(void *ptr);
void w_alloc_print();

#elif !MONITOR_MEMORY

#define w_malloc malloc
#define w_free free
#define w_alloc_print() {}

#endif

#endif