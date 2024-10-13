#include "w_parser.h"

/**
 * \brief Parses the given list of words into a parsing tree.
 * \param tokenized_code The list of list of words to parse.
 * \return A list of list of tree, representing the parsed code.
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
        printf("current_word: %s, type: %d\n", word->value, word->type);
        if (word->parsed) {
            current_word = current_word->next;
            continue;
        }
        W_Tree *tree = tree_init();
        if (word->type == KEYWORD || word->type == STR || current_word->next == NULL) {
            printf("keyword/str: %s\n", word->value);
            word->parsed = true;
            tree_set(tree, word);
        } else if ((word->type == NUMBER || word->type == IDENTIFIER) && ((W_Word *)current_word->next->value)->type != OPERATOR) {
            printf("number/id and not op: %s\n", word->value);
            printf("next: %s, type: %d\n", ((W_Word *)current_word->next->value)->value, ((W_Word *)current_word->next->value)->type);
            word->parsed = true;
            tree_set(tree, word);
        } else {
            int highest_priority = parser_highest_priority(current_word);
            for (int j = highest_priority; j >= 0; j--) {
                W_List_Element *current_word_copy = current_word;
                while (current_word_copy != NULL) {
                    W_Word *word_copy = (W_Word *)current_word_copy->value;
                    if (word_copy->type == OPERATOR) {
                        int current_priority = get_priority(word_copy->value);
                        if (current_priority == j) {
                            W_Tree *operation = parse_operation(current_word_copy);
                            if (tree->value == NULL) {
                                tree_destroy(tree);
                                tree = operation;
                            } else {
                                if (operation->left == NULL) {
                                    operation->left = tree;
                                    tree = operation;
                                } else if (operation->right == NULL) {
                                    operation->right = tree;
                                    tree = operation;
                                }
                                tree = operation;
                            }
                        }
                    } else if (word_copy->type != NUMBER && word_copy->type != IDENTIFIER) break;
                    current_word_copy = current_word_copy->next;
                }
            }
        }
        printf("tree: %s\n", tree->value->value);
        list_append(parsed_line, tree);
        current_word = current_word->next;
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
    W_Word *word = (W_Word *)current_word->value; //word is an operator
    word->parsed = true;
    tree_set(tree, word); //set the value of the tree to the operator
    W_Word *prev_word = (W_Word *)current_word->prev->value;
    W_Word *next_word = (W_Word *)current_word->next->value;
    //check if the previous and next words are numbers or identifiers
    //if they are, create a new tree with the value of the word
    //and set the left or right child of the current tree to the new tree
    if (prev_word != NULL) if ((prev_word->type == NUMBER || prev_word->type == IDENTIFIER) && !prev_word->parsed) {
        tree->left = tree_init();
        prev_word->parsed = true;
        tree_set(tree->left, prev_word);
    }
    if (next_word != NULL) if ((next_word->type == NUMBER || next_word->type == IDENTIFIER) && !next_word->parsed) {
        tree->right = tree_init();
        next_word->parsed = true;
        tree_set(tree->right, prev_word);
    }
    return tree;
}

/**
 * \brief Prints the given list of parsed code.
 * \param parsed_code The list of parsed code to print.
 * \return void
 */
void print_parsed_code(W_List *parsed_code) { //debug
    W_List_Element *current_line = parsed_code->head;
    for (int i = 0; i < parsed_code->size; i++) {
        printf("{\n");
        W_List *line = (W_List *)current_line->value;
        W_List_Element *current_tree = line->head;
        for (int j = 0; j < line->size; j++) {
            printf("    {\n");
            W_List *post_order = tree_in_order((W_Tree *)current_tree->value);
            W_List_Element *current_word = post_order->head;
            for (int k = 0; k < post_order->size; k++) {
                W_Word *word = (W_Word *)current_word->value;
                printf("        Word: %s | ", word->value);
                if (word->type == KEYWORD) {
                    printf("Type: KEYWORD ");
                } else if (word->type == OPERATOR) {
                    printf("Type: OPERATOR ");
                } else if (word->type == IDENTIFIER) {
                    printf("Type: IDENTIFIER ");
                } else if (word->type == STR) {
                    printf("Type: STRING ");
                } else if (word->type == NUMBER) {
                    printf("Type: NUMBER ");
                }
                printf("| Line: %d, ", word->line);
                current_word = current_word->next;
            }
            printf("\n    }\n");
        }
        printf("}\n");
        current_line = current_line->next;
    }
}