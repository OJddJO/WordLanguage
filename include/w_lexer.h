#ifndef __W_LEXER_H__
#define __W_LEXER_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "w_list.h"

static char *keywords[] = {
    //variables
    "int", "float", "str", "bool", "assign", "change", "to", "delete",
    //constructed types
    "list", "create", "append", "get", "set", "search", "index", "remove", "length",
    //IO expressions
    "print", "sep", "end", "ask",
    //control flow
    //conditional
    "if", "elif", "else", "endif",
    //loop
    "for", "from", "to", "step", "endfor", "infloop", "endinf", "break", "continue",
    //functions
    "def", "null", "with", "return", "enddef", "call", "store",
};

static char *operators[] = {
    //operations
    "plus", "minus", "times", "div", "mod", "ediv", "power", "sqrt",
    //comparison
    "equal", "greater", "less", "gequal", "lequal",
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
bool is_keyword(char *word);
bool is_operator(char *word);

#endif