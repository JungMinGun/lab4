#include <pthread.h>
#include <stdio.h>

void *thread(void *arg){
	printf("Thread : Hello Thread!\n");
	return arg;
}
void main(){
	pthread_t tid;
	int status;

	status = pthread_create(&tid, NULL, thread, NULL);
	if(status != 0)
		perror("Create thread");
	pthread_exit(NULL);
}

