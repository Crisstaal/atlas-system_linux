#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 8080
#define BUFFER_SIZE 4096
#define MAX_TODOS 100

/**
 * struct todo - Represents a todo item
 * @id: Unique identifier
 * @title: Title of the todo
 * @description: Description of the todo
 */
typedef struct todo
{
    int id;
    char title[256];
    char description[512];
} todo_t;

todo_t todos[MAX_TODOS];
int todo_count = 0;

void send_response(int client_fd, int status, char *message, char *body);
void handle_post_todos(int client_fd, char *body);
void handle_get_todos(int client_fd);

/**
 * parse_request - Parses the HTTP request
 * @buffer: The request buffer
 * @method: Buffer for HTTP method
 * @path: Buffer for request path
 * @body: Buffer for request body
 */
void parse_request(char *buffer, char *method, char *path, char *body)
{
    char *line = strtok(buffer, "\r\n");
    char *content_length;
    *body = '\0';

    if (!line)
        return;

    sscanf(line, "%s %s", method, path);
    while ((line = strtok(NULL, "\r\n")))
    {
        if (strncmp(line, "Content-Length:", 15) == 0)
        {
            content_length = line + 16;
        }
        if (*line == '\0')
        {
            strcpy(body, strtok(NULL, "\r\n"));
            break;
        }
    }
}

/**
 * handle_client - Handles client requests
 * @client_fd: The client file descriptor
 */
void handle_client(int client_fd)
{
    char buffer[BUFFER_SIZE], method[8], path[256], body[BUFFER_SIZE];
    ssize_t bytes_received;

    bytes_received = recv(client_fd, buffer, BUFFER_SIZE - 1, 0);
    if (bytes_received <= 0)
        return;

    buffer[bytes_received] = '\0';
    parse_request(buffer, method, path, body);

    if (strcmp(method, "POST") == 0 && strcmp(path, "/todos") == 0)
        handle_post_todos(client_fd, body);
    else if (strcmp(method, "GET") == 0 && strcmp(path, "/todos") == 0)
        handle_get_todos(client_fd);
    else
        send_response(client_fd, 404, "Not Found", "");

    close(client_fd);
}

/**
 * handle_post_todos - Handles POST /todos requests
 * @client_fd: The client file descriptor
 * @body: The request body
 */
void handle_post_todos(int client_fd, char *body)
{
    char *title = strstr(body, "title=");
    char *desc = strstr(body, "description=");

    if (!title || !desc)
    {
        send_response(client_fd, 422, "Unprocessable Entity", "");
        return;
    }

    title += 6;
    desc += 12;
    if (todo_count >= MAX_TODOS)
        return;

    todos[todo_count].id = todo_count;
    strcpy(todos[todo_count].title, title);
    strcpy(todos[todo_count].description, desc);
    todo_count++;

    char response_body[512];
    snprintf(response_body, sizeof(response_body),
             "{\"id\":%d,\"title\":\"%s\",\"description\":\"%s\"}",
             todos[todo_count - 1].id, title, desc);
    send_response(client_fd, 201, "Created", response_body);
}

/**
 * handle_get_todos - Handles GET /todos requests
 * @client_fd: The client file descriptor
 */
void handle_get_todos(int client_fd)
{
    char response_body[BUFFER_SIZE] = "[";
    for (int i = 0; i < todo_count; i++)
    {
        char todo_entry[256];
        snprintf(todo_entry, sizeof(todo_entry),
                 "{\"id\":%d,\"title\":\"%s\",\"description\":\"%s\"}",
                 todos[i].id, todos[i].title, todos[i].description);
        strcat(response_body, todo_entry);
        if (i < todo_count - 1)
            strcat(response_body, ",");
    }
    strcat(response_body, "]");
    send_response(client_fd, 200, "OK", response_body);
}

/**
 * send_response - Sends an HTTP response
 * @client_fd: The client file descriptor
 * @status: HTTP status code
 * @message: HTTP status message
 * @body: Response body
 */
void send_response(int client_fd, int status, char *message, char *body)
{
    char response[BUFFER_SIZE];
    int content_length = strlen(body);
    snprintf(response, sizeof(response),
             "HTTP/1.1 %d %s\r\nContent-Length: %d\r\nContent-Type: application/json\r\n\r\n%s",
             status, message, content_length, body);
    send(client_fd, response, strlen(response), 0);
}

/**
 * main - Entry point of the HTTP server
 * Return: Always 0
 */
int main(void)
{
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1)
        return (EXIT_FAILURE);

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1 ||
        listen(server_fd, 5) == -1)
    {
        close(server_fd);
        return (EXIT_FAILURE);
    }
    printf("Server listening on port %d\n", PORT);

    while (1)
    {
        client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_fd == -1)
            continue;
        handle_client(client_fd);
    }

    close(server_fd);
    return (0);
}