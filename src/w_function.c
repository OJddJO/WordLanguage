#include "w_function.h"

/**
 * \brief Initializes a new function. (w_malloc)
 * \return A new function.
 */
W_Func *w_func_init() {
    W_Func *f = (W_Func *)w_malloc(sizeof(W_Func));
    f->type = FUNCTION;
    f->return_type = NULL_TYPE;
    f->args = dict_init();
    f->parsed_code = list_init();
    f->destroy = &w_func_destroy;
    f->stringify = &w_func_stringify;
    f->copy = &w_func_copy;
    return f;
}

/**
 * \brief Destroys the given function.
 * \param f The function to destroy.
 */
void w_func_destroy(W_Func *f) {
    dict_destroy(f->args);
    free(f);
}

/**
 * \brief Converts the given function to a string. (w_malloc)
 * \param f The function to convert.
 * \return The string representation of the function.
 */
char *w_func_stringify(W_Func *f) {
    char *str = (char *)w_malloc(sizeof(void *)*2 + 13);
    sprintf(str, "function at %p", f);
    return str;
}

/**
 * \brief Copies the given function. (w_malloc)
 * \param f The function to copy.
 * \return A copy of the function.
 */
W_Func *w_func_copy(W_Func *f) {
    W_Func *copy = w_func_init();
    copy->return_type = f->return_type;
    dict *args_copy = dict_init();
    list_element *current_key = f->args->keys->head;
    list_element *current_value = f->args->values->head;
    for (int i = 0; i < f->args->keys->size; i++) {
        char *key = (char *)current_key->value;
        W_Type *value = current_value->value;
        W_Type *value_copy = (W_Type *)w_malloc(sizeof(W_Type));
        *value_copy = *value;
        dict_set(args_copy, key, value_copy);
        current_key = current_key->next;
        current_value = current_value->next;
    }
    copy->args = args_copy;
    list *parsed_code_copy = list_init();
    list_element *current_line = f->parsed_code->head;
    for (int i = 0; i < f->parsed_code->size; i++) {
        list_append(parsed_code_copy, current_line->value);
        current_line = current_line->next;
    }
    copy->parsed_code = parsed_code_copy;
    return copy;
}