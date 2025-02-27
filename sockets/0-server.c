#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SYSCALL_ERROR -1

int main(void)
{
	int sock_fd, bind_result, listen_result;
	struct sockaddr_in server_addr = {
	    .sin_family = AF_INET,
	    .sin_port = htons(12345),
	    .sin_addr.s_addr = htonl(INADDR_ANY)};

	/* Request socket from the kernel */
	sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (sock_fd == SYSCALL_ERROR)
	{
		perror("socket");
		exit(EXIT_FAILURE);
	}

	/* Bind socket to server address 0.0.0.0:12345 */
	bind_result = bind(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
	if (bind_result == SYSCALL_ERROR)
	{
		perror("bind");
		close(sock_fd);
		exit(EXIT_FAILURE);
	}

	/* Transition socket to listening state */
	listen_result = listen(sock_fd, 8);
	if (listen_result == SYSCALL_ERROR)
	{
		perror("listen");
		close(sock_fd);
		exit(EXIT_FAILURE);
	}

	printf("Server listening on port 12345\n");

	/* Hang indefinitely */
	while (1)
	{
		pause(); /* Wait for signals */
	}

	/* Close the socket (though this won't be reached) */
	close(sock_fd);
	return (0);
}