#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <signal.h>

#define SIZE 4096

void allocate_stack(int depth) {
    char buffer[SIZE];
    printf("Stack depth: %d\n", depth);
    sleep(3);
    allocate_stack(depth + 1);
}

void allocate_heap() {
    char *buffer;
    for (int i = 0; i < 10; i++) {
        buffer = (char *) malloc(1024 * 8 * 10); // Allocate 10 KB
        printf("Heap allocation: %p\n", buffer);
        sleep(2);
        free(buffer);
    }
}

void sigsegv_handler(int signal) {
    printf("Caught SIGSEGV: %d\n");
    exit(EXIT_FAILURE);
}

void add_pages() {
    void *map = mmap(NULL, 10 * getpagesize(), PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS,
                     -1, 0);
    printf("Mapped memory: %p\n", map);
    sleep(10);
    int result = mprotect(map, 10 * getpagesize(), PROT_EXEC);
    if (result != 0) {
        printf("mprotect error: %d\n", result);
    }
    signal(SIGSEGV, sigsegv_handler);
//    char *ptr = (char *) map;
//    char c = ptr[1];
//    ptr[1] = 'B';
    result = mprotect(map + 3 * getpagesize(), 3 * getpagesize(), PROT_READ);
    if (result != 0) {
        printf("mprotect error: %d\n", result);
    }
    sleep(10);
    munmap(map, 10 * getpagesize());
}

int main() {
    printf("PID: %d\n", getpid());
    sleep(10);
    allocate_stack(0);
//    allocate_heap();
    add_pages();
    return 0;
}
