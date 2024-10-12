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
    word_destroy(tokenized_code);
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
    int highest_priority = 0;
    int highest_priority_index = -1;
    int current_priority;
    for (int i = 0; i < line->size; i++) { //get the word with the highest priority
        W_Word *word = (W_Word *)current_word->value;
        if (word->type == OPERATOR) {
            current_priority = 0;
            for (int n = 0; n < strlen(word->value); n++) {
                if (word->value[n] == '.') {
                    current_priority++;
                }
            }
            if (current_priority > highest_priority) {
                highest_priority = current_priority;
                highest_priority_index = i;
            }
        }
        current_word = current_word->next;
    }
    return parsed_line;
}

/**
 * \brief Returns a tree representing the given operation.
 * \param current_word The current operation to parse.
 * \return A tree representing the operation.
 */
W_Tree *parse_operation(W_List_Element *current_word) {
    W_Tree *tree = tree_init();
    while (current_word != NULL) {
        W_Word *word = (W_Word *)current_word->value; //word is an operator
        tree_set(tree, word); //set the value of the tree to the operator
        W_Word *prev_word = (W_Word *)current_word->prev->value;
        W_Word *next_word = (W_Word *)current_word->next->value;
        //check if the previous and next words are numbers or identifiers
        //if they are, create a new tree with the value of the word
        //and set the left or right child of the current tree to the new tree
        if (prev_word->type == NUMBER || prev_word->type == IDENTIFIER) {
            tree->left = tree_init();
            tree_set(tree->left, prev_word->value);
            prev_word->type = -1;
        }
        if (next_word->type == NUMBER || next_word->type == IDENTIFIER) {
            tree->right = tree_init();
            tree_set(tree->right, next_word->value);
            next_word->type = -1;
        }
    }
    return tree;
}