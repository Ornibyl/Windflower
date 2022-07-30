#pragma once

#include "Token.h"

typedef struct wf_Tokenizer
{
    wf_StringSlice file_text;
    wf_FilePosition file_position;
    size_t index;
    char current_char;
} wf_Tokenizer;

void wf_tokenizer_init(wf_Tokenizer* tokenizer, const char* file_path, const char* file_text);
// wf_Tokenizer doesn't have a destroy function as it does not need to free up resources

wf_Token wf_tokenizer_next(wf_Tokenizer* tokenizer);
