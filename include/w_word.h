#ifndef __W_WORD_H__
#define __W_WORD_H__

#include <stdbool.h>

typedef enum _w_word_type {
    KEYWORD,
    IDENTIFIER,
    LITT_STR,
    NUMBER, 
} W_Word_Type;

typedef struct _w_word {
    W_Word_Type type;
    char *value;
    int line;
    bool parsed;
    bool is_generated;
} W_Word;

#endif