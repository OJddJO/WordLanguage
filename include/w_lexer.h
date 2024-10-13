#ifndef __W_LEXER_H__
#define __W_LEXER_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "w_list.h"

static char *keywords[] = {
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

static char *operators[] = {
    //operations
    "plus", "minus", "time", "div", "mod", "ediv", "power", "sqrt",
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
    bool parsed;
} W_Word;

W_List *word_tokenize(FILE *source);
W_Word_Type word_type(char *value);
void word_destroy(W_List *words);
void word_print(W_List *words);

#endif