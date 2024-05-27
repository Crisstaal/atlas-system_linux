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
int main(int argc, char **argv)
{
	char *def[] = {".", NULL};
	size_t i, file_count = 0, dir_count = 0;
	option_t options = NONE;
	char **args = _calloc(BUFSIZE, sizeof(args));
	file_t **files = malloc(sizeof(**files) * BUFSIZE);
	file_t **dirs = malloc(sizeof(**dirs) * BUFSIZE);

	/* options to be used*/
	(void) argc;
	parse_args(argv + 1, args, &options);
	if (!args[0])
		parse_args(def, args, &options);
	if (!args[0])
		perror("fail"), exit(2);

	separate_files(args, files, directory, &file_count, &d_count);

	_alphasort(files, file_count);

    /*printing*/
	dbg_printf("options = %d = ", options);
	dbg_print_binary(options);
	dbg_printf("file count = %lu\n", file_count);
	print_files_in_current_dir(files, file_count, options);

	_alphasort(directory, d_count);
	if (d_count > 1)
		puts("");
	print_files_in_directory(directory, d_count, options);

	for (i = 0; i < file_count; ++i)
		free(files[i]);
	for (i = 0; i < d_count; ++i)
		free(directory[i]);
	free(files);
	free(directory);
	free(args);

	return (0);
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