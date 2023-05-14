#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
    printf("PID: %d\n", getpid());

    sleep(1);

    char* args[] = {"./lab4", NULL};
    execv(args[0], args);

    printf("Hello world\n");

    return 0;
}