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

W_List *lexer(FILE *source) {
    W_List *code = list_init();
    int start = 0; //start of the word
    int i = -1; //cursor of the file
    int n_line = 1;
    char c;
    int eval_litt_str = 0; //if the word to eval is a litteral str
    int eval_litt_number = 0; // if the word to eval is a litteral int, float

    W_Word *w = (W_Word *)malloc(sizeof(W_Word));
    W_List *line = list_init();
    while (c != EOF) {
        c = fgetc(source);
        i++;
        printf("c: %c, i: %d, start:%d, line: %d", c, i, start, n_line);
        if (c == ' ' || c == '\n') {
            if (start != i) {
                fseek(source, start, SEEK_SET);
                char *value = (char *)malloc(i - start + 1);
                fread(value, 1, i - start, source);
                value[i - start] = '\0';
                w->value = value;
                w->type = word_type(value);
                w->line = n_line;
                list_append(line, w);
                w = (W_Word *)malloc(sizeof(W_Word));
                fseek(source, i, SEEK_SET);
            }
            start = i;
        } else if (c == '\"') {
            if (eval_litt_str) eval_litt_str = 0;
            else eval_litt_str = 1;
        }
        if (c == '\n') {
            line = list_init();
        }
    }
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
    W_List_Element *current = words->head;
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
 * \param code The list of list of words to print.
 * \return void
 */
void word_print(W_List *code) {
    W_List_Element *current_element = code->head;
    W_List *current_line = current_element->value;
    W_List_Element *current_word = current_line->head;
    while (current_line != NULL) {
        while (current_word != NULL) {
            W_Word *w = (W_Word *)current_word->value;
            printf("Word: %s | ", w->value);
            if (w->type == KEYWORD) {
                printf("Type: Keyword\n");
            } else if (w->type == IDENTIFIER) {
                printf("Type: Identifier\n");
            } else if (w->type == LITERAL) {
                printf("Type: Literal\n");
            }
            current_word = current_word->next;
        }
        current_element = current_element->next;
        current_line = current_element->value;
    }
}