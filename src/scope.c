#include "scope.h"

/**
 * \brief Initialize a new scope.
 * \return A pointer to the new scope.
 */
Scope *init_scope() {
    Scope *scope = (Scope *)malloc(sizeof(Scope));
    scope->parent = NULL;
    scope->vars = w_dict_init();
    return scope;
}

/**
 * \brief Get a variable from the scope.
 * \param scope The scope to get the variable from.
 * \param name The name of the variable.
 * \return The variable.
 */
W_Var *get_var(Scope *scope, char *name) {
    W_Var *var = w_dict_get(scope->vars, name);
    if (var != NULL) {
        return var;
    }
    if (scope->parent != NULL) {
        return get_var(scope->parent, name);
    }
    return NULL;
}

/**
 * \brief Destroy a scope.
 * \param scope The scope to destroy.
 */
void destroy_scope(Scope *scope) {
    w_dict_destroy(scope->vars);
    free(scope);
}