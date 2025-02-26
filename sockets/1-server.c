#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define PORT 12345
#define BACKLOG 5
#define SYSCALL_ERROR -1

int main(void)
{
    int sock_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char client_ip[INET_ADDRSTRLEN];

    /* Create socket */
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd == SYSCALL_ERROR)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    /* Configure server address struct */
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    /* Bind socket */
    if (bind(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == SYSCALL_ERROR)
    {
        perror("bind");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    /* Listen for incoming connections */
    if (listen(sock_fd, BACKLOG) == SYSCALL_ERROR)
    {
        perror("listen");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", PORT);

    /* Accept a client connection */
    client_fd = accept(sock_fd, (struct sockaddr *)&client_addr, &client_len);
    if (client_fd == SYSCALL_ERROR)
    {
        perror("accept");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    /* Get and print client's IP address */
    inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
    printf("Client connected: %s\n", client_ip);

    /* Close client and server sockets */
    close(client_fd);
    close(sock_fd);

    return (0);
}