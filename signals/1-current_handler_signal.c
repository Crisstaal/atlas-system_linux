#include <signal.h>
#include <stddef.h>
/**
 * current_handler_signal - Retrieves the current handler of SIGINT
 *
 * Return: Pointer to the current handler of SIGINT, or NULL on failure
 */
void (*current_handler_signal(void))(int)
{
    void (*current_handler)(int);

    current_handler = signal(SIGINT, SIG_IGN);
    
    /* Restore the original handler */
    if (signal(SIGINT, current_handler) == SIG_ERR)
    {
        return NULL;
    }

    return current_handler;
}