#include "interpreter.h"

int main(int argc, char *argv[]) {
    bool debug = false; //debug

    if (argc < 2 && !debug) {
        printf("Usage: word.exe <path>\n", argv[0]);
        return 1;
    }

    FILE *file;
    if (debug) file = fopen("test.w", "r"); //debug
    else file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("Error: Could not open file\n");
        return 1;
    }

    W_List *lexed_code = word_tokenize(file);
    if (fclose(file) != 0) {
        printf("Error: Could not close file\n");
        return 1;
    }
    if (debug) word_print(lexed_code); //debug

    W_List *parsed_code = parse(lexed_code);
    if (debug) print_parsed_code(parsed_code); //debug

    //initialize variables
    W_Type return_type = NULL_TYPE;
    W_Dict *default_args = dict_init();
    W_Bool *w_true = bool_init();
    bool_set(w_true, true);
    dict_set(default_args, "true", w_true);
    W_Bool *w_false = bool_init();
    bool_set(w_false, false);
    dict_set(default_args, "false", w_false);

    if (debug) printf("Executing...\n");
    execute(parsed_code, default_args, return_type, debug);
    if (debug) printf("Executed !\n");
    // parser_destroy(parsed_code); // TODO: Fix double free, not necessary but cleaner (memory leak)

    if (debug) printf("Done\n"); //debug
    return 0;
}

/**
 * \brief Executes the parsed code.
 * \param parsed_code The parsed code to execute.
 * \param args The arguments to pass to the code.
 * \param return_type The type of the return value.
 * \param debug Whether to print debug information.
 * \return The result of the execution
 */
void *execute(W_List *parsed_code, W_Dict *args, W_Type return_type, bool debug) {
    W_Dict *variables = args;
    void *result = NULL;
    if (debug) {
        printf("Args:\n"); //debug
        char *str = dict_stringify(variables);
        printf("%s\n", str);
        free(str);
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

        /*************************************************
         * Function **************************************
         *************************************************/
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
                current_word = current_word->next;
                while (current_word != NULL) {
                    word = current_word->value;
                    if (!is_type_keyword(word->value)) {
                        printf("Error: Expected type keyword in function arguments, line: %d\n", word->line);
                        exit(1);
                    }
                    if (strcmp(word->value, "null") == 0) {
                        printf("Error: Cannot have null argument, line: %d\n", word->line);
                        exit(1);
                    }
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
                    } else dict_set(fn_args, word->value, arg_type);
                    current_word = current_word->next;
                }
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
                    dict_destroy(variables);
                    free(stack);
                    return NULL; 
                }
            }
            word = current_word->value;
            if (word->type == KEYWORD) {
                printf("Error: Expected value after 'return', got '%s', line: %d\n", word->value, word->line);
                exit(1);
            }
            W_Var *result;
            if (word->type == IDENTIFIER) { //if variable
                result = (W_Var *)dict_get(variables, word->value);
                if (result == NULL) {
                    printf("Error: Variable '%s' does not exist, line: %d\n", word->value, word->line);
                    exit(1);
                }
                result = (result)->copy(result);
            } else { // if value (cannot be array or list)
                if (return_type == INT) {
                    if (word->type != NUMBER || is_float(word->value)) {
                        printf("Error: Expected int value after 'return', got %s, line: %d\n", word->value, word->line);
                        exit(1);
                    }
                } else if (return_type == FLOAT) {
                    if (word->type != NUMBER) {
                        printf("Error: Expected float value after 'return', got %s, line: %d\n", word->value, word->line);
                        exit(1);
                    }
                } else if (return_type == STRING) {
                    if (word->type != STR) {
                        printf("Error: Expected str value after 'return', got %s, line: %d\n", word->value, word->line);
                        exit(1);
                    }
                } else if (return_type == BOOL) {
                    if (strcmp(word->value, "true") != 0 && strcmp(word->value, "false") != 0) {
                        printf("Error: Expected bool value after 'return', got %s, line: %d\n", word->value, word->line);
                        exit(1);
                    }
                }
                result->assign(result, word->value);
            }
            char *result_type_str = w_get_type_str(result);
            if (return_type != result->type) { //if return type is not the same as the result type
                if (return_type == INT) {
                    printf("Error: Expected int value after 'return', got %s, line: %d\n", result_type_str, word->line);
                } else if (return_type == FLOAT) {
                    printf("Error: Expected float value after 'return', got %s, line: %d\n", result_type_str, word->line);
                } else if (return_type == STRING) {
                    printf("Error: Expected str value after 'return', got %s, line: %d\n", result_type_str, word->line);
                } else if (return_type == BOOL) {
                    printf("Error: Expected bool value after 'return', got %s, line: %d\n", result_type_str, word->line);
                } else if (return_type == LIST) {
                    printf("Error: Expected list value after 'return', got %s, line: %d\n", result_type_str, word->line);
                }
                exit(1);
            }
            if (debug) {
                printf("Returning: ");
                char *str = result->stringify(result);
                printf("%s\n", str);
                free(str);
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
            if (debug) printf("Called function %s\n", word->value); //debug
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
            W_Dict *fn_vars = dict_init(); //variables of the function

            //copy all variables to function variables
            if (debug) {
                printf("Copying scope variables to function variables...\n"); //debug
                printf("Scope variables:\n"); //debug
                char *str = dict_stringify(variables);
                printf("%s\n", str);
                free(str);
            }
            W_List *variables_keys = dict_keys(variables);
            W_List_Element *current_key = variables_keys->head;
            for (int i = 0; i < variables_keys->size; i++) { //copy arguments to variables
                char *key = (char *)current_key->value;
                W_Var *var = (W_Var *)dict_get(variables, key);
                void *var_copy = var->copy(var);
                dict_set(fn_vars, key, var_copy);
                current_key = current_key->next;
            }

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
                        if (word->type == KEYWORD) {
                            break;
                        }
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
                            char *arg_type_str = w_get_type_str(&dummy);
                            char *input_type_str = w_get_type_str(arg);
                            if (((W_Var *)arg)->type != *(W_Type *)dict_get(fn_args, arg_name)) {
                                printf("Error: Expected %s value for argument '%s', got %s, line: %d\n", arg_type_str, arg_name, input_type_str, word->line);
                                exit(1);
                            }
                            free(arg_type_str);
                            free(input_type_str);
                            W_Var *arg_copy = ((W_Var *)arg)->copy(arg); //copy of the argument
                            dict_set(fn_vars, arg_name, arg_copy);
                        } else { //if argument is a litteral (cannot be array or list)
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
                            dict_set(fn_vars, arg_name, word->value);
                        }
                        current_arg = current_arg->next;
                        current_word = current_word->next;
                    }
                }
                if (debug) {
                    printf("Function variables:\n"); //debug
                    char *str = dict_stringify(fn_vars);
                    printf("%s\n", str);
                    free(str);
                }
                if (current_word != NULL) {
                    word = current_word->value;
                    if (strcmp(word->value, "store") == 0) {
                        if (f->return_type == NULL_TYPE) {
                            printf("Error: Cannot store result of null function, line: %d\n", word->line);
                            exit(1);
                        }
                        current_word = current_word->next;
                        if (current_word == NULL) {
                            printf("Error: Expected variable name after 'store', line: %d\n", word->line);
                            exit(1);
                        }
                        word = current_word->value;
                        if (word->type != IDENTIFIER) {
                            printf("Error: Expected variable name after 'store', got '%s', line: %d\n", word->value, word->line);
                            exit(1);
                        }
                        if (debug) printf("Starting function execution...\n");
                        void *result = execute(f->parsed_code, fn_vars, f->return_type, debug);
                        dict_set(variables, word->value, result);
                        if (debug) printf("Function executed !\n");
                    }
                } else {
                    if (debug) printf("Starting function execution...\n");
                    execute(f->parsed_code, fn_vars, f->return_type, debug);
                    if (debug) printf("Function executed !\n");
                }
            } else if (nb_args > 0) {
                printf("Error: Expected keyword 'with' and arguments after function name, line: %d\n", word->line);
                exit(1);
            }
        } 
        /*************************************************
         * IO ********************************************
         *************************************************/
        else if (strcmp(word->value, "print") == 0) { //print
            current_word = current_word->next;
            if (current_word == NULL) {
                printf("Error: Expected value after 'print', line: %d\n", word->line);
                exit(1);
            }
            char *sep = " ";
            char *end = "\n";
            while (current_word != NULL) {
                word = current_word->value;
                if (word->type == KEYWORD) {
                    if (strcmp(word->value, "sep") == 0) {
                        current_word = current_word->next;
                        if (current_word == NULL) {
                            printf("Error: Expected value after 'sep', line: %d\n", word->line);
                            exit(1);
                        }
                        word = current_word->value;
                        if (word->type != STR) {
                            printf("Error: Expected str value after 'sep', got '%s', line: %d\n", word->value, word->line);
                            exit(1);
                        }
                        sep = word->value;
                        char str[strlen(sep)-1];
                        strncpy(str, sep+1, strlen(sep)-2);
                        str[strlen(sep)-2] = '\0';
                        strcpy(sep, str);
                    } else if (strcmp(word->value, "end") == 0) {
                        current_word = current_word->next;
                        if (current_word == NULL) {
                            printf("Error: Expected value after 'end', line: %d\n", word->line);
                            exit(1);
                        }
                        word = current_word->value;
                        if (word->type != STR) {
                            printf("Error: Expected str value after 'end', got '%s', line: %d\n", word->value, word->line);
                            exit(1);
                        }
                        end = word->value;
                        char str[strlen(end)-1];
                        strncpy(str, end+1, strlen(end)-2);
                        str[strlen(end)-2] = '\0';
                        strcpy(end, str);
                    } else {
                        printf("Error: Unexpected keyword '%s', line: %d\n", word->value, word->line);
                        exit(1);
                    }
                    current_word = current_word->next;
                } else {
                    if (word->type == IDENTIFIER) {
                        W_Var *var = (W_Var *)dict_get(variables, word->value);
                        if (var == NULL) {
                            printf("Error: Variable '%s' does not exist, line: %d\n", word->value, word->line);
                            exit(1);
                        }
                        char *str = var->stringify(var);
                        printf("%s", str);
                        free(str);
                    } else if (word->type == STR) {
                        char str[strlen(word->value)-1];
                        strncpy(str, word->value+1, strlen(word->value)-2);
                        str[strlen(word->value)-2] = '\0';
                        printf("%s", str);
                    } else printf("%s", word->value);
                    current_word = current_word->next;
                    if (current_word != NULL) {
                        printf("%s", sep);
                    }
                }
            }
            printf("%s", end);
        }
        /*************************************************
         * Variables *************************************
         *************************************************/
        else if (is_type_keyword(word->value)) { //create var
            W_Type type = w_get_type(word->value);
            char *name;
            void *value;
            current_word = current_word->next;
            if (type == LIST) { //define list
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
    if (debug) {
        printf("Exit Variables:\n"); //debug
        char *str = dict_stringify(variables);
        printf("%s\n", str);
        free(str);
    }
    dict_destroy(variables);
    return result;
}

/************************************************
 * Utility **************************************
 ************************************************/

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
            int line;
            W_List *result = list_init();
            W_List_Element *current_word = parsed_words->head;
            while (current_word != NULL) {
                W_Word *word = (W_Word *)current_word->value;
                line = word->line;
                if (word->type == OPERATOR) {
                    remove_dot(word);
                    if (strcmp(word->value, "plus") == 0) { //addition
                        if (result->size < 2) {
                            printf("Error: Expected 2 values for operator 'plus', line: %d\n", word->line);
                            exit(1);
                        }
                        W_Var *w1 = (W_Var *)list_pop(result);
                        W_Var *w2 = (W_Var *)list_pop(result);
                        list_append(result, w_plus(w2, w1));
                        w1->destroy(w1);
                        w2->destroy(w2);
                    } else if (strcmp(word->value, "minus") == 0) { //subtraction
                        if (result->size < 2) {
                            printf("Error: Expected 2 values for operator 'minus', line: %d\n", word->line);
                            exit(1);
                        }
                        W_Var *w1 = (W_Var *)list_pop(result);
                        W_Var *w2 = (W_Var *)list_pop(result);
                        list_append(result, w_minus(w2, w1));
                        w1->destroy(w1);
                        w2->destroy(w2);
                    } else if (strcmp(word->value, "times") == 0) { //multiplication
                        if (result->size < 2) {
                            printf("Error: Expected 2 values for operator 'time', line: %d\n", word->line);
                            exit(1);
                        }
                        W_Var *w1 = (W_Var *)list_pop(result);
                        W_Var *w2 = (W_Var *)list_pop(result);
                        list_append(result, w_time(w2, w1));
                        w1->destroy(w1);
                        w2->destroy(w2);
                    } else if (strcmp(word->value, "div") == 0) { //division
                        if (result->size < 2) {
                            printf("Error: Expected 2 values for operator 'div', line: %d\n", word->line);
                            exit(1);
                        }
                        W_Var *w1 = (W_Var *)list_pop(result);
                        W_Var *w2 = (W_Var *)list_pop(result);
                        list_append(result, w_div(w2, w1));
                        w1->destroy(w1);
                        w2->destroy(w2);
                    } else if (strcmp(word->value, "mod") == 0) { //modulo
                        if (result->size < 2) {
                            printf("Error: Expected 2 values for operator 'mod', line: %d\n", word->line);
                            exit(1);
                        }
                        W_Var *w1 = (W_Var *)list_pop(result);
                        W_Var *w2 = (W_Var *)list_pop(result);
                        list_append(result, w_mod(w2, w1));
                        w1->destroy(w1);
                        w2->destroy(w2);
                    } else if (strcmp(word->value, "ediv") == 0) { //euclidean division
                        if (result->size < 2) {
                            printf("Error: Expected 2 values for operator 'ediv', line: %d\n", word->line);
                            exit(1);
                        }
                        W_Var *w1 = (W_Var *)list_pop(result);
                        W_Var *w2 = (W_Var *)list_pop(result);
                        list_append(result, w_ediv(w2, w1));
                        w1->destroy(w1);
                        w2->destroy(w2);
                    } else if (strcmp(word->value, "power") == 0) { //power
                        if (result->size < 2) {
                            printf("Error: Expected 2 values for operator 'pow', line: %d\n", word->line);
                            exit(1);
                        }
                        W_Var *w1 = (W_Var *)list_pop(result);
                        W_Var *w2 = (W_Var *)list_pop(result);
                        list_append(result, w_power(w2, w1));
                        w1->destroy(w1);
                        w2->destroy(w2);
                    } else if (strcmp(word->value, "sqrt") == 0) { //square root
                        if (result->size < 1) {
                            printf("Error: Expected 1 value for operator 'sqrt', line: %d\n", word->line);
                            exit(1);
                        }
                        W_Var *w = (W_Var *)list_pop(result);
                        list_append(result, w_sqrt(w));
                        w->destroy(w);
                    }
                } else if (word->type == NUMBER) {
                    if (is_float(word->value)) {
                        W_Float *w = w_var_init(FLOAT);
                        w->assign(w, word->value);
                        list_append(result, w);
                    } else {
                        W_Int *w = w_var_init(INT);
                        w->assign(w, word->value);
                        list_append(result, w);
                    }                
                } else {
                    printf("Error: Unexpected word '%s', line: %d\n", word->value, word->line);
                    exit(1);
                }
                free(word->value);
                free(word);
                current_word = current_word->next;
            }
            if (result->size > 1) {
                printf("Error: Uncorrect number of operands, line: %d\n", line);
                exit(1);
            }
            W_Var *result_var = (W_Var *)list_pop(result);
            char *str = result_var->stringify(result_var);
            W_Word *result_word = (W_Word *)malloc(sizeof(W_Word));
            result_word->type = NUMBER;
            result_word->value = str;
            result_word->line = line;
            list_append(stack, result_word);
        }
        parsed_line = parsed_line->next;
    }
}

/**
 * \brief Removes priority dots of operations from the given word.
 * \param word The word to remove the dot from.
 */
void remove_dot(W_Word *word) {
    int priority = 0;
    for (int i = 0; i < strlen(word->value); i++) {
        if (word->value[i] == '.') {
            priority++;
        } else break;
    }
    char without_dot[strlen(word->value) - priority + 1];
    strncpy(without_dot, word->value + priority, strlen(word->value) - priority);
    without_dot[strlen(word->value) - priority] = '\0';
    word->value = (char *)realloc(word->value, strlen(without_dot) + 1);
    strcpy(word->value, without_dot);
}

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