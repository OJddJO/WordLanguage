#ifndef __W_STDLIB_H__
#define __W_STDLIB_H__

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "w_array.h"
#include "w_bool.h"
#include "w_dict.h"
#include "w_float.h"
#include "w_int.h"
#include "w_list.h"
#include "w_str.h"
#include "w_type.h"
#include "w_function.h"

// Global

W_Type w_get_type(char *str);
char *w_get_type_str(void *var);

// Operations

void *w_plus(void *a, void *b);
void *w_minus(void *a, void *b);
void *w_time(void *a, void *b);
W_Float *w_div(void *a, void *b);
W_Int *w_mod(void *a, void *b);
W_Int *w_ediv(void *a, void *b);
void *w_power(void *a, void *b);
W_Float *w_sqrt(void *a);

// Variables

void *w_var_init(W_Type type);
void w_var_assign(W_Type type, void *var, void *value);
void *w_var_get(void *var);
void *w_var_delete(void *var);

#endif