#ifndef __WORD_H__
#define __WORD_H__

#include <stdlib.h>
#include <stdio.h>
#include "lexer.h"
#include "parser.h"
#include "w_stdlib.h"

char *type_keywords[] = {
    "null", "int", "float", "str", "bool", "list",
};

void *execute(list *parsed_code, W_Dict *args, W_Type return_type);

// Utility

void eval_parsed_lines(list_element *parsed_line, W_Dict *variables, list *stack);
char *remove_dot(W_Word *word);
bool is_type_keyword(char *word);
bool is_float(char *str);

#endif