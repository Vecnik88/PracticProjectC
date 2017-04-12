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

#define SIZEBUF 1024

int set_nonblock(int fd);					// <---. делает сокет неблокирующим
int inputSocket();							// <---. извлекает нужный сокет для отправки

void resultOperation();						// <---. удаляет сокет, помечает его исполненным
void readMaterial(char* buffer, int fd);	// <---. считывает данные из сокета
void sendMaterial(char* buffer, int fd);	// <---. отправляет данные сокет
void messageError(char* str);				// <---. сообщение ошибки

void* workTCP();							// <---. рабочая функция сервера TCP
void* ServerUDP();							// <---. сервер UDP в отдельном потоке
void* ServerTCP();							// <---. сервер ТСР в отдельном потоке

int pull[5];								// <---. сокеты, которые необходимо обработать
int flag = 0;								// <---. флаг, если он 0 - pull пустой
static int k = 0;							// <---. отвечает за элементы в пуле

pthread_mutex_t mutexTCP;					// <---. mutex для блокировки

int main(int argc, char const *argv[])
{
	pthread_t threadTCP, threadUDP;			// <---. потоки для UDP u ТСР серверов

	pthread_create(&threadTCP, NULL, ServerTCP, NULL);
	pthread_create(&threadUDP, NULL, ServerUDP, NULL);
	pthread_join(threadTCP, NULL);
	pthread_join(threadUDP, NULL);

	pthread_exit(NULL);
}

void* ServerTCP()
{
	int socketTCP;
	int i, ndfs = 1;
	int newfd;
	pthread_t thread;
	struct sockaddr_in serverTCP;
	struct pollfd fds[SIZEBUF];

	socketTCP = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	serverTCP.sin_family = PF_INET;
	serverTCP.sin_port = htons(12345);
	serverTCP.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(socketTCP, (struct sockaddr*)&serverTCP, sizeof(serverTCP)) < 0)
		messageError("binding");

	bzero(pull, 5);
	listen(socketTCP, SOMAXCONN);

	pthread_create(&thread, NULL, workTCP, NULL);
	pthread_detach(thread);
	pthread_mutex_init(&mutexTCP, NULL);

	fds[0].fd = socketTCP;
	fds[0].events = POLLIN;
	set_nonblock(fds[0].fd);
	
	while(1)
	{
		poll(fds, ndfs, 0);

		for(i = 0; i < ndfs; ++i)
		{
			if(fds[i].revents & POLLIN)
			{
				if(fds[i].fd == socketTCP)
				{
					newfd = accept(socketTCP, NULL, NULL);
					fds[ndfs].fd = newfd;
					fds[ndfs].events = POLLIN;
					set_nonblock(fds[ndfs].fd);
					ndfs++;
				}
				else
				{
					/* чтобы одно событие не обрабатывалось неск раз и не более 5 клиентов */
					pthread_mutex_lock(&mutexTCP);
					if((fds[i].fd == pull[k-1]) || (k == 4))		
						{
							pthread_mutex_unlock(&mutexTCP);
							continue;
						}
					
					pull[k] = fds[i].fd;
					flag = 1;
					k++;
				
					pthread_mutex_unlock(&mutexTCP);
				}
			}
		}
	}

	pthread_mutex_destroy(&mutexTCP);
	pthread_exit(NULL);
}

void* ServerUDP()
{
	int socketUDP;
	struct sockaddr_in serverUDP, clientUDP;
	int sizeRead, sizeUDP;
	char buffer[SIZEBUF];

	socketUDP = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

	serverUDP.sin_family = PF_INET;
	serverUDP.sin_port = htons(1234);
	serverUDP.sin_addr.s_addr = htonl(INADDR_ANY);

	sizeUDP = sizeof(clientUDP);

	if(bind(socketUDP, (struct sockaddr*)&serverUDP, sizeUDP) < 0)
		messageError("binding");

	while(1)
	{
		sizeRead = recvfrom(socketUDP, buffer, SIZEBUF, 0,
				(struct sockaddr*)&clientUDP, &sizeUDP);
		buffer[sizeRead] = '\0';

		sendto(socketUDP, buffer, SIZEBUF, MSG_NOSIGNAL,
				(struct sockaddr*)&clientUDP, sizeUDP);
	}

}

void* workTCP()
{
	char buffer[SIZEBUF];
	int fd;

	while(1)
	{
		switch(flag)
		{
			case 0:
				break;
			case 1:
				fd = inputSocket();
				readMaterial(buffer, fd);
				sendMaterial(buffer, fd);
				resultOperation();
				break;
		}				
	}
}

void readMaterial(char* buffer, int fd)
{
	int sizeRead;

	sizeRead = recv(fd, buffer, SIZEBUF, MSG_NOSIGNAL);

	switch(sizeRead)
	{
		case 0:
			shutdown(fd, SHUT_RDWR);
			close(fd);
			break;

		default:
			buffer[sizeRead] = '\0';
			break;
	}
}

void sendMaterial(char* buffer, int fd)
{
	send(fd, buffer, SIZEBUF, MSG_NOSIGNAL);
}

int inputSocket()
{
	return pull[0];
}

void resultOperation()
{
	unsigned int i = 0;
	int sizePull;
	pthread_mutex_lock(&mutexTCP);

	sizePull = sizeof(pull)/sizeof(int);

	for(i = 0; i < sizePull; ++i)
	{
		if(i == sizePull - 1)
		{
			pull[i] = '\0';
			break;
		}
		else
		{
			pull[i] = pull[i+1];
		}
	}
	
	--k;
	
	if(k == 0)
		flag = 0;

	pthread_mutex_unlock(&mutexTCP);
}

int set_nonblock(int fd)
{
	int flags;
#if defined(O_NONBLOCK)
	if(-1 == (flags = fcntl(fd, F_GETFL,0)))
		flags = 0;
	return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
#else
	flags = 1;
	return ioctl(fd, FIOBIO, &flags);
#endif
}

void messageError(char* str)
{
	perror(str);
	exit(0);
}
