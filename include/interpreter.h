#ifndef __WORD_H__
#define __WORD_H__

#include <stdlib.h>
#include <stdio.h>
#include "w_alloc.h"
#include "lexer.h"
#include "parser.h"
#include "scope.h"
#include "w_stdlib.h"

char *type_keywords[] = {
    "null", "int", "float", "str", "bool", "list",
};

void *execute(list *parsed_code, Scope *current_scope, W_Type return_type, bool destroy_vars_on_exit);

// Utility

void eval_parsed_lines(list_element *parsed_line, Scope *scope, list *stack);
void destroy_stack(list *stack);
char *remove_dot(W_Word *word);
bool is_type_keyword(char *word);
bool is_float(char *str);

#endif