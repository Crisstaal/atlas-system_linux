#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SYSCALL_ERROR -1
#define PORT 12345
#define BACKLOG 8

/**
 * initialize_server - Initializes the server address struct.
 * @server_addr: Pointer to the server address struct.
 */
void initialize_server(struct sockaddr_in *server_addr)
{
    server_addr->sin_family = AF_INET;
    server_addr->sin_port = htons(PORT);
    server_addr->sin_addr.s_addr = htonl(INADDR_ANY);
}

/**
 * create_socket - Creates a socket.
 * Return: File descriptor for the socket.
 */
int create_socket(void)
{
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd == SYSCALL_ERROR)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    return sock_fd;
}

/**
 * bind_socket - Binds the socket to the server address.
 * @sock_fd: File descriptor of the socket.
 * @server_addr: Pointer to the server address struct.
 */
void bind_socket(int sock_fd, struct sockaddr_in *server_addr)
{
    if (bind(sock_fd, (struct sockaddr *)server_addr, sizeof(*server_addr))
        == SYSCALL_ERROR)
    {
        perror("bind");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }
}

/**
 * listen_socket - Puts the socket in listening mode.
 * @sock_fd: File descriptor of the socket.
 */
void listen_socket(int sock_fd)
{
    if (listen(sock_fd, BACKLOG) == SYSCALL_ERROR)
    {
        perror("listen");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }
}

/**
 * main - Entry point for the program.
 * Return: 0 on success.
 */
int main(void)
{
    int sock_fd;
    struct sockaddr_in server_addr;

    initialize_server(&server_addr);
    sock_fd = create_socket();
    bind_socket(sock_fd, &server_addr);
    listen_socket(sock_fd);

    printf("Server listening on port %d\n", PORT);

    /* Hang indefinitely */
    while (1)
    {
        pause(); /* Wait for signals */
    }

    /* Close the socket (though this won't be reached) */
    close(sock_fd);
    return (0);
}
