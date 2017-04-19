#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BUFFER_SIZE 1024

void errorMessage(char* str);

typedef struct ipheader {
 unsigned char      iph_ihl:4; 
 unsigned char      iph_ver:4;
 unsigned char      iph_tos;
 unsigned short int iph_len;
 unsigned short int iph_ident;
 unsigned char      iph_flag;
 unsigned short int iph_offset;
 unsigned char      iph_ttl;
 unsigned char      iph_protocol;
 unsigned short int iph_chksum;
 unsigned int       iph_sourceip;
 unsigned int       iph_destip;
} ip_header;

typedef struct udpheader {
unsigned short int udph_srcport;
unsigned short int udph_destport;
unsigned short int udph_len;
unsigned short int udph_chksum;
} udp_header;

unsigned short csum(unsigned short* buf, int nwords)
{
	unsigned long sum;

	for(sum = 0; nwords > 0; --nwords)
		sum += *buf++;

	sum = (sum >> 16) + (sum & 0xffff);
	sum += (sum >> 16);

	return (unsigned short)(~sum);
}

int main(void)
{
	int raw_socket_udp;
	int fromlen;
	int i;
	
	char buffer[BUFFER_SIZE];

	srand(time(NULL));

	ip_header* ip = (ip_header*) buffer;
	udp_header* udp = (udp_header*)(buffer + sizeof(ip_header));

	struct sockaddr_in message_to;
	int one = 1;
	const int *val = &one;

	memset(buffer, 0, sizeof(buffer));

	if ((raw_socket_udp = socket(AF_INET, SOCK_RAW, IPPROTO_UDP)) < 0)
		errorMessage("socket");

	message_to.sin_family = AF_INET;
	
	message_to.sin_port = htons(12345);

	inet_pton(AF_INET, "127.0.0.1",(struct in_addr *) &(message_to.sin_addr.s_addr));

	ip->iph_ihl = 5;
	ip->iph_ver = 4;
	ip->iph_tos = 16;
	ip->iph_len = sizeof(ip_header) + sizeof(udp_header);
	ip->iph_ident = htons(rand()%65000);
	ip->iph_ttl = 64;
	ip->iph_protocol = IPPROTO_UDP;
	ip->iph_sourceip = htonl(message_to.sin_addr.s_addr);
	ip->iph_destip = htonl(message_to.sin_addr.s_addr);
	ip->iph_chksum = 0;

	udp->udph_srcport = htons(1234);
	udp->udph_destport = htons(12345);

	udp->udph_len = htons(sizeof(udp_header));
	udp->udph_chksum = 0;//csum((unsigned short*)buffer, sizeof(ip_header) + sizeof(udp_header));
	//setsockopt(raw_socket_udp, IPPROTO_IP, IP_HDRINCL, val, sizeof(one));

	memset(buffer, 0, sizeof(buffer));

	strcpy(buffer, "Hello world");

	while(1) 
	{
		if(sendto(raw_socket_udp, buffer, sizeof(buffer), MSG_NOSIGNAL,
			(struct sockaddr*)&message_to, sizeof(message_to)) < 0)
			errorMessage("sendto");

		printf("Send %s\n", buffer);
		sleep(1);
	}
	exit(EXIT_SUCCESS);
}

void errorMessage(char* str)
{
	perror(str);
	exit(EXIT_FAILURE);
}
