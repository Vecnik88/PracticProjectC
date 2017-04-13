/* ### ЗАДАНИЕ ### */

/*
	Hапишите сервер который будет обслуживать каждые 5 клиентов в отдельном потоке 
	и механизм обслуживания описать конечным детерминированным автоматом (FSM).
*/

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <sys/poll.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

/* макросы состояний для FSM */
#define INIT 0
#define START_REQ 1
#define GET_REQ 2
#define SEND_REQ 3
#define END_REQ 4

#define SIZEBUF 1024

int set_nonblock(int fd);			// <---. делает сокет неблокирующим
int inputSocket();			        // <---. извлекает нужный сокет для отправки

void resultOperation();						        // <---. удаляет сокет, помечает его исполненным
void readMaterial(char* buffer, int fd);	// <---. считывает данные из сокета
void sendMaterial(char* buffer, int fd);	// <---. отправляет данные сокет
void messageError(char* str);			// <---. сообщение ошибки

void* workTCP();			       	// <---. рабочая функция сервера TCP
void* ServerUDP();				// <---. сервер UDP в отдельном потоке
void* ServerTCP();				// <---. сервер ТСР в отдельном потоке
