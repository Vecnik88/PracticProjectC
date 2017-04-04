/* 
	*** ### Client *** ###
*/

#include "shm.h"

static int idSemaphore;

int main(int argc, char const *argv[])
{
	void* shared_memory = (void*) 0;
	struct shared_use_st *shared_stuff;
	char buffer[BUFSIZ];
	int shmid;
	key_t key = ftok(".", 'a');

	shmid = shmget(key, sizeof(struct shared_use_st*), 0660 | IPC_CREAT);
	if(shmid == -1)
	{
		fprintf(stderr, "shmget failed\n");
		exit(EXIT_FAILURE);
	}

	shared_memory = shmat(shmid, (void*) 0, 0);
	shared_stuff = (struct shared_use_st*)shared_memory;
	idSemaphore = semget(key, 1, 0660 | IPC_CREAT);
	set_semvalue();
	
	printf("Client run...\n");
	while(1)
	{
		printf("Enter this your text: ");
		fgets(buffer, BUFSIZ, stdin);
		semaphore_p();
		strcpy(shared_stuff->some_text, buffer);
		shared_stuff->flag = 1;
		semaphore_v();

		if(strncmp(buffer, "end", 3) == 0)
			break;
	}
	if(shmdt(shared_memory) == -1)
	{
		fprintf(stderr, "shmdt failed\n");
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