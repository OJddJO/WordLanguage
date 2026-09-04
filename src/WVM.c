#include <stdio.h>

#include "bytes.h"
#include "WVM.h"
#include "WVM_Opcode.h"
#include "WVM_Error.h"

int WVM_init(WVMState *state, WVMContext *mainContext) {
    return 1;
}

int WVM_run(WVMState *state, WVMContext *context) {
    static const void *dispatchTable [] = {
        #define BUILD_DISPATCH_TABLE(opcode) [OP_##opcode] = &&do_##opcode,
            OPCODES(BUILD_DISPATCH_TABLE)
        #undef BUILD_DISPATCH_TABLE
    };

    register union {
        uint16_t whole;
        struct {
            uint8_t byte1;
            uint8_t byte2;
        };
    } packet;
    register uint8_t opcode = 0;
    register union {
        int64_t     i;
        double      f;
    } arg;

    #define FETCH() do {\
        packet.whole = fetch_i16_le((const uint8_t **)&context->ip);\
        opcode = packet.byte1;\
        arg.i = packet.byte2;\
    } while (0)
    #define DISPATCH() goto *dispatchTable[(opcode)];
    #define FETCH_DISPATCH() FETCH(); DISPATCH()

    FETCH_DISPATCH();

do_NOP: {
    FETCH_DISPATCH();
}

do_HALT: {
    goto end;
}

do_WORD: {
    opcode = arg.i;
    arg.i = fetch_i16_le((const uint8_t **)&context->ip);
    DISPATCH();
}

do_DWORD: {
    opcode = arg.i;
    arg.i = fetch_i32_le((const uint8_t **)&context->ip);
    DISPATCH();
}

do_QWORD: {
    opcode = arg.i;
    arg.i = fetch_i64_le((const uint8_t **)&context->ip);
    DISPATCH();
}

do_LOAD_CONST: {
    context->acc = state->consts[arg.i];
    FETCH_DISPATCH();
}

do_LOAD_LOCAL: {
    context->acc = context->locals[arg.i];
    FETCH_DISPATCH();
}

do_STOR_LOCAL: {
    context->locals[arg.i] = context->acc;
    FETCH_DISPATCH();
}

do_LOAD_GLOBL: {
    context->acc = state->globals[arg.i];
    FETCH_DISPATCH();
}

do_STOR_GLOBL: {
    state->globals[arg.i] = context->acc;
    FETCH_DISPATCH();
}

#define BUILD_LOAD_ZERO_ONE(t, typeid, value) do {\
        context->acc = (WVM_Value){\
            .type = t,\
            .as.typeid = value,\
        };\
        FETCH_DISPATCH();\
    } while (0)

do_LOAD_IZERO: BUILD_LOAD_ZERO_ONE(WVM_TYPE_INT, i, 0);
do_LOAD_IONE: BUILD_LOAD_ZERO_ONE(WVM_TYPE_INT, i, 1);
do_LOAD_FZERO: BUILD_LOAD_ZERO_ONE(WVM_TYPE_FLOAT, f, 0);
do_LOAD_FONE: BUILD_LOAD_ZERO_ONE(WVM_TYPE_FLOAT, f, 1);
do_LOAD_BZERO: BUILD_LOAD_ZERO_ONE(WVM_TYPE_BOOL, b, 0);
do_LOAD_BONE: BUILD_LOAD_ZERO_ONE(WVM_TYPE_BOOL, b, 1);
do_LOAD_NULL: {
    context->acc.as.ptr = NULL;
    FETCH_DISPATCH();
}

#undef BUILD_LOAD_ZERO_ONE

do_I2F: {
    context->acc = (WVM_Value){
        .type = WVM_TYPE_FLOAT,
        .as.f = context->acc.as.i,
    };
    FETCH_DISPATCH();
}

do_F2I: {
    context->acc = (WVM_Value){
        .type = WVM_TYPE_INT,
        .as.i = context->acc.as.f,
    };
    FETCH_DISPATCH();
}

#define BUILD_BINOP(label, typeid, op)\
    do_##label: {\
        context->acc.as.typeid = context->acc.as.typeid op arg.typeid;\
        FETCH_DISPATCH();\
    }

#define BUILD_BINOP_STACK(label, typeid, op)\
    do_STACK_##label: {\
        WVM_Value sval;\
        if (!WStackPop(&context->stack, &sval)) {\
            PRINT_ERR("error");\
            goto end;\
        }\
        context->acc.as.typeid = context->acc.as.typeid op sval.as.typeid;\
        FETCH_DISPATCH();\
    }

#define BUILD_BINOP_LOCAL(label, typeid, op)\
    do_LOCAL_##label: {\
        context->acc.as.typeid = context->acc.as.typeid op context->locals[arg.i].as.typeid;\
        FETCH_DISPATCH();\
    }

#define BUILD_BINOP_FAMILY(label, typeid, op)\
    BUILD_BINOP(label, typeid, op)\
    BUILD_BINOP_STACK(label, typeid, op)\
    BUILD_BINOP_LOCAL(label, typeid, op)

BUILD_BINOP_FAMILY(IADD, i, +)
BUILD_BINOP_FAMILY(ISUB, i, -)
BUILD_BINOP_FAMILY(IMUL, i, *)
BUILD_BINOP_FAMILY(IDIV, i, /)
BUILD_BINOP_FAMILY(IMOD, i, %)
BUILD_BINOP_FAMILY(FADD, f, +)
BUILD_BINOP_FAMILY(FSUB, f, -)
BUILD_BINOP_FAMILY(FMUL, f, *)
BUILD_BINOP_FAMILY(FDIV, f, /)

do_BNOT: {
    context->acc.as.i = ~context->acc.as.i;
    FETCH_DISPATCH();
}

BUILD_BINOP_FAMILY(BAND, i, &)
BUILD_BINOP_FAMILY(BOR, i, |)
BUILD_BINOP_FAMILY(BXOR, i, ^)
BUILD_BINOP_FAMILY(LSH, i, <<)
BUILD_BINOP_FAMILY(RSH, i, >>)

BUILD_BINOP_FAMILY(IEQ, i, ==)
BUILD_BINOP_FAMILY(ILT, i, <)
BUILD_BINOP_FAMILY(ILE, i, <=)
BUILD_BINOP_FAMILY(IGT, i, >)
BUILD_BINOP_FAMILY(IGE, i, >=)
BUILD_BINOP_FAMILY(INEQ, i, !=)
BUILD_BINOP_FAMILY(FEQ, f, ==)
BUILD_BINOP_FAMILY(FLT, f, <)
BUILD_BINOP_FAMILY(FLE, f, <=)
BUILD_BINOP_FAMILY(FGT, f, >)
BUILD_BINOP_FAMILY(FGE, f, >=)
BUILD_BINOP_FAMILY(FNEQ, f, !=)

do_NOT: {
    context->acc.as.b = !context->acc.as.i;
    context->acc.type = WVM_TYPE_BOOL;
    FETCH_DISPATCH();
}

BUILD_BINOP_STACK(AND, i, &&)
BUILD_BINOP_LOCAL(AND, i, &&)
BUILD_BINOP_STACK(OR, i, ||)
BUILD_BINOP_LOCAL(OR, i, ||)

#undef BUILD_BINOP_FAMILY
#undef BUILD_BINOP_LOCAL
#undef BUILD_BINOP_STACK
#undef BUILD_BINOP

do_JMP: {
    context->ip += arg.i;
    FETCH_DISPATCH();
}

do_JZ: {
    if (context->acc.as.i == 0) context->ip += arg.i;
    FETCH_DISPATCH();
}

do_JNZ: {
    if (context->acc.as.i != 0) context->ip += arg.i;
    FETCH_DISPATCH();
}

do_CALL: { // FOR NOW
    FETCH_DISPATCH();
}

do_RET: {
    FETCH_DISPATCH();
}

do_PUSH: {
    WStackPush(&context->stack, &context->acc);
    FETCH_DISPATCH();
}

do_POP: {
    if (!WStackPop(&context->stack, &context->acc)) {
        PRINT_ERR("error");
        goto end;
    }
    FETCH_DISPATCH();
}

#undef FETCH
#undef DISPATCH

end:
    return 0;
}
