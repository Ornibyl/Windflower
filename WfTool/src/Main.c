#include <Windflower/Windflower.h>
#include <Windflower/WfUtils.h>

#include <stdlib.h>
#include <stdio.h>

int main(int argc, const char* argv[])
{
    if(argc != 2)
    {
        fprintf(stderr, "Usage: wftool [file-path]\n");
        return EXIT_FAILURE;
    }
    wf_Environment* env = wf_new_environment(&wf_default_allocator, NULL);

    switch(wf_load_file(env, argv[1]))
    {
        case WF_OUT_OF_MEMORY:
            fprintf(stderr, "Out of memory.\n");
            wf_destroy_environment(env);
            return EXIT_FAILURE;
        case WF_FILE_ERROR:
            fprintf(stderr, "Could not run file '%s'.\n", argv[1]);
            wf_destroy_environment(env);
            return EXIT_FAILURE;
        default:
            break;
    }
    size_t file_text_position = wf_get_size(env) - 1;

    wf_CompileInfo info;
    info.main_file_path = argv[1];
    info.main_file_text = wf_get_string(env, -1);
    switch(wf_compile(env, &info))
    {
        case WF_OUT_OF_MEMORY:
            fprintf(stderr, "Out of memory.\n");
            wf_destroy_environment(env);
            return EXIT_FAILURE;
        case WF_COMPILATION_ERROR:
        {
            const char* error_messages = wf_get_string(env, -1);
            fprintf(stderr, "%s\n", error_messages);
            wf_destroy_environment(env);
            return EXIT_FAILURE;
        }
        default:
            break;
    }

    wf_remove(env, (int)file_text_position);

    wf_destroy_environment(env);
    printf("Exited successfully.\n");
    
    return EXIT_SUCCESS;
}
