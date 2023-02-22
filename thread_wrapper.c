#include <error.h>
#include <errno.h>
#include <stdlib.h>
#include "thread_wrapper.h"

#define ERR_CODE 1

pthread_mutex_t *mutex_create() {
	pthread_mutex_t *mutex = malloc(sizeof(pthread_mutex_t));
	if (!mutex) error(ERR_CODE, ENOMEM, "malloc failed!");
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK);
	int status = pthread_mutex_init(mutex, &attr);
	if (status) error(ERR_CODE, status, "mutex_create failed!");
	pthread_mutexattr_destroy(&attr);
	return mutex;
}

void mutex_lock(pthread_mutex_t *mutex) {
	int status = pthread_mutex_lock(mutex);
	if (status) error(ERR_CODE, status, "mutex_lock failed!");
}

void mutex_unlock(pthread_mutex_t *mutex) {
	int status = pthread_mutex_unlock(mutex);
	if (status) error(ERR_CODE, status, "mutex_unlock failed!");
}

void mutex_destroy(pthread_mutex_t *mutex) {
	int status = pthread_mutex_destroy(mutex);
	if (status) error(ERR_CODE, status, "mutex_destroy failed!");
	free(mutex);
}

sem_t *sema_create(int count) {
	sem_t *sem = malloc(sizeof(sem_t));
	if (!sem) error(ERR_CODE, ENOMEM, "malloc failed!");
	int status = sem_init(sem, 0, count);
	if (status) error(ERR_CODE, status, "sem_create_failed!");
	return sem;
}

void sema_post(sem_t *sem) {
	int status = sem_post(sem);
	if (status) error(ERR_CODE, status, "sem_post failed!");
}

void sema_wait(sem_t *sem) {
	int status = sem_wait(sem);
	if (status) error(ERR_CODE, status, "sem_wait failed!");
}

void sema_destroy(sem_t *sem) {
	int status = sem_destroy(sem);
	if (status) error(ERR_CODE, status, "sem_destroy failed!");
	free(sem);
}

pthread_barrier_t *bar_create(int count) {
	pthread_barrier_t *b = malloc(sizeof(pthread_barrier_t));
	int status = pthread_barrier_init(b, NULL, count);
	if (status) error(ERR_CODE, status, "bar_init failed!");
	return b;
}

void bar_destroy(pthread_barrier_t *b) {
	int status = pthread_barrier_destroy(b);
	if (status) error(ERR_CODE, status, "bar_destroy failed!");
	free(b);
}

void bar_wait(pthread_barrier_t *b) {
	int status = pthread_barrier_wait(b);
	if (status != PTHREAD_BARRIER_SERIAL_THREAD && status != 0) error(ERR_CODE, status, "bar_wait failed!");
}

void thread_create(pthread_t *thread, void *(*start_routine)(void *), void *arg) {
	int status = pthread_create(thread, NULL, start_routine, arg);
	if (status) error(ERR_CODE, status, "thread_create failed!");
}

void thread_join(pthread_t thread, void **retval) {
	int status = pthread_join(thread, retval);
	if (status) error(ERR_CODE, status, "thread_join failed!");
}

