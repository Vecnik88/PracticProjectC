#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#define SIZE_BUFFER 1024

void perrorMessage(char* str);

int main(int argc, char const *argv[])
{
	int socket_udp;
	int size_server_udp, size_client_udp;
	int sizeRead;

	char buffer[SIZE_BUFFER] = "message buffer";
	char buffer1[SIZE_BUFFER] = "message buffer 1";

	struct sockaddr_in server_udp;

	if((socket_udp = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
		perrorMessage("socket_udp");

	
	server_udp.sin_family = PF_INET;
	server_udp.sin_port = htons(12345);
	server_udp.sin_addr.s_addr = htonl(INADDR_ANY);

	size_server_udp = sizeof(server_udp);

	if(bind(socket_udp, (struct sockaddr*) &server_udp, size_server_udp) < 0)
		perrorMessage("binding");

	//strcpy(buffer, "message buffer");
	//strcpy(buffer, "message buffer 1");
	
	while(1)
	{
		printf("%s\n", buffer);
		printf("%s\n", buffer1);
		
		sizeRead = recvfrom(socket_udp, buffer1, SIZE_BUFFER, MSG_NOSIGNAL,
							(struct sockaddr*) &server_udp, &size_server_udp);

		buffer1[sizeRead] = '\0';

		printf("%s\n", buffer1);

		sendto(socket_udp, buffer, SIZE_BUFFER, MSG_NOSIGNAL,
				(struct sockaddr*) &server_udp, size_server_udp);
	}

	return 0;
}

void perrorMessage(char* str)
{
	perror(str);
	exit(0);
}


