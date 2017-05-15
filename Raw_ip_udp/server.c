#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define SIZE_BUF 512
int main()
{
	int fd;
	int sizeSin = 0;
	int bytes_read = 0;
	int bytes_send = 0;

	char buffer[SIZE_BUF];
	char buffer1[SIZE_BUF];

	struct sockaddr_in sin;

	fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(fd == -1){
		perror("Socket");
		exit(EXIT_FAILURE);
	}

	sin.sin_family = PF_INET;
	sin.sin_port = htons(12345);
	sin.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(fd, (struct sockaddr*)&sin, sizeof(sin)) == -1){
		perror("binding");
		exit(EXIT_FAILURE);
	}

	sizeSin = sizeof(sin);

	strcpy(buffer1, "Message from server");

	while(1)
	{
		bytes_read = recvfrom(fd, buffer, 4096, MSG_NOSIGNAL, (struct sockaddr*)&sin, &sizeSin);

		if(bytes_read == 0)
			break;

		printf("Message client: %s\n", buffer);

		bytes_send = sendto(fd, buffer1, 4096, MSG_NOSIGNAL, (struct sockaddr*)&sin, sizeSin);

		if(bytes_send == -1){
			perror("Sendto");
			exit(EXIT_FAILURE);
		}

	}
	return 0;
}
