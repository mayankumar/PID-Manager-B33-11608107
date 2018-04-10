#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "pid.h"

pthread_mutex_t mutex; 

void *allocator(void *parameter) {
    
	int sleepTime = rand() % MAX_SLEEP_SECS;

    int pid = -1;
    while(pid == -1) {
        pthread_mutex_lock(&mutex);
        pid = allocate_pid();
        if (pid == -1) {
            printf("Sorry!! Can\'t allocate Process id for now!\n");
        }
        pthread_mutex_unlock(&mutex);
    }

    printf("Process Id %i is active. Sleeps for %i seconds\n", pid, sleepTime);
    sleep(sleepTime);

    printf("Process Id %i has finished sleeping. Releasing this pid...\n", pid);
    release_pid(pid);

    return NULL;
}


int main(void) {
    int i;
    pthread_t thread_ids[NUM_THREADS];
    
    srand(time(NULL)); 

    pthread_mutex_init(&mutex, NULL);

    
    if (allocate_map() == -1) {
        printf("Sorry!! Could not initialize the pidmap. Terminating program...\n");
        return -1;
    }

    
    for (i = 0; i < NUM_THREADS; i++) {
        pthread_create(&thread_ids[i], NULL, allocator, NULL);
    }

    
    for (i = 0; i < NUM_THREADS; i++) {
        pthread_join(thread_ids[i], NULL);
    }

    
    if (pidmap != NULL) {
        free(pidmap);
    }

    printf("Testing done. Terminating program...\n");
    return 0;
}
