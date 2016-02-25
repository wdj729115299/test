#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

int g_var = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *thread_func(void *arg)
{
	int i, j;
	for(i = 0; i < 20; i++){
		pthread_mutex_lock(&mutex);
		j = g_var;
		j += 1;
		printf(".");
		fflush(stdout);
		sleep(1);
		g_var = j;
		pthread_mutex_unlock(&mutex);
	}
	return NULL;
}

int main(void)
{
	int i;
	pthread_t pid;
	
	if(pthread_create(&pid, NULL, thread_func, NULL)){
		perror("create thread failed.\n");
		return;
	}

	for(i = 0; i < 20; i++){
		pthread_mutex_lock(&mutex);
		g_var += 1;
		pthread_mutex_unlock(&mutex);
		printf("x");
		fflush(stdout);
		sleep(1);
	}

	if(pthread_join(pid, NULL)){
		perror("pthread join failed\n");
		return;
	}

	printf("g_var = %d\n", g_var);

	return 0;
}
