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

extern char *keywords[] = {
    //operations
    "plus", "minus", "times", "div", "mod", "ediv", "power", "root", "factorial", "abs",
    //variables
    "int", "float", "str", "bool", "assign", "delete",
    //constructed types
    "array", "list", "size", "get", "index", "set", "append", "remove", "length",
    //IO expressions
    "print", "sep", "end", "input",
    //Boolean expressions
    "true", "false",
    //comparison
    "equal", "greater", "less", "gEqual", "lEqual",
    //logical
    "and", "or", "not",
    //control flow
    //conditional
    "if", "elif", "else", "endif",
    //loop
    "for", "in", "range", "to", "step", "while", "do", "endloop", "break",
    //functions
    "def", "void", "with", "return", "enddef", "null",
};

typedef enum _w_word_type {
    KEYWORD, 
    IDENTIFIER,
    LITERAL
} W_Word_Type;

typedef struct _w_word {
    W_Word_Type type;
    char *value;
    int line;
} W_Word;

W_List *word_tokenize(FILE *source);
W_Word_Type word_type(char *value);
void word_destroy(W_List *words);
void word_print(W_List *words);

#endif