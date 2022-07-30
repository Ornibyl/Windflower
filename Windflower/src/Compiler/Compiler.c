#include "Compiler.h"

#include <inttypes.h>
#include <stdio.h>

void wf_compiler_init(wf_Compiler* compiler, const wf_CompileInfo* compile_info)
{
    wf_tokenizer_init(&compiler->tokenizer, compile_info->main_file_path, 
                        compile_info->main_file_text);
}

void wf_compiler_destroy(wf_Compiler* compiler)
{
}

// Later on I'll make a debug module that this will be moved into.
// For now I'm putting this here until the parser is created.
static const char* wf_token_type_to_string(wf_TokenType type)
{
    switch(type)
    {
        case WF_TOKEN_INT: return "INT";
        case WF_TOKEN_FLOAT: return "FLOAT";
        
        case WF_TOKEN_PLUS: return "PLUS";
        case WF_TOKEN_MINUS: return "MINUS";
        case WF_TOKEN_STAR: return "STAR";
        case WF_TOKEN_SLASH: return "SLASH";
        
        case WF_TOKEN_OPEN_PAREN: return "OPEN_PAREN";
        case WF_TOKEN_CLOSE_PAREN: return "CLOSE_PAREN";

        case WF_TOKEN_ERROR: return "ERROR";
        case WF_TOKEN_NEWLINE: return "NEWLINE";
        case WF_TOKEN_EOF: return "EOF";
    }
    // Unreachable
    return "UNKNOWN";
}

void wf_compiler_compile(wf_Compiler* compiler, wf_BytecodeObj* obj)
{
    printf("File: '%s'\n", compiler->tokenizer.file_position.file_path);
    uint32_t last_line = 0;
    while(true)
    {
        wf_Token next = wf_tokenizer_next(&compiler->tokenizer);
        if(next.position.line != last_line)
        {
            last_line = next.position.line;
            printf("%4"PRIu32" ", last_line);
        }
        else
        {
            printf("   | ");
        }
        printf("%-15s '%.*s'\n", wf_token_type_to_string(next.type), (int)next.value.length, 
                next.value.text);

        if(next.type == WF_TOKEN_EOF) break;
    }
}
