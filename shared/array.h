#ifndef __ARRAY_H__
#define __ARRAY_H__

#include <stdint.h>
#include <stddef.h>

typedef struct _Array {
    size_t      size;
    size_t      count;
    size_t      elemSize;
    int8_t      *buf;
} Array;

bool arrayInit(Array *array, size_t elemSize);
bool arrayAdd(Array *array, void *elem);
void *arrayGet(Array *array, size_t index);
void arrayDestroy(Array *array);

#endif
