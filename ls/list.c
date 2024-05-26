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
    int file_count = 0;  char path[512];
    File *new_file = malloc(sizeof(File));
    File *f;
    File **file_array;
    File *temp = head;
    
    if (!dh) {
        fprintf(stderr, "hls: cannot open directory '%s': %s\n", dir, strerror(errno));
        exit(EXIT_FAILURE);
    }

    while ((d = readdir(dh)) != NULL) {
        if ((!op_A && d->d_name[0] == '.') || (op_A && (strcmp(d->d_name, ".") == 0 || strcmp(d->d_name, "..") == 0))) continue;
        
        strncpy(new_file->name, d->d_name);
        printf(path, sizeof(path), "%s/%s", dir, d->d_name);
        stat(path, &new_file->st);
        new_file->next = NULL;

        if (!head) head = new_file;
        else tail->next = new_file;
        tail = new_file;
    }
    closedir(dh);



    for (f= head; f != NULL; f = f->next) file_count++;

    file_array = malloc(file_count * sizeof(File *));
    for (i = 0; i < file_count; i++) {
        file_array[i] = temp;
        temp = temp->next;
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