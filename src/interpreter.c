#include "interpreter.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <file>\n", argv[0]);
        return 1;
    }
    create_temp_file(argv[1]);
    FILE *temp = fopen(".tmp", "r");
    if (temp == NULL) {
        printf("Error: Could not create temp file\n");
        return 1;
    }
    W_List *words = word_tokenize(temp);
    word_print(words);
    word_destroy(words);
    fclose(temp);
    if (remove(".tmp") != 0) {
        printf("Error: Could not delete temp file\n");
        return 1;
    }
    return 0;
}

/**
 * \brief Creates a temporary file from the given file.
 * \param filename The file to create a temporary file from.
 * \return The temporary file.
 */
void create_temp_file(char *filename) {
    FILE *source = fopen(filename, "r");
    if (source == NULL) printf("Error: Could not open file: %s\n", filename);
    FILE *temp = fopen(".tmp", "w");
    if (temp == NULL) printf("Error: Could not open temp file\n");
    char c ;
    while ((c = fgetc(source)) != EOF) {
        fputc(c, temp);
    }
    fclose(source);
    fclose(temp);
}

/*****************************************************
 * Lexer *********************************************
 *****************************************************/

/**
 * \brief Tokenizes the given file into a list of words.
 * \param source The file to tokenize.
 * \return A list of list of words.
 */
W_List *word_tokenize(FILE *source) {
    W_List *code = list_init();
    int start = 0; //start of the word
    int i = 0; //cursor of the file
    int n_line = 1;
    char c = fgetc(source);
    int eval = 0; //if there is a word to eval
    int eval_litt_str = 0; //if the word to eval is a litteral str
    int eval_litt_number = 0; // if the word to eval is a litteral int, float

    W_Word *w = (W_Word *)malloc(sizeof(W_Word));
    W_List *line = list_init();
    list_append(code, line);
    while (c != EOF) {
        if (c != ' ' && c != '\t' && c != '\n' && !eval) {
            eval = 1;
            start = i;
        }
        if ((c == ' ' || c == '\t' || c == '\n' || c == EOF) && !eval_litt_str && eval) {
            fseek(source, start, SEEK_SET);
            char *value = (char *)malloc(i - start + 1);
            fread(value, 1, i - start, source);
            value[i - start] = '\0';
            w->value = value;
            // printf("value: %s\n", value); //debug
            w->type = word_type(value);
            w->line = n_line;
            list_append(line, w);
            w = (W_Word *)malloc(sizeof(W_Word));
            fseek(source, i+1, SEEK_SET);
            eval = 0;
        } else if (c == '\"' || c == '\'') {
            if (eval_litt_str) eval_litt_str = 0;
            else eval_litt_str = 1;
        } else if ((c >= '0' && c <= '9') || c == '.') {
            if (eval_litt_number) eval_litt_number = 0;
            else eval_litt_number = 1;
        }
        if (c == '\n' && !eval_litt_str) {
            line = list_init();
            list_append(code, line);
            n_line++;
            c = fgetc(source);
            i++;
        }
        // printf("c: %c, i: %d, start:%d, line: %d, eval: %d\n", c, i, start, n_line, eval); //debug
        c = fgetc(source);
        i++;
    }
    if (eval) {
        fseek(source, start, SEEK_SET);
        char *value = (char *)malloc(i - start + 1);
        fread(value, 1, i - start, source);
        value[i - start] = '\0';
        w->value = value;
        w->type = word_type(value);
        w->line = n_line;
        list_append(line, w);
    }
    return code;
}

/**
 * \brief Returns the type of the given word.
 * \param value The word to evaluate.
 * \return The type of the word.
 */
W_Word_Type word_type(char *value) {
    if ((value[0] == '\"' && value[strlen(value) - 1] == '\"') || (value[0] == '\'' && value[strlen(value) - 1] == '\'')) {
        return STR;
    }
    if ((value[0] >= '0' && value[0] <= '9') && (value[strlen(value) - 1] >= '0' && value[strlen(value) - 1] <= '9')) {
        return NUMBER;
    }
    for (int i = 0; i < sizeof(keywords) / sizeof(keywords[0]); i++) {
        if (strcmp(value, keywords[i]) == 0) {
            return KEYWORD;
        }
    }
    int dot = 0;
    for (int i = 0; i < strlen(value); i++) {
        if (value[i] == '.') {
            dot++;
        } else break;
    }
    char without_dot[strlen(value) - dot];
    strncpy(without_dot, value + dot, strlen(value) - dot);
    // printf("value: %s, dot: %d, without_dot: %s\n", value, dot, without_dot); //debug
    for (int i = 0; i < sizeof(operators) / sizeof(operators[0]); i++) {
        if (strcmp(without_dot, operators[i]) == 0) {
            return OPERATOR;
        }
    }
    return IDENTIFIER;
}

/**
 * \brief Destroys the given list of words.
 * \param code The list of list of words to destroy.
 * \return void
 */
void word_destroy(W_List *code) {
    W_List_Element *current_line = code->head;
    W_List *line = (W_List *)current_line->value;
    W_List_Element *current_word = line->head;
    for (int i = 0; i < code->size; i++) {
        while (current_word != NULL) {
            W_Word *w = (W_Word *)current_word->value;
            free(w->value);
            free(w);
            current_word = current_word->next;
        }
        if (current_line->next != NULL) {
            current_line = current_line->next;
            line = (W_List *)current_line->value;
            current_word = line->head;
        }
    }
    list_destroy(code);
}

/**
 * \brief Prints the given list of words.
 * \param code The list of list of words to print.
 * \return void
 */
void word_print(W_List *code) { //debug
    int i = 1; //line number
    W_List_Element *current_line = code->head;
    W_List *line = (W_List *)current_line->value;
    W_List_Element *current_word = line->head;
    for (int i = 0; i < code->size; i++) {
        printf("Line %d: ", i + 1);
        printf("Line size: %d\n", line->size);
        while (current_word != NULL) {
            W_Word *w = (W_Word *)current_word->value;
            printf("Word: %s | ", w->value);
            if (w->type == KEYWORD) {
                printf("Type: KEYWORD ");
            } else if (w->type == OPERATOR) {
                printf("Type: OPERATOR ");
            } else if (w->type == IDENTIFIER) {
                printf("Type: IDENTIFIER ");
            } else if (w->type == STR) {
                printf("Type: STRING ");
            } else if (w->type == NUMBER) {
                printf("Type: NUMBER ");
            }
            printf("| Line: %d\n", w->line);
            current_word = current_word->next;
        }
        printf("\n");
        if (current_line->next != NULL) {
            current_line = current_line->next;
            line = (W_List *)current_line->value;
            current_word = line->head;
        }
    }
}