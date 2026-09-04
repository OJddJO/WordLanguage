#ifndef __WVM_OPCODE_H__
#define __WVM_OPCODE_H__

#include <stdint.h>

#define OPCODES(X)\
    X(NOP)\
    X(HALT)\
    X(WORD)\
    X(DWORD)\
    X(QWORD)\
    X(LOAD_CONST)\
    X(LOAD_LOCAL)\
    X(STOR_LOCAL)\
    X(LOAD_GLOBL)\
    X(STOR_GLOBL)\
    X(LOAD_IZERO)\
    X(LOAD_IONE)\
    X(LOAD_FZERO)\
    X(LOAD_FONE)\
    X(LOAD_BZERO)\
    X(LOAD_BONE)\
    X(LOAD_NULL)\
    X(I2F)\
    X(F2I)\
    X(IADD)\
    X(ISUB)\
    X(IMUL)\
    X(IDIV)\
    X(IMOD)\
    X(FADD)\
    X(FSUB)\
    X(FMUL)\
    X(FDIV)\
    X(BNOT)\
    X(BAND)\
    X(BOR)\
    X(BXOR)\
    X(LSH)\
    X(RSH)\
    X(NOT)\
    X(IEQ)\
    X(ILT)\
    X(ILE)\
    X(IGT)\
    X(IGE)\
    X(INEQ)\
    X(FEQ)\
    X(FLT)\
    X(FLE)\
    X(FGT)\
    X(FGE)\
    X(FNEQ)\
    X(JMP)\
    X(JZ)\
    X(JNZ)\
    X(CALL)\
    X(RET)\
    X(LOCAL_IADD)\
    X(LOCAL_ISUB)\
    X(LOCAL_IMUL)\
    X(LOCAL_IDIV)\
    X(LOCAL_IMOD)\
    X(LOCAL_FADD)\
    X(LOCAL_FSUB)\
    X(LOCAL_FMUL)\
    X(LOCAL_FDIV)\
    X(LOCAL_BAND)\
    X(LOCAL_BOR)\
    X(LOCAL_BXOR)\
    X(LOCAL_LSH)\
    X(LOCAL_RSH)\
    X(LOCAL_IEQ)\
    X(LOCAL_ILT)\
    X(LOCAL_ILE)\
    X(LOCAL_IGT)\
    X(LOCAL_IGE)\
    X(LOCAL_INEQ)\
    X(LOCAL_FEQ)\
    X(LOCAL_FLT)\
    X(LOCAL_FLE)\
    X(LOCAL_FGT)\
    X(LOCAL_FGE)\
    X(LOCAL_FNEQ)\
    X(LOCAL_AND)\
    X(LOCAL_OR)\
    X(PUSH)\
    X(POP)\
    X(STACK_IADD)\
    X(STACK_ISUB)\
    X(STACK_IMUL)\
    X(STACK_IDIV)\
    X(STACK_IMOD)\
    X(STACK_FADD)\
    X(STACK_FSUB)\
    X(STACK_FMUL)\
    X(STACK_FDIV)\
    X(STACK_BAND)\
    X(STACK_BOR)\
    X(STACK_BXOR)\
    X(STACK_LSH)\
    X(STACK_RSH)\
    X(STACK_IEQ)\
    X(STACK_ILT)\
    X(STACK_ILE)\
    X(STACK_IGT)\
    X(STACK_IGE)\
    X(STACK_INEQ)\
    X(STACK_FEQ)\
    X(STACK_FLT)\
    X(STACK_FLE)\
    X(STACK_FGT)\
    X(STACK_FGE)\
    X(STACK_FNEQ)\
    X(STACK_AND)\
    X(STACK_OR)

typedef enum _WOpcode : uint8_t {
    #define BUILD_ENUM(opcode) OP_##opcode,
        OPCODES(BUILD_ENUM)
    #undef BUILD_ENUM
} WOpcode;

#endif
