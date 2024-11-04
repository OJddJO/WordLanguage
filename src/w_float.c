#include "w_float.h"

/**
 * \brief Initializes a new float. (w_malloc)
 * \return A pointer to the newly created float.
 */
W_Float *w_float_init() {
    W_Float *f = (W_Float *)w_malloc(sizeof(W_Float));
    f->type = NULL_TYPE;
    f->value = NULL;
    f->destroy = &w_float_destroy;
    f->stringify = &w_float_stringify;
    f->copy = &w_float_copy;
    f->assign = &w_float_assign;
    f->set = &w_float_set;
    f->get = &w_float_value;
    return f;
}

/**
 * \brief Assigns the value of the given float. (w_malloc)
 * \param f The float to set the value of.
 * \param value The value to set as a string.
 */
void w_float_assign(W_Float *f, char *value) {
    float *v;
    float var_value = atof(value);
    if (f->type == NULL_TYPE) {
        v = (float *)w_malloc(sizeof(float));
        f->type = FLOAT;
    } else v = f->value;
    *v = var_value;
    f->value = v;
}

/**
 * \brief Sets the value of the given float. (w_malloc)
 * \param f The float to set the value of.
 * \param value The value to set.
 */
void w_float_set(W_Float *f, float value) {
    float *v;
    if (f->type == NULL_TYPE) {
        v = (float *)w_malloc(sizeof(float));
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
float *w_float_value(W_Float *f) {
    return f->value;
}

/**
 * \brief Converts the given float to a string. (w_malloc)
 * \param f The float to convert.
 * \return The float as a string.
 */
char *w_float_stringify(W_Float *f) {
    int len = snprintf(NULL, 0, "%f", *f->value);
    char *str = (char *)w_malloc(len + 1);
    sprintf(str, "%f", *f->value);
    return str;
}

/**
 * \brief Destroys the given float.
 * \param f The float to destroy.
 */
void w_float_destroy(W_Float *f) {
    if (f->value != NULL) free(f->value);
    free(f);
}

/**
 * \brief Copies the given float. (w_malloc)
 * \param f The float to copy.
 * \return A copy of the float.
 */
W_Float *w_float_copy(W_Float *f) {
    W_Float *copy = w_float_init();
    w_float_set(copy, *f->value);
    return copy;
}