#include "w_parser.h"

/**
 * \brief Parses the given list of words into a parsing tree. (malloc)
 * \param tokenized_code The list of list of words to parse.
 * \return A list (code) of list (line) of list (operation) of words, representing the parsed code.
 *  [ [ [ word ] ], [ [ word, word, word, ... ] ], ... ]
 */
W_List *parse(W_List *tokenized_code) {
    W_List *parsed_code = list_init();
    W_List_Element *current_line = tokenized_code->head;
    for (int i = 0; i < tokenized_code->size; i++) {
        W_List *line = (W_List *)current_line->value;
        W_List *parsed_line = parse_line(line);
        list_append(parsed_code, parsed_line);
        current_line = current_line->next;
    }
    list_destroy(tokenized_code);
    return parsed_code;
}

/**
 * \brief Parses the given list of words into a parsing list. (malloc)
 * \param line The list of words to parse.
 * \return A list of lists of words, representing the parsed line.
 */
W_List *parse_line(W_List *line) {
    W_List *parsed_line = list_init();
    W_List_Element *current_word = line->head;
    W_List *parsed_words;
    for (int i = 0; i < line->size; i++) {
        W_Word *word = (W_Word *)current_word->value;
        // printf("current_word: %s, type: %d\n", word->value, word->type);
        if (word->parsed) {
            current_word = current_word->next;
            continue;
        }
        if (word->type == KEYWORD || word->type == STR || current_word->next == NULL) {
            // printf("keyword/str: %s\n", word->value); //debug
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
        list_append(parsed_line, parsed_words);
        current_word = current_word->next;
    }
    return parsed_line;
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
 * \brief Returns a list representing the given operation in post-order. (malloc)
 * \param current_word The current operation to parse.
 * \return A list representing the operation.
 */
W_List *shunting_yard(W_List_Element *current_word) {
    W_List *post_order = list_init();
    W_List *operators = list_init();
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
void print_parsed_code(W_List *parsed_code) { //debug
    W_List_Element *current_line = parsed_code->head;
    for (int i = 0; i < parsed_code->size; i++) {
        printf("{\n");
        W_List *line = (W_List *)current_line->value;
        W_List_Element *parsed_line = line->head;
        for (int j = 0; j < line->size; j++) {
            W_List *parsed_words = (W_List *)parsed_line->value;
            printf("    {\n");
            W_List_Element *current_word = parsed_words->head;
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
                printf(" | line: %d\n", word->line);
                current_word = current_word->next;
            }
            printf("    },\n");
            parsed_line = parsed_line->next;
        }
        printf("},\n");
        current_line = current_line->next;
    }
}

/**
 * \brief Destroys the given list of parsed code.
 * \param parsed_code The list of parsed code to destroy.
 */
void parser_destroy(W_List *parsed_code) {
    W_List_Element *current_line = parsed_code->head;
    while (current_line != NULL) {
        W_List_Element *next_line = current_line->next;
        W_List *line = (W_List *)current_line->value;
        W_List_Element *parsed_line = line->head;
        while (parsed_line != NULL) {
            W_List_Element *next_parsed_line = parsed_line->next;
            W_List *parsed_words = (W_List *)parsed_line->value;
            list_destroy(parsed_words);
            parsed_line = next_parsed_line;
        }
        current_line = next_line;
    }
    list_destroy(parsed_code);
}