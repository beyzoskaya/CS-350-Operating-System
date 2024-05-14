#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include <locale.h>
#include <signal.h>

#define PORT_NUMBER 8888
#define BUFFER_SIZE 1024
#define MAX_CLIENTS 10

// Global variables
int server_socket;
int client_sockets[MAX_CLIENTS];
int num_clients = 0;

// Function to handle client connections
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

    // Add client socket to the array
    client_sockets[num_clients++] = client_socket;

    while ((bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0)) > 0) {
        buffer[bytes_received] = '\0'; // Null-terminate the received message
        printf("Client %s:%d says: %s", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), buffer);

        // Echo back to the client
        send(client_socket, buffer, bytes_received, 0);
        printf("Server echoes: %s", buffer);

        memset(buffer, 0, BUFFER_SIZE); // Clear buffer for next message
    }

    // Client disconnected
    if (bytes_received == 0) {
        printf("Client %s:%d disconnected\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    } else if (bytes_received == -1) {
        perror("Error receiving data from client");
    }

    // Remove client socket from the array
    for (int i = 0; i < num_clients; i++) {
        if (client_sockets[i] == client_socket) {
            for (int j = i; j < num_clients - 1; j++) {
                client_sockets[j] = client_sockets[j + 1];
            }
            num_clients--;
            break;
        }
    }

    close(client_socket);
    pthread_exit(NULL);
}

// Signal handler function for SIGINT (Ctrl+C)
void sigint_handler(int signum) {
    printf("\nServer shutting down...\n");

    // Close all client sockets
    for (int i = 0; i < num_clients; i++) {
        close(client_sockets[i]);
    }

    // Close the server socket
    close(server_socket);

    // Exit the program
    exit(EXIT_SUCCESS);
}

int main() {
    setlocale(LC_ALL, "en_US.UTF-8");
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;

    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    } else {
        printf("Socket created successfully\n");
    }

    // Prepare the server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT_NUMBER);

    // Bind socket to address and port
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Binding failed");
        exit(EXIT_FAILURE);
    } else {
        printf("Socket binding successful\n");
    }

    // Listen for incoming connections
    if (listen(server_socket, 5) == -1) {
        perror("Listening failed");
        exit(EXIT_FAILURE);
    } else {
        printf("Listening for incoming connections...\n");
    }

    // Set up signal handler for SIGINT
    struct sigaction sa;
    sa.sa_handler = sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("Error setting up SIGINT handler");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT_NUMBER);

    while (1) {
        // Accept incoming connection
        addr_size = sizeof(client_addr);
        int client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addr_size);
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

    // Close the server socket (this should not be reached)
    close(server_socket);
    return 0;
}
