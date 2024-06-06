#include "hsh.h"

/**
 * handle_builtin - Handle built-in shell commands.
 * @args: An array of argument strings.
 * 
 * Return: 1 if the command is a built-in and was handled, 0 otherwise.
 */
int handle_builtin(char **args) {
    if (strcmp(args[0], "exit") == 0) {
        if (args[1]) {
            exit(atoi(args[1]));
        } else {
            exit(0);
        }
    }

    if (strcmp(args[0], "cd") == 0) {
        if (args[1] == NULL || strcmp(args[1], "~") == 0) {
            args[1] = getenv("HOME");
        } else if (strcmp(args[1], "-") == 0) {
            args[1] = getenv("OLDPWD");
        }
        if (chdir(args[1]) != 0) {
            perror("hsh");
        } else {
            char cwd[PATH_MAX];
            setenv("OLDPWD", getenv("PWD"), 1);
            setenv("PWD", getcwd(cwd, sizeof(cwd)), 1);
        }
        return 1;
    }

    if (strcmp(args[0], "env") == 0) {
        for (char **env = environ; *env != 0; env++) {
            printf("%s\n", *env);
        }
        return 1;
    }

    return 0;
}

/**
 * execute_command - Execute a command.
 * @args: An array of argument strings.
 * 
 * Return: 1 if the shell should continue running, 0 if it should terminate.
 */
int execute_command(char **args) {
    pid_t pid, wpid;
    int status;

    if (args[0] == NULL) {
        return 1;
    }

    if (handle_builtin(args)) {
        return 1;
    }

    pid = fork();
    if (pid == 0) {
        /*Child process*/
        if (execvp(args[0], args) == -1) {
            perror("hsh");
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        perror("hsh");
    } else {
        /*the Parent process*/
        do {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}
