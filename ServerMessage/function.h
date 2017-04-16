/*
		Слушающий сервер принимает запрос от клиента и располагает его в очереди запросов. 
		Обслуживающие потоки сервера (например три потока) изымают запрос (например напросится текущего времени) 
		и удовлетворив его располагают ответ в очереди ответов. 
		Слушающий сервер изымает ответ и посылает его клиенту. 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/msg.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/poll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BUFFER_SIZE 1024

struct data_message					// <---. структура сообщения
{
	char some_text[200];			// <---. текстовое сообщение
	int fd_client;					// <---. дескриптор клиента
};

struct message
{
	long int id_message;
	struct data_message client;
};

void* serverTCP();					// <---. ф-ция потока сервера
void* thread_1();					// <---. обслуживающий поток 1
void* thread_2();					// <---. обслуживающий поток 2
void infoComputer(char* str);		// <---. информация о пользователе компьютера
void errorMessage(char* str);		// <---. сообщение об ошибке
int set_nonblock(int fd);			// <---. делает сокет неблокирующим