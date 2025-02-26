#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 4096

typedef struct todo
{
	int id;
	char title[256];
	char description[1024];
} todo_t;

todo_t todos[100];
int todo_count = 0;

/**
 * send_response - Sends an HTTP response to the client.
 * @client_fd: Client socket.
 * @status: HTTP status code and message.
 * @body: JSON body of the response.
 */
void send_response(int client_fd, const char *status, const char *body)
{
	char response[BUFFER_SIZE];

	sprintf(response,
		"HTTP/1.1 %s\r\n"
		"Content-Length: %ld\r\n"
		"Content-Type: application/json\r\n\r\n"
		"%s",
		status, body ? strlen(body) : 0, body ? body : "");

	send(client_fd, response, strlen(response), 0);
}

/**
 * parse_http_request - Parses HTTP request to extract method, path, and body.
 * @request: The raw HTTP request.
 * @method: Buffer to store the extracted method.
 * @path: Buffer to store the extracted path.
 * @body: Buffer to store the extracted body.
 * @content_length: Pointer to store Content-Length value.
 */
void parse_http_request(char *request, char *method, char *path,
			char *body, int *content_length)
{
	char *line = strtok(request, "\r\n");

	if (line)
		sscanf(line, "%s %s HTTP/1.1", method, path);

	while ((line = strtok(NULL, "\r\n")) != NULL)
	{
		if (strncmp(line, "Content-Length:", 15) == 0)
			*content_length = atoi(line + 15);
		else if (strcmp(line, "") == 0)
			strcpy(body, strtok(NULL, ""));
	}
}

/**
 * parse_body_params - Extracts title and description from body.
 * @body: The request body.
 * @title: Buffer to store title.
 * @description: Buffer to store description.
 * Return: 1 if both params are found, 0 otherwise.
 */
int parse_body_params(char *body, char *title, char *description)
{
	char *param = strtok(body, "&"), *key, *value;
	int found_title = 0, found_desc = 0;

	while (param)
	{
		key = strtok(param, "=");
		value = strtok(NULL, "=");
		if (key && value)
		{
			if (strcmp(key, "title") == 0)
			{
				strcpy(title, value);
				found_title = 1;
			}
			else if (strcmp(key, "description") == 0)
			{
				strcpy(description, value);
				found_desc = 1;
			}
		}
		param = strtok(NULL, "&");
	}
	return (found_title && found_desc);
}

/**
 * handle_post_todo - Handles POST requests to /todos.
 * @client_fd: Client socket.
 * @body: Request body.
 * @content_length: Content-Length value.
 */
void handle_post_todo(int client_fd, char *body, int content_length)
{
	char title[256] = {0}, description[1024] = {0}, response_body[BUFFER_SIZE];

	if (content_length <= 0)
		return (send_response(client_fd, "411 Length Required", NULL));

	if (!parse_body_params(body, title, description))
		return (send_response(client_fd, "422 Unprocessable Entity", NULL));

	todos[todo_count].id = todo_count;
	strcpy(todos[todo_count].title, title);
	strcpy(todos[todo_count].description, description);
	todo_count++;

	sprintf(response_body, "{\"id\":%d,\"title\":\"%s\",\"description\":\"%s\"}",
		todo_count - 1, title, description);
	send_response(client_fd, "201 Created", response_body);
}

/**
 * handle_client - Processes client request and sends appropriate response.
 * @client_fd: Client socket.
 */
void handle_client(int client_fd)
{
	char buffer[BUFFER_SIZE] = {0}, method[8] = {0}, path[256] = {0};
	char body[BUFFER_SIZE] = {0};
	int content_length = 0;

	recv(client_fd, buffer, BUFFER_SIZE - 1, 0);
	parse_http_request(buffer, method, path, body, &content_length);

	if (strcmp(method, "POST") == 0 && strcmp(path, "/todos") == 0)
		handle_post_todo(client_fd, body, content_length);
	else
		send_response(client_fd, "404 Not Found", NULL);

	close(client_fd);
}

/**
 * main - Creates a TCP server that handles REST API requests.
 * Return: 0 on success, 1 on failure.
 */
int main(void)
{
	int server_fd, client_fd;
	struct sockaddr_in address;
	socklen_t addr_len = sizeof(address);
	char client_ip[INET_ADDRSTRLEN];

	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd == -1)
		return (perror("socket"), 1);

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
		return (perror("bind"), 1);

	if (listen(server_fd, 10) < 0)
		return (perror("listen"), 1);

	printf("Server listening on port %d\n", PORT);

	while (1)
	{
		client_fd = accept(server_fd, (struct sockaddr *)&address, &addr_len);
		if (client_fd < 0)
		{
			perror("accept");
			continue;
		}
		inet_ntop(AF_INET, &address.sin_addr, client_ip, INET_ADDRSTRLEN);
		printf("%s %s %s\n", client_ip, "POST", "/todos");
		handle_client(client_fd);
	}

	close(server_fd);
	return (0);
}