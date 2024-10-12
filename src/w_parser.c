#include "w_parser.h"

/**
 * \brief Parses the given list of words into a parsing tree.
 * \param tokenized_code The list of list of words to parse.
 * \return A list of list of tree, representing the parsed code.
 */
W_List *parse(W_List *tokenized_code) {
    W_List *parsed_code = list_init();
    W_List_Element *current_line = tokenized_code->head;
    while (current_line != NULL) {
        W_List *line = (W_List *)current_line->value;
        W_List *parsed_line = parse_line(line);
        list_append(parsed_code, parsed_line);
        current_line = current_line->next;
    }
    return parsed_code;
}

/**
 * \brief Parses the given list of words into a parsing tree.
 * \param line The list of words to parse.
 * \return A list of tree, representing the parsed line.
 */
W_List *parse_line(W_List *line) {
    W_List *parsed_line = list_init();
    W_List_Element *current_word = line->head;
    while (current_word != NULL) {
    }
    return parsed_line;
}