#ifndef HLS_H
#define HLS_H

#include <dirent.h>

typedef struct {
    int option1;
    char option2;
} option_t;


/*Struct to hold option data*/
typedef struct {
    char flag;
    option_t option;
    const char *debug_message;
} OptionInfo;



// Function prototypes
void print_error(const char *msg);
void *_memset(void *c, int a, size_t b);
void list_dir(const char *path, int include_hidden);
int is_hidden(const char *name);
void print_file_info(const char *filename);
int my_strcmp(const char *str1, const char *str2);
void parse_args(char **input_args, char **output_args, option_t *options);
void print_array(file_t **array, size_t size, FILE *stream);
void prints_binary(unsigned char r);
void print_swap(file_t **a, file_t **b, file_t **array, size_t size, FILE *stream);
int _strcoll(const char *s1, const char *s2, option_t option);
size_t _strlen(const char *src);

#endif
