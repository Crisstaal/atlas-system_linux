#include <stdio.h>
#include "_getline.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

/* Assumes this is part of a larger code file */

char *_getline(int fd) {
    static char *static_buffer = NULL;
    static size_t static_buffer_len = 0;
    
    size_t line_len = 0;
    ssize_t bytes_read = 0;
    char *newline_pos = NULL;
    char *line = malloc(line_len + 1);
    bytes_read = read(fd, static_buffer + static_buffer_len, 128);
    size_t remaining_len = static_buffer_len - line_len - 1;


    if (static_buffer == NULL) {
        static_buffer_len = 128; /* Initial buffer size */
        static_buffer = malloc(static_buffer_len);
        if (static_buffer == NULL) {
            return NULL; /* Memory allocation failed */
        }
    }

    if (bytes_read <= 0) {
        return NULL; /* End of file or read error */
    }

    static_buffer_len += bytes_read;

    newline_pos = strchr(static_buffer, '\n');
    if (newline_pos) {
        line_len = newline_pos - static_buffer; /* Length of the line */
        if (line == NULL) {
            return NULL; /* Memory allocation failed */
        }

        /* Copy the line from static_buffer */
        strncpy(line, static_buffer, line_len);
        line[line_len] = '\0';
        memmove(static_buffer, newline_pos + 1, remaining_len);
        static_buffer_len = remaining_len;

        return line;
    }

    return NULL;
}