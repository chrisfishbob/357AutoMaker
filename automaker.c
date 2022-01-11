#define _GNU_SOURCE
#define C_COMPILER "gcc"
#define CPP_COMPILER "g++"
#define OUTPUT_FOLDER_NAME "output"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>


int nameIsValid(char *file_name)
{
    int dot_count = 0;

    for (int i = 0; i < strlen(file_name) - 1; i++)
    {
        if (file_name[i] == '.')
        {
            dot_count++;
        }
    }

    // If dot_count is not one, automatically invalid
    if (dot_count != 1)
    {
        printf("File name %s is invalid. Aborting compilation of this file\n", file_name);
        return 0;
    }

    return strstr(file_name, ".c") != NULL;
}


void getAllCodeFiles()
{
    int files_compiled =0;
    struct dirent *entity;
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    DIR *dir = opendir(cwd);
    if (dir == NULL)
    {
        printf("Cannot open directory\n");
        exit(-1);
    }

    while ((entity = readdir(dir)) != NULL)
    {
        if (entity->d_type != DT_DIR)
        {
            // Skip non-c or cpp files
            if (nameIsValid(entity->d_name))
            {
                char compile_command[1024];
                char move_command[1024];
                char output_file_name[1024];

                // We need to strcpy the file name so that strtok does not destroy entity->d_name
                strcpy(output_file_name, entity->d_name);
                // Extract file name without extension
                strtok(output_file_name, ".");

                // If the substring ".cpp" is not found, the file must be a .c file
                if (strstr(entity->d_name, ".cpp") == NULL)
                {
                    sprintf(compile_command, "%s %s -o %s", C_COMPILER, entity->d_name, output_file_name);
                }
                else
                {
                    sprintf(compile_command, "%s %s -o %s", CPP_COMPILER, entity->d_name, output_file_name);
                }
                
                printf("Running: %s\n", compile_command);
                system(compile_command);
                sprintf(move_command, "mv %s %s", output_file_name, OUTPUT_FOLDER_NAME);
                system(move_command);
                files_compiled++;
            }
        }
    }

    printf("Compiled %d files\n", files_compiled);
}


void createOutputFolder()
{
    char mkdir_command[1024];
    sprintf(mkdir_command, "mkdir %s", OUTPUT_FOLDER_NAME);
    system(mkdir_command);
}


int main()
{
    createOutputFolder();
    getAllCodeFiles();


    return 0;
}