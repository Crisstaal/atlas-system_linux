#include <stdio.h>
#include <pthread.h>
#include <stdarg.h>

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

/***
 * tprintf - Prints a formatted string with the calling thread ID
 *
 * @format: The format string for the printf family
 *
 * Return: The number of characters printed (excluding the null byte)
 ***/
int tprintf(char const *format, ...)
{
    va_list args;
    int ret;
    pthread_t thread_id;

    /*** Lock the mutex to avoid race conditions ***/
    pthread_mutex_lock(&mutex);

    /*** Get the calling thread's ID ***/
    thread_id = pthread_self();

    /*** Print the thread ID and the formatted string ***/
    printf("[%lu] ", (unsigned long)thread_id);

    /*** Handle the formatted output ***/
    va_start(args, format);
    ret = vprintf(format, args);
    va_end(args);

    /*** Unlock the mutex ***/
    pthread_mutex_unlock(&mutex);

    return ret;
}