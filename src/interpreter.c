#include "interpreter.h"

int main(int argc, char *argv[]) {

    keywords = w_dict_init();

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
static void *execute(list *parsed_code, Scope *scope, W_Type return_type, bool destroy_scope_on_exit) {

}

// Utility

/**
 * \brief Initializes the keywords.
 */
static void init_keywords() {

}
