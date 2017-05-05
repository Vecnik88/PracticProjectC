#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>

#define PCKT_LEN 8192
#define DEST "127.0.0.1"

unsigned short csum(unsigned short* buf, int nwords);

typedef struct udp_header
{
    unsigned short int udph_srcport;
    unsigned short int udph_destport;
    unsigned short int udph_len;
    unsigned short int udph_chksum;
} udphdr;

typedef struct ipheader
{
    unsigned char iph_ihl;
    unsigned char iph_ver;
    unsigned char iph_tos;
    unsigned short int iph_len;
    unsigned short int iph_ident;
    unsigned char iph_flag;
    unsigned short int iph_offset;
    unsigned char iph_ttl;
    unsigned char iph_protocol;
    unsigned short int iph_chksum;
    unsigned int iph_sourceip;
    unsigned int iph_destrip;
} ip_header;

int main(int argc, char const *argv[])
{
	int socketUDP, i = 0;

	char buffer[PCKT_LEN];
	char buffer1[PCKT_LEN];

	memset(buffer, 0, PCKT_LEN);
	memset(buffer1, 0, PCKT_LEN);

	ip_header* ip = (ip_header*) buffer;
	udphdr* udp = (udphdr*)(buffer + sizeof(ip_header));

	strcpy(buffer, "Message client from buffer. This message to udp server. AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
	strcpy(buffer1, "Message client from buffer 1. This message to udp server. AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
	
	socketUDP = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);

	struct sockaddr_in clientUDP;
	
	clientUDP.sin_family = AF_INET;
	clientUDP.sin_port = htons(12345);
	inet_pton(AF_INET, DEST, (struct in_addr *)&clientUDP.sin_addr.s_addr);

	udp->udph_srcport = htons(1234);
	udp->udph_destport = htons(12345);
	udp->udph_len = htons(sizeof(udp));
	udp->udph_chksum = csum(buffer, (sizeof(buffer)+sizeof(ip)+sizeof(udp)));

	int size_clientUDP = sizeof(clientUDP);

	while(1)
	{
		sendto(socketUDP,(char*) buffer, 40, MSG_NOSIGNAL, (struct sockaddr*)&clientUDP, size_clientUDP);

		recvfrom(socketUDP, buffer1, PCKT_LEN, MSG_NOSIGNAL, (struct sockaddr*)&clientUDP, &size_clientUDP);

		printf("%s\n", buffer1);
		for(i = (sizeof(udphdr)+sizeof(ip_header)); i < sizeof(buffer); ++i)
		{
			printf("%c", buffer[i]);
		}
		sleep(2);
	}
	return 0;
}

unsigned short csum(unsigned short* buf, int nwords)
{
	unsigned long sum;

	for(sum = 0; nwords > 0; --nwords)
		sum += *buf++;

	sum = (sum >> 16) + (sum & 0xffff);
	sum += (sum >> 16);

	return (unsigned short)(~sum);
}


