#ifndef HLS_H
#define HLS_H

#include <dirent.h>

void print_error(const char *msg);
void list_dir(const char *path, int include_hidden, int list_long);
int is_hidden(const char *name);
void print_file_info(const char *filename);

#endif /* HLS_H */
