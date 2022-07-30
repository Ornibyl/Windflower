#pragma once

#include "Windflower/Windflower.h"
#include "VM/VM.h"
#include "Compiler/Compiler.h"

struct wf_Environment
{
    wf_AllocatorFunc allocator_func; 
    void* allocator_user_data;
    // The compiler is recreated and destroyed every time wf_compile is called.
    // However, the garbage collector needs to be able to reach the strings the compiler uses
    // so that it doesn't free them accidentally.
    // Because of that, the compiler is in the environment instead of being created locally.
    wf_Compiler compiler;
    wf_VM vm;
};
