#include <stdlib.h>
#include <stddef.h>

#include "array.h"

Array array_init() {
    Array ret = {
        .size = ARRAY_DEFAULT_SIZE,
        .count = 0,
        .buf = (void **)malloc(sizeof(void *) * ARRAY_DEFAULT_SIZE)
    };
    return ret;
}

int array_append(Array *array, void *element) {
    if (array->size == array->count) {
        size_t targetSize = array->size * ARRAY_GROWTH_FACTOR;
        void **new = (void **)realloc(array->buf, targetSize);
        if (!new) return 0;

        array->buf = new;
        array->size = targetSize;
    }

    array->buf[array->count++] = element;
    return 1;
}

void *array_pop(Array *array) {

}
