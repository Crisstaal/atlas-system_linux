#include <signal.h>
#include <stdio.h>

/**
 * sigquit_handler - Prints the sender PID when SIGQUIT is received
 * @signum: Signal number (expected to be SIGQUIT)
 * @info: Signal information, including the sender's PID
 * @context: Unused, for compatibility
 */
void sigquit_handler(int signum, siginfo_t *info, void *context)
{
    (void)context;

    if (signum == SIGQUIT)
    {
        printf("SIGQUIT sent by %d\n", info->si_pid);
    }
}

/**
 * trace_signal_sender - Sets a handler for SIGQUIT
 *
 * Return: 0 on success, -1 on error
 */
int trace_signal_sender(void)
{
    struct sigaction sa;

    /* Set up the sigaction structure */
    sa.sa_sigaction = sigquit_handler;
    sa.sa_flags = SA_SIGINFO;  /* Use SA_SIGINFO to access siginfo_t */
    sigemptyset(&sa.sa_mask);

    /* Set the handler for SIGQUIT */
    if (sigaction(SIGQUIT, &sa, NULL) == -1)
    {
        return (-1);
    }

    return (0);
}