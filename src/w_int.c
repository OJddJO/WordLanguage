#include "w_int.h"

/**
 * \brief Initializes a new integer. (malloc)
 * \return A pointer to the newly created integer.
 */
W_Int *int_init() {
    W_Int *i = (W_Int *)malloc(sizeof(W_Int));
    i->type = NULL_TYPE;
    i->value = NULL;
    i->destroy = &int_destroy;
    i->stringify = &int_stringify;
    i->copy = &int_copy;
    i->assign = &int_assign;
    i->set = &int_set;
    i->get = &int_value;
    return i;
}

/**
 * \brief Assigns the value of the given integer. (malloc)
 * \param i The integer to set the value of.
 * \param value The value to set as a string.
 */
void int_assign(W_Int *i, char *value) {
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
void int_set(W_Int *i, int value) {
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
int *int_value(W_Int *i) {
    return i->value;
}

/**
 * \brief Stringifies the given integer. (malloc)
 * \param i The integer to stringify.
 */
char *int_stringify(W_Int *i) {
    int len = snprintf(NULL, 0, "%d", *i->value);
    char *str = (char *)malloc(len + 1);
    sprintf(str, "%d", *i->value);
    return str;
}

/**
 * \brief Destroys the given integer.
 * \param i The integer to destroy.
 */
void int_destroy(W_Int *i) {
    if (i->value != NULL) free(i->value);
    free(i);
}

/**
 * \brief Copies the given integer. (malloc)
 * \param i The integer to copy.
 * \return A copy of the integer.
 */
W_Int *int_copy(W_Int *i) {
    W_Int *copy = int_init();
    int_set(copy, *i->value);
    return copy;
}