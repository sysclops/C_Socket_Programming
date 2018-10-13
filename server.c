#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>

int main(void) {
	printf("Hello!\n");
	const char *response_data = "<!DOCTYPE html>"
		"<html>"
		"<head>"
		"<title>Hello, World! Site Title </title>"
		"</head>"
		"<body>"
		"<h1>Hello,World!</h1>"
		"</body>"
		"</html>"
		"HTTP/1.1 200 OK\r\n"
		"Connection: close\r\n"
		"Content-Type: text/html\r\n"
		"Content-Length: 137\r\n"
		"\r\n ";
	int server_socket;
	if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("Couldn't create the socket");
		return -1;
	}
	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(8002);
	if ((server_address.sin_addr.s_addr = INADDR_ANY) == INADDR_NONE) {
		perror("Couldn't allocate the server an address");
		return -1;
	}
	int enable = 1;
	setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
	while (bind(server_socket, (struct sockaddr *) &server_address, sizeof(server_address)) != 0){
		if (errno == EADDRINUSE) {
			fprintf(stderr, "Waiting for: %s\n", strerror(errno));
			sleep(1);
		} else {
			fprintf(stderr,"Error: %s\n", strerror(errno));
			return -1;
		}
	}
	listen(server_socket, 2);

	int client_socket;
	while (1) {
		client_socket = accept(server_socket, NULL, NULL);
		send(client_socket, response_data, sizeof(response_data), 0);
		close(client_socket);
	}
	close(server_socket);
	return 0;
}
