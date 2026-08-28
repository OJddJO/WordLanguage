#ifndef __WOPCODE_H__
#define __WOPCODE_H__

#include <stdint.h>

typedef enum _WOpcode : uint8_t {
    WOP_NOP, // Do nothing
    WOP_LDC, // Load a constant value to stack (LDC <value>)
    WOP_LOD, // Load a WObject to stack (LOD <object>)
    WOP_STR, // Store a WObject to stack (STR <object>)
    WOP_POP, // Pop the top stack item (POP)
    WOP_JMP, // Jump offset (JMP <offset>)
    WOP_JIF, // Jump if false offset (JIF <offset>)
    WOP_JIT, // Jump if true offset (JIT <offset>)
    WOP_CALL,// Call a function (CALL <argcount>)
    WOP_RET, // Return from a function (RET)
    WOP_ADD, // Add the two top numbers in the stack (ADD)
    WOP_SUB, // Substract two numbers (SUB)
    WOP_MUL, // Multiply two numbers (MUL)
    WOP_DIV, // Divide two numbers (DIV)
    WOP_MOD, // Modulo (MOD)
    WOP_NEG, // Negative of a number (NEG)
    WOP_EQ,  // Equal (EQ)
    WOP_NEQ, // Not equal (NEQ)
    WOP_LT,  // Lesser than (LT)
    WOP_LTE, // Lesser than or equal (LTE)
    WOP_GT,  // Greater than (GT)
    WOP_GTE, // Greater than or equal (GTE)
    WOP_NOT, // Logical not (NOT)
    WOP_GETFIELD,   // Access a field of an object (GETFIELD <fieldname>)
    WOP_SETFIELD,   // Set a field of an object (SETFIELD <fieldname>)
    WOP_NEWARRAY,   // Create a new array (NEWARRAY <size>)
    WOP_NEWMAP,     // Create a new map (NEWMAP <size>)
    WOP_GETINDEX,   // Get an element from an array/map (GETINDEX <idx>)
    WOP_PRINT,
    WOP_HALT
} WOpcode;

#endif
