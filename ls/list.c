#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include "hls.h"

void list_directory(const char *dir, int op_l, int op_A) {
    struct dirent *d;
    DIR *dh = opendir(dir);
    int i;
    File *head = NULL, *tail = NULL;
    int file_count = 0;
    char path[512];
    File *f = head;
    File *new_file;
    size_t j;
    
    if (!dh) {
        fprintf(stderr, "hls: cannot open directory '%s': %s\n", dir, strerror(errno));
        exit(EXIT_FAILURE);
    }


    while ((d = readdir(dh)) != NULL) {
        if ((!op_A && d->d_name[0] == '.') || (op_A && (strcmp(d->d_name, ".") == 0 || strcmp(d->d_name, "..") == 0))) continue;

        new_file = malloc(sizeof(File));
        if (new_file == NULL) {
            perror("malloc");
            exit(EXIT_FAILURE);
        }

        for (j = 0; d->d_name[j] != '\0' && j < sizeof(new_file->name) - 1; ++j) {
            new_file->name[j] = d->d_name[j];
        }
        new_file->name[j] = '\0';

        size_t len = strlen(dir);
        strncpy(path, dir, sizeof(path) - 1);
        path[len] = '/';
        strncpy(path + len + 1, d->d_name, sizeof(path) - len - 1);

        if (stat(path, &new_file->st) != 0) {
            perror("stat");
            exit(EXIT_FAILURE);
        }

        new_file->next = NULL;

        if (!head) head = new_file;
        else tail->next = new_file;
        
        tail = new_file;
    }
    closedir(dh);

    while (f != NULL) {
        file_count++;
        f = f->next;
    }

    File **file_array = malloc(file_count * sizeof(File *));
    if (file_array == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    
    f = head;
    for (i = 0; i < file_count; i++) {
        file_array[i] = f;
        f = f->next;
    }

    qsort(file_array, file_count, sizeof(File *), compare_names);

    for (i = 0; i < file_count; i++) {
        if (op_l) {
            print_file_details(file_array[i]);
        } else {
            printf("%s\n", file_array[i]->name);
        }
    }

    free(file_array);
    free_file_list(head);
}
