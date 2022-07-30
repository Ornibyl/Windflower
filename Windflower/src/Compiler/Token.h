#pragma once

#include "Utils/StringUtils.h"
#include "FilePosition.h"

typedef enum wf_TokenType
{
    WF_TOKEN_INT, WF_TOKEN_FLOAT,
    WF_TOKEN_PLUS, WF_TOKEN_MINUS, WF_TOKEN_STAR, WF_TOKEN_SLASH,
    WF_TOKEN_OPEN_PAREN, WF_TOKEN_CLOSE_PAREN,
    WF_TOKEN_ERROR, WF_TOKEN_NEWLINE, WF_TOKEN_EOF,
} wf_TokenType;

typedef struct wf_Token
{
    wf_TokenType type;
    wf_StringSlice value;
    wf_FilePosition position;
} wf_Token;
