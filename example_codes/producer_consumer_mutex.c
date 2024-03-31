// preventing race conditions 

#include <stdio.h>
#include <pthread.h>
#define MAX 1000000000  /* how many numbers to produce */
pthread_mutex_t the_mutex;  // initialization of mutex
pthread_cond_t condc, condp; /* used for signaling */   // initialization of condition variables producer and consumer
int buffer = 0;    /* buffer used between consumer and producer */


/*
For each iteration, 
it locks the mutex to gain exclusive access to the buffer. 
If the buffer is not empty, 
the producer waits (pthread_cond_wait) on the condition variable condp. 
Once it gets the signal to proceed (i.e., buffer == 0), it places the item (i) into the buffer, 
signals the consumer thread to wake up (pthread_cond_signal(&condc)), and releases the mutex.
*/
void *producer(void *ptr){ /* produce data */
    int i;

    for(i=1; i<MAX; i++){
        pthread_mutex_lock(&the_mutex); /* get exclusive access to buffer */
        while(buffer != 0) pthread_cond_wait(&condp, &the_mutex);
        buffer = i;   /* put item in buffer */
        pthread_cond_signal(&condc); /* wakeup consumer */
        pthread_mutex_unlock(&the_mutex);   /* release access to buffer */

    }
    pthread_exit(0);
}


/*
Similar to the producer function, 
it locks the mutex to gain exclusive access to the buffer. 
If the buffer is empty (i.e., buffer == 0), 
the consumer waits (pthread_cond_wait) on the condition variable condp. 
Once it gets the signal to proceed (i.e., buffer != 0), 
it consumes the item by setting the buffer to 0, 
signals the producer thread to wake up (pthread_cond_signal(&condp)), and releases the mutex.
*/
void *consumer(void *ptr){ /* consume data */
    int i;

    for(i=1; i<MAX; i++){
        pthread_mutex_lock(&the_mutex); /* get exclusive access to buffer */
        while(buffer == 0) pthread_cond_wait(&condp, &the_mutex);
        buffer = 0;
        pthread_cond_signal(&condc); /* wakeup producer */
        pthread_mutex_unlock(&the_mutex);   /* release access to buffer */

    }
    pthread_exit(0);
}


/*
initializes the mutex and condition variables. 
It then creates the producer and consumer threads using pthread_create. 
After both threads are created, it waits for them to finish execution using pthread_join. 
Finally, it destroys the mutex and condition variables
*/
int main(int argc, char **argv){
    pthread_t pro,con;
    pthread_mutex_init(&the_mutex,0);
    pthread_cond_init(&condc,0);
    pthread_cond_init(&condp,0);
    pthread_create(&con,0,consumer,0);
    pthread_create(&pro,0,producer,0);
    pthread_join(pro,0);
    pthread_join(con,0);
    pthread_cond_destroy(&condc);
    pthread_cond_destroy(&condp);
    pthread_mutex_destroy(&the_mutex);


}


