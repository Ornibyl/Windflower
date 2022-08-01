#pragma once

#include <stdarg.h>

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

/**
 * @brief Creates a new string with the previous contents of base with contents appended afterwards
 * based on format.
 */
WF_FORMAT_ATTRIBUTE(3, 4) 
wf_StringObj* wf_alloc_string_concat_format(wf_VM* vm, wf_StringObj* base, const char* format, ...);

wf_StringObj* wf_alloc_string_vformat(wf_VM* vm, const char* format, va_list args);
wf_StringObj* wf_alloc_string_concat_vformat(wf_VM* vm, wf_StringObj* base, const char* format, 
                                                va_list args);

wf_BytecodeObj* wf_alloc_bytecode(wf_VM* vm);
