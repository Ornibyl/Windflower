#pragma once

#include "Windflower/Windflower.h"

#define AS_INT(value) ((value).as.wf_int)
#define AS_FLOAT(value) ((value).as.wf_float)
#define AS_OBJ(value) ((value).as.wf_object)

#define INT_VALUE(value) (wf_Value){.type = WF_VALUE_INT, .as.wf_int = (value)}
#define FLOAT_VALUE(value) (wf_Value){.type = WF_VALUE_FLOAT, .as.wf_float = (value)}
#define OBJ_VALUE(value) (wf_Value){.type = WF_VALUE_OBJECT, .as.wf_object = (wf_Object*)(value)}

typedef struct wf_Object wf_Object;

typedef enum wf_ValueType
{
    WF_VALUE_INT,
    WF_VALUE_FLOAT,
    WF_VALUE_OBJECT,
} wf_ValueType;

typedef struct wf_Value
{
    wf_ValueType type;
    union
    {
        wf_Int wf_int;
        wf_Float wf_float;
        wf_Object* wf_object;
    } as;
} wf_Value;

typedef struct wf_ValueArray
{
    wf_Value* items;
    size_t capacity;
    size_t size;
} wf_ValueArray;

void wf_value_array_init(wf_ValueArray* arr);
void wf_value_array_destroy(wf_Environment* env, wf_ValueArray* arr);
void wf_value_array_push(wf_Environment* env, wf_ValueArray* arr, wf_Value value);
