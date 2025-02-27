#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 4096

/**
 * parse_http_request - Extracts path and body from the request.
 * @request: Full HTTP request string.
 * @path: Buffer to store extracted path.
 * @body: Buffer to store extracted body.
 */
void parse_http_request(char *request, char *path, char *body)
{
	char *line = strtok(request, "\r\n");

	if (line && strncmp(line, "POST ", 5) == 0)
		sscanf(line, "POST %s HTTP/1.1", path);

	while ((line = strtok(NULL, "\r\n")) != NULL)
	{
		if (strcmp(line, "") == 0)
		{
			strcpy(body, strtok(NULL, ""));
			break;
		}
	}
}

/**
 * parse_body_params - Extracts and prints key-value pairs from body.
 * @body: The body string containing parameters.
 */
void parse_body_params(char *body)
{
	char *param = strtok(body, "&"), *key, *value;

	while (param)
	{
		key = strtok(param, "=");
		value = strtok(NULL, "=");
		if (key && value)
			printf("Body param: \"%s\" -> \"%s\"\n", key, value);
		param = strtok(NULL, "&");
	}
}

/**
 * handle_client - Processes client request and sends response.
 * @client_fd: The file descriptor for the client socket.
 * @client_ip: The IP address of the connected client.
 */
void handle_client(int client_fd, char *client_ip)
{
	char buffer[BUFFER_SIZE] = {0}, path[256] = {0}, body[BUFFER_SIZE] = {0};
	char response[] = "HTTP/1.1 200 OK\r\n\r\n";

	printf("Client connected: %s\n", client_ip);
	recv(client_fd, buffer, BUFFER_SIZE - 1, 0);
	printf("Raw request: \"%s\"\n", buffer);

	parse_http_request(buffer, path, body);
	printf("Path: %s\n", path);
	parse_body_params(body);

	send(client_fd, response, strlen(response), 0);
	close(client_fd);
}

/**
 * main - Creates a TCP server that listens for HTTP requests.
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
		handle_client(client_fd, client_ip);
	}

	close(server_fd);
	return (0);
}