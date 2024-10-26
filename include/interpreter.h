#ifndef __WORD_H__
#define __WORD_H__

#include <stdlib.h>
#include <stdio.h>
#include "w_stdlib.h"
#include "w_lexer.h"
#include "w_parser.h"

char *type_keywords[] = {
    "null", "int", "float", "str", "bool", "array", "list",
};

void create_temp_file(char *filename);
void *execute(W_List *parsed_code, W_Dict *args, W_Type return_type);

// Utility

bool is_type_keyword(char *word);
void eval_parsed_lines(W_List_Element *parsed_line, W_Dict *variables, W_List *stack);
bool is_float(char *str);

#endif