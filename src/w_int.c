#include "w_int.h"

/**
 * \brief Initializes a new integer.
 * \return A pointer to the newly created integer.
 */
W_Int *int_init() {
    int *i = (int *)malloc(sizeof(int));
    return i;
}

/**
 * \brief Sets the value of the given integer.
 * \param i The integer to set the value of.
 * \param value The value to set.
 */
void int_set(W_Int *i, int value) {
    i->value = value;
}

/**
 * \brief Gets the value of the given integer.
 * \param i The integer to get the value of.
 * \return The value of the integer.
 */
int int_value(W_Int *i) {
    return i->value;
}

/**
 * \brief Destroys the given integer.
 * \param i The integer to destroy.
 */
void int_destroy(W_Int *i) {
    free(i);
}