#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include "hls.h"

void list_directory(const char *dir, int op_l, int op_A) {
    struct dirent *d;
    DIR *dh = opendir(dir);
    int i, m, n;
    size_t j, len, dir_len = 0;
    File *head = NULL, *tail = NULL, *new_file;
    File *f = head;
    int file_count = 0;
    char path[512];
    path[0] ='\0';
    File **file_array = NULL;
    free_file_list(head);
    

    if (!dh) {
        fprintf(stderr, "hls: cannot open directory '%s': ", dir);
        
        exit(EXIT_FAILURE);
    }
    
    while (dir[dir_len] != '\0')
    {
        dir_len++;
    }

    if (dir_len + 1 >= sizeof(path)) {
        fprintf(stderr, "Path length exceeds buffer size\n");
        closedir(dh);
        free_file_list(head);
        exit(EXIT_FAILURE);
    }

    sprintf(path, "%s", dir);
    path[dir_len] = '/';
    path[dir_len + 1] = '\0';

    while ((d = readdir(dh)) != NULL) {
        if ((!op_A && d->d_name[0] == '.') || (op_A && (my_strcmp(d->d_name, ".") == 0 || my_strcmp(d->d_name, "..") == 0))) continue;

        new_file = malloc(sizeof(File));
        if (new_file == NULL) {
            perror("malloc");
            exit(EXIT_FAILURE);
        }

        for (j = 0; j < sizeof(new_file->name) - 1; ++j) {
            if (d->d_name[j] == '\0') {
                new_file->name[j] = '\0';
                break;
            }
            new_file->name[j] = d->d_name[j];
        }
        new_file->name[j] = '\0';

        len = strlen(d->d_name);
        while (d->d_name[len] != '\0')
        {
            len++;
        }
        if (dir_len + 1 + len >= sizeof(path)) {
            fprintf(stderr, "Path length exceeds buffer size\n");
            closedir(dh);
            free_file_list(head);
            exit(EXIT_FAILURE);
        }
        sprintf(path, "%s%s", dir, d->d_name);

        if (lstat(path, &new_file->st) != 0) {
            perror("lstat");
            exit(EXIT_FAILURE);
        }

        new_file->next = NULL;

        if (!head) head = new_file;
        else tail->next = new_file;
        
        tail = new_file;
        file_count++;
    }
    closedir(dh);

    file_array = malloc(file_count * sizeof(File *));
    if (file_array == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    
    f = head;
    i = 0;
    while (f != NULL) {
        file_array[i++] = f;
        f = f->next;
    }

    for (m = 1; m < file_count; ++m) {
        File *key = file_array[m];
        n = m - 1;
        while (n >= 0 && my_strcmp(file_array[n]->name, key->name) > 0) {
            file_array[n + 1] = file_array[n];
            n = n - 1;
        }
        file_array[n + 1] = key;
    }

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
    