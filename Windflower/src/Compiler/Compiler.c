#include "Compiler.h"

#include "Debug.h"

void wf_compiler_init(wf_Environment* env, wf_Compiler* compiler, const wf_CompileInfo* compile_info)
{
    compiler->env = env;
    wf_parser_init(env, &compiler->parser, compile_info);
}

void wf_compiler_destroy(wf_Compiler* compiler)
{
    wf_parser_shutdown(&compiler->parser);
}

wf_StringObj* wf_compiler_compile(wf_Compiler* compiler, wf_BytecodeObj* obj)
{
    wf_Node* ast = wf_parser_parse(&compiler->parser);
    if(compiler->parser.error_messages != NULL) return compiler->parser.error_messages;
    wf_debug_ast(stdout, ast);
    return NULL;
}
