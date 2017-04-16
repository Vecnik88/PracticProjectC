#include "function.h"

void* serverTCP()
{
	int queue1;
	int nfds = 1;
	int newSocket;
	int socketTCP;
	int sizeRead;
	int i = 0;

	key_t key1;

	//char buffer[BUFFER_SIZE];

	struct message mes;
	struct sockaddr_in server;
	struct pollfd fds[BUFSIZ];

	key1 = ftok(".", 'a');
	mes.id_message = 1;

	if((queue1 = (msgget(key1, 0666 | IPC_CREAT))) == -1)
		errorMessage("msgget serverTCP");

	socketTCP = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	set_nonblock(socketTCP);

	server.sin_family = AF_INET;
	server.sin_port = htons(12345);
	server.sin_addr.s_addr = htonl(INADDR_ANY);

	if((bind(socketTCP, (struct sockaddr*)&server, sizeof(server))) == -1)
		errorMessage("binding");

	listen(socketTCP, SOMAXCONN);

	fds[0].fd = socketTCP;
	fds[0].events = POLLIN;

	while(1)
	{
		poll(fds, nfds, 0);
		for(i = 0; i < nfds; ++i)
		{
			if(fds[i].revents & POLLIN)
			{
				if(fds[i].fd == socketTCP)
				{
					newSocket = accept(socketTCP, 0, 0);
					fds[nfds].fd = newSocket;
					fds[nfds].events = POLLIN;
					set_nonblock(fds[nfds].fd);
					++nfds;
				}
				else
				{
					sizeRead = recv(fds[i].fd, mes.client.some_text, BUFFER_SIZE, MSG_NOSIGNAL);
					mes.client.some_text[sizeRead] = '\0';

					switch(sizeRead)
					{
						case 0:
							shutdown(fds[i].fd, SHUT_RDWR);
							close(fds[i].fd);
							break;

						default:
							mes.client.fd_client = fds[i].fd;
				
							if((msgsnd(queue1, (void*)&mes, sizeof(mes.client), 0)) == -1)
								errorMessage("msgsnd serverTCP");
							break;
					}
				}
			}
		}
	}

}
void* thread_1()
{
	int queue1, queue2;

	key_t key1, key2;

	key1 = ftok(".", 'a');
	key2 = ftok(".", 'b');

	struct message mes2;
	mes2.id_message = 1;

	if((queue1 = msgget(key1, 0666 | IPC_CREAT)) == -1)
		errorMessage("thread_1 msgget1");

	if((queue2 = msgget(key2, 0666 | IPC_CREAT)) == -1)
		errorMessage("thread_1 msgget2");

	while(1)
	{
		msgrcv(queue1, (void*)&mes2, sizeof(mes2.client), 0, 0);

		if(strncmp(mes2.client.some_text, "data", 4) == 0)
		{
			strcpy(mes2.client.some_text, "Month april");
		}
		else if(strncmp(mes2.client.some_text, "info", 4) == 0)
		{
			infoComputer(mes2.client.some_text);
		}
		else if(strncmp(mes2.client.some_text, "end", 3) == 0)
		{
			break;
		}
		else
		{
			strcpy(mes2.client.some_text, "Incorrect request. Try again");
		}

		if((msgsnd(queue2, (void*)&mes2, sizeof(mes2.client), 0)) == -1)
			errorMessage("msgsnd thread1");
	}

	msgctl(queue2, IPC_RMID, 0);

	pthread_exit(NULL);
}

void* thread_2()
{
	int queue2;
	key_t key2;

	key2 = ftok(".", 'b');

	if((queue2 = msgget(key2, 0666 | IPC_CREAT)) == -1)
		errorMessage("msgget thread_2");

	struct message mes;

	while(1)
	{
		if((msgrcv(queue2, (void*)&mes, sizeof(mes.client), 0, 0)) == -1)
			errorMessage("msgrcv thread_2");

		send(mes.client.fd_client, mes.client.some_text, BUFSIZ, MSG_NOSIGNAL);
	}
}
void infoComputer(char* str)
{
	char hostName[80];

	gethostname(hostName, 80);
	sprintf(str, "Hostname computer is %s", hostName);
}

void errorMessage(char* str)
{
	perror(str);
	exit(0);
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