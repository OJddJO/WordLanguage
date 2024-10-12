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
    for (int i = 0; i < line->size; i++) {
        W_Word *word = (W_Word *)current_word->value;
        if ((word->type == IDENTIFIER || word->type == NUMBER) && current_word->next != NULL) {
            if (((W_Word *)current_word->next->value)->type == OPERATOR) {
                int highest_priority = parser_highest_priority(current_word);
                W_Tree *tree;
                for (int j = highest_priority; j >= 0; j--) {
                    W_List_Element *current_word_copy = current_word;
                    while (current_word_copy != NULL) {
                        W_Word *word_copy = (W_Word *)current_word_copy->value;
                        if (word_copy->type == OPERATOR) {
                            int current_priority = 0;
                            current_priority = get_priority(word_copy->value);
                            if (current_priority == j) {
                                W_Tree *child = parse_operation(current_word_copy);
                                if (tree == NULL) {
                                    tree = child;
                                } else {
                                    W_Tree *new_tree = tree_init();
                                    new_tree->left = tree;
                                    new_tree->right = child;
                                    tree = new_tree;
                                }
                            }
                        } else if (word_copy->type != NUMBER && word_copy->type != IDENTIFIER) break;
                        current_word_copy = current_word_copy->next;
                    }
                }
                list_append(parsed_line, tree);
            }
        } else {
            W_Tree *tree = tree_init();
            tree_set(tree, word->value);
            list_append(parsed_line, tree);
        }
    }
    return parsed_line;
}

/**
 * \brief Returns the highest priority of the given operation.
 * \param current_word The current operation to parse.
 * \return The highest priority of the operation.
 */
int parser_highest_priority(W_List_Element *current_word) {
    int highest_priority = 0;
    while (current_word != NULL) { //get the word with the highest priority
        W_Word *word = (W_Word *)current_word->value;
        if (word->type == OPERATOR) {
            int current_priority = 0;
            current_priority = get_priority(word->value);
            if (current_priority > highest_priority) {
                highest_priority = current_priority;
            }
        }
        else if (word->type != NUMBER) break;
        current_word = current_word->next;
    }
    return highest_priority;
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
    return priority;
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
            tree_set(tree->left, prev_word);
            prev_word->type = -1;
        }
        if (next_word->type == NUMBER || next_word->type == IDENTIFIER) {
            tree->right = tree_init();
            tree_set(tree->right, prev_word);
            next_word->type = -1;
        }
    }
    return tree;
}