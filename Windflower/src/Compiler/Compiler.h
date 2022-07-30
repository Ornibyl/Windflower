#pragma once

#include "Utils/StringUtils.h"
#include "Tokenizer.h"

typedef struct wf_Compiler
{
    wf_Tokenizer tokenizer;
    // When the garbage collector is implemented, this will make sure it does not collect strings
    // used by the compiler.
    wf_StringArray compiler_strings;
} wf_Compiler;

void wf_compiler_init(wf_Compiler* compiler, const wf_CompileInfo* compile_info);
void wf_compiler_destroy(wf_Compiler* compiler);

void wf_compiler_compile(wf_Compiler* compiler, wf_BytecodeObj* obj);
