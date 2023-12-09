/*
Change ip address in client to server's ip address
port number remains same in both client and server
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc,char *argv[]) {
    int client_socket;
    struct sockaddr_in server_address;
    char buffer[1024];
    char message[1024];

    client_socket = socket(AF_INET, SOCK_STREAM, 0);

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(argv[1]);
    server_address.sin_port = htons(12345);

    connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address));

    printf("Chat with the server (type 'bye' to exit):\n");

    while (1) {
        printf("Client: ");
        fgets(message, sizeof(message), stdin);
        send(client_socket, message, strlen(message), 0);

        if (strcmp(message, "bye\n") == 0) {
            printf("Terminating the chat...\n");
            break;
        }

        ssize_t bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes_received > 0) {
            buffer[bytes_received] = '\0';
            printf("Server: %s", buffer);

            if (strcmp(buffer, "bye\n") == 0) {
                printf("Server has terminated the chat.\n");
                break;
            }
        }
    }

    close(client_socket);

    return 0;
}
