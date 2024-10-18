#include "w_function.h"

/**
 * \brief Initializes a new function. (malloc)
 * \return A new function.
 */
W_Func *func_init() {
    W_Func *f = (W_Func *)malloc(sizeof(W_Func));
    f->type = FUNCTION;
    f->return_type = NULL_TYPE;
    f->args = dict_init();
    f->parsed_code = list_init();
    f->destroy = &func_destroy;
    f->print = &func_print;
    return f;
}

/**
 * \brief Destroys the given function.
 * \param f The function to destroy.
 */
void func_destroy(W_Func *f) {
    list_destroy_any(f->args->keys);
    list_destroy_any(f->args->values);
    free(f->args);
    free(f);
}

/**
 * \brief Prints the given function.
 * \param f The function to print.
 */
void func_print(W_Func *f) {
    printf("function at %p", f);
}