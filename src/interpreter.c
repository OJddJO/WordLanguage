#include "interpreter.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <file>\n", argv[0]);
        return 1;
    }
    FILE *source = fopen(argv[1], "r");
    if (source == NULL) {
        printf("Error: Could not open file %s\n", argv[1]);
        return 1;
    }
    W_List *words = word_tokenize(source);
    word_print(words);
    word_destroy(words);
    fclose(source);
    return 0;
}

W_List *word_tokenize(FILE *source) {
    W_List *words = list_init();
    int start = 0; //the start of the word in the line
    int i = 0, n_line = 0;
    char c = fgetc(source);

    W_Word *w = (W_Word *)malloc(sizeof(W_Word));
    while (c != EOF) {
        W_List *line = list_init();
        while (c != '\n') {
            if (c == '\"' || (c >= '0' && c <= '9')) {
                printf("Start litt, c:%c, i:%d, start: %d\n", c, i, start);
                c = fgetc(source);
                i++;
                while (c != '\"' || (c >= '0' && c <= '9')) {
                    printf("Litt, c:%c, i:%d\n", c, i);
                    c = fgetc(source);
                    i++;
                }
                printf("End litt, c:%c, i:%d\n", c, i);
            }
            if (c == ' ' || c == '\t' || c == '\n' || c == EOF) {
                fseek(source, start, SEEK_SET);
                char *value = (char *)malloc(i - start + 1);
                fread(value, 1, i - start, source);
                value[i - start] = '\0';
                w->value = value;
                w->type = word_type(value);
                w->line = n_line;
                list_append(words, w);
                w = (W_Word *)malloc(sizeof(W_Word));
                fseek(source, i, SEEK_SET);
                while (c == ' ' || c == '\t') {
                    c = fgetc(source);
                    i++;
                    printf("Skip, c:%c, i:%d\n", c, i);
                }
                start = i - 1;
            } else {
                printf("Next, c:%c, i:%d\n", c, i);
                c = fgetc(source);
                i++;
            }
        }
        line++;
        while (c == ' ' || c == '\t' || c == '\n') {
            if (c == '\n') {
                n_line++;
            }
            c = fgetc(source);
            i++;
        }
    }
    return words;
}

W_Word_Type word_type(char *value) {
    if (value[0] == '\"' && value[strlen(value)] == '\"') {
        return LITERAL;
    }
    if ((value[0] >= '0' && value[0] <= '9') && (value[strlen(value) - 1] >= '0' && value[strlen(value) - 1] <= '9')) {
        return LITERAL;
    }
    for (int i = 0; i < sizeof(keywords) / sizeof(keywords[0]); i++) {
        if (strcmp(value, keywords[i]) == 0) {
            return KEYWORD;
        }
    }
    return IDENTIFIER;
}

/**
 * \brief Destroys the given list of words.
 * \param words The list of words to destroy.
 * \return void
 */
void word_destroy(W_List *words) {
    W_ListElement *current = words->head;
    while (current != NULL) {
        W_Word *w = (W_Word *)current->value;
        free(w->value);
        free(w);
        current = current->next;
    }
    list_destroy(words);
}

/**
 * \brief Prints the given list of words.
 * \param words The list of words to print.
 * \return void
 */
void word_print(W_List *words) {
    W_ListElement *current = words->head;
    while (current != NULL) {
        W_Word *w = (W_Word *)current->value;
        printf("Word: %s | ", w->value);
        if (w->type == KEYWORD) {
            printf("Type: Keyword\n");
        } else if (w->type == IDENTIFIER) {
            printf("Type: Identifier\n");
        } else if (w->type == LITERAL) {
            printf("Type: Literal\n");
        }
        current = current->next;
    }
}