#pragma once

#include <stdio.h>

#include "Tokenizer.h"
#include "Nodes.h"

/**
 * @brief Runs through the entire text file of the tokenizer and 
 * prints a formatted output to file.
 * The tokenizer should already be initialized.
 * 
 */
void wf_debug_tokenizer(FILE* file, wf_Tokenizer* tokenizer);

void wf_debug_ast(FILE* file, wf_Node* node);
