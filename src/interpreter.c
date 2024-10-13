#include "interpreter.h"

// int main(int argc, char *argv[]) {
//     if (argc < 2) {
//         printf("Usage: %s <file>\n", argv[0]);
//         return 1;
//     }
//     create_temp_file(argv[1]);
//     // create_temp_file("test.w"); //debug
//     FILE *temp = fopen("exec.tmp", "r");
//     if (temp == NULL) {
//         printf("Error: Could not create temp file\n");
//         return 1;
//     }
//     W_List *lexed_code = word_tokenize(temp);
//     // word_print(lexed_code); //debug
//     if (fclose(temp) != 0) {
//         printf("Error: Could not close temp file\n");
//     }
//     W_List *parsed_code = parse(lexed_code);
//     print_parsed_code(parsed_code); //debug
//     parser_destroy(parsed_code);
//     if (remove("exec.tmp") != 0) {
//         printf("Error: Could not delete temp file\n");
//         return 1;
//     }
//     return 0;
// }

int main(int argc, char *argv[]) {
    //testing
    W_Int *a = int_init();
    int_set(a, 5);
    W_Int *b = int_init();
    int_set(b, 3);
    W_Int *c = w_minus(a, b);
    printf("value: %d\n", *int_value(c));
    return 0;
}

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