#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <time.h>

#define MAX_CLIENTS 3
#define BUFFER_SIZE 1024
#define PORT 8888
//#define IDLE_TIMEOUT_SECONDS 120 // 2 minutes keep server awake it is optional with my thoughts

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

int active_client_count = 0; // need to keep track of number of clients to chat or waited
int client_ids[MAX_CLIENTS];
int client_sockets[MAX_CLIENTS]; // client number of sockets need to be created to communicate with each client
//time_t last_client_disconnect_time = 0;

void handle_client(int client_socket, int client_id);
void* client_thread(void* arg);
int assign_client_id(); 
//void close_server();

int main() {
    int server_socket, client_socket; // create both server and client socket variables to use after
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    pthread_t thread_id;

    // Creating socket for server
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) { // control for fail to create or not
        perror("Failed to create socket");
        exit(EXIT_FAILURE);
    }

    // Creating server address features
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Binding socket 
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_socket, MAX_CLIENTS) < 0) {
        perror("Listen failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d\n", PORT);

    // Accept clients 
    while (1) {
        client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_len);
        if (client_socket == -1) {
            perror("Accept failed");
            continue;
        }

        pthread_mutex_lock(&mutex);
        if (active_client_count >= MAX_CLIENTS) {
            printf("Maximum client limit reached. Connection cannot be initialized.\n");
            close(client_socket);
        } else {
            pthread_create(&thread_id, NULL, client_thread, (void*)&client_socket);
            pthread_detach(thread_id); 
        }
        pthread_mutex_unlock(&mutex);
    }


    close(server_socket);
    return 0;
}

//void close_server() {
//    pthread_mutex_lock(&mutex);
//    time_t current_time = time(NULL);
//    double elapsed_time = difftime(current_time, last_client_disconnect_time);
//    if (active_client_count == 0 && elapsed_time >= IDLE_TIMEOUT_SECONDS) {
//        printf("Closing server due to inactivity...\n");
//    }
//    pthread_mutex_unlock(&mutex);
//}


void* client_thread(void* arg) {
    int client_socket = *(int*)arg;
    int client_id;

    pthread_mutex_lock(&mutex);
    client_id = assign_client_id();
    client_ids[client_id] = client_id;
    client_sockets[client_id] = client_socket;
    active_client_count = active_client_count + 1;

    if (active_client_count == MAX_CLIENTS) {
        printf("Client %d is waiting for an available slot.\n", client_id);
        pthread_cond_wait(&cond, &mutex); 
    }
    pthread_mutex_unlock(&mutex);

    handle_client(client_socket, client_id);

    pthread_mutex_lock(&mutex);
    active_client_count = active_client_count -1;

    if (active_client_count == MAX_CLIENTS - 1) {
        pthread_cond_signal(&cond); 
        printf("A waiting client can now connect.\n");
    }

    for (int i = client_id; i < active_client_count; i++) {
        client_ids[i] = client_ids[i + 1];
        client_sockets[i] = client_sockets[i + 1];
    }

    //if (active_client_count > client_id) {
    //    int num_remaining = active_client_count - client_id;
    //    memcpy(&client_ids[client_id], &client_ids[client_id + 1], num_remaining * sizeof(int));
    //    memcpy(&client_sockets[client_id], &client_sockets[client_id + 1], num_remaining * sizeof(int));
    //}

    pthread_mutex_unlock(&mutex);
    close(client_socket);
    pthread_exit(NULL);
}
int assign_client_id() {
    static int next_client_id = 0;
    return next_client_id++;
}

void handle_client(int client_socket, int client_id) {
    char buffer[BUFFER_SIZE];
    ssize_t bytes_received;

    while ((bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0)) > 0) {
        buffer[bytes_received] = '\0';
        printf("Client %d says: %s", client_id, buffer);
        send(client_socket, buffer, bytes_received, 0);
    }

    printf("Client %d disconnected.\n", client_id);
}
