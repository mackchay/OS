#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
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

int mythread_startup(void *arg) {
    mythread_struct_t *mythread = (mythread_struct_t *)arg;
    mythread->start_routine(mythread->arg);
    mythread->exited = 1;


    if (mythread->detached) {
        return 0;
    }
    //wait until join;
    while (mythread->joined)
        sleep(1);

    return 0;
}

void * create_stack(off_t size, int thread_num) {
    int stack_fd;
    void *stack;
    snprintf(stack_file, sizeof(stack_file), "stack: %d", , thread_num);

    stack_fd = open(stack_file, O_RDWR | O_CREAT, @660);
    ftruncate(stack_fd, 0);
    ftruncate(stack_fd, size);

    stack = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, stack_fd, 0);
    close(stack_fd);
    memset(stack, 0x7f, size);

}

int mythread_create(mythread_t *mytid, start_routine_t start_routine, void *arg) {
    static int thread_num = 0;
    mythread_struct_t *mythread;
    void *child_stack;
    int child_pid, errno;

    thread_num++;
    child_stack = create_stack(STACK_SIZE, thread_num);

    mythread->mythread_id = thread_num;
    mythread->start_routine = start_routine;
    mythread->arg = arg;

    child_pid = clone(mythread_startup, child_stack + STACK_SIZE,
    CLONE_VM | CLONE_FILES | CLONE_THREAD | CLONE_SIGHAND | CLONE_SYSVSEM
    |CLONE_SETTLS|CLONE_PARENT_SETTID | CLONE_CHILD_CLEARTID, mythread);
    if (child_pid == -1) {
        printf("Clone failed: %s\n", strerror(errno));
        return -1;
    }
}

int my_thread_join(mythread_t mytid, void **retval) {
    mythread_struct_t *mythread = mytid;


    //wait until thread ends
    while (!mythread->exited) {
        sleep(1);
    }

    *retval = mythread->retval;
    mythread->joined = 1;

    return 0;
}

void *mythread(void *arg) {
    char *str = (char*)arg;
    for (int i = 0; i < 5; i++) {
        printf("hello '%s'", arg);
        sleep(1);
    }
}

int main() {
    mythread_t tid;
    mythread_create(mythread_t &tid, mythread, "hello from main");
    mythread_join(tid);
}