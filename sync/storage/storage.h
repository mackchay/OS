#include <pthread.h>


typedef struct _Node {
    int key;
    char value[100];
    struct _Node* next;
    pthread_mutex_t sync;
} Node;

typedef struct _Storage {
    Node *first;
    int len;
} Storage;

Node *storage_get_elem(Storage *storage, int key);
void storage_swap(Storage *storage, int key_1, int key_2);
