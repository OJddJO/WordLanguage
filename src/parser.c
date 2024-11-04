#include "parser.h"

/**
 * \brief Parses the given list of words into a parsing tree. (w_malloc)
 * \param tokenized_code The list of list of words to parse.
 * \return A list (code) of list (line) of list (operation) of words, representing the parsed code.
 *  [ [ [ word ] ], [ [ word, word, word, ... ] ], ... ]
 */
list *parse(list *tokenized_code) {
    list *parsed_code = list_init();
    list_element *current_line = tokenized_code->head;
    for (int i = 0; i < tokenized_code->size; i++) {
        list *line = (list *)current_line->value;
        list *current_block = parse_line(line);
        list_append(parsed_code, current_block);
        current_line = current_line->next;
    }
    list_destroy(tokenized_code);
    return parsed_code;
}

/**
 * \brief Parses the given list of words into a parsing list. (w_malloc)
 * \param line The list of words to parse.
 * \return A list of lists of words, representing the parsed line.
 */
list *parse_line(list *line) {
    list *current_block = list_init();
    list_element *current_word = line->head;
    list *parsed_words;
    while (current_word != NULL) {
        W_Word *word = (W_Word *)current_word->value;
        if (word->parsed) {
            current_word = current_word->next;
            continue;
        }
        if (word->type == KEYWORD || word->type == STR || current_word->next == NULL) {
            parsed_words = list_init();
            word->parsed = true;
            list_append(parsed_words, word);
        } else if ((word->type == NUMBER || word->type == IDENTIFIER) && ((W_Word *)current_word->next->value)->type != OPERATOR) {
            // printf("number/id and not op: %s\n", word->value); //debug
            // printf("next: %s, type: %d\n", ((W_Word *)current_word->next->value)->value, ((W_Word *)current_word->next->value)->type); //debug
            parsed_words = list_init();
            word->parsed = true;
            list_append(parsed_words, word);
        } else {
            parsed_words = shunting_yard(current_word);
        }
        list_append(current_block, parsed_words);
        current_word = current_word->next;
    }
    return current_block;
}

/**
 * \brief Returns the priority of the given operator.
 * \param operator The operator to parse.
 * \return The priority of the operator.
 */
int get_priority(char *operator) {
    int priority = 0;
    for (int i = 0; i < strlen(operator); i++) {
        if (operator[i] == '.') {
            priority++;
        } else break;
    }
    char without_dot[strlen(operator) - priority + 1];
    strncpy(without_dot, operator + priority, strlen(operator) - priority);
    without_dot[strlen(operator) - priority] = '\0';
    if (strcmp(without_dot, "power") == 0 || strcmp(without_dot, "sqrt") == 0) {
        priority = priority + 2;
    } else if (strcmp(without_dot, "time") == 0 || strcmp(without_dot, "div") == 0 || strcmp(without_dot, "mod") == 0 || strcmp(without_dot, "ediv") == 0) {
        priority = priority + 1;
    }
    // printf("operator: %s, without_dot: %s, priority: %d\n", operator, without_dot, priority); //debug
    return priority;
}

/**
 * \brief Returns a list representing the given operation in post-order. (w_malloc)
 * \param current_word The current operation to parse.
 * \return A list representing the operation.
 */
list *shunting_yard(list_element *current_word) {
    list *post_order = list_init();
    list *operators = list_init();
    while (current_word != NULL) {
        W_Word *word = (W_Word *)current_word->value;
        if (word->type == NUMBER || word->type == IDENTIFIER) {
            list_append(post_order, word);
        } else if (word->type == OPERATOR) {
            while (operators->size > 0) {
                W_Word *top = (W_Word *)operators->tail->value;
                if (get_priority(top->value) >= get_priority(word->value)) {
                    list_append(post_order, list_pop(operators));
                } else break;
            }
            list_append(operators, word);
        } else break;
        word->parsed = true;
        current_word = current_word->next;
    }
    while (operators->size > 0) {
        list_append(post_order, list_pop(operators));
    }
    free(operators);
    return post_order;
}

/**
 * \brief Prints the given list of parsed code.
 * \param parsed_code The list of parsed code to print.
 */
void print_parsed_code(list *parsed_code) { //debug
    list_element *current_line = parsed_code->head;
    for (int i = 0; i < parsed_code->size; i++) {
        printf("[");
        list *line = (list *)current_line->value;
        printf("line size: %d\n", line->size);
        list_element *current_block = line->head;
        for (int j = 0; j < line->size; j++) {
            list *parsed_words = (list *)current_block->value;
            printf("    block size: %d    [\n", parsed_words->size);
            list_element *current_word = parsed_words->head;
            for (int k = 0; k < parsed_words->size; k++) {
                W_Word *word = (W_Word *)current_word->value;
                printf("        word: %s | type: ", word->value);
                if (word->type == KEYWORD) {
                    printf("KEYWORD");
                } else if (word->type == STR) {
                    printf("STR");
                } else if (word->type == NUMBER) {
                    printf("NUMBER");
                } else if (word->type == IDENTIFIER) {
                    printf("IDENTIFIER");
                } else if (word->type == OPERATOR) {
                    printf("OPERATOR");
                }
                printf(" | line: %d,\n", word->line);
                current_word = current_word->next;
            }
            printf("    ],\n");
            current_block = current_block->next;
        }
        printf("],\n");
        current_line = current_line->next;
    }
}

/**
 * \brief Destroys the given list of parsed code.
 * \param parsed_code The list of parsed code to destroy.
 */
void parser_destroy(list *parsed_code) {
    if (parsed_code == NULL) {
        return; // Handle null input
    }

    list_element *current_line = parsed_code->head;
    while (current_line != NULL) {
        list_element *next_line = current_line->next;
        list *line = (list *)current_line->value;
        if (line != NULL) {
            list_element *current_block = line->head;
            while (current_block != NULL) {
                list_element *next_block = current_block->next;
                list *parsed_words = (list *)current_block->value;
                if (parsed_words != NULL) {
                    list_destroy(parsed_words);
                }
                current_block = next_block;
            }
            list_destroy(line);
        }
        current_line = next_line;
    }
    list_destroy(parsed_code);
}