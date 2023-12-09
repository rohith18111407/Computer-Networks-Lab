/*
Change ip address in client to serveer's ip address
port number remains same in both client and server
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_address, client_address;
    socklen_t client_address_len = sizeof(client_address);
    char buffer[1024];

    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(12345);

    bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address));
    listen(server_socket, 5);

    printf("Waiting for a client to connect...\n");
    client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_address_len);
    printf("Client connected.\n");

    while (1) {
        ssize_t bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes_received > 0) {
            buffer[bytes_received] = '\0';
            printf("Client: %s", buffer);

            if (strcmp(buffer, "bye\n") == 0) {
                printf("Client has terminated the chat.\n");
                break;
            }
        }

        printf("Server: ");
        fgets(buffer, sizeof(buffer), stdin);
        send(client_socket, buffer, strlen(buffer), 0);

        if (strcmp(buffer, "bye\n") == 0) {
            printf("Terminating the chat...\n");
            break;
        }
    }

    close(client_socket);
    close(server_socket);

    return 0;
}
