// Created by Vecnik88 3.04.2017

/*		Server

		Реализуйте чат(между процессами) с общей комнатой с использованием очередей сообщений.
		
					Реализован простой чат ввиде клиент серверной программы,
	 				сервер принимает сообщения и выводит их на общий экран 

*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <sys/msg.h>

#define MAX_TEXT 512

struct my_msg_st
{
	long int my_msg_type;	
	char some_text[MAX_TEXT];
	char UserName[MAX_TEXT];
};

int main()
{
	int running = 1;
	int msgid;
	key_t key = ftok(".", 'a');

	struct my_msg_st some_data;
	long int ms_to_receive = 0;

	msgid = msgget(key, 0666 | IPC_CREAT);
	if(msgid == -1)
	{
		fprintf(stderr, "msgget failed with error: %d\n", errno);
		exit(EXIT_FAILURE);
	}

	printf("Server RUN...\n");
	while(running)
	{
		if(msgrcv(msgid, (void*)&some_data, BUFSIZ, ms_to_receive, 0) == -1)
		{
			fprintf(stderr, "msgrcv failed with error: %d\n", errno);
			exit(EXIT_FAILURE);
		}

		printf("%s: %s", some_data.UserName, some_data.some_text);
	}

	if(msgctl(msgid, IPC_RMID, 0) == -1)
	{
		fprintf(stderr, "msgctl(IPC_RMID) failed\n");
		exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);
}