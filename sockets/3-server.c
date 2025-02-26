#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 12345
#define BUFFER_SIZE 1024

int main(void)
{
	int server_fd, client_fd;
	struct sockaddr_in server_addr, client_addr;
	socklen_t client_len = sizeof(client_addr);
	char buffer[BUFFER_SIZE];

	/* Create the socket */
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd == -1)
	{
		perror("socket");
		exit(EXIT_FAILURE);
	}

	/* Configure server address */
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);
	server_addr.sin_addr.s_addr = INADDR_ANY;

	/* Bind the socket */
	if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
	{
		perror("bind");
		close(server_fd);
		exit(EXIT_FAILURE);
	}

	/* Listen for connections */
	if (listen(server_fd, 1) == -1)
	{
		perror("listen");
		close(server_fd);
		exit(EXIT_FAILURE);
	}

	printf("Server listening on port %d\n", PORT);

	/* Accept a client connection */
	client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
	if (client_fd == -1)
	{
		perror("accept");
		close(server_fd);
		exit(EXIT_FAILURE);
	}

	/* Print client IP address */
	printf("Client connected: %s\n", inet_ntoa(client_addr.sin_addr));

	/* Receive message */
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

	/* Close the connection */
	close(client_fd);
	close(server_fd);

	return 0;
}