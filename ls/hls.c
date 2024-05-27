#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>

/**
* list_dir-  directory listing
* @path - path
* return: list
*/

void list_dir(const char *path, int include_hidden, int list_long) {
    DIR *dir;
    struct dirent *entry;
    
    if (!(dir = opendir(path)))
    {
         fprintf(stderr, "./hls_01: cannot access %s: No such file or directory\n", path);
        return;
    }

    printf("%s:\n", path);

    while ((entry = readdir(dir)) != NULL) {
        if (!include_hidden && entry->d_name[0] == '.')
        {
            continue;
        }
        if (list_long)
        {
            struct stat sb;
            char fullpath[1024];
            sprintf(fullpath, "%s/%s", path, entry->d_name);

            if (lstat(fullpath, &sb) == -1) {
                perror("Error getting file status");
                continue;
            }

            printf("%s\n", entry->d_name);
        } else {
            printf("%s\n", entry->d_name);
        }
        
        if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            char subdir[1024];
            sprintf(subdir, "%s/%s", path, entry->d_name);
            list_dir(subdir, include_hidden, list_long);
        }
    }

    closedir(dir);
}


/**
* main - main function
* @argc - argument count
* @argv - number or arguments
*/
int main(int argc, char *argv[]) {
    int list_long = 0;
    int include_hidden = 0;
    char *path = ".";
    int i, j;

    if (argc >1) {
        i = 1;
        while (i < argc) {
            if (argv[i][0] == '-') {
                for (j = 1; argv[i][j] != '\0'; j++) {
                    switch (argv[i][j]) {
                        case '1':
                            list_long = 1;
                            break;
                        default:
                            printf("invalid option");
                            exit(EXIT_FAILURE);
                    }
                }
            } else {
                path = argv[i];
            }
            i++;
        }
    }

    struct stat path_stat;
    if (stat(path, &path_stat) != -1) {
        if (S_ISREG(path_stat.st_mode)) {
            printf("[Got]\n");
            printf("%s\n", path);
        } else if (S_ISDIR(path_stat.st_mode)) {
            list_dir(path, include_hidden, list_long);
        }
    } else {
        fprintf(strerror(errno));
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}
