#ifndef SIGNALS_H
#define SIGNALS_H

#include <sys/types.h>  /* For pid_t */
#include <signal.h>     /* For signal handling */

/* Function to set a handler for the SIGINT signal */
int handle_signal(void);

/* Function to retrieve the current handler for the SIGINT signal */
void (*current_handler_signal(void))(int);

/* Function to set a handler for the SIGINT signal using sigaction */
int handle_sigaction(void);

/* Function to retrieve the current handler for SIGINT using sigaction */
void (*current_handler_sigaction(void))(int);

/* Function to define a handler for SIGQUIT */
int trace_signal_sender(void);

/* Function to describe the given signal number */
void describe_signal(int signum);

/* Function to check if a process with the given PID exists */
int pid_exist(pid_t pid);

#endif /* SIGNALS_H */
