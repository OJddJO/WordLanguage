#include "w_stdlib.h"

/***********************************************
 * Global **************************************
 ***********************************************/

/**
 * \brief Get the type of a variable as a string (w_malloc)
 * \param var The variable to get the type of
 * \return The type of the variable
 */
char *w_get_type_str(W_Var *var) {
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
 * Math Operations *****************************
 ***********************************************/

/**
 * \brief Add two variables (w_malloc)
 * \param a The first variable
 * \param b The second variable
 * \return The result of the addition
 */
void *w_plus(W_Var *a, W_Var *b) {
    W_Type type_a = a->type, type_b = b->type;
    if (type_a == INT && type_b == INT) {
        W_Int *result = w_int_init();
        w_int_set(result, *w_int_value((W_Int *)a) + *w_int_value((W_Int *)b));
        return result;
    } else if (type_a == FLOAT && type_b == INT) {
        W_Float *result = w_float_init();
        w_float_set(result, *w_float_value((W_Float *)a) + *w_int_value((W_Int *)b));
        return result;
    } else if (type_b == FLOAT && type_a == INT) {
        W_Float *result = w_float_init();
        w_float_set(result, *w_int_value((W_Int *)a) + *w_float_value((W_Float *)b));
        return result;
    } else if (type_a == FLOAT && type_b == FLOAT) {
        W_Float *result = w_float_init();
        w_float_set(result, *w_float_value((W_Float *)a) + *w_float_value((W_Float *)b));
        return result;
    } else {
        char *type_a_str = w_get_type_str(a), *type_b_str = w_get_type_str(b);
        printf("Error: Unsupported types for addition. (%s + %s)\n", type_a_str, type_b_str);
        w_free(type_a_str);
        w_free(type_b_str);
        exit(1);
    }
}

/**
 * \brief Subtract two variables (w_malloc)
 * \param a The first variable
 * \param b The second variable
 * \return The result of the subtraction
 */
void *w_minus(W_Var *a, W_Var *b) {
    W_Type type_a = a->type, type_b = b->type;
    if (type_a == INT && type_b == INT) {
        W_Int *result = w_int_init();
        w_int_set(result, *w_int_value((W_Int *)a) - *w_int_value((W_Int *)b));
        return result;
    } else if (type_a == FLOAT && type_b == INT) {
        W_Float *result = w_float_init();
        w_float_set(result, *w_float_value((W_Float *)a) - *w_int_value((W_Int *)b));
        return result;
    } else if (type_b == FLOAT && type_a == INT) {
        W_Float *result = w_float_init();
        w_float_set(result, *w_int_value((W_Int *)a) - *w_float_value((W_Float *)b));
        return result;
    } else if (type_a == FLOAT && type_b == FLOAT) {
        W_Float *result = w_float_init();
        w_float_set(result, *w_float_value((W_Float *)a) - *w_float_value((W_Float *)b));
        return result;
    } else {
        char *type_a_str = w_get_type_str(a), *type_b_str = w_get_type_str(b);
        printf("Error: Unsupported types for subtraction. (%s - %s)\n", type_a_str, type_b_str);
        w_free(type_a_str);
        w_free(type_b_str);
        exit(1);
    }
}

/**
 * \brief Multiply two variables (w_malloc)
 * \param a The first variable
 * \param b The second variable
 * \return The result of the multiplication
 */
void *w_time(W_Var *a, W_Var *b) {
    W_Type type_a = a->type, type_b = b->type;
    if (type_a == INT && type_b == INT) {
        W_Int *result = w_int_init();
        w_int_set(result, *w_int_value((W_Int *)a) * *w_int_value((W_Int *)b));
        return result;
    } else if (type_a == FLOAT && type_b == INT) {
        W_Float *result = w_float_init();
        w_float_set(result, *w_float_value((W_Float *)a) * *w_int_value((W_Int *)b));
        return result;
    } else if (type_b == FLOAT && type_a == INT) {
        W_Float *result = w_float_init();
        w_float_set(result, *w_int_value((W_Int *)a) * *w_float_value((W_Float *)b));
        return result;
    } else if (type_a == FLOAT && type_b == FLOAT) {
        W_Float *result = w_float_init();
        w_float_set(result, *w_float_value((W_Float *)a) * *w_float_value((W_Float *)b));
        return result;
    } else {
        char *type_a_str = w_get_type_str(a), *type_b_str = w_get_type_str(b);
        printf("Error: Unsupported types for multiplication. (%s * %s)\n", type_a_str, type_b_str);
        w_free(type_a_str);
        w_free(type_b_str);
        exit(1);
    }
}

/**
 * \brief Divide two variables (w_malloc)
 * \param a The first variable
 * \param b The second variable
 * \return The result of the division
 */
W_Float *w_div(W_Var *a, W_Var *b) {
    W_Type type_a = a->type, type_b = b->type;
    W_Float *result = w_float_init();
    if (type_a == INT && type_b == INT) {
        w_float_set(result, *w_int_value((W_Int *)a) / *w_int_value((W_Int *)b));
    } else if (type_a == FLOAT && type_b == INT) {
        w_float_set(result, *w_float_value((W_Float *)a) / *w_int_value((W_Int *)b));
    } else if (type_b == FLOAT && type_a == INT) {
        w_float_set(result, *w_int_value((W_Int *)a) / *w_float_value((W_Float *)b));
    } else if (type_a == FLOAT && type_b == FLOAT) {
        w_float_set(result, *w_float_value((W_Float *)a) / *w_float_value((W_Float *)b));
    } else {
        char *type_a_str = w_get_type_str(a), *type_b_str = w_get_type_str(b);
        printf("Error: Unsupported types for division. (%s / %s)\n", type_a_str, type_b_str);
        w_free(type_a_str);
        w_free(type_b_str);
        exit(1);
    }
    return result;
}

/**
 * \brief Modulo two variables (w_malloc)
 * \param a The first variable
 * \param b The second variable
 * \return The result of the modulo
 */
W_Int *w_mod(W_Var *a, W_Var *b) {
    W_Type type_a = a->type, type_b = b->type;
    if (type_a == INT && type_b == INT) {
        W_Int *result = w_int_init();
        w_int_set(result, *w_int_value((W_Int *)a) % *w_int_value((W_Int *)b));
        return result;
    } else {
        char *type_a_str = w_get_type_str(a), *type_b_str = w_get_type_str(b);
        printf("Error: Unsupported types for modulo. (%s %% %s)\n", type_a_str, type_b_str);
        w_free(type_a_str);
        w_free(type_b_str);
        exit(1);
    }
}

/**
 * \brief Integer division two variables (w_malloc)
 * \param a The first variable
 * \param b The second variable
 * \return The result of the integer division
 */
W_Int *w_ediv(W_Var *a, W_Var *b) {
    W_Type type_a = a->type, type_b = b->type;
    if (type_a == INT && type_b == INT) {
        W_Int *result = w_int_init();
        w_int_set(result, (*w_int_value((W_Int *)a) - *w_int_value((W_Int *)a) % *w_int_value((W_Int *)b)) / *w_int_value((W_Int *)b));
        return result;
    } else {
        char *type_a_str = w_get_type_str(a), *type_b_str = w_get_type_str(b);
        printf("Error: Unsupported types for ediv. (%s // %s)\n", type_a_str, type_b_str);
        w_free(type_a_str);
        w_free(type_b_str);
        exit(1);
    }
}

/**
 * \brief Raise a variable to the power of another variable (w_malloc)
 * \param a The first variable
 * \param b The second variable
 * \return The result of the power operation
 */
void *w_power(W_Var *a, W_Var *b) {
    W_Type type_a = a->type, type_b = b->type;
    if (type_a == INT && type_b == INT) {
        W_Int *result = w_int_init();
        w_int_set(result, (int)pow(*w_int_value((W_Int *)a), *w_int_value((W_Int *)b)));
        return result;
    } else if (type_a == FLOAT && type_b == INT) {
        W_Float *result = w_float_init();
        w_float_set(result, pow(*w_float_value((W_Float *)a), *w_int_value((W_Int *)b)));
        return result;
    } else if (type_b == FLOAT && type_a == INT) {
        W_Float *result = w_float_init();
        w_float_set(result, pow(*w_int_value((W_Int *)a), *w_float_value((W_Float *)b)));
        return result;
    } else if (type_a == FLOAT && type_b == FLOAT) {
        W_Float *result = w_float_init();
        w_float_set(result, pow(*w_float_value((W_Float *)a), *w_float_value((W_Float *)b)));
        return result;
    } else {
        char *type_a_str = w_get_type_str(a), *type_b_str = w_get_type_str(b);
        printf("Error: Unsupported types for power operation. (%s ** %s)\n", type_a_str, type_b_str);
        w_free(type_a_str);
        w_free(type_b_str);
        exit(1);
    }
}

/**
 * \brief Square root a variable (w_malloc)
 * \param a The variable to square root
 * \return The result of the square root operation
 */
W_Float *w_sqrt(W_Var *a) {
    W_Type type_a = a->type;
    if (type_a == INT || type_a == FLOAT) {
        W_Float *result = w_float_init();
        w_float_set(result, sqrt(*(float *)a->get(a)));
        return result;
    } else {
        char *type_a_str = w_get_type_str(a);
        printf("Error: Unsupported type for square root operation. (sqrt %s )\n", type_a_str);
        w_free(type_a_str);
        exit(1);
    }
}

/***********************************************
 * Logical Operations **************************
 ***********************************************/

/**
 * \brief Logical AND two variables (w_malloc)
 * \param a The first variable
 * \param b The second variable
 * \return The result of the AND operation
 */
W_Bool *w_and (W_Var *a, W_Var *b) {
    W_Bool *result = w_bool_init();
    w_bool_set(result, *(bool *)a->get(a) && *(bool *)b->get(b));
    return result;
}

/**
 * \brief Logical OR two variables (w_malloc)
 * \param a The first variable
 * \param b The second variable
 * \return The result of the OR operation
 */
W_Bool *w_or (W_Var *a, W_Var *b) {
    W_Bool *result = w_bool_init();
    w_bool_set(result, *(bool *)a->get(a) || *(bool *)b->get(b));
    return result;
}

/**
 * \brief Logical NOT a variable (w_malloc)
 * \param a The variable to NOT
 * \return The result of the NOT operation
 */
W_Bool *w_not (W_Var *a) {
    W_Bool *result = w_bool_init();
    w_bool_set(result, !*(bool *)a->get(a));
    return result;
}

/***********************************************
 * Comparison Operations ***********************
 ***********************************************/

/**
 * \brief Compare two variables for equality
 * \param a The first variable
 * \param b The second variable
 * \return The result of the comparison
 */
W_Bool *w_equal(W_Var *a, W_Var *b) {
    W_Bool *result = w_bool_init();
    W_Type type_a = a->type, type_b = b->type;
    if ((type_a != INT && type_a != FLOAT && type_a != STRING && type_a != BOOL && type_a != LIST) \
        || (type_b != INT && type_b != FLOAT && type_b != STRING && type_b != BOOL && type_b != LIST)) {
        char *type_a_str = w_get_type_str(a), *type_b_str = w_get_type_str(b);
        printf("Error: Unsupported types for equality comparison. (%s == %s)\n", type_a_str, type_b_str);
        w_free(type_a_str);
        w_free(type_b_str);
        exit(1);
    }
    if (type_a == type_b) {
        if (type_a == INT) {
            w_bool_set(result, *w_int_value((W_Int *)a) == *w_int_value((W_Int *)b));
        } else if (type_a == FLOAT) {
            w_bool_set(result, *w_float_value((W_Float *)a) == *w_float_value((W_Float *)b));
        } else if (type_a == STRING) {
            w_bool_set(result, strcmp(w_str_value((W_Str *)a), w_str_value((W_Str *)b)) == 0);
        } else if (type_a == BOOL) {
            w_bool_set(result, w_bool_value((W_Bool *)a) == w_bool_value((W_Bool *)b));
        } else if (type_a == LIST) {
            if (w_list_size((W_List *)a) != w_list_size((W_List *)b)) {
                w_bool_set(result, false);
            } else {
                W_List_Element *e1 = ((W_List *)a)->head, *e2 = ((W_List *)b)->head;
                for (int i = 0; i < w_list_size((W_List *)a); i++) {
                    if (!w_equal(e1->value, e2->value)) { //recursion
                        w_bool_set(result, false);
                        break;
                    }
                    e1 = e1->next;
                    e2 = e2->next;
                }
                w_bool_set(result, true);
            }
        }
    } else w_bool_set(result, false);
    return result;
}

/**
 * \brief Compare two variables for greater than
 * \param a The first variable
 * \param b The second variable
 * \return The result of the comparison
 */
W_Bool *w_greater(W_Var *a, W_Var *b) {
    W_Bool *result = w_bool_init();
    W_Type type_a = a->type, type_b = b->type;
    if ((type_a != INT && type_a != FLOAT) || (type_b != INT && type_b != FLOAT)) {
        char *type_a_str = w_get_type_str(a), *type_b_str = w_get_type_str(b);
        printf("Error: Unsupported types for greater comparison. (%s > %s)\n", type_a_str, type_b_str);
        w_free(type_a_str);
        w_free(type_b_str);
        exit(1);
    }
    w_bool_set(result, *(float *)a->get(a) > *(float *)b->get(b));
    return result;
}

/**
 * \brief Compare two variables for less than
 * \param a The first variable
 * \param b The second variable
 * \return The result of the comparison
 */
W_Bool *w_less(W_Var *a, W_Var *b) {
    W_Bool *result = w_bool_init();
    W_Type type_a = a->type, type_b = b->type;
    if ((type_a != INT && type_a != FLOAT) || (type_b != INT && type_b != FLOAT)) {
        char *type_a_str = w_get_type_str(a), *type_b_str = w_get_type_str(b);
        printf("Error: Unsupported types for less comparison. (%s < %s)\n", type_a_str, type_b_str);
        w_free(type_a_str);
        w_free(type_b_str);
        exit(1);
    }
    w_bool_set(result, *(float *)a->get(a) < *(float *)b->get(b));
    return result;
}

/**
 * \brief Compare two variables for greater than or equal to
 * \param a The first variable
 * \param b The second variable
 * \return The result of the comparison
 */
W_Bool *w_gequal(W_Var *a, W_Var *b) {
    W_Bool *result = w_bool_init();
    W_Type type_a = a->type, type_b = b->type;
    if ((type_a != INT && type_a != FLOAT) || (type_b != INT && type_b != FLOAT)) {
        char *type_a_str = w_get_type_str(a), *type_b_str = w_get_type_str(b);
        printf("Error: Unsupported types for greater or equal comparison. (%s >= %s)\n", type_a_str, type_b_str);
        w_free(type_a_str);
        w_free(type_b_str);
        exit(1);
    }
    w_bool_set(result, *(float *)a->get(a) >= *(float *)b->get(b));
    return result;
}

/**
 * \brief Compare two variables for less than or equal to
 * \param a The first variable
 * \param b The second variable
 * \return The result of the comparison
 */
W_Bool *w_lequal(W_Var *a, W_Var *b) {
    W_Bool *result = w_bool_init();
    W_Type type_a = a->type, type_b = b->type;
    if ((type_a != INT && type_a != FLOAT) || (type_b != INT && type_b != FLOAT)) {
        char *type_a_str = w_get_type_str(a), *type_b_str = w_get_type_str(b);
        printf("Error: Unsupported types for less or equal comparison. (%s <= %s)\n", type_a_str, type_b_str);
        w_free(type_a_str);
        w_free(type_b_str);
        exit(1);
    }
    w_bool_set(result, *(float *)a->get(a) <= *(float *)b->get(b));
    return result;
}

/***********************************************
 * Variables ***********************************
 ***********************************************/

/**
 * \brief Initialize a variable (w_malloc)
 * \param type The type of the variable
 * \return The initialized variable
 */
void *w_var_init(W_Type type) {
    switch (type) {
        case INT:
            return w_int_init();
        case FLOAT:
            return w_float_init();
        case STRING:
            return w_str_init();
        case BOOL:
            return w_bool_init();
    }
}

