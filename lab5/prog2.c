#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int global_var = 10;

int main() {
    int local_var = 20;
    printf("global_var address: %p value: %d\n", &global_var, global_var);
    printf("local_var address: %p value: %d\n", &local_var, local_var);
    printf("process pid: %d\n", getpid());

    pid_t pid = fork();

    if (pid == -1) {
        printf("fork failed\n");
        exit(1);
    }
    if (pid == 0) {
        // child process
        printf("child process pid: %d, parent pid: %d\n", getpid(), getppid());
        printf("child global_var address: %p value: %d\n", &global_var, global_var);
        printf("child local_var address: %p value: %d\n", &local_var, local_var);
        global_var = 30;
        local_var = 40;
        printf("child modified global_var value: %d\n", global_var);
        printf("child modified local_var value: %d\n", local_var);
        exit(EXIT_SUCCESS);
    }
    else {
        // parent process
        sleep(30);
        printf("parent process pid: %d\n", getpid());
        printf("parent global_var address: %p value: %d\n", &global_var, global_var);
        printf("parent local_var address: %p value: %d\n", &local_var, local_var);
    }

    return 0;
}
