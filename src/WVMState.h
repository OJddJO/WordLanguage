#ifndef __WVMSTATE_H__
#define __WVMSTATE_H__

#include "hashmap.h"

typedef struct _WVMState {
    const uint16_t  *bytecode;      // Raw bytecode
    const uint16_t  *entrypoint;    // Main function

    WValue          *consts;
    Hashmap         globals;
} WVMState;

#endif
