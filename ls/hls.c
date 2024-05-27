#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include "hls.h"

/**
* list_dir-  directory listing
* @path - path
* return: list
*/
void list_dir(const char *path, int include_hidden) {
    DIR *dir;
    struct dirent *entry;
    
    if (!(dir = opendir(path)))
    {
        fprintf(stderr, "./hls: cannot access %s: %s\n", path, perror(errno));
        return;
    }
  if (!(dir = opendir(path))) {
        printf(perror(errno));
    }

    while ((entry = readdir(dir)) != NULL) {
        if (list_dir) {
            struct stat sb;
            char fullpath[1024]; // Adjust size as needed
            snprintf(fullpath, sizeof(fullpath), "%s/%s", path, entry->d_name);

            if (lstat(fullpath, &sb) == -1) {
                print_error(strerror(errno));
            }

            printf("%s\n", entry->d_name);
        } else {
            printf("%s ", entry->d_name);
        }
    }

    closedir(dir);
}

int main(int argc, char *argv[]) {
    char *path;
    int i, j;
    int include_hidden;

    if (argc == 1) {
        path = ".";
    } else {
        i = 1;
        while (i < argc) {
            if (argv[i][0] == '-') {
                for (j = 1; argv[i][j] != '\0'; j++) {
                    switch (argv[i][j]) {
                        case '1':
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

    list_dir(path, include_hidden);

    return EXIT_SUCCESS;
}