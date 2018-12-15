#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#define NUM_THREADS 3
pthread_mutex_t mutex;
int sum;

void *mutex_thread(void *arg){
	pthread_mutex_lock(&mutex);
	sum += (int)arg;
	pthread_mutex_unlock(&mutex);
	return arg;
}

int main(int argc, char *argv[]){
	pthread_t tid[NUM_THREADS];
	int arg[NUM_THREADS], i;
	void *result;
	int status;

	if(argc < 4) {
		fprintf(stderr, "Usage: mutexthread num1 num2 num3\n");
		exit(1);
	}
	
	for(i=0; i<NUM_THREADS; i++)
		arg[i] = atoi(argv[i+1]);

	pthread_mutex_init(&mutex, NULL);

	for(i=0; i<NUM_THREADS; i++){
		status = pthread_create(&tid[i], NULL, mutex_thread, (void *)arg[i]);
		if(status != 0){
			printf (stderr, "create thread %d: %d", i, status);
			exit(1);
		}
	}

	for(i=0; i<NUM_THREADS; i++){
		status = pthread_join(tid[i], &result);
		if(status != 0){
			fprintf(stderr, "join thread %d: %d", i, status);
			exit(1);
		}
}

	status = pthread_mutex_destroy (&mutex);
	if(status != 0)
		perror("destory mutex");
	printf("sum is %d\n", sum);
	pthread_exit(result);
}
