#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_ADDRESS "127.0.0.1"
#define PORT_NUMBER 8888
#define MAX_MESSAGE_SIZE 1000

int main() {
    int sock;
    struct sockaddr_in server;
    char message[MAX_MESSAGE_SIZE], server_reply[MAX_MESSAGE_SIZE];

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("Could not create socket");
        return 1;
    }

    // Prepare server address structure
    server.sin_addr.s_addr = inet_addr(SERVER_ADDRESS);
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT_NUMBER);

    // Connect to server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0) {
        perror("Connection failed");
        return 1;
    }

    printf("Connected to server\n");

    // Main loop to send messages
    while (1) {
        printf("Enter message : ");
        fgets(message, sizeof(message), stdin);

        // Send some data
        if (send(sock, message, strlen(message), 0) < 0) {
            puts("Send failed");
            return 1;
        }

        // Receive a reply from the server
        if (recv(sock, server_reply, MAX_MESSAGE_SIZE, 0) < 0) {
            puts("recv failed");
            break;
        }

        // Print both client's message and server's echo on the same line
        printf("Client: %sServer echoes: %s", message, server_reply);

        memset(message, 0, sizeof(message)); // Clear input buffer
        memset(server_reply, 0, sizeof(server_reply)); // Clear server reply buffer
    }

    close(sock);
    return 0;
}
