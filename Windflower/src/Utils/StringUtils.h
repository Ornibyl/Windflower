#pragma once

#include "VM/Object.h"

typedef struct wf_StringSlice
{
    const char* text;
    size_t length;
} wf_StringSlice;

typedef struct wf_StringArray
{
    wf_StringObj** items;
    size_t capacity;
    size_t size;
} wf_StringArray;

void wf_string_array_init(wf_StringArray* arr);
void wf_string_array_destroy(wf_Environment* env, wf_StringArray* arr);
void wf_string_array_push(wf_Environment* env, wf_StringArray* arr, wf_StringObj* value);

wf_StringSlice wf_str_slice_from_cstr(const char* text);
