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

// Variables to use in handling connection
int server_socket; // socket for sever
int client_sockets[MAX_CLIENTS]; 
int num_clients = 0; // keep track of number of clients

// Function for client connections
void *handle_client_connection(void *arg) {
    int client_socket = *((int *)arg);
    char buffer[BUFFER_SIZE]; // create buffer as an array amount of BUFFER SIZE
    ssize_t bytes_received; // control for bytes received
    struct sockaddr_in client_addr;
    socklen_t addr_size = sizeof(client_addr); // address size depend on the size of client address

    // For printing client address and peer name 
    if (getpeername(client_socket, (struct sockaddr *)&client_addr, &addr_size) == 0) {
        printf("Client connected: %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    } else { // if cannot getpername address is not known
        printf("Client connected: Peer address is unknown \n");
    }

    //num_clients = num_clients + 1;
    //client_sockets[num_clients] = client_socket;
    client_sockets[num_clients++] = client_socket; // keep track all clients with sockets in client_sockets array

    // if bytes bigger than 0 meaning that receiving is ok so we can initialize byte_received with recv function
    while ((bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0)) > 0) {
        buffer[bytes_received] = '\0'; // use for handling the special charachters seen in terminal even if not writing from client
        printf("Client %s:%d says: %s", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), buffer);

        // Echo 
        send(client_socket, buffer, bytes_received, 0);
        printf("Server echoes: %s", buffer);

        memset(buffer, 0, BUFFER_SIZE); // It is a good practice to clear buffer 
    }

    // If there is no received byte then the client is disconnected
    if (bytes_received == 0) {
        printf("Client %s:%d disconnected\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    } else if (bytes_received == -1) {
        perror("Error receiving data from client");
    }

    // Disconnected client removed from a client_sockets array
    for (int i = 0; i < num_clients; i++) {
        if (client_sockets[i] == client_socket) {
            for (int j = i; j < num_clients - 1; j++) {
                client_sockets[j] = client_sockets[j + 1];
            }
            num_clients=num_clients-1;
            break;
        }
    }

    close(client_socket); // close socket for client after sending messages
    pthread_exit(NULL);
}

// Server socket also closing after all the client sockets shuted down
void sigint_handler(int signum) {
    printf("\nServer shutting down...\n");

    for (int i = 0; i < num_clients; i++) {
        close(client_sockets[i]);
    }

    close(server_socket);
    exit(EXIT_SUCCESS);
}

int main() {
    setlocale(LC_ALL, "en_US.UTF-8"); // use to handle special chars in terminal even if client didn't use them
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;

    // Creating server socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) { // control server creation
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    } else {
        printf("Socket created successfully\n");
    }

    // Create server address features
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT_NUMBER);

    // Binding socket to specific address
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) { // control for failed or successful
        perror("Binding failed");
        exit(EXIT_FAILURE);
    } else {
        printf("Socket binding successful\n");
    }

    // Listening incoming connections
    if (listen(server_socket, 5) == -1) {
        perror("Listening failed");
        exit(EXIT_FAILURE);
    } else {
        printf("Listening for incoming connections\n");
    }

    struct sigaction sa;
    sa.sa_handler = sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("Error setting up SIGINT handler");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", PORT_NUMBER);

    while (1) {
        // Getting messages with accept them 
        addr_size = sizeof(client_addr);
        int client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addr_size);
        if (client_socket == -1) { // control for client connection
            perror("Accepting client connection failed");
            continue;
        }

        // Client thread
        pthread_t client_thread;
        if (pthread_create(&client_thread, NULL, handle_client_connection, &client_socket) != 0) {
            perror("Error creating client thread");
            close(client_socket);
            continue;
        }

        // Handle memory leak with detach
        pthread_detach(client_thread);
    }

    // Close the server socket
    close(server_socket);
    return 0;
}
