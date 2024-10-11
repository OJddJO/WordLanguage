#include "w_array.h"

static void **populate_array(W_Type type, int capacity) {
    void **data = (void **)malloc(capacity * sizeof(void *));
    for (int i = 0; i < capacity; i++) {
        switch (type) {
            case INT:
                data[i] = int_init();
            case FLOAT:
                data[i] = float_init();
            case STRING:
                data[i] = str_init();
            case BOOL:
                data[i] = bool_init();
        }
    }
    return data;
}

/**
 * \brief Initializes a new array with the given capacity.
 * \param capacity The initial capacity of the array.
 * \return A pointer to the newly created array.
 */
W_Array *array_init(int capacity, W_Type type) {
    W_Array *array = (W_Array *)malloc(sizeof(W_Array));
    array->type = ARRAY;
    array->subtype = type;
    array->data = populate_array(type, capacity);
    array->capacity = capacity;
    return array;
}

/**
 * \brief Gets the value at the given index in the array.
 * \param array The array to get the value from.
 * \param index The index of the value to get.
 */
void *array_get(W_Array *array, int index) {
    if (index < 0 || index >= array->capacity) {
        return NULL;
    }
    return array->data[index];
}


/**
 * \brief Sets the value at the given index in the array.
 * \param array The array to set the value in.
 * \param index The index of the value to set.
 * \param value The value to set.
 */
void array_set(W_Array *array, int index, void *value) {
    if (index < 0 || index >= array->capacity) {
        return;
    }
    if (((W_Var *) value)->type != array->type) {
        return;
    }
    array->data[index] = value;
}

/**
 * \brief Gets the size of the array.
 * \param array The array to get the size of.
 * \return The size of the array.
 */
int array_size(W_Array *array) {
    return array->capacity;
}

/**
 * \brief Destroys the given array.
 * \param array The array to destroy.
 */
void array_destroy(W_Array *array) {
    free(array->data);
    free(array);
}