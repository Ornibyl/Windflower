#include "Tokenizer.h"

#include <ctype.h>

void wf_tokenizer_init(wf_Tokenizer* tokenizer, const char* file_path, const char* file_text)
{
    tokenizer->file_text = wf_str_slice_from_cstr(file_text);

    tokenizer->file_position.file_path = file_path;
    tokenizer->file_position.column = 1;
    tokenizer->file_position.line = 1;

    tokenizer->index = 0;
    tokenizer->current_char = tokenizer->file_text.text[tokenizer->index];
}

static void wf_tokenizer_advance(wf_Tokenizer* tokenizer)
{
    tokenizer->index++;
    tokenizer->file_position.column++;
    if(tokenizer->current_char == '\n')
    {
        tokenizer->file_position.line++;
        tokenizer->file_position.column = 1;
    }
    tokenizer->current_char = tokenizer->file_text.text[tokenizer->index];
}

static wf_Token wf_tokenizer_make_token(wf_Tokenizer* tokenizer, wf_TokenType type, 
                                        wf_FilePosition position, size_t start_index)
{
    wf_Token token;
    token.type = type;
    token.value.text = tokenizer->file_text.text + start_index;
    token.value.length = tokenizer->index - start_index;
    token.position = position;
    return token;
}

static wf_Token wf_tokenizer_make_error(wf_Tokenizer* tokenizer, wf_FilePosition position, 
                                            const char* message)
{
    wf_Token token;
    token.type = WF_TOKEN_ERROR;
    token.position = position;
    token.value = wf_str_slice_from_cstr(message);
    return token;
}

static void wf_tokenizer_skip_whitespace(wf_Tokenizer* tokenizer)
{
    while(tokenizer->index < tokenizer->file_text.length)
    {
        switch(tokenizer->current_char)
        {
            case ' ':
            case '\t':
            case '\v':
            case '\f':
            case '\r':
                wf_tokenizer_advance(tokenizer);
                break;
            default:
                return;
        }
    }
}

wf_Token wf_tokenizer_next(wf_Tokenizer* tokenizer)
{
    if(tokenizer->index == tokenizer->file_text.length) 
    {
        return wf_tokenizer_make_token(tokenizer, WF_TOKEN_EOF, tokenizer->file_position, 
                                        tokenizer->index);
    }

    wf_tokenizer_skip_whitespace(tokenizer);

    wf_FilePosition start_position = tokenizer->file_position;
    size_t start_index = tokenizer->index;

    if(isdigit(tokenizer->current_char))
    {
        bool has_dot = false;
        while(isdigit(tokenizer->current_char) || tokenizer->current_char == '_'
                || tokenizer->current_char == '.')
        {
            if(tokenizer->current_char == '.')
            {
                if(has_dot) break;
                
                has_dot = true;
            }
            wf_tokenizer_advance(tokenizer);
        }

        // TODO: Add hex, octal, and binary literals.
        if(has_dot)
        {
            return wf_tokenizer_make_token(tokenizer, WF_TOKEN_FLOAT, start_position, start_index);
        }
        return wf_tokenizer_make_token(tokenizer, WF_TOKEN_INT, start_position, start_index);
    }

    switch(tokenizer->current_char)
    {
        case '\n':
            wf_tokenizer_advance(tokenizer);
            return wf_tokenizer_make_token(tokenizer, WF_TOKEN_NEWLINE, start_position, start_index);
        case '+':
            wf_tokenizer_advance(tokenizer);
            return wf_tokenizer_make_token(tokenizer, WF_TOKEN_PLUS, start_position, start_index);
        case '-':
            wf_tokenizer_advance(tokenizer);
            return wf_tokenizer_make_token(tokenizer, WF_TOKEN_MINUS, start_position, start_index);
        case '*':
            wf_tokenizer_advance(tokenizer);
            return wf_tokenizer_make_token(tokenizer, WF_TOKEN_STAR, start_position, start_index);
        case '/':
            wf_tokenizer_advance(tokenizer);
            return wf_tokenizer_make_token(tokenizer, WF_TOKEN_SLASH, start_position, start_index);
        case '(':
            wf_tokenizer_advance(tokenizer);
            return wf_tokenizer_make_token(tokenizer, WF_TOKEN_OPEN_PAREN, start_position, start_index);
        case ')':
            wf_tokenizer_advance(tokenizer);
            return wf_tokenizer_make_token(tokenizer, WF_TOKEN_CLOSE_PAREN, start_position, start_index);
        default:
            break;
    }
    wf_tokenizer_advance(tokenizer);
    return wf_tokenizer_make_error(tokenizer, start_position, "Invalid character in script.");
}
