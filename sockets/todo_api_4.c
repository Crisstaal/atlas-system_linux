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

/**
 * send_response - Sends an HTTP response
 * @client_fd: The client file descriptor
 * @status: HTTP status code
 * @message: HTTP status message
 * @body: Response body
 */
void send_response(int client_fd, int status, char *message, char *body);

/**
 * handle_request - Handles incoming requests
 * @client_fd: The client file descriptor
 */
void handle_request(int client_fd);

/**
 * parse_request - Parses the HTTP request
 * @buffer: The request buffer
 * @method: Buffer for HTTP method
 * @path: Buffer for request path
 * @body: Buffer for request body
 * Return: 1 if successful, 0 otherwise
 */
int parse_request(char *buffer, char *method, char *path, char *body);

/**
 * handle_post_todos - Handles POST /todos requests
 * @client_fd: The client file descriptor
 * @body: The request body
 */
void handle_post_todos(int client_fd, char *body);

/**
 * handle_get_todos - Handles GET /todos requests
 * @client_fd: The client file descriptor
 */
void handle_get_todos(int client_fd);

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
		handle_request(client_fd);
	}

	close(server_fd);
	return (0);
}

/**
 * handle_request - Handles incoming requests
 * @client_fd: The client file descriptor
 */
void handle_request(int client_fd)
{
	char buffer[BUFFER_SIZE], method[8], path[256], body[BUFFER_SIZE];
	ssize_t bytes_received;

	bytes_received = recv(client_fd, buffer, BUFFER_SIZE - 1, 0);
	if (bytes_received <= 0)
		return;

	buffer[bytes_received] = '\0';
	if (!parse_request(buffer, method, path, body))
	{
		send_response(client_fd, 400, "Bad Request", "");
		close(client_fd);
		return;
	}

	if (strcmp(method, "POST") == 0 && strcmp(path, "/todos") == 0)
		handle_post_todos(client_fd, body);
	else if (strcmp(method, "GET") == 0 && strcmp(path, "/todos") == 0)
		handle_get_todos(client_fd);
	else
		send_response(client_fd, 404, "Not Found", "");

	close(client_fd);
}

/**
 * parse_request - Parses the HTTP request
 * @buffer: The request buffer
 * @method: Buffer for HTTP method
 * @path: Buffer for request path
 * @body: Buffer for request body
 * Return: 1 if successful, 0 otherwise
 */
int parse_request(char *buffer, char *method, char *path, char *body)
{
	char *line = strtok(buffer, "\r\n");
	int content_length = 0;

	if (!line)
		return 0;

	sscanf(line, "%s %s", method, path);
	while ((line = strtok(NULL, "\r\n")))
	{
		if (strncmp(line, "Content-Length:", 15) == 0)
		{
			content_length = atoi(line + 16);
		}
		if (*line == '\0')
		{
			if (content_length > 0)
			{
				recv(client_fd, body, content_length, 0);
				body[content_length] = '\0'; // Null-terminate the body
			}
			else
			{
				body[0] = '\0'; // No body
			}
			return 1;
		}
	}

	return 0; // Missing Content-Length
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
	{
		send_response(client_fd, 507, "Insufficient Storage", "");
		return;
	}

	todos[todo_count].id = todo_count;
	strncpy(todos[todo_count].title, title, sizeof(todos[todo_count].title) - 1);
	todos[todo_count].title[sizeof(todos[todo_count].title) - 1] = '\0'; // Null-terminate
	strncpy(todos[todo_count].description, desc, sizeof(todos[todo_count].description) - 1);
	todos[todo_count].description[sizeof(todos[todo_count].description) - 1] = '\0'; // Null-terminate
	todo_count++;

	char response_body[512];
	snprintf(response_body, sizeof(response_body),
		 "{\"id\":%d,\"title\":\"%s\",\"description\":\"%s\"}",
		 todos[todo_count - 1].id, todos[todo_count - 1].title,
		 todos[todo_count - 1].description);
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
		char todo_entry[512];
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
	int body_length = strlen(body);
	snprintf(response, sizeof(response),
		 "HTTP/1.1 %d %s\r\n"
		 "Content-Length: %d\r\n"
		 "Content-Type: application/json\r\n\r\n"
		 "%s",
		 status, message, body_length, body);
	send(client_fd, response, strlen(response), 0);
}
