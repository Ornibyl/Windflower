#pragma once

#include "Tokenizer.h"
#include "Nodes.h"
#include "Utils/Arrays.h"

typedef struct wf_Parser
{
    wf_Environment* env;
    wf_Tokenizer tokenizer;
    
    wf_BoolArray ignore_line_stack;
    wf_StringObj* error_messages;
    uint16_t error_count;

    const wf_CompileInfo* info;

    wf_Token previous;
    wf_Token current;
} wf_Parser;

void wf_parser_init(wf_Environment* env, wf_Parser* parser, const wf_CompileInfo* info);
void wf_parser_shutdown(wf_Parser* parser);

wf_Node* wf_parser_parse(wf_Parser* parser);
