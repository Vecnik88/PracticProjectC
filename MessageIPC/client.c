/* Client */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <sys/msg.h>


#define MAX_TEXT 512					// <---. размер буфера

struct my_msg_st
{
	long int my_msg_type;	
	char some_text[MAX_TEXT];			// <---. текст сообщения
	char UserName[MAX_TEXT];			// <---. имя пользователя отправляющего сообщения
};

int main()
{
	int running = 1;					// <---. флаг
	int messageSize = 0;				// <---. размер отправляемого сообщения
	int msgid;							// <---. id 
	key_t key = ftok(".", 'a');			// <---. key

	struct my_msg_st some_data;
	some_data.my_msg_type = 1;			// <---. приоритет сообщения

	printf("Enter your name: ");
	fgets(some_data.UserName, MAX_TEXT, stdin);
	some_data.UserName[strlen(some_data.UserName) - 1] = '\0'; 		// <---. удаляем символ новой строки

	msgid = msgget(key, 0666 | IPC_CREAT);
	if(msgid == -1)
	{
		fprintf(stderr, "msgget failed with error: %d\n", errno);
		exit(EXIT_FAILURE);
	}

	while(running)
	{
		memset(some_data.some_text, '\0', strlen(some_data.some_text));
		printf("Enter your message: ");
		fgets(some_data.some_text, MAX_TEXT, stdin);
		
		messageSize = sizeof(some_data.some_text) + sizeof(some_data.UserName);

		if(msgsnd(msgid, (void*)&some_data, messageSize, 0) == -1)
		{
			fprintf(stderr, "msgsnd failed\n");
			exit(EXIT_FAILURE);	
		}
		if(strncmp(some_data.some_text, "end", 3) == 0)				// <---. завершает клиентскую программу
		{
			running = 0;
		}
	}

	exit(EXIT_SUCCESS);
}
