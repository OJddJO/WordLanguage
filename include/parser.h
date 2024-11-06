#ifndef __W_PARSER_H__
#define __W_PARSER_H__

#include <stdlib.h>
#include <stdio.h>
#include "w_alloc.h"
#include "lexer.h"
#include "w_list.h"

list *parse(list *tokenized_code);
list *parse_line(list *line);
int get_priority(char *operator);
list *shunting_yard(list_element *current_word);
void print_parsed_code(list *parsed_code);
void parser_destroy(list *parsed_code);

#endif