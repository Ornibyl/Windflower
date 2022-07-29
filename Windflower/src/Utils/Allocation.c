#include "Allocation.h"

#include <stdlib.h>

#include "Environment.h"

#define WF_INITIAL_ARRAY_SIZE (size_t)8
#define WF_ARRAY_GROW_FACTOR (size_t)2

void* wf_malloc(wf_Environment* env, size_t bytes)
{
    void* ptr = env->allocator_func(NULL, 0, bytes, env->allocator_user_data);
    if(ptr == NULL)
    {
        // There's definetely a better way to handle this considering this is library code
       abort();
    }
    return ptr;
}

void wf_free(wf_Environment* env, void* ptr, size_t bytes)
{
    env->allocator_func(ptr, bytes, 0, env->allocator_user_data);
}

void* wf_realloc(wf_Environment* env, void* ptr, size_t old_size, size_t new_size)
{
    ptr = env->allocator_func(ptr, old_size, new_size, env->allocator_user_data);
    if(ptr == NULL)
    {
        abort();
    }
    return ptr;
}

void wf_impl_array_grow(wf_Environment* env, void** ptr, size_t* capacity, 
                            size_t* size, size_t element_size, size_t amount)
{
    size_t old_capacity = *capacity;
    *size = amount;
    while(*size > *capacity)
    {
        if(*capacity < WF_INITIAL_ARRAY_SIZE)
        {
            *capacity = WF_INITIAL_ARRAY_SIZE;
        }
        else
        {
            *capacity *= WF_ARRAY_GROW_FACTOR;
        }
    }

    if(*capacity != old_capacity)
    {
        *ptr = wf_realloc(env, *ptr, old_capacity, *capacity);
    }
}
