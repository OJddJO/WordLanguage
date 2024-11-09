#include "parser.h"

/**
 * \brief Parses the given list of words into a parsing tree. (w_malloc)
 * \param tokenized_code The list of list of words to parse.
 * \return A list (code) of list (line) of words, representing the parsed code.
 *  [ [ word, word, word ], [ word, word ], ... ]
 */
list *parse(list *tokenized_code) {
    list *parsed_code = list_init();
    list_element *current_line = tokenized_code->head;
    for (int i = 0; i < tokenized_code->size; i++) {
        list *line = (list *)current_line->value;
        list_element *current_word = line->head;
        list *current_block = shunting_yard(current_word);
        list_append(parsed_code, current_block);
        current_line = current_line->next;
    }
    current_line = tokenized_code->head;
    while (current_line != NULL) {
        list *line = (list *)current_line->value;
        list_destroy_no_free(line);
        current_line = current_line->next;
    }
    list_destroy_no_free(tokenized_code);
    return parsed_code;
}

/**
 * \brief Returns the priority of the given keyword.
 * \param keyword The keyword to parse.
 * \return The priority of the keyword.
 */
int get_priority(char *keyword) {
    int priority = 0;
    for (int i = 0; i < strlen(keyword); i++) {
        if (keyword[i] == '.') {
            priority++;
        } else break;
    }
    char without_dot[strlen(keyword) - priority + 1];
    strncpy(without_dot, keyword + priority, strlen(keyword) - priority);
    without_dot[strlen(keyword) - priority] = '\0';
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
    list *keywords = list_init();
    while (current_word != NULL) {
        W_Word *word = (W_Word *)current_word->value;
        if (word->type == NUMBER || word->type == IDENTIFIER) {
            list_append(post_order, word);
        } else if (word->type == KEYWORD) {
            while (keywords->size > 0) {
                W_Word *top = (W_Word *)keywords->tail->value;
                if (get_priority(top->value) >= get_priority(word->value)) {
                    list_append(post_order, list_pop(keywords));
                } else break;
            }
            list_append(keywords, word);
        } else break;
        word->parsed = true;
        current_word = current_word->next;
    }
    while (keywords->size > 0) {
        list_append(post_order, list_pop(keywords));
    }
    w_free(keywords);
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
        list_element *current_word = line->head;
        for (int k = 0; k < line->size; k++) {
            W_Word *word = (W_Word *)current_word->value;
            printf("    word: %s | type: ", word->value);
            if (word->type == KEYWORD) {
                printf("KEYWORD");
            } else if (word->type == STR) {
                printf("STR");
            } else if (word->type == NUMBER) {
                printf("NUMBER");
            } else if (word->type == IDENTIFIER) {
                printf("IDENTIFIER");
            }
            printf(" | line: %d,\n", word->line);
            current_word = current_word->next;
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
            list_element *current_word = line->head;
            while (current_word != NULL) {
                W_Word *word = (W_Word *)current_word->value;
                if (word != NULL) {
                    w_free(word->value);
                    w_free(word);
                }
                current_word = current_word->next;
            }
            list_destroy_no_free(line);
        }
        current_line = next_line;
    }
    list_destroy_no_free(parsed_code);
}