#ifndef __W_LEXER_H__
#define __W_LEXER_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "list.h"

extern char *keywords[];
extern char *operators[];

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

list *word_tokenize(FILE *source);
W_Word_Type word_type(char *value);
void lexer_destroy(list *words);
void lexer_print(list *words);
bool is_keyword(char *word);
bool is_operator(char *word);

#endif