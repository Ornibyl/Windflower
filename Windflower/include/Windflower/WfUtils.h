#pragma once

/**
 * A collection of helpful utility functions that make using Windflower easier.
 * These aren't included as part of Windflower.h as they are meant to be an optional component.
 */

#include "Windflower.h"

/**
 * @brief Reads the contents of the file at file_path and pushes it as a string on the stack.
 */
wf_Status wf_load_file(wf_Environment* env, const char* file_path);

void* wf_default_allocator(void* ptr, size_t old_size, size_t new_size, void* user_data);
