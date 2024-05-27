#ifndef HLS_H
#define HLS_H

#include <dirent.h>


typedef enum {
    NONE = 0,
    OPTION_A = 1,
    OPTION_L = 2,
    OPTION_R = 4
} Options;

typedef struct {
    char *name;
    struct stat st;
} File;

/* Function prototypes */
void parse_args(char **argv, char **args, Opt *opt);
void separate_files(char **args, File **files, File **dir, size_t *file_count, size_t *dir_count);
void _alphasort(File **files, size_t count);
void print_files_in_current_dir(File **files, size_t count, Opt opt);
void print_files_in_dir(File **dir, size_t count, Opt opt);

#endif
