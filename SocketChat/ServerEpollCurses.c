#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <errno.h>
#include <fcntl.h>

int set_nonblock(int fd);

#define MAX_EVENTS 32
#define BUFSIZE 1024

int main(int argc, char const *argv[])
{
	int i, N;								// <---. i - счетчик, N - кол-во активных дескрипторов
	int Socket, SlaveSocket;
	int RecvResult;
	char buffer[BUFSIZE];

	Socket = socket(PF_INET, SOCK_STREAM, 0);

	struct sockaddr_in server;
	server.sin_family = PF_INET;
	server.sin_port = htons(12345);
	server.sin_addr.s_addr = htonl(INADDR_ANY);

	bind(Socket, (struct sockaddr*)&server, sizeof(server));
	listen(Socket, SOMAXCONN);

	int Epoll = epoll_create1(0);
	struct epoll_event Event, Events[MAX_EVENTS];
	Event.data.fd = Socket;
	Event.events = EPOLLIN;
	epoll_ctl(Epoll, EPOLL_CTL_ADD, Socket, &Event);
	
	while(1)
	{
		N = epoll_wait(Epoll, Events, MAX_EVENTS, -1);
		
		for(i = 0; i < N; ++i)
		{
			if(Events[i].data.fd == Socket)
			{
				SlaveSocket = accept(Socket, NULL, NULL);
				set_nonblock(SlaveSocket);
				struct epoll_event Event;
				Event.data.fd = SlaveSocket;
				Event.events = EPOLLIN;
				epoll_ctl(Epoll, EPOLL_CTL_ADD, SlaveSocket, &Event);
			}
			else
			{
				RecvResult = recv(Events[i].data.fd, buffer, BUFSIZE, MSG_NOSIGNAL);
				if((RecvResult == 0) && (errno != EAGAIN))
				{
					shutdown(Events[i].data.fd, SHUT_RDWR);
					close(Events[i].data.fd);
				}
				else if(RecvResult > 0)
				{	
					for(i = 0; i < N; ++i)
						send(Events[i].data.fd, buffer, BUFSIZE, MSG_NOSIGNAL);
				}
			}

		}
	}

	return 0;
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