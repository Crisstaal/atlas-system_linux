#include <pthread.h>
#include <stdio.h>
#include <stdarg.h>

/**
 * tprintf - Prints a formatted string prefixed with the thread ID
 * @format: The format string
 *
 * Return: The number of characters printed, or a negative value on failure
 */
int tprintf(char const *format, ...) {
    va_list args;
    int printed_chars = 0;

    /* Get the calling thread's ID */
    pthread_t thread_id = pthread_self();

    /* Print the thread ID */
    printf("[%lu] ", (unsigned long)thread_id);

    /* Process the formatted string */
    va_start(args, format);
    printed_chars = vprintf(format, args);
    va_end(args);

    return printed_chars;
}