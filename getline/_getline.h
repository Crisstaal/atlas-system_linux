#ifndef _GETLINE_H_
#define _GETLINE_H_

#include <string.h>
#include <stddef.h>
#include <stdlib.h>

#define OPEN_MAX 255
#define READ_SIZE 1024
#define LINEBUF_SIZE 1024

#define BLUE "\033[34m"
#define RESET "\033[0m"
#define RED "\033[31m"

typedef struct {
    int been_opened;
    char buf[READ_SIZE];
    char *bufp;
    ssize_t n;
} filebuf_t;

static filebuf_t fds[OPEN_MAX];
void dbg_printf(const char *format, ...);

ssize_t _fgetchar(const int fd);
char *_getline(const int fd);


#endif
