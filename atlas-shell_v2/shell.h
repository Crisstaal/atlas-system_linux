#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <limits.h>

extern char **environ;

/*Function declarations*/

/*main.c*/
void loop();

/*utils.c*/
void print_prompt();
char *read_command();
char **parse_command(char *line);

/*builtins.c*/
int handle_builtin(char **args);
int execute_command(char **args);

#endif
