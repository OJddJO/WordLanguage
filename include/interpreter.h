#ifndef __WORD_H__
#define __WORD_H__

#include <stdlib.h>
#include <stdio.h>
#include "w_lexer.h"
#include "w_parser.h"
#include "w_stdlib.h"

char *type_keywords[] = {
    "int", "float", "str", "bool", "assign", "delete",
};

int main(int argc, char *argv[]);
void create_temp_file(char *filename);
void *execute(W_List *parsed_code, W_Dict *args);

// Utility

bool is_type_keyword(char *word);

#endif