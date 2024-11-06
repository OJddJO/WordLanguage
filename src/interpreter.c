#include "interpreter.h"

int main(int argc, char *argv[]) {

    if (!DEBUG) { //file name checks
        if (argc < 2) {
            printf("Usage: word.exe <path>\n");
            return 1;
        }
        if (strlen(argv[1]) < 3) {
            fprintf(stderr, "Error: Invalid file name\n");
            return 1;
        }
        if (strcmp(argv[1] + strlen(argv[1]) - 2, ".w") != 0) {
            fprintf(stderr, "Error: Invalid file extension\n");
            return 1;
        }
    }

    FILE *file;
    if (DEBUG) file = fopen("test.w", "r"); //DEBUG
    else file = fopen(argv[1], "r");
    if (file == NULL) {
        fprintf(stderr, "Error: Could not open file\n");
        return 1;
    }

    list *lexed_code = word_tokenize(file);
    if (fclose(file) != 0) {
        fprintf(stderr, "Error: Could not close file\n");
        return 1;
    }
    if (DEBUG) lexer_print(lexed_code); //DEBUG

    list *parsed_code = parse(lexed_code);
    if (DEBUG) print_parsed_code(parsed_code); //DEBUG

    //initialize variables
    if (DEBUG) printf("Initializing main scope...\n"); //DEBUG
    Scope *main_scope = init_scope();
    W_Type return_type = NULL_TYPE;
    W_Dict *default_vars = main_scope->vars;
    W_Bool *w_true = w_bool_init();
    w_bool_set(w_true, true);
    w_dict_set(default_vars, "true", w_true);
    W_Bool *w_false = w_bool_init();
    w_bool_set(w_false, false);
    w_dict_set(default_vars, "false", w_false);

    if (DEBUG) printf("Executing...\n");
    execute(parsed_code, main_scope, return_type, true);
    if (DEBUG) printf("Executed !\n");
    parser_destroy(parsed_code);

    if (DEBUG) printf("Done\n"); //DEBUG
    if (MONITOR_MEMORY) w_alloc_print();

    return 0;
}

/**
 * \brief Executes the parsed code.
 * \param parsed_code The parsed code to execute.
 * \param scope The scope to execute the code in.
 * \param return_type The type of the return value.
 * \param destroy_scope_on_exit Whether to destroy the scope on exit.
 * \return The result of the execution
 */
void *execute(list *parsed_code, Scope *scope, W_Type return_type, bool destroy_scope_on_exit) {
    void *result = NULL;
    // if (DEBUG) {
    //     printf("Args:\n"); //DEBUG
    //     char *str = w_dict_stringify(scope->vars);
    //     printf("%s\n", str);
    //     w_free(str);
    //     // print parsed code
    //     printf("Parsed code:\n");
    //     print_parsed_code(parsed_code);
    // }

    list_element *current_line = parsed_code->head;
    while (current_line != NULL) {
        list *line = (list *)current_line->value;
        if (line == NULL) {
            current_line = current_line->next;
            continue;
        }
        list_element *current_block = line->head;
        if (current_block == NULL) {
            current_line = current_line->next;
            continue;
        }

        list *stack = list_init(); //the stack of words to evaluate
        eval_parsed_lines(current_block, scope, stack);
        // print stack
        // if (DEBUG) {
        //     printf("Stack:\n");
        //     list_element *current_word = stack->head;
        //     while (current_word != NULL) {
        //         W_Word *word = (W_Word *)current_word->value;
        //         printf("%s ", word->value);
        //         current_word = current_word->next;
        //     }
        //     printf("\n");
        // }

        list_element *current_word = stack->head;
        if (current_word == NULL) {
            current_line = current_line->next;
            continue;
        }
        W_Word *word = (W_Word *)current_word->value;
        char *statement; //the statement to print in case of EOL error

        /* SECTION - Functions */
        if (strcmp(word->value, "def") == 0) { //SECTION - def
            statement = "function definition";

            if (DEBUG) printf("Defining function...\n");
            W_Func *f = w_func_init(); //create function
            dict *fn_args = f->args; //create arguments dictionary

            //set return type
            current_word = current_word->next;
            if (current_word == NULL) {
                fprintf(stderr, "Error: Expected type keyword after 'def', line: %d\n", word->line);
                exit(1);
            }
            word = current_word->value;
            bool is_type = is_type_keyword(word->value);
            if (!is_type) {
                fprintf(stderr, "Error: Expected type keyword after 'def', got '%s', line: %d\n", word->value, word->line);
                exit(1);
            }
            if (strcmp(word->value, "null") == 0) {
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
            if (current_word != NULL) {
                word = current_word->value;
                if (strcmp(word->value, "with") == 0) {
                    current_word = current_word->next;
                    while (current_word != NULL) {
                        word = current_word->value;
                        if (!is_type_keyword(word->value)) {
                            fprintf(stderr, "Error: Expected type keyword in function arguments, line: %d\n", word->line);
                            exit(1);
                        }
                        if (strcmp(word->value, "null") == 0) {
                            fprintf(stderr, "Error: Cannot have null argument, line: %d\n", word->line);
                            exit(1);
                        }
                        W_Type *arg_type = (W_Type *)w_malloc(sizeof(W_Type));
                        *arg_type = w_get_type(word->value);

                        current_word = current_word->next;
                        if (current_word == NULL) {
                            fprintf(stderr, "Error: Expected variable name after type keyword, line: %d\n", word->line);
                            exit(1);
                        }
                        word = current_word->value;
                        if (word->type != IDENTIFIER) {
                            fprintf(stderr, "Error: Expected variable name after type keyword, got '%s', line: %d\n", word->value, word->line);
                            exit(1);
                        } else dict_set(fn_args, word->value, arg_type);
                        current_word = current_word->next;
                    }
                } else {
                    fprintf(stderr, "Error: Expected keyword 'with' after function name, got '%s', line: %d\n", word->value, word->line);
                    exit(1);
                }
            }

            //get function lines
            int fn_line = word->line;
            int fn_def_count = 0; //number of 'def' keywords
            list *function_lines = f->parsed_code;
            bool end = false;
            current_line = current_line->next;
            while (current_line != NULL) {
                line = (list *)current_line->value;
                current_block = line->head;
                if (current_block == NULL) {
                    current_line = current_line->next;
                    continue;
                }
                current_word = ((list *)current_block->value)->head;
                if (current_word != NULL) {
                    word = (W_Word *)current_word->value;
                    if (strcmp(word->value, "def") == 0) {
                        fn_def_count++;
                    } else if (strcmp(word->value, "enddef") == 0) {
                        if (fn_def_count == 0) {
                            end = true;
                            break;
                        } else fn_def_count--;
                    }
                }
                list_append(function_lines, line);
                current_line = current_line->next;
            }
            if (!end) {
                fprintf(stderr, "Error: Expected keyword 'enddef' at end of the function definition, line: %d\n", fn_line);
                exit(1);
            }

            //add function to inner_scope
            W_Func *prev_fn = (W_Func *)get_var(scope, name);
            if (prev_fn != NULL){
                fprintf(stderr, "Error: Variable '%s' already exists, line: %d\n", name, word->line);
                exit(1);
            }
            w_dict_set(scope->vars, name, f);
            if (DEBUG) printf("Function defined !\n"); //!SECTION - def

        } else if (strcmp(word->value, "enddef") == 0) {
            statement = "enddef";

        } else if (strcmp(word->value, "return") == 0) { //SECTION - return
            statement = "return";

            current_word = current_word->next;
            if (current_word == NULL && return_type != NULL_TYPE) {
                fprintf(stderr, "Error: Expected value after 'return', line: %d\n", word->line);
                exit(1);
            } else if (return_type == NULL_TYPE) {
                if (current_word != NULL) {
                    fprintf(stderr, "Error: Cannot return value from null function, line: %d\n", word->line);
                    exit(1);
                } else {
                    destroy_stack(stack);
                    return NULL;
                }
            }
            word = current_word->value;
            if (word->type == KEYWORD) {
                fprintf(stderr, "Error: Expected value after 'return', got '%s', line: %d\n", word->value, word->line);
                exit(1);
            }
            W_Var *result;
            if (word->type == IDENTIFIER) { //if variable
                result = get_var(scope, word->value);
                if (result == NULL) {
                    fprintf(stderr, "Error: Variable '%s' does not exist, line: %d\n", word->value, word->line);
                    exit(1);
                }
                result = (result)->copy(result);
            } else { // if value (cannot be constructed types)
                if (return_type == INT) {
                    if (word->type != NUMBER) {
                        fprintf(stderr, "Error: Expected int value after 'return', got %s, line: %d\n", word->value, word->line);
                        exit(1);
                    }
                    result = (W_Var *)w_int_init();
                } else if (return_type == FLOAT) {
                    if (word->type != NUMBER) {
                        fprintf(stderr, "Error: Expected float value after 'return', got %s, line: %d\n", word->value, word->line);
                        exit(1);
                    }
                    result = (W_Var *)w_float_init();
                } else if (return_type == STRING) {
                    if (word->type != STR) {
                        fprintf(stderr, "Error: Expected str value after 'return', got %s, line: %d\n", word->value, word->line);
                        exit(1);
                    }
                    result = (W_Var *)w_str_init();
                } else if (return_type == BOOL) {
                    if (strcmp(word->value, "true") != 0 && strcmp(word->value, "false") != 0) {
                        fprintf(stderr, "Error: Expected bool value after 'return', got %s, line: %d\n", word->value, word->line);
                        exit(1);
                    }
                    result = (W_Var *)w_bool_init();
                }
                result->assign(result, word->value);
            }
            char *result_type_str = w_get_type_str(result);
            if (return_type != result->type) { //if return type is not the same as the result type
                if (return_type == INT) {
                    fprintf(stderr, "Error: Expected int value after 'return', got %s, line: %d\n", result_type_str, word->line);
                } else if (return_type == FLOAT) {
                    fprintf(stderr, "Error: Expected float value after 'return', got %s, line: %d\n", result_type_str, word->line);
                } else if (return_type == STRING) {
                    fprintf(stderr, "Error: Expected str value after 'return', got %s, line: %d\n", result_type_str, word->line);
                } else if (return_type == BOOL) {
                    fprintf(stderr, "Error: Expected bool value after 'return', got %s, line: %d\n", result_type_str, word->line);
                } else if (return_type == LIST) {
                    fprintf(stderr, "Error: Expected list value after 'return', got %s, line: %d\n", result_type_str, word->line);
                }
                exit(1);
            }
            if (DEBUG) {
                printf("Returning: ");
                char *str = result->stringify(result);
                printf("%s\n", str);
                w_free(str);
            }
            w_free(result_type_str);
            destroy_stack(stack);
            if (DEBUG) {
                printf("Exit Variables:\n"); //DEBUG
                char *str = w_dict_stringify(scope->vars);
                printf("%s\n", str);
                w_free(str);
            }
            if (DEBUG && destroy_scope_on_exit) {
                printf("Freeing inner scope... (%p)\n", scope); //DEBUG
                char *str_dict =  w_dict_stringify(scope->vars);
                printf("%s\n", str_dict);
                w_free(str_dict);
            }
            if (destroy_scope_on_exit) destroy_scope(scope);
            if (DEBUG) printf("Exiting...\n"); //DEBUG
            return result; //!SECTION - return
        } else if (strcmp(word->value, "call") == 0) { //SECTION - call
            statement = "function call";

            current_word = current_word->next;
            if (current_word == NULL) {
                fprintf(stderr, "Error: Expected function name after 'call', line: %d\n", word->line);
                exit(1);
            }
            word = current_word->value;
            if (word->type != IDENTIFIER) {
                fprintf(stderr, "Error: Expected function name after 'call', got '%s', line: %d\n", word->value, word->line);
                exit(1);
            }
            if (DEBUG) printf("Called function %s\n", word->value); //DEBUG
            W_Func *f = (W_Func *)get_var(scope, word->value);
            if (f == NULL) {
                fprintf(stderr, "Error: Function '%s' does not exist, line: %d\n", word->value, word->line);
                exit(1);
            } else if (f->type != FUNCTION) {
                fprintf(stderr, "Error: Expected function, got variable '%s', line: %d\n", word->value, word->line);
                exit(1);
            }
            char *name = word->value; //name of the function
            dict *fn_args = f->args; //dictionary of function arguments (name: type)

            //copy all variables to function variables
            if (DEBUG) {
                printf("Copying scope variables to function variables...\n"); //DEBUG
                printf("Scope variables: (%p)\n", scope->vars); //DEBUG
                char *str = w_dict_stringify(scope->vars);
                printf("%s\n", str);
                w_free(str);
                printf("Initializing function scope...\n");
            }

            Scope *fn_scope = init_scope(); //function scope
            fn_scope->parent = scope;
            W_Dict *fn_vars = fn_scope->vars; //function variables

            int nb_args = dict_size(fn_args); //number of arguments
            current_word = current_word->next;
            if (current_word != NULL) { //if there is something to evaluate after function name
                word = current_word->value;
                if (strcmp(word->value, "with") == 0) { //if there are arguments
                    current_word = current_word->next;
                    if (current_word == NULL) {
                        fprintf(stderr, "Error: Expected arguments after 'with', line: %d\n", word->line);
                        exit(1);
                    }
                    list *args_name = (list *)dict_keys(fn_args); //list of function arguments names
                    list_element *current_arg = args_name->head; //current argument name
                    while (current_word != NULL) {
                        word = current_word->value;
                        if (word->type == KEYWORD) {
                            break;
                        }
                        if (current_arg == NULL) {
                            fprintf(stderr, "Error: Too many arguments for function '%s', line: %d\n", name, word->line);
                            exit(1);
                        }
                        char *arg_name = (char *)current_arg->value; //name of the current argument.
                        if (word->type == IDENTIFIER) { //if argument is a variable
                            W_Var *arg = get_var(scope, word->value);
                            if (arg == NULL) {
                                fprintf(stderr, "Error: Variable '%s' does not exist, line: %d\n", word->value, word->line);
                                exit(1);
                            }
                            if (arg->type != *(W_Type *)dict_get(fn_args, arg_name)) {
                                W_Var dummy;
                                dummy.type = *(W_Type *)dict_get(fn_args, arg_name);
                                char *arg_type_str = w_get_type_str(&dummy);
                                char *input_type_str = w_get_type_str(arg);
                                fprintf(stderr, "Error: Expected %s value for argument '%s', got %s, line: %d\n", arg_type_str, arg_name, input_type_str, word->line);
                                w_free(arg_type_str);
                                w_free(input_type_str);
                                exit(1);
                            }
                            W_Var *arg_copy = ((W_Var *)arg)->copy(arg); //copy of the argument
                            w_dict_set(fn_vars, arg_name, arg_copy);
                        } else { //if argument is a litteral (cannot be constructed types)
                            if (*(W_Type *)dict_get(fn_args, arg_name) == INT) {
                                if (word->type != NUMBER) {
                                    fprintf(stderr, "Error: Expected int value for argument '%s', got %s, line: %d\n", arg_name, word->value, word->line);
                                    exit(1);
                                }
                            } else if (*(W_Type *)dict_get(fn_args, arg_name) == FLOAT) {
                                if (word->type != NUMBER) {
                                    fprintf(stderr, "Error: Expected float value for argument '%s', got %s, line: %d\n", arg_name, word->value, word->line);
                                    exit(1);
                                }
                            } else if (*(W_Type *)dict_get(fn_args, arg_name) == STRING) {
                                if (word->type != STR) {
                                    fprintf(stderr, "Error: Expected str value for argument '%s', got %s, line: %d\n", arg_name, word->value, word->line);
                                    exit(1);
                                }
                            } 
                            void *arg = w_var_init(*(W_Type *)dict_get(fn_args, arg_name));
                            ((W_Var *)arg)->assign(arg, word->value);
                            w_dict_set(fn_vars, arg_name, arg);
                        }
                        current_arg = current_arg->next;
                        current_word = current_word->next;
                    }
                    if (current_arg != NULL) {
                        fprintf(stderr, "Error: Not enough arguments for function '%s', line: %d\n", name, word->line);
                        exit(1);
                    }
                } else if (nb_args > 0) {
                    fprintf(stderr, "Error: Expected keyword 'with' and %d argument(s) to call function '%s', line: %d\n", nb_args, name, word->line);
                    exit(1);
                }
                if (DEBUG) {
                    printf("Function scope initialized !\n");
                    printf("Function variables: (%p)\n", fn_vars); //DEBUG
                    char *str = w_dict_stringify(fn_vars);
                    printf("%s\n", str);
                    w_free(str);
                }
                if (current_word != NULL) {
                    word = current_word->value;
                    if (strcmp(word->value, "store") == 0) {
                        if (f->return_type == NULL_TYPE) {
                            fprintf(stderr, "Error: Cannot store result of null function, line: %d\n", word->line);
                            exit(1);
                        }
                        current_word = current_word->next;
                        if (current_word == NULL) {
                            fprintf(stderr, "Error: Expected variable name after 'store', line: %d\n", word->line);
                            exit(1);
                        }
                        word = current_word->value;
                        if (word->type != IDENTIFIER) {
                            fprintf(stderr, "Error: Expected variable name after 'store', got '%s', line: %d\n", word->value, word->line);
                            exit(1);
                        }
                        if (DEBUG) printf("Starting function execution...\n");
                        void *result = execute(f->parsed_code, fn_scope, f->return_type, true);
                        w_dict_set(scope->vars, word->value, result);
                    }
                } else {
                    if (DEBUG) printf("Starting function execution...\n");
                    void *result = execute(f->parsed_code, fn_scope, f->return_type, true);
                    if (result != NULL) ((W_Var *)result)->destroy(result);
                }
                if (DEBUG) printf("Function executed !\n");
            } else if (nb_args > 0) {
                fprintf(stderr, "Error: Expected keyword 'with' and %d argument(s) to call function '%s', line: %d\n", nb_args, name, word->line);
                exit(1);
            } else {
                if (DEBUG) printf("Starting function execution...\n");
                void *result = execute(f->parsed_code, fn_scope, f->return_type, true);
                if (result != NULL) ((W_Var *)result)->destroy(result);
                if (DEBUG) printf("Function executed !\n");
            } //!SECTION - call
        }
        //!SECTION - Functions

        /* SECTION - Control Flow */
        else if (strcmp(word->value, "if") == 0 || strcmp(word->value, "elif") == 0) { //SECTION - if/elif
            statement = "if/elif block";

            current_word = current_word->next;
            if (current_word == NULL) {
                fprintf(stderr, "Error: Expected boolean expression after 'if'/'elif', line: %d\n", word->line);
                exit(1);
            }
            word = current_word->value;
            if (word->type != IDENTIFIER) { //true and false are also identifiers
                fprintf(stderr, "Error: Expected boolean expression after 'if'/'elif', got '%s', line: %d\n", word->value, word->line);
                exit(1);
            }
            W_Bool *condition = (W_Bool *)get_var(scope, word->value);
            if (condition == NULL) {
                fprintf(stderr, "Error: Variable '%s' does not exist, line: %d\n", word->value, word->line);
                exit(1);
            }
            if (condition->type != BOOL) {
                fprintf(stderr, "Error: Expected bool value after 'if'/'elif', got '%s', line: %d\n", word->value, word->line);
                exit(1);
            }
            list *if_lines = list_init();
            int if_count = 0;
            bool end = false;
            current_line = current_line->next;
            while (current_line != NULL) {
                line = (list *)current_line->value;
                current_block = line->head;
                if (current_block == NULL) {
                    current_line = current_line->next;
                    continue;
                }
                current_word = ((list *)current_block->value)->head;
                if (current_word != NULL) {
                    word = (W_Word *)current_word->value;
                    if (strcmp(word->value, "if") == 0) {
                        if_count++;
                    } else if (if_count == 0) {
                        if (strcmp(word->value, "elif") == 0 || strcmp(word->value, "else") == 0 || strcmp(word->value, "endif") == 0) {
                            end = true;
                            break;
                        }
                    } else if_count--;
                }
                list_append(if_lines, line);
                current_line = current_line->next;
            }
            current_line = current_line->prev; //go back to the last line of the if/elif block
            if (!end) {
                fprintf(stderr, "Error: Expected keyword 'endif', 'elif' or 'else' at end of the if/elif block, line: %d\n", word->line);
                exit(1);
            }
            if (*condition->value) {
                int if_count = 0;
                while (current_line != NULL) { //skip lines until endif then execute the if block
                    line = (list *)current_line->value;
                    current_block = line->head;
                    if (current_block == NULL) {
                        current_line = current_line->next;
                        continue;
                    }
                    current_word = ((list *)current_block->value)->head;
                    if (current_word != NULL) {
                        word = (W_Word *)current_word->value;
                        if (strcmp(word->value, "if") == 0) {
                            if_count++;
                        } else if (strcmp(word->value, "endif") == 0) {
                            if (if_count == 0) {
                                break;
                            } else if_count--;
                        }
                    }
                    current_line = current_line->next;
                }
                if (DEBUG) printf("Executing if/elif block...\n");
                void *return_value = execute(if_lines, scope, return_type, false);
                if (DEBUG) printf("If/elif block executed !\n");
                if (return_value != NULL) {
                    list_destroy_no_free(if_lines);
                    destroy_stack(stack);
                    if (destroy_scope_on_exit) destroy_scope(scope);
                    return return_value;
                }
            } else if (DEBUG) printf("Skipping if/elif block...\n");
            list_destroy_no_free(if_lines); //!SECTION - if/elif
        } else if (strcmp(word->value, "else") == 0) { //SECTION - else
            statement = "else block";

            list *else_lines = list_init();
            int if_count = 0;
            bool end = false;
            current_line = current_line->next;
            while (current_line != NULL) {
                line = (list *)current_line->value;
                current_block = line->head;
                if (current_block == NULL) {
                    current_line = current_line->next;
                    continue;
                }
                current_word = ((list *)current_block->value)->head;
                if (current_word != NULL) {
                    word = (W_Word *)current_word->value;
                    if (strcmp(word->value, "endif") == 0 && if_count == 0) {
                        end = true;
                        break;
                    } else if (strcmp(word->value, "if") == 0) {
                        if_count++;
                    } else if (strcmp(word->value, "endif") == 0) {
                        if_count--;
                    }
                }
                list_append(else_lines, line);
                current_line = current_line->next;
            }
            if (!end) {
                fprintf(stderr, "Error: Expected keyword 'endif' at end of the else block, line: %d\n", word->line);
                exit(1);
            }
            if (DEBUG) printf("Executing else block...\n");
            void *return_value = execute(else_lines, scope, return_type, false);
            list_destroy_no_free(else_lines);
            if (DEBUG) printf("Else block executed !\n");
            if (return_value != NULL) {
                destroy_stack(stack);
                if (destroy_scope_on_exit) destroy_scope(scope);
                return return_value;
            } //!SECTION - else
        } else if (strcmp(word->value, "endif") == 0) {
            statement = "endif";
        } else if (strcmp(word->value, "infloop") == 0) { //SECTION - infloop
            statement = "infloop";

            list *infloop_lines = list_init();
            int infloop_count = 0;
            bool end = false;
            current_line = current_line->next;
            while (current_line != NULL) {
                line = (list *)current_line->value;
                current_block = line->head;
                if (current_block == NULL) {
                    current_line = current_line->next;
                    continue;
                }
                current_word = ((list *)current_block->value)->head;
                if (current_word != NULL) {
                    word = (W_Word *)current_word->value;
                    if (strcmp(word->value, "infloop") == 0) {
                        infloop_count++;
                    } else if (infloop_count == 0) {
                        if (strcmp(word->value, "endinf") == 0) {
                            end = true;
                            break;
                        }
                    } else infloop_count--;
                }
                list_append(infloop_lines, line);
                current_line = current_line->next;
            }
            if (!end) {
                fprintf(stderr, "Error: Expected keyword 'endinf' at end of the infloop, line: %d\n", word->line);
                exit(1);
            }
            if (DEBUG) printf("Executing infloop...\n");
            while (true) {
                void *return_value = execute(infloop_lines, scope, return_type, false);
                if (return_value != NULL) {
                    if (*(int *)return_value == 1) { // break code is 1
                        w_free(return_value);
                        break;
                    } else if (*(int *)return_value == 2) { // continue code is 2
                        w_free(return_value);
                        continue;
                    } else {
                        destroy_stack(stack);
                        if (destroy_scope_on_exit) destroy_scope(scope);
                        return return_value;
                    }
                }
            }
            list_destroy_no_free(infloop_lines);
            if (DEBUG) printf("infloop executed !\n"); //!SECTION - infloop
        } else if (strcmp(word->value, "endinf") == 0) {
            statement = "endinf";
        } else if (strcmp(word->value, "break") == 0) { //SECTION - break
            statement = "break";

            current_word = current_word->next;
            if (current_word != NULL) {
                fprintf(stderr, "Error: Unexpected value after 'break', line: %d\n", word->line);
                exit(1);
            }
            destroy_stack(stack);
            int *result = (int *)w_malloc(sizeof(int));
            *result = 1; // break code for infloop
            return result; //!SECTION - break
        } else if (strcmp(word->value, "continue") == 0) { //SECTION - continue
            statement = "continue";

            current_word = current_word->next;
            if (current_word != NULL) {
                fprintf(stderr, "Error: Unexpected value after 'continue', line: %d\n", word->line);
                exit(1);
            }
            destroy_stack(stack);
            int *result = (int *)w_malloc(sizeof(int));
            *result = 2; // continue code for infloop
            return result; //!SECTION - continue
        }
        //!SECTION - Control Flow

        /* SECTION - IO */
        else if (strcmp(word->value, "print") == 0) { //SECTION - print
            statement = "print";

            current_word = current_word->next;
            if (current_word == NULL) {
                fprintf(stderr, "Error: Expected value after 'print', line: %d\n", word->line);
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
                            fprintf(stderr, "Error: Expected value after 'sep', line: %d\n", word->line);
                            exit(1);
                        }
                        word = current_word->value;
                        if (word->type != STR) {
                            fprintf(stderr, "Error: Expected str value after 'sep', got '%s', line: %d\n", word->value, word->line);
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
                            fprintf(stderr, "Error: Expected value after 'end', line: %d\n", word->line);
                            exit(1);
                        }
                        word = current_word->value;
                        if (word->type != STR) {
                            fprintf(stderr, "Error: Expected str value after 'end', got '%s', line: %d\n", word->value, word->line);
                            exit(1);
                        }
                        end = word->value;
                        char str[strlen(end)-1];
                        strncpy(str, end+1, strlen(end)-2);
                        str[strlen(end)-2] = '\0';
                        strcpy(end, str);
                    } else {
                        fprintf(stderr, "Error: Unexpected keyword '%s', line: %d\n", word->value, word->line);
                        exit(1);
                    }
                    current_word = current_word->next;
                } else {
                    if (word->type == IDENTIFIER) {
                        W_Var *var = get_var(scope, word->value);
                        if (var == NULL) {
                            fprintf(stderr, "Error: Variable '%s' does not exist, line: %d\n", word->value, word->line);
                            exit(1);
                        }
                        char *str = var->stringify(var);
                        printf("%s", str);
                        w_free(str);
                    } else if (word->type == STR) {
                        char str[strlen(word->value)-1];
                        strncpy(str, word->value+1, strlen(word->value)-2);
                        str[strlen(word->value)-2] = '\0';
                        printf("%s", str);
                    } else if (word->type == NUMBER) {
                        printf("%s", word->value);
                    } else {
                        fprintf(stderr, "Error: Unexpected value '%s', line: %d\n", word->value, word->line);
                        exit(1);
                    }
                    current_word = current_word->next;
                    if (current_word != NULL) {
                        printf("%s", sep);
                    }
                }
            }
            printf("%s", end); //!SECTION - print
        } else if (strcmp(word->value, "ask") == 0) { //SECTION - ask
            statement = "ask";

            current_word = current_word->next;
            if (current_word == NULL) {
                fprintf(stderr, "Error: Expected variable name after 'ask', line: %d\n", word->line);
                exit(1);
            }
            word = current_word->value;
            if (word->type != STR) {
                fprintf(stderr, "Error: Expected str value after 'ask', got '%s', line: %d\n", word->value, word->line);
                exit(1);
            }

            char str[strlen(word->value)-1]; //get the prompt without quotes
            strncpy(str, word->value+1, strlen(word->value)-2);
            str[strlen(word->value)-2] = '\0';
            printf("%s\n", str); //print the prompt
            char input[103]; // get the input, 100 characters max + 1 for the null character + 2 for the quotes
            input[0] = '"';
            fgets(input+1, 100, stdin);
            input[strlen(input)-1] = '"';
            input[strlen(input)] = '\0';

            current_word = current_word->next; //get the keyword 'store'
            if (current_word == NULL) {
                fprintf(stderr, "Error: Expected keyword 'store' after prompt, line: %d\n", word->line);
                exit(1);
            }
            word = current_word->value;
            if (strcmp(word->value, "store") != 0) {
                fprintf(stderr, "Error: Expected keyword 'store' after prompt, got '%s', line: %d\n", word->value, word->line);
                exit(1);
            }

            current_word = current_word->next; //get name of the variable
            if (current_word == NULL) {
                fprintf(stderr, "Error: Expected variable name after 'store', line: %d\n", word->line);
                exit(1);
            }
            word = current_word->value;
            if (word->type != IDENTIFIER) {
                fprintf(stderr, "Error: Expected variable name after 'store', got '%s', line: %d\n", word->value, word->line);
                exit(1);
            }
            if (get_var(scope, word->value) != NULL) {
                fprintf(stderr, "Error: Variable '%s' already exists, line: %d\n", word->value, word->line);
                exit(1);
            }

            W_Str *var = w_str_init();
            w_str_assign(var, input);
            w_dict_set(scope->vars, word->value, var); //!SECTION - ask
        }
        //!SECTION - IO

        /* SECTION - Constructed types */
        else if (strcmp(word->value, "list") == 0) { //SECTION - list
            current_word = current_word->next;
            if (current_word == NULL) {
                fprintf(stderr, "Error: Expected keyword after keyword 'list', line: %d\n", word->line);
                exit(1);
            }
            word = current_word->value;
            if (strcmp(word->value, "create") == 0) { //SECTION - list: create
                statement = "list creation";

                current_word = current_word->next;
                if (current_word == NULL) {
                    fprintf(stderr, "Error: Expected variable name after keyword 'create', line: %d\n", word->line);
                    exit(1);
                }
                word = current_word->value; //get list name
                if (word->type != IDENTIFIER) {
                    fprintf(stderr, "Error: Expected variable name after keyword 'create', got '%s', line: %d\n", word->value, word->line);
                    exit(1);
                }
                if (get_var(scope, word->value) != NULL) {
                    fprintf(stderr, "Error: Variable '%s' already exists, line: %d\n", word->value, word->line);
                    exit(1);
                }
                char *name = word->value;
                W_List *var_list = w_list_init();
                w_dict_set(scope->vars, name, var_list); //!SECTION - list: create
            } else if (strcmp(word->value, "append") == 0) { //SECTION - list: append
                statement = "list append";

                current_word = current_word->next;
                if (current_word == NULL) {
                    fprintf(stderr, "Error: Expected list name after keyword 'append', line: %d\n", word->line);
                    exit(1);
                }
                word = current_word->value;
                if (word->type != IDENTIFIER) {
                    fprintf(stderr, "Error: Expected list name after keyword 'append', got '%s', line: %d\n", word->value, word->line);
                    exit(1);
                }
                W_List *var_list = (W_List *)get_var(scope, word->value);
                if (var_list == NULL) {
                    fprintf(stderr, "Error: List '%s' does not exist, line: %d\n", word->value, word->line);
                    exit(1);
                }
                if (var_list->type != LIST) {
                    char *var_type = w_get_type_str((W_Var *)var_list);
                    fprintf(stderr, "Error: Expected list name after keyword 'append', got '%s' (type: %s), line: %d\n", word->value, var_type, word->line);
                    w_free(var_type);
                    exit(1);
                }
                current_word = current_word->next;
                if (current_word == NULL) {
                    fprintf(stderr, "Error: Expected keyword 'value' after list name, line: %d\n", word->line);
                    exit(1);
                }
                word = current_word->value;
                if (strcmp(word->value, "value") != 0) {
                    fprintf(stderr, "Error: Expected keyword 'value' after list name, got '%s', line: %d\n", word->value, word->line);
                    exit(1);
                }
                current_word = current_word->next;
                if (current_word == NULL) {
                    fprintf(stderr, "Error: Expected value after keyword 'value', line: %d\n", word->line);
                    exit(1);
                }
                word = current_word->value;
                if (word->type != IDENTIFIER && word->type != NUMBER && word->type != STR && strcmp(word->value, "true") != 0 && strcmp(word->value, "false") != 0) {
                    fprintf(stderr, "Error: Expected value after keyword 'value', got '%s', line: %d\n", word->value, word->line);
                    exit(1);
                }
                W_Var *var;
                if (word->type == IDENTIFIER) {
                    var = (W_Var *)get_var(scope, word->value);
                    if (var == NULL) {
                        fprintf(stderr, "Error: Variable '%s' does not exist, line: %d\n", word->value, word->line);
                        exit(1);
                    }
                    var = var->copy(var);
                } else {
                    if (word->type == NUMBER && !is_float(word->value)) {
                        var = (W_Var *)w_int_init();
                    } else if (word->type == NUMBER) {
                        var = (W_Var *)w_float_init();
                    } else if (word->type == STR) {
                        var = (W_Var *)w_str_init();
                    } else if (strcmp(word->value, "true") == 0 || strcmp(word->value, "false") == 0) {
                        var = (W_Var *)w_bool_init();
                    }
                    var->assign(var, word->value);
                }
                w_list_append(var_list, var); //!SECTION - list: append
            } else if (strcmp(word->value, "get") == 0) { //SECTION - list: get
                statement = "list get";

                current_word = current_word->next;
                if (current_word == NULL) {
                    fprintf(stderr, "Error: Expected list name after keyword 'get', line: %d\n", word->line);
                    exit(1);
                }
                word = current_word->value;
                if (word->type != IDENTIFIER) {
                    fprintf(stderr, "Error: Expected list name after keyword 'get', got '%s', line: %d\n", word->value, word->line);
                    exit(1);
                }
                W_List *var_list = (W_List *)get_var(scope, word->value);
                if (var_list == NULL) {
                    fprintf(stderr, "Error: List '%s' does not exist, line: %d\n", word->value, word->line);
                    exit(1);
                }
                if (var_list->type != LIST) {
                    char *var_type = w_get_type_str((W_Var *)var_list);
                    fprintf(stderr, "Error: Expected list name after keyword 'append', got '%s' (type: %s), line: %d\n", word->value, var_type, word->line);
                    w_free(var_type);
                    exit(1);
                }
                current_word = current_word->next;
                if (current_word == NULL) {
                    fprintf(stderr, "Error: Expected keyword 'index' after list name, line: %d\n", word->line);
                    exit(1);
                }
                word = current_word->value;
                if (strcmp(word->value, "index") != 0) {
                    fprintf(stderr, "Error: Expected keyword 'index' after list name, got '%s', line: %d\n", word->value, word->line);
                    exit(1);
                }
                current_word = current_word->next;
                if (current_word == NULL) {
                    fprintf(stderr, "Error: Expected value after keyword 'index', line: %d\n", word->line);
                    exit(1);
                }
                word = current_word->value;
                int index;
                if (word->type == NUMBER) {
                    index = atoi(word->value);
                } else if (word->type == IDENTIFIER) {
                    W_Var *var = (W_Var *)get_var(scope, word->value);
                    if (var == NULL) {
                        fprintf(stderr, "Error: Variable '%s' does not exist, line: %d\n", word->value, word->line);
                        exit(1);
                    }
                    if (var->type != INT) {
                        char *var_type_str = w_get_type_str(var);
                        fprintf(stderr, "Error: Expected int value after keyword 'index', got '%s' (type: %s), line: %d\n", word->value, var_type_str, word->line);
                        w_free(var_type_str);
                        exit(1);
                    }
                    index = *(int *)var->value;
                } else {
                    fprintf(stderr, "Error: Expected int value after keyword 'index', got %s, line: %d\n", word->value, word->line);
                    exit(1);
                }
                if (index < 0 || index >= var_list->size) {
                    fprintf(stderr, "Error: Index out of range, line: %d\n", word->line);
                    exit(1);
                }
                W_Var *var = (W_Var *)w_list_get(var_list, index);
                W_Var *var_copy = var->copy(var);
                current_word = current_word->next;
                if (current_word == NULL) {
                    fprintf(stderr, "Error: Expected keyword 'store' after index, line: %d\n", word->line);
                    exit(1);
                }
                word = current_word->value;
                if (strcmp(word->value, "store") != 0) {
                    fprintf(stderr, "Error: Expected keyword 'store' after index, got '%s', line: %d\n", word->value, word->line);
                    exit(1);
                }  
                current_word = current_word->next;
                if (current_word == NULL) {
                    fprintf(stderr, "Error: Expected variable name after index, line: %d\n", word->line);
                    exit(1);
                }
                word = current_word->value;
                if (word->type != IDENTIFIER) {
                    fprintf(stderr, "Error: Expected variable name after index, got '%s', line: %d\n", word->value, word->line);
                    exit(1);
                }
                if (get_var(scope, word->value) != NULL) {
                    fprintf(stderr, "Error: Variable '%s' already exists, line: %d\n", word->value, word->line);
                    exit(1);
                }
                char *name = word->value;
                w_dict_set(scope->vars, name, var_copy); //!SECTION - list: get
            }
            else {
                fprintf(stderr, "Error: Unexpected keyword '%s', line: %d\n", word->value, word->line);
                exit(1);
            }
            //!SECTION - list
        }
        //!SECTION - Constructed types

        /* SECTION - Variables */
        else if (is_type_keyword(word->value)) { //SECTION - assign
            statement = "variable assignment";

            W_Type type = w_get_type(word->value);
            char *name;
            void *value;
            current_word = current_word->next;
            if (type == NULL_TYPE) { //cannot create null variable
                fprintf(stderr, "Error: Cannot create variable with type 'null', line: %d\n", word->line);
                exit(1);
            }

            value = w_var_init(type);
            if (current_word == NULL) { //get variable name
                fprintf(stderr, "Error: Expected variable name after type keyword, line: %d\n", word->line);
                exit(1);
            }
            word = current_word->value;
            if (word->type != IDENTIFIER) {
                fprintf(stderr, "Error: Expected variable name after type keyword, got '%s', line: %d\n", word->value, word->line);
                exit(1);
            }
            if (get_var(scope, word->value) != NULL) {
                fprintf(stderr, "Error: Variable '%s' already exists, line: %d\n", word->value, word->line);
                exit(1);
            }
            name = word->value;

            current_word = current_word->next; //assign keyword
            if (current_word == NULL) {
                fprintf(stderr, "Error: Expected keyword 'assign' after variable name, line: %d\n", word->line);
                exit(1);
            }
            word = current_word->value;
            if (strcmp(word->value, "assign") != 0) {
                fprintf(stderr, "Error: Expected keyword 'assign' after variable name, got '%s', line: %d\n", word->value, word->line);
                exit(1);
            }

            current_word = current_word->next; //get variable value
            if (current_word == NULL) {
                fprintf(stderr, "Error: Expected value after keyword 'assign', line: %d\n", word->line);
                exit(1);
            }
            word = current_word->value;
            if (word->type != IDENTIFIER && word->type != NUMBER && word->type != STR && strcmp(word->value, "true") != 0 && strcmp(word->value, "false") != 0) {
                fprintf(stderr, "Error: Expected value after keyword 'assign', got '%s', line: %d\n", word->value, word->line);
                exit(1);
            }
            if (word->type == IDENTIFIER) {
                W_Var *var = get_var(scope, word->value);
                if (var == NULL) {
                    fprintf(stderr, "Error: Variable '%s' does not exist, line: %d\n", word->value, word->line);
                    exit(1);
                }
                if (var->type != type) {
                    char *var_type_str = w_get_type_str(var);
                    W_Var dummy;
                    dummy.type = type;
                    char *type_str = w_get_type_str(&dummy);
                    fprintf(stderr, "Error: Expected %s value after keyword 'assign', got '%s' (type: %s), line: %d\n", type_str, word->value, var_type_str, word->line);
                    w_free(var_type_str);
                    w_free(type_str);
                    exit(1);
                }
                value = var->copy(var);
            } else {
                if (type == INT) { //get int value
                    if (word->type != NUMBER) {
                        fprintf(stderr, "Error: Expected int value after keyword 'assign', got %s, line: %d\n", word->value, word->line);
                        exit(1);
                    }
                } else if (type == FLOAT) { //get float value
                    if (word->type != NUMBER) {
                        fprintf(stderr, "Error: Expected float value after keyword 'assign', got %s, line: %d\n", word->value, word->line);
                        exit(1);
                    }
                } else if (type == STR) { //get str value
                    if (word->type != STR) {
                        fprintf(stderr, "Error: Expected str value after keyword 'assign', got %s, line: %d\n", word->value, word->line);
                        exit(1);
                    }
                } else if (type == BOOL) { //get bool value
                    if (strcmp(word->value, "true") != 0 && strcmp(word->value, "false") != 0) {
                        fprintf(stderr, "Error: Expected bool value after keyword 'assign', got %s, line: %d\n", word->value, word->line);
                        exit(1);
                    }
                }
                ((W_Var *)value)->assign(value, word->value);
            }
            w_dict_set(scope->vars, name, value); //!SECTION - assign
        } else if (strcmp(word->value, "change") == 0) { //SECTION - change
            statement = "variable change";

            current_word = current_word->next;
            if (current_word == NULL) {
                fprintf(stderr, "Error: Expected variable name after keyword 'change', line: %d\n", word->line);
                exit(1);
            }
            word = current_word->value;
            if (word->type != IDENTIFIER) {
                fprintf(stderr, "Error: Expected variable name after keyword 'change', got '%s', line: %d\n", word->value, word->line);
                exit(1);
            }
            W_Var *var = w_dict_get(scope->vars, word->value); //get local variable
            if (var == NULL) {
                fprintf(stderr, "Error: Variable '%s' does not exist, line: %d\n", word->value, word->line);
                exit(1);
            }
            W_Type type = var->type;
            current_word = current_word->next;
            if (current_word == NULL) {
                fprintf(stderr, "Error: Expected keyword 'to' after variable name, line: %d\n", word->line);
                exit(1);
            }
            word = current_word->value;
            if (strcmp(word->value, "to") != 0) {
                fprintf(stderr, "Error: Expected keyword 'to' after variable name, got '%s', line: %d\n", word->value, word->line);
                exit(1);
            }
            current_word = current_word->next;
            if (current_word == NULL) {
                fprintf(stderr, "Error: Expected value after keyword 'to', line: %d\n", word->line);
                exit(1);
            }
            word = current_word->value;
            if (word->type != IDENTIFIER && word->type != NUMBER && word->type != STR && strcmp(word->value, "true") != 0 && strcmp(word->value, "false") != 0) {
                fprintf(stderr, "Error: Expected value after keyword 'to', got '%s', line: %d\n", word->value, word->line);
                exit(1);
            }
            if (word->type == IDENTIFIER) {
                W_Var *src_var = get_var(scope, word->value);
                if (src_var == NULL) {
                    fprintf(stderr, "Error: Variable '%s' does not exist, line: %d\n", word->value, word->line);
                    exit(1);
                }
                if (src_var->type != type) {
                    char *src_type_str = w_get_type_str(src_var);
                    char *var_type_str = w_get_type_str(var);
                    fprintf(stderr, "Error: Expected %s value after keyword 'to', got %s (type: %s), line: %d\n", var_type_str, word->value, src_type_str, word->line);
                    w_free(src_type_str);
                    w_free(var_type_str);
                    exit(1);
                } else {
                    char *value = src_var->stringify(src_var);
                    var->assign(var, value);
                    w_free(value);
                }
            } else {
                if (type == INT) { //get int value
                    if (word->type != NUMBER) {
                        fprintf(stderr, "Error: Expected int value after keyword 'to', got %s, line: %d\n", word->value, word->line);
                        exit(1);
                    }
                } else if (type == FLOAT) { //get float value
                    if (word->type != NUMBER) {
                        fprintf(stderr, "Error: Expected float value after keyword 'to', got %s, line: %d\n", word->value, word->line);
                        exit(1);
                    }
                } else if (type == STR) { //get str value
                    if (word->type != STR) {
                        fprintf(stderr, "Error: Expected str value after keyword 'to', got %s, line: %d\n", word->value, word->line);
                        exit(1);
                    }
                } else if (type == BOOL) { //get bool value
                    if (strcmp(word->value, "true") != 0 && strcmp(word->value, "false") != 0) {
                        fprintf(stderr, "Error: Expected bool value after keyword 'to', got %s, line: %d\n", word->value, word->line);
                        exit(1);
                    }
                }
                var->assign(var, word->value);
            }
            //!SECTION - change
        } else if (strcmp(word->value, "delete") == 0) { //SECTION - delete
            statement = "variable deletion";

            current_word = current_word->next;
            if (current_word == NULL) {
                fprintf(stderr, "Error: Expected variable name after keyword 'delete', line: %d\n", word->line);
                exit(1);
            }
            word = current_word->value;
            if (word->type != IDENTIFIER) {
                fprintf(stderr, "Error: Expected variable name after keyword 'delete', got '%s', line: %d\n", word->value, word->line);
                exit(1);
            }
            W_Var *var = get_var(scope, word->value);
            if (var == NULL) {
                fprintf(stderr, "Error: Variable '%s' does not exist, line: %d\n", word->value, word->line);
                exit(1);
            }
            w_dict_remove(scope->vars, word->value); //!SECTION - delete
        }
        //!SECTION - Variables
        if (statement != NULL) {
            if (strcmp(statement, "print") != 0 && strcmp(statement, "function call") != 0 && strcmp(statement, "function definition") != 0) {
                current_word = current_word->next;
                if (current_word != NULL) {
                    fprintf(stderr, "Error: Expected end of line after %s, line: %d\n", statement, word->line);
                    exit(1);
                }
            }
        }
        destroy_stack(stack);
        current_line = current_line->next;
    }
    if (DEBUG) {
        printf("Exit Variables:\n"); //DEBUG
        char *str = w_dict_stringify(scope->vars);
        printf("%s\n", str);
        w_free(str);
    }
    if (DEBUG && destroy_scope_on_exit) {
        printf("Freeing inner scope... (%p)\n", scope->vars); //DEBUG
        char *dict_str = w_dict_stringify(scope->vars);
        printf("%s\n", dict_str);
        w_free(dict_str);
    }
    if (destroy_scope_on_exit) destroy_scope(scope);
    return NULL;
}

/************************************************
 * Utility **************************************
 ************************************************/

/**
 * \brief Evaluates the parsed lines.
 * \param current_block The parsed line to evaluate.
 * \param scope Current scope.
 * \param stack The stack to store the parsed lines after eval.
 */
void eval_parsed_lines(list_element *current_block, Scope *scope, list *stack) {
    if (DEBUG) printf("Evaluating parsed lines...\n"); //DEBUG
    while (current_block != NULL) {
        if (current_block->value == NULL) {
            current_block = current_block->next;
            continue;
        }
        list *parsed_words = (list *)current_block->value;
        list_element *current_word = parsed_words->head;
        if (parsed_words->size == 1) { //not an operation
            list_append(stack, current_word->value); //add to stack
        } else { //if the size of current_block is not 1 then evaluate operation
            int line;
            W_Word_Type return_type = NUMBER;
            list *result = list_init();
            list_element *current_word = parsed_words->head;
            while (current_word != NULL) {
                W_Word *word = (W_Word *)current_word->value;
                line = word->line;
                if (word->type == OPERATOR) {
                    char *without_dot = remove_dot(word);
                    if (strcmp(without_dot, "plus") == 0) { //addition
                        if (result->size < 2) {
                            fprintf(stderr, "Error: Expected 2 values for operator 'plus', line: %d\n", word->line);
                            exit(1);
                        }
                        W_Var *w1 = (W_Var *)list_pop(result);
                        W_Var *w2 = (W_Var *)list_pop(result);
                        list_append(result, w_plus(w2, w1));
                        w1->destroy(w1);
                        w2->destroy(w2);
                        return_type = NUMBER;
                    } else if (strcmp(without_dot, "minus") == 0) { //subtraction
                        if (result->size < 2) {
                            fprintf(stderr, "Error: Expected 2 values for operator 'minus', line: %d\n", word->line);
                            exit(1);
                        }
                        W_Var *w1 = (W_Var *)list_pop(result);
                        W_Var *w2 = (W_Var *)list_pop(result);
                        list_append(result, w_minus(w2, w1));
                        w1->destroy(w1);
                        w2->destroy(w2);
                        return_type = NUMBER;
                    } else if (strcmp(without_dot, "times") == 0) { //multiplication
                        if (result->size < 2) {
                            fprintf(stderr, "Error: Expected 2 values for operator 'time', line: %d\n", word->line);
                            exit(1);
                        }
                        W_Var *w1 = (W_Var *)list_pop(result);
                        W_Var *w2 = (W_Var *)list_pop(result);
                        list_append(result, w_time(w2, w1));
                        w1->destroy(w1);
                        w2->destroy(w2);
                        return_type = NUMBER;
                    } else if (strcmp(without_dot, "div") == 0) { //division
                        if (result->size < 2) {
                            fprintf(stderr, "Error: Expected 2 values for operator 'div', line: %d\n", word->line);
                            exit(1);
                        }
                        W_Var *w1 = (W_Var *)list_pop(result);
                        W_Var *w2 = (W_Var *)list_pop(result);
                        list_append(result, w_div(w2, w1));
                        w1->destroy(w1);
                        w2->destroy(w2);
                        return_type = NUMBER;
                    } else if (strcmp(without_dot, "mod") == 0) { //modulo
                        if (result->size < 2) {
                            fprintf(stderr, "Error: Expected 2 values for operator 'mod', line: %d\n", word->line);
                            exit(1);
                        }
                        W_Var *w1 = (W_Var *)list_pop(result);
                        W_Var *w2 = (W_Var *)list_pop(result);
                        list_append(result, w_mod(w2, w1));
                        w1->destroy(w1);
                        w2->destroy(w2);
                        return_type = NUMBER;
                    } else if (strcmp(without_dot, "ediv") == 0) { //euclidean division
                        if (result->size < 2) {
                            fprintf(stderr, "Error: Expected 2 values for operator 'ediv', line: %d\n", word->line);
                            exit(1);
                        }
                        W_Var *w1 = (W_Var *)list_pop(result);
                        W_Var *w2 = (W_Var *)list_pop(result);
                        list_append(result, w_ediv(w2, w1));
                        w1->destroy(w1);
                        w2->destroy(w2);
                        return_type = NUMBER;
                    } else if (strcmp(without_dot, "power") == 0) { //power
                        if (result->size < 2) {
                            fprintf(stderr, "Error: Expected 2 values for operator 'pow', line: %d\n", word->line);
                            exit(1);
                        }
                        W_Var *w1 = (W_Var *)list_pop(result);
                        W_Var *w2 = (W_Var *)list_pop(result);
                        list_append(result, w_power(w2, w1));
                        w1->destroy(w1);
                        w2->destroy(w2);
                        return_type = NUMBER;
                    } else if (strcmp(without_dot, "sqrt") == 0) { //square root
                        if (result->size < 1) {
                            fprintf(stderr, "Error: Expected 1 value for operator 'sqrt', line: %d\n", word->line);
                            exit(1);
                        }
                        W_Var *w = (W_Var *)list_pop(result);
                        list_append(result, w_sqrt(w));
                        w->destroy(w);
                        return_type = NUMBER;
                    } else if (strcmp(without_dot, "and") == 0) { //and
                        if (result->size < 2) {
                            fprintf(stderr, "Error: Expected 2 values for operator 'and', line: %d\n", word->line);
                            exit(1);
                        }
                        W_Var *w1 = (W_Var *)list_pop(result);
                        W_Var *w2 = (W_Var *)list_pop(result);
                        list_append(result, w_and(w2, w1));
                        w1->destroy(w1);
                        w2->destroy(w2);
                        return_type = IDENTIFIER;
                    } else if (strcmp(without_dot, "or") == 0) { //or
                        if (result->size < 2) {
                            fprintf(stderr, "Error: Expected 2 values for operator 'or', line: %d\n", word->line);
                            exit(1);
                        }
                        W_Var *w1 = (W_Var *)list_pop(result);
                        W_Var *w2 = (W_Var *)list_pop(result);
                        list_append(result, w_or(w2, w1));
                        w1->destroy(w1);
                        w2->destroy(w2);
                        return_type = IDENTIFIER;
                    } else if (strcmp(without_dot, "not") == 0) { //not
                        if (result->size < 1) {
                            fprintf(stderr, "Error: Expected 1 value for operator 'not', line: %d\n", word->line);
                            exit(1);
                        }
                        W_Var *w = (W_Var *)list_pop(result);
                        list_append(result, w_not(w));
                        w->destroy(w);
                        return_type = IDENTIFIER;
                    } else if (strcmp(without_dot, "equal") == 0) { //equal
                        if (result->size < 2) {
                            fprintf(stderr, "Error: Expected 2 values for operator 'equal', line: %d\n", word->line);
                            exit(1);
                        }
                        W_Var *w1 = (W_Var *)list_pop(result);
                        W_Var *w2 = (W_Var *)list_pop(result);
                        list_append(result, w_equal(w2, w1));
                        w1->destroy(w1);
                        w2->destroy(w2);
                        return_type = IDENTIFIER;
                    } else if (strcmp(without_dot, "greater") == 0) { //greater
                        if (result->size < 2) {
                            fprintf(stderr, "Error: Expected 2 values for operator 'greater', line: %d\n", word->line);
                            exit(1);
                        }
                        W_Var *w1 = (W_Var *)list_pop(result);
                        W_Var *w2 = (W_Var *)list_pop(result);
                        list_append(result, w_greater(w2, w1));
                        w1->destroy(w1);
                        w2->destroy(w2);
                        return_type = IDENTIFIER;
                    } else if (strcmp(without_dot, "less") == 0) { //less
                        if (result->size < 2) {
                            fprintf(stderr, "Error: Expected 2 values for operator 'less', line: %d\n", word->line);
                            exit(1);
                        }
                        W_Var *w1 = (W_Var *)list_pop(result);
                        W_Var *w2 = (W_Var *)list_pop(result);
                        list_append(result, w_less(w2, w1));
                        w1->destroy(w1);
                        w2->destroy(w2);
                        return_type = IDENTIFIER;
                    } else if (strcmp(without_dot, "gequal") == 0) { //greater or equal
                        if (result->size < 2) {
                            fprintf(stderr, "Error: Expected 2 values for operator 'gequal', line: %d\n", word->line);
                            exit(1);
                        }
                        W_Var *w1 = (W_Var *)list_pop(result);
                        W_Var *w2 = (W_Var *)list_pop(result);
                        list_append(result, w_gequal(w2, w1));
                        w1->destroy(w1);
                        w2->destroy(w2);
                        return_type = IDENTIFIER;
                    } else if (strcmp(without_dot, "lequal") == 0) { //less or equal
                        if (result->size < 2) {
                            fprintf(stderr, "Error: Expected 2 values for operator 'lequal', line: %d\n", word->line);
                            exit(1);
                        }
                        W_Var *w1 = (W_Var *)list_pop(result);
                        W_Var *w2 = (W_Var *)list_pop(result);
                        list_append(result, w_lequal(w2, w1));
                        w1->destroy(w1);
                        w2->destroy(w2);
                        return_type = IDENTIFIER;
                    }
                    w_free(without_dot);
                } else if (word->type == NUMBER) { //if number
                    if (is_float(word->value)) {
                        W_Float *w = w_var_init(FLOAT);
                        w->assign(w, word->value);
                        list_append(result, w);
                    } else {
                        W_Int *w = w_var_init(INT);
                        w->assign(w, word->value);
                        list_append(result, w);
                    }
                } else if (word->type == IDENTIFIER) { //if variable
                    W_Var *w = get_var(scope, word->value);
                    if (w == NULL) {
                        fprintf(stderr, "Error: Variable '%s' does not exist, line: %d\n", word->value, word->line);
                        exit(1);
                    }
                    w = w->copy(w);
                    list_append(result, w);
                } else {
                    fprintf(stderr, "Error: Unexpected word '%s', line: %d\n", word->value, word->line);
                    exit(1);
                }
                current_word = current_word->next;
            }
            if (result->size > 1) {
                fprintf(stderr, "Error: Incorrect number of operands, line: %d\n", line);
                exit(1);
            }
            W_Var *result_var = (W_Var *)list_pop(result);
            if (DEBUG) printf("Creating result word...\n"); //DEBUG
            char *str = result_var->stringify(result_var);
            W_Word *result_word = (W_Word *)w_malloc(sizeof(W_Word));
            result_word->type = return_type;
            result_word->value = str;
            result_word->line = line;
            result_word->is_generated = true;
            result_var->destroy(result_var);
            list_destroy(result);
            if (DEBUG) printf("Result created\n"); //DEBUG
            list_append(stack, result_word);
        }
        current_block = current_block->next;
    }
    if (DEBUG) printf("Evaluated parsed lines\n"); //DEBUG
}

/**
 * \brief Destroys the stack and generated words only.
 * \param stack The stack to destroy.
 */
void destroy_stack(list *stack) {
    if (DEBUG) printf("Destroying stack...\n"); //DEBUG
    list_element *current = stack->head;
    while (current != NULL) {
        W_Word *word = (W_Word *)current->value;
        list_element *next = current->next;
        if (word->is_generated) {
            w_free(word->value);
            w_free(word);
        }
        w_free(current);
        current = next;
    }
    w_free(stack);
    if (DEBUG) printf("Stack destroyed\n"); //DEBUG
}

/**
 * \brief Removes priority dots of operations from the given word.
 * \param word The word to remove the dot from.
 * \return The word without the dot.
 */
char *remove_dot(W_Word *word) {
    int priority = 0;
    for (int i = 0; i < strlen(word->value); i++) {
        if (word->value[i] == '.') {
            priority++;
        } else break;
    }
    char *without_dot = (char *)w_malloc(strlen(word->value) - priority + 1);
    strncpy(without_dot, word->value + priority, strlen(word->value) - priority);
    without_dot[strlen(word->value) - priority] = '\0';
    return without_dot;
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