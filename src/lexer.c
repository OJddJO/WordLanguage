#include "lexer.h"

char *keywords[] = {
    //variables
    "int", "float", "str", "bool", "assign", "change", "to", "delete",
    //constructed types
    "list", "create", "append", "get", "set", "search", "index", "remove", "length",
    //IO expressions
    "print", "sep", "end", "ask",
    //control flow
    //conditional
    "if", "elif", "else", "endif",
    //loop
    "for", "from", "to", "step", "endfor", "infloop", "endinf", "break", "continue",
    //functions
    "def", "null", "with", "return", "enddef", "call", "store",
};

char *operators[] = {
    //operations
    "plus", "minus", "times", "div", "mod", "ediv", "power", "sqrt",
    //comparison
    "equal", "greater", "less", "gequal", "lequal",
    //logical
    "and", "or", "not",
};

/**
 * \brief Tokenizes the given file into a list of lists of words. (w_malloc)
 * \param source The file to tokenize.
 * \return A list of list of words.
 */
list *word_tokenize(FILE *source) {
    fseek(source, 0, SEEK_END); //get the size of the file
    int size = ftell(source);
    fseek(source, 0, SEEK_SET);

    list *code = list_init();
    int start = 0; //start of the word
    int n_line = 1;
    int eval = 0; //if there is a word to eval
    int eval_str = 0; //if the word to eval is a litteral str

    list *line = list_init();
    list_append(code, line);
    for (int i = 0; i < size; i++) {
        fseek(source, i, SEEK_SET);
        char c = fgetc(source);
        if (c == '#' && !eval_str) {
            while (c != '\n' && c != EOF) {
                c = fgetc(source);
                i++;
            }
        }
        if (c != ' ' && c != '\t' && c != '\n' && !eval) {
            eval = 1;
            start = i;
        }
        if ((c == ' ' || c == '\t' || c == '\n' || c == EOF) && !eval_str && eval) {
            fseek(source, start, SEEK_SET);
            char *value = (char *)w_malloc(i - start + 1);
            fread(value, 1, i - start, source);
            value[i - start] = '\0';
            W_Word *w = (W_Word *)w_malloc(sizeof(W_Word));
            w->value = value;
            w->type = word_type(value);
            w->line = n_line;
            w->parsed = false;
            w->is_generated = false;
            list_append(line, w);
            eval = 0;
        }
        if (c == '\"') {
            if (eval_str) eval_str = 0;
            else eval_str = 1;
        }
        if (c == '\n' && !eval_str) {
            line = list_init();
            list_append(code, line);
            n_line++;
            i++;
        }
    }
    if (eval) {
        fseek(source, start, SEEK_SET);
        char *value = (char *)w_malloc(size - start + 1);
        fread(value, 1, size - start, source);
        value[size - start] = '\0';
        W_Word *w = (W_Word *)w_malloc(sizeof(W_Word));
        w->value = value;
        w->type = word_type(value);
        w->line = n_line;
        w->parsed = false;
        w->is_generated = false;
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
    if (value[0] == '\"' && value[strlen(value) - 1] == '\"') {
        return STR;
    }
    if (((value[0] >= '0' && value[0] <= '9') || value[0] == '-') && (value[strlen(value) - 1] >= '0' && value[strlen(value) - 1] <= '9')) {
        return NUMBER;
    }
    if (is_keyword(value)) return KEYWORD;
    int dot = 0;
    for (int i = 0; i < strlen(value); i++) {
        if (value[i] == '.') {
            dot++;
        } else break;
    }
    char without_dot[strlen(value) - dot + 1];
    strncpy(without_dot, value + dot, strlen(value) - dot);
    without_dot[strlen(value) - dot] = '\0';
    // printf("value: %s, dot: %d, without_dot: %s\n", value, dot, without_dot); //debug
    if (is_operator(without_dot)) return OPERATOR;
    return IDENTIFIER;
}

/**
 * \brief Destroys the given list of words.
 * \param code The list of list of words to destroy.
 * \return void
 */
void lexer_destroy(list *code) {
    list_element *current_line = code->head;
    list *line = (list *)current_line->value;
    for (int i = 0; i < code->size; i++) {
        list_destroy(line);
        if (current_line->next != NULL) {
            current_line = current_line->next;
            line = (list *)current_line->value;
        }
    }
    list_destroy(code);
}

/**
 * \brief Prints the given list of words.
 * \param code The list of list of words to print.
 * \return void
 */
void lexer_print(list *code) { //debug
    int i = 1; //line number
    list_element *current_line = code->head;
    list *line = (list *)current_line->value;
    list_element *current_word = line->head;
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
            line = (list *)current_line->value;
            current_word = line->head;
        }
    }
}

/**
 * \brief Checks if the given word is a keyword.
 * \param word The word to check.
 * \return True if the word is a type keyword, false otherwise.
 */
bool is_keyword(char *word) {
    for (int i = 0; i < sizeof(keywords) / sizeof(keywords[0]); i++) {
        if (strcmp(word, keywords[i]) == 0) return true;
    }
    return false;
}

/**
 * \brief Checks if the given word is an operator.
 * \param word The word to check.
 * \return True if the word is an operator, false otherwise.
 */
bool is_operator(char *word) {
    for (int i = 0; i < sizeof(operators) / sizeof(operators[0]); i++) {
        if (strcmp(word, operators[i]) == 0) return true;
    }
    return false;
}