#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <locale.h>

#define SERVER_ADDRESS "127.0.0.1"
#define PORT_NUMBER 8888
#define MAX_MESSAGE_SIZE 1000

int sock;

void *receive_messages(void *arg) {
    char response[MAX_MESSAGE_SIZE];
    ssize_t bytes_received;

    while ((bytes_received = recv(sock, response, MAX_MESSAGE_SIZE - 1, 0)) > 0) {
        response[bytes_received] = '\0'; // Null-terminate the received message
        printf("Server: %s\n", response);
        memset(response, 0, MAX_MESSAGE_SIZE); // Clear the message buffer
        printf("Enter message: ");
        fflush(stdout); // Flush output buffer to ensure prompt appears immediately
    }

    if (bytes_received == 0) {
        printf("Server disconnected\n");
    } else if (bytes_received == -1) {
        perror("Error receiving data from server");
    }

    close(sock);
    exit(EXIT_SUCCESS);
}

int main() {
    setlocale(LC_ALL, "en_US.UTF-8");

    struct sockaddr_in server;
    char message[MAX_MESSAGE_SIZE];

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("Could not create socket");
        exit(EXIT_FAILURE);
    }

    // Prepare server address structure
    server.sin_addr.s_addr = inet_addr(SERVER_ADDRESS);
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT_NUMBER);

    // Connect to server
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    printf("Connected to server\n");

    // Create thread to receive messages
    pthread_t receiver_thread;
    if (pthread_create(&receiver_thread, NULL, receive_messages, NULL) < 0) {
        perror("Could not create receiver thread");
        exit(EXIT_FAILURE);
    }

    // Main loop to send messages
    while (1) {
        printf("Enter message: ");
        fgets(message, sizeof(message), stdin);
        send(sock, message, strlen(message), 0);

        // Clear the message buffer
        memset(message, 0, sizeof(message));
    }

    close(sock);
    return 0;
}
