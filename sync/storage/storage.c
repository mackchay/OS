#include "storage.h"
#include "stdlib.h"
#include "string.h"
#include <time.h>
#include <stdio.h>
#define MAX_STR_SIZE 100


void generate_random_string(char *str, int length) {
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    size_t charsetLength = strlen(charset);
    srand(time(NULL));
    for (int i = 0; i < length; ++i) {
        size_t index = rand() % charsetLength;
        str[i] = charset[index];
    }
    if (length < MAX_STR_SIZE) {
        str[length] = '\0';
    }
}

Storage *storage_init(int size) {
    Storage *storage = malloc(sizeof(Storage));
    storage->len = 0;
    char str[MAX_STR_SIZE];
    for (int i = 0; i < size; i++) {
        if (i % MAX_STR_SIZE > 0) {
            generate_random_string(str, i % MAX_STR_SIZE);
        } else {
            generate_random_string(str, 1);
        }
        storage_add_elem(storage, str);
    }
    return storage;
}

Node *storage_get_elem(Storage *storage, int key) {

    Node *cur = storage->first;
    pthread_spin_lock(&cur->sync);
    while (cur != NULL && cur->key != key) {
        pthread_spin_unlock(&cur->sync);
        cur = cur->next;
        if (cur != NULL) {
            pthread_spin_lock(&cur->sync);
        }
    }
    if (cur != NULL) {
        pthread_spin_unlock(&cur->sync);
    }
    return cur;
}

Node *node_init(char *str) {
    Node *node = malloc(sizeof(Node));
    strcpy(node->value, str);
    node->next = NULL;
    pthread_spin_init(&node->sync, 0);
    return node;
}

Node *storage_add_elem(Storage *storage, char *str) {
    Node *cur = storage->first;
    Node *node = node_init(str);
    
    if (cur == NULL) {
        storage->first = node;
        node->key = 0;
        storage->len++;
        return node;
    }
    while (cur->next) {
        cur = cur->next;
    }
    cur->next = node;
    node->key = cur->key + 1;
    storage->len++;
    return node;
}

Node *storage_insert_elem(Storage *storage, int key, char* str) {
    Node *elem = storage_get_elem(storage, key - 1);
    if (!elem) {
        return NULL;
    }

    Node *node = malloc(sizeof(Node));
    node->key = key;
    strcpy(node->value, str);

    node->next = elem->next;
    elem->next = node;
    (storage->len)++;
    return node;
}

// void storage_delete_elem(Storage *storage, int key) {
//     Node *node = malloc(sizeof(Node));
    
//     Node *elem = storage_get_elem(storage, key - 1);
//     if (!elem || !elem->next) {
//         return;
//     }
//     Node *tmp = elem->next;
//     elem->next = elem->next->next;
//     free(tmp);
// }

void storage_adjacent_swap(Storage *storage, int key) {
    Node *prev = storage_get_elem(storage, key - 1);
    Node *elem = storage_get_elem(storage, key);

    if (!elem || !elem->next) {
        printf("Nodes are not found.");
        return;
    }


    pthread_spin_lock(&elem->sync);

    Node* next_node = elem->next;

    pthread_spin_lock(&next_node->sync);
    if (prev != NULL) {
        pthread_spin_lock(&prev->sync);
        prev->next = next_node;
        pthread_spin_unlock(&prev->sync);
    } else {
        pthread_spin_lock(&storage->first->sync);
        storage->first = next_node;
        pthread_spin_unlock(&storage->first->sync);
    }
    elem->next = next_node->next;
    next_node->next = elem;

    int temp = elem->key;
    elem->key = next_node->key;
    next_node->key = temp;

    pthread_spin_unlock(&elem->sync);
    pthread_spin_unlock(&next_node->sync);
}

void node_destroy(Node *node) {
    pthread_spin_destroy(&node->sync);
    free(node);
}

void storage_destroy(Storage *storage) {
    Node *cur = storage->first;
    while (cur) {
        Node *tmp = cur->next;
        node_destroy(cur);
        cur = tmp;
    }
}

