#include "interpreter.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: word.exe <path>\n", argv[0]);
        return 1;
    }
    system("pause"); //debug
    // create_temp_file("test.w"); //debug
    create_temp_file(argv[1]);
    FILE *temp = fopen("exec.tmp", "r");
    if (temp == NULL) {
        printf("Error: Could not create temp file\n");
        return 1;
    }
    W_List *lexed_code = word_tokenize(temp);
    // word_print(lexed_code); //debug
    if (fclose(temp) != 0) {
        printf("Error: Could not close temp file\n");
    }
    W_List *parsed_code = parse(lexed_code);
    // print_parsed_code(parsed_code); //debug

    parser_destroy(parsed_code);
    system("pause"); //debug

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
    dict_destroy(args);

    W_List_Element *current_line = parsed_code->head;
    for (int i = 0; i < parsed_code->size; i++) {
        W_List *line = (W_List *)current_line->value;
        W_List_Element *current_word = line->head;
        W_Word *word = (W_Word *)current_word->value;
        for (int i_ = 0; i_ < line->size; i_++) {
            if (strcmp(word->value, "def") == 0) {
                if (i_ != 0) {
                    printf("Error: def must be at the beginning of the line, line: %d\n", word->line);
                    exit(1);
                }
                W_Func *f = func_init();
                W_Dict *args = dict_init();
                current_word = current_word->next;
                word = current_word->value;
                bool type = is_type_keyword(word->value);
                if (!type) {
                    printf("Error: Expected type keyword after 'def', line: %d\n", word->line);
                    exit(1);
                }
                if (strcmp(word->value, "void") == 0) {
                    f->return_type = NULL_TYPE;
                } else {
                    f->return_type = w_get_type(word->value);
                }
                current_word = current_word->next;
                word = current_word->value;
                char *name = word->value;
                i_ += 2;
                current_word = current_word->next;
                word = current_word->value;

                for (i_ ; i_ < line->size; i_++) {
                    current_word = current_word->next;
                    word = current_word->value;

                }
            }
        }
        current_line = current_line->next;
    }
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