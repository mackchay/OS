#define _GNU_SOURCE
#include <pthread.h>
#include <assert.h>
#include <linux/futex.h>
#include <syscall.h>
#include <semaphore.h>

#include "queue.h"

pthread_spinlock_t spinlock;
pthread_mutex_t mutex;
sem_t semaphore;
pthread_cond_t condition;
volatile int condition_value = 1;

void spinlock_init(spinlock_t *lock) {
    lock->is_locked = 0;
}

void spinlock_lock(spinlock_t *lock) {
	while (__sync_lock_test_and_set(&lock->is_locked, 1)) {
        while (lock->is_locked) {

        }
    }
}

void spinlock_unlock(spinlock_t *lock) {
	__sync_lock_release(&lock->is_locked);
}


void *qmonitor(void *arg) {
	queue_t *q = (queue_t *)arg;

	printf("qmonitor: [%d %d %d]\n", getpid(), getppid(), gettid());

	while (1) {
		queue_print_stats(q);
		sleep(1);
	}

	return NULL;
}

queue_t* queue_init(int max_count) {
	int err;
	pthread_mutex_init(&mutex, NULL);
	pthread_spin_init(&spinlock, 0);
	sem_init(&semaphore, 0, 1);
	pthread_cond_init(&condition, NULL);

	queue_t *q = malloc(sizeof(queue_t));
	if (!q) {
		printf("Cannot allocate memory for a queue\n");
		abort();
	}

	q->first = NULL;
	q->last = NULL;
	q->max_count = max_count;
	q->count = 0;

	q->add_attempts = q->get_attempts = 0;
	q->add_count = q->get_count = 0;

	err = pthread_create(&q->qmonitor_tid, NULL, qmonitor, q);
	if (err) {
		printf("queue_init: pthread_create() failed: %s\n", strerror(err));
		abort();
	}

	return q;
}

void queue_destroy(queue_t *q) {
	qnode_t *node = q->first;
	while (node) {
		node = node->next;
		free(q->first);
		q->first = node;
		q->count--;
	}
	printf("queue freed: count: %d\n", q->count);
	free(q);
	pthread_mutex_destroy(&mutex);
	pthread_spin_destroy(&spinlock);
	sem_destroy(&semaphore);
	pthread_cond_destroy(&condition);
}

int queue_add(queue_t *q, int val) {
	q->add_attempts++;
	
	assert(q->count <= q->max_count);

	if (q->count == q->max_count)
		return 0;

	qnode_t *new = malloc(sizeof(qnode_t));
	if (!new) {
		printf("Cannot allocate memory for new node\n");
		abort();
	}

	new->val = val;
	new->next = NULL;

	// pthread_spin_lock(&spinlock);
	// pthread_mutex_lock(&mutex);
	// while (condition_value == 0) {
	// 	pthread_cond_wait(&condition, &mutex);
	// }
	sem_wait(&semaphore);

	if (!q->first)
		q->first = q->last = new;
	else {
		q->last->next = new;
		q->last = q->last->next;
	}

	q->count++;
	q->add_count++;

	// pthread_spin_unlock(&spinlock);
	// condition_value = 0;
    // pthread_cond_signal(&condition);
	// pthread_mutex_unlock(&mutex);
	sem_post(&semaphore);

	return 1;
}

int queue_get(queue_t *q, int *val) {
	q->get_attempts++;

	assert(q->count >= 0);

	if (q->count == 0)
		return 0;

	// pthread_spin_lock(&spinlock);
	// pthread_mutex_lock(&mutex);
	// while (condition_value == 1) {
	// 	pthread_cond_wait(&condition, &mutex);
	// }
	sem_wait(&semaphore);
	qnode_t *tmp = q->first;
	
	*val = tmp->val;
	q->first = q->first->next;

	free(tmp);
	q->count--;
	q->get_count++;

	// pthread_spin_unlock(&spinlock);
	// condition_value = 1;
    // pthread_cond_signal(&condition);
	// pthread_mutex_unlock(&mutex);
	sem_post(&semaphore);

	return 1;
}

void queue_print_stats(queue_t *q) {
	printf("queue stats: current size %d; attempts: (%ld %ld %ld); counts (%ld %ld %ld)\n",
		q->count,
		q->add_attempts, q->get_attempts, q->add_attempts - q->get_attempts,
		q->add_count, q->get_count, q->add_count -q->get_count);
}

