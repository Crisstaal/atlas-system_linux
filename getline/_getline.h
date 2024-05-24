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


void dbg_printf(const char *format, ...);

/**
 * _fgetchar - Reads a character from a file descriptor.
 * @fd: File descriptor
 * Return: Character read or EOF on error or end-of-file.
 */
ssize_t _fgetchar(const int fd);
/**
 * _getline - Reads a line from a file descriptor.
 * @fd: File descriptor to read from.
 * Return: Pointer to a buffer containing the read line, or NULL on error or end-of-file.
 */
char *_getline(const int fd);


#endif
