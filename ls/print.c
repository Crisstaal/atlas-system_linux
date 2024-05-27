#include "hls.h"
#include <stdio.h>
/**
 * print_swap - swaps two values in file
 * @a: first value
 * @b: second value
 * @array: pointer
 * @size:size
 * @stream: FILE stream to print the array
 */
void print_swap(file_t **a, file_t **b, file_t **array, size_t size, FILE *stream)
{
    file_t *tmp = *a;

    if (*a != *b)
    {
        *a = *b;
        *b = tmp;
        print_array(array, size, stream);
    }
}

/**
 * print_array - prints pathnames in an array of file
 * @array: array
 * @size: number of elements
 */

void print_array(file_t **array, size_t size, FILE *stream)
{
    if (array == NULL) {
        fprintf(stream, "Array is NULL\n");
        return;
    }

    for (size_t i = 0; i < size; ++i) {
        if (i > 0)
            fprintf(stream, ", ");
        if (array[i] != NULL)
            fprintf(stream, "%s");
        else
            fprintf(stream, "NULL");
    }
    fprintf(stream, "\n");
}

/**
 * prints_binary - prints
 * @r: unsigned char
 */
void prints_binary(unsigned char r)
{
    int i;
    for (i = 7; i >= 0; --i)
    {
        putchar((r & (1 << i)) ? '1' : '0');
    }
    putchar('\n');
}

