#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>

#include "signals.h"

static int sigint_count = 0;

/**
 * signal_handler - Prints message when SIGINT is caught
 * @signum: The signal number
 */
void signal_handler(int signum)
{
    printf("Gotcha! [%d]\n", signum);
    fflush(stdout);

     sigint_count++;

    /* After the third signal, print the "Quit" message and terminate the program*/
    if (sigint_count == 3)
    {
        printf("Quit (core dumped)\n");
        exit(0);  
    }
}

/**
 * handle_signal - Sets a handler for SIGINT
 *
 * Return: 0 on success, -1 on error
 */
int handle_signal(void)
{
    if (signal(SIGINT, signal_handler) == SIG_ERR)
    {
        return -1;
    }
    return 0;
}