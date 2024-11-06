#ifndef __SCOPE_H__
#define __SCOPE_H__

#include <stdlib.h>
#include "w_alloc.h"
#include "w_type.h"
#include "w_dict.h"

typedef struct _scope {
    struct _scope *parent;
    W_Dict *vars;
} Scope;

Scope *init_scope();
W_Var *get_var(Scope *scope, char *name);
void destroy_scope(Scope *scope);

#endif