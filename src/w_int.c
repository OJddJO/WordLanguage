#include "w_int.h"

/**
 * \brief Initializes a new integer. (w_malloc)
 * \return A pointer to the newly created integer.
 */
W_Int *w_int_init() {
    W_Int *i = (W_Int *)w_malloc(sizeof(W_Int));
    i->type = NULL_TYPE;
    i->value = NULL;
    i->destroy = &w_int_destroy;
    i->stringify = &w_int_stringify;
    i->copy = &w_int_copy;
    i->assign = &w_int_assign;
    i->set = &w_int_set;
    i->get = &w_int_value;
    return i;
}

/**
 * \brief Assigns the value of the given integer. (w_malloc)
 * \param i The integer to set the value of.
 * \param value The value to set as a string.
 */
void w_int_assign(W_Int *i, char *value) {
    float *v;
    float var_value = atof(value);
    if (i->type == NULL_TYPE) {
        v = (float *)w_malloc(sizeof(float));
        i->type = INT;
    } else v = i->value;
    *v = roundf(var_value);
    i->value = v;
}

/**
 * \brief Sets the value of the given integer. (w_malloc)
 * \param i The integer to set the value of.
 * \param value The value to set.
 */
void w_int_set(W_Int *i, float value) {
    float *v;
    if (i->type == NULL_TYPE) {
        v = (float *)w_malloc(sizeof(float));
        i->type = INT;
    } else v = i->value;
    *v = value;
    i->value = v;
}

/**
 * \brief Gets the value of the given integer.
 * \param i The integer to get the value of.
 * \return The value of the integer.
 */
float *w_int_value(W_Int *i) {
    return i->value;
}

/**
 * \brief Stringifies the given integer. (w_malloc)
 * \param i The integer to stringify.
 */
char *w_int_stringify(W_Int *i) {
    int len = snprintf(NULL, 0, "%g", *i->value);
    char *str = (char *)w_malloc(len + 1);
    sprintf(str, "%g", *i->value);
    return str;
}

/**
 * \brief Destroys the given integer.
 * \param i The integer to destroy.
 */
void w_int_destroy(W_Int *i) {
    if (i->value != NULL) w_free(i->value);
    w_free(i);
}

/**
 * \brief Copies the given integer. (w_malloc)
 * \param i The integer to copy.
 * \return A copy of the integer.
 */
W_Int *w_int_copy(W_Int *i) {
    W_Int *copy = w_int_init();
    w_int_set(copy, *i->value);
    return copy;
}