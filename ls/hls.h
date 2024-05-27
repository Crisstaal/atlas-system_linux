#ifndef HLS_H
#define HLS_H


#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

#else

#define dbg_printf(...)
#define dbg_print_binary(x)
#define dbg_print_file_array(x)
#define dbg_swap(x, y, s, t)
#define NONE		(0)
#define ONEPERLINE	(1)
#define ALL		(1 << 1)
#define ALMOSTALL	(1 << 2)
#define HUMAN		(1 << 3)
#define LISTING		(1 << 4)


#define BUFSIZE 1024
#define dbg_printf printf
#define SLICE(str, a, b) (str[b] = '\0', &str[a])
typedef unsigned char option_t;

typedef struct file_t
{
	char *path;
	struct stat statbuf;
	int is_dir;
	struct file_t **subentries;
} file_t;

struct file_node_t
{
	char filename[256];

	size_t prev;
	size_t next;
};

void separate_files(char **, file_t **, file_t **, size_t *, size_t *);
void print_files_in_current_dir(file_t **, size_t, option_t);
void read_subentries(DIR *, file_t *, size_t *);
void print_subentries(file_t *, size_t, size_t, option_t);
void print_files_in_directory(file_t **, size_t, option_t);
/* error.c */
void dbg_print_binary(unsigned char);
void dbg_print_file_array(file_t **, size_t);
void dbg_swap(file_t **, file_t **, file_t **, size_t);
void _quicksort(file_t **, size_t);
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

#else

#define dbg_printf(...)
#define dbg_print_binary(x)
#define dbg_print_file_array(x)
#define dbg_swap(x, y, s, t)
#define NONE		(0)
#define ONEPERLINE	(1)
#define ALL		(1 << 1)
#define ALMOSTALL	(1 << 2)
#define HUMAN		(1 << 3)
#define LISTING		(1 << 4)

#endif
