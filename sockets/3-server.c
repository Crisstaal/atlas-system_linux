#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 12345
#define BUFFER_SIZE 1024

void create_socket(int *server_fd)
{
    *server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (*server_fd == -1)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }
}

void configure_server(struct sockaddr_in *server_addr)
{
    server_addr->sin_family = AF_INET;
    server_addr->sin_port = htons(PORT);
    server_addr->sin_addr.s_addr = INADDR_ANY;
}

void bind_socket(int server_fd, struct sockaddr_in *server_addr)
{
    if (bind(server_fd, (struct sockaddr *)server_addr, sizeof(*server_addr)) == -1)
    {
        perror("bind");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
}

void listen_for_connections(int server_fd)
{
    if (listen(server_fd, 1) == -1)
    {
        perror("listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
}

void accept_client(int server_fd, int *client_fd, struct sockaddr_in *client_addr, socklen_t *client_len)
{
    *client_fd = accept(server_fd, (struct sockaddr *)client_addr, client_len);
    if (*client_fd == -1)
    {
        perror("accept");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
}

void receive_message(int client_fd)
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

int main(void)
{
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    create_socket(&server_fd);
    configure_server(&server_addr);
    bind_socket(server_fd, &server_addr);
    listen_for_connections(server_fd);

    printf("Server listening on port %d\n", PORT);

    accept_client(server_fd, &client_fd, &client_addr, &client_len);

    /* Print client IP address */
    printf("Client connected: %s\n", inet_ntoa(client_addr.sin_addr));

    /* Receive message */
    receive_message(client_fd);

    /* Close the connection */
    close(client_fd);
    close(server_fd);

    return (0);
}
