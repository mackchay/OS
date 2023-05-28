#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAX_BUFFER_SIZE 1024

int main() {
    int client_socket;
    struct sockaddr_in server_address;
    char buffer[MAX_BUFFER_SIZE];

    client_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (client_socket < 0) {
        perror("Failed to create socket");
        exit(EXIT_FAILURE);
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8888);

    if (inet_aton("192.168.155.70", &server_address.sin_addr) == 0) {
        fprintf(stderr, "Invalid IP address\n");
        exit(EXIT_FAILURE);
    }

    while (1) {
        printf("Enter message (or 'q' to quit): ");
        fgets(buffer, MAX_BUFFER_SIZE, stdin);

        buffer[strcspn(buffer, "\n")] = 0;

        if (strcmp(buffer, "q") == 0) {
            break;
        }

        if (sendto(client_socket, buffer, strlen(buffer), 0, (struct sockaddr *)&server_address,
                sizeof(server_address)) < 0) {
            perror("Failed to send data");
            continue;
        }

        socklen_t server_address_length = sizeof(server_address);
        int num_bytes = recvfrom(client_socket, buffer, MAX_BUFFER_SIZE, 0, (struct sockaddr *)&server_address, &server_address_length);
        if (num_bytes < 0) {
            perror("Failed to receive data");
            continue;
        }

        buffer[num_bytes] = '\0';
        printf("Server response: %s\n", buffer);
    }

    close(client_socket);

    return 0;
}

