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

    // Creating socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) { // control for socket created or not
        perror("Could not create socket");
        return 1;
    }

    // Server address features initialized
    server.sin_addr.s_addr = inet_addr(SERVER_ADDRESS);
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT_NUMBER);

    // Connecting server with socket address
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0) {
        perror("Connection failed");
        return 1;
    }

    printf("Connected to server\n");

    // Looping over for send messages from client
    while (1) {
        printf("Enter message : ");
        fgets(message, sizeof(message), stdin);

        // Send some data
        if (send(sock, message, strlen(message), 0) < 0) {
            puts("Send failed"); // control for send or not
            return 1;
        }
        
        if (recv(sock, server_reply, MAX_MESSAGE_SIZE, 0) < 0) {
            puts("recv failed"); // control for receive or not
            break;
        }

        printf("Client: %sServer echoes: %s", message, server_reply);

        memset(message, 0, sizeof(message)); // Clear the buffer for synchronization
        memset(server_reply, 0, sizeof(server_reply)); // Clear the buffer for server same reason
    }

    close(sock); // close the socket after send message
    return 0;
}
