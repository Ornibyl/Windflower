#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef int64_t wf_Int;
typedef uint64_t wf_UInt;
typedef double wf_Float;

typedef void*(*wf_AllocatorFunc)(void* ptr, size_t old_size, size_t new_size, void* user_data);

typedef struct wf_Environment wf_Environment;

typedef struct wf_CompileInfo
{
    const char* main_file_path;
    const char* main_file_text;
    uint16_t max_error_count;
    bool use_error_colors; 
} wf_CompileInfo;

typedef enum wf_Status
{
    WF_OK,
    WF_COMPILATION_ERROR,
    WF_FILE_ERROR,
    WF_OUT_OF_MEMORY,
} wf_Status;

wf_Environment* wf_new_environment(wf_AllocatorFunc allocator_func, void* allocator_user_data);
void wf_destroy_environment(wf_Environment* env);

wf_Status wf_compile(wf_Environment* env, const wf_CompileInfo* compile_info);

wf_Int wf_get_int(wf_Environment* env, int idx);
wf_Float wf_get_float(wf_Environment* env, int idx);
const char* wf_get_string(wf_Environment* env, int idx);

void wf_push_int(wf_Environment* env, wf_Int value);
void wf_push_float(wf_Environment* env, wf_Float value);
void wf_push_string(wf_Environment* env, const char* value, size_t length);
void wf_push_cstr(wf_Environment* env, const char* value);

size_t wf_get_size(wf_Environment* env);
void wf_pop(wf_Environment* env);
void wf_remove(wf_Environment* env, int idx);

#ifdef __cplusplus
}
#endif
