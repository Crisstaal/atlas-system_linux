#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

/**
 * main - Prints the description of a given signal
 * @argc: Argument count
 * @argv: Argument vector
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <signum>\n", argv[0]);
        return (EXIT_FAILURE);
    }
    printf("%s: %s\n", argv[1], strsignal(atoi(argv[1])));
    return (EXIT_SUCCESS);
}