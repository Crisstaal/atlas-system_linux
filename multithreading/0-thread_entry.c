#include "multithreading.h"
#include <pthread.h>
#include <stdio.h>

/**
 * thread_entry - Entry point for a new thread
 * @arg: Pointer to a string to be printed
 *
 * Return: NULL
 */
void *thread_entry(void *arg)
{
    if (arg == NULL)
        pthread_exit(NULL);

    printf("%s\n", (char *)arg);

    pthread_exit(NULL);
}