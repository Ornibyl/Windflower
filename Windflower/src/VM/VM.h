#pragma once

#include "Object.h"

typedef struct wf_VM
{
    wf_Environment* env;
    wf_Object* allocated_objects;
    wf_ValueArray stack;
} wf_VM;

void wf_vm_init(wf_Environment* env, wf_VM* vm);
void wf_vm_destroy(wf_VM* vm);

wf_StringObj* wf_alloc_string(wf_VM* vm, size_t length);
wf_BytecodeObj* wf_alloc_bytecode(wf_VM* vm);
