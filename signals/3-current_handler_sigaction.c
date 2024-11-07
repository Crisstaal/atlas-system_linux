#include <signal.h>

/**
 * current_handler_sigaction - Retrieves the current handler of SIGINT
 *
 * Return: Pointer to the current handler of SIGINT, or NULL on failure
 */
void (*current_handler_sigaction(void))(int)
{
    struct sigaction sa;

    /* Retrieve the current action for SIGINT */
    if (sigaction(SIGINT, NULL, &sa) == -1)
    {
        return NULL;
    }

    /* Return the current handler for SIGINT */
    return sa.sa_handler;
}
