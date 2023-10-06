#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#define SIZE 50

typedef struct Object{
    int number;
    char *str;
} Object;


void *my_thread(Object *object) {
    printf("my_thread [%d, %d, %d, %ld]: success.\n", getpid(), getppid(), gettid(), pthread_self());
    printf("number [ %d ], str [ %s ]\n", object->number, object->str);
}

int main() {
    pthread_t tid;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    Object* object = malloc(SIZE * sizeof(Object));
    object->number = 5;
    object->str = "hello_world";

    Object object1;
    object1.number = 5;
    object1.str = "hello world";
    
    printf("main [%d %d %d]: Hello from main!\n", getpid(), getppid(), gettid());
    int err = pthread_create(&tid, &attr, (void*)(*my_thread), object);
    pthread_detach(err);

    if (err) {
        printf("main: pthread_create failed: %s\n", strerror(err));
        free(object);
        return -1;
    }

    pthread_join(tid, NULL);
    pthread_attr_destroy(&attr);
    sleep(3);
    printf("%d\n", object1.number);
    free(object);

    return 0;
}