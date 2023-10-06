#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

int global = 4;

void *mythread(void *arg) {
	int local = 1;
	static int stat = 2;
	const int local_const = 3;
	printf("mythread [%d %d %d %ld]: Hello from mythread!\n", getpid(), getppid(), gettid(), pthread_self());
	printf("local: %d, local_static: %d, local_const: %d, global: %d\n", &local, &stat, &local_const, &global);
	printf("local: %d, local_static: %d, local_const: %d, global: %d\n", local, stat, local_const, global);
	local = 10;
	global = 10;
	sleep(30);
	return NULL;
}

int main() {
	pthread_t tid;
	pthread_t tid1;
	int err;

	printf("main [%d %d %d]: Hello from main!\n", getpid(), getppid(), gettid());

	sleep(10);
	for (int i = 0; i < 5; i++) {
		err = pthread_create(&tid, NULL, mythread, NULL);
		sleep(1);
		printf("tid: %ld\n", tid);
		sleep(3);
		if (err) {
	    	printf("main: pthread_create() failed: %s\n", strerror(err));
			return -1;
		}
	}
	err = pthread_create(&tid1, NULL, mythread, NULL);
	if (tid != tid1) {
		printf("hello\n");
	}
	sleep(30);
	return 0;
}

