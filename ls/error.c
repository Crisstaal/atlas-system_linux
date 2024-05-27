#include <stdio.h>


/**
* print_error- print error
* return: error
*/
void print_error(const char *msg)
{
    fprintf(stderr,"hls: %s\n", msg);
}

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