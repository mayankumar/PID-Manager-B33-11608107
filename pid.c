#include <stdio.h>
#include <stdlib.h>
#include "pid.h"


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
