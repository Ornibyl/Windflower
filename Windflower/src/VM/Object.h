#pragma once

#include "Value.h"

#define AS_STRING(value) ((wf_StringObj*)(value).as.wf_object)
#define AS_BYTECODE(value) ((wf_BytecodeObj*)(value).as.wf_object)

typedef enum wf_ObjectType
{
    WF_OBJECT_STRING,
    WF_OBJECT_BYTECODE,
} wf_ObjectType;

struct wf_Object
{
    wf_ObjectType type;
    wf_Object* next;
};

typedef struct wf_StringObj
{
    wf_Object obj;
    size_t length;
    char text[];
} wf_StringObj;

typedef struct wf_BytecodeObj
{
    wf_Object obj;
} wf_BytecodeObj;

static inline bool is_object_type(wf_Value value, wf_ObjectType type)
{
    return value.type == WF_VALUE_OBJECT && value.as.wf_object->type == type;
}

typedef struct wf_ObjectArray
{
    wf_Object** items;
    size_t capacity;
    size_t size;
} wf_ObjectArray;

void wf_object_array_init(wf_ObjectArray* arr);
void wf_object_array_destroy(wf_Environment* env, wf_ObjectArray* arr);
void wf_object_array_push(wf_Environment* env, wf_ObjectArray* arr, wf_Object* value);
