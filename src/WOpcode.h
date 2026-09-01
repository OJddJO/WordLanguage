#ifndef __WOPCODE_H__
#define __WOPCODE_H__

#include <stdint.h>

typedef enum _WOpcode : uint8_t {
    WOP_NOP,        // Do nothing
    WOP_WORD,       // Word flag, used as {WOP_WORD, WOP_instruction} {word_payload}

    // Accumulator (ACC)
    WOP_LOAD_CONST, // Load a constant to accumulator   (WOP_LOAD_CONST <const_id>)
    WOP_LOAD_LOCAL, // Load a local to accumulator      (WOP_LOAD_LOCAL <local_id>)
    WOP_STOR_LOCAL, // Store the accumulator to a local (WOP_STOR_LOCAL <local_id>)
    WOP_LOAD_GLOBL, // Load a global to accumulator     (WOP_LOAD_GLOBL <global_id>)
    WOP_STOR_GLOBL, // Store accumulator to a global    (WOP_STOR_GLOBL <global_id>)
    WOP_STOR_ZERO,  // Zero the accumulator
    WOP_STOR_ONE,   // Store one in the accumulator
    WOP_STOR_NULL,  // Store NULL in the accumulator

    // Type conversion
    WOP_I2F,        // Convert accumulator from int to float
    WOP_F2I,        // Convert accumulator from float to int

    // Arithmetic
    WOP_IADD,       // Load an int from the constant pool and do `ACC += c`
    WOP_ISUB,       // Load an int from the constant pool and do `ACC -= c`
    WOP_IMUL,       // Load an int from the constant pool and do `ACC *= c`
    WOP_IDIV,       // Load an int from the constant pool and do `ACC /= c`
    WOP_IMOD,       // Load an int from the constant pool and do `ACC %= c`

    WOP_FADD,       // Load a float from the constant pool and do `ACC += c`
    WOP_FSUB,       // Load a float from the constant pool and do `ACC -= c`
    WOP_FMUL,       // Load a float from the constant pool and do `ACC *= c`
    WOP_FDIV,       // Load a float from the constant pool and do `ACC /= c`
    WOP_FMOD,       // Load a float from the constant pool and do `ACC %= c`

    // Binary
    WOP_BNOT,       // Do a bitwise NOT on the accumulator
    WOP_BAND,       // Load a value from the constant pool and do `ACC &= c`
    WOP_BOR,        // Load a value from the constant pool and do `ACC |= c`
    WOP_BXOR,       // Load a value from the constant pool and do `ACC ^= c`

    // Logic
    WOP_NOT,        // Do logical NOT on the accumulator
    WOP_IEQ,        // Load an integer from the constant pool and do `ACC == c`
    WOP_ILT,        // Load an integer from the constant pool and do `ACC < c`
    WOP_ILE,        // Load an integer from the constant pool and do `ACC <= c`
    WOP_IGT,        // Load an integer from the constant pool and do `ACC > c`
    WOP_IGE,        // Load an integer from the constant pool and do `ACC >= c`
    WOP_INEQ,       // Load an integer from the constant pool and do `ACC != c`

    WOP_FEQ,        // Load a float from the constant pool and do `ACC == c`
    WOP_FLT,        // Load a float from the constant pool and do `ACC < c`
    WOP_FLE,        // Load a float from the constant pool and do `ACC <= c`
    WOP_FGT,        // Load a float from the constant pool and do `ACC > c`
    WOP_FGE,        // Load a float from the constant pool and do `ACC >= c`
    WOP_FNEQ,       // Load a float from the constant pool and do `ACC != c`

    // Control flow
    WOP_JMP,        // Jump
    WOP_JZ,         // Jump if zero
    WOP_JNZ,        // Jump if not zero

    // Function
    WOP_CALL_DIRECT,// Call the function passed in the ACC with the specified number of arguments (WOP_CALL <argc>)
    WOP_CALL_C,     // Call a C function
    WOP_RET,        // Return from a function

    WOP_LIMIT = (1 << 6) - 1, // NORMAL OPCODE LIMIT

    // Local pool opcode variants
    WOP_LOCAL_IADD = (1 << 6), // Load an int from the local pool and do ACC += l
    WOP_LOCAL_ISUB, // Load an int from the local pool and do ACC -= l
    WOP_LOCAL_IMUL, // Load an int from the local pool and do ACC *= l
    WOP_LOCAL_IDIV, // Load an int from the local pool and do ACC /= l
    WOP_LOCAL_IMOD, // Load an int from the local pool and do ACC %= l

    WOP_LOCAL_FADD, // Load a float from the local pool and do ACC += l
    WOP_LOCAL_FSUB, // Load a float from the local pool and do ACC -= l
    WOP_LOCAL_FMUL, // Load a float from the local pool and do ACC *= l
    WOP_LOCAL_FDIV, // Load a float from the local pool and do ACC /= l
    WOP_LOCAL_FMOD, // Load a float from the local pool and do ACC %= l

    WOP_LOCAL_BAND, // Load a value from the local pool and do ACC &= l
    WOP_LOCAL_BOR,  // Load a value from the local pool and do ACC |= l
    WOP_LOCAL_BXOR, // Load a value from the local pool and do ACC ^= l

    WOP_LOCAL_IEQ,  // Load an int from the local pool and do ACC == l
    WOP_LOCAL_ILT,  // Load an int from the local pool and do ACC < l
    WOP_LOCAL_ILE,  // Load an int from the local pool and do ACC <= l
    WOP_LOCAL_IGT,  // Load an int from the local pool and do ACC > l
    WOP_LOCAL_IGE,  // Load an int from the local pool and do ACC >= l
    WOP_LOCAL_INEQ, // Load an int from the local pool and do ACC != l

    WOP_LOCAL_FEQ,  // Load a float from the local pool and do ACC == l
    WOP_LOCAL_FLT,  // Load a float from the local pool and do ACC < l
    WOP_LOCAL_FLE,  // Load a float from the local pool and do ACC <= l
    WOP_LOCAL_FGT,  // Load a float from the local pool and do ACC > l
    WOP_LOCAL_FGE,  // Load a float from the local pool and do ACC >= l
    WOP_LOCAL_FNEQ, // Load a float from the local pool and do ACC != l

    WOP_LOCAL_AND,  // Load a value from the local pool and do ACC && l
    WOP_LOCAL_OR,   // Load a value from the local pool and do ACC || l

    WOP_LOCAL_LIMIT = (1 << 7) - 1, // LOCAL OPCODE LIMIT

    // Stack opcode
    WOP_PUSH = (1 << 7), // Push the value in the accumulator to the stack
    WOP_POP,        // Pop the stack top to the accumulator

    WOP_STACK_IADD, // Pop the stack top and do integer operand ACC += s
    WOP_STACK_ISUB, // Pop the stack top and do integer operand ACC -= s
    WOP_STACK_IMUL, // Pop the stack top and do integer operand ACC *= s
    WOP_STACK_IDIV, // Pop the stack top and do integer operand ACC /= s
    WOP_STACK_IMOD, // Pop the stack top and do integer operand ACC %= s

    WOP_STACK_FADD, // Pop the stack top and do floating operand ACC += s
    WOP_STACK_FSUB, // Pop the stack top and do floating operand ACC -= s
    WOP_STACK_FMUL, // Pop the stack top and do floating operand ACC *= s
    WOP_STACK_FDIV, // Pop the stack top and do floating operand ACC /= s
    WOP_STACK_FMOD, // Pop the stack top and do floating operand ACC %= s

    WOP_STACK_BAND, // Pop the stack top and do ACC &= s
    WOP_STACK_BOR,  // Pop the stack top and do ACC |= s
    WOP_STACK_BXOR, // Pop the stack top and do ACC ^= s

    WOP_STACK_EQ,   // Pop the stack top and do ACC == s
    WOP_STACK_LT,   // Pop the stack top and do ACC < s
    WOP_STACK_LE,   // Pop the stack top and do ACC <= s
    WOP_STACK_GT,   // Pop the stack top and do ACC > s
    WOP_STACK_GE,   // Pop the stack top and do ACC >= s
    WOP_STACK_NEQ,  // Pop the stack top and do ACC != s

    WOP_STACK_AND,  // Pop the stack top and do ACC && s
    WOP_STACK_OR,   // Pop the stack top and do ACC || s

    WOP_STACK_LIMIT = (1 << 8) - 1,
} WOpcode;

#endif
