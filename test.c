#include "interpreter.h"

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
