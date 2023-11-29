#include <pthread.h>


typedef struct Node {
    int key;
    char value[100];
    struct Node* next;
    pthread_spinlock_t sync;
} Node;

typedef struct Storage {
    Node *first;
    int len;
} Storage;

Storage *storage_init(int size);
Node *storage_add_elem(Storage *storage, char *str);
Node *storage_get_elem(Storage *storage, int key);
void storage_adjacent_swap(Storage *storage, int key);
void storage_destroy(Storage *storage);
