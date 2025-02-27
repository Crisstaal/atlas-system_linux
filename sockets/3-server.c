#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 12345
#define BUFFER_SIZE 1024

/**
 * create_socket - Creates a socket for the server.
 *
 * @server_fd: Pointer to an integer where the socket file descriptor
 *              will be stored.
 *
 */
void create_socket(int *server_fd)
{
    *server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (*server_fd == -1)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }
}

/**
 * configure_server - Configures the server address structure.
 *
 * @server_addr: Pointer to a sockaddr_in structure that will be
 *                configured with the server's address.
 */
void configure_server(struct sockaddr_in *server_addr)
{
    server_addr->sin_family = AF_INET;
    server_addr->sin_port = htons(PORT);
    server_addr->sin_addr.s_addr = INADDR_ANY;
}

/**
 * bind_socket - Binds the socket to the server address.
 *
 * @server_fd: The socket file descriptor to be bound.
 * @server_addr: Pointer to the sockaddr_in structure containing the
 *                server's address.
 */
void bind_socket(int server_fd, struct sockaddr_in *server_addr)
{
    if (bind(server_fd, (struct sockaddr *)server_addr,
             sizeof(*server_addr)) == -1)
    {
        perror("bind");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
}

/**
 * handle_client - Handles communication with a connected client.
 *
 * @client_fd: The socket file descriptor for the connected client.
 */
void handle_client(int client_fd)
{
    char buffer[BUFFER_SIZE];
    ssize_t bytes_received = recv(client_fd, buffer, BUFFER_SIZE - 1, 0);

    if (bytes_received > 0)
    {
        buffer[bytes_received] = '\0'; /* Null-terminate the string */
        printf("Message received: \"%s\"\n", buffer);
    }
    else
    {
        perror("recv");
    }
}

/**
 * main - Entry point of the server program.
 *
 * Return: Always 0 on success.
 */
int main(void)
{
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    create_socket(&server_fd);
    configure_server(&server_addr);
    bind_socket(server_fd, &server_addr);

    if (listen(server_fd, 1) == -1)
    {
        perror("listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", PORT);

    client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
    if (client_fd == -1)
    {
        perror("accept");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    /* Print client IP address */
    printf("Client connected: %s\n", inet_ntoa(client_addr.sin_addr));

    /* Handle client message */
    handle_client(client_fd);

    /* Close the connection */
    close(client_fd);
    close(server_fd);

    return (0);
}
