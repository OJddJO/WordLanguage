#ifndef __KEYWORDS_H__
#define __KEYWORDS_H__

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "w_alloc.h"
#include "list.h"
#include "dict.h"
#include "scope.h"
#include "w_word.h"

typedef struct _keyword {
    W_Word *(*func)(Scope*, list*, int, list_element**, W_Type, void *); //function pointer, takes scope, args, line, current_line
    int argc; //number of arguments, -1 for unlimited
    int priority; //priority of the keyword
} keyword;

extern dict *keywords;
extern dict *reserved_words;

void add_keyword(char *name, W_Word *(*func)(Scope*, list*, int, list_element**, W_Type, void *), int argc, int priority);
void add_reserved_word(char *name, int priority);

#endif