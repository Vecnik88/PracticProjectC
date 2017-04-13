#include "function.h"

pthread_mutex_t mutexTCP;					// <---. mutex для блокировки
int pull[5];							// <---. сокеты, которые необходимо обработать
int k = 0;					  	      	// <---. отвечает за элементы в пуле

int state = INIT;						// <---. переменная для FSM

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
					k++;
					state = START_REQ;
				
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
		switch(state)
		{
			case START_REQ:
				fd = inputSocket();
			case GET_REQ:
				readMaterial(buffer, fd);
			case SEND_REQ:
				sendMaterial(buffer, fd);
			case END_REQ:
				resultOperation();
			case INIT:
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
			state = SEND_REQ;
			break;
	}
}

void sendMaterial(char* buffer, int fd)
{
	send(fd, buffer, SIZEBUF, MSG_NOSIGNAL);
	state = END_REQ;
}

int inputSocket()
{
	state = GET_REQ;
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
		state = INIT;
	else
		state = START_REQ;

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
