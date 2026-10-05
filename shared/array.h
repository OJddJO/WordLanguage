#ifndef __ARRAY_H__
#define __ARRAY_H__

#include <stdint.h>

typedef struct _Array {
    size_t      size;
    size_t      count;
    size_t      elemSize;
    int8_t      *buf;
} Array;

int arrayInit(Array *array, size_t elemSize);
int arrayAdd(Array *array, void *elem);
void arrayDestroy(Array *array);

#endif
