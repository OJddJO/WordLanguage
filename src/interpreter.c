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
    execute(parsed_code, dict_init());

    if (remove("exec.tmp") != 0) {
        printf("Error: Could not delete temp file\n");
        return 1;
    }
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
    fclose(source);
    fclose(temp);
}

/**
 * \brief Executes the parsed code.
 * \param parsed_code The parsed code to execute.
 * \param args The arguments to pass to the code.
 * \return The result of the execution
 */
void *execute(W_List *parsed_code, W_Dict *args) {
    W_Dict *variables = dict_init();

    W_List *args_keys = dict_keys(args);
    W_List_Element *current_key = args_keys->head;
    for (int i = 0; i < args_keys->size; i++) { //add arguments to variables
        dict_set(variables, current_key->value, dict_get(args, current_key->value));
        current_key = current_key->next;
    }
    free(args->keys);
    free(args->values);
    free(args);

    W_List_Element *current_line = parsed_code->head;
    for (int i = 0; i < parsed_code->size; i++) {
        W_List *line = (W_List *)current_line->value;
        W_List_Element *parsed_line = line->head;

        W_List *stack = list_init(); //the stack of words to evaluate
        for (int i_ = 0; i_ < line->size; i_++) {
            W_List *parsed_words = (W_List *)parsed_line->value;
            W_List_Element *current_word = parsed_words->head;
            if (parsed_words->size == 1) { //not an operation
                list_append(stack, current_word->value); //add to stack
            } else { //if the size of parsed_line is not 1 then evaluate operation
                //TODO: evaluate operation
            }
            parsed_line = parsed_line->next;
        }

        W_List_Element *current_word = stack->head;
        W_Word *word = (W_Word *)current_word->value;

        if (strcmp(word->value, "def") == 0) { //function definition
            W_Func *f = func_init(); //create function
            W_Dict *args = dict_init(); //create arguments dictionary
            f->type = FUNCTION;
            f->args = args;

            //set return type
            current_word = current_word->next;
            word = current_word->value;
            printf("Return type: %s\n", word->value); //debug
            bool is_type = is_type_keyword(word->value);
            if (!is_type) {
                printf("Error: Expected type keyword after 'def', line: %d\n", word->line);
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
                    word = current_word->value;
                    if (strcmp(word->value, "do") == 0) break;
                    if (!is_type_keyword(word->value)) {
                        printf("Error: Expected type keyword in function arguments, line: %d\n", word->line);
                        exit(1);
                    } else {
                        W_Type *arg_type = (W_Type *)malloc(sizeof(W_Type));
                        *arg_type = w_get_type(word->value);
                        current_word = current_word->next;
                        word = current_word->value;
                        dict_set(args, word->value, arg_type);
                    }
                }
            } else if (strcmp(word->value, "do") != 0) {
                printf("Error: Expected 'with' or 'do' after function name, line: %d\n", word->line);
                exit(1);
            }
            current_word = current_word->next;
            if (current_word != NULL) {
                printf("Error: Expected end of line after 'do' keyword, line: %d\n", word->line);
                exit(1);
            }
            dict_set(variables, name, f);
            dict_print(args);
        }
        free(stack);
        current_line = current_line->next;
    }
    dict_destroy(variables);
    parser_destroy(parsed_code);
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