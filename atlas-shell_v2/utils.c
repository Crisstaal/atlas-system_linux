#include "hsh.h"

/**
 * print_prompt - Print the shell prompt.
 */
void print_prompt() {
    printf("($) ");
    fflush(stdout);
}

/**
 * read_command - Read a command from standard input.
 * 
 * Return: A pointer to the read command string.
 */
char *read_command() {
    char *line = NULL;
    size_t len = 0;
    if (getline(&line, &len, stdin) == -1) {
        if (feof(stdin)) {
            free(line);
            exit(0);
        }
        perror("getline");
        free(line);
        exit(EXIT_FAILURE);
    }
    return line;
}

/**
 * parse_command - Parse a command string into arguments.
 * @line: The command string to parse.
 * 
 * Return: An array of argument strings.
 */
char **parse_command(char *line) {
    char **tokens = malloc(64 * sizeof(char*));
    char *token;
    int position = 0;

    if (!tokens) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, " \t\r\n\a");
    while (token != NULL) {
        tokens[position++] = token;
        token = strtok(NULL, " \t\r\n\a");
    }
    tokens[position] = NULL;
    return tokens;
}
