#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>



void getAllCodeFiles()
{
    int files_compiled =0;
    struct dirent *entity;
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    DIR *dir = opendir(cwd);
    if (dir == NULL)
    {
        printf("Cannot open directory");
        exit(0);
    }

    while ((entity = readdir(dir)) != NULL)
    {
        if (entity->d_type != DT_DIR)
        {
            // Skip non-c or cpp files
            if (strstr(entity->d_name, ".c") != NULL)
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
                    sprintf(compile_command, "gcc %s -o %s", entity->d_name, output_file_name);
                }
                else
                {
                    sprintf(compile_command, "g++ %s -o %s", entity->d_name, output_file_name);
                }
                
                printf("Running: %s\n", compile_command);
                
                system(compile_command);

                sprintf(move_command, "mv %s %s", output_file_name, "executables");
                system(move_command);
                files_compiled++;
                
            }
        }
    }

    printf("Compiled %d files\n", files_compiled);

}


int main()
{
    system("mkdir executables");
    getAllCodeFiles();


    return 0;
}