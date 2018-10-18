#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

#define PORT_NUMBER (8002)

typedef enum {
	E_SUCCESS,
	E_CREATION,
	E_ALLOCATION,
	E_BADF,
	E_NOBUFS,
	E_ACCES,
	E_NFILE,
	E_ISCONN,
	E_ALREADY,
	E_CONNABORTED,
	E_MFILE,
	E_PROTO
} sock_error;


int main(void) {
	const char *response_data = "HTTP/1.1 200 OK\r\n"
		"Connection: close\r\n"
		"Content-Type: text/html\r\n"
		"Content-Length: 117\r\n"
		"\r\n"

		"<!DOCTYPE html>"
		"<html>"
		"<head>"
		"<title>Hello, World! Site Title </title>"
		"</head>"
		"<body>"
		"<h1>Hello,World!</h1>"
		"</body>"
		"</html>"
		"\r\n";

	int server_socket = -1, client_socket = -1, enable = 1;
	sock_error result;
	struct sockaddr_in server_address,client_address;
	socklen_t size;
	if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		if (errno == EACCES){
			perror("socket: Permission to create a socket of the specified type and/or protocol is denied.");
			result = E_ACCES;
			goto end;
		}

		else if (errno == ENOBUFS || errno == ENOMEM) {
			perror("socket: Insufficient memory is available. The socket cannot be created until sufficient resources are freed.");
			result = E_NOBUFS;
			goto end;
		}

		else if (errno == ENFILE) {
			perror("socket: The system limit on the total number of open files has been reached.");
			result = E_NFILE;
			goto end;
		}

	}

	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(PORT_NUMBER);
	server_address.sin_addr.s_addr = INADDR_ANY;

	if ((setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int))) < 0) {

		if (errno == EBADF){
			perror("setsockopt: The socket argument is not a valid file descriptor.");
			result = E_BADF;
			goto end;
		}

		else if (errno == ENOMEM || errno == ENOBUFS){
			perror("setsockopt: There was insufficient memory available for setsockopt to complete.");
			result = E_NOBUFS;
			goto end;
		}

		else if (errno == EISCONN){
			perror("setsockopt: The socket is already connected, and a specified option cannot be set while the socket is connected.");
			result = E_ISCONN;
			goto end;

		}
	}

	while (bind(server_socket, (struct sockaddr *) &server_address, sizeof(server_address)) != 0) {
		if (errno == EALREADY) {
			perror("bind: An assigment request is already in progress for the specified socket.");
			result = E_ALREADY;
			goto end;
		}

		else if (errno == EBADF) {
			perror("bind: The socket argument is not a valid file descriptor.");
			result = E_BADF;
			goto end;
		}
	}

	if ((listen(server_socket, 2)) != -1 ) {
		if (errno == EBADF) {
			perror("listen: The socket argument is not a valid file descriptor.");
			result = E_BADF;
			goto end;
		}
		if (errno == EACCES) {
			perror("listen: The calling process does not have appropriate privileges");
			result = E_ACCES;
			goto end;
		}
		if (errno == ENOBUFS) {
			perror("listen: Insufficient resources are available in the system to complete the call.");
			result = E_NOBUFS;
			goto end;
		}
	}

	printf("Listening on %s, port %d\n", inet_ntoa(server_address.sin_addr), PORT_NUMBER);

	while (1) {
		size = sizeof(client_address);
		client_socket = accept(server_socket, (struct sockaddr *) &client_address, &size);
		if (client_socket == -1) {
			if (errno == EBADF) {
				perror("accept: The descriptor is invalid.");
				result = E_BADF;
				goto end;
			}

			else if (errno == ECONNABORTED) {
				perror("accept: A connection has been aborted.");
				result = E_CONNABORTED;
				goto end;
			}

			else if (errno == EMFILE) {
				perror("accept: The per-process limit of open file descriptors has been reached.");
				result = E_MFILE;
				goto end;
			}

			else if (errno == ENFILE) {
				perror("accept: The system limit on the total number of open files has been reached.");
				result = E_NFILE;
				goto end;
			}

			else if (errno == ENOBUFS || errno == ENOMEM) {
				perror("accept: Not enough free memory.");
				result = E_NOBUFS;
				goto end;
			}

			else if (errno == EPROTO){
				perror("accept: Protocol error.");
				result = E_PROTO;
				goto end;
			}
		}
		printf("Client conncected from: %s\n", inet_ntoa(client_address.sin_addr));
		send(client_socket, response_data, strlen(response_data), 0);
		close(client_socket);
	}
	close(server_socket);
	result = E_SUCCESS;
	goto end;

	end: return result;

}
