// while(TRUE){
//     while(turn != 0) /* loop */;
//     critical_region();
//     turn = 1;
//     noncritical_region();
// }

// while(TRUE){
//     while(turn != 1) /* loop */;
//     critical_region();
//     turn = 0;
//     noncritical_region();
// }

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define TRUE 1
#define FALSE 0

int turn = 0; 

void critical_region(int process_id) {
    printf("Process %d is in the critical region\n", process_id);
    sleep(1);
}

void noncritical_region(int process_id) {
    printf("Process %d is in the non-critical region\n", process_id);
}

void *process(void *arg) {
    int process_id = *((int *) arg);

    while (TRUE) {
        while (turn != process_id) {
        }

        critical_region(process_id);
        turn = (process_id + 1) % 2;

        noncritical_region(process_id);
    }

    return NULL;
}

int main() {
    pthread_t threads[2];
    int thread_ids[2] = {0, 1};
    int i;

    for (i = 0; i < 2; i++) {
        pthread_create(&threads[i], NULL, process, &thread_ids[i]);
    }

    for (i = 0; i < 2; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
