#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <signal.h>

#define MAX_MESSAGE_SIZE 1000
#define CHAT_DURATION 10
#define PORT 8888

int client_socket;

void signal_handler(int signum);
void* receive_messages(void* arg);

int main() {
    struct sockaddr_in server_address;
    char message[MAX_MESSAGE_SIZE]; // Buffer for keeping inputs

    // Creating socket for client
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Failed to create socket");
        return 1;
    }
    printf("Socket created\n");

    // Initialize server address features
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1"); // specific server address
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);

    // Connecting to server with server address if less than 0 meaning there is a problem while connection
    if (connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        perror("Connection failed");
        return 1;
    }
    printf("Connected to server\n");

    // Signal creation for keep track of chat duration as 10 sec
    signal(SIGALRM, signal_handler);
    alarm(CHAT_DURATION); // set an alarm

    // Thread for getting messages from client
    pthread_t receiver_thread;
    if (pthread_create(&receiver_thread, NULL, receive_messages, NULL) < 0) {
        perror("Failed to create receiver thread");
        return 1;
    }

    // Looping for messages to access from server
    while (1) {
        printf("Enter message: ");
        fgets(message, sizeof(message), stdin);
        //fflush(stdin);

        if (send(client_socket, message, strlen(message), 0) < 0) {
            perror("Send failed");
            return 1;
            //break; 
        }
    }

    close(client_socket); // closing client socket after disconnect
    return 0;
}

// Handling signal for keep track of chat duration
void signal_handler(int signum) {
    if (signum == SIGALRM) {
        printf("Chat duration finished.\n");
        close(client_socket);
        exit(0);
    }
}

// Receiving messages from client socket to server
void* receive_messages(void* arg) {
    char response[MAX_MESSAGE_SIZE];

    while (1) {
        if (recv(client_socket, response, MAX_MESSAGE_SIZE, 0) > 0) {
            printf("Server echoes: %s", response);
        }
    }
}
