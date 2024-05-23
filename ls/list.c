#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<dirent.h>

void ls(const char *dir){
    struct dirent *d;
    dir *dirp =    opendir();
    if (!dir) {
        perror("Unable to read directory");
        exit(EXIT_FAILURE);
    }
    while((d = readdir(dir)) !=NULL) {
        if (d->d_name[0] == '.') continue;
        printf("%s\n", d->d_name);
    }
    closedir(dirp);
}