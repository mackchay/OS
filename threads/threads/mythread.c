#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sched.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>
#include <signal.h>
#define PAGE 4096
#define STACK_SIZE PAGE*8
typedef void * (*start_routine_t)(void*);


typedef struct _mythread {
    int                 mythread_id;
    start_routine_t     start_routine;
    void*               arg;
    void*               retval;
    volatile int        joined;
    volatile int        exited;
    volatile int        detached;
} mythread_struct_t;

typedef mythread_struct_t* mythread_t;

int mythread_startup(void *arg) {
    mythread_struct_t *mythread = (mythread_struct_t *)arg;
    mythread->retval = mythread->start_routine(mythread->arg);
    mythread->exited = 1;


    if (mythread->detached) {
        return 0;
    }
    //wait until join;
    while (mythread->joined)
        sleep(1);

    return 0;
}

void *create_stack(off_t size, int thread_num) {
    int stack_fd;
    void *stack;

    char stack_file[128]; 
    snprintf(stack_file, sizeof(stack_file), "stack: %d", thread_num);

    stack_fd = open(stack_file, O_RDWR | O_CREAT, 0660);
    if (!stack_fd) {
        perror("Can't create stack");
    }
    ftruncate(stack_fd, 0);
    ftruncate(stack_fd, size);

    stack = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, stack_fd, 0);
    memset(stack, 0x7f, size);
    close(stack_fd);
    return stack;
}

int mythread_create(mythread_t *mytid, void *(*start_routine)(void*), void *arg) {
    static int thread_num = 0;

    void *child_stack;
    int child_pid;

    thread_num++;
    child_stack = create_stack(STACK_SIZE, thread_num);

    mythread_struct_t *mythread;
    mythread = (mythread_struct_t*)(child_stack + STACK_SIZE - sizeof(mythread_struct_t));
    mythread->mythread_id = thread_num;
    mythread->start_routine = start_routine;
    mythread->arg = arg;
    mythread->detached = 0;
    mythread->exited = 0;
    mythread->joined = 0;

    child_stack = (void *) mythread;

    child_pid = clone(mythread_startup, child_stack,
    CLONE_VM | CLONE_FILES | CLONE_SIGHAND | CLONE_THREAD | SIGCHLD, mythread);
    if (child_pid == -1) {
        printf("Clone failed \n");
        return -1;
    }

    *mytid = mythread; 

    return 0;
}

int mythread_join(mythread_t mytid, void **retval) {
    mythread_struct_t *mythread = mytid;
    //wait until thread ends
    while (!mythread->exited) {
        sleep(1);
    }

    if (retval) {
        *retval = mythread->retval;
    }
    mythread->joined = 1;

    return 0;
}

void *mythread(void *arg) {
    char *str = (char*)arg;
    for (int i = 0; i < 5; i++) {
        printf("hello: '%s'\n", str);
        sleep(1);
    }
    return NULL;
}

int main() {
    mythread_t tid;
    mythread_create(&tid, mythread, "hello from main");
    // mythread_create(&tid, mythread, "hello)");
    mythread_join(tid, NULL);
}