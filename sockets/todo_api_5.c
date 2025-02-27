#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 8080
#define BUFFER_SIZE 4096
#define MAX_TODOS 100

typedef struct todo
{
    int id;
    char title[256];
    char description[512];
} todo_t;

todo_t todos[MAX_TODOS];
int todo_count = 0;

void send_response(int client_fd, int status, const char *status_msg,
                   const char *body)
{
    char response[BUFFER_SIZE];
    int content_length = strlen(body);

    snprintf(response, sizeof(response),
             "HTTP/1.1 %d %s\r\n"
             "Content-Length: %d\r\n"
             "Content-Type: application/json\r\n\r\n"
             "%s",
             status, status_msg, content_length, body);
    send(client_fd, response, strlen(response), 0);
}

void handle_get_todos(int client_fd)
{
    char body[BUFFER_SIZE] = "[";
    for (int i = 0; i < todo_count; i++)
    {
        char todo_json[BUFFER_SIZE];
        snprintf(todo_json, sizeof(todo_json),
                 "{\"id\":%d,\"title\":\"%s\",\"description\":\"%s\"}%s",
                 todos[i].id, todos[i].title, todos[i].description,
                 (i < todo_count - 1) ? "," : "");
        strcat(body, todo_json);
    }
    strcat(body, "]");
    send_response(client_fd, 200, "OK", body);
}

void handle_post_todos(int client_fd, char *body)
{
    if (!strstr(body, "title=") || !strstr(body, "description="))
    {
        send_response(client_fd, 422, "Unprocessable Entity", "");
        return;
    }
    char *title = strstr(body, "title=") + 6;
    char *desc = strstr(body, "description=") + 12;
    char *title_end = strchr(title, '&');
    if (title_end)
        *title_end = '\0';
    if (todo_count < MAX_TODOS)
    {
        todos[todo_count].id = todo_count;
        strncpy(todos[todo_count].title, title, sizeof(todos[todo_count].title));
        strncpy(todos[todo_count].description, desc, sizeof(todos[todo_count].description));
        todo_count++;
        char response_body[BUFFER_SIZE];
        snprintf(response_body, sizeof(response_body),
                 "{\"id\":%d,\"title\":\"%s\",\"description\":\"%s\"}",
                 todo_count - 1, title, desc);
        send_response(client_fd, 201, "Created", response_body);
    }
}

void handle_client(int client_fd, char *request)
{
    char method[8], path[256], body[BUFFER_SIZE] = "";
    sscanf(request, "%s %s", method, path);
    char *body_start = strstr(request, "\r\n\r\n");
    if (body_start)
        strncpy(body, body_start + 4, sizeof(body) - 1);
    if (strcmp(method, "GET") == 0 && strcmp(path, "/todos") == 0)
        handle_get_todos(client_fd);
    else if (strcmp(method, "POST") == 0 && strcmp(path, "/todos") == 0)
        handle_post_todos(client_fd, body);
    else
        send_response(client_fd, 404, "Not Found", "");
}

int main(void)
{
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE], client_ip[INET_ADDRSTRLEN];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
    bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(server_fd, 5);
    printf("Server listening on port %d\n", PORT);

    while (1)
    {
        client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len);
        inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
        recv(client_fd, buffer, BUFFER_SIZE - 1, 0);
        handle_client(client_fd, buffer);
        close(client_fd);
    }
    close(server_fd);
    return (0);
}