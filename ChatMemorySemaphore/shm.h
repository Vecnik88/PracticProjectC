#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <sys/shm.h>
#include <sys/sem.h>

#define TEXT_SZ 2048					// <---. размер буфера
	
static int set_semvalue();
static int del_semvalue();
static int semaphore_p();
static int semaphore_v();

struct shared_use_st					// <---. структура под память
{
	int flag;
	char some_text[TEXT_SZ];
};

union semun								// <---. для семафора
{
	int val;
	struct semid_ds* buf;
	unsigned short* array;
};

