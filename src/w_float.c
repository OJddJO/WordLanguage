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
    f->stringify = &float_stringify;
    f->copy = &float_copy;
    f->assign = &float_assign;
    f->set = &float_set;
    f->get = &float_value;
    return f;
}

/**
 * \brief Assigns the value of the given float. (malloc)
 * \param f The float to set the value of.
 * \param value The value to set as a string.
 */
void float_assign(W_Float *f, char *value) {
    double *v;
    double var_value = atof(value);
    if (f->type == NULL_TYPE) {
        v = (double *)malloc(sizeof(double));
        f->type = FLOAT;
    } else v = f->value;
    *v = var_value;
    f->value = v;
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
 * \brief Converts the given float to a string. (malloc)
 * \param f The float to convert.
 * \return The float as a string.
 */
char *float_stringify(W_Float *f) {
    int len = snprintf(NULL, 0, "%f", *f->value);
    char *str = (char *)malloc(len + 1);
    sprintf(str, "%f", *f->value);
    return str;
}

/**
 * \brief Destroys the given float.
 * \param f The float to destroy.
 */
void float_destroy(W_Float *f) {
    if (f->value != NULL) free(f->value);
    free(f);
}

/**
 * \brief Copies the given float. (malloc)
 * \param f The float to copy.
 * \return A copy of the float.
 */
W_Float *float_copy(W_Float *f) {
    W_Float *copy = float_init();
    float_set(copy, *f->value);
    return copy;
}