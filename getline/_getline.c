#include "_getline.h"
#include <stdio.h>
#include <string.h>


static char *static_buffer = NULL;
static size_t static_buffer_len = 0;

char *_getline(const int fd) {
    /* Declare all variables at the top */
    char buffer[READ_SIZE];
    ssize_t bytes_read;
    char *newline_pos = NULL;
    size_t line_len;
    size_t remaining_len;

    while ((newline_pos = strchr(static_buffer, '\n')) == NULL) {
        /* Read from the file descriptor into buffer */
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

        /* Extend the static buffer to accommodate new data */
        char *new_buffer = realloc(static_buffer, static_buffer_len + bytes_read);
        if (!new_buffer) {
            free(static_buffer);
            static_buffer = NULL;
            static_buffer_len = 0;
            return NULL;
        }
        static_buffer = new_buffer;

        /* Append the new data to the static buffer */
        memcpy(static_buffer + static_buffer_len, buffer, bytes_read);
        static_buffer_len += bytes_read;
    }

    /* Extract the line up to the newline character */
    line_len = newline_pos - static_buffer;
    char *result = malloc(line_len + 1);
    if (!result) {
        return NULL;
    }
    memcpy(result, static_buffer, line_len);
    result[line_len] = '\0';

    /* Update the static buffer to exclude the extracted line */
    remaining_len = static_buffer_len - line_len - 1;
    if (remaining_len > 0) {
        memmove(static_buffer, newline_pos + 1, remaining_len);
    } else {
        free(static_buffer);
        static_buffer = NULL;
    }
    static_buffer_len = remaining_len;

    return result;
}