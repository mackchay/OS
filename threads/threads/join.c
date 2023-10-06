#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

void *my_thread(void *args) {
    sleep(3);
    int *a = malloc(sizeof(int));
    printf("my_thread [%d, %d, %d, %ld]: success.\n", getpid(), getppid(), gettid(), pthread_self());
    *a = 42;
    return (void*)a;
}

void *get_str_pointer() {
    printf("my_thread_get_str_pointer [%d, %d, %d, %ld]: success.\n", getpid(), getppid(), gettid(), pthread_self());
    char *str = "hello world";
    return (void *)str;
}

int main(void) {
    pthread_t tid, tid1;
    int *res; 
    char *str;

    printf("main [%d %d %d]: Hello from main!\n", getpid(), getppid(), gettid());
    int err = pthread_create(&tid, NULL, my_thread, NULL);
    if (err) {
        printf("main: pthread_create failed: %s\n", strerror(err));
        return -1;
    }
    pthread_join(tid, (void*)&res);
    int err1 = pthread_create(&tid1, NULL, get_str_pointer, NULL);
    if (err1) {
        printf("main: pthread_create failed: %s\n", strerror(err));
        return -1;
    }
    pthread_join(tid1, (void*)&str);

    printf("thread result: %d\n", *res);
    printf("result: %s\n", str);
    free(res);
    
    return 0;
}