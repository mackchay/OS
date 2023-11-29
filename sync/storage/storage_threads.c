#include "storage.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define SIZE 1000

long count_asc_iter = 0;
long count_desc_iter = 0;
long count_equal_iter = 0;

long swap_dec_iter = 0;
long swap_equal_iter = 0;

_Noreturn void *count_asc(void *arg) {
    Storage *storage = (Storage*)arg;
    int pair_count;
    while (1) {
        Node *node = storage->first;
        pair_count = 0;
        while (node->next != NULL) {
            pthread_spin_lock(&node->sync);
            if (node->next != NULL) {
                pthread_spin_lock(&node->next->sync);
            } else {
                pthread_spin_unlock(&node->sync);
                break;
            }
            if (strlen(node->value) < strlen(node->next->value)) {
                pair_count++;
            }
            pthread_spin_unlock(&node->next->sync);
            pthread_spin_unlock(&node->sync);
            node = node->next;
        }
        count_asc_iter++;
        printf("count asc: %ld iteration completed with %d pairs\n", count_asc_iter, pair_count);
        sleep(1);
    }
}

_Noreturn void *count_desc(void *arg) {
    Storage *storage = (Storage*)arg;
    int pair_count;
    while (1) {
        Node *node = storage->first;
        pair_count = 0;
        while (node->next != NULL) {
            pthread_spin_lock(&node->sync);
            if (node->next != NULL) {
                pthread_spin_lock(&node->next->sync);
            } else {
                pthread_spin_unlock(&node->sync);
                break;
            }
            if (strlen(node->value) > strlen(node->next->value)) {
                pair_count++;
            }
            pthread_spin_unlock(&node->next->sync);
            pthread_spin_unlock(&node->sync);
            node = node->next;
        }
        count_desc_iter++;
        printf("count desc: %ld iteration completed with %d pairs\n", count_desc_iter, pair_count);
        sleep(1);
    }
}

_Noreturn void *count_equal(void *arg) {
    Storage *storage = (Storage*)arg;
    int pair_count;
    while (1) {
        Node *node = storage->first;
        pair_count = 0;
        while (node->next != NULL) {
            pthread_spin_lock(&node->sync);
            if (node->next != NULL) {
                pthread_spin_lock(&node->next->sync);
            } else {
                pthread_spin_unlock(&node->sync);
                break;
            }
            if (strlen(node->value) == strlen(node->next->value)) {
                pair_count++;
            }
            pthread_spin_unlock(&node->next->sync);
            pthread_spin_unlock(&node->sync);
            node = node->next;
        }
        count_equal_iter++;
        printf("count equal: %ld iteration completed with %d pairs\n", count_equal_iter, pair_count);
        sleep(1);
    }
}

_Noreturn void *swap_random_nodes(void *arg) {
    Storage *storage = (Storage *) arg;
    while (1) {
        Node *it_node = storage->first;
        while (it_node->next != NULL) {
            Node *node = it_node;
            pthread_spin_lock(&node->sync);
            if (node->next != NULL) {
                pthread_spin_lock(&node->next->sync);
            } else {
                pthread_spin_unlock(&node->sync);
                break;
            }

            if (node->next->next != NULL) {
                pthread_spin_lock(&node->next->next->sync);
            } else {
                pthread_spin_unlock(&node->sync);
                pthread_spin_unlock(&node->next->sync);
                break;
            }

            Node *tmp = node->next;
            node->next = tmp->next;
            tmp->next = node->next->next;
            node->next->next = tmp;

            it_node = it_node->next;
            pthread_spin_unlock(&node->next->next->sync);
            pthread_spin_unlock(&node->next->sync);
            pthread_spin_unlock(&node->sync);
        }
    }
}

int main(void) {
    pthread_t tid1, tid2, tid3;
    Storage *storage = storage_init(SIZE);

//    for (int i = 0; i < storage->len; i++) {
//        printf("%s\n", storage_get_elem(storage, i)->value);
//    }

    int err = pthread_create(&tid1, NULL, count_asc, (void *) storage);
    if (err) {
        printf("Error: can't create 'count_asc' thread.");
    }
    err = pthread_create(&tid2, NULL, count_desc, (void *) storage);
    if (err) {
        printf("Error: can't create 'count_desc' thread.");
    }
    err = pthread_create(&tid3, NULL, count_equal, (void *) storage);
    if (err) {
        printf("Error: can't create 'count_equal' thread.");
    }

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    storage_destroy(storage);
    return 0;
}