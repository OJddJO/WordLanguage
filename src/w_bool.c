#include "w_bool.h"

/**
 * \brief Initializes a new boolean. (malloc)
 * \return A pointer to the newly created boolean.
 */
W_Bool *bool_init() {
    W_Bool *b = (W_Bool *)malloc(sizeof(W_Bool));
    b->type = NULL_TYPE;
    b->value = NULL;
    b->destroy = &bool_destroy;
    b->print = &bool_print;
    b->copy = &bool_copy;
    b->assign = &bool_assign;
    b->set = &bool_set;
    b->get = &bool_value;
    return b;
}

/**
 * \brief Assigns the value of the given boolean. (malloc)
 * \param b The boolean to set the value of.
 * \param value The value to set.
 */
void bool_assign(W_Bool *b, char *value) {
    bool *v;
    bool var_value;
    if (strcmp(value, "true") == 0) {
        var_value = true;
    } else if (strcmp(value, "false") == 0) {
        var_value = false;
    }
    if (b->type == NULL_TYPE) {
        v = (bool *)malloc(sizeof(bool));
        b->type = BOOL;
    } else v = b->value;
    *v = var_value;
    b->value = v;
}

/**
 * \brief Sets the value of the given boolean. (malloc)
 * \param b The boolean to set the value of.
 * \param value The value to set.
 */
void bool_set(W_Bool *b, bool value) {
    bool *v;
    if (b->type == NULL_TYPE) {
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
bool *bool_value(W_Bool *b) {
    return b->value;
}

/**
 * \brief Prints the given boolean.
 * \param b The boolean to print.
 */
void bool_print(W_Bool *b) {
    printf("%s", *b->value ? "true" : "false");
}

/**
 * \brief Destroys the given boolean.
 * \param b The boolean to destroy.
 */
void bool_destroy(W_Bool *b) {
    if (b->value != NULL) free(b->value);
    free(b);
}

/**
 * \brief Copies the given boolean. (malloc)
 * \param b The boolean to copy.
 * \return A pointer to the newly created boolean.
 */
W_Bool *bool_copy(W_Bool *b) {
    W_Bool *copy = bool_init();
    bool_set(copy, *b->value);
    return copy;
}