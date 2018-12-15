#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define NUM_THREADS 3
void *thread(void *arg){
	printf("Thread%d: Hello Thread!\n", arg);
	return arg;
}

void main(){
	pthread_t tid[NUM_THREADS];
	int i, status;

	for(i=0; i<NUM_THREADS; i++){
		status = pthread_create (&tid[i], NULL, thread, (void *) i);
		if(status != 0){
			fprintf(stderr, "Create Thread%d : %d", status);
			exit(1);
		}
	}
	pthread_exit (NULL);
}
