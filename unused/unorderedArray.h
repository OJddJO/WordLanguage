#ifndef __UNORDEREDARRAY_H__
#define __UNORDEREDARRAY_H__

#include <stdint.h>

typedef struct _UArray {
    uint64_t    size;
    uint64_t    count;
    void        **buf;
} UArray;

int uArrayInit(UArray *array);
int uArrayAdd(UArray *array, void *element);
void *uArrayPopIdx(UArray *array, uint64_t idx);
void *uArrayPop(UArray *array);
int uArrayIndex(UArray *array, void *element, int (*cmp)(void *, void *));
void *uArrayFind(UArray *array, void *element, int (*cmp)(void *, void *));

#endif
