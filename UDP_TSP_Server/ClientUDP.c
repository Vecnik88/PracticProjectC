// Client UDP

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>

#define BUFSIZE 1024

int main()
{
	int socketUDP;
	char buffer[BUFSIZE];
	int sizeClientUdp;
	int sizeMessageUDP;

	socketUDP = socket(PF_INET, SOCK_DGRAM, 0);

	struct sockaddr_in clientUDP;

	clientUDP.sin_family = PF_INET;
	clientUDP.sin_port = htons(1234);
	clientUDP.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

	sizeClientUdp = sizeof(clientUDP);

	while(1)
	{
		printf("Enter your message UDP: ");
		fgets(buffer, BUFSIZE, stdin);
		sendto(socketUDP, buffer, BUFSIZE, MSG_NOSIGNAL, 
				(struct sockaddr*)&clientUDP, sizeClientUdp);

		sizeMessageUDP = recvfrom(socketUDP, buffer, BUFSIZE, MSG_NOSIGNAL,
								(struct sockaddr*)&clientUDP, &sizeClientUdp);

		buffer[sizeMessageUDP] = '\0';
		if(strncmp(buffer, "end", 3) == 0)
			break;

		printf("%s\n", buffer);
	}

	close(socketUDP);
	exit(0);
}
