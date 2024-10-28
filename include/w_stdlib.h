#ifndef __W_STDLIB_H__
#define __W_STDLIB_H__

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
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
char *w_get_type_str(W_Var *var);

// Math Operations

void *w_plus(W_Var *a, W_Var *b);
void *w_minus(W_Var *a, W_Var *b);
void *w_time(W_Var *a, W_Var *b);
W_Float *w_div(W_Var *a, W_Var *b);
W_Int *w_mod(W_Var *a, W_Var *b);
W_Int *w_ediv(W_Var *a, W_Var *b);
void *w_power(W_Var *a, W_Var *b);
W_Float *w_sqrt(W_Var *a);

// Logical Operations

W_Bool *w_and(W_Var *a, W_Var *b);
W_Bool *w_or(W_Var *a, W_Var *b);
W_Bool *w_not(W_Var *a);

// Comparison Operations

W_Bool *w_equal(W_Var *a, W_Var *b);
W_Bool *w_greater(W_Var *a, W_Var *b);
W_Bool *w_less(W_Var *a, W_Var *b);
W_Bool *w_gequal(W_Var *a, W_Var *b);
W_Bool *w_lequal(W_Var *a, W_Var *b);

// Variables

void *w_var_init(W_Type type);

#endif