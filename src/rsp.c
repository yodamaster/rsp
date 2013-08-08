#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAX_LISTEN_BACKLOG 1


void handle_client_connection(int fd, char *backend_addr, int backend_port) {
}


int main(int argc, char *argv[]) {
    int server_port;
    int server_socket_fd;
    int client_socket_fd;
    struct sockaddr_in server_socket_addr;
    struct sockaddr_in client_socket_addr;
    int client_socket_addr_size;
    char *backend_addr;
    int backend_port;

    if (argc != 4) {
        printf("Usage: %s <server_port> <backend_addr> <backend_port>\n", argv[0]);
        exit(1);
    }
    server_port = atoi(argv[1]);
    backend_addr = argv[2];
    backend_port = atoi(argv[3]);

    server_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket_fd < 0) {
        perror("Could not open socket");
        exit(1);
    }

    memset((char *) &server_socket_addr, 0, sizeof(server_socket_addr));
    server_socket_addr.sin_family = AF_INET;
    server_socket_addr.sin_port = htons(server_port);
    server_socket_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket_fd, (struct sockaddr *) &server_socket_addr, sizeof(server_socket_addr)) < 0) {
        perror("Could not bind");
        exit(1);
    }

    listen(server_socket_fd, MAX_LISTEN_BACKLOG);

    client_socket_addr_size = sizeof(client_socket_addr);
    client_socket_fd = accept(server_socket_fd, (struct sockaddr *) &client_socket_addr, &client_socket_addr_size);
    if (client_socket_fd < 0) {
        perror("Could not accept");
        exit(1);
    }

    handle_client_connection(client_socket_fd, backend_addr, backend_port);
}
