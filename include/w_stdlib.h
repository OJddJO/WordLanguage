#ifndef __W_STDLIB_H__
#define __W_STDLIB_H__

#include "keywords.h" // include stdlib, stdio, stdbool, w_alloc, list, dict, scope and w_word
#include "w_type.h"
#include "w_bool.h"
#include "w_dict.h"
#include "w_float.h"
#include "w_int.h"
#include "w_list.h"
#include "w_str.h"
#include "w_function.h"

#ifndef DEBUG
#define DEBUG false
#endif

// Execution

void *execute(list *parsed_code, Scope *scope, W_Type return_type, bool destroy_scope_on_exit);

// Math Operations

W_Word *kw_plus(Scope *scope, list *args, int line, list_element **current_line, W_Type return_type, void *return_value);
W_Word *kw_minus(Scope *scope, list *args, int line, list_element **current_line, W_Type return_type, void *return_value);
W_Word *kw_time(Scope *scope, list *args, int line, list_element **current_line, W_Type return_type, void *return_value);
W_Word *kw_div(Scope *scope, list *args, int line, list_element **current_line, W_Type return_type, void *return_value);
W_Word *kw_mod(Scope *scope, list *args, int line, list_element **current_line, W_Type return_type, void *return_value);
W_Word *kw_ediv(Scope *scope, list *args, int line, list_element **current_line, W_Type return_type, void *return_value);
W_Word *kw_power(Scope *scope, list *args, int line, list_element **current_line, W_Type return_type, void *return_value);
W_Word *kw_sqrt(Scope *scope, list *args, int line, list_element **current_line, W_Type return_type, void *return_value);

// Logical Operations

W_Word *kw_and(Scope *scope, list *args, int line, list_element **current_line, W_Type return_type, void *return_value);
W_Word *kw_or(Scope *scope, list *args, int line, list_element **current_line, W_Type return_type, void *return_value);
W_Word *kw_not(Scope *scope, list *args, int line, list_element **current_line, W_Type return_type, void *return_value);

// Comparison Operations

W_Word *kw_equal(Scope *scope, list *args, int line, list_element **current_line, W_Type return_type, void *return_value);
W_Word *kw_greater(Scope *scope, list *args, int line, list_element **current_line, W_Type return_type, void *return_value);
W_Word *kw_less(Scope *scope, list *args, int line, list_element **current_line, W_Type return_type, void *return_value);
W_Word *kw_gequal(Scope *scope, list *args, int line, list_element **current_line, W_Type return_type, void *return_value);
W_Word *kw_lequal(Scope *scope, list *args, int line, list_element **current_line, W_Type return_type, void *return_value);

// Variable

W_Word *kw_int(Scope *scope, list *args, int line, list_element **current_line, W_Type return_type, void *return_value);
W_Word *kw_float(Scope *scope, list *args, int line, list_element **current_line, W_Type return_type, void *return_value);
W_Word *kw_str(Scope *scope, list *args, int line, list_element **current_line, W_Type return_type, void *return_value);
W_Word *kw_bool(Scope *scope, list *args, int line, list_element **current_line, W_Type return_type, void *return_value);
W_Word *kw_list(Scope *scope, list *args, int line, list_element **current_line, W_Type return_type, void *return_value);

W_Word *kw_assign(Scope *scope, list *args, int line, list_element **current_line, W_Type return_type, void *return_value);
W_Word *kw_delete(Scope *scope, list *args, int line, list_element **current_line, W_Type return_type, void *return_value);

//IO

W_Word *kw_print(Scope *scope, list *args, int line, list_element **current_line, W_Type return_type, void *return_value);
W_Word *kw_ask(Scope *scope, list *args, int line, list_element **current_line, W_Type return_type, void *return_value);

//Function

W_Word *kw_def(Scope *scope, list *args, int line, list_element **current_line, W_Type return_type, void *return_value);
W_Word *kw_call(Scope *scope, list *args, int line, list_element **current_line, W_Type return_type, void *return_value);
W_Word *kw_return(Scope *scope, list *args, int line, list_element **current_line, W_Type return_type, void *return_value);

#endif