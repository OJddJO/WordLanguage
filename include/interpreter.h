#ifndef __WORD_H__
#define __WORD_H__

#include <stdlib.h>
#include <stdio.h>
#include "w_type.h"
#include "w_int.h"
#include "w_float.h"
#include "w_str.h"
#include "w_bool.h"
#include "w_array.h"
#include "w_list.h"
#include "w_dict.h"

char *keywords[] = {
    //variables
    "int", "float", "str", "bool", "assign", "delete",
    //constructed types
    "array", "list", "size", "get", "index", "set", "append", "remove", "length",
    //IO expressions
    "print", "sep", "end", "input",
    //Boolean expressions
    "true", "false",
    //control flow
    //conditional
    "if", "elif", "else", "endif",
    //loop
    "for", "from", "to", "step", "while", "do", "endloop", "break",
    //functions
    "def", "void", "with", "return", "enddef", "call",
};

char *operators[] = {
    //operations
    "plus", "minus", "times", "div", "mod", "ediv", "power", "root", "factorial", "abs",
    //comparison
    "equal", "greater", "less", "gEqual", "lEqual",
    //logical
    "and", "or", "not",
};

typedef enum _w_word_type {
    KEYWORD,
    OPERATOR,
    IDENTIFIER,
    STR,
    NUMBER, 
} W_Word_Type;

typedef struct _w_word {
    W_Word_Type type;
    char *value;
    int line;
} W_Word;

void create_temp_file(char *filename);
W_List *word_tokenize(FILE *source);
W_Word_Type word_type(char *value);
void word_destroy(W_List *words);
void word_print(W_List *words);

#endif