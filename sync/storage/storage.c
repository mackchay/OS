#include "storage.h"
#include "stdlib.h"
#include "string.h"
#include <time.h>

Storage *storage_init() {
    Storage *storage = malloc(sizeof(Storage));
    storage->len = 0;
    storage->first = NULL;
}

Node *storage_get_elem(Storage *storage, int key) {
    Node *cur = storage->first;
    while (cur && cur->key != key) {
        cur = cur->next;
    }
    return cur;
}

Node *storage_add_elem(Storage *storage, char *str) {
    Node *cur = storage->first;
    Node *node = malloc(sizeof(Node));
    strcpy(node->value, str);
    if (!cur) {
        storage->first = node;
    }
    while (cur->next) {
        cur = cur->next;
    }
    cur->next = node;
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

void storage_swap(Storage *storage, int key1, int key2) {
    Node *prev_elem1 = storage_get_elem(storage, key1 - 1);
    Node *elem1 = prev_elem1->next;
    Node *prev_elem2 = storage_get_elem(storage, key2 - 1);
    Node *elem2 = prev_elem2->next;
    if (!elem1 || !elem2) {
        printf("Nodes are not found.")
        return;
    }
    if (prev_elem1) {
        prev_elem1->next = elem1;
    } else {
        storage->first = elem1;
    }

    if (prev_elem2) {
        prev_elem2->next = elem2;
    } else {
        storage->first = elem2;
    }

    Node *tmp = elem1->next;
    elem1->next = elem2->next;
    elem2->next = tmp;

}