#pragma once

#include "Allocation.h"

typedef struct wf_BoolArray
{
    bool* items;
    size_t capacity;
    size_t size;
} wf_BoolArray;

void wf_bool_array_init(wf_BoolArray* arr);
void wf_bool_array_destroy(wf_Environment* env, wf_BoolArray* arr);
void wf_bool_array_push(wf_Environment* env, wf_BoolArray* arr, bool value);
