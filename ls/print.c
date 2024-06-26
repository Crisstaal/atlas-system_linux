#include "hls.h"
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>


/**
 * separate_files - it does this
 * @args: args
 *@file_count: counts files
 * @directory: directory
 * @file_count: counts
 * @d_count: counts
 */
void separate_files(char **args,file_t **files, file_t **directory,size_t *file_count, size_t *d_count)
{
	size_t i = 0;

	for (; args[i]; ++i)
	{
		file_t *f = malloc(sizeof(*f));

		if(f == NULL)
		{
			perror("malloc");
			exit(EXIT_FAILURE);
		}

		f->path = args[i];
		if (lstat(f->path, &f->statbuf) == -1)
		{
			fprintf(stderr, "%s: cannot access %s: %s\n", program_name, f-> path, strerror(errno));
			free(f);
			continue;
		}
		f->is_dir = S_ISDIR(f->statbuf.st_mode);
		if (!f->is_dir)
		{
			files[(*file_count)++] = f;
		}
		else
		{
			directory[(*d_count)++] = f;
		}
	}
}

/**
 * print_files_in_current_dir - it does this
 * @files: files
 * @file_count: counts
 * @options:
 */
void print_files_in_current_dir(file_t **files, size_t file_count, option_t options)
{
	size_t i = 0;

	for (; i < file_count; ++i)
	{
		file_t *f = files[i];
	

		if ((!(options & ALL) &&((_strcmp(f->path, ".") == 0) ||
		    ((_strcmp(f->path, "..") == 0)))))
		{
			continue;
		}
		printf("%s\n", f->path);
	}
	
}

void read_subentries(DIR *dirp, file_t *dom, size_t *sub_count)
{
	struct dirent *d;

	while ((d = readdir(dirp)) != NULL)
	{
		file_t *sub = malloc(sizeof(*sub));
		if (sub == NULL)
		{
			perror("malloc");
			exit(EXIT_FAILURE);
		}

		sub->path = strdup(d->d_name);
		if (sub->path == NULL)
		{
			perror("strdup");
			free(sub);
			exit(EXIT_FAILURE);
		}

		if (lstat(sub->path, &sub->statbuf) == -1)
		{
			fprintf(stderr, "%s: cannot access %s: %s\n", program_name, sub->path, strerror(errno));
			free(sub->path);
			free(sub);
			continue;
		}
		sub->is_dir = S_ISDIR(sub->statbuf.st_mode);
		dom->subentries[(*sub_count)++] = sub;
	}
}


/**
 * print_subentries - print
 * @options: options available
 * @sub_count: counts subs
 * @directory_count: count
 * @options: options available
 */
void print_subentries(file_t *dom,size_t sub_count,size_t d_count, option_t options)
{
	size_t i = 0;


	if (d_count > 1)
	{
		printf("%s:\n", dom->path);
	}

	for (; i < sub_count; ++i)
	{
		char *path = dom->subentries[i]->path;

		if ((!(options & ALL) &&((_strcmp(path, ".") == 0) ||((_strcmp(path, "..") == 0)))))
		{
			continue;
		}

		printf("%s\n", path);
		
	}
}
	


/**
 * print_files_in_directory - print
 * @directory: directory
 * @d_count: counts
 * @options: options
 */
void print_files_in_directory(file_t **directory,size_t count,option_t options)
{
	size_t i, j;

	for (i = 0; i< count; ++i)
	{
		file_t *dom = directory[i];
		size_t sub_count = 0;
		DIR *dir = opendir(dom->path);

		if (dir == NULL)
        {
            fprintf(stderr, "%s: cannot acess %s: %s\n", program_name, dom->path, strerror(errno));
            continue;
        }

		dom->subentries = malloc(sizeof(*dom->subentries) * BUFSIZE);

		if(dom->subentries == NULL)
		{
			perror("malloc");
			closedir(dir);
			exit(EXIT_FAILURE);
		}

		read_subentries(dir, dom, &sub_count);

		print_subentries(dom, sub_count, (size_t)count, options);

		for (j = 0; j < sub_count; ++j)
		{
			free(dom->subentries[j]->path);
			free(dom->subentries[j]);
		}
		free(dom->subentries);
		closedir(dir);
	}
}
