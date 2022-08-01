#include "Debug.h"

#include <inttypes.h>

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

void wf_debug_tokenizer(FILE* file, wf_Tokenizer* tokenizer)
{
    fprintf(file, "File: '%s'\n", tokenizer->file_position.file_path);
    uint32_t last_line = 0;
    while(true)
    {
        wf_Token next = wf_tokenizer_next(tokenizer);
        if(next.position.line != last_line)
        {
            last_line = next.position.line;
            fprintf(file, "%4"PRIu32" ", last_line);
        }
        else
        {
            fprintf(file, "   | ");
        }
        fprintf(file, "%-15s '%.*s'\n", wf_token_type_to_string(next.type), (int)next.value.length, 
                next.value.text);

        if(next.type == WF_TOKEN_EOF) break;
    }
}

static void wf_debug_print_node(FILE* file, wf_Node* node)
{
    switch(node->type)
    {
        case WF_NODE_LITERAL:
        {
            wf_LiteralNode* as_literal = (wf_LiteralNode*)node;
            fprintf(file, "%.*s", (int)as_literal->value.length, as_literal->value.text);
            break;
        }
        case WF_NODE_BINARY_OP:
        {
            wf_BinaryOpNode* as_binary_op = (wf_BinaryOpNode*)node;

            fprintf(file, "(");
            wf_debug_print_node(file, (wf_Node*)as_binary_op->left_operand);

            switch(as_binary_op->operation)
            {
                case WF_BINARY_OP_ADD:
                    fprintf(file, " + ");
                    break;
                case WF_BINARY_OP_SUB:
                    fprintf(file, " - ");
                    break;
                case WF_BINARY_OP_MUL:
                    fprintf(file, " * ");
                    break;
                case WF_BINARY_OP_DIV:
                    fprintf(file, " / ");
                    break;
            }

            wf_debug_print_node(file, (wf_Node*)as_binary_op->right_operand);

            fprintf(file, ")");

            break;
        }
        case WF_NODE_UNARY_OP:
        {
            wf_UnaryOpNode* as_unary_op = (wf_UnaryOpNode*)node;

            fprintf(file, "(");

            switch(as_unary_op->operation)
            {
                case WF_UNARY_OP_NEGATE:
                    fprintf(file, " - ");
                    break;
            }

            wf_debug_print_node(file, (wf_Node*)as_unary_op->operand);

            fprintf(file, ")");

            break;
        }
    }
}

void wf_debug_ast(FILE* file, wf_Node* node)
{
    fprintf(file, "Ast dump:\n");
    wf_debug_print_node(file, node);
    fprintf(file, "\n");
}
