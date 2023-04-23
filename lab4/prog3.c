#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <signal.h>

#define SIZE 4096

void allocate_stack(int depth) {
    char buffer[SIZE];
    printf("Stack depth: %d\n", depth);
    allocate_stack(depth + 1);
}

void allocate_heap(int size) {
    char *buffer = (char *) malloc(size);
    printf("Heap allocation: %p\n", buffer);
    sleep(1);
    free(buffer);
}

void sigsegv_handler(int signal) {
    printf("Caught SIGSEGV\n");
    exit(EXIT_FAILURE);
}

int main() {
    printf("PID: %d\n", getpid());
    sleep(10);
    allocate_stack(0);
    for (int i = 0; i < 10; i++) {
        char *buffer = (char *) malloc(1024 * 8 * 10); // Allocate 10 KB
        printf("Heap allocation: %p\n", buffer);
        sleep(1);
        free(buffer);
    }
    void *map = mmap(NULL, 10 * getpagesize(), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    printf("Mapped memory: %p\n", map);
    sleep(10);
    int result = mprotect(map, 10 * getpagesize(), PROT_EXEC);
    if (result != 0) {
        printf("mprotect error: %d\n", result);
    }
    result = mprotect(map, 10 * getpagesize(), PROT_READ);
    if (result != 0) {
        printf("mprotect error: %d\n", result);
    }
    char *ptr = (char *) map;
    ptr[0] = 'A'; // Should not cause SIGSEGV
    result = mprotect(map, 10 * getpagesize(), PROT_WRITE);
    if (result != 0) {
        printf("mprotect error: %d\n", result);
    }
    ptr[1] = 'B'; // Should cause SIGSEGV
    signal(SIGSEGV, sigsegv_handler);
    result = mprotect(map + 3 * getpagesize(), 3 * getpagesize(), PROT_NONE);
    if (result != 0) {
        printf("mprotect error: %d\n", result);
    }
    sleep(10);
    munmap(map, 10 * getpagesize());
    return 0;
}
