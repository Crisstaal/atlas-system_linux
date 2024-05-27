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

int main(int argc, char **argv) {
    option_t options = NONE;
    int j;
    size_t i, file_count = 0, d_count = 0;
     file_t **files = malloc(sizeof(**files) * BUFSIZE);
    file_t **directory = malloc(sizeof(**directory) * BUFSIZE);
    char **args = malloc(argc + 1) *(sizeof(char *));
    
    if (files == NULL || directory == NULL || args == NULL) {
        perror("Memory allocation error");
        free(files);
        free(directory);
        free(args);
        return EXIT_FAILURE;

    }

    if (argc < 2) {
        printf("Usage: %s <directory>\n", argv[0]);
        free(files);
        free(directory);
        free(args);

        return EXIT_FAILURE;
    }

    /*Copy command-line arguments to args*/
    for (j = 0; j < argc; j++) {
        args[j] = argv[j];
    }


    separate_files(args, files, directory, &file_count, &d_count);


    /* Printing */
    printf("options = %d\n", options);
    printf("file count = %lu\n", file_count);

    print_files_in_current_dir(files, file_count, options);

    
    if (d_count > 0) {
        puts("");
    }
    print_files_in_directory(directory, d_count, options);

    for (i = 0; i < file_count; ++i) {
        free(files[i]);
    }
    for (i = 0; i < d_count; ++i) {
        free(directory[i]);
    }
    free(files);
    free(directory);
    free(args);

    return EXIT_SUCCESS;
}


/**
 * _calloc - allocates memory
 * @num: integer
 * @size: integer
 * Return: null
 */
 /**
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

*/
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