#include <stdio.h>
#include "word.h"

W_List *word_tokenize(FILE *source) {
    W_List *words = list_init();
    int start = 0; //the start of the word in the line
    int i = 0, line = 0;
    char c = fgetc(source);

    W_Word *w = (W_Word *)malloc(sizeof(W_Word));
    while (c != EOF) {
        W_List *line = list_init();
        while (c == '\n') {
            if (c == '\"') {
                while (c != '\"') {
                    c = fgetc(source);
                    i++;
                }
                c = fgetc(source);
            }
            if (c == ' ' || c == '\t') {
                fseek(source, start, SEEK_SET);
                char *value = (char *)malloc(i - start);
                fread(value, 1, i - start, source);
                w->value = value;
                w->type = word_type(value);
                w->line = line;
                list_append(words, w);
                start = i;
                W_Word *w = (W_Word *)malloc(sizeof(W_Word));
                w->line = line;
            }
            c = fgetc(source);
            i++;
        }
        line++;
        while (c == ' ' || c == '\t') {
            c = fgetc(source);
        }
    }
    return words;
}

W_Word_Type word_type(char *value) {
    if (value[0] == '\"' && value[strlen(value) - 1] == '\"') {
        return LITERAL;
    }
    if (value[0] == '\'' && value[strlen(value) - 1] == '\'') {
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