#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

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

    int signum = atoi(argv[1]);
    const char *description = ssignal(signum);

    if (description)
        printf("%d: %s\n", signum, description);
    else
        printf("%d: Unknown signal %d\n", signum, signum);

    return (EXIT_SUCCESS);
}
