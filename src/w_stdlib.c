#include "w_stdlib.h"

/***********************************************
 * Utility *************************************
 ***********************************************/

/**
 * \brief Get the type of a variable as a string (w_malloc)
 * \param var The variable to get the type of
 * \return The type of the variable
 */
static char *w_get_type_str(W_Var *var) {
    char *type = (char *)w_malloc(9);
    switch (var->type) {
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
static W_Type w_get_type(char *str) {
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
            printf("Error: Unsupported type (Expected int or float, got %s), line %d\n", w_get_type_str(var), line);
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
            printf("Error: Unsupported type (Expected bool, got %s), line %d\n", w_get_type_str(var), line);
            exit(1);
        }
        res = *(bool *)var->value;
    } else {
        printf("Error: Expected variable or boolean expression, got %s, line %d", word->value, line);
    }
    return res;
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
 * \return The result of the addition
 */
W_Word *kw_plus(Scope *scope, list *args, int line, list_element *current_line) {
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

    return result_word;
}

/**
 * \brief Subtract two variables (w_malloc)
 * \param scope The scope to subtract the variables in
 * \param args The arguments to call the function with (a, b)
 * \param line The line of the code
 * \param current_line Unused
 * \return The result of the subtraction
 */
W_Word *kw_minus(Scope *scope, list *args, int line, list_element *current_line) {
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

    return result_word;
}

/**
 * \brief Multiply two variables (w_malloc)
 * \param scope The scope to multiply the variables in
 * \param args The arguments to call the function with (a, b)
 * \param line The line of the code
 * \param current_line Unused
 * \return The result of the multiplication
 */
W_Word *kw_time(Scope *scope, list *args, int line, list_element *current_line) {
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

    return result_word;
}

/**
 * \brief Divide two variables (w_malloc)
 * \param scope The scope to divide the variables in
 * \param args The arguments to call the function with (a, b)
 * \param line The line of the code
 * \param current_line Unused
 * \return The result of the division
 */
W_Word *kw_div(Scope *scope, list *args, int line, list_element *current_line) {
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

    return result_word;
}

/**
 * \brief Modulo two variables (w_malloc)
 * \param scope The scope to modulo the variables in
 * \param args The arguments to call the function with (a, b)
 * \param line The line of the code
 * \param current_line Unused
 * \return The result of the modulo
 */
W_Word *kw_mod(Scope *scope, list *args, int line, list_element *current_line) {
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

    return result_word;
}

/**
 * \brief Euclidean division two variables (w_malloc)
 * \param scope The scope to euclidean division the variables in
 * \param args The arguments to call the function with (a, b)
 * \param line The line of the code
 * \param current_line Unused
 * \return The result of the integer division
 */
W_Word *kw_ediv(Scope *scope, list *args, int line, list_element *current_line) {
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

    return result_word;
}

/**
 * \brief Raise a variable to the power of another variable (w_malloc)
 * \param scope The scope to raise the variables in
 * \param args The arguments to call the function with (a, b)
 * \param line The line of the code
 * \param current_line Unused
 * \return The result of the power operation
 */
W_Word *kw_power(Scope *scope, list *args, int line, list_element *current_line) {
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

    return result_word;
}

/**
 * \brief Square root a variable (w_malloc)
 * \param scope The scope to square root the variable in
 * \param args The arguments to call the function with (a)
 * \param line The line of the code
 * \param current_line Unused
 * \return The result of the square root operation
 */
W_Word *kw_sqrt(Scope *scope, list *args, int line, list_element *current_line) {
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
 * \return The result of the AND operation
 */
W_Word *kw_and(Scope *scope, list *args, int line, list_element *current_line) {
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

    return result_word;
}

/**
 * \brief Logical OR two variables (w_malloc)
 * \param scope The scope to OR the variables in
 * \param args The arguments to call the function with (a, b)
 * \param line The line of the code
 * \param current_line Unused
 * \return The result of the OR operation
 */
W_Word *kw_or(Scope *scope, list *args, int line, list_element *current_line) {
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

    return result_word;
}

/**
 * \brief Logical NOT a variable (w_malloc)
 * \param scope The scope to NOT the variable in
 * \param args The arguments to call the function with (a, b)
 * \param line The line of the code
 * \param current_line Unused
 * \return The result of the NOT operation
 */
W_Word *kw_not(Scope *scope, list *args, int line, list_element *current_line) {
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
 * \return The result of the comparison
 */
W_Word *kw_equal(Scope *scope, list *args, int line, list_element *current_line) {
    if (list_size(args) != 2) {
        printf("Error: Invalid number of arguments (Expected 2, got %d), line %d\n", list_size(args), line);
        exit(1);
    }

    W_Var *a, *b; //get a and b
    W_Word *a_word = list_get(args, 0), *b_word = list_get(args, 1);

    if (a_word->type == IDENTIFIER) { //initialize a
        a = w_dict_get(scope->vars, a_word->value);
        if (a == NULL) {
            printf("Error: Variable '%s' not found, line %d\n", a_word->value, line);
            exit(1);
        }
    } else if (a_word->type == NUMBER) {
        if (is_float(a_word->value)) {
            a = w_float_init();
            *(float *)a->value = atof(a_word->value);
        } else {
            a = w_int_init();
            *(int *)a->value = atoi(a_word->value);
        }
    } else if (a_word->type == STRING) {
        a = w_str_init();
        *(char *) a->value = a_word->value;
    }

    if (b_word->type == IDENTIFIER) { //initialize b
        b = w_dict_get(scope->vars, b_word->value);
        if (b == NULL) {
            printf("Error: Variable '%s' not found, line %d\n", b_word->value, line);
            exit(1);
        }
    } else if (b_word->type == NUMBER) {
        if (is_float(b_word->value)) {
            b = w_float_init();
            *(float *)b->value = atof(b_word->value);
        } else {
            b = w_int_init();
            *(int *)b->value = atoi(b_word->value);
        }
    } else if (b_word->type == STRING) {
        b = w_str_init();
        *(char *) b->value = b_word->value;
    }

    char *result = (char *)w_malloc(6); //compare a and b
    if (a->type != b->type) {
        strcpy(result, "false");
    } else {
        _equal(a, b) ? strcpy(result, "true") : strcpy(result, "false");
    }

    W_Word *result_word = (W_Word *)w_malloc(sizeof(W_Word)); //create result word
    result_word->type = IDENTIFIER;
    result_word->value = result;
    result_word->is_generated = true;
    result_word->line = line;

    return result_word;
}

/**
 * \brief Compare two variables for greater than (w_malloc)
 * \param scope The scope to compare the variables in
 * \param args The arguments to call the function with (a, b)
 * \param line The line of the code
 * \param current_line Unused
 * \return The result of the comparison
 */
W_Word *kw_greater(Scope *scope, list *args, int line, list_element *current_line) {
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

    return result_word;
}

/**
 * \brief Compare two variables for less than (w_malloc)
 * \param scope The scope to compare the variables in
 * \param args The arguments to call the function with (a, b)
 * \param line The line of the code
 * \param current_line Unused
 * \return The result of the comparison
 */
W_Word *kw_less(Scope *scope, list *args, int line, list_element *current_line) {
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

    return result_word;
}

/**
 * \brief Compare two variables for greater than or equal to (w_malloc)
 * \param scope The scope to compare the variables in
 * \param args The arguments to call the function with (a, b)
 * \param line The line of the code 
 * \param current_line Unused
 * \return The result of the comparison
 */
W_Word *kw_gequal(Scope *scope, list *args, int line, list_element *current_line) {
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

    return result_word;
}

/**
 * \brief Compare two variables for less than or equal to (w_malloc)
 * \param scope The scope to compare the variables in
 * \param args The arguments to call the function with (a, b)
 * \param line The line of the code
 * \param current_line Unused
 * \return The result of the comparison
 */
W_Word *kw_lequal(Scope *scope, list *args, int line, list_element *current_line) {
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
 * \return The initialized variable
 */
W_Word *kw_bool(Scope *scope, list *args, int line, list_element *current_line) {
    W_Bool *result = w_bool_init();
    w_dict_set(scope->vars, list_get(args, 0), result);
    W_Word *word = (W_Word *)w_malloc(sizeof(W_Word)); //create result word
    word->type = IDENTIFIER;
    char *str = (char *)w_malloc(strlen(list_get(args, 0)) + 1);
    strcpy(str, list_get(args, 0));
    word->value = str;
    word->is_generated = true;
    word->line = line;
    return word;
}

/**
 * \brief Initialize a new float variable (w_malloc)
 * \param scope The scope to initialize the variable in
 * \param args The arguments to call the function with (name)
 * \param line The line of the code
 * \param current_line Unused
 * \return The initialized variable
 */

W_Word *kw_float(Scope *scope, list *args, int line, list_element *current_line) {
    W_Float *result = w_float_init();
    w_dict_set(scope->vars, list_get(args, 0), result);
    W_Word *word = (W_Word *)w_malloc(sizeof(W_Word)); //create result word
    word->type = IDENTIFIER;
    char *str = (char *)w_malloc(strlen(list_get(args, 0)) + 1);
    strcpy(str, list_get(args, 0));
    word->value = str;
    word->is_generated = true;
    word->line = line;
    return word;
}

/**
 * \brief Initialize a new integer variable (w_malloc)
 * \param scope The scope to initialize the variable in
 * \param args The arguments to call the function with (name)
 * \param line The line of the code
 * \param current_line Unused
 * \return The initialized variable
 */
W_Word *kw_int(Scope *scope, list *args, int line, list_element *current_line) {
    W_Int *result = w_int_init();
    w_dict_set(scope->vars, list_get(args, 0), result);
    W_Word *word = (W_Word *)w_malloc(sizeof(W_Word)); //create result word
    word->type = IDENTIFIER;
    char *str = (char *)w_malloc(strlen(list_get(args, 0)) + 1);
    strcpy(str, list_get(args, 0));
    word->value = str;
    word->is_generated = true;
    word->line = line;
    return word;
}

/**
 * \brief Initialize a new string variable (w_malloc)
 * \param scope The scope to initialize the variable in
 * \param args The arguments to call the function with (name)
 * \param line The line of the code
 * \param current_line Unused
 * \return The initialized variable
 */
W_Word *kw_str(Scope *scope, list *args, int line, list_element *current_line) {
    W_Str *result = w_str_init();
    w_dict_set(scope->vars, list_get(args, 0), result);
    W_Word *word = (W_Word *)w_malloc(sizeof(W_Word)); //create result word
    word->type = IDENTIFIER;
    char *str = (char *)w_malloc(strlen(list_get(args, 0)) + 1);
    strcpy(str, list_get(args, 0));
    word->value = str;
    word->is_generated = true;
    word->line = line;
    return word;
}

/**
 * \brief Initialize a new list variable (w_malloc)
 * \param scope The scope to initialize the variable in
 * \param args The arguments to call the function with (name)
 * \param line The line of the code
 * \param current_line Unused
 * \return The initialized variable
 */
W_Word *kw_list(Scope *scope, list *args, int line, list_element *current_line) {
    W_List *result = w_list_init();
    w_dict_set(scope->vars, list_get(args, 0), result);
    W_Word *word = (W_Word *)w_malloc(sizeof(W_Word)); //create result word
    word->type = IDENTIFIER;
    char *str = (char *)w_malloc(strlen(list_get(args, 0)) + 1);
    strcpy(str, list_get(args, 0));
    word->value = str;
    word->is_generated = true;
    word->line = line;
    return word;
}

/**
 * \brief Assign a value to a variable (w_malloc)
 * \param scope The scope to assign the value in
 * \param args The arguments to call the function with (name, value)
 * \param line The line of the code
 * \param current_line Unused
 * \return The assigned variable
 */
W_Word *kw_assign(Scope *scope, list *args, int line, list_element *current_line) {
    W_Var *var = w_dict_get(scope->vars, list_get(args, 0));
    if (var == NULL) {
        printf("Error: Variable '%s' not found, line %d\n", list_get(args, 0), line);
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
                printf("Error: Unsupported type (Expected bool, got %s), line %d\n", w_get_type_str(src_var), line);
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
                printf("Error: Unsupported type (Expected int or float, got %s), line %d\n", w_get_type_str(src_var), line);
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
                printf("Error: Unsupported type (Expected int or float, got %s), line %d\n", w_get_type_str(src_var), line);
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
        if (value_word->type == STRING) {
            //remove quotes
            char *without_quotes[strlen(value_word->value) - 1];
            strncpy(without_quotes, value_word->value + 1, strlen(value_word->value) - 2);
        } else if (value_word->type == IDENTIFIER) {
            W_Var *src_var = get_var(scope, value_word->value);
            if (src_var == NULL) {
                printf("Error: Variable '%s' not found, line %d\n", value_word->value, line);
                exit(1);
            }
            if (src_var->type != STRING) {
                printf("Error: Unsupported type (Expected string, got %s), line %d\n", w_get_type_str(src_var), line);
                exit(1);
            }
            strcpy(value, (char *)src_var->value);
        } else {
            printf("Error: Unsupported type (Expected string, got %s), line %d\n", value_word->value, line);
            exit(1);
        }
        w_str_set((W_Str *)var, value);
    } else {
        printf("Error: Unsupported type (Expected int, float, string, or bool, got %s), line %d\n", w_get_type_str(var), line);
        exit(1);
    }

    W_Word *word = (W_Word *)w_malloc(sizeof(W_Word)); //create result word
    word->type = IDENTIFIER;
    char *str = (char *)w_malloc(strlen(list_get(args, 0)) + 1);
    strcpy(str, list_get(args, 0));
    word->value = str;
    word->is_generated = true;
    word->line = line;
    return word;
}

/**
 * \brief Delete a variable
 * \param scope The scope to delete the variable in
 * \param args The arguments to call the function with (name)
 * \param line The line of the code
 * \param current_line Unused
 * \return NULL
 */
W_Word *kw_delete(Scope *scope, list *args, int line, list_element *current_line) {
    W_Var *var = w_dict_get(scope->vars, (char *)list_get(args, 0));
    if (var == NULL) {
        printf("Error: Variable '%s' not found, line %d\n", list_get(args, 0), line);
        exit(1);
    }
    w_dict_remove(scope->vars, (char *)list_get(args, 0));
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
 * \return NULL
 */
W_Word *kw_print(Scope *scope, list *args, int line, list_element *current_line) {
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
        } else if (word->type == NUMBER || word->type == STRING) {
            printf("%s", word->value);
        } else {
            printf("Error: Unexpected word, line %d\n", word->value, line);
            exit(1);
        }
        if (i < args->size - 1) {
            printf(" ");
        }
    }
    printf("\n");
    return NULL;
}

/**
 * \brief Ask the user for input
 * \param scope The scope to ask in
 * \param args The arguments to call the function with (prompt)
 * \param line The line of the code
 * \param current_line Unused
 * \return The input variable
 */
W_Word *kw_input(Scope *scope, list *args, int line, list_element *current_line) {
    char *prompt = (char *)list_get(args, 0);
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
    word->type = STRING;
    word->value = input;
    word->is_generated = true;
    word->line = line;
    return word;
}

