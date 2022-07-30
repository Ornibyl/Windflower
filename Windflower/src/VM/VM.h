#pragma once

#include "Object.h"

#ifdef __clang__
#define WF_FORMAT_ATTRIBUTE(format, args) __attribute__((__format__ (__printf__, format, args)))
#else
#define WF_FORMAT_ATTRIBUTE(format, args)
#endif

typedef struct wf_VM
{
    wf_Environment* env;
    wf_Object* allocated_objects;
    wf_ValueArray stack;
} wf_VM;

void wf_vm_init(wf_Environment* env, wf_VM* vm);
void wf_vm_destroy(wf_VM* vm);

wf_StringObj* wf_alloc_string(wf_VM* vm, size_t length);

WF_FORMAT_ATTRIBUTE(2, 3) 
wf_StringObj* wf_alloc_string_format(wf_VM* vm, const char* format, ...);

wf_BytecodeObj* wf_alloc_bytecode(wf_VM* vm);
