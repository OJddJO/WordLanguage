#ifndef __W_STDLIB_H__
#define __W_STDLIB_H__

#include <stdlib.h>
#include <stdio.h>
#include "w_array.h"
#include "w_bool.h"
#include "w_dict.h"
#include "w_float.h"
#include "w_int.h"
#include "w_list.h"
#include "w_str.h"
#include "w_type.h"

// Global

char *w_get_type(W_Var *var);

// Operations

void *w_plus(void *a, void *b);
void *w_minus(void *a, void *b);
void *w_time(void *a, void *b);
void *w_div(void *a, void *b);
void *w_mod(void *a, void *b);
void *w_ediv(void *a, void *b);
void *w_power(void *a, void *b);
void *w_sqrt(void *a);


#endif