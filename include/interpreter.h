#ifndef __WORD_H__
#define __WORD_H__

#include <stdlib.h>
#include <stdio.h>
#include "w_stdlib.h"
#include "w_lexer.h"
#include "w_parser.h"

char *type_keywords[] = {
    "null", "int", "float", "str", "bool", "list",
};

void *execute(W_List *parsed_code, W_Dict *args, W_Type return_type);

// Utility

void eval_parsed_lines(W_List_Element *parsed_line, W_Dict *variables, W_List *stack);
void remove_dot(W_Word *word);
bool is_type_keyword(char *word);
bool is_float(char *str);

#endif