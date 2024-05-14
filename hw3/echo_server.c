#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include <locale.h>

#define PORT_NUMBER 8888
#define BUFFER_SIZE 1024

void *handle_client_connection(void *arg) {
    int client_socket = *((int *)arg);
    char buffer[BUFFER_SIZE];
    ssize_t bytes_received;
    struct sockaddr_in client_addr;
    socklen_t addr_size = sizeof(client_addr);

    // Get client address
    if (getpeername(client_socket, (struct sockaddr *)&client_addr, &addr_size) == 0) {
        printf("Client connected: %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    } else {
        printf("Client connected: Unknown address\n");
    }

    while ((bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0)) > 0) {
        send(client_socket, buffer, bytes_received, 0);
        memset(buffer, 0, BUFFER_SIZE);
    }

    if (bytes_received == 0) {
        printf("Client disconnected: %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    } else if (bytes_received == -1) {
        perror("Error receiving data from client");
    }

    close(client_socket);
    pthread_exit(NULL);
}

int main() {
    setlocale(LC_ALL, "en_US.UTF-8");
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;

    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Prepare the server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT_NUMBER);

    // Bind socket to address and port
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Binding failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_socket, 5) == -1) {
        perror("Listening failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT_NUMBER);

    while (1) {
        // Accept incoming connection
        addr_size = sizeof(client_addr);
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addr_size);
        if (client_socket == -1) {
            perror("Accepting client connection failed");
            continue;
        }

        // Create a new thread to handle client connection
        pthread_t client_thread;
        if (pthread_create(&client_thread, NULL, handle_client_connection, &client_socket) != 0) {
            perror("Error creating client thread");
            close(client_socket);
            continue;
        }

        // Detach the thread to avoid memory leak
        pthread_detach(client_thread);
    }

    close(server_socket);
    return 0;
}
