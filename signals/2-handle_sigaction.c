#include <signal.h>
#include <stdio.h>

/**
 * sigint_handler - Prints message when SIGINT is caught
 * @signum: The signal number
 */
void sigint_handler(int signum)
{
    printf("Gotcha! [%d]\n", signum);
    fflush(stdout);
}

/**
 * handle_sigaction - Sets a handler for SIGINT using sigaction
 *
 * Return: 0 on success, -1 on error
 */
int handle_sigaction(void)
{
    struct sigaction sa;

    /* Set up the structure*/
    sa.sa_handler = sigint_handler;
    

    
    return (sigaction(SIGINT, &sa, NULL))
}
