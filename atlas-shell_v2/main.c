#include "hsh.h"

void loop() {
    char *line;
    char **args;
    int status;

    do {
        if (isatty(STDIN_FILENO)) {
            print_prompt();
        }
        line = read_command();
        args = parse_command(line);
        status = execute_command(args);

        free(line);
        free(args);
    } while (status);
}

/**
 * main - Entry point of the shell program.
 * @argc: Argument count.
 * @argv: Argument vector.
 * 
 * Return: Always returns EXIT_SUCCESS.
 */
int main(int argc, char **argv) {
    loop();
    return EXIT_SUCCESS;
}
