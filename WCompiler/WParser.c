#include "WParser.h"

int parserInit(const char *src, WParser *parser) {
    WParser ret = {0};

    if (!lexerInit(src, &ret)) {
        return 0;
    }
}
