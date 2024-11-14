#include "interpreter.h"

static void init_keywords();

int main(int argc, char *argv[]) {
    keywords = dict_init();
    init_keywords();

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
    dict_destroy(keywords);

    if (DEBUG) printf("Done\n"); //DEBUG
    if (MONITOR_MEMORY) w_alloc_print();
    return 0;
}


/**
 * \brief Initializes the keywords.
 */
static void init_keywords() {
    add_keyword("plus", kw_plus, 2, 1);
    add_keyword("minus", kw_minus, 2, 1);
    add_keyword("time", kw_time, 2, 2);
    add_keyword("div", kw_div, 2, 2);
    add_keyword("mod", kw_mod, 2, 2);
    add_keyword("ediv", kw_ediv, 2, 2);
    add_keyword("power", kw_power, 2, 3);
    add_keyword("sqrt", kw_sqrt, 1, 3);

    add_keyword("and", kw_and, 2, 1);
    add_keyword("or", kw_or, 2, 1);
    add_keyword("not", kw_not, 1, 1);

    add_keyword("equal", kw_equal, 2, 1);
    add_keyword("greater", kw_greater, 2, 1);
    add_keyword("less", kw_less, 2, 1);
    add_keyword("gequal", kw_gequal, 2, 1);
    add_keyword("lequal", kw_lequal, 2, 1);

    add_keyword("int", kw_int, 1, 1);
    add_keyword("float", kw_float, 1, 1);
    add_keyword("str", kw_str, 1, 1);
    add_keyword("bool", kw_bool, 1, 1);
    add_keyword("list", kw_list, 1, 1);

    add_keyword("assign", kw_assign, 2, 0);
    add_keyword("delete", kw_delete, 1, 0);

    add_keyword("print", kw_print, -1, -1);
    add_keyword("ask", kw_ask, 1, -1);

    add_keyword("def", kw_def, -1, 0);
}
