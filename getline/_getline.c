#include "_getline.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

static char *static_buffer = NULL;
static size_t static_buffer_len = 0;

char *_getline(const int fd) {
    /* Declare variables with initial values */
    char buffer[READ_SIZE];
    ssize_t bytes_read = 0;
    char *newline_pos = NULL;

    /* Loop until a newline is found or no more data is available */
    while ((newline_pos = strchr(static_buffer, '\n')) == NULL) {
        bytes_read = read(fd, buffer, READ_SIZE);
        
        if (bytes_read <= 0) {  /* EOF or error */
            if (static_buffer_len == 0) {
                /* No more lines to return */
                free(static_buffer);
                static_buffer = NULL;
                static_buffer_len = 0;
                return NULL;
            } else {
                /* Return the remaining data without newline */
                char *result = malloc(static_buffer_len + 1);
                if (!result) {
                    return NULL;
                }
                memcpy(result, static_buffer, static_buffer_len);
                result[static_buffer_len] = '\0';
                free(static_buffer);
                static_buffer = NULL;
                static_buffer_len = 0;
                return result;
            }
        }

        /* Before reallocating, declare new_buffer */
        char *new_buffer = realloc(static_buffer, static_buffer_len + bytes_read);
        if (!new_buffer) {
            free(static_buffer);
            static_buffer = NULL;
            static_buffer_len = 0;
            return NULL;
        }
        static_buffer = new_buffer;

        /* Append new data to static_buffer */
        memcpy(static_buffer + static_buffer_len, buffer, bytes_read);
        static_buffer_len += bytes_read;
    }

    /* Calculate line_len and declare result before using it */
    size_t line_len = newline_pos - static_buffer;
    char *result = malloc(line_len + 1);
    if (!result) {
        return NULL;
    }
    memcpy(result, static_buffer, line_len);
    result[line_len] = '\0';

    /* Adjust static_buffer after extracting the line */
    size_t remaining_len = static_buffer_len - line_len - 1;
    if (remaining_len > 0) {
        memmove(static_buffer, newline_pos + 1, remaining_len);
    } else {
        free(static_buffer);
        static_buffer = NULL;
    }
    static_buffer_len = remaining_len;

    return result;
}