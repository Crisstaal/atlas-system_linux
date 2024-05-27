#ifndef HLS_H
#define HLS_H

#include <sys/stat.h>

typedef struct File {
    char name[256];
    struct stat st;
    struct File *next;
} File;

void free_file_list(File *head);
void print_file_details(File *file);
void list_directory(const char *dir, int op_l, int op_A);
int my_strcmp(const char *s1, const char *s2);

#endif 
