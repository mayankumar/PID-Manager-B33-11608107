#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "pid.h"

pthread_mutex_t mutex; 

int allocate_map(void) {
	
	pidmap = calloc(RANGE, sizeof(int));

	if (pidmap != NULL)
		return 1;

	return -1;
}

int allocate_pid(void) {
	int i = 0;

		while (pidmap[i] != 0) {
		i++;
	}

	
	if (i + PID_MIN > PID_MAX) {
		return -1;
	}

	
	pidmap[i] = 1;

	return i + PID_MIN;
}

void release_pid(int pid) {
	if (pid < PID_MIN || pid > PID_MAX) {
		printf("Error: Invalid pid. It must be inside the interval {%i - %i}\n", PID_MIN, PID_MAX);
		return;
	}
	pidmap[pid - PID_MIN] = 0;
}
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
