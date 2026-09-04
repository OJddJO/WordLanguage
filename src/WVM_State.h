#ifndef __WVM_STATE_H__
#define __WVM_STATE_H__

#include "WVM_Types.h"

typedef struct _WVMState {
    const uint16_t  *bytecode;      // Raw bytecode
    const uint16_t  *entrypoint;    // Main function

    WVM_Value          *consts;
    WVM_Value          *globals;
} WVMState;

#endif
