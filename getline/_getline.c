#include <stdio.h>
#include "_getline.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>


char *_getline(int fd) {
    static char *static_buffer = NULL;
    static size_t buffer_capacity = 0;
    static size_t buffer_length = 0;
    size_t line_len = 0;
    size_t remaining_len = buffer_length - (line_len + 1);
    char *new_buffer = realloc(static_buffer, buffer_capacity);
    ssize_t bytes_read = read(fd, static_buffer + buffer_length, buffer_capacity - buffer_length);

    if (fd < 0) {
        return NULL;
    }

    /*Initialize the static buffer if it's NULL*/
    if (static_buffer == NULL) {
        buffer_capacity = 128;
        static_buffer = malloc(buffer_capacity);
        if (static_buffer == NULL) {
            return NULL;
        }
        memset(static_buffer, 0xFF, buffer_capacity);
    }

    while (1) {
        char *newline_pos = strchr(static_buffer, '\n');
        if (newline_pos) {
            size_t line_len = newline_pos - static_buffer;
            char *line = malloc(line_len + 1);
            if (line == NULL) {
                return NULL; 
            }
            
            strncpy(line, static_buffer, line_len); 
            line[line_len] = '\0';

            memmove(static_buffer, newline_pos + 1, remaining_len); 
            buffer_length = remaining_len;
            return line;
        }
        
   
        if (buffer_length >= buffer_capacity) {
            buffer_capacity *= 2;
            if (new_buffer == NULL) {
                return NULL;
            }
            static_buffer = new_buffer;
              memset(static_buffer + buffer_length, 0xFF, buffer_capacity - buffer_length);
        }

        if (bytes_read <= 0) {
            return NULL;
        }
        
        buffer_length += bytes_read;
    }
}