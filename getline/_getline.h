#ifndef _GETLINE_H_
#define _GETLINE_H_

#include <unistd.h>
#include <stdlib.h>

#define READ_SIZE 128

char *_getline(const int fd);

#endif
