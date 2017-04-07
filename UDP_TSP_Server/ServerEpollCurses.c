#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdlib.h>

#define MAX_EVENTS 32
#define BUFSIZE 1024

int set_nonblock(int fd);
void* serverTCP();
void* serverUDP();
void messageError(char* str);

int main(int argc, char const *argv[])
{
	pthread_t thread1, thread2;

	pthread_create(&thread1, NULL, serverUDP, NULL);
	pthread_create(&thread2, NULL, serverTCP, NULL);

	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);

	printf("Server UDP and TCP end work. Bye user.\n");
	sleep(2);

	pthread_exit(NULL);
}

void* serverUDP()
{
	int socketUDP;
	int sizeRead;
	int sizeServerUDP;
	char buffer[BUFSIZE];

	if((socketUDP = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
		messageError("Openning SocketUDP");

	set_nonblock(socketUDP);

	struct sockaddr_in serverUDP;
	serverUDP.sin_family = PF_INET;
	serverUDP.sin_port = htons(1234);
	serverUDP.sin_addr.s_addr = (INADDR_ANY);

	sizeServerUDP = sizeof(serverUDP);

	if(bind(socketUDP, (struct sockaddr*)&serverUDP, sizeServerUDP) < 0)
		messageError("binding");

	while(1)
	{
		sizeRead = recvfrom(socketUDP, buffer, BUFSIZE, MSG_NOSIGNAL,
							(struct sockaddr*)&serverUDP, &sizeServerUDP);

		buffer[sizeRead] = '\0';

		sendto(socketUDP, buffer, sizeof(buffer), MSG_NOSIGNAL,
				(struct sockaddr*)&serverUDP, sizeServerUDP);
	}	

	pthread_exit(NULL);
}

void* serverTCP()
{
	int i, N;								// <---. i - счетчик, N - кол-во активных дескрипторов
	int SocketTCP, SlaveSocket;
	int sizeRead;
	char buffer[BUFSIZE];

	if((SocketTCP = socket(PF_INET, SOCK_STREAM, 0)) < 0)
		messageError("Openning SocketTCP");
	
	set_nonblock(SocketTCP);

	struct sockaddr_in server;
	server.sin_family = PF_INET;
	server.sin_port = htons(12345);
	server.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(SocketTCP, (struct sockaddr*)&server, sizeof(server)) < 0)
		messageError("binding");

	listen(SocketTCP, SOMAXCONN);

	int Epoll = epoll_create1(0);
	struct epoll_event Event, Events[MAX_EVENTS];
	Event.data.fd = SocketTCP;
	Event.events = EPOLLIN;
	epoll_ctl(Epoll, EPOLL_CTL_ADD, SocketTCP, &Event);
	
	while(1)
	{
		N = epoll_wait(Epoll, Events, MAX_EVENTS, -1);
		
		for(i = 0; i < N; ++i)
		{
			if(Events[i].data.fd == SocketTCP)
			{
				SlaveSocket = accept(SocketTCP, NULL, NULL);
				set_nonblock(SlaveSocket);
				struct epoll_event Event;
				Event.data.fd = SlaveSocket;
				Event.events = EPOLLIN;
				epoll_ctl(Epoll, EPOLL_CTL_ADD, SlaveSocket, &Event);
			}
			else
			{
				sizeRead = recv(Events[i].data.fd, buffer, BUFSIZE, MSG_NOSIGNAL);
				if((sizeRead == 0) && (errno != EAGAIN))
				{
					shutdown(Events[i].data.fd, SHUT_RDWR);
					close(Events[i].data.fd);
				}
				else if(sizeRead > 0)
				{	
					send(Events[i].data.fd, buffer, BUFSIZE, MSG_NOSIGNAL);
				}
			}

		}
	}

	pthread_exit(NULL);
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