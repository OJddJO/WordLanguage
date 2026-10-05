#include "array.h"

#include <stdlib.h>
#include <string.h>

#define DEFAULT_SIZE    8
#define GROWTH_FACTOR   1.5

int arrayInit(Array *array, size_t elemSize) {
    Array ret = {
        .size = DEFAULT_SIZE,
        .count = 0,
        .elemSize = elemSize,
        .buf = malloc(DEFAULT_SIZE * elemSize),
    };
    if (!ret.buf) return 0;
    *array = ret;
    return 1;
}

int arrayAdd(Array *array, void *elem) {
    if (array->size == array->count) {
        size_t targetSize = array->size * GROWTH_FACTOR;
        void *new = realloc(array->buf, targetSize * array->elemSize);
        if (!new) return 0;

        array->buf = new;
        array->size = targetSize;
    }

    memcpy(array->buf + (array->count++ * array->elemSize), elem, array->elemSize);
    return 1;
}

void arrayDestroy(Array *array) {
    free(array->buf);
}
