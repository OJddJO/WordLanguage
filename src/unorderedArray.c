#include <stdlib.h>
#include <stddef.h>

#include "unorderedArray.h"

#define DEFAULT_SIZE (1<<3)
#define GROWTH_FACTOR 1.5

int uArrayInit(UArray *array) {
    UArray ret = {
        .size = DEFAULT_SIZE,
        .count = 0,
        .buf = (void **)malloc(sizeof(void *) * DEFAULT_SIZE),
    };

    if (!ret.buf) return 0;

    *array = ret;
    return 1;
}

int uArrayAdd(UArray *array, void *element) {
    if (array->size == array->count) {
        size_t targetSize = array->size * GROWTH_FACTOR;
        void **new = (void **)realloc(array->buf, targetSize);
        if (!new) return 0;

        array->buf = new;
        array->size = targetSize;
    }

    array->buf[array->count++] = element;
    return 1;
}

void *uArrayPopIdx(UArray *array, uint64_t idx) {
    if (idx >= array->count) return NULL;

    void *ret = array->buf[idx];
    array->buf[idx] = array->buf[--array->count];

    if (array->size > DEFAULT_SIZE && array->count == array->size / GROWTH_FACTOR / GROWTH_FACTOR) {
        size_t targetSize = array->size / GROWTH_FACTOR;
        void **new = (void **)realloc(array->buf, targetSize);
        if (new) {
            array->buf = new;
            array->size = targetSize;
        }
    }

    return ret;
}

void *uArrayPop(UArray *array) {
    return uArrayPopIdx(array, array->count - 1);
}

int uArrayIndex(UArray *array, void *element, int (*cmp)(void *, void *)) {
    for (uint64_t i = 0; i < array->count; i++) {
        if (cmp(element, array->buf[i])) return i;
    }
    return -1;
}

void *uArrayFind(UArray *array, void *element, int (*cmp)(void *, void *)) {
    for (uint64_t i = 0; i < array->count; i++) {
        if (cmp(element, array->buf[i])) return array->buf[i];
    }
    return NULL;
}
