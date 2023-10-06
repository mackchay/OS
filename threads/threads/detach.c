#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

int mythread(void *args) {
    printf("my_thread [%d, %d, %d, %ld]: success.\n", getpid(), getppid(), gettid(), pthread_self());
    //pthread_detach(pthread_self());
    int a = 42;
    return a;
}

int main(void) {
    pthread_t tid;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    int number;

    pthread_create(&tid, NULL, (void*)mythread, NULL);
    pthread_join(tid, (void*)&number);
    printf("result: [%d]\n", number);
    while (1)
    {
        pthread_create(&tid, NULL, (void*)mythread, NULL);
        //pthread_join(tid, NULL);
        sleep(1);
    }

    pthread_attr_destroy(&attr);
    
    return 0;
}