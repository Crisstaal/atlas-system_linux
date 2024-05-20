#include <stdio.h>
#include "_getline.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>


char *_getline(int fd) {
    static char *buffer = NULL;
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
    if (buffer == NULL) {
       buffer = (char *)malloc(buffer_capacity);
        if (buffer == NULL) {
            return NULL;
        }
        buffer_length = 0;
    }

    while (1) {
        
        if (newline_pos) {
             size_t newline_pos = 0;
        while (newline_pos < buffer_length && buffer[newline_pos] != '\n') {
            newline_pos++;
        }

        if (newline_pos < buffer_length) {
            size_t line_len = newline_pos;
            char *line = (char *)malloc(line_len + 1);
            if (line == NULL) {
                return NULL;
            }

            /*Copy the line to the new memory*/
            memcpy(line, buffer, line_len);
            line[line_len] = '\0'; 

            memmove(buffer, newline_pos + 1, remaining_len); 
            buffer_length = remaining_len;
            return line;
        }
        
   
        if (buffer_length >= buffer_capacity) {
            buffer_capacity *= 2;
            if (new_buffer == NULL) {
                return NULL;
            }
            buffer = new_buffer;
              memset(buffer + buffer_length, 0xFF, buffer_capacity - buffer_length);
        }

        if (bytes_read <= 0) {
            return NULL;
        }
        
        buffer_length += bytes_read;
    }
}