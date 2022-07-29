#include "Environment.h"

#include <string.h>

wf_Environment* wf_new_environment(wf_AllocatorFunc allocator_func, void* allocator_user_data)
{
    wf_Environment* env = (wf_Environment*)allocator_func(NULL, 0, sizeof(wf_Environment), 
                                                            allocator_user_data);
    env->allocator_func = allocator_func;
    env->allocator_user_data = allocator_user_data;
    wf_vm_init(env, &env->vm);
    return env;
}

void wf_destroy_environment(wf_Environment* env)
{
    wf_vm_destroy(&env->vm);
    env->allocator_func(env, sizeof(wf_Environment), 0, env->allocator_user_data);
}

wf_Status wf_compile(wf_Environment* env, const wf_CompileInfo* compile_info)
{
    wf_BytecodeObj* code = wf_alloc_bytecode(&env->vm);
    wf_value_array_push(env, &env->vm.stack, OBJ_VALUE(code));
    
    return WF_OK;
}

wf_Int wf_get_int(wf_Environment* env, int idx)
{
    if(idx < 0)
    {
        return AS_INT(env->vm.stack.items[(int)env->vm.stack.size + idx]);
    }
    return AS_INT(env->vm.stack.items[idx]);
}

wf_Float wf_get_float(wf_Environment* env, int idx)
{
    if(idx < 0)
    {
        return AS_FLOAT(env->vm.stack.items[(int)env->vm.stack.size + idx]);
    }
    return AS_FLOAT(env->vm.stack.items[idx]);
}

const char* wf_get_string(wf_Environment* env, int idx)
{
    if(idx < 0)
    {
        return AS_STRING(env->vm.stack.items[(int)env->vm.stack.size + idx])->text;
    }
    return AS_STRING(env->vm.stack.items[idx])->text;
}

void wf_push_int(wf_Environment* env, wf_Int value)
{
    wf_value_array_push(env, &env->vm.stack, INT_VALUE(value));
}

void wf_push_float(wf_Environment* env, wf_Float value)
{
    wf_value_array_push(env, &env->vm.stack, FLOAT_VALUE(value));
}

void wf_push_string(wf_Environment* env, const char* value, size_t length)
{
    wf_StringObj* obj = wf_alloc_string(&env->vm, length);
    memcpy(obj->text, value, obj->length);
    wf_value_array_push(env, &env->vm.stack, OBJ_VALUE(obj));
}

void wf_push_cstr(wf_Environment* env, const char* value)
{
    wf_StringObj* obj = wf_alloc_string(&env->vm, strlen(value));
    memcpy(obj->text, value, obj->length);
    wf_value_array_push(env, &env->vm.stack, OBJ_VALUE(obj));
}

size_t wf_get_size(wf_Environment* env)
{
    return env->vm.stack.size;
}

void wf_pop(wf_Environment* env)
{
    env->vm.stack.size--;
}

void wf_remove(wf_Environment* env, int idx)
{
    size_t absolute_index;
    
    wf_ValueArray* stack = &env->vm.stack;
    if(idx < 0)
    {
        absolute_index = (size_t)((int)stack->size + idx);
    }
    else
    {
        absolute_index = (size_t)idx;
    }
    if(absolute_index == stack->size - 1)
    {
        wf_pop(env);
        return;
    }
    else if(absolute_index > stack->size - 1)
    {
        return;
    }
    while(absolute_index + 1 != stack->size)
    {
        stack->items[absolute_index] = stack->items[absolute_index + 1];
        absolute_index++;
    }
    stack->size--;
}
