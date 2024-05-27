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
            printf("%s\n", entry->d_name);
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
    DIR *dir = opendir(path);
    
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
    if (dir) {
        closedir(dir);
        list_dir(path, include_hidden, list_long);
    } else {
        fprintf(stderr, "./hls_01: cannot access %s: %s\n", path, strerror(errno));
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}

