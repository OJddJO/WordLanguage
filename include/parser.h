#ifndef __PARSER_H__
#define __PARSER_H__

#include <stdlib.h>
#include <stdio.h>
#include "w_alloc.h"
#include "keywords.h"
#include "w_word.h"
#include "list.h"

extern dict *keywords;

list *parse(list *tokenized_code);
void print_parsed_code(list *parsed_code);
void parser_destroy(list *parsed_code);

#endif