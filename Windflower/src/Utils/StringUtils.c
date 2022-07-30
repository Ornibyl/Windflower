#include "StringUtils.h"

#include <string.h>

#include "Allocation.h"

void wf_string_array_init(wf_StringArray* arr)
{
    arr->items = NULL;
    arr->capacity = 0;
    arr->size = 0;
}

void wf_string_array_destroy(wf_Environment* env, wf_StringArray* arr)
{
    wf_free(env, arr->items, sizeof(wf_Value) * arr->capacity);
}

void wf_string_array_push(wf_Environment* env, wf_StringArray* arr, wf_StringObj* value)
{
    arr->size++;
    if(arr->size < WF_ARRAY_INITIAL_SIZE)
    {
        arr->items = wf_malloc(env, sizeof(wf_StringObj*) * WF_ARRAY_INITIAL_SIZE);
        arr->capacity = WF_ARRAY_INITIAL_SIZE;
    }
    else if(arr->size > arr->capacity)
    {
        size_t old_capacity = arr->capacity;
        arr->capacity = arr->capacity * WF_ARRAY_GROW_FACTOR;
        arr->items = wf_realloc(env, arr->items, sizeof(wf_StringObj*) * old_capacity, 
                                sizeof(wf_StringObj*) * arr->capacity);
    }
    arr->items[arr->size - 1] = value;
}

wf_StringSlice wf_str_slice_from_cstr(const char* text)
{
    return (wf_StringSlice){.text = text, .length = strlen(text) };
}
