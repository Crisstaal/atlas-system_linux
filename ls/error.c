#include <stdio.h>
#include "hls.h"
#include <stdint.h>
#include <stdlib.h>

void *_memset(void *c, int a, size_t b)
{
    size_t i;
    char *ptr = c;
    for (i = 0; i < b; i++)
    {
        *ptr++ = (char)a;
    }

    return c;
}
/**
* print_error- print error
* return: error
*/
void print_error(const char *msg)
{
    fprintf(stderr,"hls: %s\n", msg);
     exit(EXIT_FAILURE);
}

/**
 * _calloc - allocates memory
 * @nmemb: integer
 * @size: number of integers for each element
 * Return: pointer or null
 */
void *_calloc(unsigned int nmemb, unsigned int size)
{
	
    if (nmemb != 0 && size > __SIZE_MAX__ / nmemb) {
        return NULL;
    }

    size_t bytes = nmemb * size;
    if (bytes == 0) {
        return NULL;
    }

    void *ptr = malloc(bytes);
    if (!ptr) {
        return NULL;
    }

    return _memset(ptr, 0, bytes);
}

int my_strcmp(const char *str1, const char *str2)
{
    while (*str1 && *str2 && (*str1 == *str2)) {
        ++str1;
        ++str2;
    }
    return *str1 - *str2;
}

/**
* parse_args - parse args
*@option_t - options
*@input_args - put in args
*/
void parse_args(char **input_args, char **output_args, option_t *options)
{
    size_t input_index = 0, output_index = 0;

    while (input_args[input_index] != NULL)
    {
        char *arg = input_args[input_index];
        printf("parse_args:\n\targ = %s\n", arg);

        if (arg[0] == '-' && my_strcmp(arg, "-") != 0)
        {
            parse_opts(arg, options);
        }
        else
        {
            output_args[output_index] = arg;
            ++output_index;
        }
        ++input_index;
    }
    output_args[output_index] = NULL;
}
/**
 * parse_opts - parses opts
 * @arg: containing flag string
 * @options: options
 */
void parse_opts(char *arg, option_t *options)
{
    while (*arg)
    {
        int found = 0;
        
        for (size_t i = 0; i < sizeof(OptionInfo) / sizeof(optionInfo[0]); ++i)
        {
            if (*arg == optionInfo[i].flag)
            {
                *options |= optionInfo[i].option;
                d_printf(optionInfo[i].debug_message, *arg);
                found = 1;
                break;
            }
        }
        
        if (!found)
        {
            dbg_printf("TODO: handle bad flag\n");
            fprintf(stderr, "Unknown flag\n");
            exit(2);
        }
        
        ++arg;
    }
}