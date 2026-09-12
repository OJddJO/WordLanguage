#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#include "unorderedArray.h"

#define DEFAULT_SIZE (1<<3)
#define GROWTH_FACTOR 1.5

int uArrayInit(UArray *array, size_t elementSize) {
    UArray ret = {
        .size = DEFAULT_SIZE,
        .count = 0,
        .elemSize = elementSize,
        .buf = (uint8_t *)malloc(elementSize * DEFAULT_SIZE),
    };

    if (!ret.buf) return 0;

    *array = ret;
    return 1;
}

int uArrayAdd(UArray *array, void *element) {
    if (array->size == array->count) {
        size_t targetSize = array->size * GROWTH_FACTOR;
        uint8_t *new = (uint8_t *)realloc(array->buf,array->elemSize * targetSize);
        if (!new) return 0;

        array->buf = new;
        array->size = targetSize;
    }

    memcpy(array->buf + array->elemSize * array->count, element, array->elemSize);
    array->count++;
    return 1;
}

void uArrayPopIdx(UArray *array, uint64_t idx, void *out) {
    if (idx >= array->count || array->count == 0) return;

    if (out) memcpy(out, array->buf + idx * array->elemSize, array->elemSize);
    memcpy(array->buf + idx * array->elemSize, array->buf + --array->count * array->elemSize, array->elemSize);

    if (array->size > DEFAULT_SIZE && array->count == array->size / GROWTH_FACTOR / GROWTH_FACTOR) {
        size_t targetSize = array->size / GROWTH_FACTOR;
        uint8_t *new = (uint8_t *)realloc(array->buf, targetSize);
        if (new) {
            array->buf = new;
            array->size = targetSize;
        }
    }
}

void uArrayPop(UArray *array, void *out) {
    if (array->count == 0) return;
    uArrayPopIdx(array, array->count - 1, out);
}

void *_uArrayFind(UArray *array, void *element, int (*cmp)(void *, void *)) {
    for (uint64_t i = 0; i < array->count; i++) {
        if (cmp(element, array->buf + i * array->elemSize) == 0) return array->buf + i * array->elemSize;
    }
    return NULL;
}
