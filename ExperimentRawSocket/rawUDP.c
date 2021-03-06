#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <string.h>
#include <time.h>

struct ipheader
{
unsigned char      iph_ihl:5, iph_ver:4;
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
};

struct udpheader 
{
unsigned short int udph_srcport;
unsigned short int udph_destport;
unsigned short int udph_len;
unsigned short int udph_chksum;
};

unsigned short csum(unsigned short *buf, int nwords)
{  
    unsigned long sum;

    for(sum=0; nwords>0; nwords--)
		sum += *buf++;

    sum = (sum >> 16) + (sum &0xffff);
    sum += (sum >> 16);

    return (unsigned short)(~sum);
}

int main()
{
	int raw_socket;
	char datagram[512];

	raw_socket = socket(PF_INET, SOCK_RAW, IPPROTO_UDP);

	struct ipheader* ip = (struct ipheader*) datagram;
	struct udpheader* udp = (struct udpheader*) (datagram+sizeof(struct udpheader));

	memset(datagram, 'A', 512);

	struct sockaddr_in raw_client;

	raw_client.sin_family = PF_INET;
	raw_client.sin_port = htons(12345);
	raw_client.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

	udp->udph_srcport = htons(12345);
	udp->udph_destport = htons(1234);
	udp->udph_len = htons(sizeof(struct udpheader));
	udp->udph_chksum = csum((unsigned short*)datagram, sizeof(struct udpheader));

	int i = 0;
	while(1)
	{
		sendto(raw_socket, (char*)datagram, ip->iph_len, MSG_NOSIGNAL, (struct sockaddr*)&raw_client, sizeof(raw_client));
		printf("Send message\n");
		sleep(2);
	}

	return 0;
}
