#include "WLexer.h"
#include "WParser.h"

#include <stdio.h>
#include <werror.h>

int main(int argc, char *argv[]) {
    printf("argc: %d\n", argc);
    if (argc != 2) return 1;
    const char *file = argv[1];

    WLexer lex;
    if (!lexerInit(file, &lex)) {
        PRINT_ERR("failed to init lexer\n");
        return 0;
    }
    printf("%s", lex.buf);

    const WToken *tok;
    while (lexerHasToken(&lex)) {
        tok = lexerPeekToken(&lex);
        switch (tok->type) {
            case (WTOK_IDENTIFIER): {
                printf("{ID, %s} | ", tok->as.id);
                break;
            }
            case (WTOK_KEYWORD): {
                printf("{KEY, %s} | ", keywords[tok->as.kw]);
                break;
            }
            case (WTOK_PUNCTUATOR): {
                printf("{PUNC, '%c'} | ", punctuator[tok->as.punc]);
                break;
            }
            case (WTOK_OPERATOR): {
                printf("{OP, %s} | ", operators[tok->as.op]);
                break;
            }
            case (WTOK_LITERAL): {
                switch (tok->as.lit.type) {
                    case (LIT_FLOAT): {
                        printf("{LIT, %lf} | ", tok->as.lit.f);
                        break;
                    }
                    case (LIT_INT): {
                        printf("{LIT, %zd} | ", tok->as.lit.i);
                        break;
                    }
                    case (LIT_STR): {
                        printf("{LIT, \"%s\"} | ", tok->as.lit.s);
                        break;
                    }
                    default:
                        return 1;
                        break;
                }
                break;
            }
        }
        lexerForward(&lex);
    }

    putchar('\n');
    WParser parser;
    if (!parserInit(file, &parser)) {
        PRINT_ERR("failed to init parser\n");
        return 1;
    }
    if (IS_ERR(parserBuildAST(&parser))) {
        PRINT_ERR("failed to parse program\n");
        return 1;
    }
    parserPrintProgram(&parser);

    return 0;
}
