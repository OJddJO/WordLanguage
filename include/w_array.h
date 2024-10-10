#ifndef __W_ARRAY_H__
#define __W_ARRAY_H__

#include <stdlib.h>
#include "w_type.h"
#include "w_int.h"
#include "w_float.h"
#include "w_str.h"

typedef struct _w_array_element {
    void *data;
    W_Array_Element *next;
    W_Array_Element *prev;
} W_Array_Element;

typedef struct _w_array {
    W_Type type;
    void **data;
    int capacity;
} W_Array;

W_Array *array_init(int capacity, W_Type type);
void *array_get(W_Array *array, int index);
void array_set(W_Array *array, int index, void *value);
int array_size(W_Array *array);
void array_destroy(W_Array *array);

#endif