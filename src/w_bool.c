#include "w_bool.h"

/**
 * \brief Initializes a new boolean. (w_malloc)
 * \return A pointer to the newly created boolean.
 */
W_Bool *w_bool_init() {
    W_Bool *b = (W_Bool *)w_malloc(sizeof(W_Bool));
    b->type = NULL_TYPE;
    b->value = NULL;
    b->destroy = &w_bool_destroy;
    b->stringify = &w_bool_stringify;
    b->copy = &w_bool_copy;
    b->assign = &w_bool_assign;
    b->set = &w_bool_set;
    b->get = &w_bool_value;
    return b;
}

/**
 * \brief Assigns the value of the given boolean. (w_malloc)
 * \param b The boolean to set the value of.
 * \param value The value to set.
 */
void w_bool_assign(W_Bool *b, char *value) {
    bool *v;
    bool var_value;
    if (strcmp(value, "true") == 0) {
        var_value = true;
    } else if (strcmp(value, "false") == 0) {
        var_value = false;
    }
    if (b->type == NULL_TYPE) {
        v = (bool *)w_malloc(sizeof(bool));
        b->type = BOOL;
    } else v = b->value;
    *v = var_value;
    b->value = v;
}

/**
 * \brief Sets the value of the given boolean. (w_malloc)
 * \param b The boolean to set the value of.
 * \param value The value to set.
 */
void w_bool_set(W_Bool *b, bool value) {
    bool *v;
    if (b->type == NULL_TYPE) {
        v = (bool *)w_malloc(sizeof(bool));
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
bool *w_bool_value(W_Bool *b) {
    return b->value;
}

/**
 * \brief Stringifies the given boolean. (w_malloc)
 * \param b The boolean to stringify.
 * \return The stringified boolean.
 */
char *w_bool_stringify(W_Bool *b) {
    char *str = (char *)w_malloc(6);
    sprintf(str, "%s", *b->value ? "true" : "false");
    return str;
}

/**
 * \brief Destroys the given boolean.
 * \param b The boolean to destroy.
 */
void w_bool_destroy(W_Bool *b) {
    if (b->value != NULL) free(b->value);
    free(b);
}

/**
 * \brief Copies the given boolean. (w_malloc)
 * \param b The boolean to copy.
 * \return A pointer to the newly created boolean.
 */
W_Bool *w_bool_copy(W_Bool *b) {
    W_Bool *copy = w_bool_init();
    w_bool_set(copy, *b->value);
    return copy;
}