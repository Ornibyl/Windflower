#include "Parser.h"

#include <stdarg.h>
#include <inttypes.h>

#include "Environment.h"

typedef enum wf_ExprPrecedence
{
    WF_PREC_NONE,
    WF_PREC_ADDITIVE,
    WF_PREC_MULTIPLICATIVE,
    WF_PREC_SIGN,
    WF_PREC_PRIMARY,
} wf_ExprPrecedence;

typedef wf_ExprNode*(*wf_PrefixExprFunc)(wf_Parser* parser);
typedef wf_ExprNode*(*wf_InfixExprFunc)(wf_Parser* parser, wf_ExprNode* prev);

typedef struct wf_ExprRule
{
    wf_PrefixExprFunc prefix;
    wf_InfixExprFunc infix;
    wf_ExprPrecedence precedence;
} wf_ExprRule;

static wf_ExprRule rules[];

WF_FORMAT_ATTRIBUTE(3, 4)
static void wf_push_error(wf_Parser* parser, wf_FilePosition position, const char* format, ...)
{
    if(parser->error_count == parser->info->max_error_count)
    {
        if(parser->error_messages == NULL)
        {
            parser->error_messages = wf_alloc_string_format(&parser->env->vm, 
                "Too many errors. Some may not show up.");
        }
        else
        {
            parser->error_messages = wf_alloc_string_concat_format(&parser->env->vm, 
                parser->error_messages, "Too many errors. Some may not show up.");
        }
        parser->error_count++;
    }
    else if(parser->error_count > parser->info->max_error_count)
    {
        return;
    }

    if(parser->error_messages == NULL)
    {
        if(parser->info->use_error_colors)
        {
            parser->error_messages = wf_alloc_string_format(&parser->env->vm, 
                "%s(ln: %u, col: %u) " WF_ERROR_COLOR "error: "WF_DEFAULT_COLOR, 
                position.file_path, position.line, position.column);
        }
        else
        {
            parser->error_messages = wf_alloc_string_format(&parser->env->vm, 
                "%s(ln: %u, col: %u) error: ", position.file_path, position.line, position.column);
        }
    }
    else
    {
        if(parser->info->use_error_colors)
        {
            parser->error_messages = wf_alloc_string_concat_format(&parser->env->vm, 
                parser->error_messages, 
                "\n%s(ln: %u, col: %u) " WF_ERROR_COLOR "error: "WF_DEFAULT_COLOR, 
                position.file_path, position.line, position.column);
        }
        else
        {
            parser->error_messages = wf_alloc_string_concat_format(&parser->env->vm, 
                parser->error_messages,"\n%s(ln: %u, col: %u) error: ", 
                position.file_path, position.line, position.column);
        }
    }
    va_list args;
    va_start(args, format);

    parser->error_messages = wf_alloc_string_concat_vformat(&parser->env->vm, 
                                                            parser->error_messages, format, args);
    va_end(args);
}

static void wf_push_newline_ignore(wf_Parser* parser, bool enabled)
{
    wf_bool_array_push(parser->env, &parser->ignore_line_stack, enabled);
}

static void wf_pop_newline_ignore(wf_Parser* parser)
{
    parser->ignore_line_stack.size--;
}

static bool wf_ignore_newlines(wf_Parser* parser)
{
    if(parser->ignore_line_stack.size == 0) return false;
    return parser->ignore_line_stack.items[parser->ignore_line_stack.size - 1];
}

static void wf_advance(wf_Parser* parser)
{
    parser->previous = parser->current;
    parser->current = wf_tokenizer_next(&parser->tokenizer);
    while(parser->current.type == WF_TOKEN_ERROR 
            || (wf_ignore_newlines(parser) && parser->current.type == WF_TOKEN_NEWLINE))
    {
        if(parser->current.type == WF_TOKEN_ERROR)
        {
            wf_push_error(parser, parser->current.position, "%s", parser->current.value.text);
        }
        parser->current = wf_tokenizer_next(&parser->tokenizer);
    }

}

static bool wf_expect_expression(wf_Parser* parser, wf_FilePosition position, wf_ExprNode* expr)
{
    if(expr == NULL)
    {
        wf_push_error(parser, position, "Expected an expression.");
        return false;
    }
    return true;
}

static bool wf_expect_close_paren(wf_Parser* parser, wf_FilePosition start)
{
    wf_advance(parser);
    if(parser->previous.type != WF_TOKEN_CLOSE_PAREN)
    {
        wf_push_error(parser, parser->previous.position, "Expected a ')'"
                        "to match the opening '(' at (ln: %"PRIu32", col: %"PRIu32").",
                        start.line, start.column);
        return false;
    }
    return true;
}

// Expressions

static wf_ExprNode* wf_parse_precedence(wf_Parser* parser, wf_ExprPrecedence precedence)
{
    wf_PrefixExprFunc prefix_func = rules[parser->current.type].prefix;
    if(prefix_func == NULL)
    {
        return NULL;
    }

    wf_ExprNode* prev = prefix_func(parser);
    if(prev == NULL)
    {
        return NULL;
    }

    if(rules[parser->current.type].infix == NULL)
    {
        return prev;
    }

    while(precedence <= rules[parser->current.type].precedence)
    {
        wf_InfixExprFunc infix_func = rules[parser->current.type].infix;
        prev = infix_func(parser, prev);

        if(prev == NULL)
        {
            return NULL;
        }
    }
    return prev;
}

static wf_ExprNode* wf_parse_expression(wf_Parser* parser)
{
    return wf_parse_precedence(parser, WF_PREC_ADDITIVE);
}

// Prefix expressions

static wf_ExprNode* wf_parse_literal(wf_Parser* parser)
{
    wf_advance(parser);

    switch(parser->previous.type)
    {
        case WF_TOKEN_INT:
        {
            wf_LiteralNode* node = (wf_LiteralNode*)wf_alloc_node(parser->env, WF_NODE_LITERAL);
            node->value_type = WF_LITERAL_INT;
            node->value = parser->previous.value;
            SET_NODE_POSITION(node, parser->previous.position);
            return (wf_ExprNode*)node;
        }
        case WF_TOKEN_FLOAT:
        {
            wf_LiteralNode* node = (wf_LiteralNode*)wf_alloc_node(parser->env, WF_NODE_LITERAL);
            node->value_type = WF_LITERAL_FLOAT;
            node->value = parser->previous.value;
            SET_NODE_POSITION(node, parser->previous.position);
            return (wf_ExprNode*)node;
        }
        default:
            break;
    }
    return NULL; // Unreachable
}

static wf_ExprNode* wf_parse_grouping(wf_Parser* parser)
{
    wf_FilePosition paren_pos = parser->current.position;
    wf_push_newline_ignore(parser, true);
    wf_advance(parser);
    wf_FilePosition expr_position = parser->current.position;
    wf_ExprNode* expr = wf_parse_expression(parser);
    wf_pop_newline_ignore(parser);

    if(!wf_expect_expression(parser, expr_position, expr)) return NULL;
    if(!wf_expect_close_paren(parser, paren_pos))
    {
        wf_destroy_node(parser->env, (wf_Node*)expr);
        return NULL;
    }

    return expr;
}

static wf_ExprNode* wf_parse_unary_op(wf_Parser* parser)
{
    wf_UnaryOpNode* node = (wf_UnaryOpNode*)wf_alloc_node(parser->env, WF_NODE_UNARY_OP);
    SET_NODE_POSITION(node, parser->current.position);

    wf_ExprPrecedence precedence;
    switch(parser->current.type)
    {
        case WF_TOKEN_MINUS:
            precedence = WF_PREC_SIGN;
            node->operation = WF_UNARY_OP_NEGATE;
            break;
        default:
            return NULL; // Unreachable
    }

    wf_advance(parser);
    wf_FilePosition operand_position = parser->current.position;
    node->operand = wf_parse_precedence(parser, precedence);
    if(!wf_expect_expression(parser, operand_position, node->operand))
    {
        wf_destroy_node(parser->env, (wf_Node*)node);
        return NULL;
    }
    return (wf_ExprNode*)node;
}

// Infix expressions

static wf_ExprNode* wf_parse_binary_op(wf_Parser* parser, wf_ExprNode* prev)
{
    wf_Token op_tok = parser->current;
    wf_advance(parser);
    wf_BinaryOpNode* node = (wf_BinaryOpNode*)wf_alloc_node(parser->env, WF_NODE_BINARY_OP);
    SET_NODE_POSITION(node, op_tok.position);
    node->left_operand = prev;
    wf_ExprPrecedence precedence = rules[op_tok.type].precedence + 1;

    wf_FilePosition right_pos = parser->current.position;
    node->right_operand = wf_parse_precedence(parser, precedence);

    if(!wf_expect_expression(parser, right_pos, node->right_operand)) return NULL;

    switch(op_tok.type)
    {
        case WF_TOKEN_PLUS:
            node->operation = WF_BINARY_OP_ADD;
            break;
        case WF_TOKEN_MINUS:
            node->operation = WF_BINARY_OP_SUB;
            break;
        case WF_TOKEN_STAR:
            node->operation = WF_BINARY_OP_MUL;
            break;
        case WF_TOKEN_SLASH:
            node->operation = WF_BINARY_OP_DIV;
            break;
        default:
            return NULL; // Unreachable
    }

    return (wf_ExprNode*)node;
}

static wf_ExprRule rules[] = {
    [WF_TOKEN_INT]          = { &wf_parse_literal,  NULL,                   WF_PREC_NONE            },
    [WF_TOKEN_FLOAT]        = { &wf_parse_literal,  NULL,                   WF_PREC_NONE            },

    [WF_TOKEN_PLUS]         = { NULL,               &wf_parse_binary_op,    WF_PREC_ADDITIVE        },
    [WF_TOKEN_MINUS]        = { &wf_parse_unary_op, &wf_parse_binary_op,    WF_PREC_ADDITIVE        },
    [WF_TOKEN_STAR]         = { NULL,               &wf_parse_binary_op,    WF_PREC_MULTIPLICATIVE  },
    [WF_TOKEN_SLASH]        = { NULL,               &wf_parse_binary_op,    WF_PREC_MULTIPLICATIVE  },

    [WF_TOKEN_OPEN_PAREN]   = { &wf_parse_grouping, NULL,                   WF_PREC_NONE            },
    [WF_TOKEN_CLOSE_PAREN]  = { NULL,               NULL,                   WF_PREC_NONE            },

    [WF_TOKEN_ERROR]        = { NULL,               NULL,                   WF_PREC_NONE            },
    [WF_TOKEN_NEWLINE]      = { NULL,               NULL,                   WF_PREC_NONE            },
    [WF_TOKEN_EOF]          = { NULL,               NULL,                   WF_PREC_NONE            },
};

// End of expressions

void wf_parser_init(wf_Environment* env, wf_Parser* parser, const wf_CompileInfo* info)
{
    parser->info = info;
    parser->env = env;
    parser->error_count = 0;
    parser->error_messages = NULL;
    wf_tokenizer_init(&parser->tokenizer, info->main_file_path, info->main_file_text);
    wf_bool_array_init(&parser->ignore_line_stack);
    wf_advance(parser);
}

void wf_parser_shutdown(wf_Parser* parser)
{
    wf_bool_array_destroy(parser->env, &parser->ignore_line_stack);
    // Will be garbage collected.
    parser->error_messages = NULL;
}

wf_Node* wf_parser_parse(wf_Parser* parser)
{
    wf_push_newline_ignore(parser, false);
    wf_Node* ast = (wf_Node*)wf_parse_expression(parser);
    wf_pop_newline_ignore(parser);
    if(parser->error_count != 0 && parser->current.type == WF_TOKEN_EOF)
    {
        wf_push_error(parser, parser->current.position, "Expected an operator");
        wf_destroy_node(parser->env, ast);
    }
    return ast;
}
