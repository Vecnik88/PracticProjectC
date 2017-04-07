#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/epoll.h>
#include <ncurses.h>

#define BUFSIZE 1024

int set_nonblock(int fd);
void* msgRead();
void* msgSend();
void deleteClearWindow(WINDOW* wnd);

int clientSocket;
pthread_spinlock_t trySpinLock;

int main(int argc, char const *argv[])
{
	initscr();                                    
    curs_set(FALSE);
    start_color();

    init_pair(1, COLOR_WHITE, COLOR_GREEN);
    wbkgd(stdscr, COLOR_PAIR(1));
    move(1, 5);
	printw("Chat room.");
	refresh();

	clientSocket = socket(PF_INET, SOCK_STREAM, 0);

	struct sockaddr_in client;
	client.sin_family = PF_INET;
	client.sin_port = htons(12345);
	client.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

	connect(clientSocket, (struct sockaddr*)&client, sizeof(client));

	pthread_t thread1;
	pthread_t thread2;
	
	pthread_spin_init(&trySpinLock, 0);

	pthread_create(&thread1, NULL, msgSend, NULL);
	pthread_create(&thread2, NULL, msgRead, NULL);

	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);

	pthread_spin_destroy(&trySpinLock);
	endwin();

	pthread_exit(0);
}

void* msgSend()
{
	char buffer[BUFSIZE];

	WINDOW* wndSnd = newwin(10, 50, 40, 40);
	wbkgd(wndSnd, COLOR_PAIR(0));
	wrefresh(wndSnd);

	while(1)
	{
		mvwprintw(wndSnd, 1, 5, "Your message: ");
		wmove(wndSnd, 3, 5);
		wgetnstr(wndSnd, buffer, BUFSIZE);

		pthread_spin_trylock(&trySpinLock);
		send(clientSocket, buffer, BUFSIZE, MSG_NOSIGNAL);
		pthread_spin_unlock(&trySpinLock);

		if(strncmp(buffer, "end", 3) == 0)
			break;
		wclear(wndSnd);
	}

	deleteClearWindow(wndSnd);
	pthread_exit(NULL);
}

void* msgRead()
{
	int i = 1;
	WINDOW* wndRnd = newwin(30, 50, 5, 40);
	wbkgd(wndRnd, COLOR_PAIR(0));
	wrefresh(wndRnd);

	int RecvSize;
	char buffer[BUFSIZE];

	int Epoll = epoll_create1(0);
	struct epoll_event Event;
	Event.data.fd = clientSocket;
	Event.events = EPOLLIN;
	epoll_ctl(Epoll, EPOLL_CTL_ADD, clientSocket, &Event);

	while(1)
	{
		epoll_wait(Epoll, &Event, 1, -1);

		pthread_spin_trylock(&trySpinLock);
		RecvSize = recv(Event.data.fd, buffer, BUFSIZE, MSG_NOSIGNAL);
		pthread_spin_unlock(&trySpinLock);

		if((RecvSize == 0) && (errno != EAGAIN))
		{
			shutdown(Event.data.fd, SHUT_RDWR);
			close(Event.data.fd);
			break;
		}
		else if(RecvSize > 0)
			{
				buffer[RecvSize] = '\0';
				wmove(wndRnd, i++, 4);
				wprintw(wndRnd, "%s\n", buffer);
				wrefresh(wndRnd);

				if(strncmp(buffer, "end", 3) == 0)
					break;
				if(i == 27)
				{
					wclear(wndRnd);
					i = 1;
				}
			}
	}

	deleteClearWindow(wndRnd);
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

void deleteClearWindow(WINDOW* wnd)
{
	wclear(wnd);
	wbkgd(wnd, COLOR_PAIR(1));
	wrefresh(wnd);
	delwin(wnd);
}