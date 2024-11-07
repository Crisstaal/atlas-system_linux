#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

/**
 * main - Sends SIGINT to a process given its PID.
 * @argc: Number of command line arguments.
 * @argv: Array of command line arguments.
 *
 * Return: EXIT_SUCCESS on success, or EXIT_FAILURE on error.
 */
int main(int argc, char *argv[])
{
    pid_t pid;

    /* Check if the correct number of arguments is provided */
    if (argc != 2)
    {
        printf("Usage: %s <pid>\n", argv[0]);
        return (EXIT_FAILURE);
    }

    /* Convert the second argument (PID) to an integer */
    pid = atoi(argv[1]);

    /* Ensure the PID is not 0 */
    if (pid == 0)
    {
        printf("Usage: %s <pid>\n", argv[0]);
        return (EXIT_FAILURE);
    }

    /* Send SIGINT signal to the process with the given PID */
    if (kill(pid, SIGINT) == -1)
    {
        perror("Error sending signal");
        return (EXIT_FAILURE);
    }

    return (EXIT_SUCCESS);
}
