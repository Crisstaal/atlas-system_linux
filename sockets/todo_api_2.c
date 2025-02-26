#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 4096

/**
 * handle_client - Handles an incoming HTTP request
 * @client_fd: File descriptor for the connected client
 */
void handle_client(int client_fd)
{
    char buffer[BUFFER_SIZE];
    ssize_t bytes_received;
    char *line, *key, *value;

    bytes_received = recv(client_fd, buffer, BUFFER_SIZE - 1, 0);
    if (bytes_received <= 0)
    {
        perror("recv");
        close(client_fd);
        return;
    }
    buffer[bytes_received] = '\0';

    printf("Raw request:\n\"%s\"\n", buffer);

    /* Extract and print headers */
    line = strtok(buffer, "\r\n");
    while (line != NULL)
    {
        if (strchr(line, ':'))
        {
            key = strtok(line, ": ");
            value = strtok(NULL, "");
            if (key && value)
                printf("Header: \"%s\" -> \"%s\"\n", key, value);
        }
        line = strtok(NULL, "\r\n");
    }

    /* Send HTTP 200 OK response */
    send(client_fd, "HTTP/1.1 200 OK\r\n\r\n", 19, 0);
    close(client_fd);
}

/**
 * main - Entry point, creates a TCP server that listens on port 8080
 *
 * Return: 0 on success, -1 on failure
 */
int main(void)
{
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len;
    char client_ip[INET_ADDRSTRLEN];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1)
    {
        perror("socket");
        return (EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("bind");
        close(server_fd);
        return (EXIT_FAILURE);
    }

    if (listen(server_fd, 10) == -1)
    {
        perror("listen");
        close(server_fd);
        return (EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", PORT);

    while (1)
    {
        client_len = sizeof(client_addr);
        client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
        if (client_fd == -1)
        {
            perror("accept");
            continue;
        }

        inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
        printf("Client connected: %s\n", client_ip);

        handle_client(client_fd);
    }

    close(server_fd);
    return (EXIT_SUCCESS);
}
