#include "w_bool.h"

/**
 * \brief Initializes a new boolean.
 * \return A pointer to the newly created boolean.
 */
W_Bool *bool_init() {
    W_Bool *b = (W_Bool *)malloc(sizeof(W_Bool));
    b->type = NULL_TYPE;
    b->value = NULL;
    return b;
}

/**
 * \brief Sets the value of the given boolean.
 * \param b The boolean to set the value of.
 * \param value The value to set.
 */
void bool_set(W_Bool *b, bool value) {
    bool *v;
    if (b->type != NULL_TYPE) {
        v = (bool *)malloc(sizeof(bool));
        b->type = BOOL;
    } else v = b->value;
    *v = value;
    b->value = v;
}

/**
 * \brief Gets the value of the given boolean.
 * \param b The boolean to get the value of.
 * \return The value of the boolean.
 */
bool bool_value(W_Bool *b) {
    return *(b->value);
}