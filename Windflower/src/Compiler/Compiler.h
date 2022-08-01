#pragma once

#include "Utils/StringUtils.h"
#include "Tokenizer.h"
#include "Parser.h"

typedef struct wf_Compiler
{
    wf_Environment* env;
    wf_Parser parser;
} wf_Compiler;

void wf_compiler_init(wf_Environment* env, wf_Compiler* compiler, const wf_CompileInfo* compile_info);
void wf_compiler_destroy(wf_Compiler* compiler);

wf_StringObj* wf_compiler_compile(wf_Compiler* compiler, wf_BytecodeObj* obj);
