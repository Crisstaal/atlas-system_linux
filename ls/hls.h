#ifndef _HLS_H_
#define _HLS_H_


#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>


#define NONE		(0)
#define ONEPERLINE	(1)
#define ALL		(1 << 1)
#define ALMOSTALL	(1 << 2)
#define HUMAN		(1 << 3)
#define LISTING		(1 << 4)


#define BUFSIZE 1024
#define SLICE(str, a, b) (str[b] = '\0', &str[a])
typedef unsigned char option_t;

typedef struct file_t
{
	char *path;
	struct stat statbuf;
	int is_dir;
	struct file_t **subentries;
}file_t;

struct file_node_t
{
	char filename[256];

	size_t prev;
	size_t next;
};

void list_dir(const char *path, int include_hidden, int list_long);
void dbg_printf(const char *format, ...);
void parse_args(char **argv, char **args, option_t *options);
void separate_files(char **, file_t **, file_t **, size_t *, size_t *);
void print_files_in_current_dir(file_t **, size_t, option_t);
void read_subentries(DIR *, file_t *, size_t *);
void print_subentries(file_t *, size_t, size_t, option_t);
void print_files_in_directory(file_t **, size_t, option_t);

/* error.c */
void dbg_print_binary(unsigned char r);
void dbg_print_array(file_t **array, size_t size);
void dbg_swap(file_t **a, file_t **b, file_t **array, size_t size);
void _quicksort(file_t **array, size_t size);
void _alphasort(file_t **, int);
void sort_subentries(file_t **, int);

/*hls.c*/
int create_fullpath(char **, const char *, char *);
int hls(const char *[]);
size_t _strlen(const char *);
int _strcmp(char *, char *);
int _strcoll(char *, char *);
void *_memset(void *p, int b, size_t n);
void *_calloc(unsigned int num, unsigned int size);
int partition(file_t **array, int low, int high);
void sort_subentries(file_t **directory, int d_count);


#endif
