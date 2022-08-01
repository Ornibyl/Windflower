#pragma once

#include "Windflower/Windflower.h"
#include "FilePosition.h"
#include "Utils/StringUtils.h"

#define SET_NODE_POSITION(node, pos) (((wf_Node*)(node))->position = (pos))

typedef enum wf_NodeType
{
    WF_NODE_LITERAL,
    WF_NODE_BINARY_OP,
    WF_NODE_UNARY_OP,
} wf_NodeType;

typedef enum wf_BinaryOpType
{
    WF_BINARY_OP_ADD, 
    WF_BINARY_OP_SUB,
    WF_BINARY_OP_MUL,
    WF_BINARY_OP_DIV,
} wf_BinaryOpType;

typedef enum wf_UnaryOpType
{
    WF_UNARY_OP_NEGATE,
} wf_UnaryOpType;

typedef enum wf_LiteralType
{
    WF_LITERAL_INT,
    WF_LITERAL_FLOAT,
} wf_LiteralType;

typedef struct wf_Node
{
    wf_NodeType type;
    wf_FilePosition position;
} wf_Node;

// When the type checker is added, this will contain more data.
typedef struct wf_ExprNode
{
    wf_Node node;
} wf_ExprNode;

typedef struct wf_BinaryOpNode
{
    wf_ExprNode expr;

    wf_BinaryOpType operation;
    wf_ExprNode* left_operand;
    wf_ExprNode* right_operand;
} wf_BinaryOpNode;

typedef struct wf_UnaryOpNode
{
    wf_ExprNode expr;

    wf_UnaryOpType operation;
    wf_ExprNode* operand;
} wf_UnaryOpNode;

typedef struct wf_LiteralNode
{
    wf_ExprNode expr;
    wf_LiteralType value_type;
    wf_StringSlice value;
} wf_LiteralNode;

wf_Node* wf_alloc_node(wf_Environment* env, wf_NodeType type);
void wf_destroy_node(wf_Environment* env, wf_Node* node);
