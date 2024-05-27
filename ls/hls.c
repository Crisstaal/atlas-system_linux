#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include "hls.h"
#include <errno.h>
#include <string.h>

/**
* list_dir-  directory listing
* @path - path
* return: list
*/
/*
void list_dir(const char *path, int include_hidden) {
    DIR *dir;
    struct dirent *entry;
    
    if (!(dir = opendir(path)))
    {
       perror("opendir");
        return;
    }
    while ((entry = readdir(dir)) != NULL) {
        if (!include_hidden && entry->d_name[0] == '.') {
            continue;
        }

            struct stat sb;
            char fullpath[1024];
            sprintf(fullpath, sizeof(fullpath), "%s/%s", path, entry->d_name);

            if (lstat(fullpath, &sb) == -1) {
            perror("lstat");
            continue;
        }

        printf("%s", entry->d_name);

        if (S_ISLNK(sb.st_mode)) {
            char link_target[1024];
            ssize_t len = readdir(fullpath, link_target, sizeof(link_target) - 1);
            if (len != -1) {
                link_target[len] = '\0';
                printf(" -> %s", link_target);
            }
        }

        printf("\n");
    }


    closedir(dir);
}
*/
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


    return EXIT_SUCCESS;
}