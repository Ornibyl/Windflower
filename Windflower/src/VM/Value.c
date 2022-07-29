#include "Value.h"

#include "Utils/Allocation.h"

void wf_value_array_init(wf_ValueArray* arr)
{
    arr->items = NULL;
    arr->capacity = 0;
    arr->size = 0;
}

void wf_value_array_destroy(wf_Environment* env, wf_ValueArray* arr)
{
    wf_free(env, arr->items, sizeof(wf_Value) * arr->capacity);
}

void wf_value_array_push(wf_Environment* env, wf_ValueArray* arr, wf_Value value)
{
    arr->size++;
    if(arr->size < WF_ARRAY_INITIAL_SIZE)
    {
        arr->items = wf_malloc(env, sizeof(wf_Value) * WF_ARRAY_INITIAL_SIZE);
        arr->capacity = WF_ARRAY_INITIAL_SIZE;
    }
    else if(arr->size > arr->capacity)
    {
        size_t old_capacity = arr->capacity;
        arr->capacity = arr->capacity * WF_ARRAY_GROW_FACTOR;
        arr->items = wf_realloc(env, arr->items, sizeof(wf_Value) * old_capacity, 
                                sizeof(wf_Value) * arr->capacity);
    }
    arr->items[arr->size - 1] = value;
}
