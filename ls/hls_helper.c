#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include "hls.h"

void free_file_list(File *head) {
    File *temp;
    while (head) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

int compare_names(const void *a, const void *b) {
    File *fa = *(File **)a;
    File *fb = *(File **)b;
    return strcmp(fa->name, fb->name);
}

void print_file_details(File *file) {
    struct stat st = file->st;
    char mode[11] = {0};
    mode[0] = S_ISDIR(st.st_mode) ? 'd' : '-';
    mode[1] = (st.st_mode & S_IRUSR) ? 'r' : '-';
    mode[2] = (st.st_mode & S_IWUSR) ? 'w' : '-';
    mode[3] = (st.st_mode & S_IXUSR) ? 'x' : '-';
    mode[4] = (st.st_mode & S_IRGRP) ? 'r' : '-';
    mode[5] = (st.st_mode & S_IWGRP) ? 'w' : '-';
    mode[6] = (st.st_mode & S_IXGRP) ? 'x' : '-';
    mode[7] = (st.st_mode & S_IROTH) ? 'r' : '-';
    mode[8] = (st.st_mode & S_IWOTH) ? 'w' : '-';
    mode[9] = (st.st_mode & S_IXOTH) ? 'x' : '-';

    char timebuf[64];
    struct tm *tm_info = localtime(&st.st_mtime);
    strftime(timebuf, sizeof(timebuf), "%b %d %H:%M", tm_info);

    struct passwd *pw = getpwuid(st.st_uid);
    struct group *gr = getgrgid(st.st_gid);

    printf("%s %lu %s %s %5lld %s %s\n", mode, st.st_nlink, pw->pw_name, gr->gr_name, (long long)st.st_size, timebuf, file->name);
}