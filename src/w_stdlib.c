#include "w_stdlib.h"

/***********************************************
 * Global **************************************
 ***********************************************/

/**
 * \brief Get the type of a variable as a string (malloc)
 * \param var The variable to get the type of
 * \return The type of the variable
 */
char *w_get_type_str(void *var) {
    char *type = (char *)malloc(9);
    switch (((W_Var *)var)->type) {
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
W_Type w_get_type(char *str) {
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

/***********************************************
 * Operations **********************************
 ***********************************************/

/**
 * \brief Add two variables (malloc)
 * \param a The first variable
 * \param b The second variable
 * \return The result of the addition
 */
void *w_plus(void *a, void *b) {
    W_Type type_a = ((W_Var *)a)->type, type_b = ((W_Var *)b)->type;
    if (type_a == INT && type_b == INT) {
        W_Int *result = int_init();
        int_set(result, *int_value(a) + *int_value(b));
        return result;
    } else if (type_a == FLOAT && type_b == INT) {
        W_Float *result = float_init();
        float_set(result, *float_value(a) + (double)*int_value(b));
        return result;
    } else if (type_b == FLOAT && type_a == INT) {
        W_Float *result = float_init();
        float_set(result, (double)*int_value(a) + *float_value(b));
        return result;
    } else if (type_a == FLOAT && type_b == FLOAT) {
        W_Float *result = float_init();
        float_set(result, *float_value(a) + *float_value(b));
        return result;
    } else {
        char *type_a_str = w_get_type_str(a), *type_b_str = w_get_type_str(b);
        printf("Error: Unsupported types for addition. (%s + %s)\n", type_a_str, type_b_str);
        free(type_a_str);
        free(type_b_str);
        exit(1);
    }
}

/**
 * \brief Subtract two variables (malloc)
 * \param a The first variable
 * \param b The second variable
 * \return The result of the subtraction
 */
void *w_minus(void *a, void *b) {
    W_Type type_a = ((W_Var *)a)->type, type_b = ((W_Var *)b)->type;
    if (type_a == INT && type_b == INT) {
        W_Int *result = int_init();
        int_set(result, *int_value(a) - *int_value(b));
        return result;
    } else if (type_a == FLOAT && type_b == INT) {
        W_Float *result = float_init();
        float_set(result, *float_value(a) - (double)*int_value(b));
        return result;
    } else if (type_b == FLOAT && type_a == INT) {
        W_Float *result = float_init();
        float_set(result, (double)*int_value(a) - *float_value(b));
        return result;
    } else if (type_a == FLOAT && type_b == FLOAT) {
        W_Float *result = float_init();
        float_set(result, *float_value(a) - *float_value(b));
        return result;
    } else {
        char *type_a_str = w_get_type_str(a), *type_b_str = w_get_type_str(b);
        printf("Error: Unsupported types for subtraction. (%s - %s)\n", type_a_str, type_b_str);
        free(type_a_str);
        free(type_b_str);
        exit(1);
    }
}

/**
 * \brief Multiply two variables (malloc)
 * \param a The first variable
 * \param b The second variable
 * \return The result of the multiplication
 */
void *w_time(void *a, void *b) {
    W_Type type_a = ((W_Var *)a)->type, type_b = ((W_Var *)b)->type;
    if (type_a == INT && type_b == INT) {
        W_Int *result = int_init();
        int_set(result, *int_value(a) * *int_value(b));
        return result;
    } else if (type_a == FLOAT && type_b == INT) {
        W_Float *result = float_init();
        float_set(result, *float_value(a) * (double)*int_value(b));
        return result;
    } else if (type_b == FLOAT && type_a == INT) {
        W_Float *result = float_init();
        float_set(result, (double)*int_value(a) * *float_value(b));
        return result;
    } else if (type_a == FLOAT && type_b == FLOAT) {
        W_Float *result = float_init();
        float_set(result, *float_value(a) * *float_value(b));
        return result;
    } else {
        char *type_a_str = w_get_type_str(a), *type_b_str = w_get_type_str(b);
        printf("Error: Unsupported types for multiplication. (%s * %s)\n", type_a_str, type_b_str);
        free(type_a_str);
        free(type_b_str);
        exit(1);
    }
}

/**
 * \brief Divide two variables (malloc)
 * \param a The first variable
 * \param b The second variable
 * \return The result of the division
 */
W_Float *w_div(void *a, void *b) {
    W_Type type_a = ((W_Var *)a)->type, type_b = ((W_Var *)b)->type;
    if (type_a == INT && type_b == INT) {
        W_Float *result = float_init();
        float_set(result, (double)*int_value(a) / *int_value(b));
        return result;
    } else if (type_a == FLOAT && type_b == INT) {
        W_Float *result = float_init();
        float_set(result, *float_value(a) / (double)*int_value(b));
        return result;
    } else if (type_b == FLOAT && type_a == INT) {
        W_Float *result = float_init();
        float_set(result, (double)*int_value(a) / *float_value(b));
        return result;
    } else if (type_a == FLOAT && type_b == FLOAT) {
        W_Float *result = float_init();
        float_set(result, *float_value(a) / *float_value(b));
        return result;
    } else {
        char *type_a_str = w_get_type_str(a), *type_b_str = w_get_type_str(b);
        printf("Error: Unsupported types for division. (%s / %s)\n", type_a_str, type_b_str);
        free(type_a_str);
        free(type_b_str);
        exit(1);
    }
}

/**
 * \brief Modulo two variables (malloc)
 * \param a The first variable
 * \param b The second variable
 * \return The result of the modulo
 */
W_Int *w_mod(void *a, void *b) {
    W_Type type_a = ((W_Var *)a)->type, type_b = ((W_Var *)b)->type;
    if (type_a == INT && type_b == INT) {
        W_Int *result = int_init();
        int_set(result, *int_value(a) % *int_value(b));
        return result;
    } else {
        char *type_a_str = w_get_type_str(a), *type_b_str = w_get_type_str(b);
        printf("Error: Unsupported types for modulo. (%s %% %s)\n", type_a_str, type_b_str);
        free(type_a_str);
        free(type_b_str);
        exit(1);
    }
}

/**
 * \brief Integer division two variables (malloc)
 * \param a The first variable
 * \param b The second variable
 * \return The result of the integer division
 */
W_Int *w_ediv(void *a, void *b) {
    W_Type type_a = ((W_Var *)a)->type, type_b = ((W_Var *)b)->type;
    if (type_a == INT && type_b == INT) {
        W_Int *result = int_init();
        int_set(result, (*int_value(a) - *int_value(a) % *int_value(b)) / *int_value(b));
        return result;
    } else {
        char *type_a_str = w_get_type_str(a), *type_b_str = w_get_type_str(b);
        printf("Error: Unsupported types for modulo. (%s %% %s)\n", type_a_str, type_b_str);
        free(type_a_str);
        free(type_b_str);
        exit(1);
    }
}

/**
 * \brief Raise a variable to the power of another variable (malloc)
 * \param a The first variable
 * \param b The second variable
 * \return The result of the power operation
 */
void *w_power(void *a, void *b) {
    W_Type type_a = ((W_Var *)a)->type, type_b = ((W_Var *)b)->type;
    if (type_a == INT && type_b == INT) {
        W_Int *result = int_init();
        int_set(result, (int)pow(*int_value(a), *int_value(b)));
        return result;
    } else if (type_a == FLOAT && type_b == INT) {
        W_Float *result = float_init();
        float_set(result, pow(*float_value(a), (double)*int_value(b)));
        return result;
    } else if (type_b == FLOAT && type_a == INT) {
        W_Float *result = float_init();
        float_set(result, pow((double)*int_value(a), *float_value(b)));
        return result;
    } else if (type_a == FLOAT && type_b == FLOAT) {
        W_Float *result = float_init();
        float_set(result, pow(*float_value(a), *float_value(b)));
        return result;
    } else {
        char *type_a_str = w_get_type_str(a), *type_b_str = w_get_type_str(b);
        printf("Error: Unsupported types for power operation. (%s ** %s)\n", type_a_str, type_b_str);
        free(type_a_str);
        free(type_b_str);
        exit(1);
    }
}

/**
 * \brief Square root a variable (malloc)
 * \param a The variable to square root
 * \return The result of the square root operation
 */
W_Float *w_sqrt(void *a) {
    W_Type type_a = ((W_Var *)a)->type;
    if (type_a == INT) {
        W_Float *result = float_init();
        float_set(result, sqrt((double)*int_value(a)));
        return result;
    } else if (type_a == FLOAT) {
        W_Float *result = float_init();
        float_set(result, sqrt(*float_value(a)));
        return result;
    } else {
        char *type_a_str = w_get_type_str(a);
        printf("Error: Unsupported type for square root operation. (sqrt %s )\n", type_a_str);
        free(type_a_str);
        exit(1);
    }
}

/***********************************************
 * Variables ***********************************
 ***********************************************/

/**
 * \brief Initialize a variable (malloc)
 * \param type The type of the variable
 * \return The initialized variable
 */
void *w_var_init(W_Type type) {
    switch (type) {
        case INT:
            return int_init();
        case FLOAT:
            return float_init();
        case STRING:
            return str_init();
        case BOOL:
            return bool_init();
    }
}

