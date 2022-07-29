#include "VM.h"

#include "Utils/Allocation.h"

static void free_object(wf_VM* vm, wf_Object* object)
{
    switch(object->type)
    {
        case WF_OBJECT_STRING:
        {
            wf_StringObj* as_string = (wf_StringObj*)object;
            wf_free(vm->env, as_string, sizeof(wf_StringObj) + as_string->length + 1);
            break;
        }
        case WF_OBJECT_BYTECODE:
        {
            wf_BytecodeObj* as_code = (wf_BytecodeObj*)object;
            wf_free(vm->env, as_code, sizeof(wf_BytecodeObj));
        }
    }
}

static void push_allocated_object(wf_VM* vm, wf_Object* obj)
{
    obj->next = vm->allocated_objects;
    vm->allocated_objects = obj;
}

void wf_vm_init(wf_Environment* env, wf_VM* vm)
{
    vm->env = env;
    vm->allocated_objects = NULL;
    wf_value_array_init(&vm->stack);
}

void wf_vm_destroy(wf_VM* vm)
{
    for(wf_Object* object = vm->allocated_objects; object != NULL;)
    {
        wf_Object* next = object->next;
        free_object(vm, object);
        object = next;
    }
    wf_value_array_destroy(vm->env, &vm->stack);
}

wf_StringObj* wf_alloc_string(wf_VM* vm, size_t length)
{
    wf_StringObj* obj = (wf_StringObj*)wf_malloc(vm->env, sizeof(wf_StringObj) + length + 1);
    obj->obj.type = WF_OBJECT_STRING;
    obj->length = length;
    push_allocated_object(vm, (wf_Object*)obj);
    return obj;
}

wf_BytecodeObj* wf_alloc_bytecode(wf_VM* vm)
{
    wf_BytecodeObj* obj = (wf_BytecodeObj*)wf_malloc(vm->env, sizeof(wf_BytecodeObj*));
    obj->obj.type = WF_OBJECT_BYTECODE;
    push_allocated_object(vm, (wf_Object*)obj);
    return obj;
}