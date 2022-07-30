#pragma once

#include <stdlib.h>

typedef struct wf_FilePosition
{
    const char* file_path;
    uint32_t line;
    uint32_t column;
} wf_FilePosition;