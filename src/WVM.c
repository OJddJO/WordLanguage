#include <stdio.h>

#include "WVM.h"
#include "hashmap.h"

static const void *_opTable [] = {

};

static Hashmap globals;

int wvm_init() {
    if (!hashmapInit(&globals)) {
        fprintf(stderr, "Failed to initialize WVM\n");
        return 0;
    }
}

int wvm_run(WVMState *state, WVMContext *context) {
    union {
        uint16_t whole;
        struct {
            uint8_t opcode;
            uint8_t payload;
        };
    } packet;

    #define FETCH() packet.whole = context->ip++

do_NOP: {
    FETCH();
}

    #undef FETCH
}
