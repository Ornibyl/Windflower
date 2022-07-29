#pragma once

#include "Windflower/Windflower.h"
#include "VM/VM.h"

struct wf_Environment
{
    wf_AllocatorFunc allocator_func; 
    void* allocator_user_data;
    wf_VM vm;
};
