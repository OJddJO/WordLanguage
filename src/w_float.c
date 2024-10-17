#include "w_float.h"

/**
 * \brief Initializes a new float. (malloc)
 * \return A pointer to the newly created float.
 */
W_Float *float_init() {
    W_Float *f = (W_Float *)malloc(sizeof(W_Float));
    f->type = NULL_TYPE;
    f->value = NULL;
    f->destroy = &float_destroy;
    return f;
}

/**
 * \brief Sets the value of the given float. (malloc)
 * \param f The float to set the value of.
 * \param value The value to set.
 */
void float_set(W_Float *f, double value) {
    double *v;
    if (f->type == NULL_TYPE) {
        v = (double *)malloc(sizeof(double));
        f->type = FLOAT;
    } else v = f->value;
    *v = value;
    f->value = v;
}

/**
 * \brief Gets the value of the given float.
 * \param f The float to get the value of.
 * \return The value of the float.
 */
double *float_value(W_Float *f) {
    return f->value;
}

/**
 * \brief Destroys the given float.
 * \param f The float to destroy.
 */
void float_destroy(W_Float *f) {
    if (f->value != NULL) free(f->value);
    free(f);
}