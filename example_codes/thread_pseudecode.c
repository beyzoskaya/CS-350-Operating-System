#include <pthread.h> // POSIX threads
#include <stdio.h>   // standard input-output printing
#include <stdlib.h>  // standard library for exit and other ones


#define NUMBER_OF_THREADS 10

void *print_hello_world(void *tid){
    /* This function prints the threads identifier and then exists*/
    printf("Hello world. Greetings from thread %d\n", tid);
    pthread_exit(NULL);

}

int main(int argc, char *argv[]){
    /* The main program creates 10 threads and then exists*/
    pthread_t threads[NUMBER_OF_THREADS]; // Declaration of pthreads array called threads to hold thread identifiers
    int status, i;

    for(i=0; i<NUMBER_OF_THREADS; i++){
        printf("Main here. Creating thread %d\n", i);
        status = pthread_create(&threads[i], NULL, print_hello_world, (void *)i);
        /*  Each thread executes the print_hello_world function, 
        which prints a message with its thread identifier (tid) and then exits */

        if (status != 0) { // checks the return status of pthread_create(). If it's not 0, indicates failure, prints an error message, and exits the program using exit(EXIT_FAILURE)
            fprintf(stderr, "pthread_create failed: %d\n", status);
            exit(EXIT_FAILURE);
        }

    }
    pthread_exit(NULL);
}