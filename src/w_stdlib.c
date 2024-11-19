#include "w_stdlib.h"

/***********************************************
 * Utility *************************************
 ***********************************************/

/**
 * \brief Get the type of a variable as a string (w_malloc)
 * \param var_type The variable to get the type of
 * \return The type of the variable
 */
static char *get_type_str(W_Type var_type) {
    char *type = (char *)w_malloc(9);
    switch (var_type) {
        case INT:
            strcpy(type, "int");
            break;
        case FLOAT:
            strcpy(type, "float");
            break;
        case STRING:
            strcpy(type, "str");
            break;
        case BOOL:
            strcpy(type, "bool");
            break;
        case LIST:
            strcpy(type, "list");
            break;
        case DICT:
            strcpy(type, "dict");
            break;
        case FUNCTION:
            strcpy(type, "function");
            break;
        case NULL_TYPE:
            strcpy(type, "null");
            break;
        default:
            strcpy(type, "unknown");
            break;
    }
    return type;
}

/**
 * \brief Get the type of a variable from a string
 * \param var The variable to get the type of
 * \return The type of the variable
 */
static W_Type get_type(char *str) {
    if (strcmp(str, "int") == 0) {
        return INT;
    } else if (strcmp(str, "float") == 0) {
        return FLOAT;
    } else if (strcmp(str, "str") == 0) {
        return STRING;
    } else if (strcmp(str, "bool") == 0) {
        return BOOL;
    } else if (strcmp(str, "list") == 0) {
        return LIST;
    } else if (strcmp(str, "dict") == 0) {
        return DICT;
    } else if (strcmp(str, "function") == 0) {
        return FUNCTION;
    } else {
        return NULL_TYPE;
    }
}

/**
 * \brief Checks if the given string is a float.
 * \param str The string to check.
 * \return True if the string is a float, false otherwise.
 */
static bool is_float(char *str) {
    int len = strlen(str);
    for (int i = 0; i < len; i++) {
        if (str[i] == '.') return true;
    }
    return false;
}

static float get_number(Scope *scope, W_Word *word, int line) { //helper function for math operations
    float res;
    if (word->type == NUMBER) {
        res = atof(word->value);
    } else if (word->type == IDENTIFIER) {
        W_Var *var = w_dict_get(scope->vars, word->value);
        if (var == NULL) {
            printf("Error: Variable '%s' not found, line %d\n", word->value, line);
            exit(1);
        }
        if (var->type != INT && var->type != FLOAT) {
            printf("Error: Unsupported type (Expected int or float, got %s), line %d\n", get_type_str(var->type), line);
            exit(1);
        }
        res = *(float *)var->value;
    }
    return res;
}

static bool get_bool(Scope *scope, W_Word *word, int line) { //helper function for w_and, w_or, w_not
    bool res;
    if (word->type == IDENTIFIER) {
        W_Var *var = w_dict_get(scope->vars, word->value);
        if (var == NULL) {
            printf("Error: Variable '%s' not found, line %d\n", word->value, line);
            exit(1);
        }
        if (var->type != BOOL) {
            printf("Error: Unsupported type (Expected bool, got %s), line %d\n", get_type_str(var->type), line);
            exit(1);
        }
        res = *(bool *)var->value;
    } else {
        printf("Error: Expected variable or boolean expression, got %s, line %d", word->value, line);
    }
    return res;
}

/***********************************************
 * Execution ***********************************
 ***********************************************/

/**
 * \brief Destroys the stack and generated words only.
 * \param stack The stack to destroy.
 */
static void destroy_stack(list *stack) {
    if (DEBUG) printf("Destroying stack...\n");
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
    if (DEBUG) printf("Stack destroyed\n");
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
    void *exec_result = NULL;

    list_element *current_line = parsed_code->head;
    while (current_line != NULL) { //iterate through each line
        list *line = (list *)current_line->value;

        list *stack = list_init();
        list_element *current_word = line->head;

        while (current_word != NULL) { //iterate through each word
            if (DEBUG) {
                for (int i = 0; i < stack->size; i++) {
                    W_Word *word = (W_Word *)list_get(stack, i);
                    printf("Stack[%d]: %s\n", i, word->value);
                }
            }

            W_Word *word = (W_Word *)current_word->value;

            if (word->type != KEYWORD) { //if the word is not a keyword, push it to the stack
                list_append(stack, word);
            } else {
                keyword *kw = dict_get(keywords, word->value); //get the keyword struct
                if (kw == NULL) {
                    fprintf(stderr, "Error: Unknown keyword '%s'\n", word->value);
                    exit(1);
                }

                list *args = list_init(); //get args from the stack to pass to the keyword
                if (kw->argc != -1) {
                    for (int i = 0; i < kw->argc; i++) {
                        if (stack->size == 0) {
                            fprintf(stderr, "Error: Not enough arguments for keyword '%s'\n", word->value);
                            exit(1);
                        }
                        if (strcmp(((W_Word *)list_get(stack, stack->size - 1))->value, "endwith") == 0) { //if the keyword is 'endwith', get the args until 'with' (from last to first)
                            list_pop(stack);
                            bool found_with = false;
                            while (stack->size > 0) {
                                W_Word *arg = list_pop(stack);
                                list_append(args, arg);
                                if (strcmp(arg->value, "with") == 0) {
                                    found_with = true;
                                    break;
                                }
                            }
                            if (!found_with) {
                                fprintf(stderr, "Error: Expected keyword 'with' before 'endwith', line %d\n", word->line);
                                exit(1);
                            }
                        } else {
                            W_Word *arg = list_pop(stack);
                            list_append(args, arg);
                        }
                    }
                } else {
                    while (stack->size > 0) {
                        list_append(args, list_pop(stack));
                    }
                }
                list_reverse(args); //reverse the args to get them in the right order
                W_Word *result = kw->func(scope, args, word->line, &current_line, return_type, exec_result); //execute the keyword function
                if (result != NULL) list_append(stack, result); //store the result on the stack if it is not NULL

                for (int i = 0; i < args->size; i++) { //destroy the args
                    W_Word *arg = list_get(args, i);
                    if (arg->is_generated) {
                        w_free(arg->value);
                        w_free(arg);
                    }
                }
                list_destroy_no_free(args);
                if (exec_result != NULL) break;
            }

            current_word = current_word->next;
        }

        destroy_stack(stack);
        if (exec_result != NULL) break;
        current_line = current_line->next;
    }

    if (destroy_scope_on_exit) destroy_scope(scope);
    printf("%d\n", exec_result==NULL);
    return exec_result;
}

/***********************************************
 * Math Operations *****************************
 ***********************************************/

/**
 * \brief Add two variables (w_malloc)
 * \param scope The scope to add the variables in
 * \param args The arguments to call the function with (a, b)
 * \param line The line of the code
 * \param current_line Unused
 * \param return_type Unused
 * \param return_value Unused
 * \return The result of the addition
 */
W_Word *kw_plus(Scope *scope, list *args, int line, list_element **current_line, W_Type return_type, void *return_value) {
    if (DEBUG) printf("[DEBUG]: kw_plus called\n");
    if (list_size(args) != 2) {
        printf("Error: Invalid number of arguments (Expected 2, got %d), line %d\n", list_size(args), line);
        exit(1);
    }
    float a, b; //get a and b
    W_Word *a_word = list_get(args, 0), *b_word = list_get(args, 1);
    
    a = get_number(scope, a_word, line);
    b = get_number(scope, b_word, line);

    float result;
    result = a + b; //add a and b
    int len = snprintf(NULL, 0, "%g", result); //convert result to string
    char *str = (char *)w_malloc(len + 1);
    sprintf(str, "%g", result);
    W_Word *result_word = (W_Word *)w_malloc(sizeof(W_Word)); //create result word
    result_word->type = NUMBER;
    result_word->value = str;
    result_word->is_generated = true;
    result_word->line = line;

    if (DEBUG) printf("[DEBUG]: kw_plus done\n");
    return result_word;
}

/**
 * \brief Subtract two variables (w_malloc)
 * \param scope The scope to subtract the variables in
 * \param args The arguments to call the function with (a, b)
 * \param line The line of the code
 * \param current_line Unused
 * \param return_type Unused
 * \param return_value Unused
 * \return The result of the subtraction
 */
W_Word *kw_minus(Scope *scope, list *args, int line, list_element **current_line, W_Type return_type, void *return_value) {
    if (DEBUG) printf("[DEBUG]: kw_minus called\n");
    if (list_size(args) != 2) {
        printf("Error: Invalid number of arguments (Expected 2, got %d), line %d\n", list_size(args), line);
        exit(1);
    }
    float a, b; //get a and b
    W_Word *a_word = list_get(args, 0), *b_word = list_get(args, 1);
    
    a = get_number(scope, a_word, line);
    b = get_number(scope, b_word, line);

    float result;
    result = a - b; //substract a and b
    int len = snprintf(NULL, 0, "%g", result); //convert result to string
    char *str = (char *)w_malloc(len + 1);
    sprintf(str, "%g", result);
    W_Word *result_word = (W_Word *)w_malloc(sizeof(W_Word)); //create result word
    result_word->type = NUMBER;
    result_word->value = str;
    result_word->is_generated = true;
    result_word->line = line;

    if (DEBUG) printf("[DEBUG]: kw_minus done\n");
    return result_word;
}

/**
 * \brief Multiply two variables (w_malloc)
 * \param scope The scope to multiply the variables in
 * \param args The arguments to call the function with (a, b)
 * \param line The line of the code
 * \param current_line Unused
 * \param return_type Unused
 * \param return_value Unused
 * \return The result of the multiplication
 */
W_Word *kw_time(Scope *scope, list *args, int line, list_element **current_line, W_Type return_type, void *return_value) {
    if (DEBUG) printf("[DEBUG]: kw_time called\n");
    if (list_size(args) != 2) {
        printf("Error: Invalid number of arguments (Expected 2, got %d), line %d\n", list_size(args), line);
        exit(1);
    }
    float a, b; //get a and b
    W_Word *a_word = list_get(args, 0), *b_word = list_get(args, 1);
    
    a = get_number(scope, a_word, line);
    b = get_number(scope, b_word, line);

    float result;
    result = a * b; //multiply a and b
    int len = snprintf(NULL, 0, "%g", result); //convert result to string
    char *str = (char *)w_malloc(len + 1);
    sprintf(str, "%g", result);
    W_Word *result_word = (W_Word *)w_malloc(sizeof(W_Word)); //create result word
    result_word->type = NUMBER;
    result_word->value = str;
    result_word->is_generated = true;
    result_word->line = line;

    if (DEBUG) printf("[DEBUG]: kw_time done\n");
    return result_word;
}

/**
 * \brief Divide two variables (w_malloc)
 * \param scope The scope to divide the variables in
 * \param args The arguments to call the function with (a, b)
 * \param line The line of the code
 * \param current_line Unused
 * \param return_type Unused
 * \param return_value Unused
 * \return The result of the division
 */
W_Word *kw_div(Scope *scope, list *args, int line, list_element **current_line, W_Type return_type, void *return_value) {
    if (DEBUG) printf("[DEBUG]: kw_div called\n");
    if (list_size(args) != 2) {
        printf("Error: Invalid number of arguments (Expected 2, got %d), line %d\n", list_size(args), line);
        exit(1);
    }
    float a, b; //get a and b
    W_Word *a_word = list_get(args, 0), *b_word = list_get(args, 1);
    a = get_number(scope, a_word, line);
    b = get_number(scope, b_word, line);

    float result;
    result = a / b; //divide a and b
    int len = snprintf(NULL, 0, "%g", result); //convert result to string
    char *str = (char *)w_malloc(len + 1);
    sprintf(str, "%g", result);
    W_Word *result_word = (W_Word *)w_malloc(sizeof(W_Word)); //create result word
    result_word->type = NUMBER;
    result_word->value = str;
    result_word->is_generated = true;
    result_word->line = line;

    if (DEBUG) printf("[DEBUG]: kw_div done\n");
    return result_word;
}

/**
 * \brief Modulo two variables (w_malloc)
 * \param scope The scope to modulo the variables in
 * \param args The arguments to call the function with (a, b)
 * \param line The line of the code
 * \param current_line Unused
 * \param return_type Unused
 * \param return_value Unused
 * \return The result of the modulo
 */
W_Word *kw_mod(Scope *scope, list *args, int line, list_element **current_line, W_Type return_type, void *return_value) {
    if (DEBUG) printf("[DEBUG]: kw_mod called\n");
    if (list_size(args) != 2) {
        printf("Error: Invalid number of arguments (Expected 2, got %d), line %d\n", list_size(args), line);
        exit(1);
    }
    float a, b; //get a and b
    W_Word *a_word = list_get(args, 0), *b_word = list_get(args, 1);
    if ((is_float(a_word->value) && a_word->type != IDENTIFIER) || (is_float(b_word->value) && b_word->type != IDENTIFIER)) {
        printf("Error: Unsupported type (Expected int), line %d\n", line);
        exit(1);
    }

    a = get_number(scope, a_word, line);
    b = get_number(scope, b_word, line);

    float result;
    result = fmod(a, b); //modulo a and b  
    int len = snprintf(NULL, 0, "%g", result); //convert result to string
    char *str = (char *)w_malloc(len + 1);
    sprintf(str, "%g", result);
    W_Word *result_word = (W_Word *)w_malloc(sizeof(W_Word)); //create result word
    result_word->type = NUMBER;
    result_word->value = str;
    result_word->is_generated = true;
    result_word->line = line;

    if (DEBUG) printf("[DEBUG]: kw_mod done\n");
    return result_word;
}

/**
 * \brief Euclidean division two variables (w_malloc)
 * \param scope The scope to euclidean division the variables in
 * \param args The arguments to call the function with (a, b)
 * \param line The line of the code
 * \param current_line Unused
 * \param return_type Unused
 * \param return_value Unused
 * \return The result of the integer division
 */
W_Word *kw_ediv(Scope *scope, list *args, int line, list_element **current_line, W_Type return_type, void *return_value) {
    if (DEBUG) printf("[DEBUG]: kw_ediv called\n");
    if (list_size(args) != 2) {
        printf("Error: Invalid number of arguments (Expected 2, got %d), line %d\n", list_size(args), line);
        exit(1);
    }
    float a, b; //get a and b
    W_Word *a_word = list_get(args, 0), *b_word = list_get(args, 1);
    if ((is_float(a_word->value) && a_word->type != IDENTIFIER) || (is_float(b_word->value) && b_word->type != IDENTIFIER)) {
        printf("Error: Unsupported type (Expected int), line %d\n", line);
        exit(1);
    }
    
    a = get_number(scope, a_word, line);
    b = get_number(scope, b_word, line);

    float result;
    result = (int)a / (int)b; //euclidean division a and b
    int len = snprintf(NULL, 0, "%g", result); //convert result to string
    char *str = (char *)w_malloc(len + 1);
    sprintf(str, "%g", result);
    W_Word *result_word = (W_Word *)w_malloc(sizeof(W_Word)); //create result word
    result_word->type = NUMBER;
    result_word->value = str;
    result_word->is_generated = true;
    result_word->line = line;

    if (DEBUG) printf("[DEBUG]: kw_ediv done\n");
    return result_word;
}

/**
 * \brief Raise a variable to the power of another variable (w_malloc)
 * \param scope The scope to raise the variables in
 * \param args The arguments to call the function with (a, b)
 * \param line The line of the code
 * \param current_line Unused
 * \param return_type Unused
 * \param return_value Unused
 * \return The result of the power operation
 */
W_Word *kw_power(Scope *scope, list *args, int line, list_element **current_line, W_Type return_type, void *return_value) {
    if (DEBUG) printf("[DEBUG]: kw_power called\n");
    if (list_size(args) != 2) {
        printf("Error: Invalid number of arguments (Expected 2, got %d), line %d\n", list_size(args), line);
        exit(1);
    }
    float a, b; //get a and b
    W_Word *a_word = list_get(args, 0), *b_word = list_get(args, 1);
    
    a = get_number(scope, a_word, line);
    b = get_number(scope, b_word, line);

    float result;
    result = pow(a, b); //power a and b
    int len = snprintf(NULL, 0, "%g", result); //convert result to string
    char *str = (char *)w_malloc(len + 1);
    sprintf(str, "%g", result);
    W_Word *result_word = (W_Word *)w_malloc(sizeof(W_Word)); //create result word
    result_word->type = NUMBER;
    result_word->value = str;
    result_word->is_generated = true;
    result_word->line = line;

    if (DEBUG) printf("[DEBUG]: kw_power done\n");
    return result_word;
}

/**
 * \brief Square root a variable (w_malloc)
 * \param scope The scope to square root the variable in
 * \param args The arguments to call the function with (a)
 * \param line The line of the code
 * \param current_line Unused
 * \param return_type Unused
 * \param return_value Unused
 * \return The result of the square root operation
 */
W_Word *kw_sqrt(Scope *scope, list *args, int line, list_element **current_line, W_Type return_type, void *return_value) {
    if (DEBUG) printf("[DEBUG]: kw_sqrt called\n");
    if (list_size(args) != 1) {
        printf("Error: Invalid number of arguments (Expected 1, got %d), line %d\n", list_size(args), line);
        exit(1);
    }
    float a; //get a
    W_Word *a_word = list_get(args, 0);
    
    a = get_number(scope, a_word, line);

    float result;
    result = sqrtf(a); //square root a
    int len = snprintf(NULL, 0, "%g", result); //convert result to string
    char *str = (char *)w_malloc(len + 1);
    sprintf(str, "%g", result);
    W_Word *result_word = (W_Word *)w_malloc(sizeof(W_Word)); //create result word
    result_word->type = NUMBER;
    result_word->value = str;
    result_word->is_generated = true;
    result_word->line = line;

    if (DEBUG) printf("[DEBUG]: kw_sqrt done\n");
    return result_word;
}

/***********************************************
 * Logical Operations **************************
 ***********************************************/

/**
 * \brief Logical AND two variables (w_malloc)
 * \param scope The scope to AND the variables in
 * \param args The arguments to call the function with (a, b)
 * \param line The line of the code
 * \param current_line Unused
 * \param return_type Unused
 * \param return_value Unused
 * \return The result of the AND operation
 */
W_Word *kw_and(Scope *scope, list *args, int line, list_element **current_line, W_Type return_type, void *return_value) {
    if (DEBUG) printf("[DEBUG]: kw_and called\n");
    if (list_size(args) != 2) {
        printf("Error: Invalid number of arguments (Expected 2, got %d), line %d\n", list_size(args), line);
        exit(1);
    }

    bool a, b; //get a and b
    W_Word *a_word = list_get(args, 0), *b_word = list_get(args, 1);
    a = get_bool(scope, a_word, line);
    b = get_bool(scope, b_word, line);

    char *str = (char *)w_malloc(6); //AND a and b
    strcpy(str, a && b ? "true" : "false");
    W_Word *result_word = (W_Word *)w_malloc(sizeof(W_Word)); //create result word
    result_word->type = IDENTIFIER;
    result_word->value = str;
    result_word->is_generated = true;
    result_word->line = line;

    if (DEBUG) printf("[DEBUG]: kw_and done\n");
    return result_word;
}

/**
 * \brief Logical OR two variables (w_malloc)
 * \param scope The scope to OR the variables in
 * \param args The arguments to call the function with (a, b)
 * \param line The line of the code
 * \param current_line Unused
 * \param return_type Unused
 * \param return_value Unused
 * \return The result of the OR operation
 */
W_Word *kw_or(Scope *scope, list *args, int line, list_element **current_line, W_Type return_type, void *return_value) {
    if (DEBUG) printf("[DEBUG]: kw_or called\n");
    if (list_size(args) != 2) {
        printf("Error: Invalid number of arguments (Expected 2, got %d), line %d\n", list_size(args), line);
        exit(1);
    }

    bool a, b; //get a and b
    W_Word *a_word = list_get(args, 0), *b_word = list_get(args, 1);
    a = get_bool(scope, a_word, line);
    b = get_bool(scope, b_word, line);

    char *str = (char *)w_malloc(6); //OR a and b
    strcpy(str, a || b ? "true" : "false");
    W_Word *result_word = (W_Word *)w_malloc(sizeof(W_Word)); //create result word
    result_word->type = IDENTIFIER;
    result_word->value = str;
    result_word->is_generated = true;
    result_word->line = line;

    if (DEBUG) printf("[DEBUG]: kw_or done\n");
    return result_word;
}

/**
 * \brief Logical NOT a variable (w_malloc)
 * \param scope The scope to NOT the variable in
 * \param args The arguments to call the function with (a)
 * \param line The line of the code
 * \param current_line Unused
 * \param return_type Unused
 * \param return_value Unused
 * \return The result of the NOT operation
 */
W_Word *kw_not(Scope *scope, list *args, int line, list_element **current_line, W_Type return_type, void *return_value) {
    if (DEBUG) printf("[DEBUG]: kw_not called\n");
    if (list_size(args) != 1) {
        printf("Error: Invalid number of arguments (Expected 1, got %d), line %d\n", list_size(args), line);
        exit(1);
    }

    bool a; //get a
    W_Word *a_word = list_get(args, 0);
    a = get_bool(scope, a_word, line);

    char *str = (char *)w_malloc(6); //NOT a
    strcpy(str, a ? "false" : "true");
    W_Word *result_word = (W_Word *)w_malloc(sizeof(W_Word)); //create result word
    result_word->type = IDENTIFIER;
    result_word->value = str;
    result_word->is_generated = true;
    result_word->line = line;

    if (DEBUG) printf("[DEBUG]: kw_not done\n");
    return result_word;
}

/***********************************************
 * Comparison Operations ***********************
 ***********************************************/

static bool _equal(W_Var *a, W_Var *b) { //helper function for w_equal
    if (a->type != b->type) {
        return false;
    } else {
        switch (a->type) {
            case INT:
                return *(float *)a->value == *(float *)b->value;
            case FLOAT:
                return *(float *)a->value == *(float *)b->value;
            case STRING:
                return strcmp((char *)a->value, (char *)b->value) == 0;
            case BOOL:
                return *(bool *)a->value == *(bool *)b->value;
            case LIST:
                if (w_list_size((W_List *)a) != w_list_size((W_List *)b)) {
                    return false;
                } else {
                    W_List_Element *e1 = ((W_List *)a)->head, *e2 = ((W_List *)b)->head;
                    for (int i = 0; i < w_list_size((W_List *)a); i++) {
                        if (!_equal(e1->value, e2->value)) { //recursion
                            return false;
                        }
                        e1 = e1->next;
                        e2 = e2->next;
                    }
                    return true;
                }
            default:
                return false;
        }
    }
}

/**
 * \brief Compare two variables for equality (w_malloc)
 * \param scope The scope to compare the variables in
 * \param args The arguments to call the function with (a, b)
 * \param line The line of the code
 * \param current_line Unused
 * \param return_type Unused
 * \param return_value Unused
 * \return The result of the comparison
 */
W_Word *kw_equal(Scope *scope, list *args, int line, list_element **current_line, W_Type return_type, void *return_value) {
    if (DEBUG) printf("[DEBUG]: kw_equal called\n");
    if (list_size(args) != 2) {
        printf("Error: Invalid number of arguments (Expected 2, got %d), line %d\n", list_size(args), line);
        exit(1);
    }

    W_Var *a, *b; //get a and b
    W_Word *a_word = list_get(args, 0), *b_word = list_get(args, 1);

    void (*destroy_a)(void *) = NULL, (*destroy_b)(void *) = NULL;
    if (a_word->type == IDENTIFIER) { //initialize a
        a = w_dict_get(scope->vars, a_word->value);
        if (a == NULL) {
            printf("Error: Variable '%s' not found, line %d\n", a_word->value, line);
            exit(1);
        }
    } else if (a_word->type == NUMBER) {
        if (is_float(a_word->value)) {
            a = (W_Var *)w_float_init();
            w_float_assign((W_Float *)a, a_word->value);
            destroy_a = (void (*)(void *))w_float_destroy;
        } else {
            a = (W_Var *)w_int_init();
            w_int_assign((W_Int *)a, a_word->value);
            destroy_a = (void (*)(void *))w_int_destroy;
        }
    } else if (a_word->type == LITT_STR) {
        a = (W_Var *)w_str_init();
        w_str_assign((W_Str *)a, a_word->value);
        destroy_a = (void (*)(void *))w_str_destroy;
    }

    if (b_word->type == IDENTIFIER) { //initialize b
        b = (W_Var *)w_dict_get(scope->vars, b_word->value);
        if (b == NULL) {
            printf("Error: Variable '%s' not found, line %d\n", b_word->value, line);
            exit(1);
        }
    } else if (b_word->type == NUMBER) {
        if (is_float(b_word->value)) {
            b = (W_Var *)w_float_init();
            w_float_assign((W_Float *)b, b_word->value);
            destroy_b = (void (*)(void *))w_float_destroy;
        } else {
            b = (W_Var *)w_int_init();
            w_int_assign((W_Int *)b, b_word->value);
            destroy_b = (void (*)(void *))w_int_destroy;
        }
    } else if (b_word->type == LITT_STR) {
        b = (W_Var *)w_str_init();
        w_str_assign((W_Str *)a, a_word->value);
        destroy_b = (void (*)(void *))w_str_destroy;
    }

    char *result = (char *)w_malloc(6); //compare a and b
    if (a->type != b->type) {
        strcpy(result, "false");
    } else {
        _equal(a, b) ? strcpy(result, "true") : strcpy(result, "false");
    }

    if (destroy_a != NULL) destroy_a(a);
    if (destroy_b != NULL) destroy_b(b);

    W_Word *result_word = (W_Word *)w_malloc(sizeof(W_Word)); //create result word
    result_word->type = IDENTIFIER;
    result_word->value = result;
    result_word->is_generated = true;
    result_word->line = line;

    if (DEBUG) printf("[DEBUG]: kw_equal done\n");
    return result_word;
}

/**
 * \brief Compare two variables for greater than (w_malloc)
 * \param scope The scope to compare the variables in
 * \param args The arguments to call the function with (a, b)
 * \param line The line of the code
 * \param current_line Unused
 * \param return_type Unused
 * \param return_value Unused
 * \return The result of the comparison
 */
W_Word *kw_greater(Scope *scope, list *args, int line, list_element **current_line, W_Type return_type, void *return_value) {
    if (DEBUG) printf("[DEBUG]: kw_greater called\n");
    if (list_size(args) != 2) {
        printf("Error: Invalid number of arguments (Expected 2, got %d), line %d\n", list_size(args), line);
        exit(1);
    }
    float a, b; //get a and b
    W_Word *a_word = list_get(args, 0), *b_word = list_get(args, 1);

    a = get_number(scope, a_word, line);
    b = get_number(scope, b_word, line);

    char *result = (char *)w_malloc(6); //compare a and b
    a > b ? strcpy(result, "true") : strcpy(result, "false");

    W_Word *result_word = (W_Word *)w_malloc(sizeof(W_Word)); //create result word
    result_word->type = IDENTIFIER;
    result_word->value = result;
    result_word->is_generated = true;
    result_word->line = line;

    if (DEBUG) printf("[DEBUG]: kw_greater done\n");
    return result_word;
}

/**
 * \brief Compare two variables for less than (w_malloc)
 * \param scope The scope to compare the variables in
 * \param args The arguments to call the function with (a, b)
 * \param line The line of the code
 * \param current_line Unused
 * \param return_type Unused
 * \param return_value Unused
 * \return The result of the comparison
 */
W_Word *kw_less(Scope *scope, list *args, int line, list_element **current_line, W_Type return_type, void *return_value) {
    if (DEBUG) printf("[DEBUG]: kw_less called\n");
    if (list_size(args) != 2) {
        printf("Error: Invalid number of arguments (Expected 2, got %d), line %d\n", list_size(args), line);
        exit(1);
    }
    float a, b; //get a and b
    W_Word *a_word = list_get(args, 0), *b_word = list_get(args, 1);

    a = get_number(scope, a_word, line);
    b = get_number(scope, b_word, line);

    char *result = (char *)w_malloc(6); //compare a and b
    a < b ? strcpy(result, "true") : strcpy(result, "false");

    W_Word *result_word = (W_Word *)w_malloc(sizeof(W_Word)); //create result word
    result_word->type = IDENTIFIER;
    result_word->value = result;
    result_word->is_generated = true;
    result_word->line = line;

    if (DEBUG) printf("[DEBUG]: kw_less done\n");
    return result_word;
}

/**
 * \brief Compare two variables for greater than or equal to (w_malloc)
 * \param scope The scope to compare the variables in
 * \param args The arguments to call the function with (a, b)
 * \param line The line of the code 
 * \param current_line Unused
 * \param return_type Unused
 * \param return_value Unused
 * \return The result of the comparison
 */
W_Word *kw_gequal(Scope *scope, list *args, int line, list_element **current_line, W_Type return_type, void *return_value) {
    if (DEBUG) printf("[DEBUG]: kw_gequal called\n");
    if (list_size(args) != 2) {
        printf("Error: Invalid number of arguments (Expected 2, got %d), line %d\n", list_size(args), line);
        exit(1);
    }
    float a, b; //get a and b
    W_Word *a_word = list_get(args, 0), *b_word = list_get(args, 1);

    a = get_number(scope, a_word, line);
    b = get_number(scope, b_word, line);

    char *result = (char *)w_malloc(6); //compare a and b
    a >= b ? strcpy(result, "true") : strcpy(result, "false");

    W_Word *result_word = (W_Word *)w_malloc(sizeof(W_Word)); //create result word
    result_word->type = IDENTIFIER;
    result_word->value = result;
    result_word->is_generated = true;
    result_word->line = line;

    if (DEBUG) printf("[DEBUG]: kw_gequal done\n");
    return result_word;
}

/**
 * \brief Compare two variables for less than or equal to (w_malloc)
 * \param scope The scope to compare the variables in
 * \param args The arguments to call the function with (a, b)
 * \param line The line of the code
 * \param current_line Unused
 * \param return_type Unused
 * \param return_value Unused
 * \return The result of the comparison
 */
W_Word *kw_lequal(Scope *scope, list *args, int line, list_element **current_line, W_Type return_type, void *return_value) {
    if (DEBUG) printf("[DEBUG]: kw_lequal called\n");
    if (list_size(args) != 2) {
        printf("Error: Invalid number of arguments (Expected 2, got %d), line %d\n", list_size(args), line);
        exit(1);
    }
    float a, b; //get a and b
    W_Word *a_word = list_get(args, 0), *b_word = list_get(args, 1);

    a = get_number(scope, a_word, line);
    b = get_number(scope, b_word, line);

    char *result = (char *)w_malloc(6); //compare a and b
    a <= b ? strcpy(result, "true") : strcpy(result, "false");

    W_Word *result_word = (W_Word *)w_malloc(sizeof(W_Word)); //create result word
    result_word->type = IDENTIFIER;
    result_word->value = result;
    result_word->is_generated = true;
    result_word->line = line;

    if (DEBUG) printf("[DEBUG]: kw_lequal done\n");
    return result_word;
}

/***********************************************
 * Variables ***********************************
 ***********************************************/

/**
 * \brief Initialize a new boolean variable (w_malloc)
 * \param scope The scope to initialize the variable in
 * \param args The arguments to call the function with (name)
 * \param line The line of the code
 * \param current_line Unused
 * \param return_type Unused
 * \param return_value Unused
 * \return The initialized variable
 */
W_Word *kw_bool(Scope *scope, list *args, int line, list_element **current_line, W_Type return_type, void *return_value) {
    if (DEBUG) printf("[DEBUG]: kw_bool called\n");
    if (list_size(args) != 1) {
        printf("Error: Invalid number of arguments (Expected 1, got %d), line %d\n", list_size(args), line);
        exit(1);
    }
    char *name = ((W_Word *)list_get(args, 0))->value;
    if (w_dict_get(scope->vars, name) != NULL) {
        printf("Error: Variable '%s' already exists, line %d\n", name, line);
        exit(1);
    }
    W_Bool *result = w_bool_init();
    w_dict_set(scope->vars, name, result);
    W_Word *word = (W_Word *)w_malloc(sizeof(W_Word)); //create result word
    word->type = IDENTIFIER;
    char *str = (char *)w_malloc(strlen(name) + 1);
    strcpy(str, name);
    word->value = str;
    word->is_generated = true;
    word->line = line;
    if (DEBUG) printf("[DEBUG]: kw_bool done\n");
    return word;
}

/**
 * \brief Initialize a new float variable (w_malloc)
 * \param scope The scope to initialize the variable in
 * \param args The arguments to call the function with (name)
 * \param line The line of the code
 * \param current_line Unused
 * \param return_type Unused
 * \param return_value Unused
 * \return The initialized variable
 */

W_Word *kw_float(Scope *scope, list *args, int line, list_element **current_line, W_Type return_type, void *return_value) {
    if (DEBUG) printf("[DEBUG]: kw_float called\n");
    if (list_size(args) != 1) {
        printf("Error: Invalid number of arguments (Expected 1, got %d), line %d\n", list_size(args), line);
        exit(1);
    }
    char *name = ((W_Word *)list_get(args, 0))->value;
    if (w_dict_get(scope->vars, name) != NULL) {
        printf("Error: Variable '%s' already exists, line %d\n", name, line);
        exit(1);
    }
    W_Float *result = w_float_init();
    w_dict_set(scope->vars, name, result);
    W_Word *word = (W_Word *)w_malloc(sizeof(W_Word)); //create result word
    word->type = IDENTIFIER;
    char *str = (char *)w_malloc(strlen(name) + 1);
    strcpy(str, name);
    word->value = str;
    word->is_generated = true;
    word->line = line;
    if (DEBUG) printf("[DEBUG]: kw_float done\n");
    return word;
}

/**
 * \brief Initialize a new integer variable (w_malloc)
 * \param scope The scope to initialize the variable in
 * \param args The arguments to call the function with (name)
 * \param line The line of the code
 * \param current_line Unused
 * \param return_type Unused
 * \param return_value Unused
 * \return The initialized variable
 */
W_Word *kw_int(Scope *scope, list *args, int line, list_element **current_line, W_Type return_type, void *return_value) {
    if (DEBUG) printf("[DEBUG]: kw_int called\n");
    if (list_size(args) != 1) {
        printf("Error: Invalid number of arguments (Expected 1, got %d), line %d\n", list_size(args), line);
        exit(1);
    }
    char *name = ((W_Word *)list_get(args, 0))->value;
    if (w_dict_get(scope->vars, name) != NULL) {
        printf("Error: Variable '%s' already exists, line %d\n", name, line);
        exit(1);
    }
    W_Int *result = w_int_init();
    w_dict_set(scope->vars, name, result);
    W_Word *word = (W_Word *)w_malloc(sizeof(W_Word)); //create result word
    word->type = IDENTIFIER;
    char *str = (char *)w_malloc(strlen(name) + 1);
    strcpy(str, name);
    word->value = str;
    word->is_generated = true;
    word->line = line;
    if (DEBUG) printf("[DEBUG]: kw_int done\n");
    return word;
}

/**
 * \brief Initialize a new string variable (w_malloc)
 * \param scope The scope to initialize the variable in
 * \param args The arguments to call the function with (name)
 * \param line The line of the code
 * \param current_line Unused
 * \param return_type Unused
 * \param return_value Unused
 * \return The initialized variable
 */
W_Word *kw_str(Scope *scope, list *args, int line, list_element **current_line, W_Type return_type, void *return_value) {
    if (DEBUG) printf("[DEBUG]: kw_str called\n");
    if (list_size(args) != 1) {
        printf("Error: Invalid number of arguments (Expected 1, got %d), line %d\n", list_size(args), line);
        exit(1);
    }
    char *name = ((W_Word *)list_get(args, 0))->value;
    if (w_dict_get(scope->vars, name) != NULL) {
        printf("Error: Variable '%s' already exists, line %d\n", name, line);
        exit(1);
    }
    W_Str *result = w_str_init();
    w_dict_set(scope->vars, name, result);
    W_Word *word = (W_Word *)w_malloc(sizeof(W_Word)); //create result word
    word->type = IDENTIFIER;
    char *str = (char *)w_malloc(strlen(list_get(args, 0)) + 1);
    strcpy(str, list_get(args, 0));
    word->value = str;
    word->is_generated = true;
    word->line = line;
    if (DEBUG) printf("[DEBUG]: kw_str done\n");
    return word;
}

/**
 * \brief Initialize a new list variable (w_malloc)
 * \param scope The scope to initialize the variable in
 * \param args The arguments to call the function with (name)
 * \param line The line of the code
 * \param current_line Unused
 * \param return_type Unused
 * \param return_value Unused
 * \return The initialized variable
 */
W_Word *kw_list(Scope *scope, list *args, int line, list_element **current_line, W_Type return_type, void *return_value) {
    if (DEBUG) printf("[DEBUG]: kw_list called\n");
    if (list_size(args) != 1) {
        printf("Error: Invalid number of arguments (Expected 1, got %d), line %d\n", list_size(args), line);
        exit(1);
    }
    char *name = ((W_Word *)list_get(args, 0))->value;
    if (w_dict_get(scope->vars, name) != NULL) {
        printf("Error: Variable '%s' already exists, line %d\n", name, line);
        exit(1);
    }
    W_List *result = w_list_init();
    w_dict_set(scope->vars, list_get(args, 0), result);
    W_Word *word = (W_Word *)w_malloc(sizeof(W_Word)); //create result word
    word->type = IDENTIFIER;
    char *str = (char *)w_malloc(strlen(name) + 1);
    strcpy(str, name);
    word->value = str;
    word->is_generated = true;
    word->line = line;
    if (DEBUG) printf("[DEBUG]: kw_list done\n");
    return word;
}

/**
 * \brief Assign a value to a variable (w_malloc)
 * \param scope The scope to assign the value in
 * \param args The arguments to call the function with (name, value)
 * \param line The line of the code
 * \param current_line Unused
 * \param return_type Unused
 * \param return_value Unused
 * \return The assigned variable
 */
W_Word *kw_assign(Scope *scope, list *args, int line, list_element **current_line, W_Type return_type, void *return_value) {
    if (DEBUG) printf("[DEBUG]: kw_assign called\n");

    if (list_size(args) != 2) {
        printf("Error: Invalid number of arguments (Expected 2, got %d), line %d\n", list_size(args), line);
        exit(1);
    }
    char *name = ((W_Word *)list_get(args, 0))->value;
    W_Var *var = w_dict_get(scope->vars, name);
    if (var == NULL) {
        printf("Error: Variable '%s' not found, line %d\n", name, line);
        exit(1);
    }

    //get value
    W_Word *value_word = list_get(args, 1);
    if (var->type == BOOL) {
        bool value;
        if (value_word->type == IDENTIFIER) {
            W_Var *src_var = get_var(scope, value_word->value);
            if (src_var == NULL) {
                printf("Error: Variable '%s' not found, line %d\n", value_word->value, line);
                exit(1);
            }
            if (src_var->type != BOOL) {
                printf("Error: Unsupported type (Expected bool, got %s), line %d\n", get_type_str(src_var->type), line);
                exit(1);
            }
            value = *(bool *)src_var->value;
        } else {
            printf("Error: Unsupported type (Expected bool, got %s), line %d\n", value_word->value, line);
            exit(1);
        }
        w_bool_set((W_Bool *)var, value);
    } else if (var->type == INT) {
        float value;
        if (value_word->type == NUMBER) {
            value = atof(value_word->value);
        } else if (value_word->type == IDENTIFIER) {
            W_Var *src_var = get_var(scope, value_word->value);
            if (src_var == NULL) {
                printf("Error: Variable '%s' not found, line %d\n", value_word->value, line);
                exit(1);
            }
            if (src_var->type != INT && src_var->type != FLOAT) {
                printf("Error: Unsupported type (Expected int or float, got %s), line %d\n", get_type_str(src_var->type), line);
                exit(1);
            }
            value = *(float *)src_var->value;
        } else {
            printf("Error: Unsupported type (Expected int or float, got %s), line %d\n", value_word->value, line);
            exit(1);
        }
        w_int_set((W_Int *)var, value);
    } else if (var->type == FLOAT) {
        float value;
        if (value_word->type == NUMBER) {
            value = atof(value_word->value);
        } else if (value_word->type == IDENTIFIER) {
            W_Var *src_var = get_var(scope, value_word->value);
            if (src_var == NULL) {
                printf("Error: Variable '%s' not found, line %d\n", value_word->value, line);
                exit(1);
            }
            if (src_var->type != INT && src_var->type != FLOAT) {
                printf("Error: Unsupported type (Expected int or float, got %s), line %d\n", get_type_str(src_var->type), line);
                exit(1);
            }
            value = *(float *)src_var->value;
        } else {
            printf("Error: Unsupported type (Expected int or float, got %s), line %d\n", value_word->value, line);
            exit(1);
        }
        w_float_set((W_Float *)var, value);
    } else if (var->type == STRING) {
        char *value;
        if (value_word->type == LITT_STR) {
            //remove quotes
            char without_quotes[strlen(value_word->value) - 1];
            strncpy(without_quotes, value_word->value + 1, strlen(value_word->value) - 2);
        } else if (value_word->type == IDENTIFIER) {
            W_Var *src_var = get_var(scope, value_word->value);
            if (src_var == NULL) {
                printf("Error: Variable '%s' not found, line %d\n", value_word->value, line);
                exit(1);
            }
            if (src_var->type != STRING) {
                printf("Error: Unsupported type (Expected string, got %s), line %d\n", get_type_str(src_var->type), line);
                exit(1);
            }
            strcpy(value, (char *)src_var->value);
        } else {
            printf("Error: Unsupported type (Expected string, got %s), line %d\n", value_word->value, line);
            exit(1);
        }
        w_str_set((W_Str *)var, value);
    } else {
        printf("Error: Unsupported type (Expected int, float, string, or bool, got %s), line %d\n", get_type_str(var->type), line);
        exit(1);
    }

    if (DEBUG) printf("[DEBUG]: Create word\n");
    W_Word *word = (W_Word *)w_malloc(sizeof(W_Word)); //create result word
    word->type = IDENTIFIER;
    char *str = (char *)w_malloc(strlen(name) + 1);
    strcpy(str, name);
    word->value = str;
    word->is_generated = true;
    word->line = line;
    if (DEBUG) printf("[DEBUG]: kw_assign done\n");
    return word;
}

/**
 * \brief Delete a variable
 * \param scope The scope to delete the variable in
 * \param args The arguments to call the function with (name)
 * \param line The line of the code
 * \param current_line Unused
 * \param return_type Unused
 * \param return_value Unused
 * \return NULL
 */
W_Word *kw_delete(Scope *scope, list *args, int line, list_element **current_line, W_Type return_type, void *return_value) {
    if (DEBUG) printf("[DEBUG]: kw_delete called\n");
    if (list_size(args) != 1) {
        printf("Error: Invalid number of arguments (Expected 1, got %d), line %d\n", list_size(args), line);
        exit(1);
    }
    char *name = ((W_Word *)list_get(args, 0))->value;
    W_Var *var = w_dict_get(scope->vars, (char *)name);
    if (var == NULL) {
        printf("Error: Variable '%s' not found, line %d\n", name, line);
        exit(1);
    }
    w_dict_remove(scope->vars, name);
    if (DEBUG) printf("[DEBUG]: kw_delete done\n");
    return NULL;
}

/***********************************************
 * I/O *****************************************
 ***********************************************/

/**
 * \brief Print all arguments in the console
 * \param scope The scope to print in
 * \param args The arguments to call the function with (value)
 * \param line The line of the code
 * \param current_line Unused
 * \param return_type Unused
 * \param return_value Unused
 * \return NULL
 */
W_Word *kw_print(Scope *scope, list *args, int line, list_element **current_line, W_Type return_type, void *return_value) {
    if (DEBUG) printf("[DEBUG]: kw_print called\n");
    for (int i = 0; i < args->size; i++) {
        W_Word *word = list_get(args, i);
        if (word->type == IDENTIFIER) {
            W_Var *var = w_dict_get(scope->vars, word->value);
            if (var == NULL) {
                printf("Error: Variable '%s' not found, line %d\n", word->value, line);
                exit(1);
            }
            char *str = var->stringify(var);
            printf("%s", str);
            w_free(str);
        } else if (word->type == NUMBER) {
            printf("%s", word->value);
        } else if (word->type == LITT_STR) {
            //remove quotes
            char without_quotes[strlen(word->value) - 1];
            strncpy(without_quotes, word->value + 1, strlen(word->value) - 2);
            without_quotes[strlen(word->value) - 2] = '\0';
            printf("%s", without_quotes);

        } else {
            printf("Error: Unexpected word, line %d\n", word->value, line);
            exit(1);
        }
        if (i < args->size - 1) {
            printf(" ");
        }
    }
    printf("\n");
    if (DEBUG) printf("[DEBUG]: kw_print done\n");
    return NULL;
}

/**
 * \brief Ask the user for input
 * \param scope The scope to ask in
 * \param args The arguments to call the function with (prompt)
 * \param line The line of the code
 * \param current_line Unused
 * \param return_type Unused
 * \param return_value Unused
 * \return The input variable
 */
W_Word *kw_ask(Scope *scope, list *args, int line, list_element **current_line, W_Type return_type, void *return_value) {
    if (DEBUG) printf("[DEBUG]: kw_ask called\n");
    char *prompt = ((W_Word *)list_get(args, 0))->value;
    printf("%s", prompt);
    int size = 0;

    //get input
    fflush(stdin);
    char *input = (char *)malloc(size + 1);
    input[0] = '\0';
    while (1) {
        char c = fgetc(stdin);
        if (c == '\n') {
            break;
        }
        size++;
        input = (char *)realloc(input, size + 1);
        input[size - 1] = c;
        input[size] = '\0';
    }

    W_Word *word = (W_Word *)w_malloc(sizeof(W_Word)); //create input word
    word->type = LITT_STR;
    word->value = input;
    word->is_generated = true;
    word->line = line;
    if (DEBUG) printf("[DEBUG]: kw_ask done\n");
    return word;
}

/***********************************************
 * Functions ***********************************
 ***********************************************/

/**
 * \brief Defines a function
 * \param scope The scope to add the variables in
 * \param args The arguments to call the function with
 * \param line The line of the code
 * \param current_line The current line of code that is evaluated
 * \param return_type Unused
 * \param return_value Unused
 * \return NULL
 */
W_Word *kw_def(Scope *scope, list *args, int line, list_element **current_line, W_Type return_type, void *return_value) {
    if (DEBUG) printf("[DEBUG]: kw_def called\n");

    char *func_name = ((W_Word *)list_get(args, 0))->value; //get the name of the new declared function

    //add function to scope
    W_Var *var = w_dict_get(scope->vars, func_name);
    if (var == NULL) {
        fprintf(stderr, "Error: Function variable '%s' definition not found, line %d", func_name, line);
        exit(1);
    }
    int type = var->type;
    if (type == FUNCTION) {
        fprintf(stderr, "Error: Expected function variable definition, got function '%s', line %d", func_name, line);
        exit(1);
    }
    W_Func *new_func = w_func_init(); //init the new function
    new_func->return_type = type;
    w_dict_set(scope->vars, func_name, new_func);

    if (list_size(args) > 1) {
        if (strcmp("with", ((W_Word *)list_get(args, 1))->value)) {
            printf("Error: Expected keyword 'with' after function name, line %d", line);
            exit(1);
        }
        for (int i = 2; i < list_size(args); i++) {
            char *arg_name = ((W_Word *)list_get(args, i))->value; //get the function argument name
            W_Var *var = w_dict_get(scope->vars, arg_name);
            if (var == NULL) {
                fprintf(stderr, "Error: Function argument '%s' not found, line %d", arg_name, line);
                exit(1);
            }
            W_Type *func_arg_type = (W_Type *)w_malloc(sizeof(W_Type));
            *func_arg_type = var->type;
            w_dict_remove(scope->vars, arg_name);
            dict_set(new_func->args, arg_name, func_arg_type);
        }
    }

    int fn_def_count = 0; //if there are function definition in the current function
    list *function_lines = new_func->parsed_code;
    bool end = false;

    *current_line = (*current_line)->next;
    while (*current_line != NULL) {
        W_Word *word = (W_Word *)((list *)(*current_line)->value)->head->value; //get the first word of the line

        if (word == NULL) { //if it is an empty line
            *current_line = (*current_line)->next;
            continue;
        };

        if (strcmp(word->value, "def") == 0){ //if it is a def keyword then increment fn_def_count (function def in the function)
            fn_def_count++;
        } else if (strcmp(word->value, "enddef") == 0) {
            if (fn_def_count == 0) { //if the enddef matches the end of the function, break
                end = true;
                break;
            }
            else fn_def_count--; //else decrement fn_def_count
        }

        list_append(function_lines, (*current_line)->value);
        *current_line = (*current_line)->next;
    }
    if (!end) {
        fprintf(stderr, "Error: Expected keyword 'enddef' at the end of the function definition, line %d", line);
        exit(1);
    }

    if (DEBUG) printf("[DEBUG]: kw_def done\n");
    return NULL;
}

/**
 * \brief Call the function
 * \param scope The scope to add the variables in
 * \param args The arguments to call the function with
 * \param line The line of the code
 * \param current_line Unused
 * \param return_type The return type of the execute code
 * \param return_value The return value of the execute code
 * \return NULL
 */
W_Word *kw_call(Scope *scope, list *args, int line, list_element **current_line, W_Type return_type, void *return_value) {
    if (DEBUG) printf("[DEBUG]: kw_call called\n");

    char *func_name = ((W_Word *)list_get(args, 0))->value; //get the function
    W_Func *f = (W_Func *)get_var(scope, func_name);
    if (f == NULL) {
        fprintf(stderr, "Error: Function '%s' does not exist, line %d", func_name, line);
        exit(1);
    } else if (f->type != FUNCTION) {
        fprintf(stderr, "Error: Expected function, got variable '%s', line: %d\n", func_name, line);
        exit(1);
    }

    Scope *fn_scope = init_scope(); //init the scope
    fn_scope->parent = f->parent_scope;

    if (list_size(args) > 1) { //if there are arguments
        int nb_args = dict_size(f->args); //get the number of arguments for the function

        if (strcmp("with", ((W_Word *)list_get(args, 1))->value) != 0) {
            fprintf(stderr, "Error: Expected keyword 'with' after function name, line %d", line);
            exit(1);
        }

        if (nb_args != list_size(args) - 2) { //-2: not count func name, with keyword
            fprintf(stderr, "Error: Invalid number of arguments for function '%s', got %d expected %d, line %d", func_name, list_size(args)-2, nb_args, line);
            exit(1);
        }

        for (int i = 0; i < nb_args; i++) { //get all arguments for function call
            W_Word *arg = (W_Word *)list_get(args, i+2); //the argument to add to the function scope
            char *arg_name = (char *)list_get(f->args->keys, i); //the argument name in the function definition
            W_Type arg_type = *(W_Type *)list_get(f->args->values, i); //the argument type in the function definition
            if (arg->type == IDENTIFIER) {
                W_Var *var = get_var(scope, arg->value); //get the variable
                if (var == NULL) {
                    fprintf(stderr, "Error: Variable '%s' does not exist, line %d", arg->value, line);
                    exit(1);
                }
                if (var->type != arg_type) {
                    fprintf(stderr, "Error: Expected argument type '%s', got '%s' (type: %s), line %d", get_type_str(arg_type), arg->value, get_type_str(var->type), line);
                    exit(1);
                }
                W_Var *var_copy = var->copy(var);
                w_dict_set(fn_scope->vars, arg_name, var_copy);
            } else if (arg->type == NUMBER) {
                void *var;
                if (is_float(arg->value)) {
                    if (arg_type != FLOAT) {
                        fprintf(stderr, "Error: Expected argument type 'float', got '%s', line %d", arg->value, line);
                        exit(1);
                    }
                    var = w_float_init();
                    w_float_assign((W_Float *)var, arg->value);
                } else {
                    if (arg_type != INT) {
                        fprintf(stderr, "Error: Expected argument type 'int', got '%s', line %d", arg->value, line);
                        exit(1);
                    }
                    var = w_int_init();
                    w_int_assign((W_Int *)var, arg->value);
                }
                w_dict_set(fn_scope->vars, arg_name, var);
            } else if (arg->type == LITT_STR) {
                if (arg_type != STRING) {
                    fprintf(stderr, "Error: Expected argument type 'string', got '%s', line %d", arg->value, line);
                    exit(1);
                }
                W_Str *var = w_str_init();
                w_str_assign(var, arg->value);
                w_dict_set(fn_scope->vars, arg_name, var);
            } else {
                fprintf(stderr, "Error: Expected argument for function call, got '%s', line %d", arg->value, line);
                exit(1);
            }
        }
    }
    void *result = execute(f->parsed_code, fn_scope, f->return_type, true);
    printf("Result NULL: %d\n", result==NULL);
    W_Word *word = NULL;
    if (result != NULL) {
        w_dict_set(scope->vars, "@return", result);
        if (DEBUG) printf("[DEBUG]: Stored return value in scope at @return\n");

        word = (W_Word *)w_malloc(sizeof(W_Word)); //create result word
        word->type = IDENTIFIER;
        char *str = (char *)w_malloc(8);
        strcpy(str, "@return");
        word->value = str;
        word->is_generated = true;
        word->line = line;
    }

    if (DEBUG) printf("[DEBUG]: kw_call done\n");
    return word;
}

/**
 * \brief Return a value from a function
 * \param scope The scope to return the value in
 * \param args The arguments to call the function with (value)
 * \param line The line of the code
 * \param current_line Unused
 * \param return_type The return type of the execute code
 * \param return_value The return value of the execute code
 * \return NULL (return value is set in return_value argument)
 */
W_Word *kw_return(Scope *scope, list *args, int line, list_element **current_line, W_Type return_type, void *return_value) {
    if (DEBUG) printf("[DEBUG]: kw_return called\n");

    if (return_type == NULL_TYPE) {
        fprintf(stderr, "Error: Unexpected return value, line %d", line);
        exit(1);
    }
    if (list_size(args) != 1) {
        fprintf(stderr, "Error: Invalid number of arguments (Expected 1, got %d), line %d", list_size(args), line);
        exit(1);
    }

    W_Word *word = list_get(args, 0);

    if (word->type == IDENTIFIER) {
        W_Var *var = get_var(scope, word->value);
        if (var == NULL) {
            fprintf(stderr, "Error: Variable '%s' does not exist, line %d", word->value, line);
            exit(1);
        }
        if (var->type != return_type) {
            fprintf(stderr, "Error: Expected return type '%s', got '%s' (type: %s), line %d", get_type_str(return_type), word->value, get_type_str(var->type), line);
            exit(1);
        }
        return_value = var->copy(var);
    } else if (word->type == NUMBER) {
        if (is_float(word->value)) {
            if (return_type != FLOAT) {
                fprintf(stderr, "Error: Expected return type 'float', got '%s', line %d", word->value, line);
                exit(1);
            }
            return_value = w_float_init();
            w_float_assign((W_Float *)return_value, word->value);
        } else {
            if (return_type != INT) {
                fprintf(stderr, "Error: Expected return type 'int', got '%s', line %d", word->value, line);
                exit(1);
            }
            return_value = w_int_init();
            w_int_assign((W_Int *)return_value, word->value);
        }
    } else if (word->type == LITT_STR) {
        if (return_type != STRING) {
            fprintf(stderr, "Error: Expected return type 'string', got '%s', line %d", word->value, line);
            exit(1);
        }
        return_value = w_str_init();
        w_str_assign((W_Str *)return_value, word->value);
    } else {
        fprintf(stderr, "Error: Expected return value, got '%s', line %d", word->value, line);
        exit(1);
    }

    if (DEBUG) printf("[DEBUG]: kw_return done\n");
    return NULL;
}

/***********************************************
 * Loops ***************************************
 ***********************************************/

/**
 * \brief Start an infloop
 * \param scope The scope to start the loop in
 * \param args The arguments to call the function with
 * \param line The line of the code
 * \param current_line The current line of code that is evaluated
 * \param return_type Unused
 * \param return_value Unused
 * \return NULL
 */
W_Word *kw_infloop(Scope *scope, list *args, int line, list_element **current_line, W_Type return_type, void *return_value) {
    if (DEBUG) printf("[DEBUG]: kw_infloop called\n");

    list *infloop_lines = list_init();
    int infloop_count = 0;
    bool end = false;

    *current_line = (*current_line)->next;
    while (current_line != NULL) { //get the lines for the infloop
        W_Word *word = (W_Word *)((list *)(*current_line)->value)->head->value; //get the first word of the line

        if (word == NULL) { //if it is an empty line
            *current_line = (*current_line)->next;
            continue;
        }

        if (strcmp(word->value, "infloop") == 0) {
            infloop_count++;
        } else if (strcmp(word->value, "endinf") == 0) {
            if (infloop_count == 0) {
                end = true;
                break;
            } else infloop_count--;
        }

        list_append(infloop_lines, (*current_line)->value);
        *current_line = (*current_line)->next;
    }

    if (!end) {
        fprintf(stderr, "Error: Expected keyword 'endinf' at the end of the infloop, line %d", line);
        exit(1);
    }

    while (true) {
        void *result = execute(infloop_lines, scope, NULL_TYPE, false);
    }

    if (DEBUG) printf("[DEBUG]: kw_infloop done\n");
    return NULL;
}