#include "w_int.h"

/**
 * \brief Initializes a new integer. (malloc)
 * \return A pointer to the newly created integer.
 */
W_Int *w_int_init() {
    W_Int *i = (W_Int *)malloc(sizeof(W_Int));
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
 * \brief Assigns the value of the given integer. (malloc)
 * \param i The integer to set the value of.
 * \param value The value to set as a string.
 */
void w_int_assign(W_Int *i, char *value) {
    int *v;
    int var_value = atoi(value);
    if (i->type == NULL_TYPE) {
        v = (int *)malloc(sizeof(int));
        i->type = INT;
    } else v = i->value;
    *v = var_value;
    i->value = v;
}

/**
 * \brief Sets the value of the given integer. (malloc)
 * \param i The integer to set the value of.
 * \param value The value to set.
 */
void w_int_set(W_Int *i, int value) {
    int *v;
    if (i->type == NULL_TYPE) {
        v = (int *)malloc(sizeof(int));
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
int *w_int_value(W_Int *i) {
    return i->value;
}

/**
 * \brief Stringifies the given integer. (malloc)
 * \param i The integer to stringify.
 */
char *w_int_stringify(W_Int *i) {
    int len = snprintf(NULL, 0, "%d", *i->value);
    char *str = (char *)malloc(len + 1);
    sprintf(str, "%d", *i->value);
    return str;
}

/**
 * \brief Destroys the given integer.
 * \param i The integer to destroy.
 */
void w_int_destroy(W_Int *i) {
    if (i->value != NULL) free(i->value);
    free(i);
}

/**
 * \brief Copies the given integer. (malloc)
 * \param i The integer to copy.
 * \return A copy of the integer.
 */
W_Int *w_int_copy(W_Int *i) {
    W_Int *copy = w_int_init();
    w_int_set(copy, *i->value);
    return copy;
}