#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "piles.h"

int main(void) {
    char code[] = "";
    struct list lines;
    init_list(&lines);
    size_t codeLen = strlen(code);
    for (size_t i = 0; i < codeLen; i++) {
        if (code[i] == '\n') {
            
            add_element(&lines, i);
        }
    }
    struct pile p;
    init_pile(&p);
    
}