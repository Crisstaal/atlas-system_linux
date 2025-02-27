#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 4096
#define RESPONSE "HTTP/1.1 200 OK\r\n\r\n"

void handle_client(int client_fd, struct sockaddr_in client_addr)
{
	char buffer[BUFFER_SIZE];
	ssize_t bytes_received;

	/* Receive HTTP request */
	bytes_received = recv(client_fd, buffer, BUFFER_SIZE - 1, 0);
	if (bytes_received <= 0)
	{
		perror("recv");
		close(client_fd);
		return;
	}

	buffer[bytes_received] = '\0'; /* Null-terminate */

	/* Print client IP */
	printf("Client connected: %s\n", inet_ntoa(client_addr.sin_addr));

	/* Print raw HTTP request */
	printf("Raw request: \"%s\"\n", buffer);

	/* Extract and print the first line (method, path, version) */
	char method[16], path[256], version[16];
	if (sscanf(buffer, "%15s %255s %15s", method, path, version) == 3)
	{
		printf("Method: %s\n", method);
		printf("Path: %s\n", path);
		printf("Version: %s\n", version);
	}

	/* Send HTTP 200 OK response */
	send(client_fd, RESPONSE, strlen(RESPONSE), 0);

	/* Close connection */
	close(client_fd);
}

int main(void)
{
	int server_fd, client_fd;
	struct sockaddr_in server_addr, client_addr;
	socklen_t client_len = sizeof(client_addr);

	/* Create socket */
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd == -1)
	{
		perror("socket");
		exit(EXIT_FAILURE);
	}

	/* Configure server */
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);
	server_addr.sin_addr.s_addr = INADDR_ANY;

	/* Bind */
	if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
	{
		perror("bind");
		close(server_fd);
		exit(EXIT_FAILURE);
	}

	/* Listen */
	if (listen(server_fd, 5) == -1)
	{
		perror("listen");
		close(server_fd);
		exit(EXIT_FAILURE);
	}

	printf("Server listening on port %d\n", PORT);

	/* Accept clients in a loop */
	while (1)
	{
		client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
		if (client_fd == -1)
		{
			perror("accept");
			continue;
		}
		handle_client(client_fd, client_addr);
	}

	/* Close server socket */
	close(server_fd);
	return 0;
}