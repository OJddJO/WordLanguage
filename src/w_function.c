#include "w_function.h"

/**
 * \brief Initializes a new function.
 * \param return_type The return type of the function.
 * \param args The arguments of the function.
 * \param parsed_code The parsed code of the function.
 * \return A new function.
 */
W_Func *func_init() {
    W_Func *f = (W_Func *)malloc(sizeof(W_Func));
    f->type = FUNCTION;
    f->return_type = NULL_TYPE;
    f->args = dict_init();
    f->parsed_code = list_init();
    f->destroy = &func_destroy;
    return f;
}

/**
 * \brief Destroys the given function.
 * \param f The function to destroy.
 */
void func_destroy(W_Func *f) {
    dict_destroy(f->args);
    parser_destroy(f->parsed_code);
    free(f);
}