#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "piles.h"

int execute_line(char *line, struct pile *p) {
    
}

int main(void) {
    char *code = "Hello World\nprint oy\n int var assign 10\nBruh not working";
    size_t code_len = strlen(code);
    int num_lines = 0;
    for (size_t i = 0; i < code_len; i++) {
        if (code[i] == '\n') {
            num_lines++;
        }
    }
    char lines[num_lines][250];
    int lineIndex = 0;
    int charIndex = 0;
    // Split code into lines
    for (size_t i = 0; i < code_len; i++) {
        if (code[i] == '\n') {
            lines[lineIndex][charIndex] = '\0';
            lineIndex++;
            charIndex = 0;
        } else {
            lines[lineIndex][charIndex] = code[i];
            charIndex++;
        }
    }
    struct pile p;
    init_pile(&p);
    printf("%c", lines[3]);
}