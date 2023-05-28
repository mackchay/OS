#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAX_BUFFER_SIZE 1024

int main() {
    int server_socket;
    struct sockaddr_in server_address, client_address;
    char buffer[MAX_BUFFER_SIZE];

    server_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_socket < 0) {
        perror("Failed to create socket");
        exit(EXIT_FAILURE);
    }

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(8888);

    if (inet_aton("192.168.155.70", &server_address.sin_addr) == 0) {
        fprintf(stderr, "Invalid IP address\n");
        exit(EXIT_FAILURE);
    }

    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("Failed to bind socket");
        exit(EXIT_FAILURE);
    }

    printf("UDP Echo Server started on port 8888\n");

    while (1) {
        memset(buffer, 0, MAX_BUFFER_SIZE);
        socklen_t client_address_length = sizeof(client_address);

        int num_bytes = recvfrom(server_socket, buffer, MAX_BUFFER_SIZE, 0,
                                 (struct sockaddr *)&client_address, &client_address_length);

        if (num_bytes < 0) {
            perror("Failed to receive data");
            continue;
        }

        printf("Received %d bytes from %s:%d\n", num_bytes, inet_ntoa(client_address.sin_addr),
               ntohs(client_address.sin_port));
        printf("Data: %s\n", buffer);

        if (sendto(server_socket, buffer, num_bytes, 0,
                   (struct sockaddr *)&client_address, client_address_length) < 0) {
            perror("Failed to send data");
            continue;
        }

        printf("Echoed back %d bytes to %s:%d\n", num_bytes, inet_ntoa(client_address.sin_addr),
               ntohs(client_address.sin_port));
    }

    close(server_socket);

    return 0;
}
