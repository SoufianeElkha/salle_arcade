/* Description: solution of exercise 6 (semaphores)
   Date: April 2022
   Author: V. Pilloux
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <assert.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdbool.h>

#define NB_REQ  200
#define DELAY_B 1000
#define DELAY_C 2000

pthread_mutex_t mutex;
sem_t sem_a_b, sem_b_c, sem_c_b;
int nb_req_a=0;


void *task_A()
{
	bool end=false;

	do
	{	  
		pthread_mutex_lock(&mutex);
		if (nb_req_a >=NB_REQ)
			end=true;
		else
		{
			sem_post(&sem_a_b);
			nb_req_a++;
		}
		printf("a: nb_req_a=%d\n", nb_req_a);
		pthread_mutex_unlock(&mutex);
		usleep(rand()%20000);
  } while(!end);
	return NULL;
}

void *task_B()
{
	int i, j;
	
	for (i=0; i<NB_REQ/10; i++)
	{
		for (j=0; j<10; j++)
		{
			sem_wait(&sem_a_b);
			usleep(DELAY_B);			
		}
		printf("b: i=%d\n", i);
		sem_post(&sem_b_c);
		sem_wait(&sem_c_b);
	}
	return NULL;
}

void *task_C()
{
	for (int i=0; i<NB_REQ/10; i++)
	{
	  sem_wait(&sem_b_c);
	  usleep(DELAY_C);
	  sem_post(&sem_c_b);
	  printf("c: i=%d\n", i);
  }
	return NULL;
}

int main() 
{
	int i;
	pthread_t task_A_h[NB_REQ/10], task_B_h, task_C_h;
	
	assert(pthread_mutex_init(&mutex, NULL)==0);
	assert(sem_init(&sem_a_b, 0, 0)==0);
	assert(sem_init(&sem_b_c, 0, 0)==0);
	assert(sem_init(&sem_c_b, 0, 0)==0);
	srand(time(NULL));
 
	assert(pthread_create(&task_B_h, NULL, task_B, NULL)==0);
	assert(pthread_create(&task_C_h, NULL, task_C, NULL)==0);
	for (i=0; i<NB_REQ/10; i++)
		assert(pthread_create(&task_A_h[i], NULL, task_A, NULL)==0);

	assert(pthread_join(task_B_h, NULL)==0);
	for (i=0; i<NB_REQ/10; i++)
		assert(pthread_join(task_A_h[i], NULL)==0);
	assert(pthread_join(task_C_h, NULL)==0);

	assert(pthread_mutex_destroy(&mutex)==0);
	assert(sem_destroy(&sem_a_b)==0);
	assert(sem_destroy(&sem_b_c)==0);
	assert(sem_destroy(&sem_c_b)==0);
	printf("Program completed\n");
}

