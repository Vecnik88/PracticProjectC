#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

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

	sum = (sum >> 16) + (sum &0xffff);
	sum += (sum >> 16);

	return (unsigned short)(~sum);
}

int main(void)
{
	int raw_socket_udp;
	int fromlen;
	int i;
	
	char buffer[BUFFER_SIZE];
	char packet[50];

	ip_header* ip = (ip_header*) buffer;
	udp_header* udp = (udp_header*)(buffer + sizeof(ip_header));

	struct sockaddr_in message_to, message_from;
	int one = 1;
	const int *val = &one;

	memset(buffer, 0, sizeof(buffer));

	if ((raw_socket_udp = socket(AF_INET, SOCK_RAW, IPPROTO_UDP)) < 0)
		errorMessage("socket");

	message_to.sin_family = AF_INET;
	message_from.sin_family = AF_INET;

	message_to.sin_port = htons(12345);
	message_from.sin_port = htons(1234);

	inet_pton(AF_INET, "127.0.0.1", &(message_to.sin_addr));
	inet_pton(AF_INET, "127.0.0.1", &(message_from.sin_addr));

	ip->iph_ihl = 5;
	ip->iph_ver = 4;
	ip->iph_tos = 16;
	ip->iph_len = sizeof(ip_header) + sizeof(udp_header);
	ip->iph_ident = htons(54321);
	ip->iph_ttl = 64;
	ip->iph_protocol = 17;
	ip->iph_sourceip = message_to.sin_addr.s_addr;
	ip->iph_destip = message_from.sin_addr.s_addr;

	udp->udph_srcport = htons(1234);
	udp->udph_destport = htons(12345);

	udp->udph_len = htons(sizeof(struct udpheader));
	udp->udph_chksum = csum((unsigned short*)buffer, sizeof(ip_header) + sizeof(udp_header));
	setsockopt(raw_socket_udp, IPPROTO_IP, IP_HDRINCL, val, sizeof(one));


	memset(packet, 0, sizeof(packet));
	memset(buffer, 0, sizeof(buffer));

	fromlen = sizeof(message_from);

	while(1) 
	{
		printf("1\n");

		if(sendto(raw_socket_udp, buffer, sizeof(buffer), MSG_NOSIGNAL,
			(struct sockaddr*)&message_to, sizeof(message_to)) < 0)
			errorMessage("sendto");

		printf("2\n");

		if (recvfrom(raw_socket_udp, (char *)&packet, sizeof(packet), 0,
			(struct sockaddr *)&message_from, &fromlen) < 0)
			perror("packet receive error:");

		i = sizeof(struct iphdr);
		printf("3\n");

		while (i < sizeof(packet)) 
		{
			fprintf(stdout, "%c", packet[i]);
			i++;
		}
		printf("\n");
	}
	exit(EXIT_SUCCESS);
}

void errorMessage(char* str)
{
	perror(str);
	exit(EXIT_FAILURE);
}