#ifndef __W_ARRAY_H__
#define __W_ARRAY_H__

#include <stdlib.h>
#include <stdio.h>
#include "w_type.h"
#include "w_int.h"
#include "w_float.h"
#include "w_str.h"
#include "w_bool.h"

typedef struct _w_array {
    W_Type type;
    W_Type subtype;
    void **data;
    int capacity;
    void (*destroy)(struct _w_array *);
    void (*print)(struct _w_array *);
} W_Array;

W_Array *array_init(W_Type type, int capacity);
void *array_get(W_Array *array, int index);
void array_set(W_Array *array, int index, void *value);
int array_index(W_Array *array, void *value);
int array_size(W_Array *array);
void array_print(W_Array *array);
void array_destroy(W_Array *array);

#endif