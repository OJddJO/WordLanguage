#ifndef __UNORDEREDARRAY_H__
#define __UNORDEREDARRAY_H__

#include <stdint.h>

typedef struct _UArray {
    uint64_t    size;
    uint64_t    count;
    size_t      elemSize;
    uint8_t     *buf;
} UArray;

int uArrayInit(UArray *array, size_t elementSize);
int uArrayAdd(UArray *array, void *element);
void uArrayPopIdx(UArray *array, uint64_t idx, void *out);
void uArrayPop(UArray *array, void *out);
void *_uArrayFind(UArray *array, void *element, int (*cmp)(void *, void *));
#define uArrayFind(array, element, cmp, type) ((type *)_uArrayFind(array, element, cmp))

#endif
