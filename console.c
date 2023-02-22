/* @brief: travail Pratique sur les sémaphores/mutex
   @date: 05/03/2023
   @author: Soufiane EL KHARMOUDI
*/

#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>
#include <errno.h>
#include "thread_wrapper.h"
#include <semaphore.h>
#include <pthread.h>


#define NB_CONSOLE 1
#define CONSOLE_LIBRE 0
#define NB_PLAYER 8
#define NB_SIEGE 8
#define CHF 100

typedef struct
{
	int count;
	pthread_mutex_t *lock;
	sem_t *sem;
} barrier_t;

static barrier_t barrier;

static void barrier_init(barrier_t *b, int count)
{
	b->count = count;
	b->lock = mutex_create();
	b->sem = sema_create(0);
}

static bool siegesLibre(int count)
{
	if (barrier.count < NB_SIEGE)
		return true;

	return false;
}

static void barrier_wait(barrier_t *b)
{
	mutex_lock(b->lock);
	b->count--;
	if (b->count == 0)
		sema_post(b->sem);
	mutex_unlock(b->lock);

	sema_wait(b->sem);
	sema_post(b->sem);
}

static void barrier_destroy(barrier_t *b)
{
	mutex_destroy(b->lock);
	sema_destroy(b->sem);
}

/// @brief 8 ms dans le simulateur
/// @param arg
/// @return
static void *compute(void *arg)
{
	int id = *((int *)arg);
	int delay = rand() % 8 + 1;

	printf("Thread %d started computation for %d usec...\n", id, delay);
	usleep(delay);
	printf("Thread %d finished computation\n", id);

	printf("Thread %d waiting at barrier...\n", id);
	barrier_wait(&barrier);
	printf("Thread %d passed the barrier!\n", id);
	return NULL;
}

int main()
{
	const int thread_count = 8;
	srand(time(NULL));
	barrier_init(&barrier, thread_count + 1);

	pthread_t t[thread_count];
	int id[thread_count];
	for (int i = 0; i < thread_count; i++)
	{
		id[i] = i;
		thread_create(&t[i], compute, &id[i]);
	}

	puts("Thread main waiting at barrier...");
	barrier_wait(&barrier);
	puts("Thread main passed the barrier!");

	for (int i = 0; i < thread_count; i++)
		thread_join(t[i], NULL);

	barrier_destroy(&barrier);

	printf("Program terminated.\n");
	return EXIT_SUCCESS;
}
