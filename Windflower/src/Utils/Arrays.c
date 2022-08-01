#include "Arrays.h"

void wf_bool_array_init(wf_BoolArray* arr)
{
    arr->items = NULL;
    arr->capacity = 0;
    arr->size = 0;
}

void wf_bool_array_destroy(wf_Environment* env, wf_BoolArray* arr)
{
    wf_free(env, arr->items, sizeof(bool) * arr->capacity);
}

void wf_bool_array_push(wf_Environment* env, wf_BoolArray* arr, bool value)
{
    arr->size++;
    if(arr->size < WF_ARRAY_INITIAL_SIZE)
    {
        arr->items = wf_malloc(env, sizeof(bool) * WF_ARRAY_INITIAL_SIZE);
        arr->capacity = WF_ARRAY_INITIAL_SIZE;
    }
    else if(arr->size > arr->capacity)
    {
        size_t old_capacity = arr->capacity;
        arr->capacity = arr->capacity * WF_ARRAY_GROW_FACTOR;
        arr->items = wf_realloc(env, arr->items, sizeof(bool) * old_capacity, 
                                sizeof(bool) * arr->capacity);
    }
    arr->items[arr->size - 1] = value;
}
