#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
	int fd, i;
	char buffer[4096];

	fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

	struct sockaddr_in sin;

	sin.sin_family = PF_INET;
	sin.sin_port = htons(12345);
	sin.sin_addr.s_addr = htonl(INADDR_ANY);

	bind(fd, (struct sockaddr*)&sin, sizeof(sin));

	int sizeSin = sizeof(sin);
	printf("Server on\n");

	while(1)
	{
		recvfrom(fd, buffer, 4096, MSG_NOSIGNAL, (struct sockaddr*)&sin, &sizeSin);
		printf("recv\n");

		sleep(1);
	}
	return 0;
}


