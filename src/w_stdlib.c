#include "w_stdlib.h"

char *w_get_type(W_Var *var) {
    char *type = (char *)malloc(11);
    switch (var->type) {
        case INT:
            strcpy(type, "<int>");
            break;
        case FLOAT:
            strcpy(type, "<float>");
            break;
        case STRING:
            strcpy(type, "<str>");
            break;
        case BOOL:
            strcpy(type, "<bool>");
            break;
        case LIST:
            strcpy(type, "<list>");
            break;
        case ARRAY:
            strcpy(type, "<array>");
            break;
        case DICT:
            strcpy(type, "<dict>");
            break;
        case FUNCTION:
            strcpy(type, "<function>");
            break;
        case NULL_TYPE:
            strcpy(type, "<null>");
            break;
        default:
            strcpy(type, "<unknown>");
            break;
    }
    return type;
}

/**
 * \brief Add two variables
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
        char *type_a_str = w_get_type(a), *type_b_str = w_get_type(b);
        printf("Error: Unsupported types for addition. (%s + %s)\n", type_a_str, type_b_str);
        free(type_a_str);
        free(type_b_str);
        exit(1);
    }
}

/**
 * \brief Subtract two variables
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
        char *type_a_str = w_get_type(a), *type_b_str = w_get_type(b);
        printf("Error: Unsupported types for subtraction. (%s - %s)\n", type_a_str, type_b_str);
        free(type_a_str);
        free(type_b_str);
        exit(1);
    }
}