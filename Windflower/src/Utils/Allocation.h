#pragma once

#include "Windflower/Windflower.h"

#define WF_ARRAY_INITIAL_SIZE (size_t)8
#define WF_ARRAY_GROW_FACTOR (size_t)2

void* wf_malloc(wf_Environment* env, size_t bytes);
void wf_free(wf_Environment* env, void* ptr, size_t bytes);
void* wf_realloc(wf_Environment* env, void* ptr, size_t old_size, size_t new_size);

void wf_impl_array_push(wf_Environment* env, void** ptr, size_t* capacity, 
                            size_t* size, size_t element_size);
