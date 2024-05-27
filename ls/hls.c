#include <stdio.h>
#include "hls.h"
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#define BUFSIZE 1024

int main(int argc, char *argv[]) {
    size_t i, file_count = 0, dir_count = 0;
    char *def[] = {".", NULL};
    Opt_opt = NONE;
    char **args = ((char **)calloc(BUFSIZE, sizeof(char *)));
    File **files = (File **)malloc(sizeof(File *) * BUFSIZE);
    File **dir = (File **)malloc(sizeof(File *) * BUFSIZE);

    if (args == NULL || files == NULL || dir == NULL) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }

    parse_args(argv + 1, args, &opt);
    if (!args[0])
        parse_args(def, args, &opt);
    if (!args[0]) {
        fprintf(stderr, "Usage: %s [directory]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    separate_files(args, files, dir, &file_count, &dir_count);

    _alphasort(files, file_count);

    printf("Opt = %d\n", opt);
    printf("File count = %lu\n", file_count);

    print_files_in_current_dir(files, file_count, opt);

    if (dir_count > 1)
        putchar('\n');

    print_files_in_dirs(dir, dir_count, opt);

    for (i = 0; i < file_count; ++i) {
        free(files[i]->name);
        free(files[i]);
    }
    for (i = 0; i < dir_count; ++i) {
        free(dir[i]->name);
        free(dir[i]);
    }

    free(files);
    free(dir);
    free(args);

    return 0;
}