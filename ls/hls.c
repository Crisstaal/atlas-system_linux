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
    int list_long = 0;
    int include_hidden = 0;
    char *path = ".";
    int i;
    DIR *dir = opendir(path);
    
   if (argc < 2) {
        list_dir(".", include_hidden);
    } else {
        for (i = 1; i < argc; i++) {
            if (argv[i][0] == '-') {
                if (argv[i][1] == 'a' && argv[i][2] == '\0') {
                    include_hidden = 1;
                } else {
                    fprintf(stderr, "./hls: invalid option -- '%s'\n", argv[i]);
                    exit(EXIT_FAILURE);
                }
            } else {
                    list_dir(argv[i], include_hidden);
                }
            }
        }

    return exit_status;
}

