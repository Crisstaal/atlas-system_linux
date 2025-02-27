#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 8080
#define BUFFER_SIZE 4096

/**
 * parse_http_request - Extracts the path and query from an HTTP request
 * @request: The HTTP request string
 * @path: Buffer to store the extracted path
 * @query: Buffer to store the extracted query parameters
 */
void parse_http_request(char *request, char *path, char *query)
{
    char *method, *url, *version, *query_start, *pair, *key, *value;

    method = strtok(request, " ");
    url = strtok(NULL, " ");
    version = strtok(NULL, "\r\n");

    if (!method || !url || !version)
        return;

    query_start = strchr(url, '?');
    if (query_start)
    {
        *query_start = '\0';
        strcpy(query, query_start + 1);
    }
    else
        query[0] = '\0';

    strcpy(path, url);
    printf("Path: %s\n", path);

    if (strlen(query) > 0)
    {
        pair = strtok(query, "&");
        while (pair)
        {
            key = strtok(pair, "=");
            value = strtok(NULL, "=");
            if (key && value)
                printf("Query: \"%s\" -> \"%s\"\n", key, value);
            pair = strtok(NULL, "&");
        }
    }
}

/**
 * handle_client - Processes an incoming client request
 * @client_fd: The file descriptor of the connected client
 * @client_ip: The client's IP address
 */
void handle_client(int client_fd, char *client_ip)
{
    char buffer[BUFFER_SIZE], path[256], query[512];
    ssize_t bytes_received;
    const char *response = "HTTP/1.1 200 OK\r\n\r\n";

    printf("Client connected: %s\n", client_ip);

    bytes_received = recv(client_fd, buffer, BUFFER_SIZE - 1, 0);
    if (bytes_received <= 0)
    {
        perror("recv");
        close(client_fd);
        return;
    }

    buffer[bytes_received] = '\0';
    printf("Raw request: \"%s\"\n", buffer);

    parse_http_request(buffer, path, query);
    send(client_fd, response, strlen(response), 0);
    close(client_fd);
}

/**
 * setup_server - Creates, binds, and listens on a server socket
 * Return: The server socket file descriptor on success, -1 on failure
 */
int setup_server(void)
{
    int server_fd;
    struct sockaddr_in server_addr;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1)
    {
        perror("socket");
        return (-1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("bind");
        close(server_fd);
        return (-1);
    }

    if (listen(server_fd, 5) == -1)
    {
        perror("listen");
        close(server_fd);
        return (-1);
    }

    return (server_fd);
}

/**
 * main - Entry point of the HTTP server
 * Return: Always 0
 */
int main(void)
{
    int server_fd, client_fd;
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    char client_ip[INET_ADDRSTRLEN];

    server_fd = setup_server();
    if (server_fd == -1)
        return (EXIT_FAILURE);

    printf("Server listening on port %d\n", PORT);

    while (1)
    {
        client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_fd == -1)
        {
            perror("accept");
            continue;
        }

        inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
        handle_client(client_fd, client_ip);
    }

    close(server_fd);
    return (0);
}