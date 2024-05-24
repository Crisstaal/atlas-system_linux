#ifndef _GETLINE_H_
#define _GETLINE_H_

#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define READ_SIZE 10284

ssize_t _fgetchar(const int fd);
char *_getline(const int fd);

#endif
