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


/**
 * _calloc - allocates memory
 * @num: integer
 * @size: integer
 * Return: null
 */
void *_calloc(unsigned int num, unsigned int size)
{
	void *t;
	size_t bytes;

	bytes = num * size;
	if (!bytes)
		return (NULL);

	t = malloc(bytes);
	if (t)
		return (_memset(t, 0, bytes));

	return (NULL);
}


/**
 * _memset - memory allocation
 * @p: pointer
 * @b: integer
 * @n: count
 * Return: pointer to the memory area s.
 */
void *_memset(void *p, int b, size_t n)
{
	char *ptr = p;

	while (n--)
		*ptr++ = b;

	return (p);
}