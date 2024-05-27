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

void list_dir(const char *path, int include_hidden) {
    DIR *dir;
    struct dirent *entry;
    
    if (!(dir = opendir(path)))
    {
        fprintf(stderr, "./hls: cannot access %s: %s\n", path, strerror(errno));
        return;
    }

    printf("%s:\n", path);

    while ((entry = readdir(dir)) != NULL) {
        if (!include_hidden && entry->d_name[0] == '.')
        {
            continue;
        }
        printf("%s\n", entry->d_name);
    }
    printf("\n");

    closedir(dir);
}


/**
* main - main function
* @argc - argument count
* @argv - number or arguments
*/
int main(int argc, char *argv[]) {
    int include_hidden = 0;
    char *path = ".";
    int i, j;
    DIR *dir = opendir(path);
    
    if (argc >1) {
        i = 1;
        while (i < argc) {
            if (argv[i][0] == '-') {
                for (j = 1; argv[i][j] != '\0'; j++) {
                    switch (argv[i][j]) {
                        case '1':
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
    if (dir) {
        closedir(dir);
        list_dir(path, include_hidden);
    } else {
        fprintf(stderr, "./hls_01: cannot access %s: %s\n", path, strerror(errno));
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}