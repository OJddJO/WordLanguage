#ifndef __LEXER_H__
#define __LEXER_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "w_alloc.h"
#include "w_word.h"
#include "list.h"
#include "dict.h"

extern dict *keywords;

list *word_tokenize(FILE *source);
// W_Word_Type word_type(char *value);
void lexer_destroy(list *words);
void lexer_print(list *words);
bool is_keyword(char *word);

#endif