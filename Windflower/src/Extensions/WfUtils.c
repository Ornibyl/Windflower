#include "Windflower/WfUtils.h"

#include <stdio.h>
#include <stdlib.h>

#include "Utils/Allocation.h"
#include "Environment.h"
#include "VM/VM.h"

wf_Status wf_load_file(wf_Environment* env, const char* file_path)
{
    FILE* file = fopen(file_path, "rb");
    if(file == NULL)
    {
        return WF_FILE_ERROR;
    }

    fseek(file, 0L, SEEK_END);
    size_t file_size = (size_t)ftell(file);
    rewind(file);
    
    wf_StringObj* str = wf_alloc_string(&env->vm, file_size);

    size_t bytes_read = fread(str->text, sizeof(char), file_size, file);
    fclose(file);

    if(file_size < bytes_read)
    {
        return WF_FILE_ERROR;
    }

    str->text[bytes_read] = '\0';

    wf_value_array_push(env, &env->vm.stack, OBJ_VALUE(str));
    return WF_OK;
}

void* wf_default_allocator(void* ptr, size_t old_size, size_t new_size, void* user_data)
{
    if(new_size == 0)
    {
        free(ptr);
        return NULL;
    }
    return realloc(ptr, new_size);
}

void wf_compile_info_defaults(wf_CompileInfo* info)
{
    info->main_file_text = NULL;
    info->main_file_path = NULL;
    info->max_error_count = 20;
    info->use_error_colors = true;
}
