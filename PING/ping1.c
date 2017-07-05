#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <arpa/inet.h>
#include <errno.h> 
#include <unistd.h> 
#include <netinet/ip.h> 
#include <netinet/ip_icmp.h> 

#define BUF_SIZE 512

unsigned short in_cksum(unsigned short *addr, int len);
void printPacket( char* buf );

int main( int argc, char** argv ){
	const int on = 1;

	/*if( argc != 3 ){
		printf( "Parameters do not match. Please try again" );
		exit( EXIT_FAILURE );
	}*/

	/*struct hostent* toAddr = malloc(sizeof(struct hostent));
	toAddr = gethostbyname( argv[1] );

	if( toAddr = NULL ){
		printf( "Error gethostbyname." );
		exit( EXIT_FAILURE );
	}*/
	//printf("%s\n", toAddr->h_addr);
	struct ip ip;
	struct icmp icmp;


	//struct sockaddr_in sin;
	u_char *packet;

	packet = (u_char *)malloc(sizeof(struct ip)+sizeof(struct icmp));

	ip.ip_hl = 0x5;
	ip.ip_v = 0x4;
	ip.ip_tos = 0x0;
	ip.ip_len = sizeof(struct ip)+sizeof(struct icmp);
	ip.ip_id = htons(12830);
	ip.ip_off = 0x0;
	ip.ip_ttl = 64;
	ip.ip_p = IPPROTO_ICMP;
	ip.ip_sum = 0x0;
	ip.ip_src.s_addr = inet_addr("192.168.0.255");
	ip.ip_dst.s_addr = inet_addr("5.255.255.70");
	ip.ip_sum = in_cksum((unsigned short* )&ip, 20);
	memcpy(packet, &ip, sizeof(struct ip));

	icmp.icmp_type = 8;//ICMP_ECHO;
	icmp.icmp_code = 0;
	//icmp.icmp_id = 1;
	//icmp.icmp_seq = 1;
	icmp.icmp_cksum = 0;
	icmp.icmp_cksum = in_cksum((unsigned short* )&icmp, sizeof(struct icmp));
	memcpy(packet + sizeof(struct ip), &icmp, sizeof(struct icmp));

	int fd_ICMP;// = 0;
	if ((fd_ICMP = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0) {
		perror("raw socket");
		exit(1);
	}

	if (setsockopt(fd_ICMP, IPPROTO_IP, IP_HDRINCL, &on, sizeof(on)) < 0) {
		perror("setsockopt");
		exit(1);
	}

	struct sockaddr_in server_ICMP;
	struct timeval tv;
	tv.tv_sec = 3;
	tv.tv_usec = 0;
	setsockopt(fd_ICMP, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv,sizeof(struct timeval));
	memset(&server_ICMP, 0, sizeof(struct sockaddr_in));

	server_ICMP.sin_family = PF_INET;
	server_ICMP.sin_addr.s_addr = ip.ip_dst.s_addr;
	
	while(1){
		if (sendto(fd_ICMP, packet, sizeof(struct icmp) + sizeof(struct ip), 0, (struct sockaddr *)&server_ICMP, sizeof(struct sockaddr)) < 0)  {
		perror("sendto");
		exit(1);
		}	
		unsigned char buf[sizeof(struct ip)+sizeof(struct icmphdr)];
	int bytes, len=sizeof(server_ICMP);

	bzero(buf, sizeof(buf));
	bytes = recvfrom(fd_ICMP, buf, sizeof(buf), 0, (struct sockaddr*)&server_ICMP, &len);

	if(bytes > 0){
		printPacket(buf);
	}
    
		sleep(2);
	}

	return 0;
}

void printPacket( char* buf ){
	struct ip *iprecv = ( struct ip* )( buf );
	puts( "\nIP HEADER" );
	printf( "\tIP version: %d\n", iprecv->ip_v );	
	printf( "\tProtocol: %d\n", iprecv->ip_p );
	printf( "\tIdentification: 0x%X\n", ntohs( iprecv->ip_id ));
	printf( "\tHeader len: %i\n", iprecv->ip_hl*4 );
	printf( "\tChecksum: 0x%X\n",ntohs( iprecv->ip_sum ));
	printf( "\tTTL: %d\n", iprecv->ip_ttl );
	printf( "\tSource IP: %s\n", inet_ntoa( iprecv->ip_src ));
	printf( "\tDestination IP: %s\n", inet_ntoa( iprecv->ip_dst ));
	
	struct icmphdr *icmprecv = ( struct icmphdr* )( buf+sizeof( struct ip )/*iprecv->ip_hl*4*/);
	puts( "\nICMP HEADER" );
	printf( "\tType: %i\n", icmprecv->type );
	printf( "\tCode: %d\n", icmprecv->code );
	printf( "\tChecksum: 0x%X\n", ntohs(icmprecv->checksum )); 
	printf( "\tIdentifier: %i\n", icmprecv->un.echo.id ); 
	printf( "Send message\n" );
}

unsigned short in_cksum( unsigned short *addr, int len ) 
{
	int nleft = len;
	int sum = 0;
	unsigned short *w = addr;
	unsigned short answer = 0;

	while (nleft > 1) {
		sum += *w++;
		nleft -= 2;
	}

	if (nleft == 1) {
		*(unsigned char *) (&answer) = *(unsigned char* ) w;
		sum += answer;
	}
	
	sum = (sum >> 16) + (sum & 0xFFFF);
	sum += (sum >> 16);
	answer = ~sum;
	return (answer);
}





