#include "_getline.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Assuming this is part of a larger code file */

char *_getline(const int fd) {
    /* Declare all variables at the beginning */
    static char *static_buffer = NULL;
    static size_t static_buffer_len = 0;
    char *newline_pos = NULL;
    size_t line_len = 0;
    size_t remaining_len = 0;

    /* Check for valid parameters */
    if (!stream || !line || !len) {
        return -1;
    }
    
    if (static_buffer == NULL) {
        static_buffer_len = 128; /* Example buffer size */
        static_buffer = malloc(static_buffer_len);
    } else {
        char *new_buffer = realloc(static_buffer, static_buffer_len + bytes_read); /* Using declared variable */
        if (new_buffer == NULL) {
            free(static_buffer);
            static_buffer = NULL;
            return -1;
        }
        static_buffer = new_buffer;
        static_buffer_len += bytes_read;
    }

    /* Find the newline position in the static buffer */
    newline_pos = strchr(static_buffer, '\n'); 

    if (newline_pos) {
        line_len = newline_pos - static_buffer; /* Using declared variable */
        *line = malloc(line_len + 1); /* Allocate memory for the line */
        strncpy(*line, static_buffer, line_len);
        (*line)[line_len] = '\0';

        remaining_len = static_buffer_len - line_len - 1; /* Using declared variable */
        memmove(static_buffer, newline_pos + 1, remaining_len);
        static_buffer_len = remaining_len;

        return line_len;
    }

    return -1;
}