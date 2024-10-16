#include "interpreter.h"

int main(int argc, char *argv[]) {
    bool debug = true; //debug

    if (argc < 2 && !debug) {
        printf("Usage: word.exe <path>\n", argv[0]);
        return 1;
    }


    if (debug) create_temp_file("test.w"); //debug
    else create_temp_file(argv[1]);

    FILE *temp = fopen("exec.tmp", "r");
    if (temp == NULL) {
        printf("Error: Could not create temp file\n");
        return 1;
    }

    W_List *lexed_code = word_tokenize(temp);
    if (fclose(temp) != 0) {
        printf("Error: Could not close temp file\n");
    }
    if (debug) word_print(lexed_code); //debug

    W_List *parsed_code = parse(lexed_code);
    if (debug) print_parsed_code(parsed_code); //debug
    execute(parsed_code, dict_init(), NULL_TYPE);

    if (remove("exec.tmp") != 0) {
        printf("Error: Could not delete temp file\n");
        return 1;
    }
    return 0;
}

// int main(int argc, char *argv[]) {
//     //testing
//     return 0;
// }

/**
 * \brief Creates a temporary file from the given file.
 * \param filename The file to create a temporary file from.
 */
void create_temp_file(char *filename) {
    FILE *source = fopen(filename, "r");
    if (source == NULL) printf("Error: Could not open file: %s\n", filename);
    FILE *temp = fopen("exec.tmp", "w");
    if (temp == NULL) printf("Error: Could not open temp file\n");
    char c ;
    while ((c = fgetc(source)) != EOF) {
        fputc(c, temp);
    }
    fclose(source);
    fclose(temp);
}

/**
 * \brief Executes the parsed code.
 * \param parsed_code The parsed code to execute.
 * \param args The arguments to pass to the code.
 * \return The result of the execution
 */
void *execute(W_List *parsed_code, W_Dict *args, W_Type *return_type) {
    W_Dict *variables = dict_init();
    void *result = NULL;

    W_List *args_keys = dict_keys(args);
    W_List_Element *current_key = args_keys->head;
    for (int i = 0; i < args_keys->size; i++) { //add arguments to variables
        dict_set(variables, current_key->value, dict_get(args, current_key->value));
        current_key = current_key->next;
    }
    free(args->keys);
    free(args->values);
    free(args);

    W_List_Element *current_line = parsed_code->head;
    while (current_line != NULL) {
        W_List *line = (W_List *)current_line->value;
        W_List_Element *parsed_line = line->head;

        W_List *stack = list_init(); //the stack of words to evaluate
        eval_parsed_lines(parsed_line, variables, stack);

        W_List_Element *current_word = stack->head;
        W_Word *word = (W_Word *)current_word->value;

        if (strcmp(word->value, "def") == 0) { //function definition
            W_Func *f = func_init(); //create function
            W_Dict *args = dict_init(); //create arguments dictionary
            f->type = FUNCTION;
            f->args = args;

            //set return type
            current_word = current_word->next;
            word = current_word->value;
            bool is_type = is_type_keyword(word->value);
            if (!is_type) {
                printf("Error: Expected type keyword after 'def', line: %d\n", word->line);
                exit(1);
            }
            if (strcmp(word->value, "void") == 0) {
                f->return_type = NULL_TYPE;
            } else {
                f->return_type = w_get_type(word->value);
            }

            //set function name
            current_word = current_word->next;
            word = current_word->value;
            char *name = word->value;

            //set arguments
            current_word = current_word->next;
            word = current_word->value;
            if (strcmp(word->value, "with") == 0) {
                while (strcmp(word->value, "do") != 0) {
                    current_word = current_word->next;
                    word = current_word->value;
                    if (strcmp(word->value, "do") == 0) break;
                    if (!is_type_keyword(word->value)) {
                        printf("Error: Expected type keyword in function arguments, line: %d\n", word->line);
                        exit(1);
                    } else {
                        W_Type *arg_type = (W_Type *)malloc(sizeof(W_Type));
                        *arg_type = w_get_type(word->value);
                        current_word = current_word->next;
                        word = current_word->value;
                        dict_set(args, word->value, arg_type);
                    }
                }
            } else if (strcmp(word->value, "do") != 0) {
                printf("Error: Expected keyword 'with' or 'do' after function name, line: %d\n", word->line);
                exit(1);
            }
            current_word = current_word->next;
            if (current_word != NULL) {
                printf("Error: Expected end of line after 'do', line: %d\n", word->line);
                exit(1);
            }

            //get function lines
            int fn_line = word->line;
            W_List *function_lines = list_init();
            bool end = false;
            current_line = current_line->next;
            while (current_line != NULL) {
                line = (W_List *)current_line->value;
                parsed_line = line->head;
                current_word = ((W_List *)parsed_line->value)->head;
                word = (W_Word *)current_word->value;
                if (strcmp(word->value, "enddef") == 0) {
                    end = true;
                    break;
                }
                list_append(function_lines, line);
                current_line = current_line->next;
            }
            if (!end) {
                printf("Error: Expected keyword 'enddef' at end of the function definition, line: %d\n", fn_line);
                exit(1);
            }
            f->parsed_code = function_lines;

            //add function to variables
            dict_set(variables, name, f);

        } else if (strcmp(word->value, "return") == 0) { //return statement
            current_word = current_word->next;
            word = current_word->value;
            if (*return_type == NULL_TYPE) {
                printf("Error: Cannot return value from null function, line: %d\n", word->line);
                exit(1);
            }
            if (word == NULL || word->value == NULL) {
                printf("Error: Expected value after 'return', line: %d\n", word->line);
                exit(1);
            }
            if (is_keyword(word->value)) {
                printf("Error: Expected value after 'return', got '%s', line: %d\n", word->value, word->line);
                exit(1);
            }
            W_Type result_type = w_get_type(word->value);
            char *result_type_str = w_get_type_str(word->value);
            if (*return_type != result_type) {
                if (*return_type == INT) {
                    printf("Error: Expected int value after 'return', got %s, line: %d\n", result_type_str, word->line);
                } else if (*return_type == FLOAT) {
                    printf("Error: Expected float value after 'return', got %s, line: %d\n", result_type_str, word->line);
                } else if (*return_type == STR) {
                    printf("Error: Expected str value after 'return', got %s, line: %d\n", result_type_str, word->line);
                } else if (*return_type == BOOL) {
                    printf("Error: Expected bool value after 'return', got %s, line: %d\n", result_type_str, word->line);
                } else if (*return_type == ARRAY) {
                    printf("Error: Expected array value after 'return', got %s, line: %d\n", result_type_str, word->line);
                } else if (*return_type == LIST) {
                    printf("Error: Expected list value after 'return', got %s, line: %d\n", result_type_str, word->line);
                }
                exit(1);
            }
            free(result_type_str);
            return result;

        } else if (is_type_keyword(word->value)) { //create var
            W_Type type = w_get_type(word->value);
            current_word = current_word->next;
            word = current_word->value;
            if (word == NULL || word->value == NULL) {
                printf("Error: Expected variable name after type keyword, line: %d\n", word->line);
                exit(1);
            }
            if (word->type != IDENTIFIER) {
                printf("Error: Expected variable name after type keyword, not '%s', line: %d\n", word->value, word->line);
                exit(1);
            }
            void *value;
            char *name;
            current_word = current_word->next;
            if (type == ARRAY) { //define array
                if (current_word == NULL) {
                    printf("Error: Expected array type after type keyword, line: %d\n", word->line);
                    exit(1);
                }
                word = current_word->value;
                if (!is_type_keyword(word->value)) { //get array type
                    printf("Error: Expected type keyword after 'array', line: %d\n", word->line);
                    exit(1);
                }
                W_Type array_type = w_get_type(word->value);

                current_word = current_word->next; //get array name
                if (current_word == NULL) {
                    printf("Error: Expected array name after array type, line: %d\n", word->line);
                    exit(1);
                }
                word = current_word->value;
                if (word->type != IDENTIFIER) {
                    printf("Error: Expected array name after array type, got '%s', line: %d\n", word->value, word->line);
                    exit(1);
                }
                if (dict_get(variables, word->value) != NULL) {
                    printf("Error: Variable '%s' already exists, line: %d\n", word->value, word->line);
                    exit(1);
                }
                name = word->value;

                current_word = current_word->next; //get array size
                if (current_word == NULL) {
                    printf("Error: Expected keyword 'size' after array name, line: %d\n", word->line);
                    exit(1);
                }
                word = current_word->value;
                if (strcmp(word->value, "size") != 0) {
                    printf("Error: Expected keyword 'size' after array name, got '%s', line: %d\n", word->value, word->line);
                    exit(1);
                }
                current_word = current_word->next;
                word = current_word->value;
                if (is_keyword(word->value)) {
                    printf("Error: Expected size value after 'size', line: %d\n", word->line);
                    exit(1);
                }
                if (word->type != NUMBER || is_float(word->value)) {
                    printf("Error: Expected int value after 'size', got %s, line: %d\n", word->value, word->line);
                    exit(1);
                }
                int size = atoi(word->value);

                value = array_init(array_type, size);
            } else if (type == LIST) {
                if (current_word == NULL) {
                    printf("Error: Expected list name after type keyword, line: %d\n", word->line);
                    exit(1);
                }
                word = current_word->value; //get list name
                if (word->type != IDENTIFIER) {
                    printf("Error: Expected variable name after list, got '%s', line: %d\n", word->value, word->line);
                    exit(1);
                }
                if (dict_get(variables, word->value) != NULL) {
                    printf("Error: Variable '%s' already exists, line: %d\n", word->value, word->line);
                    exit(1);
                }
                name = word->value;

                value = list_init();
            } else {
                if (current_word == NULL) {
                    printf("Error: Expected variable name after type keyword, line: %d\n", word->line);
                    exit(1);
                }
                word = current_word->value;
                if (word->type != IDENTIFIER) {
                    printf("Error: Expected variable name after type keyword, got '%s', line: %d\n", word->value, word->line);
                    exit(1);
                }
                if (dict_get(variables, word->value) != NULL) {
                    printf("Error: Variable '%s' already exists, line: %d\n", word->value, word->line);
                    exit(1);
                }
                name = word->value;
                if (type == NULL_TYPE) {
                    printf("Error: Cannot create variable with type 'null', line: %d\n", word->line);
                    exit(1);
                }
                current_word = current_word->next;
                if (current_word == NULL) {
                    printf("Error: Expected keyword 'assign' after variable name, line: %d\n", word->line);
                    exit(1);
                }
                word = current_word->value;
                if (strcmp(word->value, "assign") != 0) {
                    printf("Error: Expected keyword 'assign' after variable name, got '%s', line: %d\n", word->value, word->line);
                    exit(1);
                }
                current_word = current_word->next;
                if (current_word == NULL) {
                    printf("Error: Expected value after 'assign', line: %d\n", word->line);
                    exit(1);
                }
                word = current_word->value;
                
            }
        }
        free(stack);
        current_line = current_line->next;
    }
    dict_destroy(variables);
    parser_destroy(parsed_code);
}

/************************************************
 * Utility **************************************
 ************************************************/

/**
 * \brief Checks if the given word is a type keyword.
 * \param word The word to check.
 * \return True if the word is a type keyword, false otherwise.
 */
bool is_type_keyword(char *word) {
    for (int i = 0; i < sizeof(type_keywords) / sizeof(type_keywords[0]); i++) {
        if (strcmp(word, type_keywords[i]) == 0) return true;
    }
    return false;
}

/**
 * \brief Evaluates the parsed lines.
 * \param parsed_line The parsed line to evaluate.
 * \param variables The variables to use in the evaluation.
 * \param stack The stack to store the parsed lines.
 */
void eval_parsed_lines(W_List_Element *parsed_line, W_Dict *variables, W_List *stack) {
    while (parsed_line != NULL) {
        W_List *parsed_words = (W_List *)parsed_line->value;
        W_List_Element *current_word = parsed_words->head;
        if (parsed_words->size == 1) { //not an operation
            list_append(stack, current_word->value); //add to stack
        } else { //if the size of parsed_line is not 1 then evaluate operation
            //TODO: evaluate operation
        }
        parsed_line = parsed_line->next;
    }
}

/**
 * \brief Checks if the given string is a float.
 * \param str The string to check.
 * \return True if the string is a float, false otherwise.
 */
bool is_float(char *str) {
    int len = strlen(str);
    for (int i = 0; i < len; i++) {
        if (str[i] == '.') return true;
    }
    return false;
}