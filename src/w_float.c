#include "w_float.h"

/**
 * \brief Initializes a new float.
 * \return A pointer to the newly created float.
 */
W_Float *float_init() {
    W_Float *f = (W_Float *)malloc(sizeof(W_Float));
    f->type = NULL_TYPE;
    f->value = 0.0;
    return f;
}

/**
 * \brief Sets the value of the given float.
 * \param f The float to set the value of.
 * \param value The value to set.
 */
void float_set(W_Float *f, double value) {
    f->type = FLOAT;
    f->value = value;
}

/**
 * \brief Gets the value of the given float.
 * \param f The float to get the value of.
 * \return The value of the float.
 */
double float_value(W_Float *f) {
    return f->value;
}

/**
 * \brief Destroys the given float.
 * \param f The float to destroy.
 */
void float_destroy(W_Float *f) {
    free(f);
}