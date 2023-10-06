#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <malloc.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#define SIZE 16

void clean_up(void *args) {
    printf("Time to clean up!\n");
    free(args);
}

void *my_thread(void *args) {
    char *str = (char*)args;
    printf("my_thread [%d, %d, %d, %ld]: success.\n", getpid(), getppid(), gettid(), pthread_self());
    str = malloc(SIZE * sizeof(char));

    int iter = 0;
    while (1) {
        //printf("Hello from thread: %d\n", iter);
        iter++;
        //pthread_testcancel();
    }
}

int main(void) {
    pthread_t tid;
	pthread_t tid1;
	int err;
    char *str = NULL;

    pthread_cleanup_push(clean_up, str);
	printf("main [%d %d %d]: Hello from main!\n", getpid(), getppid(), gettid());
    err = pthread_create(&tid, NULL, my_thread, NULL);
	if (err) {
	    printf("main: pthread_create() failed: %s\n", strerror(err));
		return -1;
	}

    sleep(2);
    pthread_cancel(tid);
    pthread_join(tid, NULL);
    pthread_cleanup_pop(1);

    return 0;
}