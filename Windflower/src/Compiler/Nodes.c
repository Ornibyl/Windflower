#include "Nodes.h"

#include "Utils/Allocation.h"

#define SET_NODE_TYPE(node, type) (((wf_Node*)(node))->type = (type))

wf_Node* wf_alloc_node(wf_Environment* env, wf_NodeType type)
{
    switch(type)
    {
        case WF_NODE_LITERAL:
        {
            wf_LiteralNode* node = (wf_LiteralNode*)wf_malloc(env, sizeof(wf_LiteralNode));
            SET_NODE_TYPE(node, type);
            return (wf_Node*)node;
        }
        case WF_NODE_BINARY_OP:
        {
            wf_BinaryOpNode* node = (wf_BinaryOpNode*)wf_malloc(env, sizeof(wf_BinaryOpNode));
            SET_NODE_TYPE(node, type);
            node->left_operand = NULL;
            node->right_operand = NULL;
            return (wf_Node*)node;
        }
        case WF_NODE_UNARY_OP:
        {
            wf_UnaryOpNode* node = (wf_UnaryOpNode*)wf_malloc(env, sizeof(wf_UnaryOpNode));
            SET_NODE_TYPE(node, type);
            node->operand = NULL;
            return (wf_Node*)node;
        }
    }
    return NULL;
}

void wf_destroy_node(wf_Environment* env, wf_Node* node)
{
    switch(node->type)
    {
        case WF_NODE_LITERAL:
            wf_free(env, node, sizeof(wf_LiteralNode));
            break;
        case WF_NODE_BINARY_OP:
        {
            wf_BinaryOpNode* as_binary_op = (wf_BinaryOpNode*)node;
            wf_destroy_node(env, (wf_Node*)as_binary_op->left_operand);
            wf_destroy_node(env, (wf_Node*)as_binary_op->right_operand);

            wf_free(env, node, sizeof(wf_BinaryOpNode));
            break;
        }
        case WF_NODE_UNARY_OP:
        {
            wf_UnaryOpNode* as_unary_op = (wf_UnaryOpNode*)node;
            wf_destroy_node(env, (wf_Node*)as_unary_op->operand);

            wf_free(env, node, sizeof(wf_UnaryOpNode));
            break;
        }
    }
}
