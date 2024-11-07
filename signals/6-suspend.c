#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

/**
 * sigint_handler - Handles SIGINT signal and exits
 * @signum: The signal number
 */
void sigint_handler(int signum)
{
    printf("Caught %d\n", signum);
    printf("Signal received\n");
    _exit(EXIT_SUCCESS);  /* Use _exit to terminate the program */
}

/**
 * main - Sets a handler for SIGINT and waits for it
 *
 * Return: EXIT_SUCCESS
 */
int main(void)
{
    struct sigaction sa;

    /* Set up the sigaction structure */
    sa.sa_handler = sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    /* Set the handler for SIGINT */
    if (sigaction(SIGINT, &sa, NULL) == -1)
    {
        perror("sigaction");
        return (EXIT_FAILURE);
    }

    /* Suspend indefinitely until a signal is received */
    pause();

    return (EXIT_SUCCESS);
}
