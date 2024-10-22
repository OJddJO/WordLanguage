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
    W_Type return_type = NULL_TYPE;
    execute(parsed_code, dict_init(), return_type);
    // parser_destroy(parsed_code);

    if (remove("exec.tmp") != 0) {
        printf("Error: Could not delete temp file\n");
        return 1;
    }
    if (debug) printf("Done\n"); //debug
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
    fputc('\n', temp);
    fclose(source);
    fclose(temp);
}

/**
 * \brief Executes the parsed code.
 * \param parsed_code The parsed code to execute.
 * \param args The arguments to pass to the code.
 * \return The result of the execution
 */
void *execute(W_List *parsed_code, W_Dict *args, W_Type return_type) {
    W_Dict *variables = dict_init();
    void *result = NULL;

    W_List *args_keys = dict_keys(args);
    W_List_Element *current_key = args_keys->head;
    for (int i = 0; i < args_keys->size; i++) { //copy arguments to variables
        void *var_copy;
        char *key = (char *)current_key->value;
        W_Var *var = (W_Var *)dict_get(args, key);
        if (var->type == ARRAY) {
            var_copy = array_copy((W_Array *)var->value);
        } else if (var->type == LIST) {
            var_copy = list_copy((W_List *)var->value);
        } else {
            var_copy = w_var_init(var->type);
            ((W_Var *)var_copy)->set(var_copy, var->value);
        }
        dict_set(variables, key, var_copy);
        current_key = current_key->next;
    }
    dict_destroy(args);
    if (!dict_contains(variables, "true") || !dict_contains(variables, "false")) {
        W_Bool *w_true = bool_init();
        bool_set(w_true, true);
        dict_set(variables, "true", w_true);
        W_Bool *w_false = bool_init();
        bool_set(w_false, false);
        dict_set(variables, "false", w_false);
    }

    W_List_Element *current_line = parsed_code->head;
    while (current_line != NULL) {
        W_List *line = (W_List *)current_line->value;
        W_List_Element *parsed_line = line->head;
        if (parsed_line == NULL) {
            current_line = current_line->next;
            continue;
        }

        W_List *stack = list_init(); //the stack of words to evaluate
        eval_parsed_lines(parsed_line, variables, stack);

        W_List_Element *current_word = stack->head;
        if (current_word == NULL) {
            current_line = current_line->next;
            continue;
        }
        W_Word *word = (W_Word *)current_word->value;

        if (strcmp(word->value, "def") == 0) { //function definition
            W_Func *f = func_init(); //create function
            W_Dict *fn_args = f->args; //create arguments dictionary

            //set return type
            current_word = current_word->next;
            if (current_word == NULL) {
                printf("Error: Expected type keyword after 'def', line: %d\n", word->line);
                exit(1);
            }
            word = current_word->value;
            bool is_type = is_type_keyword(word->value);
            if (!is_type) {
                printf("Error: Expected type keyword after 'def', got '%s', line: %d\n", word->value, word->line);
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
                    if (current_word == NULL) {
                        printf("Error: Expected keyword 'do' after function arguments, line: %d\n", word->line);
                        exit(1);
                    }
                    word = current_word->value;
                    if (strcmp(word->value, "do") == 0) break;
                    if (!is_type_keyword(word->value)) {
                        printf("Error: Expected type keyword in function arguments, line: %d\n", word->line);
                        exit(1);
                    } else {
                        W_Type *arg_type = (W_Type *)malloc(sizeof(W_Type));
                        *arg_type = w_get_type(word->value);
                        current_word = current_word->next;
                        if (current_word == NULL) {
                            printf("Error: Expected variable name after type keyword, line: %d\n", word->line);
                            exit(1);
                        }
                        word = current_word->value;
                        if (word->type != IDENTIFIER) {
                            printf("Error: Expected variable name after type keyword, got '%s', line: %d\n", word->value, word->line);
                            exit(1);
                        }
                        dict_set(fn_args, word->value, arg_type);
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
            W_List *function_lines = f->parsed_code;
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

            //add function to variables
            dict_set(variables, name, f);

        } else if (strcmp(word->value, "return") == 0) { //return statement
            current_word = current_word->next;
            if (current_word == NULL && return_type != NULL_TYPE) {
                printf("Error: Expected value after 'return', line: %d\n", word->line);
                exit(1);
            } else if (return_type == NULL_TYPE) {
                if (current_word != NULL) {
                    printf("Error: Cannot return value from null function, line: %d\n", word->line);
                    exit(1);
                } else {
                    dict_print(variables);
                    dict_destroy(variables);
                    free(stack);
                    return NULL; 
                }
            }
            word = current_word->value;
            if (is_keyword(word->value)) {
                printf("Error: Expected value after 'return', got '%s', line: %d\n", word->value, word->line);
                exit(1);
            }
            W_Type result_type = w_get_type(word->value);
            char *result_type_str = w_get_type_str(word->value);
            if (return_type != result_type) {
                if (return_type == INT) {
                    printf("Error: Expected int value after 'return', got %s, line: %d\n", result_type_str, word->line);
                } else if (return_type == FLOAT) {
                    printf("Error: Expected float value after 'return', got %s, line: %d\n", result_type_str, word->line);
                } else if (return_type == STRING) {
                    printf("Error: Expected str value after 'return', got %s, line: %d\n", result_type_str, word->line);
                } else if (return_type == BOOL) {
                    printf("Error: Expected bool value after 'return', got %s, line: %d\n", result_type_str, word->line);
                } else if (return_type == ARRAY) {
                    printf("Error: Expected array value after 'return', got %s, line: %d\n", result_type_str, word->line);
                } else if (return_type == LIST) {
                    printf("Error: Expected list value after 'return', got %s, line: %d\n", result_type_str, word->line);
                }
                exit(1);
            }
            free(result_type_str);
            dict_destroy(variables);
            free(stack);
            return result;

        } else if (strcmp(word->value, "call") == 0) { //function call
            current_word = current_word->next;
            if (current_word == NULL) {
                printf("Error: Expected function name after 'call', line: %d\n", word->line);
                exit(1);
            }
            word = current_word->value;
            if (word->type != IDENTIFIER) {
                printf("Error: Expected function name after 'call', got '%s', line: %d\n", word->value, word->line);
                exit(1);
            }
            W_Func *f = (W_Func *)dict_get(variables, word->value);
            if (f == NULL) {
                printf("Error: Function '%s' does not exist, line: %d\n", word->value, word->line);
                exit(1);
            } else if (f->type != FUNCTION) {
                printf("Error: Expected function, got variable '%s', line: %d\n", word->value, word->line);
                exit(1);
            }
            char *name = word->value; //name of the function
            W_Dict *fn_args = f->args; //dictionary of function arguments (name: type)
            int nb_args = dict_size(fn_args); //number of arguments
            current_word = current_word->next;
            if (current_word != NULL) { //if there is something to evaluate after function name
                word = current_word->value;
                if (strcmp(word->value, "with") == 0) { //if there are arguments
                    current_word = current_word->next;
                    if (current_word == NULL) {
                        printf("Error: Expected arguments after 'with', line: %d\n", word->line);
                        exit(1);
                    }
                    W_List *args_name = (W_List *)dict_keys(fn_args); //list of function arguments names
                    W_List_Element *current_arg = args_name->head; //current argument name
                    while (current_word != NULL) {
                        word = current_word->value;
                        if (current_arg == NULL) {
                            printf("Error: Too many arguments for function '%s', line: %d\n", name, word->line);
                            exit(1);
                        }
                        char *arg_name = (char *)current_arg->value; //name of the current argument
                        if (word->type == IDENTIFIER) { //if argument is a variable
                            void *arg = dict_get(variables, word->value);
                            if (arg == NULL) {
                                printf("Error: Variable '%s' does not exist, line: %d\n", word->value, word->line);
                                exit(1);
                            }
                            W_Var dummy;
                            dummy.type = *(W_Type *)dict_get(fn_args, arg_name);
                            char *arg_type_str = w_get_type_str(dummy.type);
                            char *input_type_str = w_get_type_str(((W_Var *)arg)->type);
                            if (((W_Var *)arg)->type != *(W_Type *)dict_get(fn_args, arg_name)) {
                                printf("Error: Expected %s value for argument '%s', got %s, line: %d\n", arg_type_str, arg_name, input_type_str, word->line);
                                exit(1);
                            }
                            free(arg_type_str);
                            free(input_type_str);
                            //TODO: make a copy of the variable and add it to the function arguments
                        } else { //if argument is a litteral
                            if (*(W_Type *)dict_get(fn_args, arg_name) == INT) {
                                if (word->type != NUMBER) {
                                    printf("Error: Expected int value for argument '%s', got %s, line: %d\n", arg_name, word->value, word->line);
                                    exit(1);
                                } else if (word->type == NUMBER && is_float(word->value)) {
                                    printf("Error: Expected int value for argument '%s', got float, line: %d\n", arg_name, word->line);
                                    exit(1);
                                }
                            } else if (*(W_Type *)dict_get(fn_args, arg_name) == STRING) {
                                if (word->type != STR) {
                                    printf("Error: Expected str value for argument '%s', got %s, line: %d\n", arg_name, word->value, word->line);
                                    exit(1);
                                }
                            } 
                            void *arg = w_var_init(*(W_Type *)dict_get(fn_args, arg_name));
                            ((W_Var *)arg)->assign(arg, word->value);
                            dict_set(variables, arg_name, word->value);
                        }
                    }
                }
                if (strcmp(word->value, "store") == 0) {
                    //TODO: store function result in variable
                }
            } else if (nb_args > 0) {
                printf("Error: Expected keyword 'with' and arguments after function name, line: %d\n", word->line);
                exit(1);
            }
        } else if (is_type_keyword(word->value)) { //create var
            W_Type type = w_get_type(word->value);
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
                if (strcmp(word->value, "null") == 0) {
                    printf("Error: Cannot create array with type 'null', line: %d\n", word->line);
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

            } else if (type == LIST) { //define list
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
                if (type == NULL_TYPE) { //cannot create null variable
                    printf("Error: Cannot create variable with type 'null', line: %d\n", word->line);
                    exit(1);
                }

                value = w_var_init(type);
                if (current_word == NULL) { //get variable name
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

                current_word = current_word->next; //assign keyword
                if (current_word == NULL) {
                    printf("Error: Expected keyword 'assign' after variable name, line: %d\n", word->line);
                    exit(1);
                }
                word = current_word->value;
                if (strcmp(word->value, "assign") != 0) {
                    printf("Error: Expected keyword 'assign' after variable name, got '%s', line: %d\n", word->value, word->line);
                    exit(1);
                }

                current_word = current_word->next; //get variable value
                if (current_word == NULL) {
                    printf("Error: Expected value after 'assign', line: %d\n", word->line);
                    exit(1);
                }
                word = current_word->value;
                if (word->type != NUMBER && word->type != STR && strcmp(word->value, "true") != 0 && strcmp(word->value, "false") != 0) {
                    printf("Error: Expected value after 'assign', got '%s', line: %d\n", word->value, word->line);
                    exit(1);
                }
                if (type == INT) { //get int value
                    if (word->type != NUMBER || is_float(word->value)) {
                        printf("Error: Expected int value after 'assign', got %s, line: %d\n", word->value, word->line);
                        exit(1);
                    }
                } else if (type == FLOAT) { //get float value
                    if (word->type != NUMBER) {
                        printf("Error: Expected float value after 'assign', got %s, line: %d\n", word->value, word->line);
                        exit(1);
                    }
                } else if (type == STR) { //get str value
                    if (word->type != STR) {
                        printf("Error: Expected str value after 'assign', got %s, line: %d\n", word->value, word->line);
                        exit(1);
                    }
                } else if (type == BOOL) { //get bool value
                    if (strcmp(word->value, "true") != 0 && strcmp(word->value, "false") != 0) {
                        printf("Error: Expected bool value after 'assign', got %s, line: %d\n", word->value, word->line);
                        exit(1);
                    }
                }
                ((W_Var *)value)->assign(value, word->value);
            }
            dict_set(variables, name, value);
        }
        free(stack);
        current_line = current_line->next;
    }
    dict_print(variables);
    dict_destroy(variables);
    return result;
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
        if (parsed_line->value == NULL) {
            parsed_line = parsed_line->next;
            continue;
        }
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