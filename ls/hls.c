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

    while ((entry = readdir(dir)) != NULL) {
        if (!include_hidden && entry->d_name[0] == '.')
        {
            continue;
        }
        if (list_long) {
            struct stat sb;
            char fullpath[1024];
            sprintf(fullpath, "%s/%s", path, entry->d_name);

            if (lstat(fullpath, &sb) == -1) {
                perror("Error getting file status");
                continue;
            }

            printf("%s\n", entry->d_name);
        } else {
            printf("%s ", entry->d_name);
        }
    }

    closedir(dir);
    if (!list_long) {
        printf("\n");
    }
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
                            fprintf(stderr, "hls: invalid option -- '%c'\n", argv[i][j]);
                            exit(EXIT_FAILURE);
                    }
                }
            } else {
                path = argv[i];
            }
            i++;
        }
    }

    list_dir(path, include_hidden,list_long);

    return EXIT_SUCCESS;
}
