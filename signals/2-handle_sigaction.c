#include <signal.h>
#include <stdio.h>

/**
 * sigint_handler - Prints message when SIGINT is caught
 * @signum: The signal number
 */
void sigint_handler(int signum)
{
    printf("Gotcha! [%d]\n", signum);
}

/**
 * handle_sigaction - Sets a handler for SIGINT using sigaction
 *
 * Return: 0 on success, -1 on error
 */
int handle_sigaction(void)
{
    struct sigaction sa;

    /* Set up the structure to specify the new action */
    sa.sa_handler = sigint_handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);

    /* Set the SIGINT handler */
    if (sigaction(SIGINT, &sa, NULL) == -1)
    {
        return -1;
    }
    return 0;
}
