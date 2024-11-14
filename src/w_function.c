#include "w_function.h"

/**
 * \brief Initializes a new function. (w_malloc)
 * \return A new function.
 */
W_Func *w_func_init() {
    W_Func *f = (W_Func *)w_malloc(sizeof(W_Func));
    f->type = FUNCTION;
    f->destroy = &w_func_destroy;
    f->stringify = &w_func_stringify;
    f->copy = &w_func_copy;
    f->parent_scope = NULL;
    // f->return_type = NULL_TYPE;
    f->args = list_init();
    f->parsed_code = list_init();
    return f;
}

/**
 * \brief Destroys the given function.
 * \param f The function to destroy.
 */
void w_func_destroy(W_Func *f) {
    list_destroy(f->args);
    list_destroy_no_free(f->parsed_code);
    w_free(f);
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
    // copy->return_type = f->return_type;
    copy->parent_scope = f->parent_scope;
    list *args_copy = list_init();
    list_element *current_arg = f->args->head;
    while (current_arg != NULL) {
        char *arg_name = (char *)current_arg->value;
        char *arg_name_copy = (char *)w_malloc(strlen(arg_name) + 1);
        strcpy(arg_name_copy, arg_name);
        list_append(args_copy, arg_name_copy);
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