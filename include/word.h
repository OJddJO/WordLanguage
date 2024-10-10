#ifndef __WORD_H__
#define __WORD_H__

#include <stdlib.h>
#include <stdio.h>
#include "w_type.h"
#include "w_int.h"
#include "w_float.h"
#include "w_str.h"
#include "w_list.h"
#include "w_array.h"

typedef enum _w_word_type {
    KEYWORD, 
    IDENTIFIER,
    LITERAL,
    OPERATOR
} W_Word_Type;

typedef struct _w_word {
    W_Word_Type type;
    int line;
    int start;
    int end;
} W_Word;

W_List *word_tokenize(char *source);
void word_destroy(W_List *words);

#endif