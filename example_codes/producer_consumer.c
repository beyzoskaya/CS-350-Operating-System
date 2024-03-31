// // Producer process
// void producer() {
//     while (TRUE) {
//         // Produce an item
//         item = produce_item();

//         // Acquire mutex lock to access buffer
//         acquire(mutex);

//         // If buffer is full, wait until it's not full
//         while (buffer_is_full()) {
//             // Release mutex temporarily while waiting
//             release(mutex);
            
//             // Sleep until wakeup signal is received
//             sleep(producer_sleep_queue);
            
//             // Reacquire mutex after wakeup
//             acquire(mutex);
//         }

//         // Add item to buffer
//         insert_item(item);

//         // Signal consumers that an item is available
//         wakeup(consumer_sleep_queue);

//         // Release mutex
//         release(mutex);
//     }
// }

// // Consumer process
// void consumer() {
//     while (TRUE) {
//         // Acquire mutex lock to access buffer
//         acquire(mutex);

//         // If buffer is empty, wait until it's not empty
//         while (buffer_is_empty()) {
//             // Release mutex temporarily while waiting
//             release(mutex);
            
//             // Sleep until wakeup signal is received
//             sleep(consumer_sleep_queue);
            
//             // Reacquire mutex after wakeup
//             acquire(mutex);
//         }

//         // Remove item from buffer
//         item = remove_item();

//         // Signal producers that there's space in the buffer
//         wakeup(producer_sleep_queue);

//         // Release mutex
//         release(mutex);

//         // Consume the item
//         consume_item(item);
//     }
// }

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define BUFFER_SIZE 5
#define NUM_ITEMS 10

int buffer[BUFFER_SIZE];
int in = 0, out = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t full = PTHREAD_COND_INITIALIZER;
pthread_cond_t empty = PTHREAD_COND_INITIALIZER;

void *producer(void *arg) {
    for (int i = 0; i < NUM_ITEMS; ++i) {
        pthread_mutex_lock(&mutex);
        while ((in + 1) % BUFFER_SIZE == out) {
            pthread_cond_wait(&empty, &mutex);
        }
        buffer[in] = i;
        in = (in + 1) % BUFFER_SIZE;
        printf("Produced: %d\n", i);
        pthread_cond_signal(&full);
        pthread_mutex_unlock(&mutex);
        usleep(rand() % 1000000); // Simulate work
    }
    return NULL;
}

void *consumer(void *arg) {
    for (int i = 0; i < NUM_ITEMS; ++i) {
        pthread_mutex_lock(&mutex);
        while (in == out) {
            pthread_cond_wait(&full, &mutex);
        }
        int item = buffer[out];
        out = (out + 1) % BUFFER_SIZE;
        printf("Consumed: %d\n", item);
        pthread_cond_signal(&empty);
        pthread_mutex_unlock(&mutex);
        usleep(rand() % 1000000); // Simulate work
    }
    return NULL;
}

int main() {
    pthread_t producer_thread, consumer_thread;

    srand(time(NULL));

    pthread_create(&producer_thread, NULL, producer, NULL);
    pthread_create(&consumer_thread, NULL, consumer, NULL);

    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    return 0;
}

