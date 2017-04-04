// Created by Vecnik88 4.04.2017

/*		*** ### Server ### ***
		
		Cделайте чат, на разделяемой памяти и семафорах.
*/

#include "shm.h"

static int idSemaphore;

int main(int argc, char const *argv[])
{
	int running = 1;
	void* shared_memory = (void*) 0;
	struct shared_use_st *shared_stuff;
	int idMemory;

	key_t key = ftok(".", 'a');

	idMemory = shmget(key, sizeof(struct shared_use_st*), 0660 | IPC_CREAT);
	idSemaphore = semget(key, 1, 0660 | IPC_CREAT);
	set_semvalue();
	if(idMemory == -1)
	{
		fprintf(stderr, "shmget failed\n");
		exit(EXIT_FAILURE);
	}

	shared_memory = shmat(idMemory, (void*) 0, 0);
	shared_stuff = (struct shared_use_st *) shared_memory;
	
	while(running)
	{
		semaphore_p();
		if(shared_stuff->flag == 1)
		{	
			if(strncmp(shared_stuff->some_text, "end", 3) == 0)
				break;

			printf("You wrote: %s\n", shared_stuff->some_text);
			shared_stuff->flag = 0;
		}
		semaphore_v();
	}

	if(shmdt(shared_memory) == -1)
	{
		fprintf(stderr, "shmdt failed\n");
		exit(EXIT_FAILURE);
	}

	del_semvalue();

	if(shmctl(idMemory, IPC_RMID, 0) == -1)
	{
		fprintf(stderr, "shmctl failed\n");
		exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);
}

static int set_semvalue()
{
	union semun sem_union;

	sem_union.val = 1;
	if(semctl(idSemaphore, 0, SETVAL, sem_union) == -1)
		return 0;

	return 1;
}

static int del_semvalue()
{
	union semun sem_union;
	if(semctl(idSemaphore, 0, IPC_RMID, sem_union) == -1)
		fprintf(stderr, "Failed to delete semaphore");
}

static int semaphore_p()
{
	struct sembuf sem_b;

	sem_b.sem_num = 0;
	sem_b.sem_op = -1;
	sem_b.sem_flg = SEM_UNDO;

	if(semop(idSemaphore, &sem_b, 1) == -1)
	{
		fprintf(stderr, "Semaphhore_p failed");
		return 0;
	} 

	return 1;
}

static int semaphore_v()
{
	struct sembuf sem_b;

	sem_b.sem_num = 0;
	sem_b.sem_op = 1;
	sem_b.sem_flg = SEM_UNDO;

	if(semop(idSemaphore, &sem_b, 1) == -1)
	{
		fprintf(stderr, "Semaphhore_v failed");
		return 0;
	} 

	return 1;
}