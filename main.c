#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "piles.h"

int main(char file_name) {
    list lines;
    init_list(&lines);
    FILE *file = fopen(file_name, "r");
    if (file == NULL) {
        printf("Error opening file\n");
        return 1;
    }
    char line[100];
    while (fgets(line, 100, file) != NULL) {
        add_element(&lines, atoi(line));
    }
    fclose(file);
}