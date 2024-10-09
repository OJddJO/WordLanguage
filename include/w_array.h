#ifndef __W_ARRAY_H__
#define __W_ARRAY_H__

#include <stdlib.h>

typedef struct _w_array {
    void **data;
    int size;
    int capacity;
} W_Array;

W_Array *array_init(int capacity);
void *array_get(W_Array *array, int index);
void array_set(W_Array *array, int index, void *value);
int array_size(W_Array *array);
void array_destroy(W_Array *array);

#endif